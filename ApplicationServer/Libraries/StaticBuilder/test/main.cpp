#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <fstream>
#include <string>
#include <iomanip>

#include "StaticBuilder.hpp"
#include "AuthorizationPage.hpp"

class MockAuthorizationPage : public AuthorizationPage {
 public:
    MOCK_METHOD(void, ParseRequestData, void());
    MOCK_METHOD(void, ReceiveMetadata, void());
    MOCK_METHOD(void, CreateResponseData, void());
    MOCK_METHOD(std::vector<char>&, GetResponseData, void());
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
    MockAuthorizationPage mockPage;
    StaticBuilder* page = static_cast<StaticBuilder*>(&mockPage);
    EXPECT_CALL(mockPage, ParseRequestData)
        .Times(1)
        .WillOnce(Return(100));
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}