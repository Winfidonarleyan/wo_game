#include "ScriptMgr.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "DBCStores.h"
#include "Mail.h"
#include "Item.h"
#include "DatabaseEnv.h"

class played_time_rewards_WorldScript : WorldScript
{
public:
    played_time_rewards_WorldScript() : WorldScript("played_time_rewards") { }

    void OnUpdate(uint32 diff) override
    {
        _events.Update(diff);

        if (_events.ExecuteEvent())
        {
            auto &players = ObjectAccessor::GetPlayers();

            for (auto itr = players.begin(); itr != players.end(); itr++)
            {
                uint32 played = itr->second->GetTotalPlayedTime();

                if (IsDay(played, 350))
                {
                    if (CharTitlesEntry const* titleInfo = sCharTitlesStore.LookupEntry(141))
                        itr->second->SetTitle(titleInfo);

                    SendMail(itr->second, 34493);
                }
                else if (IsDay(played, 200))
                    SendMail(itr->second, 32458);
                else if (IsDay(played, 100))
                    SendMail(itr->second, 49287);
                else if (IsDay(played, 40))
                    SendMail(itr->second, 13335);
                else if (IsDay(played, 20))
                    SendMail(itr->second, 38050);
                else if (IsDay(played, 10))
                    SendMail(itr->second, 49284);
                else if (IsDay(played, 5))
                    SendMail(itr->second, 23713);
            }

            _events.ScheduleEvent(1, updateInterval * IN_MILLISECONDS);
        }
    }

    void OnStartup() override
    {
        _events.ScheduleEvent(1, updateInterval * IN_MILLISECONDS);
    }

    bool IsDay(uint32 played, uint32 day)
    {
        return played >= (day * DAY) && played <= ((day * DAY) + updateInterval + 10);
    }

    void SendMail(Player* player, uint32 itemId)
    {
        MailSender sender(MAIL_NORMAL, 0, MAIL_STATIONERY_GM);
        MailDraft draft("Played time reward", "Here is your reward for played time");
        SQLTransaction trans = CharacterDatabase.BeginTransaction();

        if (Item* item = Item::CreateItem(itemId, 1, 0))
        {
            item->SaveToDB(trans);              // Save to prevent being lost at next mail load. If send fails, the item will be deleted.
            draft.AddItem(item);
        }

        draft.SendMailTo(trans, MailReceiver(player, player->GetGUID().GetCounter()), sender);
        CharacterDatabase.CommitTransaction(trans);
    }

private:
    const uint32 updateInterval = 10 * MINUTE;

    EventMap _events;
};

void AddSC_PlayedTimeRewards()
{
    new played_time_rewards_WorldScript();
}