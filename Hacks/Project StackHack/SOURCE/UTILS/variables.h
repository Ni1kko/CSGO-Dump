#pragma once


/*
global variables and other stuff that is probably too small to put in another 
file (and i'm too lazy to make files for all of them) would go in here.
Stuff like fonts and shit
*/

namespace GLOBAL
{
	extern HWND csgo_hwnd;

	extern bool should_send_packet;
	extern bool round_change;
	extern bool desyncing[65];
	extern bool vel_head_check;
	extern bool is_fakewalking;
	extern bool is_shooting;
	extern int choke_amount;
	extern bool can_shoot_someone;
	extern bool disable_fakelag;
	extern void* last_cmd;
	extern bool is_slowwalking;

	extern Vector real_angles;
	extern bool jitter;
	extern Vector fake_angles;
	extern Vector strafe_angle;
	extern bool found_angle;

	extern int randomnumber;
	extern float flHurtTime;
	extern bool DisableAA;
	extern bool Aimbotting;

	using msg_t = void(__cdecl*)(const char*, ...);
	extern msg_t		Msg;

	extern Vector FakePosition;
	extern int ground_tickz;
	extern bool CircleStraferActive;
	extern SDK::CUserCmd originalCMD;
	//extern SDK::IGameEvent pEvent;
}
namespace FONTS
{
	extern unsigned int menu_tab_font;
	extern unsigned int menu_checkbox_font;
	extern unsigned int menu_slider_font;
	extern unsigned int menu_groupbox_font;
	extern unsigned int menu_combobox_font;
	extern unsigned int menu_window_font;
	extern unsigned int numpad_menu_font;
	extern unsigned int visuals_esp_font;
	extern unsigned int visuals_xhair_font;
	extern unsigned int visuals_side_font;
	extern unsigned int visuals_name_font;
	extern unsigned int visuals_lby_font;
	extern unsigned int visuals_grenade_pred_font;
	extern unsigned int  in_game_logging_font;
	bool ShouldReloadFonts();
	void InitFonts();
}
namespace SETTINGS
{
	class Sticker_t
	{
	public:
		int   iID = 0;
		float flWear = 0.f;
		float flScale = 1.f;
		int   iRotation = 0;
	};
	class Weapon_tTT
	{
	public:
		int SkinsWeapon;
		int SkinsKnife;
		int VremennyiWeapon;
		int VremennyiKnife;
		int Stikers1;
		int Stikers2;
		int Stikers3;
		int Stikers4;
		float ChangerWear = 0;
		int ChangerStatTrak = 0;
		char ChangerName[32] = "";
		bool ChangerEnabled;
	};

	class CSettings
	{
	public:
		// returns true/false whether the function succeeds, usually returns false if file doesn't exist
		bool Save(std::string file_name);
		bool Load(std::string file_name);

		bool Remove(std::string file_name);

		void CreateConfig(std::string name); // creates a blank config

		std::vector<std::string> GetConfigs();

		bool skinenabled;
		int Knife;

		Weapon_tTT weapons[520];

		struct wpnz
		{
			bool  EnabledStickers;
			bool  EnabledSkins;
			int   ChangerSkin;
			char  ChangerName[32] = "";
			int   ChangerStatTrak;
			int   ChangerSeed;
			float ChangerWear;
			Sticker_t Stickers[5];
		}Weapons[519];

		bool fix;
		bool backaimhead;
		int rage_lagcompensation_type=0;
		bool rage_lagcompensation;

		bool CUSTOMMODEL;
		int customct;
		int customtt;

