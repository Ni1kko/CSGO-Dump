#pragma once
#include "GUI.h"
#include "Controls.h"
class CRageBotTab : public CTab
{
public:
	void Setup();
	CLabel ActiveLabel;
	CCheckBox Active;
	CComboBoxYeti psilent;
	CGroupBox AimbotGroup;
	CCheckBox AimbotEnable;
	CCheckBox lag_pred;
	CCheckBox delay_shot;
	CCheckBox AimbotAutoFire;
	CSlider  AimbotFov;
	CCheckBox AimbotSilentAim;

	CComboBoxYeti AimbotResolver;

	//-------------------------
	//--------------------------
	CCheckBox LowerbodyFix;
	CCheckBox PreferBodyAim;
	CCheckBox AWPAtBody;
	CSlider BaimIfUnderXHealth;
	CSlider XShots;
	CCheckBox EnginePrediction;
	CCheckBox AimbotAimStep;
	CCheckBox pointscaleyes;
	CCheckBox Multienable;
	CSlider pointscaleval;
	CSlider Multival;
	CCheckBox nospread;
	CSlider AimbotAimStep2;
	CSlider shotlimit;
	CCheckBox AimbotKeyPress;

	CCheckBox baim_fake;
	CCheckBox baim_inair;
	CCheckBox baim_fakewalk;
	CCheckBox baim_lethal;
	CCheckBox baim_muslim;

	CKeyBind  AimbotKeyBind;
	CKeyBind  AimbotStopKey;
	CGroupBox TargetGroup;
	CComboBoxYeti TargetSelection;
	CCheckBox TargetFriendlyFire;
	CComboBoxYeti TargetHitbox;
	CComboBoxYeti TargetHitscan;
	CCheckBox TargetMultipoint;
	CSlider   TargetPointscale;
	CSlider   bruteX;
	CSlider   baim;

	//--------------//
	CGroupBox weapongroup;

	CSlider hc_auto;
	CSlider hc_scout;
	CSlider hc_awp;
	CSlider hc_pistol;
	CSlider hc_otr;

	CSlider md_auto;
	CSlider md_scout;
	CSlider md_awp;
	CSlider md_pistol;
	CSlider md_otr;

	// CComboBoxYeti target_auto;
	CDropBox target_auto;
	CDropBox target_scout;
	CDropBox target_awp;
	CDropBox target_pistol;
	CDropBox target_otr;

	CCheckBox preset_auto;
	CCheckBox preset_scout;
	CCheckBox preset_awp;
	CCheckBox preset_pistol;
	CCheckBox preset_otr;

	CCheckBox headonly_if_vis_auto;
	CCheckBox headonly_if_vis_scout;
	CCheckBox headonly_if_vis_awp;
	CCheckBox headonly_if_vis_pistol;
	CCheckBox headonly_if_vis_otr;
	//--------------//

	CGroupBox AccuracyGroup;
	CCheckBox AccuracyRecoil;

	CCheckBox AccuracyAutoWall;
	CSlider AccuracyMinimumDamage;
	CCheckBox AccuracyAutoStop;
	CSlider Multival2;

	CCheckBox AccuracyAutoCrouch;
	CCheckBox AccuracyAutoScope;
	CSlider   AccuracyHitchance;
	CComboBoxYeti resolver;

	CComboBoxYeti preso;

	CCheckBox Override;

