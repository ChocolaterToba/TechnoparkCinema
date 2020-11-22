#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <fstream>
#include <string>
#include <iomanip>

#include "StaticBuilder.hpp"
#include "AuthorizationPage.hpp"
#include "FCGIClient.hpp"

class MockClient : public FCGIClient {
 public:
    MOCK_METHOD(void, listen, void);
    MOCK_METHOD(std::vector<std::string>, ReceivePackage, void);
    MOCK_METHOD(void, SendPackage, std::vector<std::string>);
};

bool compareFiles(const std::string& p1, const std::string& p2) {
  std::ifstream f1(p1, std::ifstream::binary|std::ifstream::ate);
  std::ifstream f2(p2, std::ifstream::binary|std::ifstream::ate);

  if (f1.fail() || f2.fail()) {
    return false;  //  file problem
  }

  if (f1.tellg() != f2.tellg()) {
    return false;  //  size mismatch
  }

  //  seek back to beginning and use std::equal to compare contents
  f1.seekg(0, std::ifstream::beg);
  f2.seekg(0, std::ifstream::beg);
  return std::equal(std::istreambuf_iterator<char>(f1.rdbuf()),
                    std::istreambuf_iterator<char>(),
                    std::istreambuf_iterator<char>(f2.rdbuf()));
}

TEST(ParseRequestTesting, test1) {
    MockClient mockClient;
    StaticBuilder page;
    EXPECT_CALL(mockClient, Listen)
        .Times(1)
        .WillOnce(Return(100));
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}