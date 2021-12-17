#pragma once

#include "sdk.h"

enum ItemSchemaIndex_e
{
	WEAPON_NONE = 0,
	WEAPON_DEAGLE,
	WEAPON_DUALS,
	WEAPON_FIVE7,
	WEAPON_GLOCK,
	WEAPON_AK47 = 7,
	WEAPON_AUG,
	WEAPON_AWP,
	WEAPON_FAMAS,
	WEAPON_G3SG1,
	WEAPON_GALIL = 13,
	WEAPON_M249,
	WEAPON_M4A1 = 16,
	WEAPON_MAC10,
	WEAPON_P90 = 19,
	WEAPON_MP5 = 23,
	WEAPON_UMP45,
	WEAPON_XM1014,
	WEAPON_BIZON,
	WEAPON_MAG7,
	WEAPON_NEGEV,
	WEAPON_SAWEDOFF,
	WEAPON_TEC9,
	WEAPON_TASER,
	WEAPON_P2000,
	WEAPON_MP7,
	WEAPON_MP9,
	WEAPON_NOVA,
	WEAPON_P250,
	WEAPON_SHIELD,
	WEAPON_SCAR20,
	WEAPON_SG553,
	WEAPON_SCOUT,
	WEAPON_KNIFEGG,
	WEAPON_KNIFE,
	WEAPON_FLASH,
	WEAPON_HE,
	WEAPON_SMOKE,
	WEAPON_MOLOTOV,
	WEAPON_DECOY,
	WEAPON_INC,
	WEAPON_C4,
	WEAPON_HEALTHSHOT = 57,
	WEAPON_KNIFE_T = 59,
	WEAPON_M4A1S,
	WEAPON_USPS,
	WEAPON_CZ75 = 63,
	WEAPON_R8REVOLVER,
	WEAPON_TAGRENADE = 68,
	WEAPON_FISTS,
	WEAPON_BREACHCHARGE,
	WEAPON_TABLET = 72,
	WEAPON_MELEE = 74,
	WEAPON_AXE,
	WEAPON_HAMMER,
	WEAPON_SPANNER = 78,
	WEAPON_KNIFE_GHOST = 80,
	WEAPON_FIREBOMB,
	WEAPON_DIVERSION,
	WEAPON_FRAG_GRENADE,
	WEAPON_SNOWBALL,
	WEAPON_BUMPMINE,
	WEAPON_KNIFE_BAYONET = 500,
	WEAPON_KNIFE_FLIP = 505,
	WEAPON_KNIFE_GUT,
	WEAPON_KNIFE_KARAMBIT,
	WEAPON_KNIFE_M9BAYONET,
	WEAPON_KNIFE_HUNTSMAN,
	WEAPON_KNIFE_FALCHION = 512,
	WEAPON_KNIFE_BOWIE = 514,
	WEAPON_KNIFE_BUTTERFLY,
	WEAPON_KNIFE_SHADOWDAGGER,
	WEAPON_KNIFE_URSUS = 519,
	WEAPON_KNIFE_GYPSY_JACKKNIFE,
	WEAPON_KNIFE_STILETTO = 522,
	WEAPON_KNIFE_WIDOWMAKER,
	GLOVE_STUDDED_BLOODHOUND = 5027,
	GLOVE_T_SIDE = 5028,
	GLOVE_CT_SIDE = 5029,
	GLOVE_SPORTY = 5030,
	GLOVE_SLICK = 5031,
	GLOVE_LEATHER_WRAP = 5032,
	GLOVE_MOTORCYCLE = 5033,
	GLOVE_SPECIALIST = 5034,
	GLOVE_HYDRA = 5035
};

class CBaseCombatWeapon : public CBaseEntity
{
public:
	char* GetCustomIcon()
	{
		switch( this->GetWeaponID() )
		{
		case WEAPON_DEAGLE:
			return "A"; break;
		case WEAPON_DUALS:
			return "B"; break;
		case WEAPON_FIVE7:
			return "C"; break;
		case WEAPON_GLOCK:
			return "D"; break;
		case WEAPON_AK47:
			return "W"; break;
		case WEAPON_AUG:
			return "U"; break;
		case WEAPON_AWP:
			return "Z"; break;
		case WEAPON_FAMAS:
			return "R"; break;
		case WEAPON_G3SG1:
			return "X"; break;
		case WEAPON_GALIL:
			return "Q"; break;
		case WEAPON_M249:
			return "g"; break;
		case WEAPON_M4A1:
			return "S"; break;
		case WEAPON_MAC10:
			return "K"; break;
		case WEAPON_P90:
			return "P"; break;
		case WEAPON_MP9:
			return "O"; break;
		case WEAPON_UMP45:
			return "L"; break;
		case WEAPON_XM1014:
			return "b"; break;
		case WEAPON_BIZON:
		case WEAPON_MP5:
			return "M"; break;
		case WEAPON_MAG7:
			return "d"; break;
		case WEAPON_NEGEV:
			return "f"; break;
		case WEAPON_SAWEDOFF:
			return "c"; break;
		case WEAPON_TEC9:
			return "H"; break;
		case WEAPON_TASER:
			return "h"; break;
		case WEAPON_P2000:
			return "E"; break;
		case WEAPON_MP7:
			return "N"; break;
		case WEAPON_NOVA:
			return "e"; break;
		case WEAPON_P250:
			return "F"; break;
		case WEAPON_SCAR20:
			return "Y"; break;
		case WEAPON_SG553:
			return "V"; break;
		case WEAPON_SCOUT:
			return "a"; break;
		case WEAPON_R8REVOLVER:
			return "J"; break;
		case WEAPON_C4:
			return "o"; break;
		case WEAPON_KNIFE:
			return "["; break;
		case WEAPON_KNIFE_T:
			return "]"; break;
		case WEAPON_KNIFE_GUT:
			return "3"; break;
		case WEAPON_KNIFE_FLIP:
			return "2"; break;
		case WEAPON_KNIFE_BAYONET:
			return "1"; break;
		case WEAPON_KNIFE_KARAMBIT:
			return "4"; break;
		case WEAPON_KNIFE_M9BAYONET:
			return "5"; break;
		case WEAPON_KNIFE_BUTTERFLY:
			return "8"; break;
		case WEAPON_KNIFE_FALCHION:
			return "0"; break;
		case WEAPON_KNIFE_SHADOWDAGGER:
			return "9"; break;
		case WEAPON_KNIFE_BOWIE:
			return "7"; break;
		case WEAPON_KNIFE_HUNTSMAN:
			return "6"; break;
		case WEAPON_FLASH:
			return "i"; break;
		case WEAPON_HE:
			return "j"; break;
		case WEAPON_SMOKE:
			return "k"; break;
		case WEAPON_MOLOTOV:
			return "l"; break;
		case WEAPON_DECOY:
			return "m"; break;
		case WEAPON_INC:
			return "n"; break;
		case WEAPON_M4A1S:
			return "T"; break;
		case WEAPON_USPS:
			return "G"; break;
		case WEAPON_CZ75:
			return "I"; break;
		}
		return "";
	}

