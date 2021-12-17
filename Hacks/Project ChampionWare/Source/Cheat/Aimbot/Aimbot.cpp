#include "Aimbot.h"

using namespace Client;

#pragma warning(disable:4244)
//[junk_enable /]
//[enc_string_enable /]
byte tt_ct_best_hit_1[14] =
{
	//All Spots
	HITBOX_HEAD ,
	HITBOX_NECK ,
	HITBOX_BODY ,
	HITBOX_THORAX ,
	HITBOX_CHEST ,
	HITBOX_RIGHT_THIGH ,
	HITBOX_LEFT_THIGH ,
	HITBOX_RIGHT_HAND ,
	HITBOX_LEFT_HAND ,
	HITBOX_RIGHT_UPPER_ARM ,
	HITBOX_RIGHT_FOREARM ,
	HITBOX_LEFT_UPPER_ARM ,
	HITBOX_LEFT_FOREARM
};

byte tt_ct_best_hit_2[13] =
{
	//No Headshot
	HITBOX_NECK ,
	HITBOX_BODY ,
	HITBOX_THORAX ,
	HITBOX_CHEST ,
	HITBOX_RIGHT_THIGH ,
	HITBOX_LEFT_THIGH ,
	HITBOX_RIGHT_HAND ,
	HITBOX_LEFT_HAND ,
	HITBOX_RIGHT_UPPER_ARM ,
	HITBOX_RIGHT_FOREARM ,
	HITBOX_LEFT_UPPER_ARM ,
	HITBOX_LEFT_FOREARM
};

byte tt_ct_best_hit_3[6] =
{
	//No Arms/Legs
	HITBOX_HEAD ,
	HITBOX_NECK ,
	HITBOX_BODY ,
	HITBOX_THORAX ,
	HITBOX_CHEST
};

byte tt_ct_best_hit_4[5] =
{
	//No Arms/Legs/Neck
	HITBOX_HEAD ,
	HITBOX_BODY ,
	HITBOX_THORAX ,
	HITBOX_CHEST
};

CAimbot::CAimbot()
{
	m_pLocal = nullptr;

	m_iBestPreTarget = -1;

	m_iBestTarget = -1;
	m_iBestHitbox = -1;

	m_bClamp = false;
	m_bAttack = false;
	m_bAimShot = false;
	m_bTargetFov = false;
	m_bTargetChange = false;
	m_bAutoPistolEn = false;

	m_pShotDelay = new CTimer();

	m_vAimBestHitboxScreen.Zero();
}


bool CAimbot::IsEnable()
{
	if (!m_pLocal || !m_pCmd)
		return false;

	CBaseEntity* localplayer = (CBaseEntity*)Interfaces::EntityList()->GetClientEntity(Interfaces::Engine()->GetLocalPlayer());

	if (!Settings::Aimbot::aim_Active)
		return false;

	/*if (!Settings::Aimbot::weapon_aim_settings[iWeaponID].aim_Active)
		return false;*/

	if (!m_pLocal->WeaponAmmo || m_pLocal->bInReload)
		return false;

	if (m_pLocal->WeaponType > WEAPON_TYPE_SNIPER)
		return false;	

	/*if (Settings::Aimbot::aim_KillDelay && (Settings::Misc::KillDelayBool2 || Interfaces::GlobalVars()->tickcount <= Settings::Misc::KillDelayVar))
		return false;*/

	/*if (Settings::Aimbot::aim_OnKey && !GetAsyncKeyState(Settings::Aimbot::aim_Key))
		return false;*/

	if (Settings::Aimbot::aim_CheckFlash && localplayer->GetFlashDuration())
		return false;

	if (Settings::Aimbot::aim_CheckSmoke)
	{
		if (LineGoesThroughSmoke(m_pLocal->vEyeOrigin, m_vAimBestHitbox))
			return false;
	}

	return true;
}

int CAimbot::GetPlayerFov(CPlayer * pPlayer)
{
	int iFov = 0;
	int iFovVal = 0;

	//ESCOPETAS
	if (m_pLocal->WeaponType == WEAPON_TYPE_SHOTGUN)
	{
		if (m_pLocal->iShotsFired > 1)
		{
			iFovVal = Settings::Aimbot::aim_RcsFov[4];
		}
		else
		{
			iFovVal = Settings::Aimbot::aim_Fov[4];
		}
	}
	//RIFLES
	if (m_pLocal->WeaponType == WEAPON_TYPE_RIFLE)
	{
		if (m_pLocal->iShotsFired > 1 )
		{
			iFovVal = Settings::Aimbot::aim_RcsFov[2];
		}
		else
		{
			iFovVal = Settings::Aimbot::aim_Fov[2];
		}
	}
	//SMG
	if (m_pLocal->WeaponType == WEAPON_TYPE_SMG)
	{
		if (m_pLocal->iShotsFired > 1)
		{
			iFovVal = Settings::Aimbot::aim_RcsFov[1];
		}
		else
		{
			iFovVal = Settings::Aimbot::aim_Fov[1];
		}
	}
	//MACHINEGUN
	if (m_pLocal->WeaponType == WEAPON_TYPE_MACHINEGUN)
	{
		if (m_pLocal->iShotsFired > 1)
		{
			iFovVal = Settings::Aimbot::aim_RcsFov[0];
		}
		else
		{
			iFovVal = Settings::Aimbot::aim_Fov[0];
		}
	}
	//Pistolas
	if (m_pLocal->WeaponType == WEAPON_TYPE_PISTOL)
	{
		
			iFovVal = Settings::Aimbot::aim_Fov[3];
		
	}
	//Snipers
	if (m_pLocal->WeaponType == WEAPON_TYPE_SNIPER)
	{
		
			iFovVal = Settings::Aimbot::aim_Fov[5];
		
	}

	int base_fov = pow(iFovVal + FIX_MIN_FOV_HEAD, 2) * 90;

	//Dos casos para todas las armas SEGUN EL FOV

	//MACHINEGUN
	if (m_pLocal->WeaponType == WEAPON_TYPE_MACHINEGUN)
	{

		if (Settings::Aimbot::aim_FovType[0] <= 0) //Machinegun
		{
			iFov = (int)(base_fov / (pPlayer->iDistance * m_pLocal->iFov));
		}
		else if (Settings::Aimbot::aim_FovType[0] == 1)
		{
			iFov = (int)(base_fov / (FOV_BASE_DISTANCE * m_pLocal->iFov));
		}
	}

	//SMG
	if (m_pLocal->WeaponType == WEAPON_TYPE_SMG)
	{

		if (Settings::Aimbot::aim_FovType[1] <= 0)
		{
			iFov = (int)(base_fov / (pPlayer->iDistance * m_pLocal->iFov));
		}
		else if (Settings::Aimbot::aim_FovType[1] == 1)
		{
			iFov = (int)(base_fov / (FOV_BASE_DISTANCE * m_pLocal->iFov));
		}
	}

	//RIFLE
	if (m_pLocal->WeaponType == WEAPON_TYPE_RIFLE)
	{

		if (Settings::Aimbot::aim_FovType[2] <= 0)
		{
			iFov = (int)(base_fov / (pPlayer->iDistance * m_pLocal->iFov));
		}
		else if (Settings::Aimbot::aim_FovType[2] == 1)
		{
			iFov = (int)(base_fov / (FOV_BASE_DISTANCE * m_pLocal->iFov));
		}
	}

	//PISTOLA
	if (m_pLocal->WeaponType == WEAPON_TYPE_PISTOL)
	{

		if (Settings::Aimbot::aim_FovType[3] <= 0)
		{
			iFov = (int)(base_fov / (pPlayer->iDistance * m_pLocal->iFov));
		}
		else if (Settings::Aimbot::aim_FovType[3] == 1)
		{
			iFov = (int)(base_fov / (FOV_BASE_DISTANCE * m_pLocal->iFov));
		}
	}

	//ESCOPETA
	if (m_pLocal->WeaponType == WEAPON_TYPE_SHOTGUN)
	{
		if (Settings::Aimbot::aim_FovType[4] <= 0)
		{
			iFov = (int)(base_fov / (pPlayer->iDistance * m_pLocal->iFov));
		}
		else if (Settings::Aimbot::aim_FovType[4] == 1)
		{
			iFov = (int)(base_fov / (FOV_BASE_DISTANCE * m_pLocal->iFov));
		}
	}

	//FRANCO

	if (m_pLocal->WeaponType == WEAPON_TYPE_SNIPER)
	{

		if (Settings::Aimbot::aim_FovType[5] <= 0)
		{
			iFov = (int)(base_fov / (pPlayer->iDistance * m_pLocal->iFov));
		}
		else if (Settings::Aimbot::aim_FovType[5] == 1)
		{
			iFov = (int)(base_fov / (FOV_BASE_DISTANCE * m_pLocal->iFov));
		}
	}

	

	return iFov;
}

