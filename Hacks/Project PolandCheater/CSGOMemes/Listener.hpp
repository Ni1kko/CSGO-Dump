#pragma once
#include "singleton.hpp"
#include "features/visuals.hpp"
#include "ValveSDK/csgostructs.hpp"

bool mapchanged = true;

class item_purchase
    : public singleton<item_purchase>
{
    class item_purchase_listener
        : public IGameEventListener2
    {
    public:
        void start()
        {
            g_GameEvents->AddListener(this, "item_purchase", false);
        }
        void stop()
        {
            g_GameEvents->RemoveListener(this);
        }
        void FireGameEvent(IGameEvent *event) override
        {
            singleton()->on_fire_event(event);
        }
        int GetEventDebugID(void) override
        {
            return EVENT_DEBUG_ID_INIT /*0x2A*/;
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
        auto local = C_BasePlayer::get_local_player();
        if (!strcmp(event->GetName(), "item_purchase")) 
        {
            auto _buyer = event->GetInt("userid");
            auto _gun = event->GetString("weapon");
            auto _player_index = g_EngineClient->GetPlayerForUserID(_buyer);
            C_BasePlayer* player = (C_BasePlayer*)g_EntityList->GetClientEntity(_player_index);
            player_info_t pinfo;
            if (player && player->m_iTeamNum() != local->m_iTeamNum() && g_EngineClient->GetPlayerInfo(_player_index, &pinfo))
            {
                utils::console_print("%s bought %s\n", pinfo.szName, _gun);
            }
        }
    }
private:
    item_purchase_listener  listener;
};


class round_start
    : public singleton<round_start>
{
    class round_start_listener
        : public IGameEventListener2
    {
    public:
        void start()
        {
            g_GameEvents->AddListener(this, "round_start", false);
        }
        void stop()
        {
            g_GameEvents->RemoveListener(this);
        }
        void FireGameEvent(IGameEvent *event) override
        {
            singleton()->on_fire_event(event);
        }
        int GetEventDebugID(void) override
        {
            return EVENT_DEBUG_ID_INIT /*0x2A*/;
        }
    };
public:
    static round_start* singleton()
    {
        static round_start* instance = new round_start;
        return instance;
    }

    void initialize()
    {
        _listener.start();
    }

    void remove()
    {
        _listener.stop();
    }

    void on_fire_event(IGameEvent* event)
    {
        if (!strcmp(event->GetName(), "round_start"))
        {
            system("cls");
        }
    }
private:
    round_start_listener  _listener;
};

