#pragma once
#include "../../valve_sdk/sdk.hpp"
#include "../../features/misc/misc.hpp"
#include "../../features/event_logger/logger.hpp"


// credits: @Nixer1337
class EventsManager
{
	class Listener
		: public IGameEventListener2
	{
	public:
		void Start() {
			g_GameEvents->AddListener(this, "begin_new_match", false);
			g_GameEvents->AddListener(this, "client_disconnect", false);
			g_GameEvents->AddListener(this, "round_start", false);
			g_GameEvents->AddListener(this, "game_newmap", false);
			g_GameEvents->AddListener(this, "player_hurt", false);
			g_GameEvents->AddListener(this, "player_death", false);
			g_GameEvents->AddListener(this, "bullet_impact", false);
			g_GameEvents->AddListener(this, "bomb_planted", false);
			g_GameEvents->AddListener(this, "item_purchase", false);
			g_GameEvents->AddListener(this, "weapon_fire", false);
			g_GameEvents->AddListener(this, "inferno_startburn", false);
			g_GameEvents->AddListener(this, "inferno_expire", false);
			g_GameEvents->AddListener(this, "game_newmap", false);
			g_GameEvents->AddListener(this, "bomb_beginplant", false);
			g_GameEvents->AddListener(this, "bomb_planted", false);
			g_GameEvents->AddListener(this, "bomb_begindefuse", false);
		}
		void Stop() {
			g_GameEvents->RemoveListener(this);
		}
		void FireGameEvent(IGameEvent* event) override {
			g_Misc->fire_event(event);
			notify::notify_events(event);
		}
		int GetEventDebugID(void) override {
			return EVENT_DEBUG_ID_INIT;
		}
	};
public:
	void Init() {
		_listener.Start();
	}
	void Unload() {
		_listener.Stop();
	}
private:
	Listener _listener;
};

EventsManager g_CustomEventsManager;