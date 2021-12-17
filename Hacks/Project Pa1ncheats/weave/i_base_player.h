#pragma once
#include "Vector.h"
#include "cvar.h"

#pragma region Defines

//LIFESTATE
#define	LIFE_ALIVE				0
#define	LIFE_DYING				1
#define	LIFE_DEAD				2
#define LIFE_RESPAWNABLE		3
#define LIFE_DISCARDBODY		4

//Player flags
#define	FL_ONGROUND				(1<<0)	// At rest / on the ground
#define FL_DUCKING				(1<<1)	// Player flag -- Player is fully crouched
#define	FL_WATERJUMP			(1<<3)	// player jumping out of water
#define FL_ONTRAIN				(1<<4) // Player is _controlling_ a train, so movement commands should be ignored on client during prediction.
#define FL_INRAIN				(1<<5)	// Indicates the entity is standing in rain
#define FL_FROZEN				(1<<6) // Player is frozen for 3rd person camera
#define FL_ATCONTROLS			(1<<7) // Player can't move, but keeps key inputs for controlling another entity
#define	FL_CLIENT				(1<<8)	// Is a player
#define FL_FAKECLIENT			(1<<9)	// Fake client, simulated server side; don't send network messages to them
#define	FL_INWATER				(1<<10)	// In water

//USERCMD BUTTONS
#define IN_ATTACK		(1 << 0) 
#define IN_JUMP			(1 << 1)
#define IN_DUCK			(1 << 2)
#define IN_FORWARD		(1 << 3)
#define IN_BACK			(1 << 4)
#define IN_USE			(1 << 5)
#define IN_CANCEL		(1 << 6)
#define IN_LEFT			(1 << 7)
#define IN_RIGHT		(1 << 8)
#define IN_MOVELEFT		(1 << 9)
#define IN_MOVERIGHT	(1 << 10)
#define IN_ATTACK2		(1 << 11)
#define IN_RUN			(1 << 12)
#define IN_RELOAD		(1 << 13)
#define IN_ALT1			(1 << 14)
#define IN_ALT2			(1 << 15)
#define IN_SCORE		(1 << 16)   // Used by client_panorama.dll for when scoreboard is held down
#define IN_SPEED		(1 << 17)	// Player is holding the speed key
#define IN_WALK			(1 << 18)	// Player holding walk key
#define IN_ZOOM			(1 << 19)	// Zoom key for HUD zoom
#define IN_WEAPON1		(1 << 20)	// weapon defines these bits
#define IN_WEAPON2		(1 << 21)	// weapon defines these bits
#define IN_BULLRUSH		(1 << 22)
#define IN_GRENADE1		(1 << 23)	// grenade 1
#define IN_GRENADE2		(1 << 24)	// grenade 2

#pragma endregion

#pragma region Classes

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
	MOVETYPE_MAX_BITS = 4
};

enum CSWeaponType
{
	WEAPONTYPE_KNIFE = 0,
	WEAPONTYPE_PISTOL,
	WEAPONTYPE_SUBMACHINEGUN,
	WEAPONTYPE_RIFLE,
	WEAPONTYPE_SHOTGUN,
	WEAPONTYPE_SNIPER_RIFLE,
	WEAPONTYPE_MACHINEGUN,
	WEAPONTYPE_C4,
	WEAPONTYPE_PLACEHOLDER,
	WEAPONTYPE_GRENADE,
	WEAPONTYPE_UNKNOWN
};
enum class CSGOHitboxID
{
	Head = 0,
	Neck,
	Pelvis,
	Stomach,
	LowerChest,
	Chest,
	UpperChest,
	RightThigh,
	LeftThigh,
	RightShin,
	LeftShin,
	RightFoot,
	LeftFoot,
	RightHand,
	LeftHand,
	RightUpperArm,
	RightLowerArm,
	LeftUpperArm,
	LeftLowerArm,
};
enum ClientFrameStage_t
{
	FRAME_UNDEFINED = -1,			// (haven't run any frames yet)
	FRAME_START,

	// A network packet is being recieved
	FRAME_NET_UPDATE_START,
	// Data has been received and we're going to start calling PostDataUpdate
	FRAME_NET_UPDATE_POSTDATAUPDATE_START,
	// Data has been received and we've called PostDataUpdate on all data recipients
	FRAME_NET_UPDATE_POSTDATAUPDATE_END,
	// We've received all packets, we can now do interpolation, prediction, etc..
	FRAME_NET_UPDATE_END,

	// We're about to start rendering the scene
	FRAME_RENDER_START,
	// We've finished rendering the scene.
	FRAME_RENDER_END
};
class VarMapEntry_t
{
public:
	unsigned short		type;
	unsigned short		m_bNeedsToInterpolate;	// Set to false when this var doesn't
												// need Interpolate() called on it anymore.
	void* data;
	void* watcher;
};

struct VarMapping_t
{
	VarMapping_t()
	{
		m_nInterpolatedEntries = 0;
	}

	VarMapEntry_t* m_Entries;
	int m_nInterpolatedEntries;
	float m_lastInterpolationTime;
};
struct player_info_t
{
	__int64         unknown;            //0x0000 
	union
	{
		__int64       steamID64;          //0x0008 - SteamID64
		struct
		{
			__int32     xuidLow;
			__int32     xuidHigh;
		};
	};
	char            name[128];        //0x0010 - Player Name
	int             userId;             //0x0090 - Unique Server Identifier
	char            szSteamID[20];      //0x0094 - STEAM_X:Y:Z
	char            pad_0x00A8[0x10];   //0x00A8
	unsigned long   iSteamID;           //0x00B8 - SteamID 
	char            friendsName[128];
	bool            fakeplayer;
	bool            ishltv;
	unsigned int    customFiles[4];
	unsigned char   filesDownloaded;
	//unsigned __int64 unknown;
	//unsigned __int64 xuid;
	//char name[32];

	//char unknown01[96];

	//int userID;
	//char guid[32 + 1];
	//__int32 friendsID;
	//char friendsName[32];
	//bool fakeplayer;
	//bool ishltv;
	//unsigned long customFiles[4];
	//unsigned char filesDownloaded;
	//char pad[304];
};

#pragma endregion

#pragma region ShitClasses

struct datamap_t;
class typedescription_t;

enum
{
	TD_OFFSET_NORMAL = 0,
	TD_OFFSET_PACKED = 1,

	// Must be last
	TD_OFFSET_COUNT,
};


class typedescription_t
{
public:
	int32_t fieldType; //0x0000
	char* fieldName; //0x0004
	int fieldOffset[TD_OFFSET_COUNT]; //0x0008
	int16_t fieldSize_UNKNWN; //0x0010
	int16_t flags_UNKWN; //0x0012
	char pad_0014[12]; //0x0014
	datamap_t* td; //0x0020
	char pad_0024[24]; //0x0024
}; //Size: 0x003C

struct datamap_t
{
	typedescription_t* dataDesc;
	int                    dataNumFields;
	char const* dataClassName;
	datamap_t* baseMap;

	bool                chains_validated;
	// Have the "packed" offsets been computed
	bool                packed_offsets_computed;
	int                    packed_size;
};


//class CCSWeaponInfo
//{
//public:
//	std::uint8_t pad_0x0000[0x4]; //0x0000
//	char* WeaponName; //0x0004
//	std::uint8_t pad_0x0008[0xC]; //0x0008
//	std::uint32_t MaxClip; //0x0014
//	std::uint8_t pad_0x0018[0x68]; //0x0018
//	char* AmmoName; //0x0080
//	char* AmmoName_2; //0x0084
//	char* HudName; //0x0088
//	char* WeaponId; //0x008C
//	std::uint8_t pad_0x0090[0x3C]; //0x0090
//	std::uint32_t type; //0x00CC
//	std::uint32_t WeaponPrice; //0x00D0
//	std::uint32_t WeaponReward; //0x00D4
//	std::uint8_t pad_0x00D8[0x14]; //0x00D8
//	std::uint8_t WeaponFullAuto; //0x00EC
//	std::uint8_t pad_0x00ED[0x3]; //0x00ED
//	std::uint32_t damage; //0x00F0
//	float armor_ratio; //0x00F4
//	std::uint32_t WeaponBullets; //0x00F8
//	float penetration; //0x00FC
//	std::uint8_t pad_0x0100[0x8]; //0x0100
//	float range; //0x0108
//	float range_modifier; //0x010C
//	std::uint8_t pad_0x0110[0x20]; //0x0110
//	float MaxSpeed; //0x0130
//	float MaxSpeedAlt; //0x0134
//	std::uint8_t pad_0x0138[0x108]; //0x0138
//};

