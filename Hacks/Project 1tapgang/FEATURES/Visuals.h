#pragma once

namespace SDK
{
	class CUserCmd;
	class CBaseEntity;
}

class CVisuals
{
public:
	void Drawmodels();
	void CustomModels(SDK::CBaseEntity* entity);
	void Draw();
	void ClientDraw();
	void DrawInaccuracy();
	void DrawBulletBeams();
	void ModulateWorld();
	void ModulateSky();
	void set_hitmarker_time(float time);
	void LogEvents();
	void clan_tag();
	void autowall_crosshair();
	void chat_spam();
	void night_mode();
	void draw_circle_around_local();
	void static_crosshair();
	void radar(SDK::CBaseEntity * entity);
	void lag_comp_hitbox(SDK::CBaseEntity * entity, int index);
private:
	void DrawBox(SDK::CBaseEntity* entity, CColor color, Vector pos, Vector top);
	void ammo_bar(SDK::CBaseEntity * entity, CColor color, CColor dormant, Vector pos, Vector top);
	void DrawName(SDK::CBaseEntity* entity, CColor color, int index, Vector pos, Vector top);
	void DrawWeapon(SDK::CBaseEntity* entity, CColor color, int index);
	void fov_arrows(SDK::CBaseEntity * entity, CColor color);
	void DrawHealth(SDK::CBaseEntity * entity, CColor color, CColor dormant, Vector pos, Vector top);
	void BombPlanted(SDK::CBaseEntity * entity);
	void DrawDropped(SDK::CBaseEntity * entity);
	float resolve_distance(Vector src, Vector dest);
	void DrawDistance(SDK::CBaseEntity * entity, CColor color, Vector pos, Vector top);
	void DrawInfo(SDK::CBaseEntity * entity, CColor color, CColor alt, Vector pos, Vector top);
	void DrawInaccuracy1();
	void DrawCrosshair();
	void DrawIndicator();
	void DrawHitmarker();
	void DrawBorderLines();
public:
	std::vector<std::pair<int, float>>				Entities;
	std::deque<UTILS::BulletImpact_t>				Impacts;
};

extern CVisuals* visuals;