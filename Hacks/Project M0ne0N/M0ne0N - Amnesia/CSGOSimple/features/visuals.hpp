#pragma once

#include "../singleton.hpp"

#include "../render.hpp"
#include "../helpers/math.hpp"
#include "../valve_sdk/csgostructs.hpp"

#include <map>

struct SoundInfo_t {
	int guid;
	float soundTime;
	float alpha;
	Vector soundPos;
};

class Visuals : public IGameEventListener2, public Singleton<Visuals>
{
	friend class Singleton<Visuals>;

	CRITICAL_SECTION cs;

	float flHurtTime;

	Visuals();
	~Visuals();
public:
	class Player
	{
	public:
		struct
		{
			C_BasePlayer* pl;
			bool          is_enemy;
			bool          is_visible;
			Color         clr;
			Vector        head_pos;
			Vector        feet_pos;
			RECT          bbox;
		} ctx;

		bool Begin(C_BasePlayer * pl);
		void RenderBox();
		void RenderName();
		void RenderWeaponName();
		void RenderHealth();
		void RenderArmour();
		void RenderSkeleton(C_BaseEntity* ent);
		void RenderBacktrack();
	};

	virtual void FireGameEvent(IGameEvent *event);
	virtual int  GetEventDebugID(void);

	void DrawLine(float x1, float y1, float x2, float y2, Color color, float size = 1.f);
	void DrawBox(float x1, float y1, float x2, float y2, Color clr, float size = 1.f);
	void DrawBoxEdges(float x1, float y1, float x2, float y2, Color clr, float edge_size, float size = 1.f);

	std::map< int, std::vector< SoundInfo_t > > m_Sounds;

	void RenderSounds();
	void UpdateSounds();

	void RenderWeapon(C_BaseCombatWeapon* ent);
	void RenderDefuseKit(C_BaseEntity* ent);
	void RenderPlantedC4(C_BaseEntity* ent);
	void RenderItemEsp(C_BaseEntity* ent);
	void DrawHitmarker();
	void RenderSpectatorList();
	void ThirdPerson();
	void DrawFOV();
public:
	void AddToDrawList();
	void Render();
};