// Created with ReClass.NET by KN4CK3R
class CCSWeaponInfo
{
public:
	char pad_0x0000[0x4]; //0x0000
	char* m_szWeaponName; //0x0004 
	char pad_0x0008[0xC]; //0x0008
	int32_t m_iMaxAmmo; //0x0014 
	char pad_0x0018[0xC]; //0x0018
	int32_t m_iMaxReserveAmmo; //0x0024 
	char pad_0x0028[0x4]; //0x0028
	char* m_szWeaponModelPath; //0x002C 
	char pad_0x0030[0x4]; //0x0030
	char* m_szDroppedWeaponModelPath; //0x0034 
	char pad_0x0038[0x48]; //0x0038
	char* m_szBulletType; //0x0080 
	char pad_0x0084[0x4]; //0x0084
	char* m_szLocalizationToken; //0x0088 
	char pad_0x008C[0x40]; //0x008C
	int32_t m_iWeaponType; //0x00CC 
	int32_t m_iWeaponPrice; //0x00D0 
	int32_t m_iWeaponReward; //0x00D4 
	char* m_szWeaponGroup; //0x00D8 
	char pad_0x00DC[0x10]; //0x00DC
	unsigned char m_nFullAuto; //0x00EC 
	char pad_0x00ED[0x3]; //0x00ED
	int32_t m_iDamage; //0x00F0 
	float m_flArmorRatio; //0x00F4 
	int32_t m_iAmmo;
	float m_flPenetration; //0x00F8
	char pad_0x00F8[0x8]; //0x00FC
	float m_flRange; //0x0108 
	float m_flRangeModifier; //0x010C 
	char pad_0x0110[0x10]; //0x0110
	unsigned char m_nSilencer; //0x0120 
	char pad_0x0121[0xF]; //0x0121
	float m_flMaxSpeed; //0x0130 
	float m_flMaxSpeedAlt; //0x0134 
	char pad_0x0138[0x4C]; //0x0138
	int32_t m_iRecoilSeed; //0x0184 
	char pad_0x0188[0x68]; //0x0188
	char* m_szWeaponTracesType; //0x01F0 
	char pad_0x01F4[0x638]; //0x01F4
};

class CGameTrace;
struct Ray_t;
typedef CGameTrace trace_t;
struct model_t
{
	void* fnHandle;               //0x0000 
	char    name[260];            //0x0004 
	__int32 nLoadFlags;             //0x0108 
	__int32 nServerCount;           //0x010C 
	__int32 type;                   //0x0110 
	__int32 flags;                  //0x0114 
	Vector  vecMins;                //0x0118 
	Vector  vecMaxs;                //0x0124 
	float   radius;                 //0x0130 
	char    pad[0x1C];              //0x0134
};

class IHandleEntity;
class ICollideable
{
public:
	virtual IHandleEntity* GetEntityHandle() = 0;
	virtual Vector& OBBMins() const = 0;
	virtual Vector& OBBMaxs() const = 0;
	virtual void world_space_trigger_bounds(Vector* mins, Vector* maxs) const = 0;
	virtual bool test_collision(const Ray_t& ray, unsigned int mask, trace_t& trace) = 0;
	virtual bool test_hitboxes(const Ray_t& ray, unsigned int mask, trace_t& trace) = 0;
	virtual int get_collision_model_index() = 0;
	virtual const model_t* get_collision_model() = 0;
	virtual Vector& get_collision_origin() const = 0;
	virtual Vector& get_collision_angles() const = 0;
	virtual const matrix& collision_to_world_transform() const = 0;
	virtual int get_solid() const = 0;
	virtual int get_solid_flags() const = 0;
	virtual void* get_client_unknown() = 0;
	virtual int get_collision_group() const = 0;
	virtual void world_space_surrounding_bounds(Vector* mins, Vector* maxs) = 0;
	virtual bool should_touch_trigger(int flags) const = 0;
	virtual const matrix* get_root_parent_to_world_transform() const = 0;
};

class IBaseCombatWeapon;
class IBasePlayer;
class IClientUnknown;

typedef unsigned short ModelInstanceHandle_t;
typedef unsigned char uint8;
class IClientRenderable
{
public:
	virtual IClientUnknown* GetIClientUnknown() = 0;
	virtual Vector& const GetRenderOrigin(void) = 0;
	virtual Vector& const GetRenderAngles(void) = 0;
	virtual bool ShouldDraw(void) = 0;
	virtual int GetRenderFlags(void) = 0; // ERENDERFLAGS_xxx
	virtual void Unused(void) const
	{
	}
	virtual HANDLE GetShadowHandle() const = 0;
	virtual HANDLE& RenderHandle() = 0;
	virtual const model_t* GetModel() const = 0;
	virtual int DrawModel(int flags, const int /*RenderableInstance_t*/& instance) = 0;
	virtual int GetBody() = 0;
	virtual void Getcolor_tModulation(float* color) = 0;
	virtual bool LODTest() = 0;
	virtual void SetupWeights(const matrix* pBoneToWorld, int nFlexWeightCount, float* pFlexWeights, float* pFlexDelayedWeights) = 0;
	virtual void DoAnimationEvents(void) = 0;
	virtual void* /*IPVSNotify*/ GetPVSNotifyInterface() = 0;
	virtual void GetRenderBounds(Vector& mins, Vector& maxs) = 0;
	virtual void GetRenderBoundsWorldspace(Vector& mins, Vector& maxs) = 0;
	virtual void GetShadowRenderBounds(Vector& mins, Vector& maxs, int /*ShadowType_t*/ shadowType) = 0;
	virtual bool ShouldReceiveProjectedTextures(int flags) = 0;
	virtual bool GetShadowCastDistance(float* pDist, int /*ShadowType_t*/ shadowType) const = 0;
	virtual bool GetShadowCastDirection(Vector* pDirection, int /*ShadowType_t*/ shadowType) const = 0;
	virtual bool IsShadowDirty() = 0;
	virtual void MarkShadowDirty(bool bDirty) = 0;
	virtual IClientRenderable* GetShadowParent() = 0;
	virtual IClientRenderable* FirstShadowChild() = 0;
	virtual IClientRenderable* NextShadowPeer() = 0;
	virtual int /*ShadowType_t*/ ShadowCastType() = 0;
	virtual void CreateModelInstance() = 0;
	virtual HANDLE GetModelInstance() = 0;
	virtual const matrix& RenderableToWorldTransform() = 0;
	virtual int LookupAttachment(const char* pAttachmentName) = 0;
	virtual bool GetAttachment(int number, Vector& origin, Vector& angles) = 0;
	virtual bool GetAttachment(int number, matrix& matrix) = 0;
	virtual float* GetRenderClipPlane(void) = 0;
	virtual int GetSkin() = 0;
	virtual void OnThreadedDrawSetup() = 0;
	virtual bool UsesFlexDelayedWeights() = 0;
	virtual void RecordToolMessage() = 0;
	virtual bool ShouldDrawForSplitScreenUser(int nSlot) = 0;
	virtual uint8 OverrideAlphaModulation(uint8 nAlpha) = 0;
	virtual uint8 OverrideShadowAlphaModulation(uint8 nAlpha) = 0;
};
class IClientNetworkable
{
public:
	virtual IClientUnknown* GetIClientUnknown() = 0;
	virtual void             Release() = 0;
	virtual ClientClass* GetClientClass() = 0;
	virtual void             NotifyShouldTransmit(int state) = 0;
	virtual void             OnPreDataChanged(int updateType) = 0;
	virtual void             OnDataChanged(int updateType) = 0;
	virtual void             PreDataUpdate(int updateType) = 0;
	virtual void             PostDataUpdate(int updateType) = 0;
	virtual void             __unkn(void) = 0;
	virtual bool             IsDormant(void) = 0;
	virtual int              EntIndex(void) const = 0;
	virtual void             ReceiveMessage() = 0;
	virtual void* GetDataTableBasePtr() = 0;
	virtual void             SetDestroyedOnRecreateEntities(void) = 0;
};

class IClientThinkable;
using CBaseHandle = uint32_t;
class IHandleEntity
{
public:
	virtual ~IHandleEntity() = default;
	virtual void SetRefEHandle(const CBaseHandle& handle) = 0;
	virtual const CBaseHandle& GetRefEHandle() const = 0;
};



class IClientUnknown : public IHandleEntity
{
public:
	virtual ICollideable* GetCollideable() = 0;
	virtual IClientNetworkable* GetClientNetworkable() = 0;
	virtual IClientRenderable* GetClientRenderable() = 0;
	virtual IBasePlayer* GetIClientEntity() = 0;
	virtual IBasePlayer* GetBaseEntity() = 0;
	virtual IClientThinkable* GetClientThinkable() = 0;
};

