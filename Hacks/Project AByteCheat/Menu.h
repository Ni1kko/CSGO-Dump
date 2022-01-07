#pragma once
#include "GUI.h"
#include "Controls.h"
class CAimbotTab : public CTab
{
public:
	void Setup();
	CLabel ActiveLabel;
	CCheckBox Active;
	CComboBoxYeti psilent;
	CGroupBox AimbotGroup;
	CCheckBox AimbotEnable;
	CCheckBox lag_pred;
	CCheckBox backtrack;
	CCheckBox AimbotAutoFire;
	CCheckBox AimbotSilentAim;
	CCheckBox legit_mode;
	CCheckBox legit_trigger;
	CKeyBind legit_trigger_key;
	CCheckBox apply_smooth;
	CComboBoxYeti AimbotResolver;
	CSlider AimbotFov;
	//-------------------------
	//--------------------------
	CCheckBox RemoveSpread;
	CCheckBox RemoveRecoil;
	CCheckBox AvoidLimbs;
	CCheckBox LogMisses;
	CCheckBox LowerbodyFix;
	CCheckBox AWPAtBody;
	CSlider BaimIfUnderXHealth;
	CSlider XShots;
	CCheckBox EnginePrediction;
	CCheckBox AimbotAimStep;
	CCheckBox pointscaleyes;
	CCheckBox Multienable;
	CSlider pointscaleval;
	CSlider Multival;
	CSlider MultiVal3;
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
	CKeyBind  AimbotKeyBindXD;
	CKeyBind  AimbotStopKey;
	CGroupBox TargetGroup;
	CComboBoxYeti TargetSelection;
	CCheckBox TargetFriendlyFire;
	CComboBoxYeti TargetHitbox;
	CComboBoxYeti TargetHitscan;
	CComboBoxYeti delay_shot;
	CCheckBox TargetMultipoint;
	CSlider   TargetPointscale;
	CSlider   bruteX;
	CSlider   baim;

	//--------------//
	CGroupBox weapongroup;

	CSlider hc;
	CSlider md;
	CSlider hc_auto;
	CSlider hc_scout;
	CSlider hc_awp;
	CSlider hc_pistol;
	CSlider hc_smg;
	CSlider hc_otr;

	CSlider md_auto;
	CSlider md_scout;
	CSlider md_awp;
	CSlider md_pistol;
	CSlider md_smg;
	CSlider md_otr;
	CComboBoxYeti MultipointMultiplier;

	// CComboBoxYeti target_auto;
	CDropBox target_auto;
	CDropBox target_scout;
	CDropBox target_awp;
	CDropBox target_pistol;
	CDropBox target_smg;
	CDropBox target_otr;
	CDropBox msc_otr;


	CCheckBox enemyhp_auto;
	CCheckBox enemyhp_scout;
	CCheckBox enemyhp_awp;
	CCheckBox enemyhp_pistol;

	//--------------//

	CGroupBox AccuracyGroup;
	CCheckBox AccuracyRecoil;

	CCheckBox AccuracyAutoWall;
	CSlider AccuracyMinimumDamage;
	CCheckBox AccuracyAutoStop;
	CSlider Multival2;
	CSlider Multival4;

	CComboBoxYeti LowFPS;

	CCheckBox AccuracyAutoCrouch;
	CCheckBox AccuracyAutoScope;
	CSlider   AccuracyHitchance;
	CComboBoxYeti resolver;
	
	CCheckBox QuickStopCheck;

	CCheckBox ForceBaimCheck;

	CCheckBox AimStep;
	CCheckBox OverrideAWP;
	CComboBoxYeti preso;
	CDropBox Multipoint;
	CCheckBox custom_hitscan;

	CComboBoxYeti AccuracyBoost;
	CComboBoxYeti AccuracyBoostOptions;

	CComboBoxYeti OverrideMode;
	CKeyBind bigbaim;
	CKeyBind BaimOnKey;
	CKeyBind flip180;
	CCheckBox extrapolation;

	CKeyBind QuickStopKey;
	CComboBoxYeti QuickStop;

	CComboBoxYeti PreferBodyAim;

	CDropBox prefer_head;
	CCheckBox toggledebug;
	CDropBox debug;
};

class CAntiAimTab : public CTab
{
public:
	void Setup();
	CKeyBind AAOnKey;
	CCheckBox AEnable;
	CGroupBox AntiAimbotAngles;
	CComboBoxYeti Pitch;
	CComboBoxYeti YawBase;
	CComboBoxYeti Yaw;
	CSlider YawOffset;
	CComboBoxYeti YawJitter;
	CSlider YawJitterOffset;
	CComboBoxYeti FakeYaw;
	CSlider FakeYawOffset;
	CComboBoxYeti BodyYaw;
	CComboBoxYeti EdgeYaw;
	CComboBoxYeti Freestanding;
	CCheckBox Twist;
	CCheckBox LowerBodyYaw;

