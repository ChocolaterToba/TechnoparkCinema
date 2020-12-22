#include <string>
#include <map>
#include <fstream>
#include <streambuf>
#include <queue>

#include <cstring>

#include "exceptions.hpp"
#include "TemplateManager.hpp"

using std::string;
using std::vector;
using std::queue;



typedef enum {
    FREE,
    TEMPLATE_FOUND,
    END
} state_t;

void TemplateManager::ExtractParameters() {
    size_t begin = HTML.find("{[") + 2;
    size_t end = HTML.find("]}", begin) - 1;

    string parameters = HTML.substr(begin, end);

    begin = 0;
    while ((end = parameters.find(',', begin)) != string::npos) {
        html_parameters.push(parameters.substr(begin, end - begin));
        begin = end;
        while (std::isspace(parameters[++begin]));
    }

    html_parameters.push(parameters.substr(begin, parameters.length() - begin));
}

void TemplateManager::InsertTemplates() {
    size_t pos = 0;
    state_t state = FREE;
    pos = 0;

    while (state != END) {
        switch (state) {
            case FREE:
                pos = HTML.find("{%", pos);
                if (pos == string::npos)
                    state = END;
                else if (HTML.substr(pos + 2, pos + 3) != "FOR" &&
                         HTML.substr(pos + 2, pos + 6) != "ENDFOR")
                    state = TEMPLATE_FOUND;
                break;
            case TEMPLATE_FOUND: {
                size_t begin = pos;
                size_t end = HTML.find("%}", pos);

                pos += 2;
                while (std::isspace(HTML[++pos]));

                size_t name_begin = pos;
                while (!std::isspace(HTML[pos]) && pos < end - 1)
                    ++pos;
                size_t name_end = pos;

                string template_name = HTML.substr(name_begin, name_end);

                string path = "../static/templates/" + template_name + ".html";
                std::ifstream source(path, std::ios::binary);
                string template_doc((std::istreambuf_iterator<char>(source)), std::istreambuf_iterator<char>());

                HTML.erase(begin, end + 2);
                HTML.insert(begin, template_doc);

                pos = begin;
                state = FREE;
                break;
            }
            case END:
                break;
        }
    }
}

TemplateManager::TemplateManager(const string &url) {
    string path("../static");

    if (url.find("video") != string::npos)
        path = "../static/templates/Video.html";
    else if (url == "/")
        path  += url  +  "IndexTemplate.html";  // "index.html"
    // check url type

    std::ifstream source(path, std::ios::binary);
    HTML = string((std::istreambuf_iterator<char>(source)), std::istreambuf_iterator<char>());

    ExtractParameters();
    InsertTemplates();
}

void TemplateManager::FixCycles(string &page, size_t l, size_t r, std::map<string, string> parameters) {
    while ((l = page.find("{%FOR", l)) && (r = page.find("{%ENDFOR%}", r))
            && (r != string::npos) && (l != string::npos)) {
        l = page.find("{%FOR", l + 1);
        if ((l != string::npos) && (l < r)) {
            l = page.find("%}") + 2;
            FixCycles(page, l, r, parameters);
        } else {
            l = page.rfind("{%FOR", r)  +  6;
            string html_template = page.substr(l, page.rfind(" TO", r) - l);
            int start = std::stoi(html_template);
            // add 0 processing
            l = page.rfind(" TO ", r)  +  6;
            html_template = page.substr(l, page.rfind(" %}", r) - l - 2);
            int end;
            if (!parameters[html_template].empty()) {
                end = std::stoi(parameters[html_template]);
            } else {
                l = page.rfind(" TO ", r)  +  4;
                html_template = page.substr(l, page.rfind(" %}", r) - l);
                end = std::stoi(html_template);
            }
            l = page.rfind("%}", r)  +  2;
            html_template = page.substr(l, r - l);
            l = page.rfind("{%FOR", r);
            page.erase(l, r - l  +  10);
            for (int i = start; i < end; i++ ) {
                page.insert(l, html_template);
            }
            l = 0;
            r = 0;
            FixCycles(page, l, r, parameters);
        }
    }
}


vector<char> TemplateManager::GetHtmlFixed(std::map<string, string> parameters, string url) {
    string path("../static");
    if (url == "/")
        path  += url  +  "IndexTemplate.html";  // "index.html"

    // check url type
    if (url.find("video") != string::npos) {
        path = "../static/templates/Video.html";
    }
    // load template based on url type
    std::ifstream source(path, std::ios::binary);
    string HtmlDoc((std::istreambuf_iterator<char>(source)),
                 std::istreambuf_iterator<char>());
    // replace all {%%} with actual text, keep {{}} and {%FOR%}{%ENDFOR%}
    size_t lbracket = 0;
    size_t rbracket = 0;
    InsertTemplates(HtmlDoc, lbracket, rbracket);
    lbracket = 0;
    rbracket = 0;
    FixCycles(HtmlDoc, lbracket, rbracket, parameters);
    lbracket = 0;
    rbracket = 0;
    HTML = HtmlDoc;
    while ((lbracket = HtmlDoc.find("{{", lbracket)) && (rbracket = HtmlDoc.find("}}", rbracket))
            && (rbracket != string::npos) && (lbracket != string::npos)) {
        string parameter = HtmlDoc.substr(lbracket + 2, rbracket-lbracket-2);
        HtmlDoc.erase(lbracket, rbracket + 2-lbracket);
        HtmlDoc.insert(lbracket, parameters[parameter]);
        lbracket = 0;
        rbracket = 0;
    }
    vector<char> test (HtmlDoc.begin(), HtmlDoc.end());
    return test;
}

queue<string> TemplateManager::GetParameterNames() {
    return  html_parameters;
}