class IClientThinkable
{
public:
	virtual IClientUnknown* GetIClientUnknown() = 0;
	virtual void				ClientThink() = 0;
	virtual void* GetThinkHandle() = 0;
	virtual void				SetThinkHandle(void* hThink) = 0;
	virtual void				Release() = 0;
};
struct studiohdr_t;
struct mstudiohitboxset_t;
struct mstudiobbox_t
{
	int bone;
	int group; // intersection group
	Vector bbmin; // bounding box 
	Vector bbmax;
	int hitboxnameindex; // offset to the name of the hitbox.
	Vector rotation;
	float radius;
	int pad2[4];

	const char* getHitboxName()
	{
		if (hitboxnameindex == 0)
			return "";

		return ((char*)this) + hitboxnameindex;
	}
};

enum DataUpdateType_t
{
	DATA_UPDATE_CREATED = 0,
	//    DATA_UPDATE_ENTERED_PVS,
	DATA_UPDATE_DATATABLE_CHANGED
	//    DATA_UPDATE_LEFT_PVS,
	//DATA_UPDATE_DESTROYED,
};

class CAnimationLayer
{
public:
	bool            m_bClientBlend;            //0x00
	float            m_flBlendIn;                //0x04
	void* m_pStudioHdr;                //0x08
	int                m_nDispatchedSrc;            //0x0C
	int                m_nDispatchedDst;            //0x10
	std::uintptr_t  m_iOrder;                    //0x14
	std::uintptr_t  m_nSequence;                //0x18
	float            m_flPrevCycle;            //0x1C
	float            m_flWeight;                //0x20
	float            m_flWeightDeltaRate;        //0x24
	float            m_flPlaybackRate;            //0x28
	float            m_flCycle;                //0x2C
	void* m_pOwner;                    //0x30
	int                m_nInvalidatePhysicsBits;    //0x34
};
//class CAnimationLayer
//{
//	char pad_0000[20] = {};
//public:
//	uint32_t order{};
//	uint32_t sequence{};
//	float prev_cycle{};
//	float weight{};
//	float weight_delta_rate{};
//	float playback_rate{};
//	float cycle{};
//	void* owner{};
//private:
//	char pad_0038[4] = {};
//};
class CCSGOPlayerAnimState
{
public:
	char pad[3];
	char bUnknown; //0x4
	char pad2[91];
	void* m_pBaseEntity; //0x60
	void* m_pActiveWeapon; //0x64
	void* m_pLastActiveWeapon; //0x68
	float m_flLastClientSideAnimationUpdateTime; //0x6C
	int m_iLastClientSideAnimationUpdateFramecount; //0x70
	float m_flUpdateTimeDelta; //0x74
	float m_flEyeYaw; //0x78
	float m_flPitch; //0x7C
	float m_flGoalFeetYaw; //0x80
	float m_flCurrentFeetYaw; //0x84
	float m_flCurrentTorsoYaw; //0x88
	float m_flUnknownVelocityLean; //0x8C //changes when moving/jumping/hitting ground
	float m_flLeanAmount; //0x90
	char pad4[4]; //NaN
	float m_flFeetCycle; //0x98 0 to 1
	float m_flFeetYawRate; //0x9C 0 to 1
	float m_fUnknown2;
	float m_fDuckAmount; //0xA4
	float m_fLandingDuckAdditiveSomething; //0xA8
	float m_fUnknown3; //0xAC
	Vector m_vOrigin; //0xB0, 0xB4, 0xB8
	Vector m_vLastOrigin; //0xBC, 0xC0, 0xC4
	float m_vVelocityX; //0xC8
	float m_vVelocityY; //0xCC
	char pad5[4];
	float m_flUnknownFloat1; //0xD4 Affected by movement and direction
	char pad6[8];
	float m_flUnknownFloat2; //0xE0 //from -1 to 1 when moving and affected by direction
	float m_flUnknownFloat3; //0xE4 //from -1 to 1 when moving and affected by direction
	float m_unknown; //0xE8
	float m_velocity; //0xEC
	float flUpVelocity; //0xF0
	float m_flSpeedNormalized; //0xF4 //from 0 to 1
	float m_flFeetSpeedForwardsOrSideWays; //0xF8 //from 0 to 2. something is 1 when walking, 2.something when running, 0.653 when crouch walking
	float m_flFeetSpeedUnknownForwardOrSideways; //0xFC //from 0 to 3. something
	float m_flTimeSinceStartedMoving; //0x100
	float m_flTimeSinceStoppedMoving; //0x104
	bool m_bOnGround; //0x108
	bool m_bInHitGroundAnimation; //0x109
	char pad7[10];
	float m_flLastOriginZ; //0x114
	float m_flHeadHeightOrOffsetFromHittingGroundAnimation; //0x118 from 0 to 1, is 1 when standing
	float m_flStopToFullRunningFraction; //0x11C from 0 to 1, doesnt change when walking or crouching, only running
	char pad8[4]; //NaN
	float m_flMovingFraction; //0x124 affected while jumping and running, or when just jumping, 0 to 1
	char pad9[4]; //NaN
	float m_flUnknown3;
	char pad10[528];

	float AbsYaw()
	{
		return *(float*)((DWORD)this + 0x80);
	}
};

class CBoneAccessor
{

public:

	inline matrix* GetBoneArrayForWrite()
	{
		return m_pBones;
	}

	inline void SetBoneArrayForWrite(matrix* bone_array)
	{
		m_pBones = bone_array;
	}

	alignas(16) matrix* m_pBones;
	int32_t m_ReadableBones; // Which bones can be read.
	int32_t m_WritableBones; // Which bones can be written.
};
#pragma endregion

#define offset(func, type, offset) type& func { return *reinterpret_cast<type*>(reinterpret_cast<uintptr_t>(this) + offset); }

#define NO_INTERP 8
#define EFL_DIRTY_ABSVELOCITY 0x100

class c_studio_hdr;
class Quaternion;

class CBaseAnimating
{
public:
	c_studio_hdr* GetModelPtr()
	{
		return *(c_studio_hdr**)((DWORD)this + 0x293C);
	}
};

enum InvalidatePhysicsBits_t
{
	POSITION_CHANGED = 0x1,
	ANGLES_CHANGED = 0x2,
	VELOCITY_CHANGED = 0x4,
	ANIMATION_CHANGED = 0x8,		// Means cycle has changed, or any other event which would cause render-to-texture shadows to need to be rerendeded
	BOUNDS_CHANGED = 0x10,		// Means render bounds have changed, so shadow decal projection is required, etc.
	SEQUENCE_CHANGED = 0x20,		// Means sequence has changed, only interesting when surrounding bounds depends on sequence																				
};

class CUserCmd;
class IBaseViewModel;

class IBasePlayer : public IClientUnknown, public IClientRenderable, public IClientNetworkable, public IClientThinkable, public CBaseAnimating
{
public:
	float& m_flNextAttack();
	Vector& GetMins();
	Vector& GetMaxs();
	HANDLE GetObserverTargetHandle();
	Vector& GetAbsVelocity();
	int& GetGroundEntity();
	int GetIndex() {
		return EntIndex();
	}
	void SetupBonesFix();
	void SetLowerBodyYaw(float value);
	void InvalidateBoneCache();
	void SetVelocity(Vector velocity);
	void SetFlags(int flags);
	float& HealthShotBoostExpirationTime();
	void ResetAnimationState(CCSGOPlayerAnimState* state);
	void CreateAnimationState(CCSGOPlayerAnimState* state);
	void UpdateAnimationState(CCSGOPlayerAnimState* state, Vector angle);
	int* GetButtons();
	int& GetButtonLast();
	int& GetButtonPressed();
	int& GetButtonReleased();
	int GetButtonDisabled();
	int GetButtonForced();
	void UpdateCollisionBounds();
	void SetSequence(int iSequence);
	void StudioFrameAdvance();
	int PostThink();
	int* GetNextThinkTick();
	Vector GetBaseVelocity();
	int& GetTickBasePtr(void);
	bool SelectItem(const char* string, int sub_type = 0);
	//CHandle<IBasePlayer> GetVehicle();
	void StandardBlendingRules(c_studio_hdr* hdr, Vector* pos, Quaternion* q, float curtime, int boneMask);
	void BuildTransformations(c_studio_hdr* hdr, Vector* pos, Quaternion* q, const matrix& cameraTransform, int boneMask, BYTE* computed);
	float GetLastSeenTime();
	bool BadMatrix();