	CComboBoxYeti OverrideMode;
	CKeyBind bigbaim;
	CKeyBind flip180;
	CCheckBox extrapolation;

};
class CLegitBotTab : public CTab
{
public:
	void Setup();
	// Master Switch
	CLabel ActiveLabel;
	CCheckBox Active;
	CGroupBox AimbotGroup;
	CCheckBox AimbotEnable;
	CComboBoxYeti aimbotfiremode;
	CCheckBox AimbotFriendlyFire;
	CCheckBox AimbotKeyPress;
	CKeyBind  AimbotKeyBind;
	CCheckBox AimbotSmokeCheck;
	CCheckBox BackTrack;
	CCheckBox legitresolver;
	CGroupBox TriggerGroup;
	CComboBoxYeti triggertype;
	CCheckBox TriggerEnable;
	CKeyBind  TriggerKeyBind;
	CSlider   TriggerDelay;
	CSlider   TriggerBurst;
	CSlider   TriggerBreak;
	CSlider TriggerRecoil; CSlider   TriggerHitChanceAmmount;
	CGroupBox TriggerFilterGroup;
	CCheckBox TriggerHead;
	CCheckBox TriggerChest;
	CCheckBox TriggerStomach;
	CCheckBox TriggerArms;
	CCheckBox TriggerLegs;
	CCheckBox TriggerTeammates;
	CCheckBox TriggerFlashCheck;
	CCheckBox TriggerSmokeCheck;
	CCheckBox aaenable;
	CComboBoxYeti aatyp;
	CComboBoxYeti aatyp2;
	CSlider aatyp3;
	CSlider aimhp;
	CSlider aafl;
	CSlider trighp;
	CGroupBox weapongroup;
	CSlider   WeaponMainSpeed;
	CSlider   WeaponMainFoV;
	CSlider WeaponMainRecoil;
	CComboBoxYeti WeaponMainHitbox;
	CSlider WeaponMainAimtime;
	CSlider WeaoponMainStartAimtime;
	CGroupBox WeaponPistGroup;
	CSlider   WeaponPistSpeed;
	CSlider   WeaponPistFoV;
	CSlider WeaponPistRecoil;
	CComboBoxYeti WeaponPistHitbox;
	CSlider WeaponPistAimtime;
	CSlider WeaoponPistStartAimtime;
	CGroupBox WeaponSnipGroup;
	CSlider   WeaponSnipSpeed;
	CSlider   WeaponSnipFoV;
	CSlider WeaponSnipRecoil;
	CComboBoxYeti WeaponSnipHitbox;
	CSlider WeaponSnipAimtime;
	CSlider WeaoponSnipStartAimtime;
	CGroupBox WeaponMpGroup;
	CSlider   WeaponMpSpeed;
	CSlider   WeaponMpFoV;
	CSlider WeaponMpRecoil;
	CComboBoxYeti WeaponMpHitbox;
	CSlider WeaponMpAimtime;
	CSlider WeaoponMpStartAimtime;
	CGroupBox WeaponShotgunGroup;
	CSlider   WeaponShotgunSpeed;
	CSlider   WeaponShotgunFoV;
	CSlider WeaponShotgunRecoil;
	CComboBoxYeti WeaponShotgunHitbox;
	CSlider WeaponShotgunAimtime;
	CSlider WeaoponShotgunStartAimtime;
	CGroupBox WeaponMGGroup;
	CSlider   WeaponMGSpeed;
	CSlider   WeaponMGFoV;
	CSlider WeaponMGRecoil;
	CComboBoxYeti WeaponMGHitbox;
	CSlider WeaponMGAimtime;
	CSlider WeaoponMGStartAimtime;




};
class CVisualTab : public CTab
{
public:
	void Setup();
	CLabel ActiveLabel;
	CCheckBox Active;
	CCheckBox logs;
	CSlider flashAlpha;
	CGroupBox OptionsGroup;
	CCheckBox OptionsWeapone;
	CComboBoxYeti OptionsBox;
	CComboBoxYeti OptionsName;
	CCheckBox HitmarkerSound;
	CCheckBox OtherHitmarker;
	CCheckBox ChamsEnemyVis;
	CCheckBox ChamsLocal;
	CCheckBox aa_helper;
	CCheckBox ChamsEnemyNoVis;
	CComboBoxYeti helpermat;
	CCheckBox OffscreenESP;
	CComboBoxYeti chamstype;
	CColorSelector urdadyoufaggot;

	CSlider Glowz_lcl;
	CCheckBox BlendIfScoped;
	CSlider BlendValue;

	CComboBoxYeti OptionsChams;
	CComboBoxYeti OptionsArmor;
	CComboBoxYeti OptionsHealth;
	CComboBoxYeti OptionsWeapon;
	CComboBoxYeti manualaa_type;
	CComboBoxYeti HandCHAMS;
	CComboBoxYeti GunCHAMS;
	CComboBoxYeti LBYIndicator;
	CComboBoxYeti LCIndicator;
	CComboBoxYeti OtherThirdpersonAngle;
	CComboBoxYeti FakeAngleChams;
	CComboBoxYeti WeaponChams;