		int gloves;
		int max_fov;
		int skingloves;
		float glovewear;
		bool glovesenabled;
		bool impacts;
		bool nightmode;
		bool rankchanger;
		int KnifeSkin;
		int rank_id = 9;
		int wins = 100;
		int level = 40;
		int friendly = 999;
		int teaching = 999;
		int leader = 999;
		int AK47Skin;
		int GalilSkin;
		int M4A1SSkin;
		int M4A4Skin;
		int AUGSkin;
		int FAMASSkin;
		int AWPSkin;
		int SSG08Skin;
		bool fakelatency_enabled;
		bool fixshit;
		float fakelatency_amount;
		float stand_velocity_threshold;
		int SCAR20Skin;
		int P90Skin;
		int Mp7Skin;
		int NovaSkin;
		int UMP45Skin;
		int GlockSkin;
		int SawedSkin;
		int USPSkin;
		int MagSkin;
		int XmSkin;
		int DeagleSkin;
		int DualSkin;
		int FiveSkin;
		int RevolverSkin;
		int Mac10Skin;
		int tec9Skin;
		int Cz75Skin;
		int NegevSkin;
		int M249Skin;
		int Mp9Skin;
		int P2000Skin;
		int BizonSkin;
		int Sg553Skin;
		int P250Skin;
		int G3sg1Skin;


		struct Visualsxd
		{
			bool Enabled = false;
			bool Visuals_BoxEnabled = false;
			int Visuals_BoxType = 1;
			int Visuals_HealthBarType = 1;
			bool Visuals_Scoped = false;
			bool Visuals_AmmoESP = false;
			int Visuals_AmmoESPType = 1;

			bool Visuals_EspTeam = false;
			bool Visuals_VisableOnly = false;

			float BoxColorPickCTVIS[4] = { 1.0f,0.0f,0.0f,1.0f };
			float BoxColorPickCTINVIS[4] = { 1.0f,1.0f,0.0f,1.0f };


			float BoxColorPickTVIS[4] = { 0.0f,1.0f,0.0f,1.0f };
			float BoxColorPickTINVIS[4] = { 0.0f,1.0f,1.0f,1.0f };


			float LagCompHitboxes[4] = { 0.0f,1.0f,0.0f,1.0f };



			float SkeletonCTVIS[4] = { 1.0f,0.0f,0.0f,1.0f };
			float SkeletonCTINVIS[4] = { 1.0f,1.0f,0.0f,1.0f };

			float SkeletonTVIS[4] = { 0.0f,1.0f,0.0f,1.0f };
			float SkeletonTINVIS[4] = { 0.0f,1.0f,1.0f,1.0f };




			float CTChamsVisable[4] = { 1.0f,0.0f,0.0f,1.0f };

			float TChamsVisable[4] = { 1.0f,1.0f,0.0f,1.0f };


			float CTChamsInVisable[4] = { 0.0f,1.0f,0.0f,1.0f };
			float TChamsInVisable[4] = { 1.0f,1.0f,0.0f,1.0f };

			float beamtrace[4] = { 1.0f,0.0f,0.0f,1.0f };
			int thirdkey;
			int Hitsound;
			bool tp_bool;
			bool Monitor;
			bool Visuals_HealthBar = false;
			bool Visuals_Name = false;
			bool Visuals_ArmorBar = false;
			bool Visuals_Flashed = false;
			bool Visuals_bones = false;
			bool Visuals_Defuser = false;
			bool Visuals_Weapons = false;
			int Visuals_WeaponsType = 0;
			bool Visuals_AimLines = false;
			bool Visuals_Skeltal = false;
			bool Visuals_EngineRadar = false;
			bool Visuals_DrawLinesAA = false;
			bool Visuals_DroppedWeapons = false;
			bool Visuals_NoRecoil = false;
			bool Visuals_NoFlash = false;
			bool Visuals_NoSmoke = false;
			bool Visuals_NoSmokeVar = false;
			bool Visuals_NoScope = false;
			bool Visuals_NoZoom = false;
			bool Visuals_NoPostProcess = false;
			bool Visuals_logs = false;

			bool Visuals_Chams = false;
			bool Visuals_ChamsTeam = false;
			bool Visuals_ChamsXQZ = false;
			int  Visuals_ChamsMaterial = 0;
			bool Visuals_GhostAngle = false;
			bool Visuals_ChamsGuns = false;
			bool Visuals_Crosshair = false;
			bool Visuals_CrosshairDynamic = false;

			

			float CTGlowVisable[4] = { 1.f,1.0f,1.0f,1.0f };
			float TGlowVisable[4] = { 1.f,1.0f,1.0f,1.0f };
			float CTGlowInVisable[4] = { 1.f,1.0f,1.0f,1.0f };
			float TGlowInVisable[4] = { 1.f,1.0f,1.0f,1.0f };

