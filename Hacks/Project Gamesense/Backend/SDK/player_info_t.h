#pragma once

struct player_info_t
{
	__int64         unknown;            //0x0000 
	union {
		__int64       steamID64;          //0x0008 - SteamID64
		struct {
			int xuidlow;
			int xuidhigh;
		};
	};
	char            szName[128];        //0x0010 - Player Name
	int             userId;             //0x0090 - Unique Server Identifier
	char            szSteamID[20];      //0x0094 - STEAM_X:Y:Z
	char            pad_0x00A8[0x10];   //0x00A8
	unsigned long   iSteamID;           //0x00B8 - SteamID 
	char            szFriendsName[128];
	bool            fakeplayer; // Is BOT
	bool            ishltv;
	unsigned int    customFiles[4];
	unsigned char   filesDownloaded;
};