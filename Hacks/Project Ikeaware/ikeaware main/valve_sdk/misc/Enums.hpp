#pragma once

enum ECstrike15UserMessages
{
    CS_UM_VGUIMenu = 1,
    CS_UM_Geiger = 2,
    CS_UM_Train = 3,
    CS_UM_HudText = 4,
    CS_UM_SayText = 5,
    CS_UM_SayText2 = 6,
    CS_UM_TextMsg = 7,
    CS_UM_HudMsg = 8,
    CS_UM_ResetHud = 9,
    CS_UM_GameTitle = 10,
    CS_UM_Shake = 12,
    CS_UM_Fade = 13,
    CS_UM_Rumble = 14,
    CS_UM_CloseCaption = 15,
    CS_UM_CloseCaptionDirect = 16,
    CS_UM_SendAudio = 17,
    CS_UM_RawAudio = 18,
    CS_UM_VoiceMask = 19,
    CS_UM_RequestState = 20,
    CS_UM_Damage = 21,
    CS_UM_RadioText = 22,
    CS_UM_HintText = 23,
    CS_UM_KeyHintText = 24,
    CS_UM_ProcessSpottedEntityUpdate = 25,
    CS_UM_ReloadEffect = 26,
    CS_UM_AdjustMoney = 27,
    CS_UM_UpdateTeamMoney = 28,
    CS_UM_StopSpectatorMode = 29,
    CS_UM_KillCam = 30,
    CS_UM_DesiredTimescale = 31,
    CS_UM_CurrentTimescale = 32,
    CS_UM_AchievementEvent = 33,
    CS_UM_MatchEndConditions = 34,
    CS_UM_DisconnectToLobby = 35,
    CS_UM_PlayerStatsUpdate = 36,
    CS_UM_DisplayInventory = 37,
    CS_UM_WarmupHasEnded = 38,
    CS_UM_ClientInfo = 39,
    CS_UM_XRankGet = 40,
    CS_UM_XRankUpd = 41,
    CS_UM_CallVoteFailed = 45,
    CS_UM_VoteStart = 46,
    CS_UM_VotePass = 47,
    CS_UM_VoteFailed = 48,
    CS_UM_VoteSetup = 49,
    CS_UM_ServerRankRevealAll = 50,
    CS_UM_SendLastKillerDamageToClient = 51,
    CS_UM_ServerRankUpdate = 52,
    CS_UM_ItemPickup = 53,
    CS_UM_ShowMenu = 54,
    CS_UM_BarTime = 55,
    CS_UM_AmmoDenied = 56,
    CS_UM_MarkAchievement = 57,
    CS_UM_MatchStatsUpdate = 58,
    CS_UM_ItemDrop = 59,
    CS_UM_GlowPropTurnOff = 60,
    CS_UM_SendPlayerItemDrops = 61,
    CS_UM_RoundBackupFilenames = 62,
    CS_UM_SendPlayerItemFound = 63,
    CS_UM_ReportHit = 64,
    CS_UM_XpUpdate = 65,
    CS_UM_QuestProgress = 66,
    CS_UM_ScoreLeaderboardData = 67,
    CS_UM_PlayerDecalDigitalSignature = 68,
    MAX_ECSTRIKE15USERMESSAGES
};