			bool Visuals_Hitmarker = false;
			bool Visuals_Spread = false;
			bool Visuals_GrenadePrediction = false;
			bool Visuals_DamageESP = false;
			bool Visuals_C4 = false;
			bool eventlog_enabled = false;
			float SpreadColor[4] = { 1.0f,0.0f,0.0f,1.0f };

			float DamageESPColor[4] = { 1.0f,0.0f,0.0f,1.0f };
			bool Vis_Glow = false;
			bool Vis_Glow_Team = false;
			bool Vis_Glow_Vis = false;


			bool Visuals_NightMode = false;
			bool Visuals_Asus = false;

			bool Visuals_DrawBeams = false;
			float Visuals_DrawBeamsDuration = 3.f;
			float Visuals_DrawBeamsSize = 3.f;

			bool Visuals_DrawCapsules = false;
			bool Visuals_DrawEventLog = false;
			float capsulecolor[4] = { 1.0,0.0f,0.0f,1.0f };
			float Visuals_DrawCapsulesDuration = 5.f;
			int Visuals_Spread_Type = 0;
			struct
			{
				bool Enabled = false;
				float Distance = 800.f;
				int type = 0;
				float Radius = 10.f;
				bool Animated = false;
				bool visonly = false;
			}SoundESP;
			struct
			{
				struct
				{
					bool	ExternalRadar;
					int		RadarStyle;
					float	RadarDistance;
					bool	RadarVisibleOnly;
					bool	RadarSmoke;
				}Radar;
				bool Monitor = false;
			}Panels;

			bool lbyIndicator = false;
			bool strelkiIndicator = false;
		}Visuals;

		bool autostop;
		bool friendfire;
		bool bhop_bool;
		bool strafe_bool;
		bool esp_bool= true;
		int chams_type;
		int xhair_type;
		bool tp_bool;
		bool aim_bool;

		

		int aa_pitch_type;
		int aa_pitch1_type;
		int aa_pitch2_type;

		bool aimstep_bool;
		bool extrapolationbool;
		int aim_type;
		int aim_method;
		bool dynamic_multipoint_on_high_velocity;
		float bodypointscale, headpointscale;
		bool fake_duck;
		bool aa_bool;
		int aa_pitch;
		int aa_type;
		int memetype;
		bool astrowalk;
		bool noviusalrecoil;
		int acc_type;
		int hitchancetype;
		int pointvalue;
		bool up_bool;
		bool slowalkbool;
		bool hitmarker;
		bool dynamic_hitmarkers;
		bool preservekillfed;
		bool misc_bool;
		int config_sel;
		bool beam_bool;
		bool stop_bool;
		bool apply_freestand;
		bool night_bool;
		bool box_bool;
		bool skeleton_esp;
		bool name_bool;
		bool weap_bool;
		bool health_bool;
		bool info_bool;
		bool back_bool;
		bool rcsbool;
		bool baim_if_unplugged;
		bool preferbaim;
		bool baimiflethal;
		float baim_if_x_damage;
		bool legit_back;
		bool lag_bool;
		int box_type;
		bool reverse_bool;
		bool multi_bool = true;
		bool fakefix_bool;
		bool angle_bool;
		bool tp_angle_bool;
		bool glow_bool;
		bool dist_bool;
		bool fov_bool;
		bool smoke_bool;
		bool crouch_bool;
		bool no_vac_bool = false;
		bool scope_bool;
		bool predict_bool;
		bool fake_bool;
		int media_type;
		bool novis_bool;
		bool localglow_bool;
		bool duck_bool;
		bool showlbydelta;
		bool flindicate;
		int a_mode;
		bool lby_bar;
		bool lby_bar1;
		CColor lby_col;
		bool flipup;
		CColor vmodel_col;
		CColor imodel_col;
		CColor teamvis_color;
		CColor teaminvis_color;
		CColor localchams_col;
		CColor glowlocal_col;
		CColor teamglow_color;
		bool accuracystanding;
		bool money_bool;
		bool expresolverbool;
		int delay_shot;
		bool taser_range;
		int lag_type;
		bool cham_bool;
		bool resolve_bool;
		bool resolve2_bool;
		bool experimental_resolver;
		bool ammo_bool;
		int spread_bool;
		int lby_flick_mode;
		
