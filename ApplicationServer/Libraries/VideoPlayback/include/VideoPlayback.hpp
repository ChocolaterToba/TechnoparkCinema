#pragma once
#include <string>

  // todo enum for quality

// enum class Quality {
//    360P     = 0,
//    720P     = 1,
//    1080P    = 2,
//    1440P    = 3,
//    2080P    = 4,
// };

class VideoPlaybackHandler {
 private:
    int video_id_;
    int skip_time_;
    int quality_;
    std::string redirect_link;
    void SetMetaField();
    void SetCookie();
    bool isUserAuthorized();

 public:
    VideoPlaybackHandler(int video_id, int skip_time, int quality);
    ~VideoPlaybackHandler();
    std::string CreateResponse();
};

