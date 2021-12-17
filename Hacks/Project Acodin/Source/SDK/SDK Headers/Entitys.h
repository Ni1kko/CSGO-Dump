#pragma once
#include "../../stdafx.h"
#include <array>
#include "../../MiscDefinitions.h"

#define BONE_USED_BY_HITBOX 0x00000100

class CAnimationLayer
{
public:
	float m_flLayerAnimtime; //0
	float m_flLayerFadeOuttime;
	float m_flBlendIn;
	float m_flBlendOut;
	int Unknown1;
	int m_nOrder; //20
	int m_nSequence; //24
	float m_flPrevCycle;
	float m_flWeight; //32
	float m_flWeightDeltaRate;
	float m_flPlaybackRate;
	float m_flCycle; //44
	CBaseEntity *m_pOwner; //48
	int	m_nInvalidatePhysicsBits;
};

class CBaseAnimState
{
public:
	char pad[4];
	char bUnknown; //0x4
	char pad2[91];
	void* pBaseEntity; //0x60
	void* pActiveWeapon; //0x64
	void* pLastActiveWeapon; //0x68
	float m_flLastClientSideAnimationUpdateTime; //0x6C
	int m_iLastClientSideAnimationUpdateFramecount; //0x70
	float m_flEyePitch; //0x74
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
	float speed_2d; //0xEC
	float flUpVelocity; //0xF0
	float m_flSpeedNormalized; //0xF4 //from 0 to 1
	float m_flFeetSpeedForwardsOrSideWays; //0xF8 //from 0 to 2. something  is 1 when walking, 2.something when running, 0.653 when crouch walking
	float m_flFeetSpeedUnknownForwardOrSideways; //0xFC //from 0 to 3. something
	float m_flTimeSinceStartedMoving; //0x100
	float m_flTimeSinceStoppedMoving; //0x104
	unsigned char m_bOnGround; //0x108
	unsigned char m_bInHitGroundAnimation; //0x109
	char pad7[10];
	float m_flLastOriginZ; //0x114
	float m_flHeadHeightOrOffsetFromHittingGroundAnimation; //0x118 from 0 to 1, is 1 when standing
	float m_flStopToFullRunningFraction; //0x11C from 0 to 1, doesnt change when walking or crouching, only running
	char pad8[4]; //NaN
	float m_flUnknownFraction; //0x124 affected while jumping and running, or when just jumping, 0 to 1
	char pad9[4]; //NaN
	float m_flUnknown3;
	char pad10[528];
};

class IHandleEntity;

class CBaseHandle
{
	friend class C_BaseEntityList;
public:
protected:
};
#pragma region randomshit
template< class T >
class CHandle : public CBaseHandle
{
public:
};

#pragma endregion randomshit

struct model_t
{
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

//enum class CSGOHitboxID
//{
//	Head = 6,
//	Neck = 5,
//	NeckLower = 7,
//	Pelvis = 0,
//	Stomach = 68,
//	LowerChest = 3,
//	Chest = 80,
//	UpperChest =35,
//	RightThigh = 74,
//	LeftThigh = 52,
//	RightShin = 71,
//	LeftShin = 64,
//	RightFoot = 72,
//	LeftFoot = 70,
//	RightHand = 60,
//	LeftHand = 2,
//	RightUpperArm = 36,
//	RightLowerArm = 59,
//	LeftUpperArm = 8,
//	LeftLowerArm = 32,
//};

class CCSBomb
{
private:
	template< class T >
	inline T GetFieldValue( int offset )
	{
		return *( T* )( ( DWORD )this + offset );
	}

	template< class T >
	T* GetFieldPointer( int offset )
	{
		return ( T* )( ( DWORD )this + offset );
	}

public:

	/*
	25-Apr-2016
	=========================
	DT_PlantedC4
	|___m_flTimerLength
	|___m_bBombTicking
	|___m_flDefuseCountDown
	|___m_flC4Blow
	|___m_hBombDefuser
	|___m_flDefuseLength
	|___m_bBombDefused
	=========================
	*/
	float GetTimerLength()
	{
		static int m_flTimerLength = Netvar("DT_PlantedC4", "m_flTimerLength");
		return GetFieldValue< float >( m_flTimerLength );
	}

	float GetC4BlowTime()
	{
		static int m_flC4Blow = Netvar("DT_PlantedC4", "m_flC4Blow");
		return GetFieldValue< float >( m_flC4Blow );
	}

	float GetC4DefuseCountDown()
	{
		static int m_flDefuseCountDown = Netvar("DT_PlantedC4", "m_flDefuseCountDown");
		return GetFieldValue< float >( m_flDefuseCountDown );
	}
};

class CBaseEntity;

class CCSWeaponInfo
{
public:
	char _0x0000[20];
	__int32 max_clip;			//0x0014 
	char _0x0018[12];
	__int32 max_reserved_ammo;	//0x0024 
	char _0x0028[96];
	char* hud_name;				//0x0088 
	char* weapon_name;			//0x008C 
	char _0x0090[60];

	__int32 type;				//0x00CC 
	__int32 price;				//0x00D0 
	__int32 reward;				//0x00D4 
	char _0x00D8[20];
	BYTE full_auto;				//0x00EC 
	char _0x00ED[3];
	__int32 damage;				//0x00F0 
	float armor_ratio;			//0x00F4 
	__int32 bullets;			//0x00F8 
	float penetration;			//0x00FC 
	char _0x0100[8];
	float range;				//0x0108 
	float range_modifier;		//0x010C 
	char _0x0110[16];
	BYTE silencer;				//0x0120 
	char _0x0121[15];
	float max_speed;			//0x0130 
	float max_speed_alt;		//0x0134 
	char _0x0138[76];
	__int32 recoil_seed;		//0x0184 
	char _0x0188[32];
};

class IHandleEntity
{
public:
	virtual ~IHandleEntity()
	{
	}

	virtual void SetRefEHandle( const HANDLE& handle ) = 0;

	virtual const HANDLE& GetRefEHandle() const = 0;
};

class IClientUnknown : public IHandleEntity
{
public:
	virtual void* GetCollideable() = 0;

	virtual void* GetClientNetworkable() = 0;

	virtual void* GetClientRenderable() = 0;

	virtual void* GetIClientEntity() = 0;

	virtual void* GetBaseEntity() = 0;

	virtual void* GetClientThinkable() = 0;

	virtual void* GetClientAlphaProperty() = 0;
};

class IClientRenderable
{
public:
	virtual IClientUnknown* GetIClientUnknown() = 0;

	virtual Vector const& GetRenderOrigin( void ) = 0;

	virtual Vector const& GetRenderAngles( void ) = 0;