	CCheckBox Weapons;
	CCheckBox ChamsTeamVis;
	CCheckBox ChamsTeamNoVis;
	CCheckBox OptionsGlow_lcl;
	CCheckBox NoGlowIfScoped;
	CCheckBox NoChamsIfScoped;
	CCheckBox armorbar;
	CCheckBox OptionsInfo;
	CCheckBox BulletTrace;
	CCheckBox ChamsVisibleOnly;
	CCheckBox Ammo;
	CCheckBox GrenadePrediction;
	CCheckBox LocalPlayerESP;
	CCheckBox CanHit;
	CCheckBox OptionsHelmet;
	CCheckBox NightMode;
	CCheckBox OptionsKit;
	CCheckBox SpecList;
	CCheckBox IsScoped;
	CCheckBox CompRank;
	CCheckBox HasDefuser;
	CCheckBox GrenadeTrace;
	CCheckBox Distance;
	CCheckBox DiscoLights;
	CCheckBox Money;
	CCheckBox OptionsDefuse;
	CCheckBox OptionsDefusing;
	CCheckBox SniperCrosshair;
	CCheckBox OptionsSkeleton;
	CCheckBox NightSky;
	CCheckBox hitbone;
	CCheckBox OptionsAimSpot;
	CCheckBox OtherNoScope;
	CCheckBox OptionsCompRank;
	CCheckBox resoinfo;
	CCheckBox killfeed;
	CCheckBox override_viewmodel;
	CCheckBox selfglow;
	CGroupBox FiltersGroup;
	CCheckBox FiltersAll;
	CCheckBox show_players;
	CCheckBox show_team;
	CCheckBox show_local;
	CCheckBox FiltersWeapons;
	CKeyBind flashlight;
	CCheckBox FiltersChickens;
	CCheckBox FiltersNades;
	CCheckBox FiltersC4;
	CGroupBox ChamsGroup;
	CComboBoxYeti asus_type;
	CCheckBox ChamsThruWalls;
	CSlider  GlowZ;
	CSlider  team_glow;
	CSlider enemy_blend;
	CSlider enemy_blend_invis;
	CSlider offset_x;
	CSlider offset_y;
	CSlider offset_z;
	CCheckBox ChamsPlayers;
	CCheckBox ChamsEnemyOnly;
	CSlider blend_local;
	CGroupBox OtherGroup;
	CComboBoxYeti OtherCrosshair;
	CComboBoxYeti OtherRecoilCrosshair;
	CCheckBox AutowallCrosshair;
	CCheckBox OptionsGlow;
	CCheckBox EntityGlow;
	CCheckBox OtherEntityGlow;
	CCheckBox DisablePostProcess;
	CCheckBox OtherRadar;
	CCheckBox OtherNoVisualRecoil;
	CCheckBox OtherNoSky;
	CCheckBox OtherNoFlash;
	CCheckBox nosmoke;
	CCheckBox OtherAsusWalls;
	CComboBoxYeti OtherNoHands;
	CCheckBox AAIndicators;
	CCheckBox BulletTracers;
	CComboBoxYeti localmaterial;
	CComboBoxYeti scopemat;
	CSlider transparency;
	CSlider hand_transparency;
	CSlider gun_transparency;
	CSlider asusamount;
	CComboBoxYeti asustype;
	CSlider beamtime;
	CCheckBox watermark;
	CSlider beamsize;
	CCheckBox cheatinfo;
	CCheckBox RemoveZoom;
	CComboBoxYeti sound;
	CCheckBox SpreadCross;
	CSlider SpreadCrossSize;
	CCheckBox DamageIndicator;
	CSlider OtherViewmodelFOV;
	CComboBoxYeti SpreadCrosshair;
	CSlider OtherFOV;
	CGroupBox worldgroup;
	CCheckBox colmodupdate;
	CSlider colmod;
	CCheckBox customskies2;
	CComboBoxYeti customskies;
};
class CColorTab : public CTab
{
public:
	void Setup();
	//you pasted the fucking colorselector but didn't use it... man man freak no baka
	//you have no fucking idea how many errors and how much cancer this gave me
	CGroupBox ColorsGroup;
	CComboBoxYeti beam_type;
	CGroupBox ConfigGroup;
	CGroupBox OtherOptions;
	CGroupBox OtherOptions2;
	CCheckBox ClanTag;
	CListBox ConfigListBox;
	CButton SaveConfig;
	CButton LoadConfig;
	CButton RemoveConfig;
	CTextField NewConfigName;
	CComboBoxYeti MenuBar;
	CButton AddConfig;

