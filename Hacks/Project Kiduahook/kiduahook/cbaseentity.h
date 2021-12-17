#pragma once

#include "animation.h"
#include "sdk.h"

using CBaseHandle = unsigned long;

class ICollideable;
class IClientRenderable;
class IClientEntity;
class C_BaseEntity
{
public:
	bool IsPlayer()
	{
		typedef bool(__thiscall* oIsPlayer)(PVOID);
		return getvfunc< oIsPlayer >(this, 155)(this);
	}

	int GetIndex()
	{
		PVOID pRenderable = (void*)((DWORD)this + 0x8);
		typedef int(__thiscall* OriginalFn)(PVOID);
		return getvfunc<OriginalFn>(pRenderable, 10)(pRenderable);
	}
};
class IClientThinkable;
class IClientNetworkable;
class IClientAlphaProperty;
struct bf_read;
struct DrawModelFlags_t;

class IHandleEntity
{
public:
    virtual ~IHandleEntity()
    {
    }
    virtual void SetRefEHandle( const CBaseHandle& handle ) = 0;
    virtual const CBaseHandle& GetRefEHandle() const = 0;
};

class IClientUnknown : public IHandleEntity
{
public:
    virtual ICollideable*			GetCollideable() = 0;
    virtual IClientNetworkable*		GetClientNetworkable() = 0;
    virtual IClientRenderable*		GetClientRenderable() = 0;
    virtual IClientEntity*			GetIClientEntity() = 0;
    virtual C_BaseEntity*			GetBaseEntity() = 0;
    virtual IClientThinkable*		GetClientThinkable() = 0;
    //virtual IClientModelRenderable*	GetClientModelRenderable() = 0;
    virtual IClientAlphaProperty*	GetClientAlphaProperty() = 0;
};

class IClientThinkable
{
public:
    virtual ~IClientThinkable()
    {
    };
};
class IClientRenderable
{
public:
	virtual IClientUnknown*            GetIClientUnknown() = 0;
	virtual Vector const&              GetRenderOrigin( void ) = 0;
	virtual QAngle const&              GetRenderAngles( void ) = 0;
	virtual bool                       ShouldDraw( void ) = 0;
	virtual int                        GetRenderFlags( void ) = 0; // ERENDERFLAGS_xxx
	virtual void                       Unused( void ) const
	{
	}
	virtual HANDLE                     GetShadowHandle() const = 0;
	virtual HANDLE&                    RenderHandle() = 0;
	virtual const model_t*             GetModel() const = 0;
	virtual int                        DrawModel( int flags, const int /*RenderableInstance_t*/ &instance ) = 0;
	virtual int                        GetBody() = 0;
	virtual void                       GetColorModulation( float* color ) = 0;
	virtual bool                       LODTest() = 0;
	virtual bool                       SetupBones( matrix3x4_t *pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime ) = 0;
	virtual void                       SetupWeights( const matrix3x4_t *pBoneToWorld, int nFlexWeightCount, float *pFlexWeights, float *pFlexDelayedWeights ) = 0;
	virtual void                       DoAnimationEvents( void ) = 0;
	virtual void* /*IPVSNotify*/       GetPVSNotifyInterface() = 0;
	virtual void                       GetRenderBounds( Vector& mins, Vector& maxs ) = 0;
	virtual void                       GetRenderBoundsWorldspace( Vector& mins, Vector& maxs ) = 0;
	virtual void                       GetShadowRenderBounds( Vector &mins, Vector &maxs, int /*ShadowType_t*/ shadowType ) = 0;
	virtual bool                       ShouldReceiveProjectedTextures( int flags ) = 0;
	virtual bool                       GetShadowCastDistance( float *pDist, int /*ShadowType_t*/ shadowType ) const = 0;
	virtual bool                       GetShadowCastDirection( Vector *pDirection, int /*ShadowType_t*/ shadowType ) const = 0;
	virtual bool                       IsShadowDirty() = 0;
	virtual void                       MarkShadowDirty( bool bDirty ) = 0;
	virtual IClientRenderable*         GetShadowParent() = 0;
	virtual IClientRenderable*         FirstShadowChild() = 0;
	virtual IClientRenderable*         NextShadowPeer() = 0;
	virtual int /*ShadowType_t*/       ShadowCastType() = 0;
	virtual void                       CreateModelInstance() = 0;
	virtual HANDLE                     GetModelInstance() = 0;
	virtual const matrix3x4_t&         RenderableToWorldTransform() = 0;
	virtual int                        LookupAttachment( const char *pAttachmentName ) = 0;
	virtual   bool                     GetAttachment( int number, Vector &origin, QAngle &angles ) = 0;
	virtual bool                       GetAttachment( int number, matrix3x4_t &matrix ) = 0;
	virtual float*                     GetRenderClipPlane( void ) = 0;
	virtual int                        GetSkin() = 0;
	virtual void                       OnThreadedDrawSetup() = 0;
	virtual bool                       UsesFlexDelayedWeights() = 0;
	virtual void                       RecordToolMessage() = 0;
	virtual bool                       ShouldDrawForSplitScreenUser( int nSlot ) = 0;
	virtual unsigned char              OverrideAlphaModulation( uint8_t nAlpha ) = 0;
	virtual unsigned char              OverrideShadowAlphaModulation( uint8_t nAlpha ) = 0;
};