	virtual bool ShouldDraw( void ) = 0;

	virtual int GetRenderFlags( void ) = 0; // ERENDERFLAGS_xxx
	virtual void Unused( void ) const
	{
	}

	virtual HANDLE GetShadowHandle() const = 0;

	virtual HANDLE& RenderHandle() = 0;

	virtual const model_t* GetModel() const = 0;

	virtual int DrawModel( int flags, const int /*RenderableInstance_t*/ & instance ) = 0;

	virtual int GetBody() = 0;

	virtual void GetColorModulation( float* color ) = 0;

	virtual bool LODTest() = 0;

	virtual void SetupWeights( const matrix3x4* pBoneToWorld, int nFlexWeightCount, float* pFlexWeights, float* pFlexDelayedWeights ) = 0;

	virtual void DoAnimationEvents( void ) = 0;

	virtual void* /*IPVSNotify*/ GetPVSNotifyInterface() = 0;

	virtual void GetRenderBounds( Vector& mins, Vector& maxs ) = 0;

	virtual void GetRenderBoundsWorldspace( Vector& mins, Vector& maxs ) = 0;

	virtual void GetShadowRenderBounds( Vector& mins, Vector& maxs, int /*ShadowType_t*/ shadowType ) = 0;

	virtual bool ShouldReceiveProjectedTextures( int flags ) = 0;

	virtual bool GetShadowCastDistance( float* pDist, int /*ShadowType_t*/ shadowType ) const = 0;

	virtual bool GetShadowCastDirection( Vector* pDirection, int /*ShadowType_t*/ shadowType ) const = 0;

	virtual bool IsShadowDirty() = 0;

	virtual void MarkShadowDirty( bool bDirty ) = 0;

	virtual IClientRenderable* GetShadowParent() = 0;

	virtual IClientRenderable* FirstShadowChild() = 0;

	virtual IClientRenderable* NextShadowPeer() = 0;

	virtual int /*ShadowType_t*/ ShadowCastType() = 0;

	virtual void CreateModelInstance() = 0;

	virtual HANDLE GetModelInstance() = 0;

	virtual const matrix3x4& RenderableToWorldTransform() = 0;

	virtual int LookupAttachment( const char* pAttachmentName ) = 0;

	virtual bool GetAttachment( int number, Vector& origin, Vector& angles ) = 0;

	virtual bool GetAttachment( int number, matrix3x4& matrix ) = 0;

	virtual float* GetRenderClipPlane( void ) = 0;

	virtual int GetSkin() = 0;

	virtual void OnThreadedDrawSetup() = 0;

	virtual bool UsesFlexDelayedWeights() = 0;

	virtual void RecordToolMessage() = 0;

	virtual bool ShouldDrawForSplitScreenUser( int nSlot ) = 0;

	virtual uint8 OverrideAlphaModulation( uint8 nAlpha ) = 0;

	virtual uint8 OverrideShadowAlphaModulation( uint8 nAlpha ) = 0;
};

class bf_read
{
};

class IClientNetworkable
{
public:
	virtual IClientUnknown*	GetIClientUnknown() = 0;
	virtual void			Release() = 0;
	virtual ClientClass*	GetClientClass() = 0;// FOR NETVARS FIND YOURSELF ClientClass* stuffs
	virtual void			NotifyShouldTransmit( /* ShouldTransmitState_t state*/) = 0;
	virtual void			OnPreDataChanged( /*DataUpdateType_t updateType*/) = 0;
	virtual void			OnDataChanged( /*DataUpdateType_t updateType*/) = 0;
	virtual void			PreDataUpdate( /*DataUpdateType_t updateType*/) = 0;
	virtual void			PostDataUpdate( /*DataUpdateType_t updateType*/) = 0;
	virtual void			unknown();
	virtual bool			IsDormant(void) = 0;
	virtual int				GetIndex(void) const = 0;
	virtual void			ReceiveMessage(int classID /*, bf_read &msg*/) = 0;
	virtual void*			GetDataTableBasePtr() = 0;
	virtual void			SetDestroyedOnRecreateEntities(void) = 0;
};

class CBaseCombatWeapon;

class VarMapEntry_t
{
public:
	unsigned short		type;
	unsigned short		m_bNeedsToInterpolate;	// Set to false when this var doesn't
												// need Interpolate() called on it anymore.
	void				*data;
	void	*watcher;
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

class CBaseEntity : public IClientUnknown, public IClientRenderable, public IClientNetworkable
{
public:
	IClientNetworkable* GetNetworkable()
	{
		return ( IClientNetworkable* )( ( DWORD )this + 0x8 );
	}

	bool canHit(Vector end, CBaseEntity* ent);

	char* GetLastPlaceName()
	{
		return (char*)((DWORD)this + Netvar("DT_CSPlayer", "m_szLastPlaceName"));
	}


	template< class T >
	inline T GetFieldValue( int offset )
	{
		return *( T* )( ( DWORD )this + offset );
	}

	template< class T >
	T* GetFieldPointer( int offset )
	{
		return ( T* )( ( DWORD )this + offset );
	}

	Vector GetAbsOrigin()
	{
		//0xAC interpolated getAbsOrigin
		//0xA0 normal abs
		static int VecOrigin = Netvar("DT_BaseEntity", "m_vecOrigin");
		return GetFieldValue< Vector >( VecOrigin );
	}
	VarMapping_t* GetVarMap()
	{
		return reinterpret_cast<VarMapping_t*>((DWORD)this + 0x24);
	}
	bool InBuyZone() {
		static int m_bInBuyZone = Netvar(XorStr("DT_CSPlayer"), XorStr("m_bInBuyZone"));
		return GetFieldValue<bool>(m_bInBuyZone);
	}

	//HANDLE GetObserverTargetHandle() {
	//static int m_hObserverTarget = Netvar("DT_CSPlayer", "m_hObserverTarget");
	//return GetFieldValue<HANDLE>(m_hObserverTarget);
	//}

	Vector GetNetworkOrigin()
	{
		static int offset = Netvar("DT_CSPlayer", "m_flFriction") - sizeof(Vector);
		return GetFieldValue< Vector >( offset );
	}

	Vector GetVecOrigin()
	{
		return *( Vector* )( ( DWORD )this + 0x138);
	}

	Vector GetVecViewOffset()
	{
		static int iOffset = Netvar("DT_BasePlayer", "localdata", "m_vecViewOffset[0]");
		return *( Vector* )( ( DWORD )this + iOffset );
	}

	Vector GetEyeAngles()
	{
		static int iOffset = Netvar("DT_CSPlayer", "m_angEyeAngles[0]");
		return *( Vector* )( ( DWORD )this + iOffset );
	}

