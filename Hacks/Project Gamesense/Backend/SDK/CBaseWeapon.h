#pragma once
#include "../Interfaces/Interfaces.h"
#include "../Utility/Utilities.h"
#include "../Configuration/Settings.h"
#include "../Interfaces/NetVar.h"
#include "../Hashing/xorstr.h"
#include "vfunc.hpp"
#include "vector3d.h"
class CBaseEntity;
class CBaseWeaponWorldModel;
#include "GlobalDefenitions.h"
struct CSWeaponInfo
{
	char _0x0000[20];
	__int32 max_clip;           //0x0016
	char _0x0018[12];
	__int32 max_reserved_ammo;  //0x0024
	char _0x0028[96];
	char* hud_name;             //0x0088
	char* weapon_name;          //0x008C
	char _0x0090[60];
	__int32 type;               //0x00CC
	__int32 price;              //0x00D0
	__int32 reward;             //0x00D4
	char _0x00D8[20];
	BYTE full_auto;             //0x00EC
	char _0x00ED[3];
	__int32 damage;             //0x00F0
	float armor_ratio;          //0x00F4
	__int32 bullets;            //0x00F8
	float penetration;          //0x00FC
	char _0x0100[8];
	float range;                //0x0108
	float range_modifier;       //0x010C
	char _0x0110[16];
	BYTE silencer;              //0x0120
	char _0x0121[15];
	float max_speed;            //0x0130
	float max_speed_alt;        //0x0134
	char _0x0138[76];
	__int32 recoil_seed;        //0x0184
	char _0x0188[32];

	char* bullet_type() {
		return *(char**)((uintptr_t)this + 0x80);
	}
}; //Size: 0x1D0