class IClientNetworkable
{
public:
    virtual IClientUnknown*	GetIClientUnknown() = 0;
    virtual void			Release() = 0;
    virtual ClientClass*	GetClientClass() = 0;
    virtual void			NotifyShouldTransmit( int state ) = 0;
    virtual void			OnPreDataChanged( int updateType ) = 0;
    virtual void			OnDataChanged( int updateType ) = 0;
    virtual void			PreDataUpdate( int updateType ) = 0;
    virtual void			PostDataUpdate( int updateType ) = 0;
    virtual void			OnDataUnchangedInPVS() = 0;
    virtual bool			IsDormant() = 0;
    virtual int				EntIndex() const = 0;
    virtual void			ReceiveMessage( int classID, bf_read& msg ) = 0;
    virtual void*			GetDataTableBasePtr() = 0;
    virtual void			SetDestroyedOnRecreateEntities() = 0;
};

class IClientEntity : public IClientUnknown, public IClientRenderable, public IClientNetworkable, public IClientThinkable
{
public:
    virtual ~IClientEntity()
    {
    };
};

class CBaseEntity : public IClientEntity
{
public:
	DWORD SigScan( std::string moduleName, std::string pattern )
	{
		const char* pat = pattern.c_str();
		DWORD firstMatch = 0;
		DWORD rangeStart = ( DWORD ) GetModuleHandleA( moduleName.c_str() );
		MODULEINFO miModInfo; GetModuleInformation( GetCurrentProcess(), ( HMODULE ) rangeStart, &miModInfo, sizeof( MODULEINFO ) );
		DWORD rangeEnd = rangeStart + miModInfo.SizeOfImage;
		for( DWORD pCur = rangeStart; pCur < rangeEnd; pCur++ )
		{
			if( !*pat )
				return firstMatch;

			if( *( PBYTE ) pat == '\?' || *( BYTE* ) pCur == getByte( pat ) )
			{
				if( !firstMatch )
					firstMatch = pCur;

				if( !pat[ 2 ] )
					return firstMatch;

				if( *( PWORD ) pat == '\?\?' || *( PBYTE ) pat != '\?' )
					pat += 3;

				else
					pat += 2;    //one ?
			}
			else
			{
				pat = pattern.c_str();
				firstMatch = 0;
			}
		}
		return NULL;
	}

/*	Vector& GetAbsOrigin()
	{
		__asm
		{
			MOV ECX, this
				MOV EAX, DWORD PTR DS : [ECX]
				CALL DWORD PTR DS : [EAX + 0x28]
		}
	}

	QAngle GetAbsAngles()
	{
		__asm
		{
			MOV ECX, this;
			MOV EAX, DWORD PTR DS : [ECX]; Ge
				CALL DWORD PTR DS : [EAX + 0x2C]
		}
	}*/

	Vector& GetAbsOrigin()
	{
		typedef Vector&( __thiscall* OriginalFn )( PVOID );
		return getvfunc<OriginalFn>( this, 10 )( this );
	}

	QAngle& GetAbsAngles()
	{
		typedef QAngle&( __thiscall* OriginalFn )( PVOID );
		return getvfunc<OriginalFn>( this, 11 )( this );
	}

	CBaseHandle GetViewModel() 
	{
		return *( CBaseHandle* ) ( ( DWORD ) this + Offsets::Player::m_hViewModel );
	}

	int GetWeaponHandle()
	{
		return *( int* ) ( ( DWORD ) this + Offsets::Player::m_hWeapon );
	}