	Vector* GetEyeAnglesPointer()
	{
		static int iOffset = Netvar("DT_CSPlayer", "m_angEyeAngles[0]");
		return GetFieldPointer< Vector >( iOffset );
	}

	Vector GetRotation()
	{
		static int ioffset = Netvar("DT_BaseEntity", "m_angRotation");
		return this->GetFieldValue< Vector >( ioffset );
	}

	bool IsSpotted()
	{
		static int iOffset = Netvar("DT_BaseEntity", "m_bSpotted");
		return GetFieldValue< bool >( iOffset );
	}

	bool* IsSpottedPtr()
	{
		static int iOffset = Netvar("DT_BaseEntity", "m_bSpotted");
		return GetFieldPointer< bool >( iOffset );
	}

	Vector GetAbsAngles()
	{
		__asm
		{
			MOV ECX, this;
			MOV EAX, DWORD PTR DS : [ECX];
			CALL DWORD PTR DS : [EAX + 0x2C]
		}
	}

	player_info_t GetPlayerInfo()
	{
		player_info_t pinfo;
		Interfaces.pEngine->GetPlayerInfo( GetIndex(), &pinfo );
		return pinfo;
	}

	unsigned long long GID()
	{
		return GetPlayerInfo().xuid;
	}

	std::string GetName()
	{
		string name = GetPlayerInfo().name;
		std::size_t found = name.find( "\n" );
		while( found != std::string::npos )
		{
			name.replace( found, found + 2, "" );
			found = name.find( "\n" );
		}
		return name;
	}

	bool IsDormant()
	{
		return *(bool*)(DWORD)this + 0xED;

		/*__asm
		{
			MOV EDI, this
			LEA ECX, [EDI + 0x8]
			MOV EDX, DWORD PTR DS : [ecx]
			CALL[EDX + 0x24]
		}*/
	}
	HANDLE GetObserverTargetHandle() {
		static int m_hObserverTarget = Netvar("DT_CSPlayer", "m_hObserverTarget");
		return GetFieldValue<HANDLE>(m_hObserverTarget);
	}
	int GetIndex()
	{
		//	void* networkable = (void*)(this + 0x8);
		//typedef int(__thiscall* OriginalFn)(PVOID);
		//return CallVFunction<OriginalFn>(this, 76)(this);
		return GetNetworkable()->GetIndex();
	}

	int GetTickBase( void )
	{
		static int m_nTickBase = Netvar("DT_BasePlayer", "localdata", "m_nTickBase");
		return GetFieldValue< int >( m_nTickBase );
	}

	bool BombDefused()
	{
		static int m_bBombDefused = Netvar("DT_PlantedC4", "m_bBombDefused");
		return GetFieldValue< bool >( m_bBombDefused );
	}

	float BombTimer()
	{
		static int m_flC4Blow = Netvar("DT_PlantedC4", "m_flC4Blow");
		float flBombTime = GetFieldValue< float >( m_flC4Blow );
		return ( flBombTime > 0 ) ? flBombTime : 0.f;
	}

	bool IsPlayer()
	{
		if( !this )
		{
			return false;
		}
		if( this->IsDormant() )
		{
			return false;
		}
		if( this->GetTeam() == 0 )
		{
			return false;
		}

		player_info_t info;
		if( !( Interfaces.pEngine->GetPlayerInfo( GetIndex(), &info ) ) )
			return false;

		ClientClass* cClass = ( ClientClass* )this->GetClientClass();

		if( cClass->m_ClassID != 35 )
		{
			return false;
		}
		return true;
	}

	bool IsWeapon()
	{
		if( !this )
		{
			return false;
		}
		ClientClass* cClass = ( ClientClass* )this->GetClientClass();

		if( cClass->m_ClassID != ClientClassIDS::CAK47 && cClass->m_ClassID != ClientClassIDS::CBaseCSGrenadeProjectile && cClass->m_ClassID != ClientClassIDS::CC4 && cClass->m_ClassID != ClientClassIDS::CBaseWeaponWorldModel && cClass->m_ClassID != ClientClassIDS::CDEagle && cClass->m_ClassID != ClientClassIDS::CDecoyGrenade && cClass->m_ClassID != ClientClassIDS::CFlashbang && cClass->m_ClassID != ClientClassIDS::CHEGrenade && cClass->m_ClassID != ClientClassIDS::CKnife && cClass->m_ClassID != ClientClassIDS::CMolotovGrenade && cClass->m_ClassID != ClientClassIDS::CWeaponAWP && cClass->m_ClassID != ClientClassIDS::CSmokeGrenade && cClass->m_ClassID != ClientClassIDS::CWeaponAug && cClass->m_ClassID != ClientClassIDS::CWeaponBizon && cClass->m_ClassID != ClientClassIDS::CWeaponElite && cClass->m_ClassID != ClientClassIDS::CWeaponFamas && cClass->m_ClassID != ClientClassIDS::CWeaponFiveSeven && cClass->m_ClassID != ClientClassIDS::CWeaponG3SG1 && cClass->m_ClassID != ClientClassIDS::CWeaponGlock && cClass->m_ClassID != ClientClassIDS::CWeaponGalilAR && cClass->m_ClassID != ClientClassIDS::CWeaponXM1014 && cClass->m_ClassID != ClientClassIDS::CWeaponUMP45 && cClass->m_ClassID != ClientClassIDS::CWeaponTec9 && cClass->m_ClassID != ClientClassIDS::CWeaponTaser && cClass->m_ClassID != ClientClassIDS::CWeaponSawedoff && cClass->m_ClassID != ClientClassIDS::CWeaponSSG08 && cClass->m_ClassID != ClientClassIDS::CWeaponSG556 && cClass->m_ClassID != ClientClassIDS::CWeaponSCAR20 && cClass->m_ClassID != ClientClassIDS::CWeaponP90 && cClass->m_ClassID != ClientClassIDS::CWeaponP250 && cClass->m_ClassID != ClientClassIDS::CWeaponNOVA && cClass->m_ClassID != ClientClassIDS::CWeaponSawedoff && cClass->m_ClassID != ClientClassIDS::CWeaponSG556 && cClass->m_ClassID != ClientClassIDS::CWeaponMP9 && cClass->m_ClassID != ClientClassIDS::CWeaponMag7 && cClass->m_ClassID != ClientClassIDS::CWeaponM4A1 && cClass->m_ClassID != ClientClassIDS::CWeaponNegev && cClass->m_ClassID != ClientClassIDS::CWeaponHKP2000 )
		{
			return false;
		}
		return true;
	}