int CAimbot::GetBestTarget()
{
	float ScreenDistanceBase = 1000.f;

	int m_lBestTarget = -1;

	for (BYTE PlayerIndex = 0; PlayerIndex < g_pPlayers->GetSize() /*&& g_pPlayers->bActive*/; PlayerIndex++)
	{
		CPlayer* pPlayer = g_pPlayers->GetPlayer(PlayerIndex);

		if (pPlayer && pPlayer->m_pEntity && pPlayer->bUpdate)
		{
			if (!Settings::Aimbot::aim_Deathmatch && pPlayer->Team == m_pLocal->Team)
				continue;

			if (!Settings::Aimbot::aim_WallAttack && !pPlayer->bVisible)
				continue;

			Vector vHitbox;
			Vector vHitboxScreen;

			//De nuevo dos caso por cada typo de arma 6*2=12 casos
			//MACHINEGUN
			if (m_pLocal->WeaponType == WEAPON_TYPE_MACHINEGUN)
			{
				if (Settings::Aimbot::aim_BestHit[0] >= 1)
				{
					vHitbox = pPlayer->m_pEntity->GetHitboxPosition(HITBOX_BODY);
				}
				else
				{
					vHitbox = pPlayer->m_pEntity->GetHitboxPosition(tt_ct_best_hit_1[Settings::Aimbot::aim_Spot[0]]);
				}
				
			}

			//SMG
			if (m_pLocal->WeaponType == WEAPON_TYPE_SMG)
			{
				if (Settings::Aimbot::aim_BestHit[1] >= 1)
				{
					vHitbox = pPlayer->m_pEntity->GetHitboxPosition(HITBOX_BODY);
				}
				else
				{
					vHitbox = pPlayer->m_pEntity->GetHitboxPosition(tt_ct_best_hit_1[Settings::Aimbot::aim_BestHit[1]]);
				}
				
			}

			//RIFLE
			if (m_pLocal->WeaponType == WEAPON_TYPE_RIFLE)
			{
				if (Settings::Aimbot::aim_BestHit[2] >= 1)
				{
					vHitbox = pPlayer->m_pEntity->GetHitboxPosition(HITBOX_BODY);
				}
				else
				{
					vHitbox = pPlayer->m_pEntity->GetHitboxPosition(tt_ct_best_hit_1[Settings::Aimbot::aim_BestHit[2]]);
				}
			}

			//PISTOLA
			if (m_pLocal->WeaponType == WEAPON_TYPE_PISTOL)
			{
				if (Settings::Aimbot::aim_BestHit[3] >= 1)
				{
					vHitbox = pPlayer->m_pEntity->GetHitboxPosition(HITBOX_BODY);
				}
				else
				{
					vHitbox = pPlayer->m_pEntity->GetHitboxPosition(tt_ct_best_hit_1[Settings::Aimbot::aim_BestHit[3]]);
				}
				
			}

			//ESCOPETA
			if (m_pLocal->WeaponType == WEAPON_TYPE_SHOTGUN)
			{
				if (Settings::Aimbot::aim_BestHit[4] >= 1)
				{
					vHitbox = pPlayer->m_pEntity->GetHitboxPosition(HITBOX_BODY);
				}
				else
				{
					vHitbox = pPlayer->m_pEntity->GetHitboxPosition(tt_ct_best_hit_1[Settings::Aimbot::aim_BestHit[4]]);
				}
			}

			//FRANCO

			if (m_pLocal->WeaponType == WEAPON_TYPE_SNIPER)
			{
				if (Settings::Aimbot::aim_BestHit[5] >= 1)
				{
					vHitbox = pPlayer->m_pEntity->GetHitboxPosition(HITBOX_BODY);
				}
				else
				{
					vHitbox = pPlayer->m_pEntity->GetHitboxPosition(tt_ct_best_hit_1[Settings::Aimbot::aim_BestHit[5]]);
				}
				
			}

			

			if (WorldToScreen(vHitbox, vHitboxScreen))
			{
				Vector2D vPlayerScr = Vector2D(vHitboxScreen.x, vHitboxScreen.y);

				float fDistanceScreen = DistanceScreen(g_vCenterScreen, vPlayerScr);

				if (fDistanceScreen < ScreenDistanceBase)
				{
					ScreenDistanceBase = fDistanceScreen;
					m_lBestTarget = PlayerIndex;
				}
			}
		}
	}

	return m_lBestTarget;
}