	int GetActiveWeaponIndex()
	{
		return *reinterpret_cast<int*>( uintptr_t( this ) + Offsets::Player::m_hActiveWeapon ) & 0xFFF;
	}

	int* GetModelIndex()
	{
		return ( int* ) ( ( DWORD )this + Offsets::Weapon::m_nModelIndex );
	}

	UINT* GetMyWeapons()
	{
		return (UINT*)((DWORD)this + Offsets::Weapon::m_nModelIndex);
	}

	unsigned int PhysicsSolidMaskForEntity()
	{
		typedef unsigned int( __thiscall* oPhysicsSolidMaskForEntity )( PVOID );
		return getvfunc< oPhysicsSolidMaskForEntity >( this, 148 )( this );
	}

	bool IsPlayer()
	{
		typedef bool( __thiscall* oIsPlayer )( PVOID );
		return getvfunc< oIsPlayer >( this, 157 )( this );
	}

	bool IsBroken( void )
	{
		return *( bool* )( ( DWORD )this + Offsets::Weapon::m_bIsBroken );
	}

	int GetObserverTarget()
	{
		return *( int* )( ( DWORD )this + Offsets::Player::m_hObserverTarget );
	}

	int GetObserverMode()
	{
		return *( int* )( ( DWORD )this + Offsets::Player::m_iObserverMode );
	}

	matrix3x4_t GetBoneMatrix( int BoneID )
	{
		__try
		{
			matrix3x4_t matrix;

			auto offset = *reinterpret_cast<uintptr_t*>(uintptr_t(this) + 0x26A8);

			if (offset)
				matrix = *reinterpret_cast<matrix3x4_t*>(offset + 0x30 * BoneID);

			return matrix;
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			//g_pICvar->ConsoleColorPrintf(Color::Red(), "[KIDUAHOOK] GetBoneMatrix exception!\n");
			return matrix3x4_t();
		}
	}

	matrix3x4_t** BoneCachePointer()
	{
		// xref: "Model '%s' has skin but thinks it can render fastpath\n"
		// sig: 8B 87 ? ? ? ? 89 BF
		// Look for if (m_pIk), should be right above that.
		//renderablenigger + 0x28FC
		//  if ( retaddr >= cacheptr || cacheptr > v82 ) | in setupbones *** ERROR: Bone access not allowed (entity %i:%s)\n
		return (matrix3x4_t** ) ( ( uintptr_t ) this + 0x2910 );
	}

	matrix3x4_t* GetCachePointer()
	{
		return *reinterpret_cast<matrix3x4_t**>( uintptr_t( this ) + 0x2910 );
	}

	ClientClass* GetClientClass()
	{
		PVOID pNetworkable = ( void* )( ( DWORD )this + 0x8 );
		typedef ClientClass*( __thiscall* OriginalFn )( PVOID );
		return getvfunc<OriginalFn>( pNetworkable, 2 )( pNetworkable );
	}

	CClientClasses* GetClientClasses()
	{
		PVOID pNetworkable = ( void* ) ( ( DWORD )this + 0x8 );
		typedef CClientClasses*( __thiscall* OriginalFn )( PVOID );
		return getvfunc<OriginalFn>( pNetworkable, 2 )( pNetworkable );
	}

	CUtlVector<CAnimationLayer>& GetAnimOverlays()
	{
		return ( *reinterpret_cast< CUtlVector<CAnimationLayer>* >( reinterpret_cast< std::uintptr_t >( this ) + 0x2980 ) );
	}

	CAnimationLayer * GetAnimOverlay( int i )
	{
		Assert( i >= 0 && i < 15 );
		return &GetAnimOverlays()[ i ];
	}

	CAnimationLayer& GetAnimationOverlay( int Index )
	{
		//v5 = *(_DWORD *)(v7 + 0x298C) ? *(_DWORD *)(v7 + 0x2980) + 56 * *v4 : 0;
		return ( *( CAnimationLayer** ) ( ( DWORD )this + 0x2980 ) )[ Index ];
	}

	CAnimationLayer *GetAnimOverlay()
	{
		// to find offset: use 9/12/17 dll
		// sig: 55 8B EC 51 53 8B 5D 08 33 C0
		return *( CAnimationLayer** ) ( ( DWORD ) this + 10624 );
	}

