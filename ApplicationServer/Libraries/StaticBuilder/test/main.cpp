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

    FastCGIData DBUserResponse;
    DBUserResponse["authAccess"] = "False";
    EXPECT_CALL(mockDBClient, ReceivePackage)
        .Times(1)
        .WillOnce(Return(DBUserResponse));

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

    EXPECT_EQ(pageBuilder->GetResponseData()["authAccess"], "False")
        << "User doen't have access, but page claims they do";
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

    FastCGIData DBUserResponse;
    DBUserResponse["authAccess"] = "True";
    EXPECT_CALL(mockDBClient, ReceivePackage)
        .Times(1)
        .WillOnce(Return(DBUserResponse));

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

    EXPECT_EQ(pageBuilder->GetResponseData()["authAccess"], "True")
        << "User has access, but page claims they don't";
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
    DBFilmResponse["titles"] = "Titanic|Rain Man|Very Scary Movie";
    DBFilmResponse["previews"] = "titanic_preview_path|"
                                 "rain_man_preview_path|"
                                 "very_scary_movie_preview_path";
    DBFilmResponse["ratings"] = "10.0|10.0|0.0";
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

    EXPECT_EQ(pageBuilder->GetResponseData()["titles"],
              DBVideoResponse["titles"])
        << "Film titles were processed incorrectly";
    EXPECT_EQ(pageBuilder->GetResponseData()["previews"],
              DBVideoResponse["previews"])
        << "Film previews were processed incorrectly";
    EXPECT_EQ(pageBuilder->GetResponseData()["ratings"],
              DBVideoResponse["ratings"])
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
    DBFilmsResponse["titles"] = "Titanic|Rain Man|Very Scary Movie";
    DBFilmsResponse["previews"] = "titanic_preview_path|"
                                             "rain_man_preview_path|"
                                             "very_scary_movie_preview_path";
    DBFilmsResponse["ratings"] = "10.0|10.0|0.0";

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

    MainPage mainPage(mockDBClient, mockVideoFilesClient);

    //  Testing actual page builder.
    StaticBuilder* pageBuilder = static_cast<StaticBuilder*>(&mainPage);

    FastCGIData exampleRequestData;
    exampleRequestData["Username"] = "ExampleUser";

    pageBuilder->ParseRequestData(exampleRequestData);
    pageBuilder->AddMetadata();
    pageBuilder->CreateResponseData();

    EXPECT_EQ(pageBuilder->GetResponseData()["titlesNew"],
              "Very Scary Movie|Titanic|Rain Man")
        << "Film titles were processed incorrectly";
    EXPECT_EQ(pageBuilder->GetResponseData()["titlesHot"],
              "Titanic|Rain Man|Very Scary Movie")
        << "Film titles were processed incorrectly";
    EXPECT_EQ(pageBuilder->GetResponseData()["titlesUnfinished"],
              "Rain Man")
        << "Film titles were processed incorrectly";

    EXPECT_EQ(pageBuilder->GetResponseData()["previewsNew"],
              "very_scary_movie_preview_path|"
              "titanic_preview_path|rain_man_preview_path")
        << "Film previews were processed incorrectly";
    EXPECT_EQ(pageBuilder->GetResponseData()["previewsHot"],
              "titanic_preview_path|rain_man_preview_path|"
              "very_scary_movie_preview_path")
        << "Film previews were processed incorrectly";
    EXPECT_EQ(pageBuilder->GetResponseData()["previewsUnfinished"],
              "titanic_preview_path|rain_man_preview_path|"
              "very_scary_movie_preview_path")
        << "Film previews were processed incorrectly";

    EXPECT_EQ(pageBuilder->GetResponseData()["ratingsNew"],
              "0.0|10.0|10.0")
        << "Film ratings were processed incorrectly";
    EXPECT_EQ(pageBuilder->GetResponseData()["ratingsHot"],
              "10.0|10.0|0.0")
        << "Film ratings were processed incorrectly";
    EXPECT_EQ(pageBuilder->GetResponseData()["ratingsUnfinished"],
              "10.0")
        << "Film ratings were processed incorrectly";
}


TEST(RecommendationsPageTesting, basicTest) {
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

    FastCGIData DBFilmsResponse;
    DBFilmsResponse["titles"] = "Titanic|Rain Man|Very Scary Movie";
    DBFilmsResponse["previews"] = "titanic_preview_path|"
                                             "rain_man_preview_path|"
                                             "very_scary_movie_preview_path";
    DBFilmsResponse["ratings"] = "10.0|10.0|0.0";

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

    RecommendationsPage recommendationsPage(mockDBClient, mockVideoFilesClient);

    //  Testing actual page builder.
    StaticBuilder* pageBuilder = \
        static_cast<StaticBuilder*>(&recommendationsPage);

    FastCGIData exampleRequestData;
    exampleRequestData["Username"] = "ExampleUser";

    pageBuilder->ParseRequestData(exampleRequestData);
    pageBuilder->AddMetadata();
    pageBuilder->CreateResponseData();

    EXPECT_EQ(pageBuilder->GetResponseData()["titles"],
              "Rain Man")
        << "Film titles were processed incorrectly";
    EXPECT_EQ(pageBuilder->GetResponseData()["previews"],
              "rain_man_preview_path")
        << "Film previews were processed incorrectly";
    EXPECT_EQ(pageBuilder->GetResponseData()["ratings"],
              "10.0")
        << "Film ratings were processed incorrectly";
}


