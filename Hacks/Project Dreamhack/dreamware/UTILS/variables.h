#pragma once
#include "../ImGui/imgui.h"
#include <set>

/*
global variables and other stuff that is probably too small to put in another 
file (and i'm too lazy to make files for all of them) would go in here.
Stuff like fonts and shit
*/


enum multibox
{
	Headd = 0,
	Chest = 1,
	Stomach = 2,
	Arms = 3,
	Legs = 4,
};


struct skinInfo
{
	int seed = -1;
	int paintkit;
	std::string tagName;
};
namespace GLOBAL
{
	extern HWND csgo_hwnd;

	extern bool should_send_packet;
	extern bool is_fakewalking;
	extern int choke_amount;
	extern bool b_IsThirdPerson;
	extern bool	NormalSpeed;

	extern Vector fake_pos;
	extern Vector real_angles;
	extern Vector fake_angles;
	extern Vector strafe_angle;
	extern float cheat_start_time;
	extern bool can_shoot_someone;
	extern int randomnumber;
	extern float flHurtTime;
	extern bool DisableAA;
	extern 	bool disable_fake_lag;
	extern bool Aimbotting;
	extern 	bool do_max_fake_lag;
	extern 	bool should_choke_packets;
	using msg_t = void(__cdecl*)(const char*, ...);
	extern msg_t		Msg;
	extern std::string	old_tag;
	extern Vector FakePosition;
	extern int ground_tickz;
	extern bool CircleStraferActive;
	extern SDK::CUserCmd originalCMD;
	extern ImFont* Text;
	extern Vector angles;

	extern std::unordered_map<std::string, std::set<std::string>> weaponSkins;
	extern std::unordered_map<std::string, skinInfo> skinMap;
	extern std::unordered_map<std::string, std::string> skinNames;
}
namespace FONTS
{
	extern unsigned int visuals_gun_icon;
	extern unsigned int menu_tab_font;
	extern unsigned int menu_checkbox_font;
	extern unsigned int menu_slider_font;
	extern unsigned int menu_groupbox_font;
	extern unsigned int menu_combobox_font;
	extern unsigned int menu_window_font;
	extern unsigned int numpad_menu_font;
	extern unsigned int visuals_esp_font1;
	extern unsigned int visuals_esp_font;
	extern unsigned int visuals_xhair_font;
	extern unsigned int visuals_side_font;
	extern unsigned int visuals_name_font;
	extern unsigned int visuals_lby_font;
	extern unsigned int visuals_grenade_pred_font;
	extern 	unsigned int menulogofont;

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
	
		int Knife;
		bool skinenabled;
		int knifecustom;

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

		struct wskin
		{
			int wId;
			int paintkit;
		};
		/*struct stickers
		{
			int Stikers1;
			int Stikers2;
			int Stikers3;
			int Stikers4;
		}allin1skins[530];*/
		struct Inventarchanger
		{
		public:
			bool enable;
			bool medalenabled;
			bool musickitenabled;

			wskin weaponskins;

			std::vector<wskin> g_skins = {
				{ 508,622 },
			{ 5027,10007 },
			{ 1,527 },
			{ 2,544 },
			{ 3,387 },
			{ 4,586 },
			{ 32,338 },
			{ 36,404 },
			{ 61,653 },
			{ 63,270 },
			{ 30,652 },
			{ 7,656 },
			{ 8,305 },
			{ 10,260 },
			{ 13,629 },
			{ 14,401 },
			{ 16,632 },
			{ 60,383 },
			{ 17,433 },
			{ 19,283 },
			{ 24,688 },
			{ 25,616 },
			{ 26,306 },
			{ 27,198 },
			{ 28,483 },
			{ 29,434 },
			{ 33,481 },
			{ 34,262 },
			{ 35,450 },
			{ 39,311 },
			{ 38,232 },
			{ 11,628 },
			{ 9,344 },
			{ 40,538 }
			};