	CAnimationLayer GetAnimationLayers( int iIndex )
	{
		// to find offset: use 9/12/17 dll
		// sig: 55 8B EC 51 53 8B 5D 08 33 C0
		return ( *( CAnimationLayer** ) ( ( DWORD )this + 0x2980 ) )[ iIndex ];
	}

	void SetAnimOverlay( int Index, CAnimationLayer layer )
	{
		( *( CAnimationLayer** ) ( ( DWORD )this + 0x2980 ) )[ Index ] = layer;
	}

	int GetNumAnimOverlays()
	{
		return GetAnimOverlays().Count();
	}

	int GetNumOverlay()
	{
		return *( int* ) ( ( DWORD ) this + 0x297C + 0x10 );
	}

	float GetSpawnTime()
	{
		//NETVAR_OFFSET(spawn_time, float, "DT_CSPlayer", "m_iAddonBits", -0x4);
	//	return *( float* ) ( ( DWORD ) this + 0xA290 );

		static uintptr_t m_iAddonBits = g_pNetVars->GetOffset( "DT_CSPlayer", "m_iAddonBits" ) - 0x4;
		return *reinterpret_cast<float*>( uintptr_t( this ) + m_iAddonBits );
	}

	int* GetWearables()
	{
		static const int m_hMyWearables = g_pNetVars->GetOffset("DT_BaseCombatCharacter", "m_hMyWearables");
		return (int*)((uintptr_t)this + m_hMyWearables);
	}

	void SetAbsOrigin( Vector vec )
	{
		using SetAbsOriginFn = void( __thiscall* )( void*, const Vector &origin );
		static SetAbsOriginFn SetAbsOrigin;

		if( !SetAbsOrigin )
			SetAbsOrigin = reinterpret_cast< SetAbsOriginFn >( SigScan( "client.dll", "55 8B EC 83 E4 F8 51 53 56 57 8B F1" ) );

		SetAbsOrigin( this, vec );
	}

	void SetAbsAngles( QAngle vec )
	{
		using SetAbsAnglesFn = void( __thiscall* )( void*, const QAngle &angles );
		static SetAbsAnglesFn SetAbsAngles;

		if( !SetAbsAngles )
			SetAbsAngles = reinterpret_cast< SetAbsAnglesFn >( ( uintptr_t ) SigScan( "client.dll", "55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1 E8" ) );

		SetAbsAngles( this, vec );
	}

	void SetEyeAngles( Vector angles )
	{
		*reinterpret_cast<Vector*>( uintptr_t( this ) + Offsets::Player::m_angEyeAngles ) = angles;
	}

	void SetVelocity( Vector velocity )
	{
		*reinterpret_cast<Vector*>( uintptr_t( this ) + Offsets::Player::m_vecVelocity ) = velocity;
	}

	void SetFlags( int flags )
	{
		*reinterpret_cast<int*>( uintptr_t( this ) + Offsets::Player::m_fFlags ) = flags;
	}

	void SetLowerBody( float lby )
	{
		*reinterpret_cast< float* >( uintptr_t( this ) + Offsets::Player::m_flLowerBodyYawTarget ) = lby;
	}

	void SetRenderAngles( QAngle angles )
	{
		*( QAngle* ) ( ( DWORD )this + 0x128 ) = angles;
	}

	std::array<float, 24> &GetPoseParams()
	{
		static int _m_flPoseParameter = Offsets::Player::m_flPoseParameter;
		return *( std::array<float, 24>* )( ( uintptr_t ) this + _m_flPoseParameter );
	}

	std::array<float, 24>& GetRagPos()
	{
		static int m_ragPos = g_pNetVars->GetOffset( "DT_Ragdoll", "m_ragPos" );
		return *( std::array<float, 24>* )( ( uintptr_t ) this + m_ragPos );
	}

	float *GetRagPosF()
	{
		static int m_ragPos = g_pNetVars->GetOffset( "DT_Ragdoll", "m_ragPos" );
		return ( float* ) ( ( DWORD ) this + m_ragPos );
	}

	void SetPoseAngles( float yaw, float pitch )
	{
		auto &poses = GetPoseParams();
		poses [ 11 ] = ( pitch + 90 ) / 180;
		poses [ 2 ] = ( yaw + 180 ) / 360;
	}

	float *GetPoseParam()
	{
		return ( float* ) ( ( DWORD ) this + Offsets::Player::m_flPoseParameter );
	}

	void UpdateAnimation()
	{
		typedef void( __thiscall* UpdateClientSideAnimationFn )( PVOID );
		getvfunc<UpdateClientSideAnimationFn>( this, 223 )( this );
	}