	template<class T>
	T GetValue(const int offset)
	{
		return *reinterpret_cast<T*>(reinterpret_cast<std::uintptr_t>(this) + offset);
	}

	int ragdoll();
	void SetModelIndex(int model);
	int m_nModelIndex();


	int& get_take_damage() {
		return *(int*)((DWORD)this + 0x27C);
	}


	float GetC4Blow()
	{
		return *(float*)((DWORD)this + 0x297C);
	}
	float GetBombTicking()
	{
		return *(float*)((DWORD)this + 0x2970);
	}

	std::string GetName();
	bool PhysicsRunThink(int unk01);

	int GetNumAnimOverlays()
	{
		return *(int*)((DWORD)this + 0x298C);
	}

	int GetMoveType()
	{
		return *(int*)((DWORD)this + 0x25C);
	}

	CAnimationLayer* GetAnimOverlays()
	{
		return *(CAnimationLayer**)((DWORD)this + 0x2980);
	}

	CAnimationLayer* GetAnimOverlay(int index)
	{
		if (index < 15)
			return &GetAnimOverlays()[index];
		else
			return nullptr;
	}

	Vector GetRagdollPos();

	bool& GetClientSideAnims();

	float GetDSYDelta();

	Vector* EyeAngles()
	{
		typedef Vector* (__thiscall* OriginalFn)(IBasePlayer*);
		return getvfunc<OriginalFn>(this, 164)(this);
	}

	void PreThink()
	{
		typedef void(__thiscall* OriginalFn)(PVOID);
		getvfunc<OriginalFn>(this, 317)(this);
	}

	//OFFSET(get_model_ptr, CStudioHdr*, 0x293C)

	void Think()
	{
		typedef void(__thiscall* OriginalFn)(PVOID);
		getvfunc<OriginalFn>(this, 138)(this);
	}
	int32_t& GetThinkTick()
	{
		return *(int32_t*)((DWORD)this + 0x40);
	}
	void UnkFunc();

	void SetAbsAngles(const Vector& angles);
	void SetAbsOrigin(const Vector& origin);
	float_t m_surfaceFriction();
	void SetLocalViewAngles(Vector angle);
	datamap_t* GetDataDescMap();
	datamap_t* GetPredDescMap();
	std::array<float, 24>& m_flPoseParameter();
	float& GetFlashDuration();
	bool IsFlashed();
	Vector GetPunchAngle();
	Vector& GetPunchAngleVel();

	Vector* GetPunchAnglePtr();
	Vector GetViewPunchAngle();
	Vector* GetViewPunchAnglePtr();

	void UpdateClientSideAnimation()
	{
		typedef void(__thiscall* o_updateClientSideAnimation)(void*);
		getvfunc<o_updateClientSideAnimation>(this, 223)(this);
	}

	IBaseCombatWeapon* GetWeapon();
	int GetArmor();

	offset(get_most_recent_model_bone_counter(), uint32_t, 0x2690)
		offset(get_last_bone_setup_time(), float, 0x2924)

		bool isAlive()
	{
		if (!this)
			return false;
		BYTE lifestate = *(BYTE*)((DWORD)this + 0x025F);
		return (lifestate == 0);
	}

	bool IsValid();

	uint32_t& GetOcclusionFlags()
	{
		return *(uint32_t*)((DWORD)this + 0xA28);
	}

	uint32_t& GetOcclusionFramecount()
	{
		return *(uint32_t*)((DWORD)this + 0xA30);
	}

	uint32_t& GetReadableBones()
	{
		return *(uint32_t*)((DWORD)this + 0x26AC);
	}

	uint32_t& GetWritableBones()
	{
		return *(uint32_t*)((DWORD)this + 0x26B0);
	}

	matrix*& GetBoneArrayForWrite()
	{
		return *(matrix**)((DWORD)this + 0x26A8);
	}

	void InvalidatePhysicsRecursive(int32_t flags);
	CCSGOPlayerAnimState* GetPlayerAnimState();
	void SetAnimState(CCSGOPlayerAnimState* anims);

	int GetFlags()
	{
		return *(int*)((DWORD)this + 0x104);
	}

	int& GetFlagsPtr()
	{
		return *(int*)((DWORD)this + 0x104);
	}
	offset(get_unknown_ptr(), uint8_t, 0x274)
		int& GetEFlags();
	int& GetEffects();
	int GetHealth()
	{
		if (!this)
			return -1;
		return *(int*)((DWORD)this + 0x100);
	}
	CUtlVector<matrix>& GetBoneCache();
	void SetBoneCache(matrix* m);
	void ShootPos(Vector* vec);
	void UpdateVisibilityAllEntities();
	Vector GetEyePosition();

	void DrawServerHitboxes();
	uint8_t* GetServerEdict();
	float& GetVelocityModifier();
	Vector GetEyeAngles();
	int GetSequence();
	float& GetFallVelocity();
	Vector* GetEyeAnglesPointer();
	bool IsPlayer();
	bool IsWeapon();
	bool InDangerzone() const;
	bool IsEnemy() const;

	int GetTeam();
	player_info_t GetPlayerInfo();
	Vector GetOrigin()
	{
		if (!this)
			return Vector(0, 0, 0);
		return *(Vector*)((DWORD)this + 0x138);
	}

	Vector& GetVecViewOffset();

	float& GetDuckSpeed();

	float& GetDuckAmount();

	void SetObserverMode(int value);

	int32_t GetObserverMode();

	Vector& GetVelocity()
	{
		return *(Vector*)((DWORD)this + 0x114);
	}

	int GetShootsFired()
	{
		return *reinterpret_cast<int*>((DWORD)this + 0xA370);
	}

	bool IsHostage();
	float& GetLBY();
	bool IsChicken();
	Vector& GetAbsAngles();
	float m_angAbsRotation();
	Vector GetAbsOrigin();
	bool HasHelmet();
	bool HeavyArmor();
	Vector GetAbsOriginVec();
	int GetSequenceActivity(int sequence);
	matrix& GetrgflCoordinateFrame();
	IBaseViewModel* GetViewModel();
	std::vector< IBaseCombatWeapon* > GetWeapons();
	float GetOldSimulationTime();
	void SetCurrentCommand(CUserCmd* cmd);
	int GetTickBase();
	int* GetImpulse();
	float& GetCycle();
	float& GetAnimtime();
	bool IsScoped();

	Vector WorldSpaceCenter()
	{
		Vector Max = this->GetCollideable()->OBBMaxs() + this->GetOrigin();
		Vector Min = this->GetCollideable()->OBBMaxs() + this->GetOrigin();

		Vector Size = Max - Min;

		Size /= 2;

		Size += Min;

		return Size;
	}

	bool SetupBones(matrix* pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime);
	float& GetSimulationTime();

	float& GetSpawnTime()
	{
		return *(float*)((DWORD)this + 0xA370);
	}

	Vector GetBonePos(matrix* mat, int i)
	{
		if (!mat)
			return Vector(0, 0, 0);

		return Vector(mat[i][0][3], mat[i][1][3], mat[i][2][3]);
	}


	/*Vector GetBonePos(int i)
	{
		matrix boneMatrix[128];
		if (this->SetupBones(boneMatrix, 128, 0x00000100, GetSimulationTime()))
		{
			return Vector(boneMatrix[i][0][3], boneMatrix[i][1][3], boneMatrix[i][2][3]);
		}
		return Vector(0, 0, 0);
	}*/

	VarMapping_t* GetVarMap()
	{
		return reinterpret_cast<VarMapping_t*>((DWORD)this + 0x24);
	}

	Vector GetNetworkOrigin()
	{
		return *(Vector*)((DWORD)this + 0x140);
	}

	bool HasGunGameImmunity();
	int GetChokedPackets();
};

class IBaseViewModel : public IBasePlayer {
public:
	float& GetAnimtime();
	float& GetCycle();
};

