#pragma once
#include "SDK\SDK.h"

#define M_PI		3.14159265358979323846f





//-----SkyBox Name List-----//
#define BAGGAGE		"cs_baggage_"
#define TIBET		"cs_tibet"
#define EMBASSY		"embassy"
#define ITALY		"italy"
#define JUNGLE		"jungle"
#define NUKEBLAN		"nukeblank"
#define OFFICE		"office"
#define DAYLIGHT01	"sky_cs15_daylight01_hdr"
#define DAYLIGHT02	"sky_cs15_daylight02_hdr"
#define DAYLIGHT03	"sky_cs15_daylight03_hdr"
#define DAYLIGHT04	"sky_cs15_daylight04_hdr"
#define CLOUDY01		"sky_csgo_cloudy01"
#define NIGHT02		"sky_csgo_night02"
#define NIGHT02B		"sky_csgo_night02b"
#define DUST			"sky_dust"
#define VENICE		"sky_venice"
#define VERTIGO		"vertigo"
#define VIETNAM		"vietnam"
//-----End Of SkyBox Name List-----//





//-----Math Define List-----//
#define square( x ) ( x * x )

#define PI 3.14159265358979323846f

#define		MEMCHECK			CHECKMEM
#define		CHECKMEM			__()
#define		MEMSAFE				G::LocalPlayer->GID()

#define RADPI 57.295779513082f
//-----End Of Math Define List-----//




/*
//-----Static Offsets & NetVars-----//
#define m_nVersion				0x8222E0
#define m_dwGlowObject			0x4F89144
#define m_dwEntityList			0x04A6DA5C
#define m_dwLocalPlayer			0xA4C58C
#define m_dwClientState			0x5A29FC
#define m_dwViewMatrix			0x04a5f504

#define m_iItemIDLow			0x2FA4
#define iViewModelIndex			0x31e0
#define iWorldModelIndex		0x31E4
#define m_bIsScoped				0x389c
#define m_iDefaultFOV			0x332c
#define m_iCrossHairID			0x0000AA70
#define m_dwGlowIndex			0x0000A320
#define m_iShotsFired			0x0000A2C0
#define m_vecPunch				0x0000301C
#define boneMatrix				0x00002698
#define m_dwLocalPlayerIndex	0x00000178
#define m_bDormant				0x000000E9

#define m_dwViewAngles			0x00004D0C
#define m_flFlashDuration		0x0000A308
#define m_hMyWeapons			0x2DE8
#define m_iItemDefinitionIndex	0x2F88
#define m_bSpotted				0x0000097C
#define m_hActiveWeapon			0x2EE8
#define m_iWeaponID				0x000032EC
#define m_iFOV					0x31D4
#define m_fAccuracyPenalty		0x32C0
#define m_Collision				0x0318
#define m_bGunGameImmunity		0x000038B0
#define m_bHasDefuser			0xAA14
#define m_flTimerLength			0x2980
#define m_flC4Blow				0x297C
#define m_flDefuseCountDown		0x2994
#define m_AttributeManager		0x2D70
#define m_Item					0x2DB0
#define m_iAccountID			0x2FA8
#define m_iEntityQuality		0x2F8C
#define m_iItemIDHigh			0x2FA0
#define m_szCustomName			0x301C
#define m_OriginalOwnerXuidLow	0x3168
#define m_OriginalOwnerXuidHigh	0x316C
#define m_nFallbackPaintKit		0x3170
#define m_nFallbackSeed			0x3174
#define m_flFallbackWear		0x3178
#define m_nFallbackStatTrak		0x317C
#define m_nIndex				0x001C
#define m_bShouldGlow			0x29B8
#define m_iViewModelIndex		0x3A40
#define m_hViewModel			0x3AD4
#define m_nModelIndex			0x28C
#define m_dwMapName				0x00000284
#define m_bHasHelmet			0xA9FC



#define m_angEyeAngles			0x0000AA08
#define m_iTeamNum				0x000000F0                // DT_BaseEntity -> m_iTeamNum
#define m_vecOrigin				0x00000134            // DT_BaseEntity -> m_vecOrigin
#define m_iHealth				0x000000FC                // DT_BasePlayer -> m_iHealth
#define m_fFlags				0x00000100                // DT_BasePlayer -> m_fFlags
#define m_lifeState				0x0000025B            // DT_BasePlayer -> m_lifeState
#define m_ArmorValue			0x0000AA04            // DT_CSPlayer -> m_ArmorValue
//-----End Of Static Offsets & NetVars-----//*/





