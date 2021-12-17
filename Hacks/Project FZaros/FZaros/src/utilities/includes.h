#pragma once


#include <d3d9.h>
#pragma comment(lib, "d3dx9")

#include "../utilities/config.hpp"

#include "../sdk/Color.hpp"
#include "../utilities/input.hpp"
#include "../utilities/render.hpp"
#include "../core/managers/interfaceManager.hpp"
#include "../core/managers/hookManager.hpp"
#include "netvarManager.hpp"
#include "utilities.hpp"
#include "memory.hpp"
#include <deque>
#include <functional>
#include <fstream>
#include <filesystem>
namespace fs = std::filesystem;

//#include "../menu/config.hpp"

#include "../sdk/matrix.hpp"

#include "math.hpp"
#include "../sdk/modelInfo.hpp"
#include "../sdk/interfaces/ICvar.hpp"
#include "../sdk/materialSystem.hpp"
#include "../sdk/interfaces/IRenderView.hpp"
#include "../sdk/interfaces/engineTrace.hpp"
#include "../sdk/bspflags.hpp"
#include "../sdk/surfaceData.hpp"
#include "../sdk/prediction.hpp"
#include "../sdk/glowObjectManager.hpp"
#include "../sdk/gameEventManager.hpp"
#include "../sdk/platform.hpp"
#include "../sdk/CUtlMemory.hpp"
#include "../sdk/CUtlString.hpp"
#include "../sdk/Convar.hpp"
#include "../sdk/interfaces/renderBeams.hpp"
#include "../sdk/interfaces/debugOverlay.hpp"
#include "../sdk/prediction.hpp"

#include "../features/featureManager.hpp"

#include "../menu/config/config.hpp"

struct EventInfo
{
	std::string m_szMessage;
	float m_flExpTime;
};

namespace Globals //TODO: make these inline..
{
	extern HWND csgoWindow;
	extern WNDPROC wndProcOrig;
	extern char documentsFolder[ MAX_PATH ];

	extern CBaseEntity* localPlayer;
	extern bool bSendPacket;
	extern int chokedPackets;
	extern matrix3x4_t fakeLagMatrix[ 128 ];
	extern Vector3 fakeLagOrigin;
	extern float autowallDmg;
	extern CUserCmd* oCmd;
	extern Vector3 originalViewAngle;
	extern Vector3 realAngle;
	extern Vector3 aimbotHitbox[ 65 ][ 28 ];
	extern Vector3 enemyEyeAngles[ 65 ];
	extern int targetIndex;
	extern bool Shot[ 65 ];
	extern bool Hit[ 65 ];
	extern int MissedShots[65];
	extern bool inThirdperson;
	extern bool breakingLC;
	extern Vector3 fakeAngle;
	extern bool desyncing;
	extern bool inFakeDuck;
	extern bool isSlowWalking;
	extern matrix3x4_t aimbotHitMatrix[ 128 ];
	extern std::vector<EventInfo> eventInfo;

	extern bool fakeLagging;
	extern bool disableAA;
	extern Vector3 fakePosition;
	extern bool desyncEnabledSpecialChecks;

	// Here goes nothing..
	extern std::deque<Vector3> dequeBulletImpacts;

	extern bool bPlayerHurt[ 64 ]; //when player_hurt called
	extern bool bBulletImpact[ 64 ]; //when bullet_impact called
	extern bool bWeaponFire[ 64 ]; //when weapon_fire called
	extern bool shouldDormant;

	extern unsigned long g_iModelBoneCounter;

	using msg_t = void( __cdecl* )( const char*, ... );
	extern msg_t		Msg;

	template <typename ... Args>
	void Message( const std::string& str, Args ...arguments ) {
		Msg( std::string( str ).append( "    \n" ).c_str( ), arguments... );
	}
}

#define TIME_TO_TICKS( dt )		( (int)( 0.5 + (float)(dt) / g_Interfaces->globalVars->intervalPerTick ) )
#define TICKS_TO_TIME( t )		( g_Interfaces->globalVars->intervalPerTick *( t ) )

