#pragma once
#include <string>

class RoomCreator {
 private:
    explicit void SetMetaField();
    explicit void SetCookie();


 public:
    RoomCreator(int UserID, int LifeSpan);
    ~RoomManager();
    explicit std::string CreateResponse();
};

class RoomManager {
 private:
    explicit void SetMetaField();
    explicit void SetCookie();
 public:
    RoomManager();
    ~RoomManager();
    explicit std::string CreateResponse();
}
