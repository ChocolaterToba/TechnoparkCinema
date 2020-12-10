#include <string>
#include <map>
#include <queue>
#include <vector>
#include "exceptions.hpp"
#include <algorithm>
#include <random>
#include "DatabaseManager.hpp"
// not started yet
DatabaseManager::DatabaseManager() {

}

std::queue<std::string> DatabaseManager::GetParameters(std::queue<std::string> names) {
    std::queue<std::string> temp;
    temp.push("Titanic");
    temp.push("Subscribe Woosh.com to watch more kittens.");
    temp.push("images/Leo.jpeg");
    temp.push("Leonardo Dicaprio");
    temp.push("images/img1.jpg");
    temp.push("Titanic");
    temp.push("lorem_ipsum.mp4");
    return temp;
}

std::queue<std::string> DatabaseManager::GetTemplates(std::queue<std::string> names) {
    std::queue<std::string> temp;
    std::vector<std::string> vect;
    temp.push("<img src =\"images/cat.png\" title = \"WooshStar\">");
    temp.push("<img src =\"images/cat.png\" title = \"WooshStar\">");
    temp.push("<img src =\"images/cat.png\" title = \"WooshStar\">");
    temp.push("<img src =\"images/cat.png\" title = \"WooshStar\">");
    vect.push_back("<div class = \"col-md-2\"><img src = \"images/img1.jpg\"></div>");
    vect.push_back("<div class = \"col-md-2\"><img src = \"images/img2.jpg\"></div>");
    vect.push_back("<div class = \"col-md-2\"><img src = \"images/img3.jpg\"></div>");
    vect.push_back("<div class = \"col-md-2\"><img src = \"images/img4.jpg\"></div>");
    vect.push_back("<div class = \"col-md-2\"><img src = \"images/img5.jpg\"></div>");
    vect.push_back("<div class = \"col-md-2\"><img src = \"images/img6.jpg\"></div>");
    auto rng = std::default_random_engine {};
    std::shuffle(std::begin(vect), std::end(vect), rng);
    for (size_t i = 0; i < 6; i++) {
        temp.push(vect[i]);
    }

    return temp;
}
