#include <string>
#include <map>
#include <fstream>
#include <streambuf>
#include <queue>
#include "exceptions.hpp"
#include "TemplateManager.hpp"

TemplateManager::TemplateManager(std::ifstream &source) {
    std::string HtmlDoc((std::istreambuf_iterator<char>(source)),
                 std::istreambuf_iterator<char>());
    size_t pos = 0;

    // заменить на лямбда функции поиска от позиции и возвращать std::string
    // и для {%%} чтобы разпарсить весь документ,
    // в результате получить набор параметров
    size_t lbracket = 0;
    size_t rbracket = 0;
    while ((lbracket = HtmlDoc.find('{{', lbracket)) && (rbracket = HtmlDoc.find('}}', rbracket))) {
        std::string parameter = HtmlDoc.substr(lbracket, rbracket);
        htmlparameters.push(parameter);
        lbracket = rbracket+2;
        rbracket = rbracket+2;
    }
    // временная заглушка, поскольку может быть {{параметр}} внутри макроса {%%}
    // временно будем подставлять готовый "случайный" код из набора
    // нужно парсить {%for:N:{{parameter}}} т.е. N раз читаем parameter
    // и параметры должны быть разные (а-ля ID фильмов)
    while ((lbracket = HtmlDoc.find('{%', lbracket)) && (rbracket = HtmlDoc.find('%}', rbracket))) {
        std::string htmltemplate = HtmlDoc.substr(lbracket, rbracket);
        htmltemplates.push(htmltemplate);
        lbracket = rbracket+2;
        rbracket = rbracket+2;
    }
    HTML = HtmlDoc;
}

std::queue<std::string> TemplateManager::GetParameterNames() {
    return  htmlparameters;
}
std::queue<std::string> TemplateManager::GetTemplateNames() {
    return htmltemplates;
}

std::vector<char> TemplateManager::GetHtml(std::queue<std::string> &parameters,
                                          std::queue<std::string> &templates) {
    size_t lbracket = 0;
    size_t rbracket = 0;
    while ((lbracket = HTML.find('{{')) && (rbracket = HTML.find('}}'))) {
        std::string parameter = parameters.front();
        parameters.pop();
        HTML.erase(lbracket, rbracket+2-lbracket);
        HTML.append(parameter, lbracket, parameter.length());
    }
    // временная заглушка, поскольку может быть {{параметр}} внутри макроса {%%}
    // временно будем подставлять готовый "случайный" код из набора
    // нужно парсить {%for:N:{{parameter}}} т.е. N раз читаем parameter
    // и параметры должны быть разные (а-ля ID фильмов)
    while ((lbracket = HTML.find('{%')) && (rbracket = HTML.find('%}'))) {
        std::string htmltemplate = parameters.front();
        parameters.pop();
        HTML.erase(lbracket, rbracket+2-lbracket);
        HTML.append(htmltemplate, lbracket, htmltemplate.length());
    }
    return std::vector<char> (HTML.begin(), HTML.end());
}

