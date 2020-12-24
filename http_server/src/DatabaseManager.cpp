#include <string>
#include <map>
#include <queue>
#include <vector>
#include "exceptions.hpp"
#include <algorithm>
#include <random>
#include "DatabaseManager.hpp"
#include <chrono>
#include <set>
// not started yet
DatabaseManager::DatabaseManager() {

}

std::set<std::string> DatabaseManager::GetParameters(std::set<std::string> names) {
    std::set<std::string> temp;
    temp.insert("Titanic");
    temp.insert("Subscribe Woosh.com to watch more kittens.");
    temp.insert("images/Leo.jpeg");
    temp.insert("Leonardo Dicaprio");
    temp.insert("images/img1.jpg");
    temp.insert("Titanic");
    temp.insert("lorem_ipsum.mp4");
    return temp;
}

std::queue<std::string> DatabaseManager::GetTemplates(std::queue<std::string> names) {
    std::queue<std::string> temp;
    temp.push("<img src =\"images/cat.png\" title = \"WooshStar\">");
    temp.push("<img src =\"images/cat.png\" title = \"WooshStar\">");
    temp.push("<img src =\"images/cat.png\" title = \"WooshStar\">");
    temp.push("<img src =\"images/cat.png\" title = \"WooshStar\">");

    std::vector<std::string> vect;
    vect.push_back("<div class = \"col-md-2\"><img src = \"images/img1.jpg\"></div>");
    vect.push_back("<div class = \"col-md-2\"><img src = \"images/img2.jpg\"></div>");
    vect.push_back("<div class = \"col-md-2\"><img src = \"images/img3.jpg\"></div>");
    vect.push_back("<div class = \"col-md-2\"><img src = \"images/img4.jpg\"></div>");
    vect.push_back("<div class = \"col-md-2\"><img src = \"images/img5.jpg\"></div>");
    vect.push_back("<div class = \"col-md-2\"><img src = \"images/img6.jpg\"></div>");
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine e(seed);
    //  auto rng = std::default_random_engine {};
    std::shuffle(std::begin(vect), std::end(vect), e);
    for (size_t i = 0; i < 6; i++) {
        temp.push(vect[i]);
    }

    return temp;
}
