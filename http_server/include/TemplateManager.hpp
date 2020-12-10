#pragma once

#include <queue>
#include <string>
#include <fstream>
#include <streambuf>
#include <vector>


// работа с TemplateManager, создаем экземпляр, запрашиваем parameters/teplates
// запрашиваем их у БД, после ответа передаем в GetHtml
class TemplateManager {
 public:
    explicit TemplateManager(std::ifstream &source);
    ~TemplateManager() {}
    std::queue<std::string> GetParameterNames();
    std::queue<std::string> GetTemplateNames();
    std::vector<char> GetHtml(std::queue<std::string> &parameters,
                              std::queue<std::string> &templates);


 private:
    // будут std::vector<std::pair<int, std::string> для templates. Int для FOR
    std::string HTML;
    std::queue<std::string> htmltemplates;
    std::queue<std::string> htmlparameters;
    std::vector<std::string> GetInsertionList();
    std::string GetHTML();
};
