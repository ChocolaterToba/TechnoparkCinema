#pragma once

#include <memory>
#include <string>

#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
//#include "exceptions.hpp"

#define ALLOWED_METHODS "HEAD, OPTIONS, GET"

class ResponseBuilder {
protected:
    std::shared_ptr<HttpResponse> response;
    HttpRequest request;
public:
    explicit ResponseBuilder(const HttpRequest &request1);
    virtual void MakeResponse() = 0;
    std::shared_ptr<HttpResponse> GetResponse();
};

class StaticResponseBuilder: public ResponseBuilder {
private:
    void SetContentType(const std::string &url);
    static bool ContentExists(const std::string &url);
    static bytes GetRawData(const std::string &url);

public:
    explicit StaticResponseBuilder(const HttpRequest &request1);
    void MakeResponse() override;
};

class OptionsResponseBuilder: public ResponseBuilder {
public:
    explicit OptionsResponseBuilder(const HttpRequest &request1);
    void MakeResponse() override;
};

class VideoResponseBuilder: public ResponseBuilder {
public:
    explicit VideoResponseBuilder(const HttpRequest &request1);
    void MakeResponse() override;
};

class AuthorizationHandler_ResponseBuilder: public ResponseBuilder {
    explicit AuthorizationHandler_ResponseBuilder(const HttpRequest &request1);
    void MakeResponse() override;
};