class ClassIdManager
{
public:
	ClassIdManager();
private:
	int GetClassID(const char* classname);
public:
	int CAI_BaseNPC = -1;
	int CAK47 = -1;
	int CBaseAnimating = -1;
	int CBaseAnimatingOverlay = -1;
	int CBaseAttributableItem = -1;
	int CBaseButton = -1;
	int CBaseCombatCharacter = -1;
	int CBaseCombatWeapon = -1;
	int CBaseCSGrenade = -1;
	int CBaseCSGrenadeProjectile = -1;
	int CBaseDoor = -1;
	int CBaseEntity = -1;
	int CBaseFlex = -1;
	int CBaseGrenade = -1;
	int CBaseParticleEntity = -1;
	int CBasePlayer = -1;
	int CBasePropDoor = -1;
	int CBaseTeamObjectiveResource = -1;
	int CBaseTempEntity = -1;
	int CBaseToggle = -1;
	int CBaseTrigger = -1;
	int CBaseViewModel = -1;
	int CBaseVPhysicsTrigger = -1;
	int CBaseWeaponWorldModel = -1;
	int CBeam = -1;
	int CBeamSpotlight = -1;
	int CBoneFollower = -1;
	int CBRC4Target = -1;
	int CBreachCharge = -1;
	int CBreachChargeProjectile = -1;
	int CBreakableProp = -1;
	int CBreakableSurface = -1;
	int CC4 = -1;
	int CCascadeLight = -1;
	int CChicken = -1;
	int Ccolor_tCorrection = -1;
	int Ccolor_tCorrectionVolume = -1;
	int CCSGameRulesProxy = -1;
	int CCSPlayer = -1;
	int CCSPlayerResource = -1;
	int CCSRagdoll = -1;
	int CCSTeam = -1;
	int CDangerZone = -1;
	int CDangerZoneController = -1;
	int CDEagle = -1;
	int CDecoyGrenade = -1;
	int CDecoyProjectile = -1;
	int CDrone = -1;
	int CDronegun = -1;
	int CDynamicLight = -1;
	int CDynamicProp = -1;
	int CEconEntity = -1;
	int CEconWearable = -1;
	int CEmbers = -1;
	int CEntityDissolve = -1;
	int CEntityFlame = -1;
	int CEntityFreezing = -1;
	int CEntityParticleTrail = -1;
	int CEnvAmbientLight = -1;
	int CEnvDetailController = -1;
	int CEnvDOFController = -1;
	int CEnvGasCanister = -1;
	int CEnvParticleScript = -1;
	int CEnvProjectedTexture = -1;
	int CEnvQuadraticBeam = -1;
	int CEnvScreenEffect = -1;
	int CEnvScreenOverlay = -1;
	int CEnvTonemapController = -1;
	int CEnvWind = -1;
	int CFEPlayerDecal = -1;
	int CFireCrackerBlast = -1;
	int CFireSmoke = -1;
	int CFireTrail = -1;
	int CFish = -1;
	int CFists = -1;
	int CFlashbang = -1;
	int CFogController = -1;
	int CFootstepControl = -1;
	int CFunc_Dust = -1;
	int CFunc_LOD = -1;
	int CFuncAreaPortalWindow = -1;
	int CFuncBrush = -1;
	int CFuncConveyor = -1;
	int CFuncLadder = -1;
	int CFuncMonitor = -1;
	int CFuncMoveLinear = -1;
	int CFuncOccluder = -1;
	int CFuncReflectiveGlass = -1;
	int CFuncRotating = -1;
	int CFuncSmokeVolume = -1;
	int CFuncTrackTrain = -1;
	int CGameRulesProxy = -1;
	int CGrassBurn = -1;
	int CHandleTest = -1;
	int CHEGrenade = -1;
	int CHostage = -1;
	int CHostageCarriableProp = -1;
	int CIncendiaryGrenade = -1;
	int CInferno = -1;
	int CInfoLadderDismount = -1;
	int CInfoMapRegion = -1;
	int CInfoOverlayAccessor = -1;
	int CItem_Healthshot = -1;
	int CItemCash = -1;
	int CItemDogtags = -1;
	int CKnife = -1;
	int CKnifeGG = -1;
	int CLightGlow = -1;
	int CMaterialModifyControl = -1;
	int CMelee = -1;
	int CMolotovGrenade = -1;
	int CMolotovProjectile = -1;
	int CMovieDisplay = -1;
	int CParadropChopper = -1;
	int CParticleFire = -1;
	int CParticlePerformanceMonitor = -1;
	int CParticleSystem = -1;
	int CPhysBox = -1;
	int CPhysBoxMultiplayer = -1;
	int CPhysicsProp = -1;
	int CPhysicsPropMultiplayer = -1;
	int CPhysMagnet = -1;
	int CPhysPropAmmoBox = -1;
	int CPhysPropLootCrate = -1;
	int CPhysPropRadarJammer = -1;
	int CPhysPropWeaponUpgrade = -1;
	int CPlantedC4 = -1;
	int CPlasma = -1;
	int CPlayerResource = -1;
	int CPointCamera = -1;
	int CPointCommentaryNode = -1;
	int CPointWorldText = -1;
	int CPoseController = -1;
	int CPostProcessController = -1;
	int CPrecipitation = -1;
	int CPrecipitationBlocker = -1;
	int CPredictedViewModel = -1;
	int CProp_Hallucination = -1;
	int CPropCounter = -1;
	int CPropDoorRotating = -1;
	int CPropJeep = -1;
	int CPropVehicleDriveable = -1;
	int CRagdollManager = -1;
	int CRagdollProp = -1;
	int CRagdollPropAttached = -1;
	int CRopeKeyframe = -1;
	int CSCAR17 = -1;
	int CSceneEntity = -1;
	int CSensorGrenade = -1;
	int CSensorGrenadeProjectile = -1;
	int CShadowControl = -1;
	int CSlideshowDisplay = -1;
	int CSmokeGrenade = -1;
	int CSmokeGrenadeProjectile = -1;
	int CSmokeStack = -1;
	int CSnowball = -1;
	int CSnowballPile = -1;
	int CSnowballProjectile = -1;
	int CSpatialEntity = -1;
	int CSpotlightEnd = -1;
	int CSprite = -1;
	int CSpriteOriented = -1;
	int CSpriteTrail = -1;
	int CStatueProp = -1;
	int CSteamJet = -1;
	int CSun = -1;
	int CSunlightShadowControl = -1;
	int CSurvivalSpawnChopper = -1;
	int CTablet = -1;
	int CTeam = -1;
	int CTeamplayRoundBasedRulesProxy = -1;
	int CTEArmorRicochet = -1;
	int CTEBaseBeam = -1;
	int CTEBeamEntPoint = -1;
	int CTEBeamEnts = -1;
	int CTEBeamFollow = -1;
	int CTEBeamLaser = -1;
	int CTEBeamPoints = -1;
	int CTEBeamRing = -1;
	int CTEBeamRingPoint = -1;
	int CTEBeamSpline = -1;
	int CTEBloodSprite = -1;
	int CTEBloodStream = -1;
	int CTEBreakModel = -1;
	int CTEBSPDecal = -1;
	int CTEBubbles = -1;
	int CTEBubbleTrail = -1;
	int CTEClientProjectile = -1;
	int CTEDecal = -1;
	int CTEDust = -1;
	int CTEDynamicLight = -1;
	int CTEEffectDispatch = -1;
	int CTEEnergySplash = -1;
	int CTEExplosion = -1;
	int CTEFireBullets = -1;
	int CTEFizz = -1;
	int CTEFootprintDecal = -1;
	int CTEFoundryHelpers = -1;
	int CTEGaussExplosion = -1;
	int CTEGlowSprite = -1;
	int CTEImpact = -1;
	int CTEKillPlayerAttachments = -1;
	int CTELargeFunnel = -1;
	int CTEMetalSparks = -1;
	int CTEMuzzleFlash = -1;
	int CTEParticleSystem = -1;
	int CTEPhysicsProp = -1;
	int CTEPlantBomb = -1;
	int CTEPlayerAnimEvent = -1;
	int CTEPlayerDecal = -1;
	int CTEProjectedDecal = -1;
	int CTERadioIcon = -1;
	int CTEShatterSurface = -1;
	int CTEShowLine = -1;
	int CTesla = -1;
	int CTESmoke = -1;
	int CTESparks = -1;
	int CTESprite = -1;
	int CTESpriteSpray = -1;
	int CTest_ProxyToggle_Networkable = -1;
	int CTestTraceline = -1;
	int CTEWorldDecal = -1;
	int CTriggerPlayerMovement = -1;
	int CTriggerSoundOperator = -1;
	int CVGuiScreen = -1;
	int CVoteController = -1;
	int CWaterBullet = -1;
	int CWaterLODControl = -1;
	int CWeaponAug = -1;
	int CWeaponAWP = -1;
	int CWeaponBaseItem = -1;
	int CWeaponBizon = -1;
	int CWeaponCSBase = -1;
	int CWeaponCSBaseGun = -1;
	int CWeaponCycler = -1;
	int CWeaponElite = -1;
	int CWeaponFamas = -1;
	int CWeaponFiveSeven = -1;
	int CWeaponG3SG1 = -1;
	int CWeaponGalil = -1;
	int CWeaponGalilAR = -1;
	int CWeaponGlock = -1;
	int CWeaponHKP2000 = -1;
	int CWeaponM249 = -1;
	int CWeaponM3 = -1;
	int CWeaponM4A1 = -1;
	int CWeaponMAC10 = -1;
	int CWeaponMag7 = -1;
	int CWeaponMP5Navy = -1;
	int CWeaponMP7 = -1;
	int CWeaponMP9 = -1;
	int CWeaponNegev = -1;
	int CWeaponNOVA = -1;
	int CWeaponP228 = -1;
	int CWeaponP250 = -1;
	int CWeaponP90 = -1;
	int CWeaponSawedoff = -1;
	int CWeaponSCAR20 = -1;
	int CWeaponScout = -1;
	int CWeaponSG550 = -1;
	int CWeaponSG552 = -1;
	int CWeaponSG556 = -1;
	int CWeaponSSG08 = -1;
	int CWeaponTaser = -1;
	int CWeaponTec9 = -1;
	int CWeaponTMP = -1;
	int CWeaponUMP45 = -1;
	int CWeaponUSP = -1;
	int CWeaponXM1014 = -1;
	int CWorld = -1;
	int CWorldVguiText = -1;
	int DustTrail = -1;
	int MovieExplosion = -1;
	int ParticleSmokeGrenade = -1;
	int RocketTrail = -1;
	int SmokeTrail = -1;
	int SporeExplosion = -1;
	int SporeTrail = -1;
};

