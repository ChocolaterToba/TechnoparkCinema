#include "HttpResponseReader.hpp"

HttpResponseReader::HttpResponseReader(const string &message) {
    return_code = GetReturnCode(message);
    headers = ExtractHeaders(message);
    if (headers.contains("Content-length"))
        body = ExtractBody(message);
}

string HttpResponseReader::GetReturnCode(const string &message) const {
    size_t pos = message.find('\n');
    while (!std::isalnum(message[pos]))
        --pos;

    return message.substr(pos - 2, 3);  // 3 is length of "xxx", which means the code
}

string HttpResponseReader::GetReturnCode() const {
    return return_code;
}

map<string, string> HttpResponseReader::GetAllHeaders() const {
    return headers;
}

size_t HttpResponseReader::GetContentLength() const {
    if (headers.contains("Content-length"))
        return std::stoi(headers.at("Content-length"));
    else
        return 0;
}

string HttpResponseReader::GetHeaderValue(const string &header_name) const {
    if (headers.contains(header_name))
        return headers.at(header_name);
    else
        return string();
}

vector<char> HttpResponseReader::GetBody() const {
    return body;
}

map<string, string> HttpResponseReader::ExtractHeaders(const string &message) {
    map<string, string> result;

    size_t search = message.find('\r', 0) + 2;  // "\r\n"
    size_t pos;
    while ((pos = message.find(": ", search)) != string::npos) {
        string header_name = message.substr(search, pos - search);
        pos += 2;  // ": "
        search = message.find('\r', pos);
        if (message[search + 2] == '\r')  // it means that headers part is over
            break;

        string header_value = message.substr(pos, search - pos);
        result[header_name] = header_value;
        search += 2;  // \r\n 2 symbols
    }

    return result;
}

vector<char> HttpResponseReader::ExtractBody(const string &message) const {
    size_t end_of_head = message.find("/r/n/r/n", 0);  // first empty line is separating the head and the body
    size_t body_size;
    if (headers.contains("Content-length"))
        body_size = stoi(headers.at("Content-length"));
    else
        body_size = message.size() - end_of_head;

    string body_str = message.substr(end_of_head + 2, body_size);
    return vector<char>(body_str.begin(), body_str.end());
}
