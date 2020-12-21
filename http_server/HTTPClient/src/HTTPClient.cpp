#include "HTTPClient.hpp"
#include <iostream>
#include <string>
#include <algorithm>

int BUFFER_SIZE = 256;


HTTPClient::HTTPClient(std::shared_ptr<Socket> socket) : socket(socket) {
    socket->setRcvTimeout(/*sec*/ 120, /*microsec*/ 0);
}

HTTPClient::HTTPClient(int port, int queueSize) {
    // TODO:: add "only for sending" option
    socket = std::make_shared<Socket>();
    socket->createServerSocket(port, queueSize);
    socket->setRcvTimeout(/*sec*/ 120, /*microsec*/ 0);
}

std::vector<char>::iterator HTTPClient::parseBuffer(std::vector<char>& buffer, std::string& target) {
    // Returns true if '\0' was found, which means that binary body started.
    auto endlineIter = std::find(buffer.begin(), buffer.end(), '\0');
    target += std::string(buffer.begin(), endlineIter);
    
    return endlineIter;
}

void HTTPClient::setBody(std::queue<std::string>& bodyQueue, const std::string& separator) {
    body.clear();
    for (size_t i = 0; i < bodyQueue.size(); ++i) {
        std::string& str = bodyQueue.front();
        std::copy(str.begin(), str.end(), std::back_inserter(body));
        bodyQueue.pop();
        bodyQueue.push(str);
        for (auto& character : separator) {
            if (character) {
                body.push_back(character);
            }
        }
    }
}

void HTTPClient::recvHeader() {
    header.clear();
    body.clear();
    std::string result;
    std::vector<char> buffer;

    bool binaryBodyStarted = false;
    while (result.find("\r\n\r\n") == std::string::npos &&
           result.find("\n\n") == std::string::npos) {
        buffer = std::move(socket->recvVector());

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
    // std::cerr << "Received header: " << std::endl << header << std::endl;
    std::cerr << "Received header" << std::endl;

    std::vector<char> temp(result.begin() + bodyStartIndex + shift, result.end());
    if (binaryBodyStarted) {
        temp.insert(temp.end(), body.begin(), body.end());
    }
    body = std::move(temp);
}

void HTTPClient::recvBody(size_t contentLength) {
    contentLength -= body.size();
    std::vector<char> receivedBody = std::move(socket->recvVector(contentLength));
    body.insert(body.begin(), receivedBody.begin(), receivedBody.end());
    std::cerr << "Successfully received body, size: " << body.size() << " bytes" << std::endl;
}

void HTTPClient::send(bool close) {
    socket->send(header + "\r\n\r\n");
    socket->send(body);
    if (close) {
        socket->close();
    }
}

void HTTPClient::send(std::vector<char> data, bool close) {
    socket->send(std::move(data));
    if (close) {
        socket.reset();
    }
}

int HTTPClient::getPort() const {
    if (socket != nullptr) {
        return socket->getPort();
    }
    return -1;
}

int HTTPClient::getSd() const {
    if (socket != nullptr) {
        return socket->sd();
    }
    return -1;
}
