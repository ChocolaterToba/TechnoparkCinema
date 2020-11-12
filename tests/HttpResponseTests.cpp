#include <gtest/gtest.h>
//#include <gmock/gmock.h>
#include <string>
#include <memory>
#include <fstream>

#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "ResponseBuilder.hpp"

//using ::testing::AtLeast;
//using ::testing::Return;
//using ::testing::DoAll;
//using ::testing::SetArgReferee;

//class MockRequest : public HttpRequest {
//public:
//    MOCK_METHOD(std::string, GetURL, (), (const, override));
//    MOCK_METHOD(RequestMethod, GetRequestMethod, (), (const, override));
//};



static bytes ReadBytes(const std::string &src) {
    bytes data;
    std::ifstream source(src, std::ios::binary);
    char buffer[BUF_SIZE] = {0};
    while (source.read(buffer, BUF_SIZE)) {
        data.insert(data.end(), buffer, buffer + BUF_SIZE);
    }
    data.insert(data.end(), buffer, buffer + source.gcount());
    return data;
}

TEST(ResponseBuilderTest, GetStaticTest) {
    bytes expected_response_data = ReadBytes("../tests/data/responses/static_get.bytes");
    HttpRequest request(ReadBytes("../tests/data/requests/static_get.bytes"));

    std::shared_ptr<ResponseBuilder> builder = std::make_shared<StaticResponseBuilder>(request);
    builder->MakeResponse();
    auto response = builder->GetResponse();
    bytes received_response_data = response->GetFullData();

    ASSERT_EQ(received_response_data, expected_response_data);
}

TEST(ResponseBuilderTest, OptionsTest) {
    bytes expected_response_data = ReadBytes("../tests/data/responses/options.bytes");
    HttpRequest request(ReadBytes("../tests/data/requests/options.bytes"));

    std::shared_ptr<ResponseBuilder> builder = std::make_shared<OptionsResponseBuilder>(request);
    builder->MakeResponse();
    auto response = builder->GetResponse();
    bytes received_response_data = response->GetFullData();

    ASSERT_EQ(received_response_data, expected_response_data);
}

TEST(ResponseBuilderTest, VideoTest) {
    bytes expected_response_data = ReadBytes("../tests/data/responses/video_1.bytes");
    HttpRequest request(ReadBytes("../tests/data/requests/video_1.bytes"));

    std::shared_ptr<ResponseBuilder> builder = std::make_shared<VideoResponseBuilder>(request);
    builder->MakeResponse();
    std::cout << builder.get() << std::endl;
    auto response = builder->GetResponse();
    bytes received_response_data = response->GetFullData();

    ASSERT_EQ(received_response_data, expected_response_data);
}

TEST(ResponseBuilderTest, AuthorizationTest) {
    bytes expected_response_data = ReadBytes("../tests/data/responses/authorization_1.bytes");
    HttpRequest request(ReadBytes("../tests/data/requests/authorization_1.bytes"));

    auto builder = std::make_shared<AuthorizationHandler_ResponseBuilder>(request);
    builder->MakeResponse();
    auto response = builder->GetResponse();
    bytes received_response_data = response->GetFullData();

    ASSERT_EQ(received_response_data, expected_response_data);
}
