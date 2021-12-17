#pragma once
#include <string>
#include <vector>
struct Variables
{
	struct
	{
		bool Enable;
		bool Silent;
		bool LagFix;
		bool bInterpLagComp;
		bool AutoFire;
		bool AutoScope;
		int TargetSelection = 0;
		struct
		{
			float Hitchance;
			float Mindamage;
			bool PositionAdjustment;
			bool DelayShot;
			bool RemoveRecoil;
			struct
			{
				bool Pitch;
				bool Yaw;
				int Override;
			}Resolver;
		}Accuracy;
		struct
		{
			bool Multipoint;
			float PointScale = 70.f;
			int PriorityHitbox;
			int Multibox;
			int Bodyaim;
		}Hitscan;
		struct
		{
			bool Enable, Desync, Freestand;

			int Pitch;
			int Yaw;
			int AtTarget;

			struct
			{
				int Type;
			}Fakelag;
		/*	bool Breaker;
			bool Freestanding;*/
		}Antiaim;
	}Ragebot;

	//struct
	//{
	//	int x, y, x2, y2;
	//}test;

	struct
	{
		struct
		{
			bool Box;
			bool Health;
			bool Name;
			bool Weapon;
			bool Armor;
			bool SnapLines;
			bool Skeleton;
			bool Hostage;
			bool LastHittedhitbox;
			bool Glow;
			bool LocalGlow;
			bool PulseLocalGlow;
		}Player;

		bool DroppedWeapons;
		bool C4;

		struct
		{
			int ThirdPersonKey, TpAngs;
			bool Radar;
			bool Crosshair;
			bool Hitmarker;
			bool RecoilCrosshair;
			bool FovArrows_Enable;
			bool Nightmode;
			bool BulletTracers;
			int BulletTracerType;
			bool SpectatorList;
		}Other;

		struct
		{
			bool NoFlash;
			bool NoSmoke;
			bool NoScope;
			bool NoZoom;
			bool NoPostProcessing;
			bool NoVisRecoil;
		}Effects;

		struct
		{
			int Material;
			bool Visible;
			bool Enable;
			bool Weapons;
			bool Enemy;
			bool Team;
			bool Local;
			bool Wireframe_Hand;
			float Wireframe_Hand_Color[4] = { 1.f, 1.f, 1.f, 1.f };
		}Chams;

		struct
		{
			float Box[4]= { 1.f, 1.f, 1.f, 1.f };
			float Glow[4]= { 1.f, 0.f, 1.f, 0.7f };
			float LocalGlow[4]= { 1.f, 0.f, 1.f, 0.7f };
			float SnapLines[4]= { 1.f, 1.f, 1.f, 1.f };
			float Skeleton[4]= { 1.f, 1.f, 1.f, 1.f };
			float BulletTracers[4] = { 1.f, 1.f, 1.f, 1.f };
			float FovArrows[4] = { 1.f, 1.f, 1.f, 1.f };
			struct
			{
				float Visible[4]= { 1.f, 1.f, 1.f, 1.f };
				float XQZ[4]= { 1.f, 1.f, 1.f, 1.f };
				float Weapons[4]= { 1.f, 1.f, 1.f, 1.f };
				float WeaponsXQZ[4]= { 1.f, 1.f, 1.f, 1.f };
				float Team[4]= { 1.f, 1.f, 1.f, 1.f };
				float TeamXQZ[4]= { 1.f, 1.f, 1.f, 1.f };
				float Local[4] = { 1.f, 1.f, 1.f, 1.f };
				float LocalXQZ[4] = { 1.f, 1.f, 1.f, 1.f };
			}Chams;
		}Colors;
	}Visuals;

	struct
	{
		bool	Playerlist;
		bool	Resolveall;
		struct
		{
			//int YAngle[64];
			int PAngle[64];
			int YAngle[64];
			bool MiscFixes;
		}AAA;

	}Players;

	struct
	{
		bool AntiUT = true;
		bool MM_Mode = false;
		bool asdasad = false;
		bool Knifebot;
		bool Zeusbot;
		bool Bunnyhop;
		float FOV = 90;

		int Config;
		char configname[128];

		struct
		{
			bool Enable;
			int Type;
			bool ZStrafe;
			int ZStrafeKey;
			bool CircleStrafe;
			int CircleStrafeKey;
			bool DuckInAir;
		}Strafers;
	}Misc;

	struct
	{
		bool IsEdging;
		bool ShouldBaim;
	} pLocal;

	struct
	{
		int CurMemeShots;
	} Globals;

	struct
	{
		bool Opened, Init;
	}Menu;
};

extern Variables Vars;