#pragma once

extern void RenderInterface();

struct Variables
{
	Variables()
	{

	}

	struct Ragebot_s
	{
		bool MainSwitch;
		bool 	Enabled;
		bool 	AutoFire;
		float 	FOV;
		bool 	Silent;
		bool AutoPistol;
		int KeyPress;
		bool AimStep;
		

			bool	EnabledAntiAim;
			int		Pitch;
			int		YawTrue;
			int		YawFake;
			bool	AtTarget;
			bool	Edge;
			bool KnifeAA;


			bool FriendlyFire;
			int		Hitbox;
			int		Hitscan;
			float Pointscale;
			bool Multipoint;
			float Multipoints;


			bool AntiRecoil;
			bool AutoWall;
			bool AutoStop;
			bool AutoCrouch;
			bool AutoScope;
			float MinimumDamage;
			bool Hitchance;
			float HitchanceAmount;
			bool Resolver;
			bool FakeLagFix;
			bool playerlist;
            int BAIMkey;
	} Ragebot;

	struct
	{
		bool MainSwitch;
        bool backtrack;
		struct
		{
			bool 	Enabled;
			bool AutoPistol;
			bool	Resolver;
            
		} Aimbot;


			int MainKey = 1;
            float MainSmooth;
			float Mainfov;
            float main_random_smooth;
            float main_recoil_min;
            float main_recoil_max;
            float main_randomized_angle;



			int PistolKey = 6;
			float Pistolfov;
			float PistolSmooth;
            float pistol_random_smooth;
            float pistol_recoil_min;
            float pistol_recoil_max;
            float pistol_randomized_angle;



			int SniperKey = 6;
			float Sniperfov;
			float SniperSmooth;
            float sniper_random_smooth;
            float sniper_recoil_min;
            float sniper_recoil_max;
            float sniper_randomized_angle;


		struct
		{
			bool	Enabled;
			float Delay;
			int Key = 6;
			float hitchance;
			struct
			{
				bool Head;
				bool Arms;
				bool Chest;
				bool Stomach;
				bool Legs;
			} Filter;

		} Triggerbot;

	} Legitbot;

	struct
	{
		bool 	Enabled;

		bool Box;
		bool Name;
		bool HP;
		bool Weapon;
		bool Info;
		bool Chams;
		bool Skeleton;
		bool AimSpot;
		bool DLight;
		bool SpreadCrosshair;
		bool GrenadeESP;
		bool C4;

		bool Glow;

		bool NoVisualRecoil;
		int Hands;
		int Weapons;
		float FOVChanger;
		float viewmodelChanger;
		bool NoFlash;
		bool NoSmoke;
		bool ThirdPerson;
		bool Time;

		bool money;

		bool C4World;
		bool WeaponsWorld;
		bool noscopeborder;
		bool GrenadePrediction;

		struct
		{
			bool Players;
			bool EnemyOnly;
		} Filter;

		struct
		{
			bool Players;
			bool EnemyOnly;
			bool Weapons;
		} GlowFilter;

	} Visuals;

	struct
	{
        bool silentstealer;
        int ragequit;
		bool 	Bhop;
		bool 	AutoStrafe;
		float MinVel;
		int		AirStuckKey;
		int lagexploit;
		int lagexploitmultiplier = 3;
		float lagexploitvalue;
		float FakeLag;
		bool AdaptiveFakeLag;
        bool nightMode;
		int NameSpammer;
		bool NameChangerFix;
		bool NoName;
		int		ChatSpamMode;
		bool ClantagChanger;
		int ClanTagSpeed;
        bool syncclantag;
		bool SpecList;
		bool FPSBooster;
		int SkyBoxChanger;
        bool namespam;
        int spammer;
		int AutoDefuse;
		bool Spam;
		bool isRecording;
		bool isReplaying;
		bool RecordPath;
		bool AutoAccept;
		bool SpoofConfirmation = false;
		bool animatedclantag = false;
		int customtab;

        bool niggatest;


	} Misc;
	struct
	{
		bool Enabled;
		int Knife;
        int gloves;
		int KnifeSkin;
		int AK47Skin;
		int M4A1SSkin;
		int M4A4Skin;
		int AUGSkin;
		int FAMASSkin;
		int AWPSkin;
		int SSG08Skin;
		int SCAR20Skin;
		int P90Skin;
		int UMP45Skin;
		int GlockSkin;
		int USPSkin;
		int DeagleSkin;
		int tec9Skin;
		int P2000Skin;
		int P250Skin;
	} Skinchanger;

	struct
	{
		bool	Opened = false;
		int 	Key;
		bool	Ragebot = false;
		bool	Legitbot = false;
		bool	Visual = false;
		bool	Misc = false;
		int		ConfigFile = 0;
		int		Theme = 0;
		bool	Colors = false;
	} Menu;
};

extern Variables g_Options;