		bool desync_resolver_bool;

		float spreadcirclecol[4] = { 1.0f,0.0f,0.0f,0.3f };
		bool trashtalk = false;
		bool trashtalk2 = false;
		int stand_lag;
		int move_lag;
		int jump_lag;

		bool debug_bool;

		CColor refixsign = CColor(255, 0, 255, 180);


	//	float vmodel_col[4] = { 1.0f,1.0f,0.0f,0.5f };

		//float imodel_col[4] = { 0.0f,1.0f,0.0f,0.5f };
		
	
		
		CColor box_col;
		CColor name_col;
		CColor weapon_col;
		CColor distance_col;

		float taserrange_col[4] = { 0.0f,0.0f,0.0f,1.0f };
		float cSound[4] = { 0.0f,0.0f,0.0f,1.0f };
		bool bSound;
		bool noFlash;
		//float localchams_col[4] = { 1.0f,0.0f,1.0f,0.5f };
		float grenadepredline_col[4] = { 1.0f,0.0f,1.0f,0.5f };



		float bulletlocal_col[4] = { 1.0f,0.0f,1.0f,1.0f };
		float bulletenemy_col[4] = { 0.0f,1.0f,1.0f,1.0f };
		float bulletteam_col[4]{ 1.0f,1.0f,1.0f,1.0f };

		float bulletlife = 3.0f;
		float bulletsize = 2.0f;
	
		CColor menu_col = CColor(60, 60, 60);
		CColor checkbox_col = CColor(5, 135, 5);
		CColor slider_col = CColor(255, 255, 255);
		CColor tab_col = CColor(91, 91, 91);
		
	//	float glow_col[4] = { 0.5f,0.5f,1.0f,1.0f };

		//float glowlocal_col[4] = { 0.3f,0.3f,0.9f,0.7f };
		
		CColor fov_col;

		bool fakeaashit;
		//=========================
		bool legit_bot_aimbot;
		float legit_bot_fov;
		float legit_bot_smooth;
		bool lbot_hitscan;
		//=========================
		int rbot_accuraccy_type;
		bool autowall_optimization;


		float FakeLatencyAmount;
		bool FakeLatency;
		int TickType;
		int  Accuracyboost;
		int ResolverMode;
		bool FpsBoosting;
		bool lagcomphitbox;
		bool anti_noob;
		bool fakelag_onpeek;

		int handStyle;

		//bool [censored]_tracers;
		bool experimental_features;

		float chance_val;
		float spread_val;
		float damage_val;
		float delta_val;
		float point_val;
		float body_val;
		bool misc_clantag;
		float headscale;
		float neckscale;
		float chestscale;
		float pelvisscale;
		float bodyscale;
		float armsscale;
		float legsscale;
		float footscale;
		float bruteshots;
		bool bspottedradar;
		bool head_scan;
		bool neck_scan;
		bool chest_scan;
		bool body_scan;
		bool pelvis_scan;
		bool arms_scan;
		bool legs_scan;
		bool multi_head;
		bool multi_neck;
		bool multi_chest;
		bool multi_body;
		bool multi_pelvis;
		bool multi_arms;
		bool multi_legs;
		int priority;
		bool hand_chams;
		bool baimsmartaccuracy;

		float glowalphavalue;
		bool awallcsh;
		bool expreso;
		bool fractionantifs;
		bool localesp;
		int localchams;
		float fov_val=90;
		float viewfov_val = 60;

		bool visual_resolver;

		bool fakechams;
		float fakechamscol[4] = { 1.0f,1.0f,1.0f,1.0f };


		bool flip_bool;
		int aa_side;

		bool legit_bool;
		int legit_key;
		bool rcs_bool;
		float legitfov_val;
		int legitbone_int;
		float rcsamount_min;
		float rcsamount_max;
		float legitaim_val;
		bool legittrigger_bool;
		int legittrigger_key;
		int forceheadaim;
		bool forceheadaimonkey;

		int forcebaim;
		int manual_aa_switch_meme;
		bool forcebaimonkey;
		bool edgejump;

		bool showplayerposonhit;
		int flip_int;