	const char *GetWeaponName()
	{
		switch( this->GetWeaponID() )
		{
		case WEAPON_DEAGLE:
			return "deagle"; break;
		case WEAPON_DUALS:
			return "duals"; break;
		case WEAPON_FIVE7:
			return "five seven"; break;
		case WEAPON_GLOCK:
			return "glock"; break;
		case WEAPON_AK47:
			return "ak47"; break;
		case WEAPON_AUG:
			return "aug"; break;
		case WEAPON_AWP:
			return "awp"; break;
		case WEAPON_FAMAS:
			return "famas"; break;
		case WEAPON_G3SG1:
			return "g3sg1"; break;
		case WEAPON_GALIL:
			return "galil"; break;
		case WEAPON_M249:
			return "m249"; break;
		case WEAPON_M4A1:
			return "m4a1"; break;
		case WEAPON_MAC10:
			return "mac10"; break;
		case WEAPON_P90:
			return "p90"; break;
		case WEAPON_MP9:
			return "mp9"; break;
		case WEAPON_UMP45:
			return "ump45"; break;
		case WEAPON_XM1014:
			return "xm1014"; break;
		case WEAPON_BIZON:
			return "bizon"; break;
		case WEAPON_MP5:
			return "mp5"; break;
		case WEAPON_MAG7:
			return "mag7"; break;
		case WEAPON_NEGEV:
			return "negev"; break;
		case WEAPON_SAWEDOFF:
			return "sawed off"; break;
		case WEAPON_TEC9:
			return "tec9"; break;
		case WEAPON_TASER:
			return "taser"; break;
		case WEAPON_P2000:
			return "p2000"; break;
		case WEAPON_MP7:
			return "mp7"; break;
		case WEAPON_NOVA:
			return "nova"; break;
		case WEAPON_P250:
			return "p250"; break;
		case WEAPON_SCAR20:
			return "scar20"; break;
		case WEAPON_SG553:
			return "sg553"; break;
		case WEAPON_SCOUT:
			return "scout"; break;
		case WEAPON_R8REVOLVER:
			return "revolver"; break;
		case WEAPON_C4:
			return "c4"; break;
		case WEAPON_KNIFE:
			return "ct_knife"; break;
		case WEAPON_KNIFE_T:
			return "t_knife"; break;
		case WEAPON_KNIFE_GUT:
			return "gut_knife"; break;
		case WEAPON_KNIFE_FLIP:
			return "flip_knife"; break;
		case WEAPON_KNIFE_BAYONET:
			return "bayonet"; break;
		case WEAPON_KNIFE_KARAMBIT:
			return "karambit"; break;
		case WEAPON_KNIFE_M9BAYONET:
			return "m9_bayonet"; break;
		case WEAPON_KNIFE_BUTTERFLY:
			return "butterfly"; break;
		case WEAPON_KNIFE_FALCHION:
			return "falchion"; break;
		case WEAPON_KNIFE_SHADOWDAGGER:
			return "dagger"; break;
		case WEAPON_KNIFE_BOWIE:
			return "bowie"; break;
		case WEAPON_KNIFE_HUNTSMAN:
			return "huntsman"; break;
		case WEAPON_FLASH:
			return "flashbang"; break;
		case WEAPON_HE:
			return "he grenade"; break;
		case WEAPON_SMOKE:
			return "smoke"; break;
		case WEAPON_MOLOTOV:
			return "molotov"; break;
		case WEAPON_DECOY:
			return "decoy"; break;
		case WEAPON_INC:
			return "inc grenade"; break;
		case WEAPON_M4A1S:
			return "m4a1-s"; break;
		case WEAPON_USPS:
			return "usp-s"; break;
		case WEAPON_CZ75:
			return "cz75"; break;
		default:
			return ""; break;
		}
		return "";
	}

	CBaseCombatWeapon* GetActiveWeapon( CBaseEntity *pEntity );

	int GetClip()
	{
		return *( int* )( ( DWORD )this + Offsets::Weapon::m_iClip1 );
	}

	float GetLastShotTime()
	{
		static uintptr_t m_fLastShotTime = g_pNetVars->GetOffset("DT_WeaponCSBase", "m_fLastShotTime");
		return *reinterpret_cast<float*>(uintptr_t(this) + m_fLastShotTime);
	}

	float GetNextPrimaryAttack()
	{
		return *( float* )( ( DWORD )this + Offsets::Weapon::m_flNextPrimaryAttack );
	}

	float GetFireReadyTime()
	{
		return *( float* )( ( DWORD )this + Offsets::Weapon::m_flPostponeFireReadyTime );
	}

	int GetWeaponID()
	{
		return *( short* )( ( DWORD )this + Offsets::Attribute::m_iItemDefinitionIndex );
	}

	int GetZoomLevel()
	{
		return *( int* )( ( DWORD )this + Offsets::Player::m_ZoomLevel );
	}

	bool bPinPulled()
	{
		return *( bool* )( ( DWORD )this + Offsets::Weapon::m_bPinPulled );
	}

	float GetThrowStrength()
	{
		static uintptr_t m_flThrowStrength = g_pNetVars->GetOffset("DT_BaseCSGrenade", "m_flThrowStrength");
		return *reinterpret_cast<float*>(uintptr_t(this) + m_flThrowStrength);
	}

	float GetThrowTime()
	{
		return *( float* )( ( DWORD )this + Offsets::Weapon::m_fThrowTime );
	}

	float GetRecoilIndex()
	{
		return *( float* )( ( DWORD )this + Offsets::Weapon::m_flRecoilIndex );
	}

	/*
	 (*(void (__thiscall **)(int *))(*v2 + 1340))(v2); //spread
        v20 = a2;
        (*(void (__thiscall **)(int *))(*v2 + 1748))(v2); //inaccuracy
        v21 = a2;
        (*(void (__thiscall **)(int *, _DWORD))(*v2 + 1868))(v2, LODWORD(v21)); //updateaccuracy
		*/
	float GetSpread()// "Default.ClipEmpty_Pistol" -> xref of the func is where spread gets calculated
	{
		typedef float( __thiscall* GetSpreadFn )( PVOID );
		GetSpreadFn GetSpread = ( GetSpreadFn )( ( *( PDWORD* )this )[ 452 ] );
		return GetSpread( this );
	}

	float GetInaccuracy()
	{
		typedef float( __thiscall* GetInaccuracyFn )( PVOID );
		GetInaccuracyFn GetCone = ( GetInaccuracyFn )( ( *( PDWORD* )this )[ 482 ] );
		return GetCone( this );
	}

	void UpdateAccuracyPenalty()
	{
		typedef void( __thiscall* UpdateAccuracyPenaltyFn )( PVOID );
		UpdateAccuracyPenaltyFn AccuracyPenalty = ( UpdateAccuracyPenaltyFn )( ( *( PDWORD* )this )[ 483 ] );
		AccuracyPenalty( this );
	}

	CWeaponData* GetWeaponData( void )
	{
		typedef CWeaponData*( __thiscall* GetWeaponData_t )( PVOID );
		return getvfunc< GetWeaponData_t >(this, 460)(this);
	}

	void PreDataUpdate( int updateType )	
	{ 
		PVOID pNetworkable = ( PVOID )( ( DWORD )( this ) + 0x8 );		
		typedef void( __thiscall* OriginalFn )( PVOID, int );		
		return getvfunc<OriginalFn>( pNetworkable, 6 )( pNetworkable, updateType );
	} 	

	void SetGloveModelIndex( int modelIndex )	
	{
		typedef void( __thiscall* OriginalFn )( PVOID, int );		
		getvfunc<OriginalFn>( this, 75 )( this, modelIndex );
	}

	void Release()
	{
		PVOID pNetworkable = (PVOID)((DWORD)(this) + 0x8);
		typedef void(__thiscall* OriginalFn)(PVOID);
		return getvfunc<OriginalFn>(pNetworkable, 1)(pNetworkable);
	}

	void SetDestroyedOnRecreateEntities(void)
	{
		PVOID pNetworkable = (PVOID)((DWORD)(this) + 0x8);
		typedef void(__thiscall* OriginalFn)(PVOID);
		return getvfunc<OriginalFn>(pNetworkable, 13)(pNetworkable);
	}