extern ClassIdManager* ClassId;

enum weapons
{
	weapon_deagle = 1,
	weapon_elite = 2,
	weapon_fiveseven = 3,
	weapon_glock = 4,
	weapon_ak47 = 7,
	weapon_aug = 8,
	weapon_awp = 9,
	weapon_famas = 10,
	weapon_g3sg1 = 11,
	weapon_galilar = 13,
	weapon_m249 = 14,
	weapon_m4a1 = 16,
	weapon_mac10 = 17,
	weapon_p90 = 19,
	weapon_ump = 24,
	weapon_mp5sd = 23,
	weapon_xm1014 = 25,
	weapon_bizon = 26,
	weapon_mag7 = 27,
	weapon_negev = 28,
	weapon_sawedoff = 29,
	weapon_tec9 = 30,
	weapon_taser = 31,
	weapon_hkp2000 = 32,
	weapon_mp7 = 33,
	weapon_mp9 = 34,
	weapon_nova = 35,
	weapon_p250 = 36,
	weapon_scar20 = 38,
	weapon_sg556 = 39,
	weapon_ssg08 = 40,
	weapon_knife = 42,
	weapon_flashbang = 43,
	weapon_hegrenade = 44,
	weapon_smokegrenade = 45,
	weapon_molotov = 46,
	weapon_decoy = 47,
	weapon_incgrenade = 48,
	weapon_c4 = 49,
	weapon_knife_t = 59,
	weapon_m4a1_silencer = 60,
	weapon_usp_silencer = 61,
	weapon_cz75a = 63,
	weapon_revolver = 64,
	weapon_bayonet = 500,
	weapon_knife_flip = 505,
	weapon_knife_gut = 506,
	weapon_knife_karambit = 507,
	weapon_knife_m9_bayonet = 508,
	weapon_knife_tactical = 509,
	weapon_knife_falchion = 512,
	weapon_knife_survival_bowie = 514,
	weapon_knife_butterfly = 515,
	weapon_knife_push = 516
};

enum
{
	deagle = 1,
	p2000 = 32,
	fiveseven = 3,
	glock = 4,
	dualelites = 2,
	ak = 7,
	aug = 8,
	awp = 9,
	famas = 10,
	g3sg1 = 11,
	galil = 13,
	p250 = 36,
	tec9 = 30,
	m249 = 14,
	m4 = 16,
	mac10 = 17,
	p90 = 19,
	ump45 = 24,
	ppbizon = 26,
	negev = 28,
	mp7 = 33,
	mp9 = 34,
	scar30 = 38,
	sg553 = 39,
	ssg08 = 40,
	goldknife = 41,
	ctknife = 42,
	tknife = 59,
	flashgren = 43,
	hegren = 44,
	smoke = 45,
	molotov = 46,
	incendiary = 48,
	decoygren = 47,
	bomb = 49,
	nova = 35,
	xm1014 = 25,
	sawedoff = 29,
	mag7 = 27,
	zeus = 31
};
enum eWeaponType1 {
	WEAPON_DEAGLE = 1,
	WEAPON_DUALBERETTA = 2,
	WEAPON_FIVESEVEN = 3,
	WEAPON_GLOCK = 4,
	WEAPON_AK47 = 7,
	WEAPON_AUG = 8,
	WEAPON_AWP = 9,
	WEAPON_FAMAS = 10,
	WEAPON_G3SG1 = 11,
	WEAPON_GALIL = 13,
	WEAPON_M249 = 14,
	WEAPON_M4A1 = 16,
	WEAPON_MAC10 = 17,
	WEAPON_P90 = 19,
	WEAPON_MP5SD = 23,
	WEAPON_UMP45 = 24,
	WEAPON_XM1014 = 25,
	WEAPON_BIZON = 26,
	WEAPON_MAG7 = 27,
	WEAPON_NEGEV = 28,
	WEAPON_SAWEDOFF = 29,
	WEAPON_TEC9 = 30,
	WEAPON_ZEUSX27 = 31,
	WEAPON_USP = 32,
	WEAPON_MP7 = 33,
	WEAPON_MP9 = 34,
	WEAPON_NOVA = 35,
	WEAPON_P250 = 36,
	WEAPON_SCAR20 = 38,
	WEAPON_SG553 = 39,
	WEAPON_SSG08 = 40,
	WEAPON_KNIFE_GG = 41,
	WEAPON_KNIFE = 42,
	WEAPON_FLASHBANG = 43,
	WEAPON_HEGRENADE = 44,
	WEAPON_SMOKEGRENADE = 45,
	WEAPON_MOLOTOV = 46,
	WEAPON_DECOY = 47,
	WEAPON_INC = 48,
	WEAPON_C4 = 49,
	WEAPON_KNIFE_T = 59,
	WEAPON_M4A1S = 60,
	WEAPON_USPS = 61,
	WEAPON_CZ75 = 63,
	WEAPON_REVOLVER = 64,
	WEAPON_KNIFE_BAYONET = 500,
	WEAPON_KNIFE_FLIP = 505,
	WEAPON_KNIFE_GUT = 506,
	WEAPON_KNIFE_KARAMBIT = 507,
	WEAPON_KNIFE_M9_BAYONET = 508,
	WEAPON_KNIFE_TACTICAL = 509,
	WEAPON_KNIFE_FALCHION = 512,
	WEAPON_KNIFE_SURVIVAL_BOWIE = 514,
	WEAPON_KNIFE_BUTTERFLY = 515,
	WEAPON_KNIFE_PUSH = 516
};

class IBaseCombatWeapon
{
public:
	bool CanFire();
	bool InReload();
	float& NextSecondaryAttack();
	int GetItemDefinitionIndex();
	float NextPrimaryAttack();
	float GetLastShotTime();
	int GetAmmo(bool second);
	int32_t GetZoomLevel();

	bool m_bReloadVisuallyComplete();
	float& GetAccuracyPenalty();
	float& GetRecoilIndex();
	float LastShotTime();
	bool StartedArming();
	float GetPostponeFireReadyTime();
	void SetPostPoneTime(float asdasdasd);
	bool IsCanScope()
	{
		if (!this)
			return false;

		int WeaponId = this->GetItemDefinitionIndex();

		return WeaponId == WEAPON_AWP || WeaponId == WEAPON_SCAR20 || WeaponId == WEAPON_G3SG1 || WeaponId == WEAPON_SSG08 || WeaponId == WEAPON_SG553 || WeaponId == WEAPON_AUG;
	}

	bool IsAuto()
	{
		if (this == nullptr)
			return false;
		return this->GetItemDefinitionIndex() == WEAPON_SCAR20 || this->GetItemDefinitionIndex() == WEAPON_G3SG1;
	}

	bool IsAWP()
	{
		if (this == nullptr)
			return false;
		int WeaponId = this->GetItemDefinitionIndex();

		return WeaponId == WEAPON_AWP;
	}
	float GetInaccuracy()
	{
		if (!this)
			return 0.f;

		typedef float(__thiscall* OriginalFn)(void*);
		return getvfunc< OriginalFn >(this, 482)(this);
	}