enum ItemDefinitionIndex
{
	/*WEAPON_NONE = 0,
	WEAPON_DEAGLE,
	WEAPON_ELITE,
	WEAPON_FIVESEVEN,
	WEAPON_GLOCK,
	WEAPON_AK47 = 7,
	WEAPON_AUG,
	WEAPON_AWP,
	WEAPON_FAMAS,
	WEAPON_G3SG1,
	WEAPON_GALILAR = 13,
	WEAPON_M249,
	WEAPON_M4A1 = 16,
	WEAPON_MAC10,
	WEAPON_P90 = 19,
	WEAPON_MP5SD = 23,
	WEAPON_UMP45,
	WEAPON_XM1014,
	WEAPON_BIZON,
	WEAPON_MAG7,
	WEAPON_NEGEV,
	WEAPON_SAWEDOFF,
	WEAPON_TEC9,
	WEAPON_TASER,
	WEAPON_HKP2000,
	WEAPON_MP7,
	WEAPON_MP9,
	WEAPON_NOVA,
	WEAPON_P250,
	WEAPON_SCAR20 = 38,
	WEAPON_SG556,
	WEAPON_SSG08,
	WEAPON_KNIFEGG,
	WEAPON_KNIFE,
	WEAPON_FLASHBANG,
	WEAPON_HEGRENADE,
	WEAPON_SMOKEGRENADE,
	WEAPON_MOLOTOV,
	WEAPON_DECOY,
	WEAPON_INCGRENADE,
	WEAPON_C4,
	WEAPON_HEALTHSHOT = 57,
	WEAPON_KNIFE_T = 59,
	WEAPON_M4A1_SILENCER,
	WEAPON_USP_SILENCER,
	WEAPON_CZ75A = 63,
	WEAPON_REVOLVER,
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
	WEAPON_KNIFE_BAYONET = 500,
	WEAPON_KNIFE_FLIP = 505,
	WEAPON_KNIFE_GUT,
	WEAPON_KNIFE_KARAMBIT,
	WEAPON_KNIFE_M9_BAYONET,
	WEAPON_KNIFE_TACTICAL,
	WEAPON_KNIFE_FALCHION = 512,
	WEAPON_KNIFE_SURVIVAL_BOWIE = 514,
	WEAPON_KNIFE_BUTTERFLY,
	WEAPON_KNIFE_PUSH,
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
	GLOVE_HYDRA = 5035*/
	WEAPON_DEAGLE = 1,
	WEAPON_ELITE = 2,
	WEAPON_FIVESEVEN = 3,
	WEAPON_GLOCK = 4,
	WEAPON_AK47 = 7,
	WEAPON_AUG = 8,
	WEAPON_AWP = 9,
	WEAPON_FAMAS = 10,
	WEAPON_G3SG1 = 11,
	WEAPON_GALILAR = 13,
	WEAPON_M249 = 14,
	WEAPON_M4A1 = 16,
	WEAPON_MAC10 = 17,
	WEAPON_P90 = 19,
	WEAPON_MP5SD = 23,
	WEAPON_UMP45 = 24,
	WEAPON_XM1014 = 25,
	WEAPON_BIZON = 26,
	WEAPON_MAG7 = 27,
	WEAPON_NEGEV = 28,
	WEAPON_SAWEDOFF = 29,
	WEAPON_TEC9 = 30,
	WEAPON_TASER = 31,
	WEAPON_HKP2000 = 32,
	WEAPON_MP7 = 33,
	WEAPON_MP9 = 34,
	WEAPON_NOVA = 35,
	WEAPON_P250 = 36,
	WEAPON_SHIELD = 37,
	WEAPON_SCAR20 = 38,
	WEAPON_SG553 = 39,
	WEAPON_SG556 = 39,
	WEAPON_SSG08 = 40,
	WEAPON_KNIFEGG = 41,
	WEAPON_KNIFE = 42,
	WEAPON_KNIFE_CT = 42,
	WEAPON_FLASHBANG = 43,
	WEAPON_HEGRENADE = 44,
	WEAPON_SMOKEGRENADE = 45,
	WEAPON_MOLOTOV = 46,
	WEAPON_DECOY = 47,
	WEAPON_INCGRENADE = 48,
	WEAPON_C4 = 49,
	WEAPON_HEALTHSHOT = 57,
	WEAPON_KNIFE_T = 59,
	WEAPON_M4A1_SILENCER = 60,
	WEAPON_USP_SILENCER = 61,
	WEAPON_CZ75A = 63,
	WEAPON_REVOLVER = 64,
	WEAPON_TAGRENADE = 68,
	WEAPON_FISTS = 69,
	WEAPON_BREACHCHARGE = 70,
	WEAPON_TABLET = 72,
	WEAPON_MELEE = 74,
	WEAPON_AXE = 75,
	WEAPON_HAMMER = 76,
	WEAPON_SPANNER = 78,
	WEAPON_KNIFE_GHOST = 80,
	WEAPON_FIREBOMB = 81,
	WEAPON_DIVERSION = 82,
	WEAPON_FRAG_GRENADE = 83,
	WEAPON_SNOWBALL = 84,
	WEAPON_BUMPMINE = 85,
	WEAPON_KNIFE_BAYONET = 500,
	WEAPON_KNIFE_CSS = 503,
	WEAPON_KNIFE_FLIP = 505,
	WEAPON_KNIFE_GUT = 506,
	WEAPON_KNIFE_KARAMBIT = 507,
	WEAPON_KNIFE_M9_BAYONET = 508,
	WEAPON_KNIFE_TACTICAL = 509,
	WEAPON_KNIFE_FALCHION = 512,
	WEAPON_KNIFE_SURVIVAL_BOWIE = 514,
	WEAPON_KNIFE_BUTTERFLY = 515,
	WEAPON_KNIFE_PUSH = 516,
	WEAPON_KNIFE_CORD = 517,
	WEAPON_KNIFE_CANIS = 518,
	WEAPON_KNIFE_URSUS = 519,
	WEAPON_KNIFE_GYPSY_JACKKNIFE = 520,
	WEAPON_KNIFE_OUTDOOR = 521,
	WEAPON_KNIFE_STILETTO = 522,
	WEAPON_KNIFE_WIDOWMAKER = 523,
	WEAPON_KNIFE_SKELETON = 525,

	GLOVE_STUDDED_BLOODHOUND = 5027,
	GLOVE_T_SIDE = 5028,
	GLOVE_CT_SIDE = 5029,
	GLOVE_SPORTY = 5030,
	GLOVE_SLICK = 5031,
	GLOVE_LEATHER_WRAP = 5032,
	GLOVE_MOTORCYCLE = 5033,
	GLOVE_SPECIALIST = 5034,
	GLOVE_HYDRA,
	MAX_ITEMDEFINITIONINDEX
};

enum WeaponType
{
	WEAPONTYPE_KNIFE = 0,
	WEAPONTYPE_PISTOL,
	WEAPONTYPE_SUBMACHINEGUN,
	WEAPONTYPE_RIFLE,
	WEAPONTYPE_SHOTGUN,
	WEAPONTYPE_MACHINEGUN,
	WEAPONTYPE_SNIPER_RIFLE,
	WEAPONTYPE_C4,
	WEAPONTYPE_GRENADE,
	WEAPONTYPE_UNKNOWN
};

enum GlovePaintKit
{
	GLOVE_PAINTKIT_BRONZED = 10008,
	GLOVE_PAINTKIT_CHARRED = 10006,
	GLOVE_PAINTKIT_GUERRILLA = 10039,
	GLOVE_PAINTKIT_SNAKEBITE = 10007,

