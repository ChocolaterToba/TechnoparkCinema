#include <string>
#include <map>
#include <fstream>
#include <streambuf>
#include <queue>
#include "exceptions.hpp"
#include "TemplateManager.hpp"

using std::string;
using std::vector;
using std::queue;


TemplateManager::TemplateManager(const string &url) {
    string path("../static");

    if (url.find("video") != string::npos)
        path = "../static/templates/Video.html";
    else if (url == "/")
        path  += url  +  "IndexTemplate.html";  // "index.html"
    // check url type


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
    while ((lbracket = HtmlDoc.find("{{", lbracket)) && (rbracket = HtmlDoc.find("}}", rbracket))
            && (rbracket != string::npos) && (lbracket != string::npos)) {
        string parameter = HtmlDoc.substr(lbracket + 2, rbracket - lbracket - 2);
        html_parameters.push(parameter);
        lbracket = rbracket + 2;
        rbracket = rbracket + 2;
    }
    HTML = HtmlDoc;
}

void TemplateManager::InsertTemplates(string &page, size_t l, size_t r) {
    while ((l = page.find("{%", l)) && (r = page.find("%}", r))
            && (r != string::npos) && (l != string::npos)) {
        string html_template = page.substr(l + 2, r - l - 2);
        l = html_template.find("FOR");
        if ((l != string::npos)) {
            l = r  +  2;
            r  += 2;
            InsertTemplates(page, l, r);
        } else {
            string path("../static/templates/" + html_template + ".html");
            std::ifstream source(path, std::ios::binary);
            string HtmlDoc((std::istreambuf_iterator<char>(source)),
                std::istreambuf_iterator<char>());
            l = r - html_template.length() - 2;
            page.erase(l, r-l + 2);
            page.insert(l, HtmlDoc);
        }
        l = r + 2;
        r = r + 2;
    }
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


vector<char> TemplateManager::GetHtmlFixed(std::map<string, string> parameters,
                                                 string url) {
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

// парсинг вынести в 2 отдельные функции. Шаблоны рекурсивно.
TemplateManagerOld::TemplateManagerOld(std::ifstream &source) {
    string HtmlDoc((std::istreambuf_iterator<char>(source)),
                   std::istreambuf_iterator<char>());
    // заменить на лямбда функции поиска от позиции и возвращать string
    // и для {%%} чтобы разпарсить весь документ,
    // в результате получить набор параметров
    size_t lbracket = 0;
    size_t rbracket = 0;
    lbracket = HtmlDoc.find("{{", lbracket);
    rbracket = HtmlDoc.find("}}", rbracket);
    while ((lbracket = HtmlDoc.find("{{", lbracket)) && (rbracket = HtmlDoc.find("}}", rbracket))
           && (rbracket != string::npos) && (lbracket != string::npos)) {
        string parameter = HtmlDoc.substr(lbracket + 2, rbracket-lbracket-2);
        html_parameters.push(parameter);
        lbracket = rbracket + 2;
        rbracket = rbracket + 2;
    }
    // временная заглушка, поскольку может быть {{параметр}} внутри макроса {%%}
    // временно будем подставлять готовый "случайный" код из набора
    // нужно парсить {%for:N:{{parameter}}} т.е. N раз читаем parameter
    // и параметры должны быть разные (а-ля ID фильмов)
    lbracket = 0;
    rbracket = 0;
    while ((lbracket = HtmlDoc.find("{%", lbracket)) && (rbracket = HtmlDoc.find("%}", rbracket))
           && (rbracket != string::npos) && (lbracket != string::npos)) {
        string html_template = HtmlDoc.substr(lbracket + 2, rbracket-lbracket-2);
        html_templates.push(html_template);
        lbracket = rbracket + 2;
        rbracket = rbracket + 2;
    }
    HTML = HtmlDoc;
}
//  queue будет заменена на map для хранения параметров
queue<string> TemplateManagerOld::GetParameterNames() {
    return  html_parameters;
}
queue<string> TemplateManagerOld::GetTemplateNames() {
    return html_templates;
}

vector<char> TemplateManagerOld::GetHtml(queue<string> &parameters, queue<string> &templates) {
    size_t lbracket = 0;
    size_t rbracket = 0;
    while ((lbracket = HTML.find("{{")) && (rbracket = HTML.find("}}"))
           && (rbracket != string::npos) && (lbracket != string::npos)) {
        string parameter = parameters.front();
        parameters.pop();
        HTML.erase(lbracket, rbracket + 2-lbracket);
        HTML.insert(lbracket, parameter);
        // HTML.append(parameter, lbracket, parameter.length());
    }
    // временная заглушка, поскольку может быть {{параметр}} внутри макроса {%%}
    // временно будем подставлять готовый "случайный" код из набора
    // нужно парсить {%for:N:{{parameter}}} т.е. N раз читаем parameter
    // и параметры должны быть разные (а-ля ID фильмов)
    lbracket = 0;
    rbracket = 0;
    while ((lbracket = HTML.find("{%")) && (rbracket = HTML.find("%}"))
           && (rbracket != string::npos) && (lbracket != string::npos)) {
        string html_template = templates.front();
        templates.pop();
        HTML.erase(lbracket, rbracket + 2-lbracket);
        HTML.insert(lbracket, html_template);
        // HTML.append(html_template, lbracket, html_template.length());
    }
    return vector<char> (HTML.begin(), HTML.end());
}


vector<char> TemplateManagerOld::GetHtmlFixed(queue<string> &parameters, queue<string> &templates, string url) {
    size_t lbracket = 0;
    size_t rbracket = 0;

    string path("../static"  +  url);
    string path_templates("../static/templates/");
    while ((lbracket = HTML.find("{%")) && (rbracket = HTML.find("%}"))
           && (rbracket != string::npos) && (lbracket != string::npos)) {
        string html_template = templates.front();
        templates.pop();
        HTML.erase(lbracket, rbracket + 2-lbracket);
        HTML.insert(lbracket, html_template);
        // HTML.append(html_template, lbracket, html_template.length());
    }

    return vector<char> (HTML.begin(), HTML.end());
}