	bool& IsAnimating()
	{
		return getvfunc<bool&( __thiscall* )( void* )>( this, 154 )( this );
	}

/*	void BuildTransformations( void* pStudioHdr, Vector *pos, Quaternion* q, matrix3x4_t& cameraTransform, int boneMask, byte *computed )
	{
		typedef void( __thiscall* oStandardBlendingRules )( void*, void*, Vector*, Quaternion*, matrix3x4_t&, int, byte* );
		getvfunc<oStandardBlendingRules>( this, 184 )( this, pStudioHdr, pos, q, cameraTransform, boneMask, computed );
	}

	void StandardBlendingRules( void* pStudioHdr, Vector pos[], Quaternion q[], float currentTime, int boneMask )
	{
		typedef void( __thiscall* oStandardBlendingRules )( void*, void*, Vector[], Quaternion[] , float, int );
		getvfunc<oStandardBlendingRules>( this, 200 )( this, pStudioHdr, pos, q, currentTime, boneMask );
	}*/

	void DrawModel()
	{
		PVOID pRenderable = ( void* )( ( DWORD )this + 0x4 );
		typedef void( __thiscall* OriginalFn )( PVOID, int, const int /*RenderableInstance_t*/ &instance );
		getvfunc<OriginalFn>( pRenderable, 9 )( pRenderable, 0x1, 255 );
	}

	int GetIndex()
	{
		PVOID pRenderable = ( void* ) ( ( DWORD )this + 0x8 );
		typedef int( __thiscall* OriginalFn )( PVOID );
		return getvfunc<OriginalFn>( pRenderable, 10 )( pRenderable );
	}

	bool IsDormant()
	{
		PVOID pRenderable = ( void* ) ( ( DWORD )this + 0x8 );
		typedef bool( __thiscall* OriginalFn )( PVOID );
		return getvfunc<OriginalFn>( pRenderable, 9 )( pRenderable );
	}

	model_t* GetModel()
	{
		PVOID pRenderable = ( void* ) ( ( DWORD )this + 0x4 );
		typedef model_t*( __thiscall* OriginalFn )( PVOID );
		return getvfunc<OriginalFn>( pRenderable, 8 )( pRenderable );
	}

	bool SetupBones(matrix3x4_t* matrix, int maxbones, int mask, float time)
	{
		PVOID pRenderable = (void*)((DWORD)this + 0x4);
		typedef bool(__thiscall* OriginalFn)(PVOID, matrix3x4_t*, int, int, float);
		return getvfunc<OriginalFn>(pRenderable, 13)(pRenderable, matrix, maxbones, mask, time);
	}
	
	void InvalidateBoneCache()
	{
		//void thiscall void
		/*
		23F8C750  /$ 803D 883A8424 >CMP BYTE PTR DS:[24843A88],0 // s_bInvalidateBoneCache ?
		23F8C757  |. 74 16          JE SHORT client.23F8C76F // skip if its 0 (thats why its rebuild below)
		23F8C759  |. A1 B480D628    MOV EAX,DWORD PTR DS:[28D680B4] // eax = *(DWORD*)0x28D680B4 (GetMostRecentBoneCounter)
		23F8C75E  |. 48             DEC EAX // EAX = GetMostRecentBoneCounter - 1
		23F8C75F  |. C781 14290000 >MOV DWORD PTR DS:[ECX+2914],FF7FFFFF // GetLastBoneSetupTime -FLT_MAX
		23F8C769  |. 8981 80260000  MOV DWORD PTR DS:[ECX+2680],EAX // 2680 = GetWritableBones, will be set to our decremented GetMostRecentBoneCounter
		23F8C76F  \> C3             RETN
		*/

		static auto SigResult = SigScan("client.dll", "80 3D ? ? ? ? ? 74 16 A1 ? ? ? ? 48 C7 81");

		unsigned long g_iModelBoneCounter = **(unsigned long**)(SigResult + 10);
		*(unsigned int*)((DWORD)this + 0x2914) = 0xFF7FFFFF; // m_flLastBoneSetupTime = -FLT_MAX;
		*(unsigned int*)((DWORD)this + 0x2680) = (g_iModelBoneCounter - 1); // m_iMostRecentModelBoneCounter = g_iModelBoneCounter - 1;
	}

/*	int GetIndex()
	{
		if( this == nullptr )
			return 0;

		__asm
		{
			MOV EDI, this
				LEA ECX, DWORD PTR DS : [EDI + 0x8]	//pointer to iclientrenderable (cbaseentity + 0x8)
				MOV EDX, DWORD PTR DS : [ECX]
				CALL DWORD PTR DS : [EDX + 0x28]	//pointer to getindex()
		}
	}

	bool IsDormant()
	{
		__asm
		{
			MOV EDI, this
				LEA ECX, [ EDI + 0x8 ]
				MOV EDX, DWORD PTR DS : [ecx]
				CALL[ EDX + 0x24 ]
		}
	}

	model_t* GetModel()
	{
		__asm
		{
			MOV EDI, this
				LEA ECX, [ EDI + 0x4 ]
				MOV EDX, DWORD PTR DS : [ECX]
				CALL[ EDX + 0x20 ]
		}
	}

	bool SetupBones( matrix3x4* matrix, int maxbones, int mask, float time )
	{
		__asm
		{
			MOV EDI, this
				LEA ECX, DWORD PTR DS : [EDI + 0x4]
				MOV EDX, DWORD PTR DS : [ECX]
				PUSH time
				PUSH mask
				PUSH maxbones
				PUSH matrix
				CALL DWORD PTR DS : [EDX + 0x34]
		}
	}*/

