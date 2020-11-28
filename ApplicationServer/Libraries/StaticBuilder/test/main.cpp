#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <fstream>
#include <iomanip>
#include <string>
#include <vector>

#include "FCGIClient.hpp"
#include "StaticBuilder.hpp"
#include "AuthorizationPage.hpp"
#include "HotVideosPage.hpp"
#include "MainPage.hpp"
#include "RecommendationsPage.hpp"
#include "RoomPage.hpp"
#include "VideoPreviewBlock.hpp"


class MockClient : public FCGIClient {
 public:
    MOCK_METHOD(void, listen, void);
    MOCK_METHOD(FastCGIData, ReceivePackage, void);
    MOCK_METHOD(void, SendPackage, FastCGIData);
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


TEST(AuthPageTesting, AuthUnsuccessfull) {
    //  Setting up mock client for database.
    MockClient mockDBClient;

    EXPECT_CALL(mockDBClient, SendPackage)
        .Times(1)
        .WillOnce(Return(void));

    EXPECT_CALL(mockDBClient, Listen)
        .Times(1)
        .WillOnce(Return(void));

    FastCGIData DBResponse;
    DBResponse["authAccess"] = "False";
    EXPECT_CALL(mockDBClient, ReceivePackage)
        .Times(1)
        .WillOnce(Return(DBResponse));

    //  Setting up mock client for video fileserver.
    MockClient mockVideoFilesClient;

    EXPECT_CALL(mockVideoFilesClient, SendPackage)
        .Times(0);

    EXPECT_CALL(mockVideoFilesClient, Listen)
        .Times(0);

    EXPECT_CALL(mockVideoFilesClient, ReceivePackage)
        .Times(0);

    AuthorizationPage authPage(mockDBClient, mockVideoFilesClient);

    //  Testing actual page builder.
    StaticBuilder* pageBuilder = static_cast<StaticBuilder*>(&authPage);

    FastCGIData RequestData;
    RequestData["Username"] = "ExampleUser";

    pageBuilder->ParseRequestData(RequestData);
    pageBuilder->AddMetadata();
    pageBuilder->CreateResponseData();

    EXPECT_EQ(pageBuilder->GetResponseData()["authAccess"], "False");
}


TEST(AuthPageTesting, AuthSuccessfull) {
    //  Setting up mock client for database.
    MockClient mockDBClient;

    EXPECT_CALL(mockDBClient, SendPackage)
        .Times(1)
        .WillOnce(Return(void));

    EXPECT_CALL(mockDBClient, Listen)
        .Times(1)
        .WillOnce(Return(void));

    FastCGIData DBResponse;
    DBResponse["authAccess"] = "True";
    EXPECT_CALL(mockDBClient, ReceivePackage)
        .Times(1)
        .WillOnce(Return(DBResponse));

    //  Setting up mock client for video fileserver.
    MockClient mockVideoFilesClient;

    EXPECT_CALL(mockVideoFilesClient, SendPackage)
        .Times(0);

    EXPECT_CALL(mockVideoFilesClient, Listen)
        .Times(0);

    EXPECT_CALL(mockVideoFilesClient, ReceivePackage)
        .Times(0);

    AuthorizationPage authPage(mockDBClient, mockVideoFilesClient);

    //  Testing actual page builder.
    StaticBuilder* pageBuilder = static_cast<StaticBuilder*>(&authPage);

    FastCGIData RequestData;
    RequestData["Username"] = "ExampleUser";

    pageBuilder->ParseRequestData(RequestData);
    pageBuilder->AddMetadata();
    pageBuilder->CreateResponseData();

    EXPECT_EQ(pageBuilder->GetResponseData()["authAccess"], "True");
}

TEST(HotPageTesting, basicTest) {
    //  Setting up mock client for database.
    MockClient mockDBClient;

    EXPECT_CALL(mockDBClient, SendPackage)
        .Times(2)
        .WillOnce(Return(void));

    EXPECT_CALL(mockDBClient, Listen)
        .Times(2)
        .WillOnce(Return(void));

    FastCGIData DBUserResponse;
    DBUserResponse["alreadyWatched"] = "Titanic";

    FastCGIData DBVideoResponse;
    DBFilmResponse["filmTitles"] = "Titanic|Rain Man|Very Scary Movie";
    DBFilmResponse["filmPreviews"] = "titanic_preview_path|"
                                     "rain_man_preview_path|"
                                     "very_scary_movie_preview_path";
    DBFilmResponse["filmRatings"] = "10.0|10.0|0.0";
    EXPECT_CALL(mockDBClient, ReceivePackage)
        .Times(2)
        .WillOnce(Return(DBUserResponse)
        .WillOnce(Return(DBFilmResponse));

    //  Setting up mock client for video fileserver.
    MockClient mockVideoFilesClient;

    EXPECT_CALL(mockVideoFilesClient, SendPackage)
        .Times(0);

    EXPECT_CALL(mockVideoFilesClient, Listen)
        .Times(0);

    EXPECT_CALL(mockVideoFilesClient, ReceivePackage)
        .Times(0);

    HotVideosPage hotPage(mockDBClient, mockVideoFilesClient);

    //  Testing actual page builder.
    StaticBuilder* pageBuilder = static_cast<StaticBuilder*>(&hotPage);

    FastCGIData exampleRequestData;
    exampleRequestData["Username"] = "ExampleUser";

    pageBuilder->ParseRequestData(exampleRequestData);
    pageBuilder->AddMetadata();
    pageBuilder->CreateResponseData();

    EXPECT_EQ(pageBuilder->GetResponseData()["filmTitles"],
              DBVideoResponse["filmTitles"])
        << "Film titles were processed incorrectly";
    EXPECT_EQ(pageBuilder->GetResponseData()["filmPreviews"],
              DBVideoResponse["filmPreviews"])
        << "Film previews were processed incorrectly";
    EXPECT_EQ(pageBuilder->GetResponseData()["filmRatings"],
              DBVideoResponse["filmRatings"])
        << "Film ratings were processed incorrectly";
    EXPECT_EQ(pageBuilder->GetResponseData()["alreadyWatched"],
              DBUserResponse["alreadyWatched"])
        << "Already watched films were processed incorrectly";
}


TEST(MainPageTesting, basicTest) {
    //  Setting up mock client for database.
    MockClient mockDBClient;

    EXPECT_CALL(mockDBClient, SendPackage)
        .Times(2)
        .WillOnce(Return(void));

    EXPECT_CALL(mockDBClient, Listen)
        .Times(2)
        .WillOnce(Return(void));

    FastCGIData DBUserResponse;
    DBUserResponse["alreadyWatched"] = "Titanic";
    DBUserResponse["unfinished"] = "Rain Man";

    FastCGIData DBFilmsResponse;
    DBFilmsResponse["filmTitles"] = "Titanic|Rain Man|Very Scary Movie";
    DBFilmsResponse["filmPreviews"] = "titanic_preview_path|"
                                             "rain_man_preview_path|"
                                             "very_scary_movie_preview_path";
    DBFilmsResponse["filmRatings"] = "10.0|10.0|0.0";

    EXPECT_CALL(mockDBClient, ReceivePackage)
        .Times(2)
        .WillOnce(Return(DBUserResponse)
        .WillOnce(Return(DBFilmsResponse));

    //  Setting up mock client for video fileserver.
    MockClient mockVideoFilesClient;

    EXPECT_CALL(mockVideoFilesClient, SendPackage)
        .Times(0);

    EXPECT_CALL(mockVideoFilesClient, Listen)
        .Times(0);

    EXPECT_CALL(mockVideoFilesClient, ReceivePackage)
        .Times(0);

    HotVideosPage hotPage(mockDBClient, mockVideoFilesClient);

    //  Testing actual page builder.
    StaticBuilder* pageBuilder = static_cast<StaticBuilder*>(&hotPage);

    FastCGIData exampleRequestData;
    exampleRequestData["Username"] = "ExampleUser";

    pageBuilder->ParseRequestData(exampleRequestData);
    pageBuilder->AddMetadata();
    pageBuilder->CreateResponseData();

    EXPECT_EQ(pageBuilder->GetResponseData()["filmTitlesNew"],
              "Very Scary Movie|Titanic|Rain Man")
        << "Film titles were processed incorrectly";
    EXPECT_EQ(pageBuilder->GetResponseData()["filmTitlesHot"],
              "Titanic|Rain Man|Very Scary Movie")
        << "Film titles were processed incorrectly";
    EXPECT_EQ(pageBuilder->GetResponseData()["filmTitlesUnfinished"],
              "Rain Man")
        << "Film titles were processed incorrectly";

    EXPECT_EQ(pageBuilder->GetResponseData()["filmPreviewsNew"],
              "very_scary_movie_preview_path|"
              "titanic_preview_path|rain_man_preview_path")
        << "Film previews were processed incorrectly";
    EXPECT_EQ(pageBuilder->GetResponseData()["filmPreviewsHot"],
              "titanic_preview_path|rain_man_preview_path|"
              "very_scary_movie_preview_path")
        << "Film previews were processed incorrectly";
    EXPECT_EQ(pageBuilder->GetResponseData()["filmPreviewsUnfinished"],
              "titanic_preview_path|rain_man_preview_path|"
              "very_scary_movie_preview_path")
        << "Film previews were processed incorrectly";

    EXPECT_EQ(pageBuilder->GetResponseData()["filmRatingsNew"],
              "0.0|10.0|10.0")
        << "Film ratings were processed incorrectly";
    EXPECT_EQ(pageBuilder->GetResponseData()["filmRatingsHot"],
              "10.0|10.0|0.0")
        << "Film ratings were processed incorrectly";
    EXPECT_EQ(pageBuilder->GetResponseData()["filmRatingsUnfinished"],
              "10.0")
        << "Film ratings were processed incorrectly";
}


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
