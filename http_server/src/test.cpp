#include "HttpResponse.hpp"
#include "HttpRequest.hpp"

#include <iostream>
#include <fstream>
#include <string>

int main() {
    HttpRequest request(std::string(""));
    HttpResponse response(request);

    std::fstream fs("response.http", fs.out);
    fs.write(response.GetString().c_str(), response.GetString().size());

    return 0;
}
