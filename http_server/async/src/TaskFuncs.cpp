#include "TaskFuncs.hpp"

#include <vector>
#include "HTTPClient.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"

MainFuncType PreProcess(std::string& headers, std::vector<char>& data, HTTPClient& input) {
    input.recvHeader();
    headers = input.getHeader();
    HttpRequest request(headers);
    HttpResponse response(request);

    input.send(std::move(response.GetData()), true);
    return MainProcessBasic;
}
void MainProcessBasic(std::string& headers, std::vector<char>& body, HTTPClient& output) {}
void PostProcess(std::string& headers, std::vector<char>& body, HTTPClient& output) {}