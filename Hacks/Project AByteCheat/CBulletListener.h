#pragma once

#include "Interfaces.h"


#include "Entities.h"
#include "Menu.h"

#include "Hooks.h"
#include "Utilities.h"
#include "RenderManager.h"

class trace_info
{
public:
	trace_info(Vector starts, Vector positions, float times, int userids)
	{
		this->start = starts;
		this->position = positions;
		this->time = times;
		this->userid = userids;
	}

	Vector position;
	Vector start;
	float time;
	int userid;
};

extern std::vector<trace_info> trace_logs;
#define g_pGameEventManager interfaces::event_manager

class CBulletListener
{
	class BulletListener : IGameEventListener2
	{
	public:
		void start()
		{
			if (!g_pGameEventManager->AddListener(this, "bullet_impact", false))
			{
				throw std::exception("Failed to register the event");
			}
		}
		void stop()
		{
			g_pGameEventManager->RemoveListener(this);
		}
		void FireGameEvent(IGameEvent* event) override
		{
			CBulletListener::singleton()->OnFireEvent(event);
		}
		int GetEventDebugID(void) override
		{
			return EVENT_DEBUG_ID_INIT /*0x2A*/;
		}
	};
public:
	static CBulletListener* singleton()
	{
		static CBulletListener* instance = new CBulletListener();
		return instance;
	}

	void init()
	{
		_listener.start();
	}

	void OnFireEvent(IGameEvent* event)
	{
		if (!strcmp(event->GetName(), "bullet_impact"))
		{
			auto shooter = interfaces::ent_list->get_client_entity(interfaces::engine->GetPlayerForUserID(event->GetInt("userid")));

			Vector position(event->GetFloat("x"), event->GetFloat("y"), event->GetFloat("z"));
			if (shooter)
				trace_logs.push_back(trace_info(shooter->GetEyePosition(), position, interfaces::globals->curtime, event->GetInt("userid")));

		}

	}

	void OnStudioRender()
	{
		if (options::menu.visuals_tab.BulletTracers.GetState())
		{
			if (trace_logs.size() > 0) {
				for (unsigned int i = 0; i < trace_logs.size(); i++) {
					IClientEntity* LocalPlayer = (IClientEntity*)interfaces::ent_list->get_client_entity(interfaces::engine->GetLocalPlayer());
					auto *shooter = interfaces::ent_list->get_client_entity(interfaces::engine->GetPlayerForUserID(trace_logs[i].userid));

					if (!shooter) return;

					Color color;
					if (shooter == LocalPlayer)
						color = Color(2, 255, 44, 255);
					else
						color = Color(255, 0, 0, 255);

				//	Hooks::DrawBeamd(trace_logs[i].start, trace_logs[i].position, color);

					if (trace_logs.size())
						trace_logs.erase(trace_logs.begin() + i);
				}
			}
		}
	}

private:
	BulletListener _listener;

	class cbullet_tracer_info
	{
	public:
		cbullet_tracer_info(Vector src, Vector dst, float time, Color color)
		{
			this->src = src;
			this->dst = dst;
			this->time = time;
			this->color = color;
		}

		Vector src, dst;
		float time;
		Color color;
	};

	std::vector<cbullet_tracer_info> logs;
};
