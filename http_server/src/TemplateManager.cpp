#include <string>
#include <map>
#include <fstream>
#include <streambuf>
#include <queue>
#include "exceptions.hpp"
#include "TemplateManager.hpp"

  // парсинг вынести в 2 отдельные функции. Шаблоны рекурсивно.
TemplateManagerOld::TemplateManagerOld(std::ifstream &source) {
    std::string HtmlDoc((std::istreambuf_iterator<char>(source)),
                 std::istreambuf_iterator<char>());
    // заменить на лямбда функции поиска от позиции и возвращать std::string
    // и для {%%} чтобы разпарсить весь документ,
    // в результате получить набор параметров
    size_t lbracket = 0;
    size_t rbracket = 0;
    lbracket = HtmlDoc.find("{{", lbracket);
    rbracket = HtmlDoc.find("}}", rbracket);
    while ((lbracket = HtmlDoc.find("{{", lbracket)) && (rbracket = HtmlDoc.find("}}", rbracket))
            && (rbracket != std::string::npos) && (lbracket != std::string::npos)) {
        std::string parameter = HtmlDoc.substr(lbracket+2, rbracket-lbracket-2);
        htmlparameters.push(parameter);
        lbracket = rbracket+2;
        rbracket = rbracket+2;
    }
    // временная заглушка, поскольку может быть {{параметр}} внутри макроса {%%}
    // временно будем подставлять готовый "случайный" код из набора
    // нужно парсить {%for:N:{{parameter}}} т.е. N раз читаем parameter
    // и параметры должны быть разные (а-ля ID фильмов)
    lbracket = 0;
    rbracket = 0;
    while ((lbracket = HtmlDoc.find("{%", lbracket)) && (rbracket = HtmlDoc.find("%}", rbracket))
            && (rbracket != std::string::npos) && (lbracket != std::string::npos)) {
        std::string htmltemplate = HtmlDoc.substr(lbracket+2, rbracket-lbracket-2);
        htmltemplates.push(htmltemplate);
        lbracket = rbracket+2;
        rbracket = rbracket+2;
    }
    HTML = HtmlDoc;
}
//  queue будет заменена на map для хранения параметров
std::queue<std::string> TemplateManagerOld::GetParameterNames() {
    return  htmlparameters;
}
std::queue<std::string> TemplateManagerOld::GetTemplateNames() {
    return htmltemplates;
}

std::vector<char> TemplateManagerOld::GetHtml(std::queue<std::string> &parameters,
                                          std::queue<std::string> &templates) {
    size_t lbracket = 0;
    size_t rbracket = 0;
    while ((lbracket = HTML.find("{{")) && (rbracket = HTML.find("}}"))
            && (rbracket != std::string::npos) && (lbracket != std::string::npos)) {
        std::string parameter = parameters.front();
        parameters.pop();
        HTML.erase(lbracket, rbracket+2-lbracket);
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
            && (rbracket != std::string::npos) && (lbracket != std::string::npos)) {
        std::string htmltemplate = templates.front();
        templates.pop();
        HTML.erase(lbracket, rbracket+2-lbracket);
        HTML.insert(lbracket, htmltemplate);
        // HTML.append(htmltemplate, lbracket, htmltemplate.length());
    }
    return std::vector<char> (HTML.begin(), HTML.end());
}


std::vector<char> TemplateManagerOld::GetHtmlFixed(std::queue<std::string> &parameters,
                                          std::queue<std::string> &templates, std::string url) {
    size_t lbracket = 0;
    size_t rbracket = 0;

    std::string path("../static" + url);
    std::string path_templates("../static/templates/");
    while ((lbracket = HTML.find("{%")) && (rbracket = HTML.find("%}"))
            && (rbracket != std::string::npos) && (lbracket != std::string::npos)) {
        std::string htmltemplate = templates.front();
        templates.pop();
        HTML.erase(lbracket, rbracket+2-lbracket);
        HTML.insert(lbracket, htmltemplate);
        // HTML.append(htmltemplate, lbracket, htmltemplate.length());
    }

    /* 
std::string path("../static" + url);
    if (url == "/")
        path += "IndexTemplate.html";  // "index.html"

    std::ifstream source(path, std::ios::binary);
    TemplateManager TManager(source);
    std::queue<std::string> parameters = TManager.GetParameterNames();
    std::queue<std::string> temp = TManager.GetTemplateNames();
    DatabaseManager DManager;

    std::queue<std::string> parameters_new = DManager.GetParameters(parameters);
    std::queue<std::string> temp_new = DManager.GetTemplates(temp);

    std::vector<char> result = TManager.GetHtml(parameters_new, temp_new);

    // char buffer[BUF_SIZE] = {0};
    // while (source.read(buffer, BUF_SIZE)) {
    //     data.insert(data.end(), buffer, buffer + BUF_SIZE);
    // }
    // data.insert(data.end(), buffer, buffer + source.gcount());
    data.insert(data.end(), result.begin(), result.end());
    */
    return std::vector<char> (HTML.begin(), HTML.end());
}


