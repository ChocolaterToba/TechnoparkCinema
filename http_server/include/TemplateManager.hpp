#pragma once

#include <queue>
#include <string>
#include <fstream>
#include <streambuf>
#include <vector>
#include <map>
#include <set>

using std::string;
using std::vector;
using std::queue;
using std::set;

// работа с TemplateManager, создаем экземпляр, запрашиваем parameters/templates
// запрашиваем их у БД, после ответа передаем в GetHtml

class TemplateManager {
 public:
    explicit TemplateManager(const string &url);
    ~TemplateManager() = default;
    std::set<string> GetParameterNames();
    vector<char> GetHtmlFinal(const std::map<string, string> &parameters);
private:
    string HTML;
    std::set<string> html_parameters;

    void ExtractParameters();
    void InsertTemplates();

    void EvaluateCycles(const std::map<string, string> &parameters);
    void EvaluateParameters(const std::map<string, string> &parameters);
};
