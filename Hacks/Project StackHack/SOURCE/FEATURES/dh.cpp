
#include "dh.h"
#include "../includes.h"
#include "../UTILS/Interfaces.h"
#include "../SDK/CClientEntityList.h"
#include "../SDK/IEngine.h"
#include "../SDK/CBaseEntity.h"
#include "../SDK/CGlobalVars.h"
#include "../SDK/CBaseWeapon.h"
#include "../SDK/ISurface.h"
#include "../SDK/ConVar.h"
#include "../FEATURES/Backtracking.h"
#include "../FEATURES/Resolver.h"
#include "../FEATURES/Visuals.h"
#include "../UTILS\render.h"
#include <ctime>
#include <chrono>


Hitmarker* pHitmarker = new Hitmarker();

int64 GetEpochMS() {

	int64 now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	return now;
}

void Hitmarker::Initialize() {

	INTERFACES::GameEventManager->AddListener(this, "player_hurt", false);
	INTERFACES::GameEventManager->AddListener(this, "bullet_impact", false);
}

SDK::CBaseEntity* Hitmarker::GetPlayer(int userid) {

	int pIndex = INTERFACES::Engine->GetPlayerForUserID(userid);
	SDK::CBaseEntity* pEntity = INTERFACES::ClientEntityList->GetClientEntity(pIndex);

	return pEntity;
}

void Hitmarker::Paint() {

	auto pLocal = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

	if (!SETTINGS::settings.dynamic_hitmarkers)
		return;

	if (!INTERFACES::Engine->IsConnected() || !INTERFACES::Engine->IsInGame() || !pLocal) {

		if (!pImpacts.empty())
			pImpacts.clear();

		if (!pHitmarkers.empty())
			pHitmarkers.clear();

		return;
	}

	long long pTime = GetEpochMS();

	std::vector<pHitmarkerInfo>::iterator pIter;

	for (pIter = pHitmarkers.begin(); pIter != pHitmarkers.end(); ) {

		bool pExpired = pTime > pIter->pImpact.time + 1000; // 2000
		static int pAlphaInterval = 255 / 50;

		if (pExpired)
			pIter->alpha -= pAlphaInterval;

		if (pExpired && pIter->alpha <= 0) {

			pIter = pHitmarkers.erase(pIter);
			continue;
		}

		Vector pos3D = Vector(pIter->pImpact.x, pIter->pImpact.y, pIter->pImpact.z), pos2D;

		if (!RENDER::WorldToScreen(pos3D, pos2D)) {

			++pIter;
			continue;
		}

		CColor pColor = CColor(255, 255, 255, pIter->alpha);

		int pLineSize = 6;

		RENDER::DrawLine(pos2D.x - pLineSize, pos2D.y - pLineSize, pos2D.x - (pLineSize / 4), pos2D.y - (pLineSize / 4), pColor);
		RENDER::DrawLine(pos2D.x + pLineSize, pos2D.y - pLineSize, pos2D.x + (pLineSize / 4), pos2D.y - (pLineSize / 4), pColor);
		RENDER::DrawLine(pos2D.x - pLineSize, pos2D.y + pLineSize, pos2D.x - (pLineSize / 4), pos2D.y + (pLineSize / 4), pColor);
		RENDER::DrawLine(pos2D.x + pLineSize, pos2D.y + pLineSize, pos2D.x + (pLineSize / 4), pos2D.y + (pLineSize / 4), pColor);

		++pIter;
	}
}

void Hitmarker::FireGameEvent(SDK::IGameEvent* pEvent) {

	auto pLocal = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

	if (!pEvent || !pLocal)
		return;

	if (!strcmp(pEvent->GetName(), "player_hurt"))
		PlayerHurt(pEvent);

	if (!strcmp(pEvent->GetName(), "bullet_impact"))
		BulletImpact(pEvent);
}

int Hitmarker::GetEventDebugID(void) { return 0x2A; }

void Hitmarker::PlayerHurt(SDK::IGameEvent* pEvent) {

	if (!pEvent)
		return;

	auto pLocal = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

	SDK::CBaseEntity* pAttacker = GetPlayer(pEvent->GetInt("attacker"));
	SDK::CBaseEntity* pVictim = GetPlayer(pEvent->GetInt("userid"));

	int pDamage = pEvent->GetInt("dmg_health");

	if (!pAttacker || !pVictim || pAttacker != pLocal)
		return;

	Vector enemyPosition = pVictim->GetVecOrigin();
	pImpactInfo pBestImpact;

	float pBestImpactDistance = -1;
	long long pTime = GetEpochMS();

	std::vector<pImpactInfo>::iterator pIter;

	for (pIter = pImpacts.begin(); pIter != pImpacts.end(); ) {

		if (pTime > pIter->time + 25) {

			pIter = pImpacts.erase(pIter);
			continue;
		}

		Vector position = Vector(pIter->x, pIter->y, pIter->z);
		float pDistance = position.DistTo(enemyPosition);

		if (pDistance < pBestImpactDistance || pBestImpactDistance == -1) {

			pBestImpactDistance = pDistance;
			pBestImpact = *pIter;
		}
		++pIter;
	}

	if (pBestImpactDistance == -1)
		return;

	pHitmarkerInfo pInfo;
	pInfo.pImpact = pBestImpact;
	pInfo.alpha = 255;
	pInfo.damage = pDamage;
	pHitmarkers.push_back(pInfo);
}

void Hitmarker::BulletImpact(SDK::IGameEvent* pEvent) {

	if (!pEvent)
		return;

	auto pLocal = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

	SDK::CBaseEntity* shooter = GetPlayer(pEvent->GetInt("userid"));

	if (!shooter || shooter != pLocal)
		return;

	pImpactInfo info;
	info.x = pEvent->GetFloat("x");
	info.y = pEvent->GetFloat("y");
	info.z = pEvent->GetFloat("z");

	info.time = GetEpochMS();
	pImpacts.push_back(info);
}


































