TemplateManager::TemplateManager(const std::string &url) {
    std::string path("../static");
    if (url == "/")
        path += url + "IndexTemplate.html";  // "index.html"
    // check url type
    if (url.find("video") != std::string::npos) {
        path = "../static/templates/Video.html";
    }
    // load template based on url type
    std::ifstream source(path, std::ios::binary);
    std::string HtmlDoc((std::istreambuf_iterator<char>(source)),
                 std::istreambuf_iterator<char>());
    // replace all {%%} with actual text, keep {{}} and {%FOR%}{%ENDFOR%}
    size_t lbracket = 0;
    size_t rbracket = 0;
    InsertTemplates(HtmlDoc, lbracket, rbracket);
    lbracket = 0;
    rbracket = 0;
    while ((lbracket = HtmlDoc.find("{{", lbracket)) && (rbracket = HtmlDoc.find("}}", rbracket))
            && (rbracket != std::string::npos) && (lbracket != std::string::npos)) {
        std::string parameter = HtmlDoc.substr(lbracket+2, rbracket-lbracket-2);
        htmlparameters.push(parameter);
        lbracket = rbracket+2;
        rbracket = rbracket+2;
    }
    HTML = HtmlDoc;
}

void TemplateManager::InsertTemplates(std::string &page, size_t l, size_t r) {
    while ((l = page.find("{%", l)) && (r = page.find("%}", r))
            && (r != std::string::npos) && (l != std::string::npos)) {
        std::string htmltemplate = page.substr(l+2, r-l-2);
        l = htmltemplate.find("FOR");
        if ((l != std::string::npos)) {
            l = r + 2;
            r += 2;
            InsertTemplates(page, l, r);
        } else {
            std::string path("../static/templates/"+htmltemplate+".html");
            std::ifstream source(path, std::ios::binary);
            std::string HtmlDoc((std::istreambuf_iterator<char>(source)),
                std::istreambuf_iterator<char>());
            l = r - htmltemplate.length() - 2;
            page.erase(l, r-l+2);
            page.insert(l, HtmlDoc);
        }
        l = r+2;
        r = r+2;
    }
}

void TemplateManager::FixCycles(std::string &page, size_t l, size_t r, std::map<std::string, std::string> parameters) {
    while ((l = page.find("{%FOR", l)) && (r = page.find("{%ENDFOR%}", r))
            && (r != std::string::npos) && (l != std::string::npos)) {
        l = page.find("{%FOR", l+1);
        if ((l != std::string::npos) && (l < r)) {
            l = page.find("%}")+2;
            FixCycles(page, l, r, parameters);
        } else {
            l = page.rfind("{%FOR", r) + 6;
            std::string htmltemplate = page.substr(l, page.rfind(" TO", r) - l);
            int start = std::stoi(htmltemplate);
            // add 0 processing
            l = page.rfind(" TO ", r) + 6;
            htmltemplate = page.substr(l, page.rfind(" %}", r) - l - 2);
            int end;
            if (!parameters[htmltemplate].empty()) {
                end = std::stoi(parameters[htmltemplate]);
            } else {
                l = page.rfind(" TO ", r) + 4;
                htmltemplate = page.substr(l, page.rfind(" %}", r) - l);
                end = std::stoi(htmltemplate);
            }
            l = page.rfind("%}", r) + 2;
            htmltemplate = page.substr(l, r - l);
            l = page.rfind("{%FOR", r);
            page.erase(l, r - l + 10);
            for (int i = start; i < end; i++) {
                page.insert(l, htmltemplate);
            }
            l = 0;
            r = 0;
            FixCycles(page, l, r, parameters);
        }
    }
}


std::vector<char> TemplateManager::GetHtmlFixed(std::map<std::string, std::string> parameters,
                                                 std::string url) {
    std::string path("../static");
    if (url == "/")
        path += url + "IndexTemplate.html";  // "index.html"

    // check url type
    if (url.find("video") != std::string::npos) {
        path = "../static/templates/Video.html";
    }
    // load template based on url type
    std::ifstream source(path, std::ios::binary);
    std::string HtmlDoc((std::istreambuf_iterator<char>(source)),
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
            && (rbracket != std::string::npos) && (lbracket != std::string::npos)) {
        std::string parameter = HtmlDoc.substr(lbracket+2, rbracket-lbracket-2);
        HtmlDoc.erase(lbracket, rbracket+2-lbracket);
        HtmlDoc.insert(lbracket, parameters[parameter]);
        lbracket = 0;
        rbracket = 0;
    }
    std::vector<char> test (HtmlDoc.begin(), HtmlDoc.end());
    return test;
}

std::queue<std::string> TemplateManager::GetParameterNames() {
    return  htmlparameters;
}
