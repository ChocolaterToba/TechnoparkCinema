#include "HttpResponse.hpp"
#include "HttpRequest.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <gtest/gtest.h>


TEST(StartTest, test) {
    std::string value("POST /cgi-bin/qtest HTTP/1.1\r\nHost: aram\r\nUser-Agent: Mozilla/5.0 Gecko/2009042316 Firefox/3.0.10\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,/;q=0.8\r\nAccept-Language: en-us,en;q=0.5\r\nAccept-Encoding: gzip,deflate\r\nAccept-Charset: ISO-8859-1,utf-8;q=0.7,*;q=0.7\r\nKeep-Alive: 300\r\nConnection: keep-alive\r\nReferer: http://aram/~martind/banner.htm\r\nContent-Type: multipart/form-data; boundary=2a8ae6ad-f4ad-4d9a-a92c-6d217011fe0f\r\nContent-Length: 514\r\n");
    HttpRequest request(value);
    RequestMethod method = request.GetRequestMethod();
    std::string url = request.GetURL();
    std::string test_url("/cgi-bin/qtest");
    std::string http_ver = request.GetHTTPVersion();
    std::string http_ver_test("1.1");
    ASSERT_EQ(method, POST);
    ASSERT_EQ(url, test_url);
    ASSERT_EQ(http_ver, "1.1");
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}