int CAimbot::GetBestHitBox()
{
	float ScreenDistanceBase = 1000.f;

	int m_lBestHitbox = -1;

	if (m_pLocal->WeaponType == WEAPON_TYPE_SHOTGUN || m_pLocal->WeaponType == WEAPON_TYPE_RIFLE || m_pLocal->WeaponType == WEAPON_TYPE_MACHINEGUN || m_pLocal->WeaponType == WEAPON_TYPE_SMG)
	{
		if (m_iBestHitbox && m_bAttack && !m_bTargetChange)
			m_lBestHitbox = m_iBestHitbox;
	}

	CPlayer* pPlayer = g_pPlayers->GetPlayer(m_iBestTarget);

	if (pPlayer && pPlayer->m_pEntity && pPlayer->bUpdate)
	{
		if (m_lBestHitbox == -1)
		{
			//Casos: 5*6 armas = 30 posibilidades
			//MACHINEGUN
			if (m_pLocal->WeaponType == WEAPON_TYPE_MACHINEGUN)
			{
				if (Settings::Aimbot::aim_BestHit[0] >= 4)
				{
					for (byte bHitbox = 0; bHitbox < TT_CT_BEST_HIT_SIZE_4; bHitbox++)
					{
						Vector vHitBox = pPlayer->m_pEntity->GetHitboxPosition(tt_ct_best_hit_4[bHitbox]);
						Vector vHitBoxScreen;

						if (!WorldToScreen(vHitBox, vHitBoxScreen))
							continue;

						Vector2D vHitboxSrc = Vector2D(vHitBoxScreen.x, vHitBoxScreen.y);

						float fDistanceScreen = DistanceScreen(g_vCenterScreen, vHitboxSrc);

						if (fDistanceScreen < ScreenDistanceBase)
						{
							ScreenDistanceBase = fDistanceScreen;
							m_lBestHitbox = tt_ct_best_hit_4[bHitbox];
						}
					}
				}

				else if (Settings::Aimbot::aim_BestHit[0] >= 3)
				{
					for (byte bHitbox = 0; bHitbox < TT_CT_BEST_HIT_SIZE_3; bHitbox++)
					{
						Vector vHitBox = pPlayer->m_pEntity->GetHitboxPosition(tt_ct_best_hit_3[bHitbox]);
						Vector vHitBoxScreen;

						if (!WorldToScreen(vHitBox, vHitBoxScreen))
							continue;

						Vector2D vHitboxSrc = Vector2D(vHitBoxScreen.x, vHitBoxScreen.y);

						float fDistanceScreen = DistanceScreen(g_vCenterScreen, vHitboxSrc);

						if (fDistanceScreen < ScreenDistanceBase)
						{
							ScreenDistanceBase = fDistanceScreen;
							m_lBestHitbox = tt_ct_best_hit_3[bHitbox];
						}
					}
				}

				else if (Settings::Aimbot::aim_BestHit[0] >= 2)
				{
					for (byte bHitbox = 0; bHitbox < TT_CT_BEST_HIT_SIZE_2; bHitbox++)
					{
						Vector vHitBox = pPlayer->m_pEntity->GetHitboxPosition(tt_ct_best_hit_2[bHitbox]);
						Vector vHitBoxScreen;

						if (!WorldToScreen(vHitBox, vHitBoxScreen))
							continue;

						Vector2D vHitboxSrc = Vector2D(vHitBoxScreen.x, vHitBoxScreen.y);

						float fDistanceScreen = DistanceScreen(g_vCenterScreen, vHitboxSrc);

						if (fDistanceScreen < ScreenDistanceBase)
						{
							ScreenDistanceBase = fDistanceScreen;
							m_lBestHitbox = tt_ct_best_hit_2[bHitbox];
						}
					}
				}

				else if (Settings::Aimbot::aim_BestHit[0] >= 1)
				{
					for (byte bHitbox = 0; bHitbox < TT_CT_BEST_HIT_SIZE_1; bHitbox++)
					{
						Vector vHitBox = pPlayer->m_pEntity->GetHitboxPosition(tt_ct_best_hit_1[bHitbox]);
						Vector vHitBoxScreen;

						if (!WorldToScreen(vHitBox, vHitBoxScreen))
							continue;

						Vector2D vHitboxSrc = Vector2D(vHitBoxScreen.x, vHitBoxScreen.y);

						float fDistanceScreen = DistanceScreen(g_vCenterScreen, vHitboxSrc);

						if (fDistanceScreen < ScreenDistanceBase)
						{
							ScreenDistanceBase = fDistanceScreen;
							m_lBestHitbox = tt_ct_best_hit_1[bHitbox];
						}
					}
				}

				else
				{
					m_lBestHitbox = tt_ct_best_hit_1[Settings::Aimbot::aim_Spot[0]];
				}
				
			}

			//SMG
			if (m_pLocal->WeaponType == WEAPON_TYPE_SMG)
			{
				if (Settings::Aimbot::aim_BestHit[1] >= 4)
				{
					for (byte bHitbox = 0; bHitbox < TT_CT_BEST_HIT_SIZE_4; bHitbox++)
					{
						Vector vHitBox = pPlayer->m_pEntity->GetHitboxPosition(tt_ct_best_hit_4[bHitbox]);
						Vector vHitBoxScreen;

						if (!WorldToScreen(vHitBox, vHitBoxScreen))
							continue;

						Vector2D vHitboxSrc = Vector2D(vHitBoxScreen.x, vHitBoxScreen.y);

						float fDistanceScreen = DistanceScreen(g_vCenterScreen, vHitboxSrc);

						if (fDistanceScreen < ScreenDistanceBase)
						{
							ScreenDistanceBase = fDistanceScreen;
							m_lBestHitbox = tt_ct_best_hit_4[bHitbox];
						}
					}
				}

				else if (Settings::Aimbot::aim_BestHit[1] >= 3)
				{
					for (byte bHitbox = 0; bHitbox < TT_CT_BEST_HIT_SIZE_3; bHitbox++)
					{
						Vector vHitBox = pPlayer->m_pEntity->GetHitboxPosition(tt_ct_best_hit_3[bHitbox]);
						Vector vHitBoxScreen;

						if (!WorldToScreen(vHitBox, vHitBoxScreen))
							continue;

						Vector2D vHitboxSrc = Vector2D(vHitBoxScreen.x, vHitBoxScreen.y);

						float fDistanceScreen = DistanceScreen(g_vCenterScreen, vHitboxSrc);

						if (fDistanceScreen < ScreenDistanceBase)
						{
							ScreenDistanceBase = fDistanceScreen;
							m_lBestHitbox = tt_ct_best_hit_3[bHitbox];
						}
					}
				}

				else if (Settings::Aimbot::aim_BestHit[1] >= 2)
				{
					for (byte bHitbox = 0; bHitbox < TT_CT_BEST_HIT_SIZE_2; bHitbox++)
					{
						Vector vHitBox = pPlayer->m_pEntity->GetHitboxPosition(tt_ct_best_hit_2[bHitbox]);
						Vector vHitBoxScreen;

						if (!WorldToScreen(vHitBox, vHitBoxScreen))
							continue;

						Vector2D vHitboxSrc = Vector2D(vHitBoxScreen.x, vHitBoxScreen.y);

						float fDistanceScreen = DistanceScreen(g_vCenterScreen, vHitboxSrc);

						if (fDistanceScreen < ScreenDistanceBase)
						{
							ScreenDistanceBase = fDistanceScreen;
							m_lBestHitbox = tt_ct_best_hit_2[bHitbox];
						}
					}
				}

				else if (Settings::Aimbot::aim_BestHit[1] >= 1)
				{
					for (byte bHitbox = 0; bHitbox < TT_CT_BEST_HIT_SIZE_1; bHitbox++)
					{
						Vector vHitBox = pPlayer->m_pEntity->GetHitboxPosition(tt_ct_best_hit_1[bHitbox]);
						Vector vHitBoxScreen;

						if (!WorldToScreen(vHitBox, vHitBoxScreen))
							continue;

						Vector2D vHitboxSrc = Vector2D(vHitBoxScreen.x, vHitBoxScreen.y);

						float fDistanceScreen = DistanceScreen(g_vCenterScreen, vHitboxSrc);

						if (fDistanceScreen < ScreenDistanceBase)
						{
							ScreenDistanceBase = fDistanceScreen;
							m_lBestHitbox = tt_ct_best_hit_1[bHitbox];
						}
					}
				}

				else
				{
					m_lBestHitbox = tt_ct_best_hit_1[Settings::Aimbot::aim_Spot[2]];
				}



			}

			//RIFLE
			if (m_pLocal->WeaponType == WEAPON_TYPE_RIFLE)
			{
				if (Settings::Aimbot::aim_BestHit[2] >= 4)
				{
					for (byte bHitbox = 0; bHitbox < TT_CT_BEST_HIT_SIZE_4; bHitbox++)
					{
						Vector vHitBox = pPlayer->m_pEntity->GetHitboxPosition(tt_ct_best_hit_4[bHitbox]);
						Vector vHitBoxScreen;

						if (!WorldToScreen(vHitBox, vHitBoxScreen))
							continue;

						Vector2D vHitboxSrc = Vector2D(vHitBoxScreen.x, vHitBoxScreen.y);

						float fDistanceScreen = DistanceScreen(g_vCenterScreen, vHitboxSrc);

						if (fDistanceScreen < ScreenDistanceBase)
						{
							ScreenDistanceBase = fDistanceScreen;
							m_lBestHitbox = tt_ct_best_hit_4[bHitbox];
						}
					}
				}

				else if (Settings::Aimbot::aim_BestHit[2] >= 3)
				{
					for (byte bHitbox = 0; bHitbox < TT_CT_BEST_HIT_SIZE_3; bHitbox++)
					{
						Vector vHitBox = pPlayer->m_pEntity->GetHitboxPosition(tt_ct_best_hit_3[bHitbox]);
						Vector vHitBoxScreen;

						if (!WorldToScreen(vHitBox, vHitBoxScreen))
							continue;

						Vector2D vHitboxSrc = Vector2D(vHitBoxScreen.x, vHitBoxScreen.y);

						float fDistanceScreen = DistanceScreen(g_vCenterScreen, vHitboxSrc);

						if (fDistanceScreen < ScreenDistanceBase)
						{
							ScreenDistanceBase = fDistanceScreen;
							m_lBestHitbox = tt_ct_best_hit_3[bHitbox];
						}
					}
				}

				else if (Settings::Aimbot::aim_BestHit[2] >= 2)
				{
					for (byte bHitbox = 0; bHitbox < TT_CT_BEST_HIT_SIZE_2; bHitbox++)
					{
						Vector vHitBox = pPlayer->m_pEntity->GetHitboxPosition(tt_ct_best_hit_2[bHitbox]);
						Vector vHitBoxScreen;

						if (!WorldToScreen(vHitBox, vHitBoxScreen))
							continue;

						Vector2D vHitboxSrc = Vector2D(vHitBoxScreen.x, vHitBoxScreen.y);

						float fDistanceScreen = DistanceScreen(g_vCenterScreen, vHitboxSrc);

						if (fDistanceScreen < ScreenDistanceBase)
						{
							ScreenDistanceBase = fDistanceScreen;
							m_lBestHitbox = tt_ct_best_hit_2[bHitbox];
						}
					}
				}

				else if (Settings::Aimbot::aim_BestHit[2] >= 1)
				{
					for (byte bHitbox = 0; bHitbox < TT_CT_BEST_HIT_SIZE_1; bHitbox++)
					{
						Vector vHitBox = pPlayer->m_pEntity->GetHitboxPosition(tt_ct_best_hit_1[bHitbox]);
						Vector vHitBoxScreen;

						if (!WorldToScreen(vHitBox, vHitBoxScreen))
							continue;

						Vector2D vHitboxSrc = Vector2D(vHitBoxScreen.x, vHitBoxScreen.y);

						float fDistanceScreen = DistanceScreen(g_vCenterScreen, vHitboxSrc);

						if (fDistanceScreen < ScreenDistanceBase)
						{
							ScreenDistanceBase = fDistanceScreen;
							m_lBestHitbox = tt_ct_best_hit_1[bHitbox];
						}
					}
				}

				else
				{
					m_lBestHitbox = tt_ct_best_hit_1[Settings::Aimbot::aim_Spot[2]];
				}
				
			}

			//PISTOLA
			if (m_pLocal->WeaponType == WEAPON_TYPE_PISTOL)
			{
				if (Settings::Aimbot::aim_BestHit[3] >= 4)
				{
					for (byte bHitbox = 0; bHitbox < TT_CT_BEST_HIT_SIZE_4; bHitbox++)
					{
						Vector vHitBox = pPlayer->m_pEntity->GetHitboxPosition(tt_ct_best_hit_4[bHitbox]);
						Vector vHitBoxScreen;

						if (!WorldToScreen(vHitBox, vHitBoxScreen))
							continue;

						Vector2D vHitboxSrc = Vector2D(vHitBoxScreen.x, vHitBoxScreen.y);

						float fDistanceScreen = DistanceScreen(g_vCenterScreen, vHitboxSrc);

						if (fDistanceScreen < ScreenDistanceBase)
						{
							ScreenDistanceBase = fDistanceScreen;
							m_lBestHitbox = tt_ct_best_hit_4[bHitbox];
						}
					}
				}

				else if (Settings::Aimbot::aim_BestHit[3] >= 3)
				{
					for (byte bHitbox = 0; bHitbox < TT_CT_BEST_HIT_SIZE_3; bHitbox++)
					{
						Vector vHitBox = pPlayer->m_pEntity->GetHitboxPosition(tt_ct_best_hit_3[bHitbox]);
						Vector vHitBoxScreen;

						if (!WorldToScreen(vHitBox, vHitBoxScreen))
							continue;

						Vector2D vHitboxSrc = Vector2D(vHitBoxScreen.x, vHitBoxScreen.y);

						float fDistanceScreen = DistanceScreen(g_vCenterScreen, vHitboxSrc);

						if (fDistanceScreen < ScreenDistanceBase)
						{
							ScreenDistanceBase = fDistanceScreen;
							m_lBestHitbox = tt_ct_best_hit_3[bHitbox];
						}
					}
				}

				else if (Settings::Aimbot::aim_BestHit[3] >= 2)
				{
					for (byte bHitbox = 0; bHitbox < TT_CT_BEST_HIT_SIZE_2; bHitbox++)
					{
						Vector vHitBox = pPlayer->m_pEntity->GetHitboxPosition(tt_ct_best_hit_2[bHitbox]);
						Vector vHitBoxScreen;

						if (!WorldToScreen(vHitBox, vHitBoxScreen))
							continue;

						Vector2D vHitboxSrc = Vector2D(vHitBoxScreen.x, vHitBoxScreen.y);

						float fDistanceScreen = DistanceScreen(g_vCenterScreen, vHitboxSrc);

						if (fDistanceScreen < ScreenDistanceBase)
						{
							ScreenDistanceBase = fDistanceScreen;
							m_lBestHitbox = tt_ct_best_hit_2[bHitbox];
						}
					}
				}

				else if (Settings::Aimbot::aim_BestHit[3] >= 1)
				{
					for (byte bHitbox = 0; bHitbox < TT_CT_BEST_HIT_SIZE_1; bHitbox++)
					{
						Vector vHitBox = pPlayer->m_pEntity->GetHitboxPosition(tt_ct_best_hit_1[bHitbox]);
						Vector vHitBoxScreen;

						if (!WorldToScreen(vHitBox, vHitBoxScreen))
							continue;

						Vector2D vHitboxSrc = Vector2D(vHitBoxScreen.x, vHitBoxScreen.y);

						float fDistanceScreen = DistanceScreen(g_vCenterScreen, vHitboxSrc);

						if (fDistanceScreen < ScreenDistanceBase)
						{
							ScreenDistanceBase = fDistanceScreen;
							m_lBestHitbox = tt_ct_best_hit_1[bHitbox];
						}
					}
				}

				else
				{
					m_lBestHitbox = tt_ct_best_hit_1[Settings::Aimbot::aim_Spot[3]];
				}
				
			}

			//ESCOPETA
			if (m_pLocal->WeaponType == WEAPON_TYPE_SHOTGUN)
			{
				if (Settings::Aimbot::aim_BestHit[4] >= 4)
				{
					for (byte bHitbox = 0; bHitbox < TT_CT_BEST_HIT_SIZE_4; bHitbox++)
					{
						Vector vHitBox = pPlayer->m_pEntity->GetHitboxPosition(tt_ct_best_hit_4[bHitbox]);
						Vector vHitBoxScreen;

						if (!WorldToScreen(vHitBox, vHitBoxScreen))
							continue;

						Vector2D vHitboxSrc = Vector2D(vHitBoxScreen.x, vHitBoxScreen.y);

						float fDistanceScreen = DistanceScreen(g_vCenterScreen, vHitboxSrc);

						if (fDistanceScreen < ScreenDistanceBase)
						{
							ScreenDistanceBase = fDistanceScreen;
							m_lBestHitbox = tt_ct_best_hit_4[bHitbox];
						}
					}
				}

				else if (Settings::Aimbot::aim_BestHit[4] >= 3)
				{
					for (byte bHitbox = 0; bHitbox < TT_CT_BEST_HIT_SIZE_3; bHitbox++)
					{
						Vector vHitBox = pPlayer->m_pEntity->GetHitboxPosition(tt_ct_best_hit_3[bHitbox]);
						Vector vHitBoxScreen;

						if (!WorldToScreen(vHitBox, vHitBoxScreen))
							continue;

						Vector2D vHitboxSrc = Vector2D(vHitBoxScreen.x, vHitBoxScreen.y);

						float fDistanceScreen = DistanceScreen(g_vCenterScreen, vHitboxSrc);

						if (fDistanceScreen < ScreenDistanceBase)
						{
							ScreenDistanceBase = fDistanceScreen;
							m_lBestHitbox = tt_ct_best_hit_3[bHitbox];
						}
					}
				}

				else if (Settings::Aimbot::aim_BestHit[4] >= 2)
				{
					for (byte bHitbox = 0; bHitbox < TT_CT_BEST_HIT_SIZE_2; bHitbox++)
					{
						Vector vHitBox = pPlayer->m_pEntity->GetHitboxPosition(tt_ct_best_hit_2[bHitbox]);
						Vector vHitBoxScreen;

						if (!WorldToScreen(vHitBox, vHitBoxScreen))
							continue;

						Vector2D vHitboxSrc = Vector2D(vHitBoxScreen.x, vHitBoxScreen.y);

						float fDistanceScreen = DistanceScreen(g_vCenterScreen, vHitboxSrc);

						if (fDistanceScreen < ScreenDistanceBase)
						{
							ScreenDistanceBase = fDistanceScreen;
							m_lBestHitbox = tt_ct_best_hit_2[bHitbox];
						}
					}
				}

				else if (Settings::Aimbot::aim_BestHit[4] >= 1)
				{
					for (byte bHitbox = 0; bHitbox < TT_CT_BEST_HIT_SIZE_1; bHitbox++)
					{
						Vector vHitBox = pPlayer->m_pEntity->GetHitboxPosition(tt_ct_best_hit_1[bHitbox]);
						Vector vHitBoxScreen;

						if (!WorldToScreen(vHitBox, vHitBoxScreen))
							continue;

						Vector2D vHitboxSrc = Vector2D(vHitBoxScreen.x, vHitBoxScreen.y);

						float fDistanceScreen = DistanceScreen(g_vCenterScreen, vHitboxSrc);

						if (fDistanceScreen < ScreenDistanceBase)
						{
							ScreenDistanceBase = fDistanceScreen;
							m_lBestHitbox = tt_ct_best_hit_1[bHitbox];
						}
					}
				}

				else
				{
					m_lBestHitbox = tt_ct_best_hit_1[Settings::Aimbot::aim_Spot[4]];
				}
			}

			//FRANCO

			if (m_pLocal->WeaponType == WEAPON_TYPE_SNIPER)
			{
				if (Settings::Aimbot::aim_BestHit[5] >= 4)
				{
					for (byte bHitbox = 0; bHitbox < TT_CT_BEST_HIT_SIZE_4; bHitbox++)
					{
						Vector vHitBox = pPlayer->m_pEntity->GetHitboxPosition(tt_ct_best_hit_4[bHitbox]);
						Vector vHitBoxScreen;

						if (!WorldToScreen(vHitBox, vHitBoxScreen))
							continue;

						Vector2D vHitboxSrc = Vector2D(vHitBoxScreen.x, vHitBoxScreen.y);

						float fDistanceScreen = DistanceScreen(g_vCenterScreen, vHitboxSrc);

						if (fDistanceScreen < ScreenDistanceBase)
						{
							ScreenDistanceBase = fDistanceScreen;
							m_lBestHitbox = tt_ct_best_hit_4[bHitbox];
						}
					}
				}

				else if (Settings::Aimbot::aim_BestHit[5] >= 3)
				{
					for (byte bHitbox = 0; bHitbox < TT_CT_BEST_HIT_SIZE_3; bHitbox++)
					{
						Vector vHitBox = pPlayer->m_pEntity->GetHitboxPosition(tt_ct_best_hit_3[bHitbox]);
						Vector vHitBoxScreen;

						if (!WorldToScreen(vHitBox, vHitBoxScreen))
							continue;

						Vector2D vHitboxSrc = Vector2D(vHitBoxScreen.x, vHitBoxScreen.y);

						float fDistanceScreen = DistanceScreen(g_vCenterScreen, vHitboxSrc);

						if (fDistanceScreen < ScreenDistanceBase)
						{
							ScreenDistanceBase = fDistanceScreen;
							m_lBestHitbox = tt_ct_best_hit_3[bHitbox];
						}
					}
				}

				else if (Settings::Aimbot::aim_BestHit[5] >= 2)
				{
					for (byte bHitbox = 0; bHitbox < TT_CT_BEST_HIT_SIZE_2; bHitbox++)
					{
						Vector vHitBox = pPlayer->m_pEntity->GetHitboxPosition(tt_ct_best_hit_2[bHitbox]);
						Vector vHitBoxScreen;

						if (!WorldToScreen(vHitBox, vHitBoxScreen))
							continue;

						Vector2D vHitboxSrc = Vector2D(vHitBoxScreen.x, vHitBoxScreen.y);

						float fDistanceScreen = DistanceScreen(g_vCenterScreen, vHitboxSrc);

						if (fDistanceScreen < ScreenDistanceBase)
						{
							ScreenDistanceBase = fDistanceScreen;
							m_lBestHitbox = tt_ct_best_hit_2[bHitbox];
						}
					}
				}

				else if (Settings::Aimbot::aim_BestHit[5] >= 1)
				{
					for (byte bHitbox = 0; bHitbox < TT_CT_BEST_HIT_SIZE_1; bHitbox++)
					{
						Vector vHitBox = pPlayer->m_pEntity->GetHitboxPosition(tt_ct_best_hit_1[bHitbox]);
						Vector vHitBoxScreen;

						if (!WorldToScreen(vHitBox, vHitBoxScreen))
							continue;

						Vector2D vHitboxSrc = Vector2D(vHitBoxScreen.x, vHitBoxScreen.y);

						float fDistanceScreen = DistanceScreen(g_vCenterScreen, vHitboxSrc);

						if (fDistanceScreen < ScreenDistanceBase)
						{
							ScreenDistanceBase = fDistanceScreen;
							m_lBestHitbox = tt_ct_best_hit_1[bHitbox];
						}
					}
				}

				else
				{
					m_lBestHitbox = tt_ct_best_hit_1[Settings::Aimbot::aim_Spot[5]];
				}
				
			}





			
		}

		Vector vHitBox = pPlayer->m_pEntity->GetHitboxPosition(m_lBestHitbox);

		if (vHitBox.IsValid() && WorldToScreen(vHitBox, m_vAimBestHitboxScreen))
		{
			m_vAimBestHitbox = vHitBox;
			return m_lBestHitbox;
		}
		else
		{
			m_vAimBestHitbox.Zero();
			m_vAimBestHitboxScreen.Zero();
			return -1;
		}
	}

	return m_lBestHitbox;
}

