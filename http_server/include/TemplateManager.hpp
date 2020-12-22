#pragma once

#include <queue>
#include <string>
#include <fstream>
#include <streambuf>
#include <vector>
#include <map>

using std::string;
using std::vector;
using std::queue;

// работа с TemplateManager, создаем экземпляр, запрашиваем parameters/templates
// запрашиваем их у БД, после ответа передаем в GetHtml

class TemplateManager {
 public:
    explicit TemplateManager(const string &url);
    ~TemplateManager() {}
    queue<string> GetParameterNames();
    vector<char> GetHtmlFixed(std::map<string, string> parameters, string url);

    void ExtractParameters();
    void InsertTemplates();

private:

    // заменить очереди на map
    // будут vector<std::pair<int, string> для templates. Int для FOR

    string HTML;
    queue<string> html_parameters;
    void FixCycles(string &page, size_t l, size_t r, std::map<string, string> parameters);
    string GetHTML();
};


// работа с TemplateManager, создаем экземпляр, запрашиваем parameters/teplates
// запрашиваем их у БД, после ответа передаем в GetHtml