	CColorSelector NameCol;
	CColorSelector HandChamsCol;
	CColorSelector GunChamsCol;
	CColorSelector BoxCol;
	CColorSelector BoxColteam;
	CColorSelector Skeleton;
	CColorSelector Skeletonteam;
	CColorSelector GlowEnemy; //no one has fucking team esp on so i'm not even gonna bother making a color selector for that
	CColorSelector GlowTeam;
	CColorSelector GlowLocal;
	CColorSelector GlowOtherEnt;
	CColorSelector Skyboxcolor;
	CColorSelector spreadcrosscol;
	CColorSelector Weapons;
	CColorSelector Weaponsteam;
	CColorSelector Ammo;
	CColorSelector Ammoteam;
	CColorSelector Money; //we don't really need to be able to change the color for all the flags it's just retarded
	CColorSelector Offscreen;
	CColorSelector ChamsLocal;
	CColorSelector ChamsEnemyVis;
	CColorSelector ChamsEnemyNotVis;
	CColorSelector ChamsTeamVis;
	CColorSelector ChamsTeamNotVis;
	CColorSelector Bullettracer;
	CColorSelector Menu;
	CColorSelector bomb_timer;
	CColorSelector scoped_c;
	CColorSelector misc_backtrackchams;
	CColorSelector misc_lagcomp;
	CColorSelector misc_lagcompChams;
	CColorSelector misc_lagcompBones;

	CComboBoxYeti chamcount;
	CComboBoxYeti bonecount;
	CComboBoxYeti asus_type;

	CCheckBox DebugLagComp;
	CCheckBox BackTrackBones2;
	CCheckBox BackTrackBones;
	CCheckBox ambient;
	CCheckBox ModulateSkyBox;

	CSlider sky_r;
	CSlider sky_g;
	CSlider sky_b;

	CSlider AmbientRed;
	CSlider AmbientGreen;
	CSlider AmbientBlue;


	CGroupBox gcol;
	CSlider outl_r;
	CSlider outl_g;
	CSlider outl_b;

	CSlider inl_r;
	CSlider inl_g;
	CSlider inl_b;

	CSlider inr_r;
	CSlider inr_g;
	CSlider inr_b;


	CSlider outr_r;
	CSlider outr_g;
	CSlider outr_b;

	CSlider cr;
	CSlider cg;
	CSlider cb;

	CCheckBox experimental_backtrack;
	CSlider quickstop_speed;
};
class CMiscTab : public CTab
{
public:
	void Setup();
	CCheckBox DisablePostProcess;
	CComboBoxYeti hitmarker_sound;
	CComboBoxYeti autojump_type;
	CComboBoxYeti airduck_type;
	CGroupBox OtherGroup;
	CCheckBox OtherAutoJump;
	CCheckBox meme;
	CCheckBox OtherAutoStrafe;
	CKeyBind Zstrafe;
	CComboBoxYeti OtherSafeMode;
	CCheckBox freestandtype;
	CKeyBind OtherCircleButton;
	CCheckBox AutoDefuse;
	CCheckBox OtherCircleStrafe;
	CCheckBox QuickStop;
	CCheckBox CircleStrafe;
	CKeyBind CircleStrafeKey;
	CKeyBind minimal_walk;
	CKeyBind OtherCircle;
	CCheckBox choked_shot;
	CTextField CustomClantag;
	CCheckBox CheatsByPass;
	CCheckBox DebugHitbones;
	CSlider freerange;
	CCheckBox aa_helper;
	CCheckBox infinite_duck;
	CComboBoxYeti buybot_primary;
	CComboBoxYeti buybot_secondary;
	CDropBox buybot_otr;
	CCheckBox AutoAccept;
	CSlider custom_pitch;
	CCheckBox SniperCrosshair;
	CSlider CircleAmount;
	CKeyBind OtherSlowMotion;
	CKeyBind plugwalk;
	CComboBoxYeti FakeLagTyp;
	CCheckBox autofw;
	CKeyBind fw;
	CComboBoxYeti FakeYaw;
	CComboBoxYeti FakeYaw2;
	CComboBoxYeti FakeYaw3;
	CKeyBind manualleft;
	CKeyBind manualright;
	CKeyBind manualback;
	CKeyBind manualfront;
	CCheckBox do_freestanding;
	CCheckBox disable_on_dormant;
	CCheckBox fake_crouch;
	CKeyBind fake_crouch_key;
	CGroupBox AntiAimGroup;

