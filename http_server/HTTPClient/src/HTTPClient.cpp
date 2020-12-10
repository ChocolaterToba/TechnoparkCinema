#include "HTTPClient.hpp"
#include <iostream>
#include <string>
#include <algorithm>

int BUFFER_SIZE = 256;

std::vector<char>::iterator HTTPClient::parseBuffer(std::vector<char>& buffer, std::string& target) {
    // Returns true if '\0' was found, which means that binary body started.
    auto endlineIter = std::find(buffer.begin(), buffer.end(), '\0');
    target += std::string(buffer.begin(), endlineIter);
    
    return endlineIter;
}

void HTTPClient::recvHeader(std::shared_ptr<Socket> client) {
    header.clear();
    body.clear();
    std::string result;
    std::vector<char> buffer;

    bool binaryBodyStarted = false;
    while (result.find("\r\n\r\n") == std::string::npos &&
           result.find("\n\n") == std::string::npos) {
        buffer = std::move(client->recvVector());

        auto endlineIter = parseBuffer(buffer, result);
        if (endlineIter != buffer.end()) {  // if '\0' was in buffer
            binaryBodyStarted = true;
            body.insert(body.begin(), endlineIter, buffer.end());
            break;
        }
    }

    size_t bodyStartIndex = result.find("\r\n\r\n");
    size_t shift = 4;
    if (bodyStartIndex == std::string::npos) {
        bodyStartIndex = result.find("\n\n");
        shift = 2;
        if (bodyStartIndex == std::string::npos) {
            throw std::runtime_error(std::string("recvHeader: Met \\0 character in header!"));
        }
    }

    header = result.substr(0, bodyStartIndex + shift / 2);
    std::cerr << "Received header: " << std::endl << header << std::endl;

    std::vector<char> temp(result.begin() + bodyStartIndex + shift, result.end());
    if (binaryBodyStarted) {
        temp.insert(temp.end(), body.begin(), body.end());
    }
    body = std::move(temp);
}

void HTTPClient::recvBody(std::shared_ptr<Socket> client, size_t contentLength) {
    contentLength -= body.size();
    std::vector<char> receivedBody = std::move(client->recvVector(contentLength));
    body.insert(body.begin(), receivedBody.begin(), receivedBody.end());
    std::cerr << "Successfully received body, size: " << body.size() << " bytes" << std::endl;
}
