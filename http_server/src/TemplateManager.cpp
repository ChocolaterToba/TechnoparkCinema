#include <string>
#include <map>
#include <fstream>
#include <streambuf>
#include <queue>
#include <iostream>
#include <cstring>
#include <set>

#include "exceptions.hpp"
#include "TemplateManager.hpp"

typedef enum {
    FREE,
    TEMPLATE_FOUND,
    CYCLE_STARTED,
    END
} state_t;

void TemplateManager::ExtractParameters() {
    size_t begin = 0;
    while ((begin = HTML.find("{[")) != std::string::npos) {
        size_t end = HTML.find("]}", begin);

        std::string parameters = HTML.substr(begin + 2, end - begin - 2);
        HTML.erase(begin - 2, end - begin + 2);

        begin = 0;
        while ((end = parameters.find(',', begin)) != std::string::npos) {
            html_parameters.insert(parameters.substr(begin, end - begin));
            begin = end;
            while (std::isspace(parameters[++begin]));
        }

        html_parameters.insert(parameters.substr(begin, parameters.length() - begin));
    }
}

void TemplateManager::InsertTemplates() {
    size_t pos = 0;
    state_t state = FREE;

    while (state != END) {
        switch (state) {
            case FREE:
                pos = HTML.find("{%", pos);
                if (pos == std::string::npos) {
                    state = END;
                } else if (HTML.substr(pos + 2, strlen("FOR")) != "FOR" &&
                           HTML.substr(pos + 2, strlen("ENDFOR")) != "ENDFOR") {
                    state = TEMPLATE_FOUND;
                } else {
                    ++pos;
                }
                break;

            case TEMPLATE_FOUND: {
                size_t begin = pos;
                size_t end = HTML.find("%}", pos);

                ++pos;
                while (std::isspace(HTML[++pos]));

                size_t name_begin = pos;
                while (!std::isspace(HTML[pos]) && pos < end - 1) {
                    ++pos;
                }
                size_t name_end = pos;

                std::string template_name = HTML.substr(name_begin, name_end - name_begin);

                std::string path = "../static/templates/" + template_name + ".html";
                std::ifstream source(path, std::ios::binary);
                std::string template_doc((std::istreambuf_iterator<char>(source)), std::istreambuf_iterator<char>());

                HTML.erase(begin, end - begin + 2);
                HTML.insert(begin, template_doc);

                pos = 0;
                state = FREE;
                break;
            }

            default:
                break;
        }
    }
}

TemplateManager::TemplateManager(const std::string &url) {
    std::string path("../static");

    if (url.find("watch") != std::string::npos) {
        path = "../static/watch.html";
        if (url.find('?') != std::string::npos) {
            ID = url.substr(url.find('?')+1);
        }
    } else if (url == "/") {
        path  += url  +  "Index.html";
    } else {
        path += url;
    }

    std::ifstream source(path, std::ios::binary);
    HTML = std::string((std::istreambuf_iterator<char>(source)), std::istreambuf_iterator<char>());

    InsertTemplates();
    ExtractParameters();
}

void TemplateManager::EvaluateCycles(const std::map<std::string, std::string> &parameters) {
    state_t state = FREE;
    size_t pos = 0;
    size_t endfor_pos;
    while (state != END) {
        switch (state) {
            case FREE:
                endfor_pos = HTML.find("{%ENDFOR%}", pos);
                if (endfor_pos == std::string::npos) {
                    state = END;
                } else {
                    pos = HTML.rfind("{%FOR", endfor_pos);
                    state = CYCLE_STARTED;
                }
                break;

            case CYCLE_STARTED: {
                size_t counter_begin = HTML.find("TO ", pos) + strlen("TO ");
                size_t counter_end = counter_begin;
                while (!std::isspace(HTML[counter_end + 1]) && HTML[counter_end + 1] != '%') {
                    ++counter_end;
                }
                std::string parameter_name = HTML.substr(counter_begin, counter_end - counter_begin + 1);

                std::string total_cycle_replacement;

                size_t cycle_counter = std::stoi(parameters.at(parameter_name));
                size_t body_begin = HTML.find("}", pos) + 1;
                size_t body_end = endfor_pos - 1;
                for (size_t i = 0; i < cycle_counter; ++i) {
                    std::string cycle_body = HTML.substr(body_begin, body_end - body_begin);

                    size_t param_begin;
                    size_t body_pos = 0;
                    while ((param_begin = cycle_body.find("{#" + parameter_name + "#}", body_pos)) != std::string::npos) {
                        size_t param_end = param_begin + ("{#" + parameter_name + "#}").length();

                        cycle_body.erase(param_begin, param_end - param_begin);
                        cycle_body.insert(param_begin, parameters.at(parameter_name + std::to_string(i)));

                        body_pos = param_end;
                    }

                    total_cycle_replacement.append(cycle_body);
                }

                HTML.erase(pos, endfor_pos + strlen("{%ENDFOR%}") - pos);
                HTML.insert(pos, total_cycle_replacement);

                state = FREE;
                pos = 0;
                break;
            }

            default:
                break;
        }
    }
}

void TemplateManager::EvaluateParameters(const std::map<std::string, std::string> &parameters) {
    size_t begin = 0;
    while ((begin = HTML.find("{{", begin)) != std::string::npos) {
        size_t begin_pos = begin + 1;
        while (std::isspace(HTML[++begin_pos]));

        size_t end_pos = begin_pos;
        while (!std::isspace(HTML[end_pos]) && HTML[end_pos] != '}') {
            ++end_pos;
        }
        std::string substr = HTML.substr(begin_pos, end_pos - begin_pos);
        std::string replacement = parameters.at(substr);
        size_t end = HTML.find("}}", begin);

        HTML.erase(begin, end - begin + 2);
        HTML.insert(begin, replacement);
    }
}

std::vector<char> TemplateManager::GetHtmlFinal(const std::map<std::string, std::string> &parameters) {
    EvaluateCycles(parameters);
    EvaluateParameters(parameters);

    return std::vector<char>(HTML.begin(), HTML.end());
}

std::set<std::string> TemplateManager::GetParameterNames() {
    return  html_parameters;
}