	int GetHealth()
	{
		return *( int* )( ( DWORD )this + Offsets::Player::m_iHealth );
	}

	int GetArmor()
	{
		return *( int* )( ( DWORD )this + Offsets::Player::m_ArmorValue );
	}

	int GetTeamNumber()
	{
		return *( int* )( ( DWORD )this + Offsets::Player::m_iTeamNum );
	}

	int GetBrTeam()
	{
		static uintptr_t m_nSurvivalTeam = g_pNetVars->GetOffset("DT_CSPlayer", "m_nSurvivalTeam");
		return *reinterpret_cast<int*>(uintptr_t(this) + m_nSurvivalTeam);
	}

	int GetFlags()
	{
		return *(int*)((DWORD)this + Offsets::Player::m_fFlags);
	}

	int GetEffects()
	{
		return *(int*)((DWORD)this + Offsets::Entity::m_fEffects);
	}

	void SetEffects(int Eff)
	{
		*(int*)((DWORD)this + Offsets::Entity::m_fEffects) = Eff;
	}

	int m_Local()
	{
		return *( int* )( ( DWORD )this + Offsets::Player::m_Local );
	}

	int GetTickBase()
	{
		return *( int* )( ( DWORD )this + Offsets::Player::m_nTickBase );
	}

	int GetShotsFired()
	{
		return *( int* )( ( DWORD )this + Offsets::Player::m_iShotsFired );
	}

	int GetCash()
	{
		return *( int* )( ( DWORD )this + Offsets::Player::m_iAccount );
	}

	int GetHitboxSet()
	{
		return *( int * ) ( ( DWORD ) this + Offsets::Player::m_nHitboxSet );
	}

    float GetFallVelocity()
	{
        return *(float*)( (DWORD)this + Offsets::Player::m_flFallVelocity );
	}

    float m_flStepSize()
    {
        return *(float*)( (DWORD)this + Offsets::Player::m_flStepSize );
    }

    float m_flMaxspeed()
	{
        return *(float*)( (DWORD)this + Offsets::Player::m_flMaxspeed );
	}

	float DuckAmount()//quack
	{
		return *(float*)((DWORD)this + Offsets::Player::m_flDuckAmount);
	}

    Vector m_vecBaseVelocity()
    {
        return *(Vector*)( (DWORD)this + Offsets::Player::m_vecBaseVelocity );
    }

	float LowerBodyYawTarget()
	{
		return *( float* )( ( DWORD )this + Offsets::Player::m_flLowerBodyYawTarget );
	}

	float GetSimulationTime()
	{
		return *( float* )( ( DWORD )this + Offsets::Player::m_flSimulationTime );
	}

	float GetOldSimulationTime()
	{
		return *( float* ) ( ( DWORD )this + Offsets::Player::m_flSimulationTime + 0x4 );
	}

	float GetAnimTime()
	{
		return this->GetOldSimulationTime() + g_pGlobalVars->interval_per_tick;
	}

	float GetMaxSpeed()
	{
		return *( float* )( ( DWORD )this + Offsets::Player::m_flMaxSpeed );
	}

	float GetC4Blow()
	{
		return *( float* )( ( DWORD )this + Offsets::Player::m_flC4Blow );
	}