void CAimbot::OnRender()
{
	if (!IsEnable() || m_iBestTarget == -1 || m_iBestHitbox == -1)
		return;

	if (m_vAimBestHitboxScreen.x <= 0.f || m_vAimBestHitboxScreen.y <= 0.f)
		return;

	CPlayer* pTargetPlayer = g_pPlayers->GetPlayer(m_iBestTarget);

	if (Settings::Aimbot::aim_DrawSpot)
	{
		g_pRender->DrawFillBox(m_vAimBestHitboxScreen.x, m_vAimBestHitboxScreen.y,
			2, 2, g_pEsp->GetPlayerColor(pTargetPlayer));
	}

	if (Settings::Aimbot::aim_DrawFov)
	{
		int iPlayerFov = GetPlayerFov(pTargetPlayer);

		Color EspVisibleColor = g_pEsp->GetPlayerVisibleColor(pTargetPlayer);

		//Dos casos para 6 tipos de armas = 12 posibilidades
		//MACHINEGUN
		if (m_pLocal->WeaponType == WEAPON_TYPE_MACHINEGUN)
		{
			if (Settings::Aimbot::aim_FovType[0] == 0)
			{
				g_pRender->DrawOutlineBox(m_vAimBestHitboxScreen.x - iPlayerFov, m_vAimBestHitboxScreen.y - iPlayerFov,
					iPlayerFov * 2, iPlayerFov * 2,
					Color(int(Settings::Aimbot::aim_FovColor[0] * 255.f),
						int(Settings::Aimbot::aim_FovColor[1] * 255.f),
						int(Settings::Aimbot::aim_FovColor[2] * 255.f)));

			}

			if (Settings::Aimbot::aim_FovType[0] == 1)
			{
				g_pRender->DrawOutlineBox(g_vCenterScreen.x - iPlayerFov, g_vCenterScreen.y - iPlayerFov, iPlayerFov * 2, iPlayerFov * 2,
					Color(int(Settings::Aimbot::aim_FovColor[0] * 255.f),
						int(Settings::Aimbot::aim_FovColor[1] * 255.f),
						int(Settings::Aimbot::aim_FovColor[2] * 255.f)));

			}
		}

		//SMG
		if (m_pLocal->WeaponType == WEAPON_TYPE_SMG)
		{
			if (Settings::Aimbot::aim_FovType[1] == 0)
			{
				g_pRender->DrawOutlineBox(m_vAimBestHitboxScreen.x - iPlayerFov, m_vAimBestHitboxScreen.y - iPlayerFov,
					iPlayerFov * 2, iPlayerFov * 2,
					Color(int(Settings::Aimbot::aim_FovColor[0] * 255.f),
						int(Settings::Aimbot::aim_FovColor[1] * 255.f),
						int(Settings::Aimbot::aim_FovColor[2] * 255.f)));

			}

			if (Settings::Aimbot::aim_FovType[1] == 1)
			{
				g_pRender->DrawOutlineBox(g_vCenterScreen.x - iPlayerFov, g_vCenterScreen.y - iPlayerFov, iPlayerFov * 2, iPlayerFov * 2,
					Color(int(Settings::Aimbot::aim_FovColor[0] * 255.f),
						int(Settings::Aimbot::aim_FovColor[1] * 255.f),
						int(Settings::Aimbot::aim_FovColor[2] * 255.f)));

			}

		}

		//RIFLE
		if (m_pLocal->WeaponType == WEAPON_TYPE_RIFLE)
		{
			if (Settings::Aimbot::aim_FovType[2] == 0)
			{
				g_pRender->DrawOutlineBox(m_vAimBestHitboxScreen.x - iPlayerFov, m_vAimBestHitboxScreen.y - iPlayerFov,
					iPlayerFov * 2, iPlayerFov * 2,
					Color(int(Settings::Aimbot::aim_FovColor[0] * 255.f),
						int(Settings::Aimbot::aim_FovColor[1] * 255.f),
						int(Settings::Aimbot::aim_FovColor[2] * 255.f)));

			}

			if (Settings::Aimbot::aim_FovType[2] == 1)
			{
				g_pRender->DrawOutlineBox(g_vCenterScreen.x - iPlayerFov, g_vCenterScreen.y - iPlayerFov, iPlayerFov * 2, iPlayerFov * 2,
					Color(int(Settings::Aimbot::aim_FovColor[0] * 255.f),
						int(Settings::Aimbot::aim_FovColor[1] * 255.f),
						int(Settings::Aimbot::aim_FovColor[2] * 255.f)));

			}

		}

		//PISTOLA
		if (m_pLocal->WeaponType == WEAPON_TYPE_PISTOL)
		{
			if (Settings::Aimbot::aim_FovType[3] == 0)
			{
				g_pRender->DrawOutlineBox(m_vAimBestHitboxScreen.x - iPlayerFov, m_vAimBestHitboxScreen.y - iPlayerFov,
					iPlayerFov * 2, iPlayerFov * 2,
					Color(int(Settings::Aimbot::aim_FovColor[0] * 255.f),
						int(Settings::Aimbot::aim_FovColor[1] * 255.f),
						int(Settings::Aimbot::aim_FovColor[2] * 255.f)));

			}

			if (Settings::Aimbot::aim_FovType[3] == 1)
			{
				g_pRender->DrawOutlineBox(g_vCenterScreen.x - iPlayerFov, g_vCenterScreen.y - iPlayerFov, iPlayerFov * 2, iPlayerFov * 2,
					Color(int(Settings::Aimbot::aim_FovColor[0] * 255.f),
						int(Settings::Aimbot::aim_FovColor[1] * 255.f),
						int(Settings::Aimbot::aim_FovColor[2] * 255.f)));

			}

		}

		//ESCOPETA
		if (m_pLocal->WeaponType == WEAPON_TYPE_SHOTGUN)
		{
			if (Settings::Aimbot::aim_FovType[4] == 0)
			{
				g_pRender->DrawOutlineBox(m_vAimBestHitboxScreen.x - iPlayerFov, m_vAimBestHitboxScreen.y - iPlayerFov,
					iPlayerFov * 2, iPlayerFov * 2,
					Color(int(Settings::Aimbot::aim_FovColor[0] * 255.f),
						int(Settings::Aimbot::aim_FovColor[1] * 255.f),
						int(Settings::Aimbot::aim_FovColor[2] * 255.f)));

			}

			if (Settings::Aimbot::aim_FovType[4] == 1)
			{
				g_pRender->DrawOutlineBox(g_vCenterScreen.x - iPlayerFov, g_vCenterScreen.y - iPlayerFov, iPlayerFov * 2, iPlayerFov * 2,
					Color(int(Settings::Aimbot::aim_FovColor[0] * 255.f),
						int(Settings::Aimbot::aim_FovColor[1] * 255.f),
						int(Settings::Aimbot::aim_FovColor[2] * 255.f)));

			}
		}

		//FRANCO

		if (m_pLocal->WeaponType == WEAPON_TYPE_SNIPER)
		{
			if (Settings::Aimbot::aim_FovType[5] == 0)
			{
				g_pRender->DrawOutlineBox(m_vAimBestHitboxScreen.x - iPlayerFov, m_vAimBestHitboxScreen.y - iPlayerFov,
					iPlayerFov * 2, iPlayerFov * 2,
					Color(int(Settings::Aimbot::aim_FovColor[0] * 255.f),
						int(Settings::Aimbot::aim_FovColor[1] * 255.f),
						int(Settings::Aimbot::aim_FovColor[2] * 255.f)));

			}

			if (Settings::Aimbot::aim_FovType[5] == 1)
			{
				g_pRender->DrawOutlineBox(g_vCenterScreen.x - iPlayerFov, g_vCenterScreen.y - iPlayerFov, iPlayerFov * 2, iPlayerFov * 2,
					Color(int(Settings::Aimbot::aim_FovColor[0] * 255.f),
						int(Settings::Aimbot::aim_FovColor[1] * 255.f),
						int(Settings::Aimbot::aim_FovColor[2] * 255.f)));

			}

		}




	}

}