	GLOVE_PAINTKIT_ARBOREAL = 10056,
	GLOVE_PAINTKIT_BADLANDS = 10036,
	GLOVE_PAINTKIT_COBALT_SKULLS = 10053,
	GLOVE_PAINTKIT_DUCT_TAPE = 10055,
	GLOVE_PAINTKIT_LEATHER = 10009,
	GLOVE_PAINTKIT_OVERPRINT = 10054,
	GLOVE_PAINTKIT_SLAUGHTER = 10021,
	GLOVE_PAINTKIT_SPRUCE_DDPAT = 10010,

	GLOVE_PAINTKIT_CONVOY = 10015,
	GLOVE_PAINTKIT_CRIMSON_WEAVE = 10016,
	GLOVE_PAINTKIT_DIAMONDBACK = 10040,
	GLOVE_PAINTKIT_IMPERIAL_PLAID = 10042,
	GLOVE_PAINTKIT_KING_SNAKE = 10041,
	GLOVE_PAINTKIT_LUNAR_WEAVE = 10013,
	GLOVE_PAINTKIT_OVERTAKE = 10043,
	GLOVE_PAINTKIT_RACING_GREEN = 10044,

	GLOVE_PAINTKIT_AMPHIBIOUS = 10045,
	GLOVE_PAINTKIT_ARID = 10019,
	GLOVE_PAINTKIT_BONZE_MORPH = 10046,
	GLOVE_PAINTKIT_HEDGE_MAZE = 10038,
	GLOVE_PAINTKIT_OMEGA = 10047,
	GLOVE_PAINTKIT_PANDORAS_BOX = 10037,
	GLOVE_PAINTKIT_SUPERCONDUCTOR = 10018,
	GLOVE_PAINTKIT_VICE = 10048,

	GLOVE_PAINTKIT_BUCKSHOT = 10062,
	GLOVE_PAINTKIT_CRIMSON_KIMONO = 10033,
	GLOVE_PAINTKIT_CRIMSON_WEB = 10061,
	GLOVE_PAINTKIT_EMERALD_WEB = 10034,
	GLOVE_PAINTKIT_FADE = 10063,
	GLOVE_PAINTKIT_FOREST_DDPAT = 10030,
	GLOVE_PAINTKIT_FOUNDATION = 10035,
	GLOVE_PAINTKIT_MOGUL = 10064,

	GLOVE_PAINTKIT_BOOM = 10027,
	GLOVE_PAINTKIT_COOL_MINT = 10028,
	GLOVE_PAINTKIT_ECLIPSE = 10024,
	GLOVE_PAINTKIT_POW = 10049,
	GLOVE_PAINTKIT_POLYGON = 10052,
	GLOVE_PAINTKIT_SPEARMINT = 10026,
	GLOVE_PAINTKIT_TRANSPORT = 10051,
	GLOVE_PAINTKIT_TURTLE = 10050,

	GLOVE_PAINTKIT_CASE_HARDENED = 10060,
	GLOVE_PAINTKIT_EMERALD = 10057,
	GLOVE_PAINTKIT_MANGROVE = 10058,
	GLOVE_PAINTKIT_RATTLER = 10059,
};

using namespace Cheat;

class CEconItemView
{
public:
	char _pad_0x0000[0x14];
	Vector	m_CustomMaterials; //0x0014
	char _pad_0x0028[0x1F8];
	Vector	m_VisualsDataProcessors; //0x0220
};

class CAttributeManager
{
public:
	char	_pad_0x0000[0x18];
	__int32						m_iReapplyProvisionParity; //0x0018 
	uintptr_t						m_hOuter; //0x001C 
	char	_pad_0x0020[0x4];
	__int32						m_ProviderType; //0x0024 
	char	_pad_0x0028[0x18];
	CEconItemView				m_Item; //0x0040 
};

class CBaseWeapon
{
	public:
		float GetNextPrimaryAttack()
		{
			if ( !this )
				return 0.f;

			return *reinterpret_cast <float*>( uintptr_t( this ) + offys.m_flnextprimaryattack );
		}

		float GetNextSecondaryAttack()
		{
			if ( !this )
				return 0.f;

			return *( float* )( ( DWORD )this + offys.m_flNextSecondaryAttack );
		}

		int GetAmmoInClip()
		{
			if ( !this )
				return 0;

			return *( int* )( ( uintptr_t )this + offys.m_iclip1 );
		}

		float GetLastShotTime()
		{
			if (!this)
				return 0.f;

			return *(float*)((DWORD)this + offys.m_fLastShotTime);
		}

		short GetItemDefenitionIndex()
		{
			if ( this != nullptr )
				return *reinterpret_cast <short*>( uintptr_t( this ) + offys.m_iitemdefinitionindex );

			return 0;
		}

		float GetPostponeFireReadyTime()
		{
			if ( !this )
				return 0.f;

			return *reinterpret_cast <float*>( uintptr_t( this ) + offys.m_flpostponefirereadytime );
		}

