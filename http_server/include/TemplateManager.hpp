#pragma once

#include <queue>
#include <string>
#include <fstream>
#include <streambuf>
#include <vector>
#include <map>


// работа с TemplateManager, создаем экземпляр, запрашиваем parameters/teplates
// запрашиваем их у БД, после ответа передаем в GetHtml
class TemplateManager {
 public:
    explicit TemplateManager(const std::string &url);
    ~TemplateManager() {}
    std::queue<std::string> GetParameterNames();
    std::vector<char> GetHtmlFixed(std::map<std::string, std::string> parameters,
                                                 std::string url);


 private:
    // заменить очереди на map
    // будут std::vector<std::pair<int, std::string> для templates. Int для FOR
    std::string HTML;
    std::queue<std::string> htmlparameters;
    void InsertTemplates(std::string &page, size_t l, size_t r);
    void FixCycles(std::string &page, size_t l, size_t r, std::map<std::string, std::string> parameters);
    std::string GetHTML();
};


// работа с TemplateManager, создаем экземпляр, запрашиваем parameters/teplates
// запрашиваем их у БД, после ответа передаем в GetHtml
class TemplateManagerOld {
 public:
    explicit TemplateManagerOld(std::ifstream &source);
    ~TemplateManagerOld() {}
    std::queue<std::string> GetParameterNames();
    std::queue<std::string> GetTemplateNames();
    std::vector<char> GetHtml(std::queue<std::string> &parameters,
                              std::queue<std::string> &templates);
    std::vector<char> GetHtmlFixed(std::queue<std::string> &parameters,
                              std::queue<std::string> &templates, std::string url);


 private:
    // заменить очереди на map
    // будут std::vector<std::pair<int, std::string> для templates. Int для FOR
    std::string HTML;
    std::queue<std::string> htmltemplates;
    std::queue<std::string> htmlparameters;
    std::vector<std::string> GetInsertionList();
    std::string GetHTML();
};