void CAimbot::Aimbot()
{
	m_bAimShot = false;
	m_bTargetFov = false;

	if (!IsEnable())
		return;

	m_bAttack = (m_pCmd->buttons & IN_ATTACK);

	if (!g_pPlayers->GetPlayer(m_iBestTarget)->bUpdate)
	{
		m_iBestPreTarget = -1;
		m_iBestTarget = -1;
		m_iBestHitbox = -1;
	}

	m_iBestPreTarget = m_iBestTarget;
	m_iBestTarget = GetBestTarget();

	if (m_bAttack)
	{
		// Solo usamos numero 3 solo para pistolas y el 3 para evitar confusiones

		if (Settings::Aimbot::aim_AutoPistol[3] && m_pLocal->WeaponType == WEAPON_TYPE_PISTOL && !m_bAutoPistolEn)
		{
			AutoPistol();
		}
	}

	if (m_iBestTarget == -1)
		return;

	if (m_iBestPreTarget != m_iBestTarget && m_bAttack)
	{
		m_bTargetChange = true;
	}
	else
	{
		m_bTargetChange = false;
	}

	m_iBestHitbox = GetBestHitBox();

	if (m_iBestHitbox == -1)
		return;

	CPlayer* pPreTargetPlayer = g_pPlayers->GetPlayer(m_iBestPreTarget);
	CPlayer* pTargetPlayer = g_pPlayers->GetPlayer(m_iBestTarget);

	int iPlayerFov = GetPlayerFov(pTargetPlayer);

	if (CheckPlayerFov(m_vAimBestHitboxScreen, iPlayerFov))
		m_bTargetFov = true;

	static bool bLockDelay = false;
	static bool bLockAttack = false;

	if (!m_bAttack)
	{
		m_bClamp = true;
		bLockDelay = false;
		bLockAttack = false;
		m_pShotDelay->reset();
	}

	//Casos especiales para solo pistola

	if (Settings::Aimbot::aim_AutoPistol[3] && m_pLocal->WeaponType == WEAPON_TYPE_PISTOL)
	{
		if (m_bTargetFov && !m_bAttack)
		{
			m_bAutoPistolEn = true;
		}
		else if (!m_bTargetFov && !m_bAttack)
		{
			m_bAutoPistolEn = false;
		}
		else if (!m_bTargetFov && m_bAttack)
		{
			m_bAutoPistolEn = false;
		}
	}

	if (!Settings::Aimbot::aim_AutoPistol[3] && m_pLocal->WeaponType == WEAPON_TYPE_PISTOL)
	{
		if (m_pLocal->iShotsFired == 1 && bLockDelay)
			return;
	}

	AIM_ATTACK_TYPE AttackType = AA_SHOTGUN;

	if (m_pLocal->WeaponType == WEAPON_TYPE_SNIPER || m_pLocal->WeaponType == WEAPON_TYPE_PISTOL)
		AttackType = AA_SNIPER_PISTOL;

	static bool bAimAttack = false;
	static bool bAimCheckAttack = false;

	if (AttackType == AA_SHOTGUN)
	{
		if (m_bAttack && m_bTargetFov)
		{
			bAimCheckAttack = true;
		}
		else
		{
			bAimCheckAttack = false;
		}
	}
	else if (AttackType == AA_SNIPER_PISTOL)
	{
		if (!Settings::Aimbot::aim_AutoPistol[3])
		{
			if (m_bTargetFov && !m_bAttack)
			{
				bAimAttack = true;
			}
			else if (!m_bTargetFov && m_bAttack)
			{
				bAimAttack = false;
			}
			else if (!m_bTargetFov && !m_bAttack)
			{
				bAimAttack = false;
			}

			if (m_bAttack && bAimAttack && m_bTargetFov)
			{
				bAimCheckAttack = true;
			}
			else
			{
				bAimCheckAttack = false;
			}
		}
		else
		{
			if (m_bAttack && m_bTargetFov)
			{
				bAimCheckAttack = true;
			}
			else
			{
				bAimCheckAttack = false;
				m_bClamp = true;
			}
		}
	}

	//Separamos casos
	if (m_pLocal->WeaponType == WEAPON_TYPE_SNIPER)
		Settings::Aimbot::aim_RcsClampType[5] = 2;


	if (m_pLocal->WeaponType == WEAPON_TYPE_PISTOL)
		Settings::Aimbot::aim_RcsClampType[3] = 2;

	//Casos diferentes
	//Aqui entraban escopetas y rifles y armas pesadas ahora metemos todo eso
	if (Settings::Aimbot::aim_RcsClampType[4] >= 1 && m_pLocal->WeaponType == WEAPON_TYPE_SHOTGUN)
	{
		if (!bLockAttack)
		{
			if (m_bTargetFov && !m_bAttack)
			{
				bAimAttack = true;
			}
			else if (!m_bTargetFov && m_bAttack)
			{
				bAimAttack = false;
			}
			else if (!m_bTargetFov && !m_bAttack)
			{
				bAimAttack = false;
			}
		}

		if (m_bAttack && bAimAttack && m_bTargetFov && !bLockAttack)
		{
			bAimCheckAttack = true;
			bLockAttack = true;
		}
		else if (m_bAttack && m_bTargetFov && bLockAttack)
		{
			bAimCheckAttack = true;
			bLockAttack = true;
		}
		else
			bAimCheckAttack = false;
	}
	if (Settings::Aimbot::aim_RcsClampType[2] >= 1 && m_pLocal->WeaponType == WEAPON_TYPE_RIFLE)
	{
		if (!bLockAttack)
		{
			if (m_bTargetFov && !m_bAttack)
			{
				bAimAttack = true;
			}
			else if (!m_bTargetFov && m_bAttack)
			{
				bAimAttack = false;
			}
			else if (!m_bTargetFov && !m_bAttack)
			{
				bAimAttack = false;
			}
		}

		if (m_bAttack && bAimAttack && m_bTargetFov && !bLockAttack)
		{
			bAimCheckAttack = true;
			bLockAttack = true;
		}
		else if (m_bAttack && m_bTargetFov && bLockAttack)
		{
			bAimCheckAttack = true;
			bLockAttack = true;
		}
		else
			bAimCheckAttack = false;
	}
	if (Settings::Aimbot::aim_RcsClampType[0] >= 1 && m_pLocal->WeaponType == WEAPON_TYPE_MACHINEGUN)
	{
		if (!bLockAttack)
		{
			if (m_bTargetFov && !m_bAttack)
			{
				bAimAttack = true;
			}
			else if (!m_bTargetFov && m_bAttack)
			{
				bAimAttack = false;
			}
			else if (!m_bTargetFov && !m_bAttack)
			{
				bAimAttack = false;
			}
		}

		if (m_bAttack && bAimAttack && m_bTargetFov && !bLockAttack)
		{
			bAimCheckAttack = true;
			bLockAttack = true;
		}
		else if (m_bAttack && m_bTargetFov && bLockAttack)
		{
			bAimCheckAttack = true;
			bLockAttack = true;
		}
		else
			bAimCheckAttack = false;
	}
	if (Settings::Aimbot::aim_RcsClampType[1] >= 1 && m_pLocal->WeaponType == WEAPON_TYPE_SMG)
	{
		if (!bLockAttack)
		{
			if (m_bTargetFov && !m_bAttack)
			{
				bAimAttack = true;
			}
			else if (!m_bTargetFov && m_bAttack)
			{
				bAimAttack = false;
			}
			else if (!m_bTargetFov && !m_bAttack)
			{
				bAimAttack = false;
			}
		}

		if (m_bAttack && bAimAttack && m_bTargetFov && !bLockAttack)
		{
			bAimCheckAttack = true;
			bLockAttack = true;
		}
		else if (m_bAttack && m_bTargetFov && bLockAttack)
		{
			bAimCheckAttack = true;
			bLockAttack = true;
		}
		else
			bAimCheckAttack = false;
	}

	//Un caso para 6 armas = 6 posibilidades

	//MACHINEGUN
	if (m_pLocal->WeaponType == WEAPON_TYPE_MACHINEGUN)
	{
		if (Settings::Aimbot::aim_RcsClampType[0] >= 2)
		{
			if (m_bClamp && m_bTargetChange)
				m_bClamp = false;
		}
		
	}

	//SMG
	if (m_pLocal->WeaponType == WEAPON_TYPE_SMG)
	{
		if (Settings::Aimbot::aim_RcsClampType[1] >= 2)
		{
			if (m_bClamp && m_bTargetChange)
				m_bClamp = false;
		}
		
	}

	//RIFLE
	if (m_pLocal->WeaponType == WEAPON_TYPE_RIFLE)
	{
		if (Settings::Aimbot::aim_RcsClampType[2] >= 2)
		{
			if (m_bClamp && m_bTargetChange)
				m_bClamp = false;
		}
		
	}

	//PISTOLA
	if (m_pLocal->WeaponType == WEAPON_TYPE_PISTOL)
	{
		if (Settings::Aimbot::aim_RcsClampType[3] >= 2)
		{
			if (m_bClamp && m_bTargetChange)
				m_bClamp = false;
		}
	}

	//ESCOPETA
	if (m_pLocal->WeaponType == WEAPON_TYPE_SHOTGUN)
	{
		if (Settings::Aimbot::aim_RcsClampType[4] >= 2)
		{
			if (m_bClamp && m_bTargetChange)
				m_bClamp = false;
		}
	}

	//FRANCO

	if (m_pLocal->WeaponType == WEAPON_TYPE_SNIPER)
	{
		if (Settings::Aimbot::aim_RcsClampType[5] >= 2)
		{
			if (m_bClamp && m_bTargetChange)
				m_bClamp = false;
		}
		
	}

	

	if (!(pTargetPlayer->m_pEntity->GetFlags() & FL_ONGROUND) && Settings::Aimbot::aim_AntiJump)
		return;

	if (bAimCheckAttack)
	{
		int AimDelay = Settings::Aimbot::weapon_aim_settings[iWeaponID].aim_Delay;

		if (AimDelay)
		{
			float fDistanceAimScreen = DistanceScreen(g_vCenterScreen, Vector2D(m_vAimBestHitboxScreen.x, m_vAimBestHitboxScreen.y));

			if (m_pShotDelay->delay(AimDelay))
				m_bAimShot = true;

			if (fDistanceAimScreen <= 3.5f && !bLockDelay)
			{
				m_bAimShot = true;
				bLockDelay = true;
			}
			else if (bLockDelay)
			{
				m_bAimShot = true;
			}
		}
		else
		{
			m_bAimShot = true;
		}


		if (m_bClamp)
		{
			AimbotSet();
		}

		if (m_pLocal->WeaponType == WEAPON_TYPE_SHOTGUN || m_pLocal->WeaponType == WEAPON_TYPE_RIFLE  || m_pLocal->WeaponType == WEAPON_TYPE_MACHINEGUN || m_pLocal->WeaponType == WEAPON_TYPE_SMG ||  !Settings::Aimbot::aim_AutoPistol[3])
		{
			if (m_bAimShot)
			{
				m_pCmd->buttons |= IN_ATTACK;
			}
			else
			{
				m_pCmd->buttons &= ~IN_ATTACK;
			}
		}

		if (m_pLocal->WeaponType == WEAPON_TYPE_PISTOL && Settings::Aimbot::aim_AutoPistol[3] && m_bAutoPistolEn)
		{
			if (m_bAimShot)
			{
				m_pCmd->buttons |= IN_ATTACK;
				m_bAutoPistolEn = false;
			}
			else
				m_pCmd->buttons &= ~IN_ATTACK;
		}
	}
}