	HANDLE GetWeaponWorldModel()
	{
		return *( HANDLE* ) ( ( uintptr_t )this + Offsets::Weapon::m_hWeaponWorldModel );
	}

	void SetSkin(int iSeed, int iSkinID, int iStattrak, float flCondition )
	{
		*( PINT )( ( DWORD )this + Offsets::Attribute::m_nFallbackSeed ) = iSeed;
		*( PINT )( ( DWORD )this + Offsets::Attribute::m_nFallbackPaintKit ) = iSkinID;
		*( PFLOAT )( ( DWORD )this + Offsets::Attribute::m_flFallbackWear ) = 0.99f - ( ( flCondition - 2 ) / 100.0f );

	//	if( iStattrak > 0 )
		//	*( PINT )( ( DWORD )this + Offsets::Attribute::m_iEntityQuality ) = 1;

	//	*( PINT )( ( DWORD )this + Offsets::Attribute::m_nFallbackStatTrak ) = iStattrak;

		*( PINT )( ( DWORD )this + Offsets::Attribute::m_iItemIDHigh ) = -1;
	}

	void SetPattern( CBaseEntity* pLocal, const int skin, float condition, int Seed, int stval, std::string name )
	{
		if( skin == 1 )
			return;

		bool st = stval > 2;

		*( PINT ) ( ( DWORD ) this + Offsets::Attribute::m_iItemIDHigh ) = -1; //m_iItemIDHigh

		VEngineClient::player_info_t pInfo;

		if( !g_pEngine->GetPlayerInfo( pLocal->GetIndex(), &pInfo ) )
			return;

		*( PINT ) ( ( DWORD ) this + Offsets::Attribute::m_iAccountID ) = pInfo.xuid;

		if( st && !this->IsKnife() )
			*( PINT ) ( ( DWORD ) this + Offsets::Attribute::m_iEntityQuality ) = 1; // m_iEntityQuality
		else
			if( this->IsKnife() )
				*( PINT ) ( ( DWORD ) this + Offsets::Attribute::m_iEntityQuality ) = 3; // m_iEntityQuality

		*( PINT ) ( ( DWORD ) this + Offsets::Attribute::m_nFallbackPaintKit ) = skin; //m_nFallbackPaintKit

		*( PINT )( ( DWORD )this + Offsets::Attribute::m_nFallbackSeed ) = Seed; //m_nFallbackSeed
		
		*( PFLOAT )( ( DWORD )this + Offsets::Attribute::m_flFallbackWear ) = condition; //m_flFallbackWear
		
		if( st )
			*( PINT )( ( DWORD )this + Offsets::Attribute::m_nFallbackStatTrak ) = stval; //m_nFallbackStatTrak

		if( !name.empty() )
		    strcpy_s( ( char* )this + Offsets::Attribute::m_szCustomName, sizeof( ( char* )this + Offsets::Attribute::m_szCustomName ), name.c_str() ); // m_szCustomName
	}

	bool IsEmpty() 
	{
		return this->GetClip() == 0; 
	}

	int CBaseCombatWeapon::GetPrimaryClip()
	{
		switch( this->GetWeaponID() )
		{
		case WEAPON_NEGEV:
			return 150;
			break;
		case WEAPON_M249:
			return 100;
			break;
		case WEAPON_BIZON:
			return 64;
			break;
		case WEAPON_P90:
			return 50;
			break;
		case WEAPON_GALIL:
			return 35;
			break;
		case WEAPON_DUALS:
		case WEAPON_MP9:
		case WEAPON_MP7:
		case WEAPON_AK47:
		case WEAPON_MAC10:
		case WEAPON_M4A1:
		case WEAPON_SG553:
		case WEAPON_AUG:
		case WEAPON_MP5:
			return 30;
			break;
		case WEAPON_UMP45:
		case WEAPON_FAMAS:
			return 25;
			break;
		case WEAPON_GLOCK:
		case WEAPON_FIVE7:
		case WEAPON_M4A1S:
		case WEAPON_G3SG1:
		case WEAPON_SCAR20:
			return 20; 
			break;
		case WEAPON_TEC9:
			return 18;
			break;
		case WEAPON_P2000:
		case WEAPON_P250:
			return 13;
			break;
		case WEAPON_USPS:
		case WEAPON_CZ75:
			return 12; 
			break;
		case WEAPON_SCOUT:
		case WEAPON_AWP:
			return 10;
			break;
		case WEAPON_R8REVOLVER:
		case WEAPON_NOVA:
			return 8;
			break;
		case WEAPON_DEAGLE:
		case WEAPON_SAWEDOFF:
		case WEAPON_XM1014:
			return 7;
			break;
		case WEAPON_MAG7:
			return 5;
			break;
		}
		return 0;
	}

	int CBaseCombatWeapon::GetGroupID()
	{
		switch( this->GetWeaponID() )
		{
		case WEAPON_DUALS:
		case WEAPON_FIVE7:
		case WEAPON_GLOCK:
		case WEAPON_TEC9:
		case WEAPON_P2000:
		case WEAPON_USPS:
		case WEAPON_P250:
		case WEAPON_CZ75:
		case WEAPON_TASER:
			return 0;
			break;
		case WEAPON_DEAGLE:
		case WEAPON_R8REVOLVER:
			return 1;
			break;
		case WEAPON_MAC10:
		case WEAPON_P90:
		case WEAPON_UMP45:
		case WEAPON_BIZON:
		case WEAPON_MP7:
		case WEAPON_MP9:
		case WEAPON_NEGEV:
		case WEAPON_M249:
		case WEAPON_MP5:
			return 2;
			break;
		case WEAPON_AK47:
		case WEAPON_AUG:
		case WEAPON_FAMAS:
		case WEAPON_GALIL:
		case WEAPON_M4A1:
		case WEAPON_M4A1S:
		case WEAPON_SG553:
			return 3;
			break;
		case WEAPON_XM1014:
		case WEAPON_MAG7:
		case WEAPON_SAWEDOFF:
		case WEAPON_NOVA:
			return 4;
			break;
		case WEAPON_SCOUT:
			return 5;
			break;
		case WEAPON_AWP:
			return 6;
			break;
		case WEAPON_G3SG1:
		case WEAPON_SCAR20:
			return 7;
			break;
		default:
			return 8;
			break;
		}
		return 8;
	}

	bool CBaseCombatWeapon::IsPistol( void )
	{
		switch( this->GetWeaponID() )
		{
		case WEAPON_DEAGLE:
		case WEAPON_DUALS:
		case WEAPON_FIVE7:
		case WEAPON_GLOCK:
		case WEAPON_TEC9:
		case WEAPON_P2000:
		case WEAPON_USPS:
		case WEAPON_P250:
		case WEAPON_CZ75:
		case WEAPON_R8REVOLVER:
			return true;
		default:
			return false;
		}
	}

	bool CBaseCombatWeapon::IsShotgun( void )
	{
		switch( this->GetWeaponID() )
		{
		case WEAPON_XM1014:
		case WEAPON_MAG7:
		case WEAPON_SAWEDOFF:
		case WEAPON_NOVA:
		default:
			return false;
		}
	}

	bool CBaseCombatWeapon::IsAutomatic( void )
	{
		switch( this->GetWeaponID() )
		{
		case WEAPON_AK47:
		case WEAPON_AUG:
		case WEAPON_FAMAS:
		case WEAPON_GALIL:
		case WEAPON_M249:
		case WEAPON_M4A1:
		case WEAPON_M4A1S:
		case WEAPON_MAC10:
		case WEAPON_P90:
		case WEAPON_UMP45:
		case WEAPON_BIZON:
		case WEAPON_NEGEV:
		case WEAPON_MP7:
		case WEAPON_MP9:
		case WEAPON_SG553:
		case WEAPON_MP5:
			return true;
		default:
			return false;
		}
	}