	float GetDefuseLenght()
	{
		return *( float* )( ( DWORD )this + Offsets::Player::m_flDefuseLength );
	}

	float GetYawAngles()
	{
		return *( float* )( ( DWORD )this + Offsets::Player::m_angEyeAnglesY );
	}

	float GetNextAttack()
	{
		return *( float* ) ( ( DWORD )this + Offsets::Weapon::m_flNextAttack );
	}

	float GetFlashDuration()
	{
		return *( float* ) ( ( DWORD ) this + 0xA2EC );
	}

	bool IsProtected()
	{
		return *( bool* )( ( DWORD )this + Offsets::Player::m_bGunGameImmunity );
	}

	bool IsDefusing()
	{
		return *( bool* )( ( DWORD )this + Offsets::Player::m_bIsDefusing );
	}

	bool HasHelm()
	{
		return *( bool* )( ( DWORD )this + Offsets::Player::m_bHasHelmet );
	}

	bool bInScope()
	{
		return *( bool* )( ( DWORD )this + Offsets::Player::m_bIsScoped );
	}

	bool bIsGhost()
	{
		return *( bool* ) ( ( DWORD )this + Offsets::Player::m_bIsPlayerGhost );
	}

	bool PlayerCheck()
	{
		if( this != nullptr && this && !this->IsDormant() && this->GetHealth() > 0 && !bIsGhost() && !this->IsProtected() && !( this->GetFlags() & FL_FROZEN ) && this->GetClientClass()->GetClassID() == CCSPlayer )
			return true;

		return false;
	}

	bool PlayerCheckWithoutFrozen()
	{
		if( this != nullptr && this && !this->IsDormant() && this->GetHealth() > 0 && !bIsGhost() && this->GetClientClass()->GetClassID() == CCSPlayer )
			return true;

		return false;
	}

	bool PlayerCheckWithoutDormant()
	{
		if( this != nullptr && this->GetHealth() > 0 && !( this->GetFlags() & FL_FROZEN ) && !bIsGhost() && this->GetClientClass()->GetClassID() == CCSPlayer )
			return true;

		return false;
	}

	bool IsDucking()
	{
		return *( bool* ) ( ( DWORD ) this + Offsets::Player::m_bDucking );
	}

	bool HasHeavyArmor()
	{
		static uintptr_t m_bHasHeavyArmor = g_pNetVars->GetOffset( "CCSPlayer", "m_bHasHeavyArmor" );
		return *reinterpret_cast<bool*>( uintptr_t( this ) + m_bHasHeavyArmor );
	}

	bool IsAlive() 
	{ 
		return this->GetLifeState() == LIFE_ALIVE;
	}

	signed char GetLifeState()
	{
		return *( signed char* )( ( DWORD )this + Offsets::Player::m_lifeState );
	}

	const char *GetPosition()
	{
		return ( char* ) ( ( DWORD )this + Offsets::Player::m_szLastPlaceName );
	}

	Vector GetRenderAngle()
	{
		return *( Vector* ) ( ( DWORD )this + 0x128 );
	}

	Vector GetViewPunchAngle()
	{
		return *( Vector* )( ( DWORD )this + Offsets::Player::m_Local + Offsets::Player::m_ViewPunchAngle );
	}

	Vector GetPunchAngle()
	{
		return *( Vector* )( ( DWORD )this + Offsets::Player::m_Local + Offsets::Player::m_aimPunchAngle );
	}

	Vector GetPunchAngleVelocity()
	{
		return *( Vector* ) ( ( DWORD )this + Offsets::Player::m_Local + Offsets::Player::m_aimPunchAngleVel );
	}

	Vector GetViewOffset()
	{
		return *( Vector* ) ( ( DWORD )this + Offsets::Player::m_Local + Offsets::Player::m_vecViewOffset );
	}

	QAngle GetQPunchAngle()
	{
		return *( QAngle* ) ( ( DWORD ) this + Offsets::Player::m_Local + Offsets::Player::m_aimPunchAngle );
	}

	Vector GetOrigin()
	{
		return *( Vector* )( ( DWORD )this + Offsets::Player::m_vecOrigin );
	}

	Vector GetEyePosition()
	{
		Vector rtn{ };
		getvfunc<void( __thiscall* )( void*, Vector& )>( this, 284 )( this, rtn );
		return rtn;

	//	return ( *( Vector* )( ( DWORD )this + Offsets::Player::m_vecOrigin ) + *( Vector* )( ( DWORD )this + Offsets::Player::m_vecViewOffset ) );
	}

