#include "offset.h"
#include "iinputsystem.h"

COffset g_Offset;

void COffset::init()
{
	if( GetModuleHandleA( "client.dll" ) )
	{
		bIsPanorama = true;
		szClient = "client.dll";
	}
	else
	{
		bIsPanorama = false;
		szClient = "client.dll";
	}

    loadSky = g_Pattern.PatternSearchIDA( "engine.dll", "55 8B EC 81 EC ? ? ? ? 56 57 8B F9 C7 45" );
    InvalidateBoneCache = g_Pattern.PatternSearchIDA( szClient.c_str(), "80 3D ? ? ? ? ? 74 16 A1 ? ? ? ? 48 C7 81" );
    LobbyScreen = g_Pattern.PatternSearchIDA( szClient.c_str(), "8B 0D ? ? ? ? 85 C9 75 57" ) + 0x2;
    ServerConfirmedReservationCallback = g_Pattern.PatternSearchIDA( szClient.c_str(), "A1 ? ? ? ? 85 C0 74 0D 83 B8 ? ? ? ? ? 0F 84 ? ? ? ? 6A 0A" ) + 0x1;
    BehindSmoke = g_Pattern.PatternSearchIDA( szClient.c_str(), "55 8B EC 83 EC 08 8B 15 ? ? ? ? 0F 57 C0" );
    isReady = g_Pattern.PatternSearchIDA( szClient.c_str(), "55 8B EC 83 E4 F8 83 EC 08 56 8B 35 ? ? ? ? 57 83 BE" );
    RetStaticProp = g_Pattern.PatternSearchIDA( "engine.dll", "B9 ? ? ? ? FF 50 34 85 C0 75 03 32 C0" );
    GlowObjectManager = g_Pattern.PatternSearchIDA( szClient.c_str(), "0F 11 05 ? ? ? ? 83 C8 01 C7 05 ? ? ? ? 00 00 00 00" ) + 3;
    AllowInventory = g_Pattern.PatternSearchIDA( szClient.c_str(), "84 C0 75 04 B0 01 5F" );
    SmokeCount = g_Pattern.PatternSearchIDA( szClient.c_str(), "A3 ? ? ? ? 57 8B CB" );
    RankReveal = g_Pattern.PatternSearchIDA( szClient.c_str(), "55 8B EC 8B 0D ? ? ? ? 68" );
}