	bool CBaseCombatWeapon::IsSniper( void )
	{
		switch( this->GetWeaponID() )
		{
		case WEAPON_AWP:
		case WEAPON_G3SG1:
		case WEAPON_SCAR20:
		case WEAPON_SCOUT:
			return true;
		default:
			return false;
		}
	}

	bool CBaseCombatWeapon::IsKnife( void )
	{
		switch( this->GetWeaponID() )
		{//floeten sagt is gut
		case WEAPON_KNIFE:
		case WEAPON_KNIFE_T:
		case WEAPON_KNIFE_GUT:
		case WEAPON_KNIFE_FLIP:
		case WEAPON_KNIFE_BAYONET:
		case WEAPON_KNIFE_M9BAYONET:
		case WEAPON_KNIFE_KARAMBIT:
		case WEAPON_KNIFE_HUNTSMAN:
		case WEAPON_KNIFE_BUTTERFLY:
		case WEAPON_KNIFE_BOWIE:
		case WEAPON_KNIFE_FALCHION:
		case WEAPON_KNIFE_SHADOWDAGGER:
				return true;
		default:
			return false;
		}
	}

	bool CBaseCombatWeapon::IsOther( void )
	{
		switch( this->GetWeaponID() )
		{
		case WEAPON_NONE:
		case WEAPON_FLASH:
		case WEAPON_HE:
		case WEAPON_SMOKE:
		case WEAPON_MOLOTOV:
		case WEAPON_DECOY:
		case WEAPON_INC:
		case WEAPON_C4:
		case WEAPON_TASER:
			return true;
		default:
			return false;
		}
	}

	bool CBaseCombatWeapon::IsGrenade( void )
	{
		switch( this->GetWeaponID() )
		{
		case WEAPON_FLASH:
		case WEAPON_HE:
		case WEAPON_SMOKE:
		case WEAPON_MOLOTOV:
		case WEAPON_DECOY:
		case WEAPON_INC:
			return true;
		default:
			return false;
		}
	}
};

class WeaponGeneric
{
public:
	bool WeaponGeneric::IsIDPistol( int m_iWeaponId )
	{
		switch( m_iWeaponId )
		{
		case WEAPON_DEAGLE:
			return true;
		case WEAPON_DUALS:
			return true;
		case WEAPON_FIVE7:
			return true;
		case WEAPON_GLOCK:
			return true;
		case WEAPON_TEC9:
			return true;
		case WEAPON_P2000:
			return true;
		case WEAPON_USPS:
			return true;
		case WEAPON_P250:
			return true;
		case WEAPON_CZ75:
			return true;
		case WEAPON_R8REVOLVER:
			return true;
		default:
			return false;
		}
	}

	bool WeaponGeneric::IsIDShotgun( int m_iWeaponId )
	{
		switch( m_iWeaponId )
		{
		case WEAPON_XM1014:
			return true;
		case WEAPON_MAG7:
			return true;
		case WEAPON_SAWEDOFF:
			return true;
		case WEAPON_NOVA:
			return true;
		default:
			return false;
		}
	}

	bool WeaponGeneric::IsIDAutomatic( int m_iWeaponId )
	{
		switch( m_iWeaponId )
		{
		case WEAPON_AK47:
			return true;
		case WEAPON_AUG:
			return true;
		case WEAPON_FAMAS:
			return true;
		case WEAPON_GALIL:
			return true;
		case WEAPON_M249:
			return true;
		case WEAPON_M4A1:
			return true;
		case WEAPON_M4A1S:
			return true;
		case WEAPON_MAC10:
			return true;
		case WEAPON_P90:
			return true;
		case WEAPON_UMP45:
			return true;
		case WEAPON_BIZON:
			return true;
		case WEAPON_NEGEV:
			return true;
		case WEAPON_MP7:
			return true;
		case WEAPON_MP9:
			return true;
		case WEAPON_SG553:
			return true;
		default:
			return false;
		}
	}

	bool WeaponGeneric::IsIDSniper( int m_iWeaponId )
	{
		switch( m_iWeaponId )
		{
		case WEAPON_AWP:
			return true;
		case WEAPON_G3SG1:
			return true;
		case WEAPON_SCAR20:
			return true;
		case WEAPON_SCOUT:
			return true;
		default:
			return false;
		}
	}

	bool WeaponGeneric::IsIDKnife( int m_iWeaponId )
	{
		switch( m_iWeaponId )
		{
		case WEAPON_KNIFE:
			return true;
		case WEAPON_KNIFE_T:
			return true;
		case WEAPON_KNIFE_GUT:
			return true;
		case WEAPON_KNIFE_FLIP:
			return true;
		case WEAPON_KNIFE_BAYONET:
			return true;
		case WEAPON_KNIFE_M9BAYONET:
			return true;
		case WEAPON_KNIFE_KARAMBIT:
			return true;
		case WEAPON_KNIFE_HUNTSMAN:
			return true;
		case WEAPON_KNIFE_BUTTERFLY:
			return true;
		case WEAPON_KNIFE_FALCHION:
			return true;
		case WEAPON_KNIFE_SHADOWDAGGER:
			return true;
		case WEAPON_KNIFE_BOWIE:
			return true;
		default:
			return false;
		}
	}

	bool WeaponGeneric::IsIDOther( int m_iWeaponId )
	{
		switch( m_iWeaponId )
		{
		case WEAPON_NONE:
			return true;
		case WEAPON_TASER:
			return true;
		case WEAPON_FLASH:
			return true;
		case WEAPON_HE:
			return true;
		case WEAPON_SMOKE:
			return true;
		case WEAPON_MOLOTOV:
			return true;
		case WEAPON_DECOY:
			return true;
		case WEAPON_INC:
			return true;
		case WEAPON_C4:
			return true;
		default:
			return false;
		}
	}

	const char *WeaponGeneric::GetWeaponName( int m_iWeaponId )
	{
		switch( m_iWeaponId )
		{
		case WEAPON_DEAGLE:
			return "DEAGLE"; break;
		case WEAPON_DUALS:
			return "DUALS"; break;
		case WEAPON_FIVE7:
			return "FIVE7"; break;
		case WEAPON_GLOCK:
			return "GLOCK"; break;
		case WEAPON_AK47:
			return "AK47"; break;
		case WEAPON_AUG:
			return "AUG"; break;
		case WEAPON_AWP:
			return "AWP"; break;
		case WEAPON_FAMAS:
			return "FAMAS"; break;
		case WEAPON_G3SG1:
			return "G3SG1"; break;
		case WEAPON_GALIL:
			return "GALIL"; break;
		case WEAPON_M249:
			return "M249"; break;
		case WEAPON_M4A1:
			return "M4A1"; break;
		case WEAPON_MAC10:
			return "MAC10"; break;
		case WEAPON_P90:
			return "P90"; break;
		case WEAPON_MP9:
			return "MP9"; break;
		case WEAPON_UMP45:
			return "UMP45"; break;
		case WEAPON_XM1014:
			return "XM1014"; break;
		case WEAPON_BIZON:
			return "BIZON"; break;
		case WEAPON_MAG7:
			return "MAG7"; break;
		case WEAPON_NEGEV:
			return "NEGEV"; break;
		case WEAPON_SAWEDOFF:
			return "SAWED OFF"; break;
		case WEAPON_TEC9:
			return "TEC9"; break;
		case WEAPON_TASER:
			return "ZEUS"; break;
		case WEAPON_P2000:
			return "P2000"; break;
		case WEAPON_MP7:
			return "MP7"; break;
		case WEAPON_NOVA:
			return "NOVA"; break;
		case WEAPON_P250:
			return "P250"; break;
		case WEAPON_SCAR20:
			return "SCAR20"; break;
		case WEAPON_SG553:
			return "SG553"; break;
		case WEAPON_SCOUT:
			return "SCOUT"; break;
		case WEAPON_R8REVOLVER:
			return "REVOLVER"; break;
		case WEAPON_KNIFE:
			return "CT KNIFE"; break;
		case WEAPON_KNIFE_T:
			return "T KNIFE"; break;
		case WEAPON_KNIFE_GUT:
			return "GUT KNIFE"; break;
		case WEAPON_KNIFE_FLIP:
			return "FLIP KNIFE"; break;
		case WEAPON_KNIFE_BAYONET:
			return "BAYONET"; break;
		case WEAPON_KNIFE_KARAMBIT:
			return "KARAMBIT"; break;
		case WEAPON_KNIFE_M9BAYONET:
			return "M9 BAYONET"; break;
		case WEAPON_KNIFE_BUTTERFLY:
			return "BUTTERFLY"; break;
		case WEAPON_KNIFE_FALCHION:
			return "FALCHION"; break;
		case WEAPON_KNIFE_SHADOWDAGGER:
			return "SHADOW DAGGERS"; break;
		case WEAPON_KNIFE_BOWIE:
			return "BOWIE"; break;
		case WEAPON_KNIFE_HUNTSMAN:
			return "HUNTSMAN"; break;
		case WEAPON_M4A1S:
			return "M4A1 S"; break;
		case WEAPON_USPS:
			return "USP S"; break;
		case WEAPON_CZ75:
			return "CZ 75"; break;
		default:
			return "INVALID"; break;
		}
		return "INVALID";
	}

