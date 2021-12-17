#pragma once

#include "../other/singleton.hpp"

#include "../render/render.hpp"
#include "../../helpers/math.hpp"
#include "../../sdk/structs.hpp"

#include <map>

struct SoundInfo_t {
	int guid;
	float soundTime;
	float alpha;
	Vector soundPos;
};

struct DamageIndicator_t {
	int iDamage;
	bool bInitialized;
	float flEraseTime;
	float flLastUpdate;
	C_BasePlayer* Player;
	Vector Position;
};

class Visuals : public IGameEventListener2, public Singleton<Visuals> {
	friend class Singleton<Visuals>;

	CRITICAL_SECTION cs;

	float flHurtTime;

	Visuals();
	~Visuals();
public:

	std::vector<DamageIndicator_t> DamageIndicator;

	class Player {
	public:
		struct {
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
		void DrawFlags();
	};

	std::map< int, std::vector< SoundInfo_t > > m_Sounds;
	CUtlVector<SndInfo_t> sounds;

	virtual void FireGameEvent(IGameEvent *event);
	virtual int  GetEventDebugID(void);

	void DrawLine(float x1, float y1, float x2, float y2, Color color, float size = 1.f);
	void DrawBox(float x1, float y1, float x2, float y2, Color clr, float size = 1.f);
	void DrawBoxEdges(float x1, float y1, float x2, float y2, Color clr, float edge_size, float size = 1.f);
	void NoSmoke();
	void RenderCrosshair();
	void RenderWeapon(C_BaseCombatWeapon* ent);
	void RenderDefuseKit(C_BaseEntity* ent);
	void RenderPlantedC4(C_BaseEntity* ent);
	void RenderItemEsp(C_BaseEntity* ent);
	void DrawHitmarker();
	void RenderSpectatorList();
	void ThirdPerson();
	void DrawFOV();
	void RenderSounds();
	void UpdateSounds();
	void Radar();
public:
	void AddToDrawList();
	void Render();
};