		bool glowenable;
		int glowstyle;
		bool glowlocal;
		int glowstylelocal;
		int hitmarker_val;

		int aa_mode;

		int stand_yaw;
		int stand_pitch;

		int move_yaw;
		int desync_mode;
		int move_pitch;

		int air_yaw;
		int air_pitch;

		int stand_desync;
		int move_desync;
		int air_desync;

		int body_lean;
		int jitter_width;

		int aa_real_type;
		int aa_real1_type;
		int aa_real2_type;
		float aa_realadditive_val;
		int real3;

		bool desync_aa3;

		bool anti_untrusted;

		float spinangle;
		float spinspeed;

		float spinangle1;
		float spinspeed1;

		float spinangle2;
		float spinspeed2;

		float spinanglefake;
		float spinspeedfake;

		float spinanglefake1;
		float spinspeedfake1;

		float spinanglefake2;
		float spinspeedfake2;

		bool lbyflickup;
		bool lbyflickup1;
		bool lbyflickup2;

		bool aa_fakeangchams_bool;

		int chamstype;
		float fov_time;
		bool rifk_arrow;

		int glowteamselection;
		bool glowteam;

		int chamsteamselection;
		int chamsteam;

		int espteamselection;
		int espteamcolourselection;
		bool boxteam;
		bool nameteam;
		bool weaponteam;
		bool flagsteam;
		bool healthteam;
		bool moneyteam;
		bool ammoteam;
		bool arrowteam;
		CColor boxteam_col;
		CColor nameteam_col;
		CColor weaponteam_col;
		CColor arrowteam_col;


		//float teamvis_color[4] = { 1.0f,0.5f,0.5f,0.5f };
		//float teaminvis_color[4] = { 1.0f,1.0f,0.5f,0.5f };


	
		float gradientspeed;
		bool gradientglow;


		//float teamglow_color[4] = { 1.0f,1.0f,0.5f,0.5f };
		CColor glow_col;

		bool matpostprocessenable;
		bool removescoping;
		bool fixscopesens;
		bool forcecrosshair;

		int quickstopkey;
		bool stop_flip;
		bool chamsmetallic;
		int flashlightkey;
		int overridekey;
		int autostopmethod;
		int overridemethod;
		bool overridething;
		bool sleevebool;
		bool overrideenable;
		bool lbyenable;
		int circlestrafekey;
		float circlstraferetract;
		float fakewalkspeed;
		float daytimevalue = 98;

		float circlemin;
		float circlemax;
		float circlevel;
		float circlenormalizemultiplier;

		bool skinchangerenable;

		int knifeToUse;
		int bayonetID, karambitID, flipID, gutID, m9ID, huntsmanID;
		int gloveToUse;
		int bloodhoundID, driverID, handwrapsID, motoID, specialistID, sportID, hydraID;

		int uspID, p2000ID, glockID, dualberettaID, p250ID, fivesevenID, tech9ID, r8ID, deagleID;
		int novaID, xm1014ID, mag7ID, sawedoffID, m249ID, negevID;
		int mp9ID, mac10ID, mp7ID, ump45ID, p90ID, ppbizonID;
		int famasID, galilID, ak47ID, m4a4ID, m4a1sID, ssg08ID, augID, sg553ID, awpID, scar20ID, g3sg1ID;


	private:
	}; extern CSettings settings;
}

extern bool using_fake_angles[65];
extern bool full_choke;
extern bool is_shooting;

extern bool in_tp;
extern bool fake_walk;

extern int resolve_type[65];

extern int target;
extern int shots_fired[65];
extern int shots_hit[65];
extern int shots_missed[65];
extern int backtrack_missed[65];

extern bool didShot;
extern bool didMiss;

extern float tick_to_back[65];
extern float lby_to_back[65];
extern bool backtrack_tick[65];

extern float lby_delta;
extern float update_time[65];
extern float walking_time[65];

extern float local_update;

extern int hitmarker_time;
extern int random_number;

extern bool menu_hide;
extern bool hurtcalled;

extern int oldest_tick[65];
extern float compensate[65][12];
extern Vector backtrack_hitbox[65][20][12];
extern float backtrack_simtime[65][12];