	const char *WeaponGeneric::GetSkinName( int iSkinID )
	{
		switch( iSkinID )
		{
		case 1: return "None"; break;
		case 2: return "Groundwater";  break;
		case 3: return "Candy Apple";  break;
		case 5: return "Forest DDPAT";  break;
		case 6: return "Arctic Camo";  break;
		case 8: return "Desert Storm";  break;
		case 9: return "Bengal Tiger";  break;
		case 10: return "Copperhead";  break;
		case 11: return "Skulls";  break;
		case 12: return "Crimson Web";  break;
		case 13: return "Blue Streak";  break;
		case 14: return "Red Laminate";  break;
		case 15: return "Gunsmoke";  break;
		case 16: return "Jungle Tiger";  break;
		case 17: return "Urban DDPAT";  break;
		case 20: return "Virus";  break;
		case 21: return "Granite Marbleized";  break;
		case 22: return "Contrast Spray";  break;
		case 25: return "Forest Leaves";  break;
		case 26: return "Lichen Dashed";  break;
		case 27: return "Bone Mask";  break;
		case 28: return "Anodized Navy";  break;
		case 30: return "Snake Camo";  break;
		case 32: return "Silver";  break;
		case 33: return "Hot Rod";  break;
		case 34: return "Metallic DDPAT";  break;
		case 36: return "Ossified";  break;
		case 37: return "Blaze";  break;
		case 38: return "Fade";  break;
		case 39: return "Bulldozer";  break;
		case 40: return "Night";  break;
		case 41: return "Copper";  break;
		case 42: return "Blue Steel";  break;
		case 43: return "Stained";  break;
		case 44: return "Case Hardened";  break;
		case 46: return "Contractor";  break;
		case 47: return "Colony";  break;
		case 48: return "Dragon Tattoo";  break;
		case 51: return "Lightning Strike";  break;
		case 59: return "Slaughter";  break;
		case 60: return "Dark Water";  break;
		case 61: return "Hypnotic";  break;
		case 62: return "Bloomstick";  break;
		case 67: return "Cold Blooded";  break;
		case 70: return "Carbon Fiber";  break;
		case 71: return "Scorpion";  break;
		case 72: return "Safari Mesh";  break;
		case 73: return "Wings";  break;
		case 74: return "Polar Camo";  break;
		case 75: return "Blizzard Marbleized";  break;
		case 76: return "Winter Forest";  break;
		case 77: return "Boreal Forest";  break;
		case 78: return "Forest Night";  break;
		case 83: return "Orange DDPAT";  break;
		case 84: return "Pink DDPAT";  break;
		case 90: return "Mudder";  break;
		case 92: return "Cyanospatter";  break;
		case 93: return "Caramel";  break;
		case 95: return "Grassland";  break;
		case 96: return "Blue Spruce";  break;
		case 98: return "Ultraviolet";  break;
		case 99: return "Sand Dune";  break;
		case 100: return "Storm";  break;
		case 101: return "Tornado";  break;
		case 102: return "Whiteout";  break;
		case 104: return "Grassland Leaves";  break;
		case 107: return "Polar Mesh";  break;
		case 110: return "Condemned";  break;
		case 111: return "Glacier Mesh";  break;
		case 116: return "Sand Mesh";  break;
		case 119: return "Sage Spray";  break;
		case 122: return "Jungle Spray";  break;
		case 124: return "Sand Spray";  break;
		case 135: return "Urban Perforated";  break;
		case 136: return "Waves Perforated";  break;
		case 141: return "Orange Peel";  break;
		case 143: return "Urban Masked";  break;
		case 147: return "Jungle Dashed";  break;
		case 148: return "Sand Dashed";  break;
		case 149: return "Urban Dashed";  break;
		case 151: return "Jungle";  break;
		case 153: return "Demolition";  break;
		case 154: return "Afterimage";  break;
		case 155: return "Bullet Rain";  break;
		case 156: return "Death by Kitty";  break;
		case 157: return "Palm";  break;
		case 158: return "Walnut";  break;
		case 159: return "Brass";  break;
		case 162: return "Splash";  break;
		case 164: return "Modern Hunter";  break;
		case 165: return "Splash Jam";  break;
		case 166: return "Blaze Orange";  break;
		case 167: return "Radiation Hazard";  break;
		case 168: return "Nuclear Threat";  break;
		case 169: return "Fallout Warning";  break;
		case 170: return "Predator";  break;
		case 171: return "Irradiated Alert";  break;
		case 172: return "Black Laminate";  break;
		case 174: return "BOOM";  break;
		case 175: return "Scorched";  break;
		case 176: return "Faded Zebra";  break;
		case 177: return "Memento";  break;
		case 178: return "Doomkitty";  break;
		case 179: return "Nuclear Threat";  break;
		case 180: return "Fire Serpent";  break;
		case 181: return "Corticera";  break;
		case 182: return "Emerald Dragon";  break;
		case 183: return "Overgrowth";  break;
		case 184: return "Corticera";  break;
		case 185: return "Golden Koi";  break;
		case 186: return "Wave Spray";  break;
		case 187: return "Zirka";  break;
		case 188: return "Graven";  break;
		case 189: return "Bright Water";  break;
		case 190: return "Black Limba";  break;
		case 191: return "Tempest";  break;
		case 192: return "Shattered";  break;
		case 193: return "Bone Pile";  break;
		case 194: return "Spitfire";  break;
		case 195: return "Demeter";  break;
		case 196: return "Emerald";  break;
		case 197: return "Anodized Navy";  break;
		case 198: return "Hazard";  break;
		case 199: return "Dry Season";  break;
		case 200: return "Mayan Dreams";  break;
		case 201: return "Palm";  break;
		case 202: return "Jungle DDPAT";  break;
		case 203: return "Rust Coat";  break;
		case 204: return "Mosaico";  break;
		case 205: return "Jungle";  break;
		case 206: return "Tornado";  break;
		case 207: return "Facets";  break;
		case 208: return "Sand Dune";  break;
		case 209: return "Groundwater";  break;
		case 210: return "Anodized Gunmetal";  break;
		case 211: return "Ocean Foam";  break;
		case 212: return "Graphite";  break;
		case 213: return "Ocean Foam";  break;
		case 214: return "Graphite";  break;
		case 215: return "X-Ray";  break;
		case 216: return "Blue Titanium";  break;
		case 217: return "Blood Tiger";  break;
		case 218: return "Hexane";  break;
		case 219: return "Hive";  break;
		case 220: return "Hemoglobin";  break;
		case 221: return "Serum";  break;
		case 222: return "Blood in the Water";  break;
		case 223: return "Nightshade";  break;
		case 224: return "Water Sigil";  break;
		case 225: return "Ghost Camo";  break;
		case 226: return "Blue Laminate";  break;
		case 227: return "Electric Hive";  break;
		case 228: return "Blind Spot";  break;
		case 229: return "Azure Zebra";  break;
		case 230: return "Steel Disruption";  break;
		case 231: return "Cobalt Disruption";  break;
		case 232: return "Crimson Web";  break;
		case 233: return "Tropical Storm";  break;
		case 234: return "Ash Wood";  break;
		case 235: return "VariCamo";  break;
		case 236: return "Night Ops";  break;
		case 237: return "Urban Rubble";  break;
		case 238: return "VariCamo Blue";  break;
		case 240: return "CaliCamo";  break;
		case 241: return "Hunting Blind";  break;
		case 242: return "Army Mesh";  break;
		case 243: return "Gator Mesh";  break;
		case 244: return "Teardown";  break;
		case 245: return "Army Recon";  break;
		case 246: return "Amber Fade";  break;
		case 247: return "Damascus Steel";  break;
		case 248: return "Red Quartz";  break;
		case 249: return "Cobalt Quartz";  break;
		case 250: return "Full Stop";  break;
		case 251: return "Pit Viper";  break;
		case 252: return "Silver Quartz";  break;
		case 253: return "Acid Fade";  break;
		case 254: return "Nitro";  break;
		case 255: return "Asiimov";  break;
		case 256: return "The Kraken";  break;
		case 257: return "Guardian";  break;
		case 258: return "Mehndi";  break;
		case 259: return "Redline";  break;
		case 260: return "Pulse";  break;
		case 261: return "Marina";  break;
		case 262: return "Rose Iron";  break;
		case 263: return "Rising Skull";  break;
		case 264: return "Sandstorm";  break;
		case 265: return "Kami";  break;
		case 266: return "Magma";  break;
		case 267: return "Cobalt Halftone";  break;
		case 268: return "Tread Plate";  break;
		case 269: return "The Fuschia Is Now";  break;
		case 270: return "Victoria";  break;
		case 271: return "Undertow";  break;
		case 272: return "Titanium Bit";  break;
		case 273: return "Heirloom";  break;
		case 274: return "Copper Galaxy";  break;
		case 275: return "Red FragCam";  break;
		case 276: return "Panther";  break;
		case 277: return "Stainless";  break;
		case 278: return "Blue Fissure";  break;
		case 279: return "Asiimov";  break;
		case 280: return "Chameleon";  break;
		case 281: return "Corporal";  break;
		case 282: return "Redline";  break;
		case 283: return "Trigon";  break;
		case 284: return "Heat";  break;
		case 285: return "Terrain";  break;
		case 286: return "Antique";  break;
		case 287: return "Pulse";  break;
		case 288: return "Sergeant";  break;
		case 289: return "Sandstorm";  break;
		case 290: return "Guardian";  break;
		case 291: return "Heaven Guard";  break;
		case 293: return "Death Rattle";  break;
		case 294: return "Green Apple";  break;
		case 295: return "Franklin";  break;
		case 296: return "Meteorite";  break;
		case 297: return "Tuxedo";  break;
		case 298: return "Army Sheen";  break;
		case 299: return "Caged Steel";  break;
		case 300: return "Emerald Pinstripe";  break;
		case 301: return "Atomic Alloy";  break;
		case 302: return "Vulcan";  break;
		case 303: return "Isaac";  break;
		case 304: return "Slashed";  break;
		case 305: return "Torque";  break;
		case 306: return "Antique";  break;
		case 307: return "Retribution";  break;
		case 308: return "Kami";  break;
		case 309: return "Howl";  break;
		case 310: return "Curse";  break;
		case 311: return "Desert Warfare";  break;
		case 312: return "Cyrex";  break;
		case 313: return "Orion";  break;
		case 314: return "Heaven Guard";  break;
		case 315: return "Poison Dart";  break;
		case 316: return "Jaguar";  break;
		case 317: return "Bratatat";  break;
		case 318: return "Road Rash";  break;
		case 319: return "Detour";  break;
		case 320: return "Red Python";  break;
		case 321: return "Master Piece";  break;
		case 322: return "Nitro";  break;
		case 323: return "Rust Coat";  break;
		case 325: return "Chalice";  break;
		case 326: return "Knight";  break;
		case 327: return "Chainmail";  break;
		case 328: return "Hand Cannon";  break;
		case 329: return "Dark Age";  break;
		case 330: return "Briar";  break;
		case 332: return "Royal Blue";  break;
		case 333: return "Indigo";  break;
		case 334: return "Twist";  break;
		case 335: return "Module";  break;
		case 336: return "Desert-Strike";  break;
		case 337: return "Tatter";  break;
		case 338: return "Pulse";  break;
		case 339: return "Caiman";  break;
		case 340: return "Jet Set";  break;
		case 341: return "First Class";  break;
		case 342: return "Leather";  break;
		case 343: return "Commuter";  break;
		case 344: return "Dragon Lore";  break;
		case 345: return "First Class";  break;
		case 346: return "Coach Class";  break;
		case 347: return "Pilot";  break;
		case 348: return "Red Leather";  break;
		case 349: return "Osiris";  break;
		case 350: return "Tigris";  break;
		case 351: return "Conspiracy";  break;
		case 352: return "Fowl Play";  break;
		case 353: return "Water Elemental";  break;
		case 354: return "Urban Hazard";  break;
		case 355: return "Desert-Strike";  break;
		case 356: return "Koi";  break;
		case 357: return "Ivory";  break;
		case 358: return "Supernova";  break;
		case 359: return "Asiimov";  break;
		case 360: return "Cyrex";  break;
		case 361: return "Abyss";  break;
		case 362: return "Labyrinth";  break;
		case 363: return "Traveler";  break;
		case 364: return "Business Class";  break;
		case 365: return "Olive Plaid";  break;
		case 366: return "Green Plaid";  break;
		case 367: return "Reactor";  break;
		case 368: return "Setting Sun";  break;
		case 369: return "Nuclear Waste";  break;
		case 370: return "Bone Machine";  break;
		case 371: return "Styx";  break;
		case 372: return "Nuclear Garden";  break;
		case 373: return "Contamination";  break;
		case 374: return "Toxic";  break;
		case 375: return "Radiation Hazard";  break;
		case 376: return "Chemical Green";  break;
		case 377: return "Hot Shot";  break;
		case 378: return "Fallout Warning";  break;
		case 379: return "Cerberus";  break;
		case 380: return "Wasteland Rebel";  break;
		case 381: return "Grinder";  break;
		case 382: return "Murky";  break;
		case 383: return "Basilisk";  break;
		case 384: return "Griffin";  break;
		case 385: return "Firestarter";  break;
		case 386: return "Dart";  break;
		case 387: return "Urban Hazard";  break;
		case 388: return "Cartel";  break;
		case 389: return "Fire Elemental";  break;
		case 390: return "Highwayman";  break;
		case 391: return "Cardiac";  break;
		case 392: return "Delusion";  break;
		case 393: return "Tranquility";  break;
		case 394: return "Cartel";  break;
		case 395: return "Man-o'-war";  break;
		case 396: return "Urban Shock";  break;
		case 397: return "Naga";  break;
		case 398: return "Chatterbox";  break;
		case 399: return "Catacombs";  break;
		case 400: return "Dragon King";  break;
		case 401: return "System Lock";  break;
		case 402: return "Malachite";  break;
		case 403: return "Deadly Poison";  break;
		case 404: return "Muertos";  break;
		case 405: return "Serenity";  break;
		case 406: return "Grotto";  break;
		case 407: return "Quicksilver";  break;
		case 409: return "Tiger Tooth";  break;
		case 410: return "Damascus Steel";  break;
		case 411: return "Damascus Steel";  break;
		case 413: return "Marble Fade";  break;
		case 414: return "Rust Coat";  break;
		case 415: return "Doppler";  break;
		case 416: return "Doppler";  break;
		case 417: return "Doppler";  break;
		case 418: return "Doppler";  break;
		case 419: return "Doppler";  break;
		case 420: return "Doppler";  break;
		case 421: return "Doppler";  break;
		case 422: return "Elite Build";  break;
		case 423: return "Armor Core";  break;
		case 424: return "Worm God";  break;
		case 425: return "Bronze Deco";  break;
		case 426: return "Valence";  break;
		case 427: return "Monkey Business";  break;
		case 428: return "Eco";  break;
		case 429: return "Djinn";  break;
		case 430: return "Hyper Beast";  break;
		case 431: return "Heat";  break;
		case 432: return "Man-o'-war";  break;
		case 433: return "Neon Rider";  break;
		case 434: return "Origami";  break;
		case 435: return "Pole Position";  break;
		case 436: return "Grand Prix";  break;
		case 437: return "Twilight Galaxy";  break;
		case 438: return "Chronos";  break;
		case 439: return "Hades";  break;
		case 440: return "Icarus Fell";  break;
		case 441: return "Minotaur's Labyrinth";  break;
		case 442: return "Asterion";  break;
		case 443: return "Pathfinder";  break;
		case 444: return "Daedalus";  break;
		case 445: return "Hot Rod";  break;
		case 446: return "Medusa";  break;
		case 447: return "Duelist";  break;
		case 448: return "Pandora's Box";  break;
		case 449: return "Poseidon";  break;
		case 450: return "Moon in Libra";  break;
		case 451: return "Sun in Leo";  break;
		case 452: return "Shipping Forecast";  break;
		case 453: return "Emerald";  break;
		case 454: return "Para Green";  break;
		case 455: return "Akihabara Accept";  break;
		case 456: return "Hydroponic";  break;
		case 457: return "Bamboo Print";  break;
		case 458: return "Bamboo Shadow";  break;
		case 459: return "Bamboo Forest";  break;
		case 460: return "Aqua Terrace";  break;
		case 462: return "Counter Terrace";  break;
		case 463: return "Terrace";  break;
		case 464: return "Neon Kimono";  break;
		case 465: return "Orange Kimono";  break;
		case 466: return "Crimson Kimono";  break;
		case 467: return "Mint Kimono";  break;
		case 468: return "Midnight Storm";  break;
		case 469: return "Sunset Storm";  break;
		case 470: return "Sunset Storm";  break;
		case 471: return "Daybreak";  break;
		case 472: return "Impact Drill";  break;
		case 473: return "Seabird";  break;
		case 474: return "Aquamarine Revenge";  break;
		case 475: return "Hyper Beast";  break;
		case 476: return "Yellow Jacket";  break;
		case 477: return "Neural Net";  break;
		case 478: return "Rocket Pop";  break;
		case 479: return "Bunsen Burner";  break;
		case 480: return "Evil Daimyo";  break;
		case 481: return "Nemesis";  break;
		case 482: return "Ruby Poison Dart";  break;
		case 483: return "Loudmouth";  break;
		case 484: return "Ranger";  break;
		case 485: return "Handgun";  break;
		case 486: return "Elite Build";  break;
		case 487: return "Cyrex";  break;
		case 488: return "Riot";  break;
		case 489: return "Torque";  break;
		case 490: return "Frontside Misty";  break;
		case 491: return "Dualing Dragons";  break;
		case 492: return "Survivor Z";  break;
		case 493: return "Flux";  break;
		case 494: return "Stone Cold";  break;
		case 495: return "Wraiths";  break;
		case 496: return "Nebula Crusader";  break;
		case 497: return "Golden Coil";  break;
		case 498: return "Rangeen";  break;
		case 499: return "Cobalt Core";  break;
		case 500: return "Special Delivery";  break;
		case 501: return "Wingshot";  break;
		case 502: return "Green Marine";  break;
		case 503: return "Big Iron";  break;
		case 504: return "Kill Confirmed";  break;
		case 505: return "Scumbria";  break;
		case 506: return "Point Disarray";  break;
		case 507: return "Ricochet";  break;
		case 508: return "Fuel Rod";  break;
		case 509: return "Corinthian";  break;
		case 510: return "Retrobution";  break;
		case 511: return "The Executioner";  break;
		case 512: return "Royal Paladin";  break;
		case 514: return "Power Loader";  break;
		case 515: return "Imperial";  break;
		case 516: return "Shapewood";  break;
		case 517: return "Yorick";  break;
		case 518: return "Outbreak";  break;
		case 519: return "Tiger Moth";  break;
		case 520: return "Avalanche";  break;
		case 521: return "Teclu Burner";  break;
		case 522: return "Fade";  break;
		case 523: return "Amber Fade";  break;
		case 524: return "Fuel Injector";  break;
		case 525: return "Elite Build";  break;
		case 526: return "Photic Zone";  break;
		case 527: return "Kumicho Dragon";  break;
		case 528: return "Cartel";  break;
		case 529: return "Valence";  break;
		case 530: return "Triumvirate";  break;
		case 532: return "Royal Legion";  break;
		case 533: return "The Battlestar";  break;
		case 534: return "Lapis Gator";  break;
		case 535: return "Praetorian";  break;
		case 536: return "Impire";  break;
		case 537: return "Hyper Beast";  break;
		case 538: return "Necropos";  break;
		case 539: return "Jambiya";  break;
		case 540: return "Lead Conduit";  break;
		case 541: return "Fleet Flock";  break;
		case 542: return "Judgement of Anubis";  break;
		case 543: return "Red Astor";  break;
		case 544: return "Ventilators";  break;
		case 545: return "Orange Crash";  break;
		case 546: return "Firefight";  break;
		case 547: return "Spectre";  break;
		case 548: return "Chantico's Fire";  break;
		case 549: return "Bioleak";  break;
		case 550: return "Oceanic";  break;
		case 551: return "Asiimov";  break;
		case 552: return "Fubar";  break;
		case 553: return "Atlas";  break;
		case 554: return "Ghost Crusader";  break;
		case 555: return "Re-Entry";  break;
		case 556: return "Primal Saber";  break;
		case 557: return "Black Tie";  break;
		case 558: return "Lore";  break;
		case 559: return "Lore";  break;
		case 560: return "Lore";  break;
		case 561: return "Lore";  break;
		case 562: return "Lore";  break;
		case 563: return "Black Laminate";  break;
		case 564: return "Black Laminate";  break;
		case 565: return "Black Laminate";  break;
		case 566: return "Black Laminate";  break;
		case 567: return "Black Laminate";  break;
		case 568: return "Gamma Doppler Emerald Marble";  break;
		case 569: return "Gamma Doppler Phase 1";  break;
		case 570: return "Gamma Doppler Phase 2";  break;
		case 571: return "Gamma Doppler Phase 3";  break;
		case 572: return "Gamma Doppler Phase 4";  break;
		case 573: return "Autotronic";  break;
		case 574: return "Autotronic";  break;
		case 575: return "Autotronic";  break;
		case 576: return "Autotronic";  break;
		case 577: return "Autotronic";  break;
		case 578: return "Bright Water";  break;
		case 579: return "Bright Water";  break;
		case 580: return "Freehand";  break;
		case 581: return "Freehand";  break;
		case 582: return "Freehand";  break;
		case 583: return "Aristocrat";  break;
		case 584: return "Phobos";  break;
		case 585: return "Violent Daimyo";  break;
		case 586: return "Wasteland Rebel";  break;
		case 587: return "Mecha Industries";  break;
		case 588: return "Desolate Space";  break;
		case 589: return "Carnivore";  break;
		case 590: return "Exo";  break;
		case 591: return "Imperial Dragon";  break;
		case 592: return "Iron Clad";  break;
		case 593: return "Chopper";  break;
		case 594: return "Harvester";  break;
		case 595: return "Reboot";  break;
		case 596: return "Limelight";  break;
		case 597: return "Bloodsport";  break;
		case 598: return "Aerial";  break;
		case 599: return "Ice Cap";  break;
		case 600: return "Neon Revolution";  break;
		case 601: return "Syd Mead";  break;
		case 602: return "Imprint";  break;
		case 603: return "Directive";  break;
		case 604: return "Roll Cage";  break;
		case 605: return "Scumbria";  break;
		case 606: return "Ventilator";  break;
		case 607: return "Weasel";  break;
		case 608: return "Petroglyph";  break;
		case 609: return "Airlock";  break;
		case 610: return "Dazzle";  break;
		case 611: return "Grim";  break;
		case 612: return "Powercore";  break;
		case 613: return "Triarch";  break;
		case 614: return "Fuel Injector";  break;
		case 615: return "Briefing";  break;
		case 616: return "Slipstream";  break;
		case 617: return "Doppler";  break;
		case 618: return "Doppler";  break;
		case 619: return "Doppler";  break;
		case 620: return "Ultraviolet";  break;
		case 621: return "Ultraviolet";  break;
		case 622: return "Polymer";  break;
		case 623: return "Ironwork";  break;
		case 624: return "Dragonfire";  break;
		case 625: return "Royal Consorts";  break;
		case 626: return "Mecha Industries";  break;
		case 627: return "Cirrus";  break;
		case 628: return "Stinger";  break;
		case 629: return "Black Sand";  break;
		case 630: return "Sand Scale";  break;
		case 631: return "Flashback";  break;
		case 632: return "Buzz Kill";  break;
		case 633: return "Sonar";  break;
		case 634: return "Gila";  break;
		case 635: return "Turf";  break;
		case 636: return "Shallow Grave";  break;
		case 637: return "Cyrex";  break;
		case 638: return "Wasteland Princess";  break;
		case 639: return "Bloodsport";  break;
		case 640: return "Fever Dream";  break;
		case 641: return "Jungle Slipstream";  break;
		case 642: return "Blueprint";  break;
		case 643: return "Xiangliu";  break;
		case 644: return "Decimator";  break;
		case 645: return "Oxide Blaze";  break;
		case 646: return "Capillary";  break;
		case 647: return "Crimson Tsunami";  break;
		case 648: return "Emerald Poison Dart";  break;
		case 649: return "Akoben";  break;
		case 650: return "Ripple";  break;
		case 651: return "Last Dive";  break;
		case 652: return "Scaffold";  break;
		case 653: return "Neo-Noir";  break;
		case 654: return "Seasons";  break;
		case 655: return "Zander";  break;
		case 10006: return "Charred";  break;
		case 10007: return "Snakebite";  break;
		case 10008: return "Bronzed";  break;
		case 10009: return "Leather";  break;
		case 10010: return "Spruce DDPAT";  break;
		case 10013: return "Lunar Weave";  break;
		case 10015: return "Convoy";  break;
		case 10016: return "Crimson Weave";  break;
		case 10018: return "Superconductor";  break;
		case 10019: return "Arid";  break;
		case 10021: return "Slaughter";  break;
		case 10024: return "Eclipse";  break;
		case 10026: return "Spearmint";  break;
		case 10027: return "Boom!";  break;
		case 10028: return "Cool Mint";  break;
		case 10030: return "Forest DDPAT";  break;
		case 10033: return "Crimson Kimono";  break;
		case 10034: return "Emerald Web";  break;
		case 10035: return "Foundation";  break;
		case 10036: return "Badlands";  break;
		case 10037: return "Pandora's Box";  break;
		case 10038: return "Hedge Maze";  break;
		case 10039: return "Guerrilla";  break;
		case 10040: return "Diamondback";  break;
		case 10041: return "King Snake"; break;
		case 10042: return "Imperial Plaid"; break;
		case 10043: return "Overtake"; break;
		case 10044: return "Racing Green"; break;
		case 10045: return "Amphibious"; break;
		case 10046: return "Bronze Morph"; break;
		case 10047: return "Omega"; break;
		case 10048: return "Vice"; break;
		case 10049: return "POW!"; break;
		case 10050: return "Turtle"; break;
		case 10051: return "Transport"; break;
		case 10052: return "Polygon"; break;
		case 10053: return "Cobalt Skulls"; break;
		case 10054: return "Overprint"; break;
		case 10055: return "Duct Tape"; break;
		case 10056: return "Arboreal"; break;
		case 10057: return "Emerald"; break;
		case 10058: return "Mangrove"; break;
		case 10059: return "Rattler"; break;
		case 10060: return "Case Hardened"; break;
		case 10061: return "Crimson Web"; break;
		case 10062: return "Buckshot"; break;
		case 10063: return "Fade"; break;
		case 10064: return "Mogul"; break;
		default:
			return ""; break;
		}
		return "";
	}