//--------------------------------------
//-----XorStr Function [Not List]-----//
//--------------------------------------
#include <string>
#include <array>
#include <cstdarg>

#define BEGIN_NAMESPACE( x ) namespace x {
#define END_NAMESPACE }

BEGIN_NAMESPACE(XorCompileTime)
constexpr auto time = __TIME__;
constexpr auto seed = static_cast<int>(time[7]) + static_cast<int>(time[6]) * 10 + static_cast<int>(time[4]) * 60 + static_cast<int>(time[3]) * 600 + static_cast<int>(time[1]) * 3600 + static_cast<int>(time[0]) * 36000;

// 1988, Stephen Park and Keith Miller
// "Random Number Generators: Good Ones Are Hard To Find", considered as "minimal standard"
// Park-Miller 31 bit pseudo-random number generator, implemented with G. Carta's optimisation:
// with 32-bit math and without division

template <int N>
struct RandomGenerator
{
private:
	static constexpr unsigned a = 16807; // 7^5
	static constexpr unsigned m = 2147483647; // 2^31 - 1

	static constexpr unsigned s = RandomGenerator<N - 1>::value;
	static constexpr unsigned lo = a * (s & 0xFFFF); // Multiply lower 16 bits by 16807
	static constexpr unsigned hi = a * (s >> 16); // Multiply higher 16 bits by 16807
	static constexpr unsigned lo2 = lo + ((hi & 0x7FFF) << 16); // Combine lower 15 bits of hi with lo's upper bits
	static constexpr unsigned hi2 = hi >> 15; // Discard lower 15 bits of hi
	static constexpr unsigned lo3 = lo2 + hi;

public:
	static constexpr unsigned max = m;
	static constexpr unsigned value = lo3 > m ? lo3 - m : lo3;
};

template <>
struct RandomGenerator<0>
{
	static constexpr unsigned value = seed;
};

template <int N, int M>
struct RandomInt
{
	static constexpr auto value = RandomGenerator<N + 1>::value % M;
};

template <int N>
struct RandomChar
{
	static const char value = static_cast<char>(1 + RandomInt<N, 0x7F - 1>::value);
};

template <size_t N, int K>
struct XorString
{
private:
	const char _key;
	std::array<char, N + 1> _encrypted;

	constexpr char enc(char c) const
	{
		return c ^ _key;
	}

	char dec(char c) const
	{
		return c ^ _key;
	}

public:
	template <size_t... Is>
	constexpr __forceinline XorString(const char* str, std::index_sequence<Is...>) : _key(RandomChar<K>::value), _encrypted{ enc(str[Is])... }
	{
	}

	__forceinline decltype(auto) decrypt(void)
	{
		for (size_t i = 0; i < N; ++i)
		{
			_encrypted[i] = dec(_encrypted[i]);
		}
		_encrypted[N] = '\0';
		return _encrypted.data();
	}
};

//--------------------------------------------------------------------------------
//-- Note: XorStr will __NOT__ work directly with functions like printf.
//         To work with them you need a wrapper function that takes a const char*
//         as parameter and passes it to printf and alike.
//
//         The Microsoft Compiler/Linker is not working correctly with variadic 
//         templates!
//  
//         Use the functions below or use std::cout (and similar)!
//--------------------------------------------------------------------------------

static auto w_printf = [](const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	vprintf_s(fmt, args);
	va_end(args);
};

static auto w_printf_s = [](const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	vprintf_s(fmt, args);
	va_end(args);
};

static auto w_sprintf = [](char* buf, const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	vsprintf(buf, fmt, args);
	va_end(args);
};

static auto w_sprintf_s = [](char* buf, size_t buf_size, const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	vsprintf_s(buf, buf_size, fmt, args);
	va_end(args);
};
static bool w_strcmp(const char* str1, const char* str2)
{
	return strcmp(str1, str2);
};

#define XorStr( s ) ( s )

/*#ifdef NDEBUG
#define XorStr( s ) ( XorCompileTime::XorString< sizeof( s ) - 1, __COUNTER__ >( s, std::make_index_sequence< sizeof( s ) - 1>() ).decrypt() )
#endif
#define XorStr( s ) ( s )
#endif*/

END_NAMESPACE
//---------------------------------------------
//-----End Of XorStr Function [Not List]-----//
//---------------------------------------------
