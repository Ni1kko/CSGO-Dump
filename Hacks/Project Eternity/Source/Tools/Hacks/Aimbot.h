#pragma once
#include "../../stdafx.h"
#include "../Utils/Hitbox.h"
#include "../Menu/Menu.h"
#include "../Utils/Playerlist.h"
#include "../Utils/HitboxLists.h"
#include "../../Singleton.h"
#include "../Utils/LocalInfo.h"
#include "../../AWall.h"
#define TIME_TO_TICKS(dt) ((int)( 0.5f + (float)(dt) / Interfaces.pGlobalVars->interval_per_tick))
#define TICKS_TO_TIME(t) (Interfaces.pGlobalVars->interval_per_tick * (t) )

class CAim : public Singleton<CAim> {
private:
	std::vector< CBaseEntity* > entities;
	bool bFindGoodTarget = false;

	std::vector<Vector> GetPointsToScan(CBaseEntity* pBaseEntity, int iHitbox, matrix3x4 BoneMatrix[128])
	{
		std::vector<Vector> vPoints;

		auto VectorTransform_Wrapper = [](const Vector& in1, const matrix3x4 &in2, Vector &out)
		{
			auto VectorTransform = [](const float *in1, const matrix3x4& in2, float *out)
			{
				auto DotProducts = [](const float *v1, const float *v2)
				{
					return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
				};
				out[0] = DotProducts(in1, in2[0]) + in2[0][3];
				out[1] = DotProducts(in1, in2[1]) + in2[1][3];
				out[2] = DotProducts(in1, in2[2]) + in2[2][3];
			};
			VectorTransform(&in1.x, in2, &out.x);
		};
		if (!pBaseEntity)
			return vPoints;


		studiohdr_t* pStudioModel = Interfaces.g_pModelInfo->GetStudioModel(pBaseEntity->GetModel());
		mstudiohitboxset_t* set = pStudioModel->GetHitboxSet(0);

		if (!set)
			return vPoints;

		mstudiobbox_t* untransformedBox = set->pHitbox(iHitbox);
		if (!untransformedBox)
			return vPoints;

		//Vector vecMin = { 0, 0, 0 };
		//VectorTransform_Wrapper(untransformedBox->bbmin, BoneMatrix[untransformedBox->bone], vecMin);

		//Vector vecMax = { 0, 0, 0 };
		//VectorTransform_Wrapper(untransformedBox->bbmax, BoneMatrix[untransformedBox->bone], vecMax);

		float mod = untransformedBox->radius != -1.f ? untransformedBox->radius : 0.f;
		Vector max;
		Vector min;

		VectorTransform_Wrapper(untransformedBox->bbmax + Vector(mod, mod, mod), BoneMatrix[untransformedBox->bone], max);
		VectorTransform_Wrapper(untransformedBox->bbmin - Vector(mod, mod, mod), BoneMatrix[untransformedBox->bone], min);
		auto center = (min + max) * 0.5f;
		Vector curAngles = g_Math.CalcAngle(center, Hacks.LocalPlayer->GetEyePosition());
		Vector forward;
		g_Math.angleVectors(curAngles, forward);
		Vector Right = forward.Cross(Vector(0, 0, 1));
		Vector Left = Vector(-Right.x, -Right.y, Right.z);

		Vector Top = Vector(0, 0, 1);
		Vector Bot = Vector(0, 0, -1);
		float ps = 0.75f;

		if (Vars.Ragebot.Hitscan.Multipoint)
		{
			switch (iHitbox) {
			case (int)CSGOHitboxID::Head:
				for (auto i = 0; i < 4; ++i)
				{
					vPoints.emplace_back(center);
				}
				vPoints[1] += Top * (untransformedBox->radius * ps);
				vPoints[2] += Right * (untransformedBox->radius * ps);
				vPoints[3] += Left * (untransformedBox->radius * ps);
				break;

			default:

				for (auto i = 0; i < 3; ++i)
				{
					vPoints.emplace_back(center);
				}
				vPoints[1] += Right * (untransformedBox->radius * ps);
				vPoints[2] += Left * (untransformedBox->radius * ps);
				break;
			}
		}
		else
			vPoints.emplace_back(center);

		return vPoints;
	}

	int GetPriority()
	{
		auto weapon = Hacks.LocalWeapon;
		if (!weapon)
			return -1;

		if (!Hacks.LocalPlayer->isAlive())
			return -1;

		if (*weapon->GetItemDefinitionIndex() == WEAPON_TASER || *weapon->GetItemDefinitionIndex() == WEAPON_AWP)
			return (int)CSGOHitboxID::Pelvis;

		return Vars.Ragebot.Hitscan.PriorityHitbox;
	}