	bool IsHostage()
	{
		if( !this )
		{
			return false;
		}
		ClientClass* cClass = ( ClientClass* )this->GetClientClass();

		return cClass->m_ClassID == ClientClassIDS::CHostage;
	}

	bool IsChicken()
	{
		if( !this )
		{
			return false;
		}
		ClientClass* cClass = ( ClientClass* )this->GetClientClass();

		return cClass->m_ClassID == ClientClassIDS::CChicken;
	}

	bool isValidPlayer()
	{
		if( !isAlive() )
			return false;
		if( !( GetHealth() > 0 ) )
			return false;
		if( IsDormant() )
			return false;
		player_info_t info;
		if( !( Interfaces.pEngine->GetPlayerInfo( GetIndex(), &info ) ) )
			return false;
		return true;
	}

	int GetHealth()
	{
		return *reinterpret_cast< int* >( ( DWORD )this + 0x100 );
	}

	bool isAlive()
	{
		BYTE lifestate = *( BYTE* )( ( DWORD )this + 0x25F);
		return ( lifestate == 0 );
	}

	int GetFlags()
	{
		return *reinterpret_cast< int* >( ( DWORD )this + 0x104);
	}

	int &GetFlagsPtr()
	{
		return *(int*)((DWORD)this + 0x104);
	}

	int &GetTickBasePtr(void)
	{
		static int m_nTickBase = Netvar("DT_BasePlayer", "m_nTickBase");
		return *(int*)((DWORD)this + m_nTickBase);
	}

	int GetTeam()
	{
		return *reinterpret_cast< int* >( ( DWORD )this + 0xF4);
	}

	float GetFlashDuration()
	{
		return *( float* )( ( DWORD )this + Netvar(("DT_CSPlayer"), ("m_flFlashDuration")) );
	}

	bool IsFlashed()
	{
		return GetFlashDuration() > 0;
	}

	std::string GetClientClassName()
	{
		uintptr_t pNext = *( uintptr_t* )( ( DWORD )this + 0x8 );
		pNext = *( uintptr_t* )( ( DWORD )pNext + 0x8 );
		pNext = *( uintptr_t* )( ( DWORD )pNext + 0x1 );
		return std::string( *( char** )( ( DWORD )pNext + 0x8 ) );
	}

	Vector GetVecVelocity()
	{
		return *( Vector* )( ( DWORD )this + 0x114);
	}

	std::array< float, 24 >* CBaseEntity::GetPoseParameters()
	{
		return ( std::array< float, 24 >* ) ( ( DWORD )this + Netvar("DT_BaseAnimating", "m_flPoseParameter") );
	}

	Vector GetPunchAngle()
	{
		static int m_aimPunchAngle = Netvar("DT_BasePlayer", "localdata", "m_Local", "m_aimPunchAngle");
		return *GetFieldPointer< Vector >( m_aimPunchAngle );
	}

	Vector GetPunchAngleVel()
	{
		static int m_aimPunchAngle = Netvar("DT_BasePlayer", "localdata", "m_Local", "m_aimPunchAngleVel");
		return *GetFieldPointer< Vector >( m_aimPunchAngle );
	}

	Vector* GetPunchAnglePtr()
	{
		static int m_aimPunchAngle = Netvar("DT_BasePlayer", "localdata", "m_Local", "m_aimPunchAngle");
		return GetFieldPointer< Vector >( m_aimPunchAngle );
	}

	CBaseAnimState* GetAnimState()
	{
		return *reinterpret_cast<CBaseAnimState**>(uintptr_t(this) + 0x3900);
	}

	void SetAngle2(Vector wantedang)
	{
		typedef void(__thiscall* SetAngleFn)(void*, const Vector &);
		static SetAngleFn SetAngle = (SetAngleFn)((DWORD)Utils.FindPattern("client_panorama.dll", "55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1"));
		SetAngle(this, wantedang);
	}


	void UpdateClientSideAnimation()
	{
		getvfunc<void(__thiscall*)(void*)>(this, 219)(this);
	}

	void SetVelocity(Vector velocity)
	{
		*reinterpret_cast<Vector*>(uintptr_t(this) + 0x110) = velocity;
	}

	void SetAbsOrigin(Vector ArgOrigin)
	{
		using Fn = void(__thiscall*)(CBaseEntity*, const Vector &origin);
		static Fn func;

		if (!func)
			func = (Fn)(Utils.PatternSearch("client_panorama.dll", (PBYTE)"\x55\x8B\xEC\x83\xE4\xF8\x51\x53\x56\x57\x8B\xF1\xE8\x00\x00", "xxxxxxxxxxxxx??", 0, 0));

		func(this, ArgOrigin);
	}

	std::array<float, 24>& get_ragdoll_pos()
	{
		return *reinterpret_cast<std::array<float, 24>*>(reinterpret_cast<uintptr_t>(this) + 0x00002970);
	}

	bool& GetClientSideAnimation()
	{
		static int offset = Netvar("DT_BaseAnimating", "m_bClientSideAnimation");;
		return *reinterpret_cast<bool*>((DWORD)this + offset);
	}

	int GetNumAnimOverlays()
	{
		return *(int*)((DWORD)this + 0x298C);
	}
	CAnimationLayer *GetAnimOverlays()
	{
		return *(CAnimationLayer**)((DWORD)this + 0x2980);
	}

	float GetSimulationTime()
	{
		static int m_flSimulationTime = Netvar("DT_BaseEntity", "m_flSimulationTime");
		return GetFieldValue< float >( m_flSimulationTime );
	}

	float GetOldSimulationTime()
	{
		static int m_flSimulationTime = Netvar("DT_BaseEntity", "m_flSimulationTime");
		return GetFieldValue< float >(m_flSimulationTime + 0x4);
	}

	Vector GetPunchViewAngle()
	{
		static int m_aimPunchAngle = Netvar("DT_BasePlayer", "localdata", "m_Local", "m_viewPunchAngle");
		return *GetFieldPointer< Vector >( m_aimPunchAngle );
	}

	Vector* GetViewPunchAnglePtr()
	{
		static int m_aimPunchAngle = Netvar("DT_BasePlayer", "localdata", "m_Local", "m_viewPunchAngle");
		return GetFieldPointer< Vector >( m_aimPunchAngle );
	}

	CAnimationLayer *GetAnimOverlay(int index)
	{
		return *(CAnimationLayer**)((DWORD)this + 0x2980);
	}


	int SetFov( int fovvalue )
	{
		static int iOffset = Netvar("DT_BasePlayer", "m_iDefaultFOV");
		*( int* )( ( DWORD )this + iOffset ) = fovvalue;
		static int iOffset2 = Netvar("DT_CSPlayer", "m_bIsScoped");
		bool scopecheck = *( bool* )( ( DWORD )this + iOffset2 );
		if( !scopecheck )
		{
			static int iOffset3 = Netvar("DT_BasePlayer", "m_iFOV");
			*( int* )( ( DWORD )this + iOffset3 ) = fovvalue;
		}
		return true;
	}