	CGroupBox Fakelag;
	CKeyBind FakelagKey;
	CCheckBox FEnable;
	CDropBox Triggers;
	CComboBoxYeti Amount;
	CSlider Variance;
	CSlider Limit;
	CCheckBox BHopReset;
	CCheckBox WhileShooting;

	CGroupBox Other;
	CKeyBind FakeCrouchKey;
	CCheckBox FakeCrouch;
	CKeyBind SlowMotionKey;
	CCheckBox SlowMotion;
	CCheckBox FixLegMovement;
	CCheckBox InfiniteDuck;
	CCheckBox OnShotAA;
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
	CCheckBox SkeetLegitTab;
	CComboBoxYeti aimbotfiremode;
	CCheckBox AimbotFriendlyFire;
	CCheckBox AimbotKeyPress;
	CKeyBind  AimbotKeyBind;
	CCheckBox AimbotSmokeCheck;
	CCheckBox BackTrack;
	CCheckBox legitresolver;
	CCheckBox legitonkey;
	CGroupBox TriggerGroup;
	CComboBoxYeti triggertype;
	CCheckBox TriggerEnable;
	CCheckBox TriggerAuto;
	CCheckBox TriggerHeadSkeet;
	CCheckBox TriggerChestSkeet;
	CCheckBox TriggerStomachSkeet;
	CKeyBind  TriggerKeyBind;
	CSlider   TriggerDelay;
	CSlider   TriggerBurst;
	CSlider   TriggerBreak;
	CSlider TriggerRecoil; CSlider   TriggerHitChanceAmmount;
	CSlider TriggerRecoilSkeet; CSlider   TriggerHitChanceAmmountSkeet;
	CGroupBox TriggerFilterGroup;
	CSlider   SpeedSkeet;
	CSlider   SpeedSkeetInattack;
	CSlider   SpeedSkeetscale;
	CSlider   SpeedSkeetMax;
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
	CGroupBox normalgroup;
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
	CSlider WeaponMGSkeet;
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
	CCheckBox visualize_desync;
	CSlider flashAlpha;
	CGroupBox OptionsGroup;
	CCheckBox OptionsWeapone;
	CCheckBox OptionsBox;
	CCheckBox OptionsName;
	CColorSelector CrosshairColor;
	CCheckBox HitmarkerSound;
	CCheckBox OtherHitmarker;
	CColorSelector HitmarkerColor;
	CCheckBox ChamsEnemyvisible;
	CCheckBox ChamsLocal;
	CCheckBox aa_helper;
	CCheckBox ChamsEnemyinVisible;
	CComboBoxYeti helpermat;
	CColorSelector Offscreen;
	CCheckBox OffscreenESP;
	CComboBoxYeti WeaponArray;
	CColorSelector DroppedWeaponNameCLR;
	CColorSelector DroppedWeaponAmmoCLR;
	CColorSelector ThrownNadesColor;
	CComboBoxYeti chamstype;
	CColorSelector urdadyoufaggot;
	CColorSelector ChamsEnemyVis;
	CColorSelector ChamsEnemyNotVis;
	CColorSelector GlowEnemy; //no one has fucking team esp on so i'm not even gonna bother making a color selector for that
	CCheckBox lolol;
	CSlider Glowz_lcl;
	CCheckBox BlendIfScoped;
	CSlider BlendValue;
	CGroupBox Effects;
	CCheckBox EnemyGlowCheck;
	CComboBoxYeti OptionsChams;
	CComboBoxYeti OptionsArmor;
	CCheckBox OptionsHealth;
	CComboBoxYeti OptionsWeapon;
	CComboBoxYeti manualaa_type;
	CColorSelector WeaponTextColor;
	CColorSelector WeaponIconColor;
	CComboBoxYeti HandCHAMS;
	CComboBoxYeti GunCHAMS;
	CCheckBox LBYIndicator;
	CCheckBox LCIndicator;
	CCheckBox DroppedWeaponAmmo;
	CComboBoxYeti DroppedWeaponsType;
	CComboBoxYeti FakeDuckIndicator;
	CComboBoxYeti OtherThirdpersonAngle;
	CComboBoxYeti WeaponChams;
	CColorSelector misc_lagcompBones;
	CColorSelector spreadcrosscol;
	CComboBoxYeti fakelag_ghost;
	CComboBoxYeti visible_chams_type;
	CComboBoxYeti invisible_chams_type;
	CSlider Testaaa;
	CCheckBox DebugLagComp;
	CCheckBox BackTrackBones2;
	CColorSelector BoxCol;
	CColorSelector BoxColteam;
	CColorSelector Skeleton;
	CColorSelector HitHitboxColor;
	CColorSelector Skeletonteam;
	CColorSelector NameCol;
	CCheckBox SleeveChams;
	CCheckBox Weapons;
	CCheckBox ChamsTeamVis;
	CCheckBox ChamsTeamNoVis;
	CCheckBox OptionsGlow_lcl;
	CCheckBox NoGlowIfScoped;
	CCheckBox NoChamsIfScoped;
	CCheckBox armorbar;
	CCheckBox OptionsInfo;
	CCheckBox BulletTrace;
	CCheckBox BulletTrace_enemy;
	CCheckBox ChamsVisibleOnly;
	CColorSelector AmmoC;
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
	CCheckBox show_hostage;
	CCheckBox DiscoLights;
	CCheckBox Money;
	CCheckBox OptionsDefuse;
	CCheckBox OptionsDefusing;
	CCheckBox SniperCrosshair;
	CCheckBox OptionsSkeleton;
	CCheckBox NightSky;
	CCheckBox OtherThirdperson;
	CKeyBind ThirdPersonKeyBind;
	CCheckBox hitbone;
	CCheckBox OptionsAimSpot;
	CCheckBox OtherNoScope;
	CCheckBox OptionsCompRank;
	CCheckBox resoinfo;
	CCheckBox killfeed;
	CCheckBox override_viewmodel;
	CCheckBox selfglow;
	CGroupBox FiltersGroup;
	CCheckBox WeaponIconESP;
	CCheckBox FiltersAll;
	CCheckBox show_players;
	CCheckBox show_team;
	CCheckBox show_local;
	CCheckBox FiltersWeapons;
	CCheckBox WeaponIconDrop;
	CKeyBind flashlight;
	CCheckBox FiltersChickens;
	CCheckBox FiltersNades;
	CCheckBox FiltersC4;
	CCheckBox FiltersChicken;
	CGroupBox ChamsGroup;
	CComboBoxYeti asus_type;
	CCheckBox ChamsThruWalls;
	CCheckBox ModulateSkyBox;
	CCheckBox hitmarker_sound;
	CSlider sky_r;
	CSlider sky_g;
	CSlider sky_b;
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
	CSlider nosmoke_slider;
	CCheckBox OtherAsusWalls;
	CComboBoxYeti OtherNoHands;
	CCheckBox AAIndicators;
	CCheckBox BulletTracers;
	CComboBoxYeti localmaterial;
	CComboBoxYeti scopemat;
	CSlider transparency;
	CSlider hand_transparency;
	CSlider gun_transparency;
	CSlider sleeve_transparency;
	CSlider asusamount;
	CButton UpdateWorld;
	CComboBoxYeti asustype;
	CSlider beamtime;
	CSlider beamsize;
	CCheckBox cheatinfo;
	CCheckBox RemoveZoom;
	CComboBoxYeti sound;
	CCheckBox SpreadCross;
	CSlider SpreadCrossSize;
	CCheckBox DamageIndicator;
	CSlider OtherViewmodelFOV;
	CCheckBox SpreadCrosshair;
	CSlider OtherFOV;
	CGroupBox worldgroup;
	CCheckBox colmodupdate;
	CSlider colmod;
	CCheckBox customskies2;
	CComboBoxYeti customskies;
	CCheckBox optimize;
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
	CSlider radar_alpha;
	CSlider owo_slider;
	CColorSelector HandChamsCol;
	CColorSelector GunChamsCol;
	CColorSelector GlowTeam;
	CColorSelector GlowLocal;
	CColorSelector GlowOtherEnt;
	CColorSelector Skyboxcolor;
	CColorSelector Weapons;
	CColorSelector Weaponsteam;
	CColorSelector Ammoteam;
	CColorSelector Money; //we don't really need to be able to change the color for all the flags it's just retarded
	CColorSelector ChamsLocal;
	CColorSelector ChamsTeamVis;
	CColorSelector ChamsTeamNotVis;
	CColorSelector Bullettracer_local;
	CColorSelector Bullettracer_enemy;
	CColorSelector bomb_timer;
	CColorSelector SleeveChams_col;
	CColorSelector scoped_c;
	CColorSelector misc_backtrackchams;
	CColorSelector misc_lagcomp;
	CColorSelector misc_lagcompChams;
	CColorSelector fakelag_ghost;