enum ItemDefinitionIndex
{
	WEAPON_INVALID = -1,
	WEAPON_NONE,
	WEAPON_DEAGLE,
	WEAPON_ELITE,
	WEAPON_FIVESEVEN,
	WEAPON_GLOCK,
	WEAPON_AK47 = 7,
	WEAPON_AUG,
	WEAPON_AWP,
	WEAPON_FAMAS,
	WEAPON_G3SG1,
	WEAPON_GALILAR = 13,
	WEAPON_M249,
	WEAPON_M4A1 = 16,
	WEAPON_MAC10,
	WEAPON_P90 = 19,
	WEAPON_MP5 = 23,
	WEAPON_UMP45,
	WEAPON_XM1014,
	WEAPON_BIZON,
	WEAPON_MAG7,
	WEAPON_NEGEV,
	WEAPON_SAWEDOFF,
	WEAPON_TEC9,
	WEAPON_TASER,
	WEAPON_HKP2000,
	WEAPON_MP7,
	WEAPON_MP9,
	WEAPON_NOVA,
	WEAPON_P250,
	WEAPON_SHIELD,
	WEAPON_SCAR20,
	WEAPON_SG556,
	WEAPON_SSG08,
	WEAPON_KNIFEGG,
	WEAPON_KNIFE,
	WEAPON_FLASHBANG,
	WEAPON_HEGRENADE,
	WEAPON_SMOKEGRENADE,
	WEAPON_MOLOTOV,
	WEAPON_DECOY,
	WEAPON_INCGRENADE,
	WEAPON_C4,
	WEAPON_HEALTHSHOT = 57,
	WEAPON_KNIFE_T = 59,
	WEAPON_M4A1_SILENCER,
	WEAPON_USP_SILENCER,
	WEAPON_CZ75A = 63,
	WEAPON_REVOLVER,
	WEAPON_TAGRENADE = 68,
	WEAPON_FISTS,
	WEAPON_BREACHCHARGE,
	WEAPON_TABLET = 72,
	WEAPON_MELEE = 74,
	WEAPON_AXE,
	WEAPON_HAMMER,
	WEAPON_SPANNER = 78,
	WEAPON_KNIFE_GHOST = 80,
	WEAPON_FIREBOMB,
	WEAPON_DIVERSION,
	WEAPON_FRAG_GRENADE,
	WEAPON_SNOWBALL,
	WEAPON_BUMPMINE,
	WEAPON_KNIFE_BAYONET = 500,
	WEAPON_KNIFE_FLIP = 505,
	WEAPON_KNIFE_GUT,
	WEAPON_KNIFE_KARAMBIT,
	WEAPON_KNIFE_M9_BAYONET,
	WEAPON_KNIFE_TACTICAL,
	WEAPON_KNIFE_FALCHION = 512,
	WEAPON_KNIFE_SURVIVAL_BOWIE = 514,
	WEAPON_KNIFE_BUTTERFLY,
	WEAPON_KNIFE_PUSH,
	WEAPON_KNIFE_URSUS = 519,
	WEAPON_KNIFE_GYPSY_JACKKNIFE,
	WEAPON_KNIFE_STILETTO = 522,
	WEAPON_KNIFE_WIDOWMAKER,
	GLOVE_STUDDED_BLOODHOUND = 5027,
	GLOVE_T_SIDE = 5028,
	GLOVE_CT_SIDE = 5029,
	GLOVE_SPORTY = 5030,
	GLOVE_SLICK = 5031,
	GLOVE_LEATHER_WRAP = 5032,
	GLOVE_MOTORCYCLE = 5033,
	GLOVE_SPECIALIST = 5034,
	GLOVE_HYDRA = 5035
};