	bool GetScope()
	{
		static int iOffset2 = Netvar("DT_CSPlayer", "m_bIsScoped");
		bool scopecheck = *( bool* )( ( DWORD )this + iOffset2 );

		if( scopecheck )
		{
			return true;
		}

		return false;
	}

	CColor GetHealthColor()
	{
		int nHealth = GetHealth();
		float r = 255.f - float( nHealth ) * 2.55f;
		float g = float( nHealth ) * 2.55f;
		return CColor( int( r ), int( g ), 0, 200 );
	}

	std::vector< CBaseCombatWeapon* > GetWeapons();

	CBaseCombatWeapon* GetActiveBaseCombatWeapon();

	bool SetupBones( matrix3x4* pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime )
	{
		//static DWORD m_nWritableBones = Netvar("DT_BaseAnimating", "m_nForceBone");
		//static DWORD m_dwOcclusionArray = *( uintptr_t* )( Utils.PatternSearch( "client_panorama.dll", ( PBYTE ) "\xA1\x00\x00\x00\x00\x8B\xB7\x00\x00\x00\x00\x89\x75\xF8", "x????xx????xxx", 0, 0 ) + 0x1 ); //"A1 ? ? ? ? 8B B7 ? ? ? ? 89 75 F8"
		//static DWORD m_bDidCheckForOcclusion = *( uintptr_t* )( Utils.PatternSearch( "client_panorama.dll", ( PBYTE ) "\xA1\x00\x00\x00\x00\x8B\xB7\x00\x00\x00\x00\x89\x75\xF8", "x????xx????xxx", 0, 0 ) + 0x7 ); //"A1 ? ? ? ? 8B B7 ? ? ? ? 89 75 F8"

		//*( int* )( ( uintptr_t )this + m_nWritableBones ) = 0;
		//*( int* )( ( uintptr_t )this + m_bDidCheckForOcclusion ) = reinterpret_cast< int* >( m_dwOcclusionArray )[ 1 ];

		__asm
		{
			mov edi, this
			lea ecx, dword ptr ds : [edi + 0x4]
			mov edx, dword ptr ds : [ecx]
			push currentTime
			push boneMask
			push nMaxBones
			push pBoneToWorldOut
			call dword ptr ds : [edx + 0x34]
		}

		//typedef bool(__thiscall* oSetupBones)(PVOID, matrix3x4*, int, int, float);
		//return getvfunc< oSetupBones>(this, 13)(this, pBoneToWorldOut, nMaxBones, boneMask, currentTime);
	}

	Vector GetBonePos( int i )
	{
		matrix3x4 boneMatrix[128];
		if( this->SetupBones( boneMatrix, 128, BONE_USED_BY_HITBOX, ( float )GetTickCount64() ) )
		{
			return Vector( boneMatrix[ i ][ 0 ][ 3 ], boneMatrix[ i ][ 1 ][ 3 ], boneMatrix[ i ][ 2 ][ 3 ] );
		}
		return Vector( 0, 0, 0 );
	}

	matrix3x4_t GetBoneMatrix(int BoneID)
	{
		matrix3x4_t matrix;
		static int m_dwBoneMatrix = m_dwBoneMatrix = 0x2698;
		auto offset = *reinterpret_cast<uintptr_t*>(uintptr_t(this) + m_dwBoneMatrix);
		if (offset)
			matrix = *reinterpret_cast<matrix3x4_t*>(offset + 0x30 * BoneID);

		return matrix;
	}

	Vector GetEyePosition()
	{
		return GetVecOrigin() + GetVecViewOffset();
	}

	int GetArmor()
	{
		static int Armor = Netvar("DT_CSPlayerResource", "m_iArmor");
		return GetFieldValue< int >( Armor );
	}

	int GetArmorValue()
	{
		static int Armor = Netvar("DT_CSPlayer", "m_ArmorValue");
		return GetFieldValue< int >( Armor );
	}

	bool HasHelmet()
	{
		static int Helm = Netvar("DT_CSPlayer", "m_bHasHelmet");
		return GetFieldValue< bool >( Helm );
	}

	char* GetArmorIcon()
	{
		if( GetArmor() > 0 )
		{
			if( HasHelmet() )
				return "hk";
			else
				return "k";
		}
		else
			return " ";
	}

	bool HasKit()
	{
		static int iOffset = Netvar("DT_CSPlayer", "m_bHasDefuser");
		return GetFieldValue< bool >( iOffset );
	}

	bool HasGunGameImmunity()
	{
		static int m_bGunGameImmun = Netvar("DT_CSPlayer", "m_bGunGameImmunity");
		return *GetFieldPointer< bool >( m_bGunGameImmun );
	}

	Vector BBMin()
	{
		static int m_vecMins = Netvar("DT_BaseEntity", "m_Collision", "m_vecMins");
		return GetFieldValue< Vector >( m_vecMins );
	}

	Vector BBMax()
	{
		static int m_vecMaxs = Netvar("DT_BaseEntity", "m_Collision", "m_vecMaxs");
		return GetFieldValue< Vector >( m_vecMaxs );
	}

	bool m_bIsScoped()
	{
		static int m_bIsScoped = Netvar("DT_CSPlayer", "m_bIsScoped");
		return GetFieldValue< bool >( m_bIsScoped );
	}

	int GetMoveType()
	{
		return GetFieldValue< int >( 0x258 );
	}

	const char* CompRank()
	{
		static const char* Ranks[19] =
		{
			"-",
			"Silver I",
			"Silver II",
			"Silver III",
			"Silver IV",
			"Silver Elite",
			"Silver Elite Master",

			"Gold Nova I",
			"Gold Nova II",
			"Gold Nova III",
			"Gold Nova Master",
			"Master Guardian I",
			"Master Guardian II",

			"Master Guardian Elite",
			"Distinguished Master Guardian",
			"Legendary Eagle",
			"Legendary Eagle Master",
			"Supreme Master First Class",
			"Global Elite"
		};
		static int m_iCompetitiveRanking = Netvar("DT_CSPlayerResource", "m_iCompetitiveRanking");
		int Rank = *( int* )( ( DWORD )Interfaces.GameResources + ( DWORD )Interfaces.pClient + m_iCompetitiveRanking + ( int )GetIndex() * 4 );
		return Ranks[ Rank ];
	}