		float GetSpreadCone()
		{
			typedef float( __thiscall * Fn )( void* );

			if ( this )
				return vfunc < Fn >( this, 452)( this );
		}

		float GetInaccuracy()
		{
			typedef float( __thiscall * Fn )( void* );

			if ( this )
				return vfunc < Fn >( this, 482)( this );
		}

		HANDLE GetOwnerHandle()
		{
			if ( this )
				return *reinterpret_cast <HANDLE*>( ( DWORD )this + offys.m_howner );
		}

		Vector GetOrigin()
		{
			if ( this )
				return *( Vector* )( ( uintptr_t )this + 0x00000134 );
		}

		bool is_gun()
		{
			auto pWeaponData = get_csweapon_info();
			if (!pWeaponData)
				return false;
			switch (pWeaponData->type)
			{
			case WEAPONTYPE_C4:
				return false;
			case WEAPONTYPE_GRENADE:
				return false;
			case WEAPONTYPE_KNIFE:
				return false;
			case WEAPONTYPE_UNKNOWN:
				return false;
			default:
				return true;
			}
		}

		bool is_revolver()
		{
			if ( this )
			{
				int iWeaponID = this->GetItemDefenitionIndex();
				return ( iWeaponID == 64 );
			}
		}

		bool is_knife()
		{
			if ( this )
			{
				int idx = this->GetItemDefenitionIndex();
				return (idx == WEAPON_KNIFE || idx == WEAPON_KNIFE_BAYONET || idx == WEAPON_KNIFE_BUTTERFLY || idx == WEAPON_KNIFE_FALCHION
					|| idx == WEAPON_KNIFE_FLIP || idx == WEAPON_KNIFE_GUT || idx == WEAPON_KNIFE_KARAMBIT || idx == WEAPON_KNIFE_M9_BAYONET || idx == WEAPON_KNIFE_PUSH
					|| idx == WEAPON_KNIFE_SURVIVAL_BOWIE || idx == WEAPON_KNIFE_T || idx == WEAPON_KNIFE_TACTICAL);
			}
		}

		bool IsPinPulled()
		{
			if ( !this || !this->is_grenade() )
				return false;

			return *( bool* )( ( DWORD )this + offys.m_bPinPulled );
		}

		float GetThrowTime()
		{
			if ( !this || !this->is_grenade() )
				return 0.f;

			return *( float* )( ( DWORD )this + offys.m_fThrowTime );
		}

		bool is_grenade()
		{
			if ( !this )
				return false;

			int WeaponId = this->GetItemDefenitionIndex();

			if ( !WeaponId )
				return false;

			if ( WeaponId == 43 || WeaponId == 44 || WeaponId == 45 || WeaponId == 46 || WeaponId == 47 || WeaponId == 48 )
				return true;

			return false;
		}

		bool is_pistol()
		{
			if (!this)
				return false;

			int WeaponId = this->GetItemDefenitionIndex();

			if (!WeaponId)
				return false;
			
			if (WeaponId == 1 || WeaponId == 2 || WeaponId == 3 || WeaponId == 4 || WeaponId == 30 || WeaponId == 32 || WeaponId == 36 || WeaponId == 61 || WeaponId == 63)
				return true;

			return false;
		}

		int GetZoomLevel()
		{
			return *( PINT )( ( DWORD )this + offys.m_izoomLevel );
		}

		bool is_sniper()
		{
			if ( !this )
				return false;

			int WeaponId = this->GetItemDefenitionIndex();

			if ( !WeaponId )
				return false;

			if ( WeaponId == 9 || WeaponId == 11 || WeaponId == 38 || WeaponId == 40 )
				return true;

			return false;
		}

		bool is_rifle()
		{
			switch (this->get_csweapon_info()->type)
			{
			case WEAPONTYPE_RIFLE:
				return true;
			case WEAPONTYPE_SUBMACHINEGUN:
				return true;
			case WEAPONTYPE_SHOTGUN:
				return true;
			case WEAPONTYPE_MACHINEGUN:
				return true;
			default:
				return false;
			}
		}

		UINT* GetWeapons()
		{
			// DT_BasePlayer -> m_hMyWeapons
			return (UINT*)((DWORD)this + NetVarManager->getOffs("CBaseCombatCharacter", "m_hMyWeapons"));
		}

		PDWORD m_hWeaponWorldModel()
		{
			return (PDWORD)((DWORD)this + NetVarManager->getOffs("CBaseCombatWeapon", "m_hWeaponWorldModel"));
		}

		int* m_iViewModelIndex()
		{
			return (int*)((DWORD)this + NetVarManager->getOffs("CBaseCombatWeapon", "m_iViewModelIndex"));
		}