			int medalidequip;
			int medalid;
			std::vector<uint32_t> packets_medals = { 1372, 958, 957, 956, 955 };





		}Inventarchanger;

		int gloves;
		int skingloves;
		float glovewear;
		bool glovesenabled;
		bool rankchanger;
		int KnifeSkin;
		int rank_id = 9;
		int wins = 100;
		int level = 40;
		int friendly = 999;
		int teaching = 999;
		int leader = 999;

		class RankChanger
		{
		public:
			bool enabled;
			bool semiinventar;
			int cmd_friendly;
			int cmd_teaching;
			int cmd_leader;
			int rank_id;
			int wins;
			int player_level;
			int player_level_xp;
			int ban = 0;
			int time = 0;
		}RankChanger;

		int AK47Skin;
		int GalilSkin;
		int M4A1SSkin;
		int M4A4Skin;
		int AUGSkin;
		int FAMASSkin;
		int AWPSkin;
		int SSG08Skin;
		bool fixshit;
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
		bool bBlockBot;
		int iBlockBotKey;
		bool bAutoJump;
		bool bAutoStrafe;
		int  iAutoStrafeMode;
		bool bZStrafe;
		int  iZStrafeKey;
		int edgejumpkey = 0;
		bool strafeedgejump;
		bool Slowwalkenable = false;
		bool namespam;
		bool infinityduck;
		bool chatspam;
		bool antiafk;
		bool zbhob;
		int zbhop_key;
		int Slowwalkkey = 0;
		float Slowwalkspeed;
		bool bhop_bool;
		bool AutoJump;
		bool AutoStrafe;
		int FakeLag;
		bool laggs;
		bool wasd;
		bool strafe_bool;
		bool esp_bool;
		int chams_type;
		bool playerchams;
		bool playerbehindwall;
		int xhair_type;
		bool tp_bool;
		bool removesky;
		bool CBasePlayerAnimState;
		bool override_keybind;
		bool aim_bool;
		bool silentaim;
		bool IgnoreLimbs;
		bool ignoreheads;
		bool automaticscope;
		int aim_type;
		bool Clantag;
		bool aa_bool;
		float desynoth;
		bool Logdamage;
		bool logweaponpurchase;
		bool Antiuntrusted = true;
		int standpitch;
		int yawbaseangle;
		float yawadd;
		int yawangle;
		float jitterrange;
		int manualright;
		int manualback;
		int manualleft;
		bool enablefreestanding;
		bool extrapolation;
		bool resolvedesync;
		int freestand_type;
		float frestandingjitter;
		bool fpsping;
		CColor fpsping1;
		int  iAirStuck;
		int  baimafterx;
		int  Accuracyboost;
		int  bodyiaimtype;
		bool baimifslow;
		bool baimiflethal;
		bool baimifmove;
		int  baimonkey;
		int  iAirStuckKey;
		int visual_spread;
		CColor lvmodel_col;
		bool sky_enabled;
		CColor skycolor;
		CColor night_col;
		bool fakelatency_enabled = false;
		float fakelatency_amount = 0.f;
		int aa_pitch;
		int aa_type;
		bool wolrd_enabled;
		int acc_type;
		bool bt_vis_chams_enabled;
		bool up_bool;
		bool misc_bool;
		bool HandleGameEvent;
		int config_sel;
		bool beam_bool;
		int bullet_type;
		bool gradientbullet;
		float flTracersDuration;
		float flTracersWidth;
		bool stop_bool;
		int resolvermode;
		bool fakelag_adaptive_enabled = false;
		bool fakelag_on_peek = false;
		float fakelag_moving = 1.f;
		float fakelag_jumping = 1.f;
		bool vecvelocityprediction;
		bool removerecoil;
		bool fakelagpredicion;
		bool auto_revolver_enabled;
		int weap_bool;
		bool astro;
		bool hsinair;
		bool onlyhs;
		bool bomb;
		bool bomb_beep;
		bool movement_bool;
		bool ragdoll_bool;
		float transparency_when_scoped;
		bool misc_no_sleeves;
		bool asus_props = true;
		float asus_props_o = 3.f;
		float Knifwear = 10;
		int bAimattarget;
		bool targethitbox[4];