	float GetSpread()
	{
		if (!this)
			return 0.f;

		typedef float(__thiscall* OriginalFn)(void*);
		return getvfunc< OriginalFn >(this, 452)(this);
	}

	CCSWeaponInfo* GetCSWpnData();
	int32_t WeaponMode();
	void UpdateAccuracyPenalty()
	{
		if (!this) return;

		typedef void(__thiscall* oUpdateAccuracyPenalty)(PVOID);
		return getvfunc<oUpdateAccuracyPenalty>(this, 483)(this);
	}

	float hitchance()
	{
		float hitchance = 101.f;
		if (!this) return 0.f;

		float inaccuracy = GetInaccuracy();
		if (inaccuracy == 0) inaccuracy = 0.0000001;
		inaccuracy = 1 / inaccuracy;
		hitchance = inaccuracy;
		return hitchance;
	}

	const char* GetGunName()
	{
		if (!this)
			return "";
		switch (this->GetItemDefinitionIndex())
		{
		case WEAPON_AK47:
			return "AK47";
			break;
		case WEAPON_AUG:
			return "AUG";
			break;
		case weapon_revolver:
			return "R8";
			break;
		case WEAPON_AWP:
			return "AWP";
			break;
		case WEAPON_BIZON:
			return "Bizon";
			break;
		case WEAPON_C4:
			return "Bomb";
			break;
		case WEAPON_CZ75:
			return "CZ75";
			break;
		case WEAPON_DEAGLE:
			return "Deagle";
			break;
		case WEAPON_DECOY:
			return "Decoy";
			break;
		case WEAPON_DUALBERETTA:
			return "DualBerettas";
			break;
		case WEAPON_FAMAS:
			return "Famas";
			break;
		case WEAPON_FIVESEVEN:
			return "FiveSeven";
			break;
		case WEAPON_FLASHBANG:
			return "Flash";
			break;
		case WEAPON_G3SG1:
			return "G3SG1";
			break;
		case WEAPON_GALIL:
			return "Galil";
			break;
		case WEAPON_GLOCK:
			return "Glock-18";
			break;
		case WEAPON_INC:
			return "Molotov";
			break;
		case WEAPON_MOLOTOV:
			return "Molotov";
			break;
		case WEAPON_SSG08:
			return "SSG-08";
			break;
		case WEAPON_HEGRENADE:
			return "HE Grenade";
			break;
		case WEAPON_M249:
			return "M249";
			break;
		case WEAPON_M4A1:
			return "M4A1";
			break;
		case WEAPON_MAC10:
			return "MAC10";
			break;
		case WEAPON_MAG7:
			return "MAG7";
			break;
		case WEAPON_MP7:
			return "MP7";
			break;
		case WEAPON_MP9:
			return "MP9";
			break;
		case WEAPON_NOVA:
			return "Nova";
			break;
		case WEAPON_NEGEV:
			return "Negev";
			break;
		case WEAPON_P250:
			return "P250";
			break;
		case WEAPON_P90:
			return "P90";
			break;
		case WEAPON_SAWEDOFF:
			return "SawedOff";
			break;
		case WEAPON_SCAR20:
			return "SCAR20";
			break;
		case WEAPON_SMOKEGRENADE:
			return "Smoke";
			break;
		case WEAPON_SG553:
			return "SG553";
			break;
		case WEAPON_TEC9:
			return "TEC9";
			break;
		case WEAPON_USP:
			return "P2000";
			break;
		case WEAPON_USPS:
			return "USP-S";
			break;
		case WEAPON_UMP45:
			return "UMP";
			break;
		case WEAPON_XM1014:
			return "XM1014";
			break;
		case WEAPON_ZEUSX27:
			return "Taser";
			break;
		case WEAPON_M4A1S:
			return "M4A1-S";
		}
		if (IsKnife())
			return "Knife";
		return "";
	}
	bool IsZeus()
	{
		if (!this)
			return false;

		int WeaponId = this->GetItemDefinitionIndex();

		return WeaponId == eWeaponType1::WEAPON_ZEUSX27;
	}
	bool IsBomb()
	{
		int iWeaponID = this->GetItemDefinitionIndex();

		return iWeaponID == bomb;
	}
	bool IsNade()
	{
		int WeaponId = this->GetItemDefinitionIndex();

		return WeaponId == WEAPON_FLASHBANG || WeaponId == WEAPON_HEGRENADE || WeaponId == WEAPON_SMOKEGRENADE ||
			WeaponId == WEAPON_MOLOTOV || WeaponId == WEAPON_DECOY || WeaponId == WEAPON_INC;
	}

	bool IsMiscWeapon()
	{
		if (!this)
			return false;
		int iWeaponID = this->GetItemDefinitionIndex();
		return (iWeaponID == tknife || iWeaponID == ctknife
			|| iWeaponID == goldknife || iWeaponID == bomb
			|| iWeaponID == hegren || iWeaponID == decoygren
			|| iWeaponID == flashgren || iWeaponID == molotov
			|| iWeaponID == smoke || iWeaponID == incendiary || iWeaponID == 59
			|| iWeaponID == 500 || iWeaponID == 505 || iWeaponID == 506
			|| iWeaponID == 507 || iWeaponID == 508 || iWeaponID == 509
			|| iWeaponID == 515);
	}
	bool IsKnife()
	{
		if (!this)
			return false;
		int iWeaponID = this->GetItemDefinitionIndex();
		return (iWeaponID == tknife || iWeaponID == ctknife
			|| iWeaponID == goldknife || iWeaponID == 59 || iWeaponID == 41
			|| iWeaponID == 500 || iWeaponID == 505 || iWeaponID == 506
			|| iWeaponID == 507 || iWeaponID == 508 || iWeaponID == 509
			|| iWeaponID == 515);
	}
	bool isPistol()
	{
		int WeaponId = this->GetItemDefinitionIndex();

		return WeaponId == WEAPON_DEAGLE || WeaponId == eWeaponType1::WEAPON_DUALBERETTA || WeaponId == WEAPON_FIVESEVEN || WeaponId == WEAPON_P250 ||
			WeaponId == WEAPON_GLOCK || WeaponId == WEAPON_USPS || WeaponId == WEAPON_CZ75 || WeaponId == WEAPON_USP ||
			WeaponId == WEAPON_TEC9 || WeaponId == weapon_revolver;
	}
	bool IsHeavyPistol() {
		int WeaponId = this->GetItemDefinitionIndex();

		return WeaponId == WEAPON_DEAGLE || WeaponId == weapon_revolver;
	}

	bool isShotgun()
	{
		int WeaponId = this->GetItemDefinitionIndex();

		return WeaponId == WEAPON_XM1014 || WeaponId == WEAPON_NOVA || WeaponId == WEAPON_SAWEDOFF || WeaponId == WEAPON_MAG7;
	}
	bool isRifle()
	{
		int WeaponId = this->GetItemDefinitionIndex();

		return WeaponId == WEAPON_AK47 || WeaponId == WEAPON_M4A1 || WeaponId == WEAPON_M4A1S || WeaponId == WEAPON_GALIL || WeaponId == WEAPON_FAMAS ||
			WeaponId == WEAPON_AUG || WeaponId == WEAPON_SG553;
	}
	bool isSniper()
	{
		int WeaponId = this->GetItemDefinitionIndex();

		return WeaponId == WEAPON_AWP || WeaponId == WEAPON_SCAR20 || WeaponId == WEAPON_G3SG1 || WeaponId == WEAPON_SSG08;
	}
	bool DTable() {
		int WeaponId = this->GetItemDefinitionIndex();
		return IsGun() && !isShotgun() && WeaponId != WEAPON_SSG08 && WeaponId != WEAPON_AWP;
	}
	bool isAutoSniper()
	{
		int WeaponId = this->GetItemDefinitionIndex();

		return WeaponId == WEAPON_SCAR20 || WeaponId == WEAPON_G3SG1;
	}
	bool IsGun()
	{
		if (!this)
			return false;
		int id = this->GetItemDefinitionIndex();
		if (!id)
			return false;
		if (id == WEAPON_KNIFE || id == WEAPON_HEGRENADE || id == WEAPON_DECOY || id == WEAPON_INC ||
			id == WEAPON_MOLOTOV || id == WEAPON_C4 /*|| id == WEAPON_ZEUSX27*/ ||
			id == WEAPON_FLASHBANG || id == WEAPON_SMOKEGRENADE || id == WEAPON_KNIFE_T)
			return false;
		else
			return true;
	}


};