void CAimbot::AimbotSet()
{
	//Asignamos el valor del backtrack segun el tipo de arma
	//para no tener que pasarnos la variable de m_Plocal
	if (m_pLocal->WeaponType == WEAPON_TYPE_MACHINEGUN)
		Settings::Aimbot::aim_Globalbacktrack = Settings::Aimbot::aim_backtrack[0];
	if (m_pLocal->WeaponType == WEAPON_TYPE_SMG)
		Settings::Aimbot::aim_Globalbacktrack = Settings::Aimbot::aim_backtrack[1];
	if (m_pLocal->WeaponType == WEAPON_TYPE_RIFLE)
		Settings::Aimbot::aim_Globalbacktrack = Settings::Aimbot::aim_backtrack[2];
	if (m_pLocal->WeaponType == WEAPON_TYPE_PISTOL)
		Settings::Aimbot::aim_Globalbacktrack = Settings::Aimbot::aim_backtrack[3];
	if (m_pLocal->WeaponType == WEAPON_TYPE_SHOTGUN)
		Settings::Aimbot::aim_Globalbacktrack = Settings::Aimbot::aim_backtrack[4];
	if (m_pLocal->WeaponType == WEAPON_TYPE_SNIPER)
		Settings::Aimbot::aim_Globalbacktrack = Settings::Aimbot::aim_backtrack[5];


	if (!m_vAimBestHitbox.IsValid())
		return;

	Vector vAimAngle, vSmoothAimAngle;

	VectorAngles(m_vAimBestHitbox - m_pLocal->vEyeOrigin, vAimAngle);

	bool bEnableRcs = false;

	//dOS CASOS PARA 6 ARMAS = 12 POSIBILIDADES
	//MACHINEGUN
	if (m_pLocal->WeaponType == WEAPON_TYPE_MACHINEGUN)
	{
		if (Settings::Aimbot::aim_RCSType[0] <= 0)
		{
			if (Settings::Aimbot::aim_Rcs[0])
			{
				if ((m_pLocal->WeaponType == WEAPON_TYPE_SHOTGUN && m_pLocal->iShotsFired > 1) || (m_pLocal->WeaponType == WEAPON_TYPE_RIFLE && m_pLocal->iShotsFired > 1) || (m_pLocal->WeaponType == WEAPON_TYPE_MACHINEGUN && m_pLocal->iShotsFired > 1))
					bEnableRcs = true;

				if (m_pLocal->WeaponType == WEAPON_TYPE_PISTOL && m_pLocal->iShotsFired <= 1)
					bEnableRcs = true;
			}
		}
		else if (Settings::Aimbot::aim_RCSType[0] >= 1)
		{
			if (Settings::Aimbot::aim_Rcsx[0] && Settings::Aimbot::aim_Rcsy[0])
			{
				if ((m_pLocal->WeaponType == WEAPON_TYPE_SHOTGUN && m_pLocal->iShotsFired > int(Settings::Aimbot::aim_RcsAfterXShot[0])) || (m_pLocal->WeaponType == WEAPON_TYPE_RIFLE && m_pLocal->iShotsFired > int(Settings::Aimbot::aim_RcsAfterXShot[0])) || (m_pLocal->WeaponType == WEAPON_TYPE_MACHINEGUN && m_pLocal->iShotsFired > int(Settings::Aimbot::aim_RcsAfterXShot[0])))
					bEnableRcs = true;

				if (m_pLocal->WeaponType == WEAPON_TYPE_PISTOL && m_pLocal->iShotsFired <= int(Settings::Aimbot::aim_RcsAfterXShot[3]))
					bEnableRcs = true;
			}
		}
		
	}

	//SMG
	if (m_pLocal->WeaponType == WEAPON_TYPE_SMG)
	{
		if (Settings::Aimbot::aim_RCSType[1] <= 0)
		{
			if (Settings::Aimbot::aim_Rcs[1])
			{
				if ((m_pLocal->WeaponType == WEAPON_TYPE_SHOTGUN && m_pLocal->iShotsFired > 1) || (m_pLocal->WeaponType == WEAPON_TYPE_RIFLE && m_pLocal->iShotsFired > 1) || (m_pLocal->WeaponType == WEAPON_TYPE_MACHINEGUN && m_pLocal->iShotsFired > 1) || (m_pLocal->WeaponType == WEAPON_TYPE_SMG && m_pLocal->iShotsFired > 1))
					bEnableRcs = true;

				if (m_pLocal->WeaponType == WEAPON_TYPE_PISTOL && m_pLocal->iShotsFired <= 1)
					bEnableRcs = true;
			}
		}
		else if (Settings::Aimbot::aim_RCSType[1] >= 1)
		{
			if (Settings::Aimbot::aim_Rcsx[1] && Settings::Aimbot::aim_Rcsy[1])
			{
				if ((m_pLocal->WeaponType == WEAPON_TYPE_SHOTGUN && m_pLocal->iShotsFired > int(Settings::Aimbot::aim_RcsAfterXShot[1])) || (m_pLocal->WeaponType == WEAPON_TYPE_RIFLE && m_pLocal->iShotsFired > int(Settings::Aimbot::aim_RcsAfterXShot[1])) || (m_pLocal->WeaponType == WEAPON_TYPE_MACHINEGUN && m_pLocal->iShotsFired > int(Settings::Aimbot::aim_RcsAfterXShot[1])))
					bEnableRcs = true;

				if (m_pLocal->WeaponType == WEAPON_TYPE_PISTOL && m_pLocal->iShotsFired <= int(Settings::Aimbot::aim_RcsAfterXShot[3]))
					bEnableRcs = true;
			}
		}
	}

	//RIFLE
	if (m_pLocal->WeaponType == WEAPON_TYPE_RIFLE)
	{
		if (Settings::Aimbot::aim_RCSType[2] <= 0)
		{
			if (Settings::Aimbot::aim_Rcs[2])
			{
				if ((m_pLocal->WeaponType == WEAPON_TYPE_SHOTGUN && m_pLocal->iShotsFired > 1) || (m_pLocal->WeaponType == WEAPON_TYPE_RIFLE && m_pLocal->iShotsFired > 1) || (m_pLocal->WeaponType == WEAPON_TYPE_MACHINEGUN && m_pLocal->iShotsFired > 1))
					bEnableRcs = true;

				if (m_pLocal->WeaponType == WEAPON_TYPE_PISTOL && m_pLocal->iShotsFired <= 1)
					bEnableRcs = true;
			}
		}
		else if (Settings::Aimbot::aim_RCSType[2] >= 1)
		{
			if (Settings::Aimbot::aim_Rcsx[2] && Settings::Aimbot::aim_Rcsy[2])
			{
				if ((m_pLocal->WeaponType == WEAPON_TYPE_SHOTGUN && m_pLocal->iShotsFired > int(Settings::Aimbot::aim_RcsAfterXShot[2])) || (m_pLocal->WeaponType == WEAPON_TYPE_RIFLE && m_pLocal->iShotsFired > int(Settings::Aimbot::aim_RcsAfterXShot[2])) || (m_pLocal->WeaponType == WEAPON_TYPE_MACHINEGUN && m_pLocal->iShotsFired > int(Settings::Aimbot::aim_RcsAfterXShot[2])))
					bEnableRcs = true;

				if (m_pLocal->WeaponType == WEAPON_TYPE_PISTOL && m_pLocal->iShotsFired <= int(Settings::Aimbot::aim_RcsAfterXShot[3]))
					bEnableRcs = true;
			}
		}
		
	}

	//PISTOLA
	if (m_pLocal->WeaponType == WEAPON_TYPE_PISTOL)
	{
		if (Settings::Aimbot::aim_RCSType[3] <= 0)
		{
			if (Settings::Aimbot::aim_Rcs[3])
			{
				if ((m_pLocal->WeaponType == WEAPON_TYPE_SHOTGUN && m_pLocal->iShotsFired > 1) || (m_pLocal->WeaponType == WEAPON_TYPE_RIFLE && m_pLocal->iShotsFired > 1) || (m_pLocal->WeaponType == WEAPON_TYPE_MACHINEGUN && m_pLocal->iShotsFired > 1))
					bEnableRcs = true;

				if (m_pLocal->WeaponType == WEAPON_TYPE_PISTOL && m_pLocal->iShotsFired <= 1)
					bEnableRcs = true;
			}
		}
		else if (Settings::Aimbot::aim_RCSType[3] >= 1)
		{
			if (Settings::Aimbot::aim_Rcsx[3] && Settings::Aimbot::aim_Rcsy[3])
			{
				if ((m_pLocal->WeaponType == WEAPON_TYPE_SHOTGUN && m_pLocal->iShotsFired > int(Settings::Aimbot::aim_RcsAfterXShot[3])) || (m_pLocal->WeaponType == WEAPON_TYPE_RIFLE && m_pLocal->iShotsFired > int(Settings::Aimbot::aim_RcsAfterXShot[3])) || (m_pLocal->WeaponType == WEAPON_TYPE_MACHINEGUN && m_pLocal->iShotsFired > int(Settings::Aimbot::aim_RcsAfterXShot[3])))
					bEnableRcs = true;

				if (m_pLocal->WeaponType == WEAPON_TYPE_PISTOL && m_pLocal->iShotsFired <= int(Settings::Aimbot::aim_RcsAfterXShot[3]))
					bEnableRcs = true;
			}
		}
		
	}

	//ESCOPETA
	if (m_pLocal->WeaponType == WEAPON_TYPE_SHOTGUN)
	{
		if (Settings::Aimbot::aim_RCSType[0] <= 0)
		{
			if (Settings::Aimbot::aim_Rcs[0])
			{
				if ((m_pLocal->WeaponType == WEAPON_TYPE_SHOTGUN && m_pLocal->iShotsFired > 1) || (m_pLocal->WeaponType == WEAPON_TYPE_RIFLE && m_pLocal->iShotsFired > 1) || (m_pLocal->WeaponType == WEAPON_TYPE_MACHINEGUN && m_pLocal->iShotsFired > 1))
					bEnableRcs = true;

				if (m_pLocal->WeaponType == WEAPON_TYPE_PISTOL && m_pLocal->iShotsFired <= 1)
					bEnableRcs = true;
			}
		}
		else if (Settings::Aimbot::aim_RCSType[0] >= 1)
		{
			if (Settings::Aimbot::aim_Rcsx[0] && Settings::Aimbot::aim_Rcsy[0])
			{
				if ((m_pLocal->WeaponType == WEAPON_TYPE_SHOTGUN && m_pLocal->iShotsFired > int(Settings::Aimbot::aim_RcsAfterXShot[0])) || (m_pLocal->WeaponType == WEAPON_TYPE_RIFLE && m_pLocal->iShotsFired > int(Settings::Aimbot::aim_RcsAfterXShot[0])) || (m_pLocal->WeaponType == WEAPON_TYPE_MACHINEGUN && m_pLocal->iShotsFired > int(Settings::Aimbot::aim_RcsAfterXShot[0])))
					bEnableRcs = true;

				if (m_pLocal->WeaponType == WEAPON_TYPE_PISTOL && m_pLocal->iShotsFired <= int(Settings::Aimbot::aim_RcsAfterXShot[3]))
					bEnableRcs = true;
			}
		}
	}

	//FRANCO

	if (m_pLocal->WeaponType == WEAPON_TYPE_SNIPER)
	{
		if (Settings::Aimbot::aim_RCSType[5] <= 0)
		{
			if (Settings::Aimbot::aim_Rcs[5])
			{
				if ((m_pLocal->WeaponType == WEAPON_TYPE_SHOTGUN && m_pLocal->iShotsFired > 1) || (m_pLocal->WeaponType == WEAPON_TYPE_RIFLE && m_pLocal->iShotsFired > 1) || (m_pLocal->WeaponType == WEAPON_TYPE_MACHINEGUN && m_pLocal->iShotsFired > 1))
					bEnableRcs = true;

				if (m_pLocal->WeaponType == WEAPON_TYPE_PISTOL && m_pLocal->iShotsFired <= 1)
					bEnableRcs = true;
			}
		}
		else if (Settings::Aimbot::aim_RCSType[5] >= 1)
		{
			if (Settings::Aimbot::aim_Rcsx[5] && Settings::Aimbot::aim_Rcsy[5])
			{
				if ((m_pLocal->WeaponType == WEAPON_TYPE_SHOTGUN && m_pLocal->iShotsFired > int(Settings::Aimbot::aim_RcsAfterXShot[5])) || (m_pLocal->WeaponType == WEAPON_TYPE_RIFLE && m_pLocal->iShotsFired > int(Settings::Aimbot::aim_RcsAfterXShot[5])) || (m_pLocal->WeaponType == WEAPON_TYPE_MACHINEGUN && m_pLocal->iShotsFired > int(Settings::Aimbot::aim_RcsAfterXShot[5])))
					bEnableRcs = true;

				if (m_pLocal->WeaponType == WEAPON_TYPE_PISTOL && m_pLocal->iShotsFired <= int(Settings::Aimbot::aim_RcsAfterXShot[5]))
					bEnableRcs = true;
			}
		}
		
	}







	








	if (bEnableRcs)
	{
		//MACHINEGUN
		if (m_pLocal->WeaponType == WEAPON_TYPE_MACHINEGUN)
		{
			if (Settings::Aimbot::aim_RCSType[0] <= 0)
			{
				float MulVar = Settings::Aimbot::aim_Rcs[0] * 0.02f;
				if (Settings::Aimbot::aim_RcsType[0] <= 0)
				{
					vAimAngle -= m_pLocal->vAimPunch * MulVar;
				}
				else if (Settings::Aimbot::aim_RcsType[0] == 1)
				{
					vAimAngle -= (m_pLocal->vPunch + m_pLocal->vAimPunch) * MulVar;
				}
				else if (Settings::Aimbot::aim_RcsType[0] >= 2)
				{
					vAimAngle -= (m_pLocal->vPunch + (m_pLocal->vAimPunch * 2 * 0.5f)) * MulVar;
				}
			}
			else if (Settings::Aimbot::aim_RCSType[0] >= 1)
			{
				if (Settings::Aimbot::aim_RcsType[0] == 0)
				{
					vAimAngle -= m_pLocal->vAimPunch * Vector(Settings::Aimbot::aim_Rcsx[0] * 0.0221521f, Settings::Aimbot::aim_Rcsy[0] * 0.0221521f, 1);
				}
				else if (Settings::Aimbot::aim_RcsType[0] == 1)
				{
					vAimAngle -= (m_pLocal->vPunch + m_pLocal->vAimPunch) * Vector(Settings::Aimbot::aim_Rcsx[0] * 0.0221521f, Settings::Aimbot::aim_Rcsy[0] * 0.0221521f, 1);
				}
				else if (Settings::Aimbot::aim_RcsType[0] == 2)
				{
					vAimAngle -= (m_pLocal->vPunch + (m_pLocal->vAimPunch * 2 * 0.5f)) * Vector(Settings::Aimbot::aim_Rcsx[0] * 0.0221521f, Settings::Aimbot::aim_Rcsy[0] * 0.0221521f, 1);
				}
			}
			
		}

		//SMG
		if (m_pLocal->WeaponType == WEAPON_TYPE_SMG)
		{
			if (Settings::Aimbot::aim_RCSType[1] <= 0)
			{
				float MulVar = Settings::Aimbot::aim_Rcs[1] * 0.02f;
				if (Settings::Aimbot::aim_RcsType[1] <= 0)
				{
					vAimAngle -= m_pLocal->vAimPunch * MulVar;
				}
				else if (Settings::Aimbot::aim_RcsType[1] == 1)
				{
					vAimAngle -= (m_pLocal->vPunch + m_pLocal->vAimPunch) * MulVar;
				}
				else if (Settings::Aimbot::aim_RcsType[1] >= 2)
				{
					vAimAngle -= (m_pLocal->vPunch + (m_pLocal->vAimPunch * 2 * 0.5f)) * MulVar;
				}
			}
			else if (Settings::Aimbot::aim_RCSType[1] >= 1)
			{
				if (Settings::Aimbot::aim_RcsType[1] == 0)
				{
					vAimAngle -= m_pLocal->vAimPunch * Vector(Settings::Aimbot::aim_Rcsx[1] * 0.0221521f, Settings::Aimbot::aim_Rcsy[1] * 0.0221521f, 1);
				}
				else if (Settings::Aimbot::aim_RcsType[1] == 1)
				{
					vAimAngle -= (m_pLocal->vPunch + m_pLocal->vAimPunch) * Vector(Settings::Aimbot::aim_Rcsx[1] * 0.0221521f, Settings::Aimbot::aim_Rcsy[1] * 0.0221521f, 1);
				}
				else if (Settings::Aimbot::aim_RcsType[1] == 2)
				{
					vAimAngle -= (m_pLocal->vPunch + (m_pLocal->vAimPunch * 2 * 0.5f)) * Vector(Settings::Aimbot::aim_Rcsx[1] * 0.0221521f, Settings::Aimbot::aim_Rcsy[1] * 0.0221521f, 1);
				}
			}
			
		}

		//RIFLE
		if (m_pLocal->WeaponType == WEAPON_TYPE_RIFLE)
		{
			if (Settings::Aimbot::aim_RCSType[2] <= 0)
			{
				float MulVar = Settings::Aimbot::aim_Rcs[2] * 0.02f;
				if (Settings::Aimbot::aim_RcsType[2] <= 0)
				{
					vAimAngle -= m_pLocal->vAimPunch * MulVar;
				}
				else if (Settings::Aimbot::aim_RcsType[2] == 1)
				{
					vAimAngle -= (m_pLocal->vPunch + m_pLocal->vAimPunch) * MulVar;
				}
				else if (Settings::Aimbot::aim_RcsType[2] >= 2)
				{
					vAimAngle -= (m_pLocal->vPunch + (m_pLocal->vAimPunch * 2 * 0.5f)) * MulVar;
				}
			}
			else if (Settings::Aimbot::aim_RCSType[2] >= 1)
			{
				if (Settings::Aimbot::aim_RcsType[2] == 0)
				{
					vAimAngle -= m_pLocal->vAimPunch * Vector(Settings::Aimbot::aim_Rcsx[2] * 0.0221521f, Settings::Aimbot::aim_Rcsy[2] * 0.0221521f, 1);
				}
				else if (Settings::Aimbot::aim_RcsType[2] == 1)
				{
					vAimAngle -= (m_pLocal->vPunch + m_pLocal->vAimPunch) * Vector(Settings::Aimbot::aim_Rcsx[2] * 0.0221521f, Settings::Aimbot::aim_Rcsy[2] * 0.0221521f, 1);
				}
				else if (Settings::Aimbot::aim_RcsType[2] == 2)
				{
					vAimAngle -= (m_pLocal->vPunch + (m_pLocal->vAimPunch * 2 * 0.5f)) * Vector(Settings::Aimbot::aim_Rcsx[2] * 0.0221521f, Settings::Aimbot::aim_Rcsy[2] * 0.0221521f, 1);
				}
			}
			
		}

		//PISTOLA
		if (m_pLocal->WeaponType == WEAPON_TYPE_PISTOL)
		{
			if (Settings::Aimbot::aim_RCSType[3] <= 0)
			{
				float MulVar = Settings::Aimbot::aim_Rcs[3] * 0.02f;
				if (Settings::Aimbot::aim_RcsType[3] <= 0)
				{
					vAimAngle -= m_pLocal->vAimPunch * MulVar;
				}
				else if (Settings::Aimbot::aim_RcsType[3] == 1)
				{
					vAimAngle -= (m_pLocal->vPunch + m_pLocal->vAimPunch) * MulVar;
				}
				else if (Settings::Aimbot::aim_RcsType[3] >= 2)
				{
					vAimAngle -= (m_pLocal->vPunch + (m_pLocal->vAimPunch * 2 * 0.5f)) * MulVar;
				}
			}
			else if (Settings::Aimbot::aim_RCSType[3] >= 1)
			{
				if (Settings::Aimbot::aim_RcsType[3] == 0)
				{
					vAimAngle -= m_pLocal->vAimPunch * Vector(Settings::Aimbot::aim_Rcsx[3] * 0.0221521f, Settings::Aimbot::aim_Rcsy[3] * 0.0221521f, 1);
				}
				else if (Settings::Aimbot::aim_RcsType[3] == 1)
				{
					vAimAngle -= (m_pLocal->vPunch + m_pLocal->vAimPunch) * Vector(Settings::Aimbot::aim_Rcsx[3] * 0.0221521f, Settings::Aimbot::aim_Rcsy[3] * 0.0221521f, 1);
				}
				else if (Settings::Aimbot::aim_RcsType[3] == 2)
				{
					vAimAngle -= (m_pLocal->vPunch + (m_pLocal->vAimPunch * 2 * 0.5f)) * Vector(Settings::Aimbot::aim_Rcsx[3] * 0.0221521f, Settings::Aimbot::aim_Rcsy[3] * 0.0221521f, 1);
				}
			}
			
		}

		//ESCOPETA
		if (m_pLocal->WeaponType == WEAPON_TYPE_SHOTGUN)
		{
			if (Settings::Aimbot::aim_RCSType[4] <= 0)
			{
				float MulVar = Settings::Aimbot::aim_Rcs[4] * 0.02f;
				if (Settings::Aimbot::aim_RcsType[4] <= 0)
				{
					vAimAngle -= m_pLocal->vAimPunch * MulVar;
				}
				else if (Settings::Aimbot::aim_RcsType[4] == 1)
				{
					vAimAngle -= (m_pLocal->vPunch + m_pLocal->vAimPunch) * MulVar;
				}
				else if (Settings::Aimbot::aim_RcsType[4] >= 2)
				{
					vAimAngle -= (m_pLocal->vPunch + (m_pLocal->vAimPunch * 2 * 0.5f)) * MulVar;
				}
			}
			else if (Settings::Aimbot::aim_RCSType[4] >= 1)
			{
				if (Settings::Aimbot::aim_RcsType[4] == 0)
				{
					vAimAngle -= m_pLocal->vAimPunch * Vector(Settings::Aimbot::aim_Rcsx[4] * 0.0221521f, Settings::Aimbot::aim_Rcsy[4] * 0.0221521f, 1);
				}
				else if (Settings::Aimbot::aim_RcsType[4] == 1)
				{
					vAimAngle -= (m_pLocal->vPunch + m_pLocal->vAimPunch) * Vector(Settings::Aimbot::aim_Rcsx[4] * 0.0221521f, Settings::Aimbot::aim_Rcsy[4] * 0.0221521f, 1);
				}
				else if (Settings::Aimbot::aim_RcsType[4] == 2)
				{
					vAimAngle -= (m_pLocal->vPunch + (m_pLocal->vAimPunch * 2 * 0.5f)) * Vector(Settings::Aimbot::aim_Rcsx[4] * 0.0221521f, Settings::Aimbot::aim_Rcsy[4] * 0.0221521f, 1);
				}
			}
		}

		//FRANCO

		if (m_pLocal->WeaponType == WEAPON_TYPE_SNIPER)
		{

			if (Settings::Aimbot::aim_RCSType[5] <= 0)
			{
				float MulVar = Settings::Aimbot::aim_Rcs[5] * 0.02f;
				if (Settings::Aimbot::aim_RcsType[5] <= 0)
				{
					vAimAngle -= m_pLocal->vAimPunch * MulVar;
				}
				else if (Settings::Aimbot::aim_RcsType[5] == 1)
				{
					vAimAngle -= (m_pLocal->vPunch + m_pLocal->vAimPunch) * MulVar;
				}
				else if (Settings::Aimbot::aim_RcsType[5] >= 2)
				{
					vAimAngle -= (m_pLocal->vPunch + (m_pLocal->vAimPunch * 2 * 0.5f)) * MulVar;
				}
			}
			else if (Settings::Aimbot::aim_RCSType[5] >= 1)
			{
				if (Settings::Aimbot::aim_RcsType[5] == 0)
				{
					vAimAngle -= m_pLocal->vAimPunch * Vector(Settings::Aimbot::aim_Rcsx[5] * 0.0221521f, Settings::Aimbot::aim_Rcsy[5] * 0.0221521f, 1);
				}
				else if (Settings::Aimbot::aim_RcsType[5] == 1)
				{
					vAimAngle -= (m_pLocal->vPunch + m_pLocal->vAimPunch) * Vector(Settings::Aimbot::aim_Rcsx[5] * 0.0221521f, Settings::Aimbot::aim_Rcsy[5] * 0.0221521f, 1);
				}
				else if (Settings::Aimbot::aim_RcsType[5] == 2)
				{
					vAimAngle -= (m_pLocal->vPunch + (m_pLocal->vAimPunch * 2 * 0.5f)) * Vector(Settings::Aimbot::aim_Rcsx[5] * 0.0221521f, Settings::Aimbot::aim_Rcsy[5] * 0.0221521f, 1);
				}
			}
		}





		

	}

	float fSmooth = 10.f;

	//MACHINEGUN
	if (m_pLocal->WeaponType == WEAPON_TYPE_MACHINEGUN)
	{
		if (bEnableRcs)
		{
			fSmooth = Settings::Aimbot::aim_RcsSmooth[0];
		}
		else
		{
			fSmooth = Settings::Aimbot::aim_Smooth[0];
		}
	
	}

	//SMG
	if (m_pLocal->WeaponType == WEAPON_TYPE_SMG)
	{
		if (bEnableRcs)
		{
			fSmooth = Settings::Aimbot::aim_RcsSmooth[1];
		}
		else
		{
			fSmooth = Settings::Aimbot::aim_Smooth[1];
		}

	}

	//RIFLE
	if (m_pLocal->WeaponType == WEAPON_TYPE_RIFLE)
	{
		if (bEnableRcs)
		{
			fSmooth = Settings::Aimbot::aim_RcsSmooth[2];
		}
		else
		{
			fSmooth = Settings::Aimbot::aim_Smooth[2];
		}
		
	}


	//ESCOPETA
	if (m_pLocal->WeaponType == WEAPON_TYPE_SHOTGUN)
	{
		if (bEnableRcs)
		{
			fSmooth = Settings::Aimbot::aim_RcsSmooth[4];
		}
		else
		{
			fSmooth = Settings::Aimbot::aim_Smooth[4];
		}
	}

	//pistola
	if (m_pLocal->WeaponType == WEAPON_TYPE_PISTOL)
	{
		
		fSmooth = Settings::Aimbot::aim_Smooth[3];
		
	}

	//franco
	if (m_pLocal->WeaponType == WEAPON_TYPE_SNIPER)
	{
		
		
		
			fSmooth = Settings::Aimbot::aim_Smooth[5];
		
	}


	

	AngleNormalize(vAimAngle);

	if (fSmooth < 10.f)
		fSmooth = 10.f;

	fSmooth = fSmooth / 10.f;

	SmoothAngles(m_pCmd->viewangles, vAimAngle, vSmoothAimAngle, fSmooth);

	m_pCmd->viewangles = vSmoothAimAngle;
}