		int aimfov = 0;

		int Menu_theme;
		int ResolverEnable;
		bool autozeus;
		bool xhair_bool;
		bool aa_lines;
		int hsinair2;
		bool gradient;
		float gradientspeed;
		bool dmg_bool;
		CColor dmg_color;
		bool fullbright;

		bool stickers;
		bool customchams;
		int number;

		bool cs_win_panel_round;
		bool achievement_earned;
		bool secrettab;
		bool sky_enable;
		CColor spread_coll2;
		CColor backtrack_col;
		bool spread_circle_enabled;
		int type_spread_circle;
		CColor sky_col;
		int night_bool;
		bool buybot_enabled = false;
		bool trashtalk = false;
		bool trashtalk2 = false;
		bool buybot_zeus = false;
		bool buybot_grenade = false;
		int buybot_pistol = 0;
		int buybot_rifle = 0;
		int buybot_armor = 0;



		bool box_bool;
		bool name_bool;
		CColor spread_Col;
		bool impacts;
		int AK47;
		int AWP;
		int SSG08;
		int SCAR20;
		int G3SG1;
		int M4A1;
		int M4A1S;
		int USP;
		int P2000;
		int MAC10;
		int MP9;
		int UMP45;
		int P90;
		int BIZON;
		int AUG;
		int SG553;
		int MP7;
		int P250;
		int Glock18c;
		int DUALB;
		int FAMAS;
		int Galil;
		int Deagle;
		int FiveSeven;
		int Tec9;
		int CZ75;
		bool skinenable;

		//SKINCHANGER NEW

		bool skinchanger_enable;
		int skinchanger_knifemodel = 0;
		int skinchanger_knifeskin = 0;
		float Checkmark[4] = { 0.3f,0.3f,0.9f,0.7f };
		float Sliderr[4] = { 0.0f,1.0f,1.0f,1.0f };

		bool removeflash;
		bool fpsboost;
		bool no_vis_recoil;
		bool health_bool;
		bool info_bool;
		int weaponstype;
		bool back_bool;
		bool lag_bool;
		float alphatrans;
		bool autozeus_bool;
		bool autoknife_bool;
		bool autorevolver_bool;
		bool fakewalk;
		int box_type;
		bool reverse_bool;
		bool multi_bool;


		float arms_val;
		float legs_val;
		float foot_val;
		float auto_chance_val;
		float scout_chance_val;
		float pistols_chance_val;
		float revolver_chance_val;
		float other_chance_val;
		float auto_damage_val;
		float scout_damage_val;
		float pistols_damage_val;
		float revolver_damage_val;
		bool bodyair;
		bool bodyawp;
		bool bodyfake;
		float other_damage_val;
		bool backtrack;
		bool Changepitchonland;
		bool accuracystanding;
		bool controlrecoil;
		bool mindamagevall;
		bool hitchancevall;
		bool NoSpread;
		bool fakefix_bool;
		bool angle_bool;
		bool tp_angle_bool;
		bool glow_bool;
		bool dist_bool;
		bool fov_bool;
		bool grennades;
		bool smoke_bool;
		bool scope_bool;
		bool predict_bool;
		bool fake_bool;
		int media_type;
		bool novis_bool;
		bool localglow_bool;
		bool duck_bool;
		bool money_bool;
		bool rlogs;
		int delay_shot;
		int resolver_type;
		int lag_type;
		bool cham_bool;
		bool resolve_bool;
		bool resolver2;
		bool ammo_bool;
		bool Hitmarker;
		bool spread_bool;
		CColor awcolor;
		bool hitmarker;
		bool prediction;
		bool lby_timer;
		bool lby_bar1;
		bool drawdistance;