	Vector GetVelocity()
	{
		return *( Vector* )( ( DWORD )this + Offsets::Player::m_vecVelocity );
	}

	Vector SetAbsVelocity(Vector velo)
	{
		return *(Vector*)((DWORD)this + 0x94) = velo;
	}

	Vector GetEyeAngles()
	{
		return *( Vector* )( ( DWORD )this + Offsets::Player::m_angEyeAngles );
	}

	QAngle GetQEyeAngles()
	{
		return *(QAngle*)((DWORD)this + Offsets::Player::m_angEyeAngles);
	}

	QAngle GetAngRotation()
	{
		return *( QAngle* ) ( ( DWORD )this + Offsets::Player::m_angRotation );
	}

	QAngle GetAbsRotation()
	{
		return *( QAngle* ) ( ( DWORD )this + Offsets::Player::m_angRotation - 12 );
	}

	BYTE GetMoveType()
	{
		/* v16 = *(_BYTE *)(v1 + 0x25C);<-
          v17 = (*(int (__thiscall **)(int))(*(_DWORD *)v1 + 560))(v1);
          result = Warning("PhysicsSimulate: %s bad movetype %d", v17, v16);*/
		return *( BYTE* )( ( DWORD )this + 0x25C);
	}

	Color GetTeamColor( bool bIsVisible = false )
	{
		int iTeam = GetTeamNumber();

		Color color = ( iTeam == 2 ) ? Color( 255, 0, 0 )/*tinvisible*/ : Color( 30, 200, 255 )/*ctinvisible*/;
		if( bIsVisible )
			color = ( iTeam == 2 ) ? Color( 255, 255, 0 )/*tvisible*/ : Color( 0, 255, 0 )/*ctvisible*/;

		return color;
	}

	void GetEstimatedVelocity( Vector& vec )
	{
		typedef void( __thiscall* GetSpreadFn )( void*, Vector& );
		GetSpreadFn Spread = ( GetSpreadFn )( ( *( PDWORD_PTR* )this )[ 141 ] );
		Spread( this, vec );
	}

    CBaseCombatWeapon* GetWeapon()
	{
		DWORD dwActiveWeapon = *( DWORD* ) ( ( DWORD )this + Offsets::Player::m_hActiveWeapon );

        if( dwActiveWeapon )
			return ( CBaseCombatWeapon* ) g_pEntList->GetClientEntityFromHandle( dwActiveWeapon );
       
        return nullptr;
	}

	CAnimState* GetAnimState()
	{
		static uintptr_t dwAnimState = *( uintptr_t* ) ( ( uintptr_t ) SigScan( "client.dll", "8B 8E ? ? ? ? F3 0F 10 48 04 E8 ? ? ? ? E9" ) + 0x2 );
		return *( CAnimState** ) ( ( uintptr_t ) this + dwAnimState );
	}
//	OFFSET( GetAnimState, CAnimState*, 0x38A0 );
};

class CBaseViewModel : public CBaseEntity 
{
public:
	CBaseHandle GetOwner() 
	{
		static uintptr_t m_hOwner = g_pNetVars->GetOffset( "CBaseViewModel", "m_hOwner" );
		return *reinterpret_cast<CBaseHandle*>( uintptr_t( this ) + m_hOwner );
	}

	int GetWeapon() 
	{
		static uintptr_t m_hWeapon = g_pNetVars->GetOffset( "CBaseViewModel", "m_hWeapon" );
		return *reinterpret_cast<int*>( uintptr_t( this ) + m_hWeapon );
	}
};

class CRenderAble
{
public:
	inline CBaseEntity* player()
	{
		return reinterpret_cast<CBaseEntity*>(this - 0x4);
	}
};

struct RenderableInfo_t
{
	CRenderAble* m_pRenderable;
	void* m_pAlphaProperty;
	int m_EnumCount;
	int m_nRenderFrame;
	unsigned short m_FirstShadow;
	unsigned short m_LeafList;
	short m_Area;
	uint16_t m_Flags;   // 0x0016
	uint16_t m_Flags2; // 0x0018
	Vector m_vecBloatedAbsMins;
	Vector m_vecBloatedAbsMaxs;
	Vector m_vecAbsMins;
	Vector m_vecAbsMaxs;
	int pad;
};