	CCheckBox AntiAimEnable;
	CComboBoxYeti AntiAimPitch;
	CComboBoxYeti AntiAimYaw;
	CComboBoxYeti antiaimtype_stand;
	CComboBoxYeti antiaimtype_move;
	CCheckBox desync_aa_stand;
	CCheckBox desync_aa_move;
	CComboBoxYeti desync_aa_air;
	CSlider staticr;
	CSlider staticf;
	CSlider ThirdPersonVal;
	CComboBoxYeti AntiAimYaw3;
	CComboBoxYeti AntiAimYawrun;
	CCheckBox Radar;
	CCheckBox OtherTeamChat;
	CSlider ClanTagSpeed;
	CSlider	  OtherChatDelay;
	CCheckBox NameChanger;
	CCheckBox AutoPistol;
	CCheckBox ClanTag;
	CCheckBox TrashTalk;
	CKeyBind  OtherAirStuck;
	CCheckBox OtherSpectators;
	CCheckBox OtherThirdperson;
	CComboBoxYeti OtherClantag;

	CSlider spinspeed_fake;
	CKeyBind fakelag_key;
	CKeyBind ThirdPersonKeyBind;
	CSlider FakeWalkSpeed;
	CSlider ThirdPersonValue;
	CCheckBox standing_desync;
	CCheckBox moving_desync;
	CCheckBox air_desync;
	CSlider twitchf;
	CSlider spinf;
	CSlider randlbyr;
	CSlider randlbyf;
	CSlider spinspeed;
	CSlider lby1;
	CCheckBox squaredance;
	CComboBoxYeti antilby;
	CCheckBox pitch_up;
	CCheckBox experimental;
	CSlider BreakLBYDelta;
	CSlider BreakLBYDelta2;
	CGroupBox FakeLagGroup;
	CSlider RadarX;
	CSlider RadarY;
	CCheckBox FakeLagEnable;
	CSlider Fakelagjump;
	CCheckBox PingSpike;
	CKeyBind LagSpikeKey;
	CComboBoxYeti KnifeModel;
	CButton   SkinApply;
	CCheckBox SkinEnable;
	CSlider FakelagMove;
	CSlider FakelagStand;
	CCheckBox fl_onshot;
	CCheckBox FakelagBreakLC;
	CSlider   FakeLagChoke;
	CCheckBox FakelagOnground;
	CCheckBox antilby2;
	CSlider FakeLagChoke2;
	CKeyBind lagkey;
	CComboBoxYeti ConfigBox;
	CGroupBox ConfigGroup;

	CSlider pingspike_val;
	CKeyBind pingspike_key;

	CDropBox fl_spike;
};

class CSkinTab : public CTab
{
public:
	void Setup();
	CGroupBox group_main;
	CButton update;

	CCheckBox toggle;

	CComboBoxYeti knife;
	CComboBoxYeti scar20_sniper_skin;
	CComboBoxYeti ak_skin;

	CTextField skin_id;
};

class mirror_window : public CWindow
{
public:
	void Setup();
	CRageBotTab RageBotTab;
	CLegitBotTab LegitBotTab;
	CVisualTab VisualsTab;
	CMiscTab MiscTab;
	CSkinTab SkinTab;

	CColorTab ColorsTab;
	CButton   SkinApply;
	CButton SaveButton1;
	CButton LoadButton1;
	CButton SaveButton;
	CButton LoadButton;
	CButton UnloadButton;
	CButton PanicButton;
	CButton ThirdButton;
	CButton ByPass;
	CButton FirstButton;
	CComboBox ConfigBox;
};
namespace Options
{
	void SetupMenu();
	void DoUIFrame();
	extern mirror_window Menu;
};