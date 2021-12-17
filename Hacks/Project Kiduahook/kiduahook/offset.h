#pragma once
#include "main.h"

class COffset
{
public:
	std::string szClient;
	bool bIsPanorama;
    void init();

    DWORD loadSky;
    DWORD InvalidateBoneCache;
    DWORD LobbyScreen;
    DWORD ServerConfirmedReservationCallback;
    DWORD BehindSmoke;
    DWORD isReady;
    DWORD RetStaticProp;
    DWORD GlowObjectManager;
    DWORD AllowInventory;
    DWORD SmokeCount;
    DWORD RankReveal;
};

extern COffset g_Offset;