	CComboBoxYeti chamcount;
	CComboBoxYeti bonecount;
	CComboBoxYeti asus_type;

	CCheckBox BackTrackBones;
	CCheckBox ambient;

	CSlider AmbientRed;
	CSlider AmbientGreen;
	CSlider AmbientBlue;

	CCheckBox whitelist;
	CCheckBox AllowSharedEsp;
	CCheckBox DisableVisuals;
	CCheckBox PlayerlistHighPriority;
	CComboBoxYeti Forceyaw;
	CSlider Forceyawvalue;
	CCheckBox Forcepitch;
	CSlider Forcepitchvalue;
	CComboBoxYeti Overrideantiaim;
	CCheckBox Edgecorrection;
	CComboBoxYeti Overridepreferbodyaim;
	CComboBoxYeti Overrideaccuracyboost;
	CButton Applytoall;
	
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
	CComboBoxYeti autojump_type;
	CComboBoxYeti airduck_type;
	CComboBoxYeti airstrafe;
	CSlider Airstrafesmoothing;
	CCheckBox ZHop;
	CCheckBox Prespeed;
	CCheckBox Knifebot;
	CCheckBox Zeusbot;
	CCheckBox Blockbot;
	CCheckBox Automaticweapons;
	CCheckBox Edgejump;
	CCheckBox Rank;
	CCheckBox Autommm;
	CCheckBox Logdmgs;
	CCheckBox Logdmgpurchases;
	CCheckBox Automaticgrenade;
	CCheckBox Pingspike;
	CCheckBox Fastwalk;
	CButton stealplayername;
	CButton dumpmmwins;
	CGroupBox OtherGroup;
	CGroupBox SettingsGroup;
	CKeyBind MenuKey;
	CListBox ConfigListBox;
	CButton SaveConfig;
	CButton LoadConfig;
	CButton Export;
	CButton Restmenu;
	CButton luaapi;
	CButton Unload;
	CButton Import;
	CButton RemoveConfig;
	CTextField NewConfigName;
	CButton AddConfig;
	CLabel MenuKeyText;
	CColorSelector MenuColor;
	CLabel MenuColorText;
	CCheckBox HideFromOBS;
	CCheckBox LowFPSWarning;
	CCheckBox LockMenuLayout;
	CCheckBox AntiUntrusted;
	CCheckBox OtherAutoJump;
	CCheckBox meme;
	CCheckBox OtherAutoStrafe;
	CKeyBind Zstrafe;
	CComboBoxYeti OtherSafeMode;
	CCheckBox freestandtype;
	CKeyBind OtherCircleButton;
	CCheckBox AutoDefuse;
	CCheckBox OtherCircleStrafe;
	CCheckBox CircleStrafe;
	CKeyBind CircleStrafeKey;
	CKeyBind minimal_walk;
	CKeyBind OtherCircle;
	CTextField CustomClantag;
	CCheckBox CheatsByPass;
	CCheckBox DebugHitbones;
	CCheckBox aa_helper;
	CCheckBox infinite_duck;
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
	CCheckBox do_freestanding;
	CCheckBox disable_on_dormant;
	CCheckBox logs;
	CCheckBox watermark;
	CComboBoxYeti buybot_primary;
	CComboBoxYeti buybot_secondary;
	CDropBox buybot_otr;