void CAimbot::AutoPistol()
{
	float NextAttack = m_pLocal->m_pWeaponEntity->GetNextPrimaryAttack();
	float Tick = m_pLocal->m_pEntity->GetTickBase() * Interfaces::GlobalVars()->interval_per_tick;

	if (NextAttack < Tick)
		return;

	if (*m_pLocal->m_pWeaponEntity->GeteAttributableItem()->GetItemDefinitionIndex() == WEAPON_REVOLVER)
	{
		m_pCmd->buttons &= ~IN_ATTACK2;
	}
	else
	{
		m_pCmd->buttons &= ~IN_ATTACK;
	}
}

void CAimbot::KillDelay(IGameEvent* pEvent)
{
	if (!strcmp(pEvent->GetName(), "player_death"))
	{
		if (Interfaces::Engine()->GetPlayerForUserID(pEvent->GetInt("attacker")) == Interfaces::Engine()->GetLocalPlayer())
		{
			if (Settings::Aimbot::aim_KillDelayTime == 0)
			{
				Settings::Misc::KillDelayBool2 = false;
			}
			else
			{
				Settings::Misc::KillDelayVar = Interfaces::GlobalVars()->tickcount + Settings::Aimbot::aim_KillDelayTime;
			}
		}
	}
	if (!strcmp(pEvent->GetName(), "game_newmap"))
	{
		Settings::Misc::KillDelayBool2 = false;
		Settings::Aimbot::aim_KillDelayTime;
	}
}

void CAimbot::OnCreateMove(CUserCmd * pCmd, CMe * pLocal)
{
	m_pLocal = pLocal;
	m_pCmd = pCmd;
	

	Aimbot();
}