	std::vector <int> GetHitscanPoints()
	{
		std::vector< int > hitboxes;

		switch (Vars.Ragebot.Hitscan.Multibox)
		{
		case 1:
			hitboxes.push_back((int)CSGOHitboxID::Head);
			hitboxes.push_back((int)CSGOHitboxID::Neck);
			hitboxes.push_back((int)CSGOHitboxID::Chest);
			hitboxes.push_back((int)CSGOHitboxID::Pelvis);
			hitboxes.push_back((int)CSGOHitboxID::Stomach);
			break;
		case 2:
			hitboxes.push_back((int)CSGOHitboxID::Head);
			hitboxes.push_back((int)CSGOHitboxID::Neck);
			hitboxes.push_back((int)CSGOHitboxID::Chest);
			hitboxes.push_back((int)CSGOHitboxID::Pelvis);
			hitboxes.push_back((int)CSGOHitboxID::Stomach);
			hitboxes.push_back((int)CSGOHitboxID::LowerChest);
			hitboxes.push_back((int)CSGOHitboxID::Chest);
			hitboxes.push_back((int)CSGOHitboxID::UpperChest);
			hitboxes.push_back((int)CSGOHitboxID::RightFoot);
			hitboxes.push_back((int)CSGOHitboxID::LeftFoot);
			break;
		}

		return hitboxes;
	}

	bool CanShoot()
	{
		if (!Interfaces.pGlobalVars)
			return false;

		if (!Hacks.LocalPlayer || !Hacks.LocalWeapon)
			return false;

		auto flServerTime = (float)Hacks.LocalPlayer->GetTickBase() * Interfaces.pGlobalVars->interval_per_tick;
		auto flNextPrimaryAttack = Hacks.LocalWeapon->NextPrimaryAttack();

		return(!(flNextPrimaryAttack > flServerTime));
	}

public:
	void Run() {
		if (!Vars.Ragebot.Enable)
			return;
		if (!Hacks.LocalPlayer)
			return;
		if (!Hacks.LocalPlayer->isAlive())
			return;
		if (Hacks.LocalWeapon->IsMiscWeapon() || !Hacks.LocalWeapon->HasAmmo())
			return;
		if (!CanShoot())
			return;

		auto bSortHP = [](CBaseEntity* ent1, CBaseEntity* ent2) {
			return ent1->GetHealth() < ent2->GetHealth();
		};

		Vector angles;
		Interfaces.pEngine->GetViewAngles(angles);

		for (int i = 1; i < 65; ++i) {
			auto Entity = Interfaces.pEntList->GetClientEntity(i);
			if (!Entity->IsValid(true))
				continue;
			entities.push_back(Entity);
		}

		std::sort(entities.begin(), entities.end(), bSortHP);
		Vector aimAng, haimAng;

		for (auto Ent : entities) {
			if (bFindGoodTarget)
				break;

			matrix3x4 matrix[128];
			if (!Ent->SetupBones(matrix, 128, 0x7FF00, Interfaces.pGlobalVars->curtime))
				continue;

			auto AimPoints = GetPointsToScan(Ent, GetPriority(), matrix);
			for (auto hAimPoint : AimPoints)
			{
				if (bFindGoodTarget)
					break;

				if (!hAimPoint.IsValid())
					break;

				Misc::CalcAngle(Hacks.LocalPlayer->GetEyePosition(), hAimPoint, aimAng);

				float damage = Autowall::GetDamage(hAimPoint);

				if (damage > Vars.Ragebot.Accuracy.Mindamage)
				{
					if (Vars.Ragebot.AutoFire)
					{
						if (Hacks.LocalWeapon->hitchance() > Vars.Ragebot.Accuracy.Hitchance)
						{
							Hacks.CurrentCmd->buttons |= IN_ATTACK;
						}
					}
					if (Hacks.CurrentCmd->buttons & IN_ATTACK)
						Hacks.CurrentCmd->viewangles = aimAng;

					bFindGoodTarget = true;
				}
			}

			if (Vars.Ragebot.Hitscan.Multibox > 0) {
				for (auto hitboxes : GetHitscanPoints()) {
					if (bFindGoodTarget)
						break;

					auto AimPoints = GetPointsToScan(Ent, hitboxes, matrix);
					for (auto AimPoint : AimPoints)
					{
						if (bFindGoodTarget)
							break;

						if (!AimPoint.IsValid())
							break;

						Misc::CalcAngle(Hacks.LocalPlayer->GetEyePosition(), AimPoint, haimAng);

						float damage = Autowall::GetDamage(AimPoint);

						if (damage > Vars.Ragebot.Accuracy.Mindamage)
						{
							if (Vars.Ragebot.AutoFire)
							{
								if (Hacks.LocalWeapon->hitchance() > Vars.Ragebot.Accuracy.Hitchance)
								{
									Hacks.CurrentCmd->buttons |= IN_ATTACK;
								}
							}
							if (Hacks.CurrentCmd->buttons & IN_ATTACK)
								Hacks.CurrentCmd->viewangles = haimAng;

							bFindGoodTarget = true;
						}
					}
				}
			}
		}

		bFindGoodTarget = false;
		entities.clear();
	}
	void AfterAimShit() {
		if (Misc::Shooting())
		{
			if (!Vars.Ragebot.Silent && Vars.Ragebot.Enable)
				Interfaces.pEngine->SetViewAngles(Hacks.CurrentCmd->viewangles);
			if (Vars.Ragebot.Accuracy.RemoveRecoil)
				Hacks.CurrentCmd->viewangles -= LocalInfo.PunchAns * (vec_t)2.f;
		}
		if (*Hacks.LocalWeapon->GetItemDefinitionIndex() == WEAPON_REVOLVER && Vars.Ragebot.Enable)
		{
			if (Hacks.LocalWeapon->GetPostponeFireReadyTime() - Misc::GetServerTime() > 0.05)
				Hacks.CurrentCmd->buttons |= IN_ATTACK;
		}
	}
};