		float stand_lag;
		float move_lag;
		bool hitmarker_screen;
		float jump_lag;
		int Fakelagamount;
		bool addabtivelag;
		bool breaklagcomp;
		bool vis_sound;
		float vis_soundcolor[4] = { 0.0f,1.0f,1.0f,1.0f };
		float snaplines_color[4] = { 1.0f,0.0f,0.0f,0.3f };
		float skeletoncolor[4] = { 1.0f,0.0f,0.0f,0.3f };
		float lag_amount;
		bool lagcompensatedhitboxes_bool;
		bool EspTeam = false;
		bool snaplines;
		bool debug_bool;

		bool opencolor;
		bool opencfg;
		bool connectinventory;


		struct
		{
			struct
			{
				int pitch;
				int yaw;
				int fakeyaw;
			}stand;
			struct
			{
				int pitch;
				int yaw;
				int fakeyaw;
			}move;
			struct
			{
				int pitch;
				int yaw;
				int fakeyaw;
			}air;

			float spinspeed;
			float jitterrange;
			struct
			{
				float delta;
				bool enable;
				int type;
				int flipkey;
			}breaklby;
			struct
			{
				float range;
				float speed;
			}switch_aa;
		}antiaim;
		
	
		float aa_arrows_height;
		float aa_arrows_width;
		float vmodel_col[4] = { 1.0f,1.0f,1.0f,1.0f };
		float imodel_col[4] = { 1.0f,1.0f,1.0f,1.0f };
		float localchams_col[4] = { 1.0f,1.0f,1.0f,1.0f };

		float box_col[4] = { 0.3f,0.3f,0.9f,0.7f };
		float name_col[4] = { 0.3f,0.3f,0.9f,0.7f };
		float weapon_col[4] = { 0.3f,0.3f,0.9f,0.7f };
		float grenadepredline_col[4] = { 1.0f,1.0f,1.0f,1.0f };
		float bulletenemy_col[4] = { 1.0f,1.0f,1.0f,1.0f };
		float bulletteam_col[4] = { 1.0f,1.0f,1.0f,1.0f };

		bool teammodel;
		bool teambehindwall;

		//CColor name_col;
		//CColor weapon_col;
		CColor distance_col;
		CColor grenadepredbox_col;

		CColor bulletlocal_col;

		CColor menu_col = LIGHTGREY;
		CColor title_bar = CColor(132, 0, 218);

		CColor menu_text = CColor(255, 255, 255, 255);

		CColor groupbox_body = PURPLE;
		CColor groupbox_outline = BLACK;

		CColor scrollbar_body = BLACK;

		CColor separator_text_color = GREY;

		CColor checkbox_not_clicked_col = GREY;
		CColor checkbox_col = PURPLE;
		CColor button_color = PURPLE;
		CColor combobox_selected = PURPLE;
		CColor combobox_item = PURPLE;
		CColor slider_col = GREY;
		CColor slider_color_value = PURPLE;
		CColor tab_col = WHITE;
		CColor tab_body_col = WHITE;
		float glow_col[4] = { 0.5f,0.5f,1.0f,1.0f };
		float glowlocal_col[4] = { 0.3f,0.3f,0.9f,0.7f };
		float gloweapon[4] = { 0.3f,0.3f,0.9f,0.7f };
		float lagcompcolor[4] = { 0.3f,0.3f,0.9f,0.7f };
		float fov_col[4] = { 0.3f,0.3f,0.9f,1.0f };
		float arrowradius;
		float arrowsizesh;
		CColor lbytimer_color;
		struct
		{
			bool Enabled;
			bool OnlyEnemy;
			float Range;
			bool Nicks;
		} Radargui;
		bool spectators;
		bool recoilindic;
		float recoilindicolor[4] = { 0.3f,0.3f,0.9f,1.0f };