TEST(RoomPageTesting, basicTest) {
    //  Setting up mock client for database.
    MockClient mockDBClient;

    EXPECT_CALL(mockDBClient, SendPackage)
        .Times(1)
        .WillOnce(Return(void));

    EXPECT_CALL(mockDBClient, Listen)
        .Times(1)
        .WillOnce(Return(void));

    FastCGIData DBFilmsResponse;
    DBFilmsResponse["chat"] = "alice: This is great!|bob:No, that's wrong!";
    DBFilmsResponse["startTimestamp"] = "00::10::25";

    EXPECT_CALL(mockDBClient, ReceivePackage)
        .Times(1)
        .WillOnce(Return(DBFilmsResponse));

    //  Setting up mock client for video fileserver.
    MockClient mockVideoFilesClient;

    EXPECT_CALL(mockVideoFilesClient, SendPackage)
        .Times(0);

    EXPECT_CALL(mockVideoFilesClient, Listen)
        .Times(0);

    EXPECT_CALL(mockVideoFilesClient, ReceivePackage)
        .Times(0);

    RoomPage roomPage(mockDBClient, mockVideoFilesClient);

    //  Testing actual page builder.
    StaticBuilder* pageBuilder = static_cast<StaticBuilder*>(&roomPage);

    FastCGIData exampleRequestData;
    exampleRequestData["Username"] = "ExampleUser";
    exampleRequestData["RoomCode"] = "12345678";

    pageBuilder->ParseRequestData(exampleRequestData);
    pageBuilder->AddMetadata();
    pageBuilder->CreateResponseData();

    EXPECT_EQ(pageBuilder->GetResponseData()["chat"],
              DBFilmsResponse["chat"])
        << "Chat was processed incorrectly";
    EXPECT_EQ(pageBuilder->GetResponseData()["startTimestamp"],
              DBFilmsResponse["startTimestamp"])
        << "'Starting from' timestap was processed incorrectly";
}


TEST(VideoPreviewBlock, basicTest) {
    //  Setting up mock client for database.
    MockClient mockDBClient;

    EXPECT_CALL(mockDBClient, SendPackage)
        .Times(1)
        .WillOnce(Return(void));

    EXPECT_CALL(mockDBClient, Listen)
        .Times(1)
        .WillOnce(Return(void));

    FastCGIData DBFilmsResponse;
    DBFilmsResponse["previewImagePath"] = "titanic_preview_path|";
    DBFilmsResponse["previewVideoPath"] = "titanic_preview_video_path|";
    DBFilmsResponse["rating"] = "10.0";
    DBFilmsResponse["director"] = "James Cameron";
    DBFilmsResponse["premiered"] = "18.11.1997";
    DBFilmsResponse["actors"] = "Leonardo DiCaprio|Kate Winslet|"
                                "William Zane|William Paxton";

    EXPECT_CALL(mockDBClient, ReceivePackage)
        .Times(1)
        .WillOnce(Return(DBFilmsResponse));

    //  Setting up mock client for video fileserver.
    MockClient mockVideoFilesClient;

    EXPECT_CALL(mockVideoFilesClient, SendPackage)
        .Times(0);

    EXPECT_CALL(mockVideoFilesClient, Listen)
        .Times(0);

    EXPECT_CALL(mockVideoFilesClient, ReceivePackage)
        .Times(0);

    VideoPreviewBlock previewBlock(mockDBClient, mockVideoFilesClient);

    //  Testing actual page builder.
    StaticBuilder* pageBuilder = static_cast<StaticBuilder*>(&previewBlock);

    FastCGIData exampleRequestData;
    exampleRequestData["movieName"] = "Titanic";

    pageBuilder->ParseRequestData(exampleRequestData);
    pageBuilder->AddMetadata();
    pageBuilder->CreateResponseData();

    DBFilmsResponse["previewImagePath"] = "titanic_preview_path|";
    DBFilmsResponse["previewVideoPath"] = "titanic_preview_video_path|";
    DBFilmsResponse["rating"] = "10.0";
    DBFilmsResponse["director"] = "James Cameron";
    DBFilmsResponse["premiered"] = "18.11.1997";
    DBFilmsResponse["actors"] = "Leonardo DiCaprio|Kate Winslet|"
                                "William Zane|William Paxton";

    EXPECT_EQ(pageBuilder->GetResponseData()["previewImagePath"],
              DBFilmsResponse["previewImagePath"])
        << "Film image preview was processed incorrectly";
    EXPECT_EQ(pageBuilder->GetResponseData()["previewVideoPath"],
              DBFilmsResponse["previewVideoPath"])
        << "Film video preview was processed incorrectly";
    EXPECT_EQ(pageBuilder->GetResponseData()["rating"],
              DBFilmsResponse["rating"])
        << "Film rating was processed incorrectly";
    EXPECT_EQ(pageBuilder->GetResponseData()["director"],
              DBFilmsResponse["director"])
        << "Film director was processed incorrectly";
    EXPECT_EQ(pageBuilder->GetResponseData()["premiered"],
              DBFilmsResponse["premiered"])
        << "Film premiere date was processed incorrectly";
    EXPECT_EQ(pageBuilder->GetResponseData()["actors"],
              DBFilmsResponse["actors"])
        << "Film actors were processed incorrectly";
}


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