		int* m_iWorldModelIndex()
		{
			return (int*)((DWORD)this + NetVarManager->getOffs("CBaseCombatWeapon", "m_iWorldModelIndex"));
		}
		/*void set_model_index(int index)
		{
			typedef void(__thiscall* Fn)(void*, int);

			if (this)
				vfunc < Fn >(this, 75)(this, index);
		}
		short& m_iItemDefinitionIndex()
		{
			return *(short*)((UINT_PTR)this + offys.m_iitemdefinitionindex);
		}
		int& m_nModelIndex()
		{
			return *(int*)((UINT_PTR)this + offys.m_nModelIndex);
		}
		int& m_iViewModelIndex()
		{
			return *(int*)((UINT_PTR)this + offys.m_iViewModelIndex);
		}
		int& m_iWorldModelIndex()
		{
			return *(int*)((UINT_PTR)this + offys.m_iWorldModelIndex);
		}
		int& m_iEntityQuality()
		{
			return *(int*)((UINT_PTR)this + offys.m_iEntityQuality);
		}
		int& m_OriginalOwnerXuidLow()
		{
			return *(int*)((UINT_PTR)this + offys.m_OriginalOwnerXuidLow);
		}
		int& m_OriginalOwnerXuidHigh()
		{
			return *(int*)((UINT_PTR)this + offys.m_OriginalOwnerXuidHigh);
		}
		float& m_flFallbackWear()
		{
			return *(float*)((UINT_PTR)this + offys.m_flFallbackWear);
		}
		int& m_iItemIDHigh()
		{
			return *(int*)((UINT_PTR)this + offys.m_iItemIDHigh);
		}
		int& m_nFallbackPaintKit()
		{
			return *(int*)((UINT_PTR)this + offys.m_nFallbackPaintKit);
		}*/
#define GUNICON(t, x) case t: \
				return #x;

