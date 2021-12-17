#pragma once
#include "../MISC/includes.h"
#include "../SDK/GameEvents.h"
#include "../utils/vector2D.h"
#include "../SDK/CGlobalVars.h"
#include "../SDK/CClientEntityList.h"
#include "../SDK/ISurface.h"
#include "../SDK/IViewRenderBeams.h"
#include "../SDK/IEngine.h"


/* definitions */
#define create_event_listener(class_name)\
class class_name : public SDK::IGameEventListener\
{\
public:\
	~class_name() { INTERFACES::GameEventManager->RemoveListener(this); }\
\
	virtual void FireGameEvent(SDK::IGameEvent* game_event);\
};\

/* sound info */
class sound_info
{
public:
	sound_info(Vector positions, float times, int userids)
	{
		this->position = positions;
		this->time = times;
		this->userid = userids;
	}

	Vector position;
	float time;
	int userid;
};

/* functions */
class soundesp
{
public:
	void draw();
	void draw_circle(CColor color, Vector position);
	void listener();
	create_event_listener(player_hurt_event);
	create_event_listener(player_footstep_event);
};

/* extern sound esp */
extern soundesp g_soundesp;