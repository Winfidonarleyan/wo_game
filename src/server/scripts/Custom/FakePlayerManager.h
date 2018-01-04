#ifndef _FAKE_PLAYER_MGR_H_
#define _FAKE_PLAYER_MGR_H_

#include "fake_players.h"
#include "Player.h"
#include "QueryCallbackProcessor.h"
#include <boost/thread/locks.hpp>
#include <boost/thread/shared_mutex.hpp>

class FakePlayerContainer
{
public:
    typedef std::unordered_map<uint32 /* accountId */, FakePlayer* /* player */> PlayerMap;
    typedef std::vector<FakePlayer*> PlayerVector;

    void Add(FakePlayer* player, bool lock = true);

    void Get(uint32 accountId, std::function<void(FakePlayer*)> func);
    PlayerMap &GetAll() { return _all; }

    bool Empty();

    void Reserve(size_t size)
    {
        _all.reserve(size);
    }

    size_t Size();

    void Do(std::function<void(PlayerMap&)> func);
    
    PlayerVector GetRandom(uint32 count, std::function<bool(FakePlayer*)> predicate) const;

private:
    PlayerMap _all;

    boost::shared_mutex *GetLock();
};

class FakePlayerMgr
{
public:
    static FakePlayerMgr* instance();

    FakePlayerMgr();

    // Load methods
    void LoadNames();
    void LoadZones();
    void LoadPlayers();

    // Get containers

    std::vector<std::string> &GetUnusedNames() { return _unusedNames; }
    std::vector<std::string> &GetUsedNames() { return _usedNames; }
    const std::unordered_map<uint32, ZoneInfo*> &GetZones() const { return _zones; }
    std::vector<uint8> &GetAllowedClasses(uint8 race, bool allowHeroic) const;
    std::vector<ZoneInfo*> GetZones(uint8 level, uint32 raceMask) const;

    // Get single item
    std::string GetRandomName(bool setAsUsed);

    ZoneInfo* GetZone(uint32 id) const;
    ZoneInfo* GetRandomZone(uint8 level, uint32 raceMask) const;

    uint8 GetRandomRace() const;
    uint8 GetRandomClass(uint8 race, bool allowHeroic) const;

    void Create(std::string& charName, uint8 race, uint8 Class, uint8 gender, std::function<void(FakePlayer*)> callback);
    void Login(uint32 accountId, std::function<void(FakePlayer*)> callback);
    void Login(FakePlayer* player, std::function<void(FakePlayer*)> callback);

    uint32 GetEventTime(FakePlayerEvents eventId, Player* player) const;
    uint32 GetAllowedLoginTime() const;

    void Update(uint32 diff);

    void OnLogin(Player* player);
    void OnLogout(Player* player);

    void SetHourValues(std::vector<uint32> &values)
    {
        _hourValues = values;
    }

private:
    FakePlayerContainer _players;
    uint32 _requiredCount;
    std::vector<uint32> _hourValues;

    std::vector<std::string> _unusedNames;
    std::vector<std::string> _usedNames;
    std::unordered_map<uint32, ZoneInfo*> _zones;

    EventMap _events;

    QueryCallbackProcessor _queryProcessor;
};

#define sFakePlayerMgr FakePlayerMgr::instance()

#endif
