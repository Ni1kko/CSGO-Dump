#pragma once

#include "../../Engine/Engine.h"

#define TT_CT_BEST_HIT_SIZE_1 ( sizeof( tt_ct_best_hit_1 ) / sizeof( *tt_ct_best_hit_1 ) )
#define TT_CT_BEST_HIT_SIZE_2 ( sizeof( tt_ct_best_hit_2 ) / sizeof( *tt_ct_best_hit_2 ) )
#define TT_CT_BEST_HIT_SIZE_3 ( sizeof( tt_ct_best_hit_3 ) / sizeof( *tt_ct_best_hit_3 ) )
#define TT_CT_BEST_HIT_SIZE_4 ( sizeof( tt_ct_best_hit_4 ) / sizeof( *tt_ct_best_hit_4 ) )
class CTimer;

namespace Engine
{
	class CMe;
	class CPlayer;
}

using namespace Engine;

enum AIM_ATTACK_TYPE
{
	AA_SHOTGUN ,
	AA_SNIPER_PISTOL
};



class CAimbot
{
public:

	CAimbot();

//[swap_lines]
	bool	IsEnable();
	int		GetPlayerFov( CPlayer* pPlayer );

	int		GetBestTarget();
	int		GetBestHitBox();

	void	OnRender();



	void	Aimbot();
	void	AimbotSet();



	void	AutoPistol();
	void    KillDelay(IGameEvent * pEvent);
	void	OnCreateMove( CUserCmd* pCmd , CMe* pLocal );
	CMe*		m_pLocal;

	CUserCmd*	m_pCmd;
//[/swap_lines]
private:

	//CMe*		m_pLocal;
	//CUserCmd*	m_pCmd;

	int		m_iBestPreTarget;

	int		m_iBestTarget;
	int		m_iBestHitbox;

	bool	m_bClamp;
	bool	m_bAttack;
	bool	m_bAimShot;
	bool	m_bTargetFov;
	bool	m_bTargetChange;
	bool	m_bAutoPistolEn;

	CTimer*	m_pShotDelay;

	Vector	m_vAimBestHitbox;
	Vector  m_vAimBestHitboxScreen;
};