class CBaseCSGrenade : IBaseCombatWeapon
{
public:
	float GetThrowTime();
};
class bf_read {
public:
	const char* m_pDebugName;
	bool m_bOverflow;
	int m_nDataBits;
	unsigned int m_nDataBytes;
	unsigned int m_nInBufWord;
	int m_nBitsAvail;
	const unsigned int* m_pDataIn;
	const unsigned int* m_pBufferEnd;
	const unsigned int* m_pData;

	bf_read() = default;

	bf_read(const void* pData, int nBytes, int nBits = -1) {
		StartReading(pData, nBytes, 0, nBits);
	}

	void StartReading(const void* pData, int nBytes, int iStartBit, int nBits) {
		// Make sure it's dword aligned and padded.
		m_pData = (uint32_t*)pData;
		m_pDataIn = m_pData;
		m_nDataBytes = nBytes;

		if (nBits == -1) {
			m_nDataBits = nBytes << 3;
		}
		else {
			m_nDataBits = nBits;
		}
		m_bOverflow = false;
		m_pBufferEnd = reinterpret_cast<uint32_t const*>(reinterpret_cast<uint8_t const*>(m_pData) + nBytes);
		if (m_pData)
			Seek(iStartBit);
	}

	bool Seek(int nPosition) {
		bool bSucc = true;
		if (nPosition < 0 || nPosition > m_nDataBits) {
			m_bOverflow = true;
			bSucc = false;
			nPosition = m_nDataBits;
		}
		int nHead = m_nDataBytes & 3; // non-multiple-of-4 bytes at head of buffer. We put the "round off"
									  // at the head to make reading and detecting the end efficient.

		int nByteOfs = nPosition / 8;
		if ((m_nDataBytes < 4) || (nHead && (nByteOfs < nHead))) {
			// partial first dword
			uint8_t const* pPartial = (uint8_t const*)m_pData;
			if (m_pData) {
				m_nInBufWord = *(pPartial++);
				if (nHead > 1)
					m_nInBufWord |= (*pPartial++) << 8;
				if (nHead > 2)
					m_nInBufWord |= (*pPartial++) << 16;
			}
			m_pDataIn = (uint32_t const*)pPartial;
			m_nInBufWord >>= (nPosition & 31);
			m_nBitsAvail = (nHead << 3) - (nPosition & 31);
		}
		else {
			int nAdjPosition = nPosition - (nHead << 3);
			m_pDataIn = reinterpret_cast<uint32_t const*>(
				reinterpret_cast<uint8_t const*>(m_pData) + ((nAdjPosition / 32) << 2) + nHead);
			if (m_pData) {
				m_nBitsAvail = 32;
				GrabNextDWord();
			}
			else {
				m_nInBufWord = 0;
				m_nBitsAvail = 1;
			}
			m_nInBufWord >>= (nAdjPosition & 31);
			m_nBitsAvail = min(m_nBitsAvail, 32 - (nAdjPosition & 31)); // in case grabnextdword overflowed
		}
		return bSucc;
	}

	FORCEINLINE void GrabNextDWord(bool bOverFlowImmediately = false) {
		if (m_pDataIn == m_pBufferEnd) {
			m_nBitsAvail = 1; // so that next read will run out of words
			m_nInBufWord = 0;
			m_pDataIn++; // so seek count increments like old
			if (bOverFlowImmediately)
				m_bOverflow = true;
		}
		else if (m_pDataIn > m_pBufferEnd) {
			m_bOverflow = true;
			m_nInBufWord = 0;
		}
		else {
			m_nInBufWord = DWORD(*(m_pDataIn++));
		}
	}
};
class bf_write {
public:
	unsigned char* m_pData;
	int m_nDataBytes;
	int m_nDataBits;
	int m_iCurBit;
	bool m_bOverflow;
	bool m_bAssertOnOverflow;
	const char* m_pDebugName;

	void StartWriting(void* pData, int nBytes, int iStartBit = 0, int nBits = -1) {
		// Make sure it's dword aligned and padded.
		// The writing code will overrun the end of the buffer if it isn't dword aligned, so truncate to force alignment
		nBytes &= ~3;

		m_pData = (unsigned char*)pData;
		m_nDataBytes = nBytes;

		if (nBits == -1) {
			m_nDataBits = nBytes << 3;
		}
		else {
			m_nDataBits = nBits;
		}

		m_iCurBit = iStartBit;
		m_bOverflow = false;
	}

	bf_write() {
		m_pData = NULL;
		m_nDataBytes = 0;
		m_nDataBits = -1; // set to -1 so we generate overflow on any operation
		m_iCurBit = 0;
		m_bOverflow = false;
		m_bAssertOnOverflow = true;
		m_pDebugName = NULL;
	}

	// nMaxBits can be used as the number of bits in the buffer.
	// It must be <= nBytes*8. If you leave it at -1, then it's set to nBytes * 8.
	bf_write(void* pData, int nBytes, int nBits = -1) {
		m_bAssertOnOverflow = true;
		m_pDebugName = NULL;
		StartWriting(pData, nBytes, 0, nBits);
	}

	bf_write(const char* pDebugName, void* pData, int nBytes, int nBits = -1) {
		m_bAssertOnOverflow = true;
		m_pDebugName = pDebugName;
		StartWriting(pData, nBytes, 0, nBits);
	}
};
class CLC_Move {
private:
	char __PAD0[0x8]; // 0x0 two vtables
public:
	int m_nBackupCommands; // 0x8
	int m_nNewCommands;    // 0xC
	std::string* m_data;   // 0x10 std::string
	bf_read m_DataIn;      // 0x14
	bf_write m_DataOut;    // 0x38
};                       // size: 0x50

class INetChannel;
class INetMessage
{
public:
	virtual					~INetMessage() { }
	virtual void			SetNetChannel(void* pNetChannel) = 0;
	virtual void			SetReliable(bool bState) = 0;
	virtual bool			Process() = 0;
	virtual	bool			ReadFromBuffer(bf_read& buffer) = 0;
	virtual	bool			WriteToBuffer(bf_write& buffer) = 0;
	virtual bool			IsReliable() const = 0;
	virtual int				GetType() const = 0;
	virtual int				GetGroup() const = 0;
	virtual const char* GetName() const = 0;
	virtual void* GetNetChannel(void) const = 0;
	virtual const char* ToString() const = 0;
};

class INetChannel
{
public:
	//char pad_0000[20];           //0x0000
	//bool m_bProcessingMessages;  //0x0014
	//bool m_bShouldDelete;        //0x0015
	//char pad_0016[2];            //0x0016
	//int32_t m_nOutSequenceNr;    //0x0018 last send outgoing sequence number
	//int32_t m_nInSequenceNr;     //0x001C last received incoming sequnec number
	//int32_t m_nOutSequenceNrAck; //0x0020 last received acknowledge outgoing sequnce number
	//int32_t m_nOutReliableState; //0x0024 state of outgoing reliable data (0/1) flip flop used for loss detection
	//int32_t m_nInReliableState;  //0x0028 state of incoming reliable data
	//int32_t m_nChokedPackets;    //0x002C number of choked packets
	//bool SendNetMsg(INetMessage* msg, bool bForceReliable, bool bVoice)
	//{
	//	typedef bool(__thiscall* func1)(void*, INetMessage*, bool, bool);
	//	return getvfunc<func1>(this, 42)(this, msg, bForceReliable, bVoice);
	//}
	//int SendDatagram(void*, void *datagram) {
	//	typedef int(__thiscall* func1)(void*, void*);
	//	return getvfunc<func1>(this, 46)(this, datagram);
	//}
	//char pad_0030[1044];         //0x0030
	std::byte	pad0[0x14];				//0x0000
	bool		bProcessingMessages;	//0x0014
	bool		bShouldDelete;			//0x0015
	std::byte	pad1[0x2];				//0x0016
	int			iOutSequenceNr;			//0x0018 last send outgoing sequence number
	int			iInSequenceNr;			//0x001C last received incoming sequence number
	int			iOutSequenceNrAck;		//0x0020 last received acknowledge outgoing sequence number
	int			iOutReliableState;		//0x0024 state of outgoing reliable data (0/1) flip flop used for loss detection
	int			iInReliableState;		//0x0028 state of incoming reliable data
	int			iChokedPackets;			//0x002C number of choked packets
	std::byte	pad2[0x414];			//0x0030

	int	SendDatagram()
	{
		typedef int(__thiscall* func1)(void*, void*);
		return getvfunc<func1>(this, 46)(this, NULL);
		//return getvfunc<int>(this, 46, pDatagram);
	}
}; //Size: 0x0444

template < typename T >
class CNetMessagePB : public INetMessage, public T {};
using CCLCMsg_Move_t = CNetMessagePB< CLC_Move >;