	Vector WorldSpaceCenter()
	{
		Vector Max = this->BBMax() + this->GetAbsOrigin();
		Vector Min = this->BBMin() + this->GetAbsOrigin();

		Vector Size = Max - Min;

		Size /= 2;

		Size += Min;

		return Size;
	}

	float pelvisangs()
	{
		static int ioffset = Netvar("DT_CSPlayer", "m_flLowerBodyYawTarget");
		return this->GetFieldValue< float >( ioffset );
	}

	float lastpelvisangs()
	{
		static int ioffset = Netvar("DT_CSPlayer", "m_flLowerBodyYawTarget");
		return this->GetFieldValue< float >(ioffset);
	}
	bool IsValid(bool need_local);
	int GetActivity(int sequence);
};

class CBaseCombatWeapon : public CBaseEntity
{
public:
	template< class T >
	inline T GetFieldValue(int offset)
	{
		return *(T*)((DWORD)this + offset);
	}

	template< class T >
	T* GetFieldPointer(int offset)
	{
		return (T*)((DWORD)this + offset);
	}

	CBaseEntity* GetOwner()
	{
		//static int m_hOwnerEntity = Netvar(("DT_BaseEntity"), ("m_hOwnerEntity"));
		//return (CBaseEntity*)Interfaces.pEntList->GetClientEntityFromHandle(GetFieldValue<CHandle<CBaseEntity>>(m_hOwnerEntity));
		return nullptr;
	}


	int ScopeLevel();

	HANDLE GetOwnerHandle()
	{
		static int m_hOwnerEntity = Netvar(("DT_BaseEntity"), ("m_hOwnerEntity"));
		return GetFieldValue< HANDLE >(m_hOwnerEntity);
	}

	float GetPostponeFireReadyTime()
	{
		static int offset = Netvar("DT_WeaponCSBase", "m_flPostponeFireReadyTime");
		return *(float*)((uintptr_t)this + offset);
	}

	float NextPrimaryAttack()
	{
		static int m_flNextPrimaryAttack = Netvar(("DT_BaseCombatWeapon"), ("LocalActiveWeaponData"), ("m_flNextPrimaryAttack"));
		return GetFieldValue< float >(m_flNextPrimaryAttack);
	}

	int GetClip()
	{
		return *(int*)((DWORD)this + Netvar(("DT_BaseCombatWeapon"), ("m_iClip1")));
	}

	int GetiShots()
	{
		static DWORD m_iShotsFired = 0x0000A2C0;
		return GetFieldValue< int >(m_iShotsFired);
	}

	int GetId()
	{
		typedef int(__thiscall* tGetId)(void*);
		return getvfunc< tGetId >(this, 458)(this);
	}

	const char* GetName()
	{
		typedef const char* (__thiscall* tGetName)(void*);
		return getvfunc< tGetName >(this, 378)(this);
	}

	char* GetPName()
	{
		typedef char* (__thiscall* tGetName)(void*);
		return getvfunc< tGetName >(this, 378)(this);
	}

	short* GetItemDefinitionIndex()
	{
		// DT_BaseAttributableItem -> m_AttributeManager -> m_Item -> m_iItemDefinitionIndex
		static int m_iItemDefinitionIndex = Netvar("DT_BaseAttributableItem", "m_AttributeManager", "m_Item", "m_iItemDefinitionIndex");
		return GetFieldPointer< short >(m_iItemDefinitionIndex);
	}

	int* GetItemIDHigh()
	{
		// DT_BaseAttributableItem -> m_AttributeManager -> m_Item -> m_iItemIDHigh
		static int m_iItemIDHigh = Netvar(("DT_BaseAttributableItem"), ("m_AttributeManager"), ("m_Item"), ("m_iItemIDHigh"));
		return GetFieldPointer< int >(m_iItemIDHigh);
	}

	int* GetAccountID()
	{
		// DT_BaseAttributableItem -> m_AttributeManager -> m_Item -> m_iAccountID
		static int m_iAccountID = Netvar(("DT_BaseAttributableItem"), ("m_AttributeManager"), ("m_Item"), ("m_iAccountID"));
		return GetFieldPointer< int >(m_iAccountID);
	}

	int* GetEntityQuality()
	{
		// DT_BaseAttributableItem -> m_AttributeManager -> m_Item -> m_iEntityQuality
		static int m_iEntityQuality = Netvar(("DT_BaseAttributableItem"), ("m_AttributeManager"), ("m_Item"), ("m_iEntityQuality"));
		return GetFieldPointer< int >(m_iEntityQuality);
	}

	char* GetCustomName()
	{
		// DT_BaseAttributableItem -> m_AttributeManager -> m_Item -> m_szCustomName
		static int m_szCustomName = Netvar(("DT_BaseAttributableItem"), ("m_AttributeManager"), ("m_Item"), ("m_szCustomName"));
		return GetFieldPointer< char >(m_szCustomName);
	}

	int* GetOriginalOwnerXuidLow()
	{
		// DT_BaseAttributableItem -> m_OriginalOwnerXuidLow
		static int m_OriginalOwnerXuidLow = Netvar(("DT_BaseAttributableItem"), ("m_OriginalOwnerXuidLow"));
		return GetFieldPointer< int >(m_OriginalOwnerXuidLow);
	}

	int* GetOriginalOwnerXuidHigh()
	{
		// DT_BaseAttributableItem -> m_OriginalOwnerXuidHigh
		static int m_OriginalOwnerXuidHigh = Netvar(("DT_BaseAttributableItem"), ("m_OriginalOwnerXuidHigh"));
		return GetFieldPointer< int >(m_OriginalOwnerXuidHigh);
	}

	int* GetFallbackPaintKit()
	{
		// DT_BaseAttributableItem -> m_nFallbackPaintKit
		static int m_nFallbackPaintKit = Netvar(("DT_BaseAttributableItem"), ("m_nFallbackPaintKit"));
		return GetFieldPointer< int >(m_nFallbackPaintKit);
	}

	int* GetFallbackSeed()
	{
		// DT_BaseAttributableItem -> m_nFallbackSeed
		static int m_nFallbackSeed = Netvar(("DT_BaseAttributableItem"), ("m_nFallbackSeed"));
		return GetFieldPointer< int >(m_nFallbackSeed);
	}

	float* GetFallbackWear()
	{
		// DT_BaseAttributableItem -> m_flFallbackWear
		static int m_flFallbackWear = Netvar("DT_BaseAttributableItem", "m_flFallbackWear");
		return GetFieldPointer< float >(m_flFallbackWear);
	}

	int* GetFallbackStatTrak()
	{
		// DT_BaseAttributableItem -> m_nFallbackStatTrak
		static int m_nFallbackStatTrak = Netvar(("DT_BaseAttributableItem"), ("m_nFallbackStatTrak"));
		return GetFieldPointer< int >(m_nFallbackStatTrak);
	}