		const char* GetGunText( short itemdef )
		{
			switch ( ( ItemDefinitionIndex )itemdef )
			{
					GUNICON(ItemDefinitionIndex::WEAPON_DEAGLE, DESERT EAGLE);
					GUNICON(ItemDefinitionIndex::WEAPON_ELITE, DUAL BERETTAS);
					GUNICON(ItemDefinitionIndex::WEAPON_FIVESEVEN, FIVE-SEVEN);
					GUNICON(ItemDefinitionIndex::WEAPON_GLOCK, GLOCK-18);
					GUNICON(ItemDefinitionIndex::WEAPON_AK47, AK-47);
					GUNICON(ItemDefinitionIndex::WEAPON_AUG, AUG);
					GUNICON(ItemDefinitionIndex::WEAPON_AWP, AWP);
					GUNICON(ItemDefinitionIndex::WEAPON_FAMAS, FAMAS);
					GUNICON(ItemDefinitionIndex::WEAPON_G3SG1, G3SG1);
					GUNICON(ItemDefinitionIndex::WEAPON_GALILAR, GALIL-AR);
					GUNICON(ItemDefinitionIndex::WEAPON_M249, M249);
					GUNICON(ItemDefinitionIndex::WEAPON_M4A1, M4A4);
					GUNICON(ItemDefinitionIndex::WEAPON_MAC10, MAC-10);
					GUNICON(ItemDefinitionIndex::WEAPON_P90, P90);
					GUNICON(ItemDefinitionIndex::WEAPON_UMP45, UMP-45);
					GUNICON(ItemDefinitionIndex::WEAPON_XM1014, XM1014);
					GUNICON(ItemDefinitionIndex::WEAPON_BIZON, PP-BIZON);
					GUNICON(ItemDefinitionIndex::WEAPON_MAG7, MAG-7);
					GUNICON(ItemDefinitionIndex::WEAPON_NEGEV, NEGEV);
					GUNICON(ItemDefinitionIndex::WEAPON_SAWEDOFF, SAWED-OFF);
					GUNICON(ItemDefinitionIndex::WEAPON_TEC9, TEC-9);
					GUNICON(ItemDefinitionIndex::WEAPON_TASER, ZEUS X27);
					GUNICON(ItemDefinitionIndex::WEAPON_HKP2000, P2000);
					GUNICON(ItemDefinitionIndex::WEAPON_MP7, MP7);
					GUNICON(ItemDefinitionIndex::WEAPON_MP9, MP9);
					GUNICON(ItemDefinitionIndex::WEAPON_NOVA, NOVA);
					GUNICON(ItemDefinitionIndex::WEAPON_P250, P250);
					GUNICON(ItemDefinitionIndex::WEAPON_SCAR20, SCAR-20);
					GUNICON(ItemDefinitionIndex::WEAPON_SG556, SG-556);
					GUNICON(ItemDefinitionIndex::WEAPON_SSG08, SSG 08);
					GUNICON(ItemDefinitionIndex::WEAPON_KNIFE, KNIFE);
					GUNICON(ItemDefinitionIndex::WEAPON_FLASHBANG, FLASHBANG);
					GUNICON(ItemDefinitionIndex::WEAPON_HEGRENADE, HIGH EXPLOSIVE GRENADE);
					GUNICON(ItemDefinitionIndex::WEAPON_SMOKEGRENADE, SMOKE GRENADE);
					GUNICON(ItemDefinitionIndex::WEAPON_MOLOTOV, MOLOTOV);
					GUNICON(ItemDefinitionIndex::WEAPON_DECOY, DECOY);
					GUNICON(ItemDefinitionIndex::WEAPON_INCGRENADE, INCENDIARY GRENADE);
					GUNICON(ItemDefinitionIndex::WEAPON_C4, C4);
					GUNICON(ItemDefinitionIndex::WEAPON_KNIFE_T, KNIFE);
					GUNICON(ItemDefinitionIndex::WEAPON_M4A1_SILENCER, M4A1-S);
					GUNICON(ItemDefinitionIndex::WEAPON_USP_SILENCER, USP-S);
					GUNICON(ItemDefinitionIndex::WEAPON_CZ75A, CZ75-AUTO);
					GUNICON(ItemDefinitionIndex::WEAPON_REVOLVER, R8 REVOLVER);
					GUNICON(ItemDefinitionIndex::WEAPON_KNIFE_BAYONET, KNIFE);
					GUNICON(ItemDefinitionIndex::WEAPON_KNIFE_FLIP, KNIFE);
					GUNICON(ItemDefinitionIndex::WEAPON_KNIFE_GUT, KNIFE);
					GUNICON(ItemDefinitionIndex::WEAPON_KNIFE_KARAMBIT, KNIFE);
					GUNICON(ItemDefinitionIndex::WEAPON_KNIFE_M9_BAYONET, KNIFE);
					GUNICON(ItemDefinitionIndex::WEAPON_KNIFE_TACTICAL, KNIFE);
					GUNICON(ItemDefinitionIndex::WEAPON_KNIFE_FALCHION, KNIFE);
					GUNICON(ItemDefinitionIndex::WEAPON_KNIFE_SURVIVAL_BOWIE, KNIFE);
					GUNICON(ItemDefinitionIndex::WEAPON_KNIFE_BUTTERFLY, KNIFE);
					GUNICON(ItemDefinitionIndex::WEAPON_KNIFE_PUSH, KNIFE);
					GUNICON(ItemDefinitionIndex::WEAPON_MP5SD, MP5-SD);
					GUNICON(ItemDefinitionIndex::WEAPON_KNIFEGG, KNIFE);
					GUNICON(ItemDefinitionIndex::WEAPON_HEALTHSHOT, HEALTHSHOT);
					GUNICON(ItemDefinitionIndex::WEAPON_HAMMER, HAMMER);
					GUNICON(ItemDefinitionIndex::WEAPON_FISTS, FISTS);
					GUNICON(ItemDefinitionIndex::WEAPON_KNIFE_STILETTO, KNIFE);
					GUNICON(ItemDefinitionIndex::WEAPON_AXE, AXE);
					GUNICON(ItemDefinitionIndex::WEAPON_TABLET, TABLET);
					GUNICON(ItemDefinitionIndex::WEAPON_BREACHCHARGE, BREACH CHARGE);
					GUNICON(ItemDefinitionIndex::WEAPON_TAGRENADE, TA GRENADE);
					GUNICON(ItemDefinitionIndex::WEAPON_DIVERSION, DIVERSION GRENADE);
					GUNICON(ItemDefinitionIndex::WEAPON_KNIFE_GYPSY_JACKKNIFE, KNIFE);
					GUNICON(ItemDefinitionIndex::WEAPON_KNIFE_URSUS, KNIFE);
					GUNICON(ItemDefinitionIndex::WEAPON_KNIFE_WIDOWMAKER, KNIFE);
					GUNICON(ItemDefinitionIndex::WEAPON_SPANNER, SPANNER);

				default:
					return "UNKNOWN";
			}
		}
		char* GetGunIcon(short itemdef)
		{
			switch (itemdef)
			{
			case WEAPON_KNIFE:
			case WEAPON_KNIFE_T:
			case 500:
			case 505:
			case 506:
			case 507:
			case 508:
			case 509:
			case 512:
			case 514:
			case 515:
			case 516:
			case 520:
			case 522:
			case 519:
			case 523:
				return "]";
			case WEAPON_DEAGLE:
				return "A";
			case WEAPON_ELITE:
				return "B";
			case WEAPON_FIVESEVEN:
				return "C";
			case WEAPON_GLOCK:
				return "D";
			case WEAPON_HKP2000:
				return "E";
			case WEAPON_P250:
				return "F";
			case WEAPON_USP_SILENCER:
				return "G";
			case WEAPON_TEC9:
				return "H";
			case WEAPON_CZ75A:
				return "I";
			case WEAPON_REVOLVER:
				return "J";
			case WEAPON_MAC10:
				return "K";
			case WEAPON_UMP45:
				return "L";
			case WEAPON_BIZON:
				return "M";
			case WEAPON_MP7:
				return "N";
			case WEAPON_MP9:
				return "O";
			case WEAPON_P90:
				return "P";
			case WEAPON_GALILAR:
				return "Q";
			case WEAPON_FAMAS:
				return "R";
			case WEAPON_M4A1_SILENCER:
				return "S";
			case WEAPON_M4A1:
				return "T";
			case WEAPON_AUG:
				return "U";
			case WEAPON_SG556:
				return "V";
			case WEAPON_AK47:
				return "W";
			case WEAPON_G3SG1:
				return "X";
			case WEAPON_SCAR20:
				return "Y";
			case WEAPON_AWP:
				return "Z";
			case WEAPON_SSG08:
				return "a";
			case WEAPON_XM1014:
				return "b";
			case WEAPON_SAWEDOFF:
				return "c";
			case WEAPON_MAG7:
				return "d";
			case WEAPON_NOVA:
				return "e";
			case WEAPON_NEGEV:
				return "f";
			case WEAPON_M249:
				return "g";
			case WEAPON_TASER:
				return "h";
			case WEAPON_FLASHBANG:
				return "i";
			case WEAPON_HEGRENADE:
				return "j";
			case WEAPON_SMOKEGRENADE:
				return "k";
			case WEAPON_MOLOTOV:
				return "l";
			case WEAPON_DECOY:
				return "m";
			case WEAPON_INCGRENADE:
				return "n";
			case WEAPON_C4:
				return "o";
			default:
				return " ";
			}
		}

