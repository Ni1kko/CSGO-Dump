#pragma once

namespace SDK
{
	class CUserCmd;
	class CBaseEntity;
}

class CVisuals
{
public:
	struct damage_indicator_t {
		int dmg;
		bool initializes;
		float earse_time;
		float last_update;
		SDK::CBaseEntity * player;
		Vector Position;
	};
	std::vector<damage_indicator_t> dmg_indicator;
	bool NightmodeComplete = false;
	void DrawDamageIndicator();

	void Draw();
	void ClientDraw();
	void DrawInaccuracy();
	void DrawInaccuracy2();
	void AsusProps();
	void DrawIndicator();
	void otherindicators();
	void DrawBulletBeams();
	void GameSense();
	void LogEvents();
	void watermark();
	void DoFSN();
	void ModulateWorld();
	void ModulateSky();
	void set_hitmarker_time( float time );
	void DrawMisc();
	void DrawLBYBar(SDK::CBaseEntity * entity, CColor color);
	void LagCompHitbox(SDK::CBaseEntity* entity, int index);
	void ApplyNightmode();

private:
	void DrawLBYTimer(SDK::CBaseEntity * entity, CColor color, CColor dormant);
	void DrawBox(SDK::CBaseEntity* entity, CColor color, Vector pos, Vector top);
	void DrawName(SDK::CBaseEntity* entity, CColor color, int index, Vector pos, Vector top);
	void DrawWeapon(SDK::CBaseEntity * entity, CColor color, int index, Vector pos, Vector top);
	void DrawWeapon2(SDK::CBaseEntity * entity, CColor color, int index, Vector pos, Vector top);
	void DrawHealth(SDK::CBaseEntity * entity, CColor color, CColor dormant, Vector pos, Vector top);
	void DrawAmmo(SDK::CBaseEntity * entity, CColor color, CColor dormant, Vector pos, Vector top);
	float resolve_distance(Vector src, Vector dest);
	void DrawDistance(SDK::CBaseEntity * entity, CColor color, Vector pos, Vector top);
	void DrawInfo(SDK::CBaseEntity * entity, CColor color, CColor alt, Vector pos, Vector top);
	void DrawFovArrows(SDK::CBaseEntity* entity, CColor color);
	void DrawCrosshair();
	void DrawHitmarker();
	void AntiAimLines();
	void DrawBorderLines();
	void DrawSnaplines();
	void PaintSkeleton(SDK::CBaseEntity * entity, CColor color, int alpha);

	void viewmodelxyz();

public:
	std::vector<std::pair<int, float>>				Entities;
	std::deque<UTILS::BulletImpact_t>				Impacts;
};

extern CVisuals* visuals;
