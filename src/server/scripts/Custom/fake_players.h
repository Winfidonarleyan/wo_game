#ifndef _FAKE_PLAYERS_H_
#define _FAKE_PLAYERS_H_

enum FakePlayerEvents
{
    LEVEL_UP = 1,
    ZONE_CHANGE,
    LOGOUT,

    TOTAL_EVENTS
};

class FakePlayer
{
public:
    FakePlayer() : accountId(0), guid(ObjectGuid::Empty), zoneId(0), logoutTime(0),
        player(nullptr), session(nullptr), _allowed(true)
    {

    }

    uint32 accountId;
    ObjectGuid guid;
    uint32 zoneId;
    uint32 logoutTime;

    Player* player;
    WorldSession* session;

    bool IsOnline() const { return session != nullptr; }
    bool IsInGame() const { return player != nullptr; }

    bool IsAllowed() const { return !IsOnline() && _allowed; }
    void SetAllowed(bool val) { _allowed = val; }

private:
    bool _allowed;
};

class ZoneInfo
{
public:
    ZoneInfo(uint8 miLevel, uint8 maLevel, uint32 posMapId, float posX, float posY, float posZ, float posO, uint32 rMask = -1)
    {
        id = 0;
        minLevel = miLevel;
        maxLevel = maLevel;
        mapId = posMapId;
        x = posX;
        y = posY;
        z = posZ;
        o = posO;
        raceMask = rMask;
    }

    uint32 id;
    uint8 minLevel;
    uint8 maxLevel;
    uint32 mapId;
    float x;
    float y;
    float z;
    float o;

    uint32 raceMask;
};

//std::string utf8_substr(const std::string& str, uint32 start, uint32 leng)
//{
//    if (leng == 0) { return ""; }
//    uint32 c, i, ix, q, min = (uint32)std::string::npos, max = (uint32)std::string::npos;
//    for (q = 0, i = 0, ix = str.length(); i < ix; i++, q++)
//    {
//        if (q == start) { min = i; }
//        if (q <= start + leng || leng == std::string::npos) { max = i; }
//
//        c = (unsigned char)str[i];
//        if (
//            //c>=0   &&
//            c <= 127) i += 0;
//        else if ((c & 0xE0) == 0xC0) i += 1;
//        else if ((c & 0xF0) == 0xE0) i += 2;
//        else if ((c & 0xF8) == 0xF0) i += 3;
//        //else if (($c & 0xFC) == 0xF8) i+=4; // 111110bb //byte 5, unnecessary in 4 byte UTF-8
//        //else if (($c & 0xFE) == 0xFC) i+=5; // 1111110b //byte 6, unnecessary in 4 byte UTF-8
//        else return "";//invalid utf8
//    }
//    if (q <= start + leng || leng == std::string::npos) { max = i; }
//    if (min == std::string::npos || max == std::string::npos) { return ""; }
//    return str.substr(min, max);
//}

#endif
