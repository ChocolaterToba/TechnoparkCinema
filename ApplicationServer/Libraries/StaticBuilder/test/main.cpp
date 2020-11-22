#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <fstream>
#include <iomanip>
#include <string>
#include <vector>

#include "StaticBuilder.hpp"
#include "AuthorizationPage.hpp"
#include "FCGIClient.hpp"

class MockClient : public FCGIClient {
 public:
    MOCK_METHOD(void, listen, void);
    MOCK_METHOD(std::map<std::string, std::string>, ReceivePackage, void);
    MOCK_METHOD(void, SendPackage, std::map<std::string, std::string>);
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

TEST(AuthPageTesting, test1) {
    //  Setting up mock client for database.
    MockClient mockDBClient;

    EXPECT_CALL(mockDBClient, SendPackage)
        .Times(1)
        .WillOnce(Return(void));

    EXPECT_CALL(mockDBClient, Listen)
        .Times(1)
        .WillOnce(Return(void));

    std::map<std::string, std::string> exampleDBResponse;
    exampleDBResponse["authAccess"] = "False";
    EXPECT_CALL(mockDBClient, ReceivePackage)
        .Times(1)
        .WillOnce(Return(exampleDBResponse));
    
    //  Setting up mock client for video fileserver.
    MockClient mockVideoFilesClient;
    
    AuthorizationPage authPage(mockDBClient, mockVideoFilesClient);
    
    //  Testing actual page builder.
    StaticBuilder* pageBuilder = static_cast<StaticBuilder*>(&authPage);

    std::map<std::string, std::string> exampleRequestData;
    exampleRequestData["Username"] = "ExampleUser";

    pageBuilder->ParseRequestData(exampleRequestData);
    pageBuilder->AddMetadata();
    pageBuilder->CreateResponseData();

    EXPECT_EQ(pageBuilder->GetResponseData()["authAccess"], "False")
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}