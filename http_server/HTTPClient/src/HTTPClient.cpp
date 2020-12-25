#include <string>
#include <vector>
#include <queue>
#include <set>
#include <map>
#include <memory>

#include "HTTPClient.hpp"

int BUFFER_SIZE = 256;


HTTPClient::HTTPClient(std::shared_ptr<Socket> socket) : socket(socket) {
    socket->setRcvTimeout(/*sec*/ 120, /*microsec*/ 0);  // magic??
}

HTTPClient::HTTPClient(int port, int queueSize) {
    socket = std::make_shared<Socket>();
    socket->createServerSocket(port, queueSize);
    socket->setRcvTimeout(/*sec*/ 120, /*microsec*/ 0);  // magic??
}

HTTPClient::HTTPClient(const string& host, int port) {
    socket = std::make_shared<Socket>();
    socket->connect(host, port);
}

HTTPClient::HTTPClient() {
    socket = std::make_shared<Socket>();
}

vector<char>::iterator HTTPClient::parseBuffer(vector<char>& buffer, string& target) {
    // Returns true if '\0' was found, which means that binary body started.
    auto endlineIter = std::find(buffer.begin(), buffer.end(), '\0');
    target += string(buffer.begin(), endlineIter);
    
    return endlineIter;
}

void HTTPClient::setBody(std::queue<string>& bodyQueue, const string& separator) {
    body.clear();
    for (size_t i = 0; i < bodyQueue.size(); ++i) {
        string& str = bodyQueue.front();
        std::copy(str.begin(), str.end(), std::back_inserter(body));
        bodyQueue.pop();
        bodyQueue.push(str);
        if (i < bodyQueue.size() - 1)
            for (auto& character : separator)
                body.push_back(character);
    }
}

std::queue<string> HTTPClient::getBodyQueue(const string& separator) const {
    return splitVectorToQueue(body, separator);
}

std::queue<string> HTTPClient::splitVectorToQueue(const vector<char>& origin, const string& separator) {
    string bodyString(origin.begin(), origin.end());
    std::queue<string> result;

    size_t start = 0, end = 0;
    while ((end = bodyString.find(separator, start)) != string::npos) {
        result.push(bodyString.substr(start, end - start));
        start = end + separator.length();
    }
    if (start < bodyString.size())  // Avoiding cases when origin ends in separator
        result.push(bodyString.substr(start));

    return result;
}

vector<char> HTTPClient::mergeQueueToVector(std::queue<string>& origin, const string& separator) {
    vector<char> result;
    for (size_t i = 0; i < origin.size(); ++i) {
        result.insert(result.end(), origin.front().begin(), origin.front().end());
        origin.push(origin.front());
        origin.pop();
        result.insert(result.end(), separator.begin(), separator.end());
    }

    return result;
}

std::set<string> HTTPClient::splitVectorToSet(const vector<char>& origin, const string& separator) {
    string bodyString(origin.begin(), origin.end());
    std::set<string> result;

    size_t start = 0, end = 0;
    while ((end = bodyString.find(separator, start)) != string::npos) {
        result.insert(bodyString.substr(start, end - start));
        start = end + separator.length();
    }
    if (start < bodyString.size())  // Avoiding cases when origin ends in separator
        result.insert(bodyString.substr(start));

    return result;
}

vector<char> HTTPClient::mergeSetToVector(std::set<string>& origin, const string& separator) {
    vector<char> result;
    for (auto& param : origin) {
        result.insert(result.end(), param.begin(), param.end());
        result.insert(result.end(), separator.begin(), separator.end());
    }

    return result;
}

std::map<string, string> HTTPClient::splitVectorToMap(const vector<char>& origin, const string& separator, const string&
pairSeparator) {
    string bodyString(origin.begin(), origin.end());
    std::map<string, string> result;

    size_t start = 0;
    size_t end = 0;
    while ((end = bodyString.find(separator, start)) != string::npos) {
        string paramPair = bodyString.substr(start, end - start);
        size_t splitPos = paramPair.find(pairSeparator);
        result.insert(std::pair<string, string>(paramPair.substr(0, splitPos), paramPair.substr(splitPos + 2)));
        start = end + separator.length();
    }
    if (start < bodyString.size()) {  // Avoiding cases when origin ends in separator
        string paramPair = bodyString.substr(start);
        size_t splitPos = paramPair.find(": ");
        result.insert(std::pair<string, string>(paramPair.substr(0, splitPos), paramPair.substr(splitPos + 2)));
    }

    return result;
}

vector<char> HTTPClient::mergeMapToVector(std::map<string, string>& origin, const string& separator, const string&
pairSeparator) {
    vector<char> result;
    for (auto& paramPair : origin) {
        result.insert(result.end(), paramPair.first.begin(), paramPair.first.end());
        result.insert(result.end(), pairSeparator.begin(), pairSeparator.end());
        result.insert(result.end(), paramPair.second.begin(), paramPair.second.end());
        result.insert(result.end(), separator.begin(), separator.end());
    }

    return result;
}

void HTTPClient::recvHeader() {
    header.clear();
    body.clear();
    string result;
    vector<char> buffer;

    bool binaryBodyStarted = false;
    while (result.find("\r\n\r\n") == string::npos &&
           result.find("\n\n") == string::npos) {
        buffer = socket->recvVector();

        auto endlineIter = parseBuffer(buffer, result);
        if (endlineIter != buffer.end()) {  // if '\0' was in buffer
            binaryBodyStarted = true;
            body.insert(body.begin(), endlineIter, buffer.end());
            break;
        }
    }

    size_t bodyStartIndex = result.find("\r\n\r\n");
    size_t shift = 4;
    if (bodyStartIndex == string::npos) {
        bodyStartIndex = result.find("\n\n");
        shift = 2;
        if (bodyStartIndex == string::npos)
            throw std::runtime_error(string("recvHeader: Met \\0 character in header!"));
    }

    header = result.substr(0, bodyStartIndex + shift / 2);
    // std::cerr << "Received header: " << std::endl << header << std::endl;
    std::cerr << "Received header: " << std::endl << header << std::endl;

    vector<char> temp(result.begin() + bodyStartIndex + shift, result.end());
    if (binaryBodyStarted)
        temp.insert(temp.end(), body.begin(), body.end());

    body = std::move(temp);
}

void HTTPClient::recvBody(size_t contentLength) {
    contentLength -= body.size();
    vector<char> receivedBody = socket->recvVector(contentLength);
    body.insert(body.end(), receivedBody.begin(), receivedBody.end());

    std::cerr << "Successfully received body, size: " << body.size() << " bytes" << std::endl;
}

void HTTPClient::send(bool close) {
    socket->send(header + "\r\n\r\n");
    socket->send(body);
    if (close) {
        std::cerr << "Closing client, port: " << socket->getPort() << std::endl;
        socket->close();
    }
}

void HTTPClient::send(vector<char> data, bool close) {
    socket->send(std::move(data));
    if (close) {
        std::cerr << "Closing client, port: " << socket->getPort() << std::endl;
        socket.reset();
    }
}

int HTTPClient::getPort() const {
    if (socket != nullptr)
        return socket->getPort();
    else
        return -1;
}

int HTTPClient::getSd() const {
    if (socket != nullptr)
        return socket->sd();
    else
        return -1;
}

void HTTPClient::clear() {
    header.clear();
    body.clear();
}