		//---------------bameware+ barryhook pasted staff-------------------// working features but alot of staff fucked up so removed.....
		bool desyncstand;

		bool baimifunplaged; // still working on it wait...... :')
		float baimafterhp; // same xD
		bool baimifstand;
		bool correction;
		bool trailers;
		bool aw_crosshair;
		bool auto_knife;
		bool entity_skelet;
		bool team_skelet;
		bool backtrackchamsenemy;
		bool teamchams;
		CColor backtrackcolor;
		CColor backtrackcolor2;
		CColor skeletcolor;
		float backtrackcolors;
		bool backtrackskelet;
		float backtrackskeletcolor;
		//-----------------------------------------// working features but alot of staff fucked up so removed.....

		float chance_val;
		float damage_val;
		float delta_val;
		float point_val;
		float body_val;
		float awall_damage_val;

		bool misc_clantag;
		bool BacktrackChams;
		bool localesp;
		bool awpoverride;
		bool localchams;
		CColor localchams_col_lag;
		int clantags;
		bool flipup;
		bool localchams_fakelag;
		bool fakechams;
		CColor fakechamscol;
		float fov_val = 90;
		int viewfov_val = 75;

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

		int thirdperson_int;
		int thirdpersonkey;
		int Fakecrouchkey;
		int flip_int;
		bool fakecrouch;
		/* viewmodel*/
		int viewmodel = 105;
		int viewmodel_x = 10;
		int viewmodel_y = 2;
		int viewmodel_z = 4;
		/*       */
		int TickType;
		bool FakeLatency;
		bool dreamhackmarkers;
		float FakeLatencyAmount;
		bool glowenable;
		int glowstyle;
		bool glowlocal;
		bool weapongglows;
		int glowstylelocal;
		bool hitmarker_val;
		bool friendfire;
		int aa_mode;

		int aa_real_type;
		int aa_real1_type;
		int aa_real2_type;

		int aa_fake_type;
		int aa_fake1_type;
		int aa_fake2_type;

		int aa_pitch_type;
		int aa_pitch1_type;
		int aa_pitch2_type;
		float custompitch;

		float aa_realadditive_val;
		float aa_fakeadditive_val;

		float aa_realadditive1_val;
		float aa_fakeadditive1_val;
		float delta1_val;

		float aa_realadditive2_val;
		float aa_fakeadditive2_val;
		float delta2_val;

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
		bool showhistorychams;
		float historychamscolor[4] = { 1.f, 1.f, 1.f, 1.f };
		float dmg_ccolor[4] = { 1.f, 1.f, 1.f, 1.f };

		int espteamselection;
		int espteamcolourselection;
		bool boxteam;
		bool nameteam;
		bool weaponteam;
		bool flagsteam;
		bool healthteam;
		bool moneyteam;
		bool ammoteam;
		bool bones;
		bool arrowteam;
		CColor boxteam_col;
		CColor nameteam_col;
		CColor weaponteam_col;
		CColor arrowteam_col;

		float teamvis_color[4] = { 1.0f,0.5f,0.5f,0.5f };
		float teaminvis_color[4] = { 1.0f,1.0f,0.5f,0.5f };
		float teamglow_color[4] = { 1.0f,1.0f,0.5f,0.5f };

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
		bool nospread;
		bool overrideenable;
		bool autoindicator;
		CColor autocolor;
		bool lbyenable;
		int circlestrafekey;
		int cyrclstrafer;
		float Retrack = 0.f;
		bool secretwindow;
		float circlstraferetract;
		float fakewalkspeed;
		float daytimevalue = 50;
		bool norecoil;
		float circlemin;
		bool forcehair;
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
		bool logs;

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
extern bool hurtcalled;
extern bool menu_hide;

extern int oldest_tick[65];
extern float compensate[65][12];
extern Vector backtrack_hitbox[65][20][12];
extern float backtrack_simtime[65][12];

extern bool side1;

extern bool side2;

extern bool back;