	void CBaseCombatWeapon::UpdateAccuracyPenalty()
	{
		typedef void(__thiscall* OriginalFn)(void*);
		getvfunc< OriginalFn >(this, 472)(this);
	}

	float CBaseCombatWeapon::GetInaccuracy()
	{
		if (!this)
			return 0.f;

		typedef float(__thiscall* OriginalFn)(void*);
		return getvfunc< OriginalFn >(this, 471)(this);
	}

	float CBaseCombatWeapon::GetSpread()
	{
		if (!this)
			return 0.f;

		typedef float(__thiscall* OriginalFn)(void*);
		return getvfunc< OriginalFn >(this, 440)(this);
	}

	int ammo()
	{
		static int m_Clip = Netvar("DT_BaseCombatWeapon", "m_iClip1");
		return *GetFieldPointer< int >(m_Clip);
	}

	int ammo2()
	{
		static int m_Clip = Netvar("DT_BaseCombatWeapon", "m_iClip2");
		return *GetFieldPointer< int >(m_Clip);
	}

	bool HasAmmo()
	{
		if (IsMiscWeapon())
			return false;
		return ammo() > 0;
	}

	bool isPistol()
	{
		int WeaponId = *this->GetItemDefinitionIndex();

		return WeaponId == WEAPON_DEAGLE || WeaponId == WEAPON_DUALBERETTA || WeaponId == WEAPON_FIVESEVEN || WeaponId == WEAPON_P250 ||
			WeaponId == WEAPON_GLOCK || WeaponId == WEAPON_USPS || WeaponId == WEAPON_CZ75 || WeaponId == WEAPON_USP || WeaponId == WEAPON_TEC9 || WeaponId == WEAPON_REVOLVER;
	}

	bool isPistolNotR8()
	{
		int WeaponId = *this->GetItemDefinitionIndex();

		return WeaponId == WEAPON_DEAGLE || WeaponId == WEAPON_DUALBERETTA || WeaponId == WEAPON_FIVESEVEN || WeaponId == WEAPON_P250 ||
			WeaponId == WEAPON_GLOCK || WeaponId == WEAPON_USPS || WeaponId == WEAPON_CZ75 || WeaponId == WEAPON_USP || WeaponId == WEAPON_TEC9;
	}

	bool isShotgun()
	{
		int WeaponId = *this->GetItemDefinitionIndex();

		return WeaponId == WEAPON_XM1014 || WeaponId == WEAPON_NOVA || WeaponId == WEAPON_SAWEDOFF || WeaponId == WEAPON_MAG7;
	}

	bool isSniper()
	{
		int WeaponId = *this->GetItemDefinitionIndex();

		return WeaponId == WEAPON_AWP || WeaponId == WEAPON_SCAR20 || WeaponId == WEAPON_G3SG1 || WeaponId == WEAPON_SSG08;
	}

	bool isScoped()
	{
		int WeaponId = *this->GetItemDefinitionIndex();

		return WeaponId == WEAPON_AWP || WeaponId == WEAPON_SCAR20 || WeaponId == WEAPON_G3SG1 || WeaponId == WEAPON_SSG08 || WeaponId == WEAPON_SG553 || WeaponId == WEAPON_AUG;
	}

	bool isAWP()
	{
		int WeaponId = *this->GetItemDefinitionIndex();

		return WeaponId == WEAPON_AWP;
	}

	bool isCZ()
	{
		int WeaponId = *this->GetItemDefinitionIndex();

		return WeaponId == WEAPON_CZ75;
	}

	bool isScout()
	{
		int WeaponId = *this->GetItemDefinitionIndex();

		return WeaponId == WEAPON_SSG08;
	}

	bool isDak()
	{
		int WeaponId = *this->GetItemDefinitionIndex();

		return WeaponId == WEAPON_SCAR20 || WeaponId == WEAPON_G3SG1;
	}

	bool IsGun()
	{
		int iWeaponID = *GetItemDefinitionIndex();

		if (iWeaponID == WEAPON_KNIFE
			|| iWeaponID == WEAPON_C4
			|| iWeaponID == WEAPON_HEGRENADE || iWeaponID == WEAPON_DECOY
			|| iWeaponID == WEAPON_FLASHBANG || iWeaponID == WEAPON_MOLOTOV
			|| iWeaponID == WEAPON_SMOKEGRENADE || iWeaponID == WEAPON_INC
			|| iWeaponID == 500 || iWeaponID == 505 || iWeaponID == 506
			|| iWeaponID == 507 || iWeaponID == 508 || iWeaponID == 509
			|| iWeaponID == 515 || iWeaponID == KNIFE_PUSH ||
			iWeaponID == KNIFE_STILETTO ||
			iWeaponID == KNIFE_URSUS ||
			iWeaponID == KNIFE_WIDOWMAKER)
			return false;
		else
			return true;
	}

	bool isRifle()
	{
		int WeaponId = *this->GetItemDefinitionIndex();

		return WeaponId == WEAPON_AK47 || WeaponId == WEAPON_M4A1 || WeaponId == WEAPON_M4A1S || WeaponId == WEAPON_GALILAR || WeaponId == WEAPON_FAMAS ||
			WeaponId == WEAPON_AUG || WeaponId == WEAPON_SG553;
	}

	bool isSmgW()
	{
		int WeaponId = *this->GetItemDefinitionIndex();

		return WeaponId == WEAPON_MAC10 || WeaponId == WEAPON_MP7 || WeaponId == WEAPON_MP9 || WeaponId == WEAPON_P90 ||
			WeaponId == WEAPON_BIZON || WeaponId == WEAPON_UMP45;
	}

	bool isMachineW()
	{
		int WeaponId = *this->GetItemDefinitionIndex();

		return WeaponId == WEAPON_NEGEV || WeaponId == WEAPON_M249;
	}

	bool IsNade()
	{
		int WeaponId = *this->GetItemDefinitionIndex();

		return WeaponId == WEAPON_FLASHBANG || WeaponId == WEAPON_HEGRENADE || WeaponId == WEAPON_SMOKEGRENADE ||
			WeaponId == WEAPON_MOLOTOV || WeaponId == WEAPON_DECOY || WeaponId == WEAPON_INC;
	}

	float hitchance()
	{
		float inaccuracy = GetInaccuracy();
		if (inaccuracy == 0)
			inaccuracy = 0.0000001;
		inaccuracy = 1 / inaccuracy;
		return inaccuracy;
	}