	CCheckBox fake_crouch;
	CKeyBind fake_crouch_key;
	CKeyBind fake_stand_key;
	CGroupBox AntiAimGroup;

	CCheckBox AntiAimEnable;
	CComboBoxYeti AntiAimPitch;
	CComboBoxYeti AntiAimYaw;
	CComboBoxYeti antiaimtype_stand;
	CComboBoxYeti antiaimtype_move;
	CCheckBox desync_aa_stand;
	CCheckBox desync_aa_move;

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
	CKeyBind  OtherAirStuck;
	CCheckBox OtherSpectators;

	CKeyBind fakelag_key;
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

	CComboBoxYeti fl_spike;
};

class CSkinTab : public CTab
{
public:
	void Setup();
	//--------------------------//
	CGroupBox knifegroup;
	CGroupBox snipergroup;

	//--------------------------//
	CComboBoxYeti t_knife_index;
	CComboBoxYeti ct_knife_index;

	//--------------------------//
	CSlider t_knife_wear;
	CSlider t_sniperSCAR_wear;
	CSlider t_sniperAWP_wear;
	CComboBoxYeti Overrideknife;

	//--------------------------//
	CComboBoxYeti t_knife_skin_id;
	CComboBoxYeti t_sniperAWP_skin_id;
	CComboBoxYeti t_sniperSCAR_skin_id;
};

class mirror_window : public CWindow
{
public:
	void Setup();
	CAimbotTab aimbot_tab;
	CAntiAimTab AntiAimTab;
	CLegitBotTab LegitBotTab;
	CVisualTab visuals_tab;
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
namespace options
{
	void SetupMenu();
	void DoUIFrame();
	extern mirror_window menu;
};