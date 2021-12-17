#pragma once

#include "main.h"

struct Var
{
	struct Ragebot
	{
		int Enable;
		int Autoshoot;
		int Multipoint = 1;
		int Multipointbox[ 5 ];
		float HeadPointScale = 0.85;
		float PointScale = 0.85;
		int AutoPointScale;
		float BodyAfter;
		int Team;
		int Spot;
		int Recoil;
		int Spread;
		int Autozoom;
		int Autostop;
		int ForceBaim;
		int PosAdjust;
		int Selection;
		int Multibox;
		int Multiboxes[ 5 ];
		int IgnoreHead[ 5 ];
		int PriorityRecord[6];
		float BodyAimHp;
		int Limbs[ 2 ];
		int Silent = 1;
		float Fov = 180;
		float Hitchance;
		int Autowall = 1;
		float Mindamage = 20;
		float Walldamage = 20;
		int AutoMindDamage;
	} Ragebot[ 9 ];

	struct AntiAim
	{
		int Enable;
		int Pitch;

		int Move;
		int MoveEnemyBased;
		int MoveStyle;
		float MoveValue;
		float MoveSpeed;

		int Air;
		int AirEnemyBased;
		int AirStyle;
		float AirValue;
		float AirSpeed;

		int Stand;
		int StandEnemyBased;
		int StandStyle;
		float StandValue;
		float StandSpeed;

		int Break;
		int BreakToggle;
		float BreakRad;

		int LeftToggle, RightToggle;
		int BackToggle, FrontToggle;
		int LockToggle;
		float Points = 8;
		float Tolerance = 45;
		float SideWaysTolerance = 90;
		int Override;
		int Resolve;
		int Fakewalk;
	}AntiAim;

	struct Fakelag
	{
		int Enable;
		float Factor = 13;
		int Style;
		int Flags[ 6 ];
	}Fakelag;

	struct Visuals
	{
		struct Player
		{
			int Enable;
			int Team;
			int Box = 1;
			int Name = 1;
			int Health = 1;
			int Weapon = 2;
			int KevHelm = 1;
			int AmmoBar = 1;
			int LowerBody = 1;
			int Bone;
			float Surrounding;
			int Radar;
			int Spectators;

		} Player;

		struct Local
		{
			int Enable;
			int Chams;
			int FakeChams;
			int Glow;
			int Draw[ 6 ];
			float Transparency = 100.f;
		} Local;

		struct Chams
		{
			int Enable;
			int Players;
			int Team;
			float TeamTransparency = 100.f;
			int DrawOriginal;
			float Transparency = 100.f;
			int Weapons;
			int View;
			int Hands;
			float HandsTransparency = 100.f;
			int BacktrackMaterial;
			float BacktrackAlpha;
		} Chams;

		struct Glow
		{
			int Enable;
			int Player;
			int Weapons;
		} Glow;

		struct World
		{
			float ViewModel;
			float ViewModelOffset[3] = { 90.f, 90.f, 90.f };
			float Fieldofview;
			float PropsAlpha = 100;
			float Nightmode = 100;
			int Grenade[ 6 ];
			int Crosshair[ 4 ];
			int Removals[ 6 ];
			int Hitmarker;
			int BulletTracer[ 5 ];
			int Fullbright;
			int Ragdoll;
		} World;
	} Visuals;

	struct Misc
	{
		int EventLog[ 4 ];
		int Indicator[ 6 ];
		int Clantag;
		int ZeusBot[2];
		int KnifeBot[2];
		int ExtendKillfeed = 1;
		int Insults;

		int AutoJump;
		int AutoStrafe;
		int Slide;
		int CrouchJump;
		int FakeDuck;

		int DebugResolver;
		int DebugMultiPoint;

        int Thirdperson;
		float ThirdpersonDist = 160;
        int ThirdpersonToggleKey;

		int Untrusted = 1;

		struct BuyBot
		{
			int Primary;
			int Secondary;
			int Gear[ 10 ];
			int Grenades[ 10 ];
		} BuyBot;

		int AnimFix;
	} Misc;

	struct Color
	{
		int Menu[ 3 ] = { 66, 176, 244 };

		int HandChams[ 3 ] = { 0, 255, 255 };
		int HandChamsVisible[ 3 ] = { 170, 255, 45 };

		int WeaponChams[ 3 ] = { 0, 255, 255 };
		int WeaponChamsVisible[ 3 ] = { 255, 255, 255 };

		int ViewChams[ 3 ] = { 255, 255, 255 };

		int Glow[ 3 ] = { 170, 255, 45 };
		int GlowVisible[ 3 ] = { 255, 255, 255 };

		int Chams[3] = { 170, 255, 45 };
		int ChamsVisible[3] = { 255, 255, 255 };

		int BacktrackChams[3] = { 255, 255, 255 };

		int TeamChams[ 3 ] = { 0, 255, 255 };
		int TeamChamsVisible[ 3 ] = { 255, 255, 255 };

		int LocalChams[ 3 ] = { 255, 255, 255 };
		int LocalGlow[ 3 ] = { 255, 255, 255 };

		int LocalFakeChams[3] = { 153, 186, 239 };

		int Surrounding[ 3 ] = { 255, 255, 255 };

		int Name[ 3 ] = { 255, 255, 255 };
		int Weapon[ 3 ] = { 255, 255, 255 };
		int Kevlar[ 3 ] = { 255, 255, 255 };
		int Health[ 3 ] = { 0, 255, 0 };

		int Player[ 3 ] = { 255, 255, 255 };
		int PlayerVisible[ 3 ] = { 255, 255, 255 };

		int Team[ 3 ] = { 255, 255, 255 };
		int TeamVisible[ 3 ] = { 255, 255, 255 };

		int AmmoBar[ 3 ] = { 51, 153, 255 };

		int LowerBodyBar[ 3 ] = { 155, 255, 66 };

		int BulletTracer[ 3 ] = { 100, 100, 255 };
	} Color;
};

extern Var g_Var;

struct Skins
{
	int Enable[ 517 ];
	int Knife;

	int Glove;
	int GlovePaint;

	int PaintKit[ 517 ];
	int Seed[ 517 ];
	float Condition[ 517 ];
};

extern Skins g_Skins;

/*static void LoadSkinConfig()
{
	std::string szCfg = "c:/kidua/skins.cfg";

	std::ifstream f( szCfg.c_str() );

	if( !f.good() )
		return;

	FILE* fin = fopen( szCfg.c_str(), "r" );
	fread( &g_Var, sizeof( Skins ), 1, fin );
	fclose( fin );
}

inline bool exists( const std::string& name ) {
	struct stat buffer;
	return ( stat( name.c_str(), &buffer ) == 0 );
}

static void SaveSkinConfig()
{
	if( !exists( "c:/kidua/" ) )
		_mkdir( "c:/kidua/" );

	std::string szCfg = "c:/kidua/skins.cfg";

	FILE* fout = fopen( szCfg.c_str(), "w" );
	fwrite( &g_Var, sizeof( Skins ), 1, fout );
	fclose( fout );
}*/

class CConfigFile
{
public:
	int m_iID;
	const bool LoadConfig();
	const bool LoadSkinConfig();
	const bool SaveConfig();
	const bool SaveSkinConfig();
};

extern CConfigFile g_ConfigFile;