		int GetLoadedAmmo()
		{
			if ( this )
				return *( int* )( ( DWORD )this + 0x3204 );
		}

		ClientClass* GetClientClass()
		{
			void* Networkable = ( void* )( this + 0x8 );
			typedef ClientClass*( __thiscall * OriginalFn )( void* );

			if ( this )
				return vfunc<OriginalFn>( Networkable, 2 )( Networkable );
		}
		float GetAccuracyPenalty()
		{
			static int m_fAccuracyPenalty = NetVarManager->getOffs("CWeaponCSBase", "m_fAccuracyPenalty");
			return *(float*)((DWORD)this + m_fAccuracyPenalty);
		}
		void UpdateAccuracyPenalty()
		{
			typedef void( __thiscall * Fn )( void* );

			if ( this )
				return vfunc<Fn>( this, 483)( this );
		}

		bool IsReloading()
		{
			if(this)
				return *( bool* )((uint32_t)this +*(uint32_t*)offys.dwReloading);
		}

		int32_t WeaponMode()
		{
			return *(int32_t*)((DWORD)this + offys.m_weaponMode);
		}

		CSWeaponInfo* get_full_info()
		{
			/*if (!this || this == NULL)
				return NULL;

			using Fn = CSWeaponInfo * (__thiscall*)(void*);

			static auto fn = reinterpret_cast<Fn>(Utilities->Memory_PatternScan("client.dll", "55 8B EC 81 EC ? ? ? ? 53 8B D9 56 57 8D 8B"));
			return fn(this);*/
			if ( this && this != NULL && this != nullptr )
			{
				typedef CSWeaponInfo *( __thiscall * fn )( void* );
				return vfunc<fn>( this, 460)( this );
			}
		}
		CSWeaponInfo * get_csweapon_info() {
			if (!this || this == NULL)
				return NULL;

			using Fn = CSWeaponInfo * (__thiscall*)(void*);

			static auto fn = reinterpret_cast<Fn>(Utilities->Memory_PatternScan("client.dll", "55 8B EC 81 EC ? ? ? ? 53 8B D9 56 57 8D 8B ? ? ? ? 85 C9 75 04 33 FF EB 2F"));
			return fn(this);
		}