enum ECSPlayerBones
{
    PELVIS = 0,
    SPINE_0,
    SPINE_1,
    SPINE_2,
    SPINE_3,
    NECK_0,
    HEAD_0,
    CLAVICLE_L,
    ARM_UPPER_L,
    ARM_LOWER_L,
    HAND_L,
    FINGER_MIDDLE_META_L,
    FINGER_MIDDLE_0_L,
    FINGER_MIDDLE_1_L,
    FINGER_MIDDLE_2_L,
    FINGER_PINKY_META_L,
    FINGER_PINKY_0_L,
    FINGER_PINKY_1_L,
    FINGER_PINKY_2_L,
    FINGER_INDEX_META_L,
    FINGER_INDEX_0_L,
    FINGER_INDEX_1_L,
    FINGER_INDEX_2_L,
    FINGER_THUMB_0_L,
    FINGER_THUMB_1_L,
    FINGER_THUMB_2_L,
    FINGER_RING_META_L,
    FINGER_RING_0_L,
    FINGER_RING_1_L,
    FINGER_RING_2_L,
    WEAPON_HAND_L,
    ARM_LOWER_L_TWIST,
    ARM_LOWER_L_TWIST1,
    ARM_UPPER_L_TWIST,
    ARM_UPPER_L_TWIST1,
    CLAVICLE_R,
    ARM_UPPER_R,
    ARM_LOWER_R,
    HAND_R,
    FINGER_MIDDLE_META_R,
    FINGER_MIDDLE_0_R,
    FINGER_MIDDLE_1_R,
    FINGER_MIDDLE_2_R,
    FINGER_PINKY_META_R,
    FINGER_PINKY_0_R,
    FINGER_PINKY_1_R,
    FINGER_PINKY_2_R,
    FINGER_INDEX_META_R,
    FINGER_INDEX_0_R,
    FINGER_INDEX_1_R,
    FINGER_INDEX_2_R,
    FINGER_THUMB_0_R,
    FINGER_THUMB_1_R,
    FINGER_THUMB_2_R,
    FINGER_RING_META_R,
    FINGER_RING_0_R,
    FINGER_RING_1_R,
    FINGER_RING_2_R,
    WEAPON_HAND_R,
    ARM_LOWER_R_TWIST,
    ARM_LOWER_R_TWIST1,
    ARM_UPPER_R_TWIST,
    ARM_UPPER_R_TWIST1,
    LEG_UPPER_L,
    LEG_LOWER_L,
    ANKLE_L,
    BALL_L,
    LFOOT_LOCK,
    LEG_UPPER_L_TWIST,
    LEG_UPPER_L_TWIST1,
    LEG_UPPER_R,
    LEG_LOWER_R,
    ANKLE_R,
    BALL_R,
    RFOOT_LOCK,
    LEG_UPPER_R_TWIST,
    LEG_UPPER_R_TWIST1,
    FINGER_PINKY_L_END,
    FINGER_PINKY_R_END,
    VALVEBIPED_WEAPON_BONE,
    LH_IK_DRIVER,
    PRIMARY_JIGGLE_JNT,
    MAX_ECSPLAYERBONES
};

enum EntityFlags
{
    FL_ONGROUND = (1 << 0),
    FL_DUCKING = (1 << 1),
    FL_WATERJUMP = (1 << 2),
    FL_ONTRAIN = (1 << 3),
    FL_INRAIN = (1 << 4),
    FL_FROZEN = (1 << 5),
    FL_ATCONTROLS = (1 << 6),
    FL_CLIENT = (1 << 7),
    FL_FAKECLIENT = (1 << 8),
    MAX_ENTITYFLAGS
};

enum LifeState : unsigned char
{
    LIFE_ALIVE = 0,// alive
    LIFE_DYING = 1, // playing death animation or still falling off of a ledge waiting to hit ground
    LIFE_DEAD = 2, // dead. lying still.
    MAX_LIFESTATE
};

enum WeaponSound_t
{
    EMPTY,
    SINGLE,
    SINGLE_NPC,
    WPN_DOUBLE, // Can't be "DOUBLE" because windows.h uses it.
    DOUBLE_NPC,
    BURST,
    RELOAD,
    RELOAD_NPC,
    MELEE_MISS,
    MELEE_HIT,
    MELEE_HIT_WORLD,
    SPECIAL1,
    SPECIAL2,
    SPECIAL3,
    TAUNT,
    FAST_RELOAD,
    // Add new shoot sound types here
    REVERSE_THE_NEW_SOUND,
    NUM_SHOOT_SOUND_TYPES,
    MAX_WEAPONSOUND
};

enum MoveType_t
{
    MOVETYPE_NONE = 0,
    MOVETYPE_ISOMETRIC,
    MOVETYPE_WALK,
    MOVETYPE_STEP,
    MOVETYPE_FLY,
    MOVETYPE_FLYGRAVITY,
    MOVETYPE_VPHYSICS,
    MOVETYPE_PUSH,
    MOVETYPE_NOCLIP,
    MOVETYPE_LADDER,
    MOVETYPE_OBSERVER,
    MOVETYPE_CUSTOM,
    MOVETYPE_LAST = MOVETYPE_CUSTOM,
    MOVETYPE_MAX_BITS = 4,
    MAX_MOVETYPE
};
