#include "HttpResponse.hpp"
#include "HttpRequest.hpp"

#include <iostream>
#include <fstream>
#include <string>

int main() {
    HttpResponse response;

    response.SetData("/");

    return 0;
}