	char* GetGunName()
	{
		int WeaponId = *this->GetItemDefinitionIndex();
		switch (WeaponId)
		{
		case WEAPON_AK47:
			return "AK47";
			break;
		case WEAPON_AUG:
			return "AUG";
			break;
		case WEAPON_REVOLVER:
			return "Revolver";
			break;
		case WEAPON_AWP:
			return "AWP";
		case WEAPON_MP5SD:
			return "MP5 SD";
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
		case WEAPON_GALILAR:
			return "Galil";
			break;
		case WEAPON_GLOCK:
			return "Glock";
			break;
		case WEAPON_INC:
			return "Molotov";
			break;
		case WEAPON_MOLOTOV:
			return "Molotov";
			break;
		case WEAPON_SSG08:
			return "Scout";
			break;
		case WEAPON_HEGRENADE:
			return "Grenade";
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
			return "USP";
			break;
		case WEAPON_UMP45:
			return "UMP";
			break;
		case WEAPON_XM1014:
			return "XM1014";
			break;
		case WEAPON_TASER:
			return "Zeus";
			break;
		case WEAPON_M4A1S:
			return "M4A1-S";
		}
		if (IsKnife())
			return "Knife";
		return "";
	}

	char* GetGunIcon()
	{
		int WeaponId = *this->GetItemDefinitionIndex();
		switch (WeaponId)
		{
		case WEAPON_KNIFE:
		case 500:
		case 505:
		case 506:
		case 507:
		case 508:
		case 509:
		case 512:
		case 514:
		case 515:
		case 516:
		case 520:
		case 522:
		case 519:
		case 523:
			return "]";
		case WEAPON_DEAGLE:
			return "A";
		case WEAPON_DUALBERETTA:
			return "B";
		case WEAPON_FIVESEVEN:
			return "C";
		case WEAPON_GLOCK:
			return "D";
		case WEAPON_USP:
			return "E";
		case WEAPON_P250:
			return "F";
		case WEAPON_USPS:
			return "G";
		case WEAPON_TEC9:
			return "H";
		case WEAPON_CZ75:
			return "I";
		case WEAPON_REVOLVER:
			return "J";
		case WEAPON_MAC10:
			return "K";
		case WEAPON_UMP45:
			return "L";
		case WEAPON_BIZON:
			return "M";
		case WEAPON_MP7:
			return "N";
		case WEAPON_MP9:
			return "O";
		case WEAPON_P90:
			return "P";
		case WEAPON_GALILAR:
			return "Q";
		case WEAPON_FAMAS:
			return "R";
		case WEAPON_M4A1S:
			return "S";
		case WEAPON_M4A1:
			return "T";
		case WEAPON_AUG:
			return "U";
		case WEAPON_SG553:
			return "V";
		case WEAPON_AK47:
			return "W";
		case WEAPON_G3SG1:
			return "X";
		case WEAPON_SCAR20:
			return "Y";
		case WEAPON_AWP:
			return "Z";
		case WEAPON_SSG08:
			return "a";
		case WEAPON_XM1014:
			return "b";
		case WEAPON_SAWEDOFF:
			return "c";
		case WEAPON_MAG7:
			return "d";
		case WEAPON_NOVA:
			return "e";
		case WEAPON_NEGEV:
			return "f";
		case WEAPON_M249:
			return "g";
		case WEAPON_TASER:
			return "h";
		case WEAPON_FLASHBANG:
			return "i";
		case WEAPON_HEGRENADE:
			return "j";
		case WEAPON_SMOKEGRENADE:
			return "k";
		case WEAPON_MOLOTOV:
			return "l";
		case WEAPON_DECOY:
			return "m";
		case WEAPON_INC:
			return "n";
		case WEAPON_C4:
			return "o";
		default:
			return " ";
		}
	}

	bool IsMiscWeapon()
	{
		int iWeaponID = *this->GetItemDefinitionIndex();
		return (iWeaponID == WEAPON_KNIFE
			|| iWeaponID == WEAPON_C4
			|| iWeaponID == WEAPON_HEGRENADE || iWeaponID == WEAPON_DECOY
			|| iWeaponID == WEAPON_FLASHBANG || iWeaponID == WEAPON_MOLOTOV
			|| iWeaponID == WEAPON_SMOKEGRENADE || iWeaponID == WEAPON_INC || iWeaponID == WEAPON_KNIFE_T
			|| iWeaponID == 500 || iWeaponID == 505 || iWeaponID == 506
			|| iWeaponID == 507 || iWeaponID == 508 || iWeaponID == 509
			|| iWeaponID == 515 || iWeaponID == KNIFE_PUSH ||
			iWeaponID == KNIFE_STILETTO ||
			iWeaponID == KNIFE_URSUS ||
			iWeaponID == KNIFE_WIDOWMAKER);
	}

	bool IsGrenade()
	{
		int iWeaponID = *this->GetItemDefinitionIndex();
		return ( iWeaponID == WEAPON_DECOY
			|| iWeaponID == WEAPON_FLASHBANG || iWeaponID == WEAPON_MOLOTOV
			|| iWeaponID == WEAPON_SMOKEGRENADE || iWeaponID == WEAPON_INC);
	}

	bool IsKnife()
	{
		int iWeaponID = *this->GetItemDefinitionIndex();
		return ( iWeaponID == WEAPON_KNIFE_T || iWeaponID == WEAPON_KNIFE
			|| iWeaponID == WEAPON_KNIFEGG || iWeaponID == 59 || iWeaponID == 41
			|| iWeaponID == 500 || iWeaponID == 505 || iWeaponID == 506
			|| iWeaponID == 507 || iWeaponID == 508 || iWeaponID == 509
			|| iWeaponID == 515 || iWeaponID == KNIFE_PUSH ||
			iWeaponID == KNIFE_STILETTO ||
			iWeaponID == KNIFE_URSUS ||
			iWeaponID == KNIFE_WIDOWMAKER);
	}

	bool IsReloading()
	{
		false; // GET THIS, ISNT A NETVAR?
	}

	CCSWeaponInfo* GetCSWpnData()
	{
		if (!this) return nullptr;
		typedef CCSWeaponInfo*(__thiscall* OriginalFn)(void*);
		return getvfunc<OriginalFn>(this, 448)(this);
	}
};


class CBaseCSGrenade : CBaseCombatWeapon
{
public:
	float GetPinPulled()
	{
		static int m_bPinPulled = Netvar(("DT_BaseCSGrenade"), ("m_bPinPulled"));
		return GetFieldValue< bool >( m_bPinPulled );
	}

	float GetThrowTime()
	{
		static int m_fThrowTime = Netvar(("DT_BaseCSGrenade"), ("m_fThrowTime"));
		return *( float* )( ( uintptr_t )this + m_fThrowTime );
	}
};