#define IN_ATTACK        (1 << 0)
#define IN_JUMP          (1 << 1)
#define IN_DUCK          (1 << 2)
#define IN_FORWARD       (1 << 3)
#define IN_BACK          (1 << 4)
#define IN_USE           (1 << 5)
#define IN_CANCEL        (1 << 6)
#define IN_LEFT          (1 << 7)
#define IN_RIGHT         (1 << 8)
#define IN_MOVELEFT      (1 << 9)
#define IN_MOVERIGHT     (1 << 10)
#define IN_ATTACK2       (1 << 11)
#define IN_RUN           (1 << 12)
#define IN_RELOAD        (1 << 13)
#define IN_ALT1          (1 << 14)
#define IN_ALT2          (1 << 15)
#define IN_SCORE         (1 << 16) // Used by client.dll for when scoreboard is held down
#define IN_SPEED         (1 << 17) // Player is holding the speed key
#define IN_WALK          (1 << 18) // Player holding walk key
#define IN_ZOOM          (1 << 19) // Zoom key for HUD zoom
#define IN_WEAPON1       (1 << 20) // weapon defines these bits
#define IN_WEAPON2       (1 << 21) // weapon defines these bits
#define IN_BULLRUSH      (1 << 22)
#define IN_GRENADE1      (1 << 23) // grenade 1
#define IN_GRENADE2      (1 << 24) // grenade 2
#define IN_ATTACK3       (1 << 25)

#define HITGROUP_GENERIC  0
#define HITGROUP_HEAD     1
#define HITGROUP_CHEST    2
#define HITGROUP_STOMACH  3
#define HITGROUP_LEFTARM  4    
#define HITGROUP_RIGHTARM 5
#define HITGROUP_LEFTLEG  6
#define HITGROUP_RIGHTLEG 7
#define HITGROUP_GEAR    10

#define HITBOX_HEAD   0
#define HITBOX_NECK   1
#define HITBOX_PELVIS 2
#define HITBOX_BELLY  3
#define HITBOX_THORAX 4
#define HITBOX_LOWER_CHEST 5
#define HITBOX_UPPER_CHEST 6
#define HITBOX_RIGHT_THIGH 7
#define HITBOX_LEFT_THIGH  8
#define HITBOX_RIGHT_CALF  9
#define HITBOX_LEFT_CALF  10
#define HITBOX_RIGHT_FOOT 11
#define HITBOX_LEFT_FOOT  12
#define HITBOX_RIGHT_HAND 13
#define HITBOX_LEFT_HAND  14
#define HITBOX_RIGHT_UPPER_ARM 15
#define HITBOX_RIGHT_FOREARM   16
#define HITBOX_LEFT_UPPER_ARM  17
#define HITBOX_LEFT_FOREARM    18
#define HITBOX_MAX 19


#include "../features/featureManager.hpp"

class c_config {
private:
	std::unordered_map< std::string, std::string > config_map;

public:
	void save( const std::string &file_name );
	void load( const std::string &file_name );
	//bool save( std::string file_name );
	//bool load( std::string file_name );

	void set( const std::string &key, bool value );
	void set( const std::string &key, int value );
	void set( const std::string &key, float value );
	void set( const std::string &key, double value );
	void set( const std::string &key, const std::string &value );
	void set( const std::string &key, const Color &value );
	void set( const std::string &key, const Vector2 &value );
	void set( const std::string &key, const Vector3 &value );

	bool get_bool( const std::string &key, bool default_value = false );
	int get_int( const std::string &key, int default_value = 0 );
	float get_float( const std::string &key, float default_value = 0.0f );
	double get_double( const std::string &key, double default_value = 0.0 );
	std::string get_string( const std::string &key, const std::string &default_value = "" );
	Color get_color( const std::string &key, const Color &default_value = Color( 255, 255, 255 ) );
	Vector2 get_vector2( const std::string &key, const Vector2 &default_value = Vector2( 0, 0 ) );
	Vector3 get_vector3( const std::string &key, const Vector3 &default_value = Vector3( 0, 0, 0 ) );

};

extern c_config *config;