#pragma once
#include "../includes.h"
#include "../SDK/GameEvents.h"

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
	void apply_clantag();
	void DrawInaccuracy();
	void DrawImpactBox(Vector start, Vector end);
	void DrawBox();
	void DrawBulletBox();
	void DrawSkeleton(SDK::CBaseEntity * entity, CColor color);
	void TranslateResolverRecord(SDK::CBaseEntity* entity, CColor color, Vector top, Vector pos);
	void DrawBulletTracers();
	void DrawBoneBoi(SDK::CBaseEntity * pEntity);
	void DrawBulletBeams();
	void ModulateWorld();
	void preservefeed(SDK::IGameEvent* Event);
	void ModulateSky();
	void set_hitmarker_time( float time );
	void LagCompHitbox(SDK::CBaseEntity * entity, int index);
	void LogEvents();
	void LagCompHitbox(SDK::CBaseEntity * entity);
	void Clantag();
	void GameSense1();
	void DisableFlashDuration();
	//void DrawAutowallCrosshair();
private:
	void DrawBox(SDK::CBaseEntity* entity, CColor color);
	void DrawName(SDK::CBaseEntity* entity, CColor color, int index, Vector pos, Vector top);
	void DrawWeapon(SDK::CBaseEntity * entity, CColor color, int index, Vector pos, Vector top);
	void DrawHealth(SDK::CBaseEntity * entity, CColor color, CColor dormant, Vector pos, Vector top);
	void BombPlanted(SDK::CBaseEntity * entity);
	void DrawDropped(SDK::CBaseEntity * entity);
	void DrawAmmo(SDK::CBaseEntity * entity, CColor color, CColor dormant, Vector pos, Vector top);
	float resolve_distance(Vector src, Vector dest);
	void DrawDistance(SDK::CBaseEntity * entity, CColor color, Vector pos, Vector top);
	void DrawInfo(SDK::CBaseEntity * entity, CColor color, CColor alt, Vector pos, Vector top);
	void BoxAndText(SDK::CBaseEntity * entity, std::string text);
	void DrawSkeleton(SDK::CBaseEntity * pEntity);
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