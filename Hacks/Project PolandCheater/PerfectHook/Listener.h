#pragma once
#include "singleton.hpp"
#include "MiscClasses.h"
#include "Interfaces.h"
class item_purchase
    : public singleton<item_purchase>
{
    class item_purchase_listener
        : public IGameEventListener2
    {
    public:
        void start()
        {
            g_EventManager->AddListener(this, "item_purchase", false);
        }
        void stop()
        {
            g_EventManager->RemoveListener(this);
        }
        void FireGameEvent(IGameEvent *event) override
        {
            singleton()->on_fire_event(event);
        }
        int GetEventDebugID(void) override
        {
            return 42 /*0x2A*/;
        }
    };
public:
    static item_purchase* singleton()
    {
        static item_purchase* instance = new item_purchase;
        return instance;
    }

    void initialize()
    {
        listener.start();
    }

    void remove()
    {
        listener.stop();
    }

    void on_fire_event(IGameEvent* event)
    {
        C_BaseEntity* local = g_EntityList->GetClientEntity(g_Engine->GetLocalPlayer());
        if (!strcmp(event->GetName(), "item_purchase"))
        {
            auto buyer = event->GetInt("userid");
            std::string gun = event->GetString("weapon");

            if (strstr(gun.c_str(), "molotov")
                || strstr(gun.c_str(), "nade")
                || strstr(gun.c_str(), "kevlar")
                || strstr(gun.c_str(), "decoy")
                || strstr(gun.c_str(), "suit")
                || strstr(gun.c_str(), "flash")
                || strstr(gun.c_str(), "vest")
                || strstr(gun.c_str(), "cutter")
                || strstr(gun.c_str(), "defuse")
                )  return;
            auto player_index = g_Engine->GetPlayerForUserID(buyer);
            C_BaseEntity* player = (C_BaseEntity*)g_EntityList->GetClientEntity(player_index);
            player_info_t pinfo;
            
            if (player && local &&  player->GetTeamNum() != local->GetTeamNum() && g_Engine->GetPlayerInfo(player_index, &pinfo))
            {
                if (g_ChatElement)
                {
                    gun.erase(gun.find("weapon_"), 7);
                    g_ChatElement->ChatPrintf(0, 0, " ""\x04""%s bought %s\n", pinfo.name, gun.c_str());
                }
            }
        }
    }
private:
    item_purchase_listener  listener;
};