	std::vector< WeaponInfo > WeaponGeneric::GetWeaponInfo( bool exludeknife = false )
	{
		static std::vector<WeaponInfo> ret;
		static bool once = false;
		if( !once )
		{
			for( int i = 1; i < 65; i++ )
			{
				if( !IsIDPistol( i ) && !IsIDAutomatic( i ) && !IsIDSniper( i ) && !IsIDShotgun( i ) )
					continue;

				WeaponInfo w;
				w._id = i;
				w._name = GetWeaponName( i );

				ret.push_back( w );
			}

			if( exludeknife )
			{
				for( int i = 500; i < 517; i++ )
				{
					if( !IsIDKnife( i ) )
						continue;

					WeaponInfo w;
					w._id = i;
					w._name = GetWeaponName( i );

					ret.push_back( w );
				}
			}
			once = true;
		}
		return ret;
	}

	std::vector < WeaponInfo > WeaponGeneric::GetSkinInfo()
	{
		static std::vector <WeaponInfo> vRet;
		static bool once = false;
//		static std::string szSearch = SkinSearch.Text;
	
		if( !once )
		{
			for( int i = 0; i <= 10064; i++ )
			{
				WeaponInfo w;
				w._name = GetSkinName( i );

				if( w._name.empty() )
					continue;

	//			if( !SkinSearch.Text.empty() && w._name.find( SkinSearch.Text.c_str() ) != std::string::npos )
	//				continue;

				w._id = i;
				vRet.push_back( w );
			}
			once = true;
		}

/*		if( SkinSearch.Text != szSearch )
		{
			once = false;
			szSearch.clear();
			szSearch.append( SkinSearch.Text.c_str() );
		}*/

		return vRet;
	}
};

extern WeaponGeneric g_WeaponInfo;