		int Type()
		{
			if ( !this )
				return WEAPONTYPE_UNKNOWN;

			auto id = this->GetItemDefenitionIndex();

			switch ( ( ItemDefinitionIndex )id )
			{
				case ItemDefinitionIndex::WEAPON_DEAGLE:
				case ItemDefinitionIndex::WEAPON_ELITE:
				case ItemDefinitionIndex::WEAPON_FIVESEVEN:
				case ItemDefinitionIndex::WEAPON_HKP2000:
				case ItemDefinitionIndex::WEAPON_USP_SILENCER:
				case ItemDefinitionIndex::WEAPON_CZ75A:
				case ItemDefinitionIndex::WEAPON_TEC9:
				case ItemDefinitionIndex::WEAPON_REVOLVER:
				case ItemDefinitionIndex::WEAPON_GLOCK:
				case ItemDefinitionIndex::WEAPON_P250:
					return WEAPONTYPE_PISTOL;
					break;

				case ItemDefinitionIndex::WEAPON_AK47:
				case ItemDefinitionIndex::WEAPON_M4A1:
				case ItemDefinitionIndex::WEAPON_M4A1_SILENCER:
				case ItemDefinitionIndex::WEAPON_GALILAR:
				case ItemDefinitionIndex::WEAPON_AUG:
				case ItemDefinitionIndex::WEAPON_FAMAS:
				case ItemDefinitionIndex::WEAPON_SG556:
					return WEAPONTYPE_RIFLE;
					break;

				case ItemDefinitionIndex::WEAPON_P90:
				case ItemDefinitionIndex::WEAPON_BIZON:
				case ItemDefinitionIndex::WEAPON_MP7:
				case ItemDefinitionIndex::WEAPON_MP9:
				case ItemDefinitionIndex::WEAPON_MAC10:
				case ItemDefinitionIndex::WEAPON_UMP45:
				case ItemDefinitionIndex::WEAPON_MP5SD:
					return WEAPONTYPE_SUBMACHINEGUN;
					break;

				case ItemDefinitionIndex::WEAPON_AWP:
				case ItemDefinitionIndex::WEAPON_G3SG1:
				case ItemDefinitionIndex::WEAPON_SCAR20:
				case ItemDefinitionIndex::WEAPON_SSG08:
					return WEAPONTYPE_SNIPER_RIFLE;
					break;

				case ItemDefinitionIndex::WEAPON_NEGEV:
				case ItemDefinitionIndex::WEAPON_M249:
					return WEAPONTYPE_MACHINEGUN;
					break;

				case ItemDefinitionIndex::WEAPON_MAG7:
				case ItemDefinitionIndex::WEAPON_SAWEDOFF:
				case ItemDefinitionIndex::WEAPON_NOVA:
				case ItemDefinitionIndex::WEAPON_XM1014:
					return WEAPONTYPE_SHOTGUN;
					break;

				case ItemDefinitionIndex::WEAPON_KNIFE:
				case ItemDefinitionIndex::WEAPON_KNIFE_BAYONET:
				case ItemDefinitionIndex::WEAPON_KNIFE_BUTTERFLY:
				case ItemDefinitionIndex::WEAPON_KNIFE_FALCHION:
				case ItemDefinitionIndex::WEAPON_KNIFE_FLIP:
				case ItemDefinitionIndex::WEAPON_KNIFE_GUT:
				case ItemDefinitionIndex::WEAPON_KNIFE_KARAMBIT:
				case ItemDefinitionIndex::WEAPON_KNIFE_TACTICAL:
				case ItemDefinitionIndex::WEAPON_KNIFE_M9_BAYONET:
				case ItemDefinitionIndex::WEAPON_KNIFE_PUSH:
				case ItemDefinitionIndex::WEAPON_KNIFE_SURVIVAL_BOWIE:
				case ItemDefinitionIndex::WEAPON_KNIFE_T:
					return WEAPONTYPE_KNIFE;
					break;

				default:
					return WEAPONTYPE_UNKNOWN;
			}
		}
		char	_pad_0x0000[0x09CC];
		Vector	m_CustomMaterials; //0x09DC
		char	_pad_0x09F0[0x2380];
		CAttributeManager			m_AttributeManager; //0x2D70
		char	_pad_0x2D84[0x2F9];
		bool    m_bCustomMaterialInitialized;

};



class CCSBomb
{
	public:
		// Is bomb still ticking down
		bool IsBombTicking()
		{
			if ( this )
				return *( bool* )( ( DWORD )this + offys.m_bBombTicking );
		}

		// Where is bomb planted
		int GetBombSite()
		{
			if ( this )
				return *( int* )( ( DWORD )this + offys.m_nBombSite );
		}

		// When bomb will be exploded
		float GetBlowTime()
		{
			if ( this )
				return *( float* )( ( DWORD )this + offys.m_flC4Blow );
		}

		// Length of bomb timer
		float GetTimerLength()
		{
			if ( this )
				return *( float* )( ( DWORD )this + offys.m_flTimerLength );
		}

		// Length of bomb defuse timer
		float GetDefuseLength()
		{
			if ( this )
				return *( float* )( ( DWORD )this + offys.m_flDefuseLength );
		}

		// When bomb will be defused
		float GetDefuseTime()
		{
			if ( this )
				return *( float* )( ( DWORD )this + offys.m_flDefuseCountDown );
		}

		// Was bomb defused
		bool IsBombDefused()
		{
			if ( this )
				return *( bool* )( ( DWORD )this + offys.m_bBombDefused );
		}

		ULONG GetBombDefuser()
		{
			if ( !this )
				return NULL;

			return *( PULONG )( ( DWORD )this + offys.m_hBombDefuser );
		}
};
