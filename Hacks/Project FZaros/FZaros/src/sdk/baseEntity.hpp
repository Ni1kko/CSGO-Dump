#pragma once

#include "interfaces/clientUnknown.hpp"
#include "../utilities/netvarManager.hpp"
#include "animation.hpp"

#include "CUtlVector.hpp"

#include <algorithm>

#include <intrin.h>
#include "..\sdk\matrix.hpp"

// sse min.
template< typename t = float >
t minimumdesync(const t & a, const t & b) {
	// check type.
	static_assert(std::is_arithmetic< t >::value, "Math::min only supports integral types.");
	return (t)_mm_cvtss_f32(
		_mm_min_ss(_mm_set_ss((float)a),
			_mm_set_ss((float)b))
	);
}

// sse max.
template< typename t = float >
t maximumdesync(const t & a, const t & b) {
	// check type.
	static_assert(std::is_arithmetic< t >::value, "Math::max only supports integral types.");
	return (t)_mm_cvtss_f32(
		_mm_max_ss(_mm_set_ss((float)a),
			_mm_set_ss((float)b))
	);
}

class CUserCmd;

enum move_type : int
{
	// never moves
	movetype_none = 0,

	// For players -- in TF2 commander view, etc.
	movetype_isometric = 1,

	// Player only - moving on the ground
	movetype_walk = 2,

	// gravity, special edge handling -- monsters use this
	movetype_step = 3,

	// No gravity, but still collides with stuff
	movetype_fly = 4,

	// flies through the air + is affected by gravity
	movetype_flygravity = 5,

	// uses VPHYSICS for simulation
	movetype_vphysics = 6,

	// no clip to world, push and crush
	movetype_push = 7,

	// No gravity, no collisions, still do velocity/avelocity
	movetype_noclip = 8,

	// Used by players only when going onto a ladder
	movetype_ladder = 9,

	// Observer movement, depends on player's observer mode
	movetype_observer = 10,

	// Allows the entity to describe its own physics
	movetype_custom = 11,

	// should always be defined as the last item in the list
	movetype_last = movetype_observer,

	movetype_max_bits = 4
};

class VarMapEntry_t
{
public:
	unsigned short type;
	unsigned short m_bNeedsToInterpolate;	// Set to false when this var doesn't
											// need Interpolate() called on it anymore.
	void *data;
	void *watcher;
};

struct VarMapping_t
{
	CUtlVector<VarMapEntry_t> m_Entries;
	int m_nInterpolatedEntries;
	float m_lastInterpolationTime;
};

enum EntityFlags : int
{
	FL_ONGROUND = ( 1 << 0 ),
	FL_DUCKING = ( 1 << 1 ),
	FL_WATERJUMP = ( 1 << 2 ),
	FL_ONTRAIN = ( 1 << 3 ),
	FL_INRAIN = ( 1 << 4 ),
	FL_FROZEN = ( 1 << 5 ),
	FL_ATCONTROLS = ( 1 << 6 ),
	FL_CLIENT = ( 1 << 7 ),
	FL_FAKECLIENT = ( 1 << 8 )
};

class CUtlVectorSimple
{
public:
	unsigned memory;
	char pad[ 8 ];
	unsigned int count;
	inline void* Retrieve( int index, unsigned sizeofdata )
	{
		return ( void* ) ( ( *( unsigned* ) this ) + ( sizeofdata * index ) );
	}
};

class CBaseCombatWeapon;

class CSWeaponInfo
{
public:
	char _0x0000[ 20 ];
	__int32 maxClip;			//0x0014 
	char _0x0018[ 12 ];
	__int32 maxReservedAmmo;	//0x0024 
	char _0x0028[ 96 ];
	char* hudName;				//0x0088 
	char* weaponName;			//0x008C 
	char _0x0090[ 60 ];
	__int32 type;				//0x00CC 
	__int32 price;				//0x00D0 
	__int32 reward;				//0x00D4 
	char _0x00D8[ 20 ];
	unsigned char fullAuto;				//0x00EC 
	char _0x00ED[ 3 ];
	__int32 damage;				//0x00F0 
	float armorRatio;			//0x00F4 
	__int32 bullets;			//0x00F8 
	float penetration;			//0x00FC 
	char _0x0100[ 8 ];
	float range;				//0x0108 
	float rangeModifier;		//0x010C 
	char _0x0110[ 16 ];
	unsigned char silencer;				//0x0120 
	char _0x0121[ 15 ];
	float maxSpeed;			//0x0130 
	float maxSpeedAlt;		//0x0134 
	char _0x0138[ 76 ];
	__int32 recoilSeed;		//0x0184 
	char _0x0188[ 32 ];
};// Size=0x01A8

class IWeaponSystem
{
	virtual void unused0( ) = 0;
	virtual void unused1( ) = 0;
public:
	virtual const CSWeaponInfo *GetWpnData( unsigned int ItemDefinitionIndex ) = 0;
};

struct SpatializationInfo_t;
class IClientEntity : public IClientUnknown, public IClientRenderable, public IClientNetworkable, public IClientThinkable
{
public:
	virtual void             Release( void ) = 0;
	virtual const Vector3     &GetAbsOrigin( void ) const = 0;
	virtual const Vector3     &GetAbsAngles( void ) const = 0;
	virtual void* GetMouth( void ) = 0;
	virtual bool             GetSoundSpatialization( SpatializationInfo_t info ) = 0;
	virtual bool             IsBlurred( void ) = 0;
};

enum item_definition_indexes
{
	item_none = 0,
	weapon_deagle = 1,
	weapon_duals = 2,
	weapon_five7 = 3,
	weapon_glock = 4,
	weapon_ak47 = 7,
	weapon_aug = 8,
	weapon_awp = 9,
	weapon_famas = 10,
	weapon_g3sg1 = 11,
	weapon_galil = 13,
	weapon_m249 = 14,
	weapon_m4a1 = 16,
	weapon_mac10 = 17,
	weapon_p90 = 19,
	weapon_ump45 = 24,
	weapon_xm1014 = 25,
	weapon_bizon = 26,
	weapon_mag7 = 27,
	weapon_negev = 28,
	weapon_sawedoff = 29,
	weapon_tec9 = 30,
	weapon_taser = 31,
	weapon_p2000 = 32,
	weapon_mp7 = 33,
	weapon_mp9 = 34,
	weapon_nova = 35,
	weapon_p250 = 36,
	weapon_scar20 = 38,
	weapon_sg553 = 39,
	weapon_scout = 40,
	weapon_knife_t = 42,
	weapon_flash = 43,
	weapon_he = 44,
	weapon_smoke = 45,
	weapon_molotov = 46,
	weapon_decoy = 47,
	weapon_inc = 48,
	weapon_c4 = 49,
	weapon_knife_ct = 59,
	weapon_m4a1s = 60,
	weapon_usps = 61,
	weapon_cz75 = 63,
	weapon_revolver = 64,
	weapon_knife_bayonet = 500,
	weapon_knife_flip = 505,
	weapon_knife_gut = 506,
	weapon_knife_karambit = 507,
	weapon_knife_m9bayonet = 508,
	weapon_knife_huntsman = 509,
	weapon_knife_falchion = 512,
	weapon_knife_bowie = 514,
	weapon_knife_butterfly = 515,
	weapon_knife_dagger = 516,
	weapon_max
};


class CBaseEntity : public IClientEntity
{
private:
	template<class T>
	T getValue( const int offset )
	{
		return *reinterpret_cast< T* >( reinterpret_cast< uintptr_t >( this ) + offset );
	}

	template <typename T>
	T& setValue( uintptr_t offset )
	{
		return *reinterpret_cast< T* >( reinterpret_cast< uintptr_t >( this ) + offset );
	}

public:

	CBaseCombatWeapon* activeWeapon( );

	bool isKnifeorNade( );

	bool isKnife( );

	bool isNade( );

	bool& clientSideAnimation( )
	{
		static int m_bClientSideAnimation = g_NetVarManager->GetOffset( "DT_BaseAnimating", "m_bClientSideAnimation" );
		return setValue<bool>( m_bClientSideAnimation );
	}

	void ClientAnimations(bool value)
	{
		static int m_bClientSideAnimation = g_NetVarManager->GetOffset("DT_BaseAnimating", "m_bClientSideAnimation");
		*reinterpret_cast<bool*>(uintptr_t(this) + m_bClientSideAnimation) = value;
	}

	void setCurrentCommand( CUserCmd* cmd )
	{
		static int test = g_NetVarManager->GetOffset( "DT_BasePlayer", "localdata", "m_hConstraintEntity" );

		*reinterpret_cast< CUserCmd** >( uintptr_t( this ) + ( test - 0xC ) ) = cmd;
	}

	template <typename t>
	static t GetVFunc( void* class_pointer, size_t index )
	{
		return ( *( t** ) class_pointer )[ index ];
	}

	void updateClientAnimation( )
	{
		GetVFunc<void( __thiscall* )( void* )>( this, 222 )( this );
	}

	/*CBasePlayerAnimState* animState()
	{
		return *(CBasePlayerAnimState**)(PVOID)((DWORD)(this) + 0x38A0);
	}

	void setAnimState(CBasePlayerAnimState* State)
	{
		*(CBasePlayerAnimState**)(PVOID)((DWORD)(this) + 0x38A0) = State;
	}*/

	float m_fDuckSpeed( )
	{
		//DT_BasePlayer
		static int m_flDuckSpeed = g_NetVarManager->GetOffset( "DT_BasePlayer", "m_flDuckSpeed" );
		return *( float* ) ( ( uintptr_t ) this + ( uintptr_t ) m_flDuckSpeed );
	}

	move_type moveType( )
	{
		return *( move_type* ) ( ( unsigned long ) this + 0x25C );
	}

	int &getTakeDamage( ) {
		return *reinterpret_cast< int * >( uintptr_t( this ) + 0x280 );
	}

	void setAbsAngles( Vector3 angles );
	void setAbsOrigin( Vector3 ArgOrigin );

	void invalidateBoneCache( );

	void invalidatePhysicsRecursive( int nChangeFlags );

	CUtlVectorSimple* animOverlayStruct( );

	CAnimationLayer& animOverlayStructByIndex( int index );

	CAnimationLayer* animOverlay( int i );

	int numAnimOverlays( );

	int sequenceActivity( int sequence );

	float& simulationTime( )
	{
		static int m_flSimulationTime = g_NetVarManager->GetOffset( "DT_BaseEntity", "m_flSimulationTime" );
		return *( float* ) ( ( uintptr_t ) this + ( uintptr_t ) m_flSimulationTime );
	}

	Vector3 ragdollPos( )
	{
		static int m_ragPos = g_NetVarManager->GetOffset( "DT_Ragdoll", "m_ragPos" );
		return *reinterpret_cast< Vector3* >( uintptr_t( this ) + m_ragPos );
	}

	CBasePlayerAnimState* animState( )
	{
		return *reinterpret_cast< CBasePlayerAnimState** >( uintptr_t( this ) + 0x3900 );
	}

	float calculateDesyncDelta( )
	{
		if (!this)
			return 0.f;

		auto anim_state = this->animState();
		if (!anim_state)
			return 0.f;

		float duck_amount = anim_state->m_fDuckAmount;
		float speed_fraction = maximumdesync< float >(0, minimumdesync< float >(anim_state->m_flFeetSpeedUnknownForwardOrSideways, 1));
		float speed_factor = maximumdesync< float >(0, minimumdesync< float >(1, anim_state->m_flFeetSpeedUnknownForwardOrSideways));

		float yaw_modifier = (((anim_state->m_flStopToFullRunningFraction * -0.3f) - 0.2f) * speed_fraction) + 1.0f;

		if (duck_amount > 0.f) {
			yaw_modifier += ((duck_amount * speed_factor) * (0.5f - yaw_modifier));
		}

		return anim_state->velocity_subtract_y * yaw_modifier;
	}

	float calculateDesyncValue( )
	{
		auto animstate = this->animState();
		if (!animstate)
			return 0.f;

		float flRunningSpeed = std::clamp(animstate->m_flFeetSpeedForwardsOrSideWays, 0.f, 1.f);
		float flYawModifier = ((animstate->m_flStopToFullRunningFraction * -0.3f) - 0.2f) * flRunningSpeed;
		float flYawModifier2 = flYawModifier + 1.f;

		if (animstate->m_fDuckAmount > 0.f)
		{
			float maxVelocity = std::clamp(animstate->m_flFeetSpeedForwardsOrSideWays, 0.f, 1.f);
			float duckSpeed = animstate->m_fDuckAmount * maxVelocity;
			flYawModifier2 += (duckSpeed * (0.5f - flYawModifier2));
		}

		return *(float*)((uintptr_t)animstate + 0x334) * flYawModifier2;
	}

	float animationTime( )
	{
		static int m_flAnimTime = g_NetVarManager->GetOffset( "DT_CSPlayer", "m_flAnimTime" );
		return getValue<float>( m_flAnimTime );
	}

	float oldSimulationTime( )
	{
		static int m_flOldSimulationTime = g_NetVarManager->GetOffset( "DT_BaseEntity", "m_flSimulationTime" );
		return getValue<float>( m_flOldSimulationTime + 4 );
	}

	float lowerBodyYawTarget( )
	{
		static int m_flLowerBodyYawTarget = g_NetVarManager->GetOffset( "DT_CSPlayer", "m_flLowerBodyYawTarget" );
		return getValue<float>( m_flLowerBodyYawTarget );
	}

	void setLowerBodyYawTarget( float value )
	{
		static int m_flLowerBodyYawTarget = g_NetVarManager->GetOffset( "DT_CSPlayer", "m_flLowerBodyYawTarget" );
		*reinterpret_cast< float* >( uintptr_t( this ) + m_flLowerBodyYawTarget ) = value;
	}

	CBaseHandle owner( )
	{
		static int m_hOwnerEntity = g_NetVarManager->GetOffset( "DT_BaseEntity", "m_hOwnerEntity" );

		return getValue<CBaseHandle>( m_hOwnerEntity );
	}

	int tickBase( )
	{
		static int m_nTickBase = g_NetVarManager->GetOffset( "DT_BasePlayer", "localdata", "m_nTickBase" );
		return getValue<int>( m_nTickBase );
	}

	void setTickBase( int tickbase )
	{
		static int m_nTickBase = g_NetVarManager->GetOffset( "DT_BasePlayer", "localdata", "m_nTickBase" );
		*reinterpret_cast< int* >( uintptr_t( this ) + m_nTickBase ) = tickbase;
	}

	Vector3 viewPunchAngle( )
	{
		return *reinterpret_cast< Vector3* >( uintptr_t( this ) + uintptr_t( 0x3020 ) );
	}

	Vector3 aimPunchAngle( )
	{
		return *reinterpret_cast< Vector3* >( uintptr_t( this ) + uintptr_t( 0x302C ) );
	}

	bool lifeState( )
	{
		static int m_lifeState = g_NetVarManager->GetOffset( "DT_BasePlayer", "m_lifeState" );
		return getValue<bool>( m_lifeState );
	}

	bool dormant( )
	{
		return *reinterpret_cast< bool* >( uintptr_t( this ) + 0xED ); //m_bDormant
	}

	bool heavyArmor( )
	{
		static int m_bHasHeavyArmor = g_NetVarManager->GetOffset( "DT_CSPlayer", "m_bHasHeavyArmor" );
		return getValue<bool>( m_bHasHeavyArmor );
	}

	int health( )
	{
		static int m_iHealth = g_NetVarManager->GetOffset( "DT_BasePlayer", "m_iHealth" );
		return getValue<int>( m_iHealth );
	}

	bool scoped( )
	{
		static int m_bIsScoped = g_NetVarManager->GetOffset( "DT_CSPlayer", "m_bIsScoped" );
		return getValue<bool>( m_bIsScoped );
	}

	bool rescuing( )
	{
		static int m_bIsScoped = g_NetVarManager->GetOffset( "DT_CSPlayer", "m_bIsRescuing" );
		return getValue<bool>( m_bIsScoped );
	}

	bool defusing( )
	{
		static int m_bIsScoped = g_NetVarManager->GetOffset( "DT_CSPlayer", "m_bIsDefusing" );
		return getValue<bool>( m_bIsScoped );
	}

	int armorValue( )
	{
		static int m_ArmorValue = g_NetVarManager->GetOffset( "DT_CSPlayer", "m_ArmorValue" );
		return getValue<int>( m_ArmorValue );
	}

	std::array<float, 24>& m_flPoseParameter( )
	{
		return *( std::array<float, 24>* )( ( uintptr_t ) this + ( uintptr_t ) 0x2764 );
	}

	bool hasHelmet( )
	{
		static int m_bHasHelmet = g_NetVarManager->GetOffset( "DT_CSPlayer", "m_bHasHelmet" );
		return getValue<bool>( m_bHasHelmet );
	}

	int m_iAccount( )
	{
		static int m_iAccount = g_NetVarManager->GetOffset( "DT_CSPlayer", "m_iAccount" );
		return getValue<int>( m_iAccount );
	}

	std::string armorName( )
	{
		if ( armorValue( ) > 0 )
		{
			if ( hasHelmet( ) )
				return "hk";
			else
				return "k";
		}
		else
			return " ";
	}

	int sequence( )
	{
		static int m_nSequence = g_NetVarManager->GetOffset( "DT_CSPlayer", "m_nSequence" );
		return getValue<int>( m_nSequence );
	}

	float cycle( )
	{
		return *reinterpret_cast< float* >( ( unsigned long ) this + g_NetVarManager->GetOffset( "DT_CSPlayer", "m_flCycle" ) );
	}

	bool alive( )
	{
		return this->health( ) > 0 && this->lifeState( ) == 0;
	}

	uintptr_t observerTarget( )
	{
		static int dadd = g_NetVarManager->GetOffset( "DT_BasePlayer", "m_hObserverTarget" );
		return getValue<uintptr_t>( dadd );
	}

	Vector3 worldSpaceCenter( )
	{
		Vector3 vecOrigin = this->vecOrigin( );
		auto collide = ( ICollideable* ) this->GetCollideable( );

		Vector3 min = collide->OBBMins( ) + vecOrigin;
		Vector3 max = collide->OBBMaxs( ) + vecOrigin;

		Vector3 size = max - min;
		size /= 2.f;
		size += min;

		return size;
	}

	Vector3 vecOrigin( )
	{
		static int m_vecOrigin = g_NetVarManager->GetOffset( "DT_BaseEntity", "m_vecOrigin" );
		return getValue<Vector3>( m_vecOrigin );
	}

	bool immunity( )
	{
		static int m_bGunGameImmunity = g_NetVarManager->GetOffset( "DT_CSPlayer", "m_bGunGameImmunity" );
		return getValue<bool>( m_bGunGameImmunity );
	}

	int team( )
	{
		static int m_iTeamNum = g_NetVarManager->GetOffset( "DT_BaseEntity", "m_iTeamNum" );
		return getValue<int>( m_iTeamNum );
	}

	int index( )
	{
		return *( int* ) ( uintptr_t( this ) + 0x64 );
	}

	VarMapping_t* varMapping( )
	{
		return reinterpret_cast< VarMapping_t* >( ( unsigned long ) this + 0x24 );
	}

	Vector3& eyeAngles( )
	{
		static int m_angEyeAngles = g_NetVarManager->GetOffset( "DT_CSPlayer", "m_angEyeAngles" );
		return *reinterpret_cast< Vector3* >( uintptr_t( this ) + m_angEyeAngles );
	}

	void setEyeAngles( Vector3 Angle )
	{
		static int m_angEyeAngles = g_NetVarManager->GetOffset( "DT_CSPlayer", "m_angEyeAngles" );
		*reinterpret_cast< Vector3* >( uintptr_t( this ) + m_angEyeAngles ) = Angle;
	}

	Vector3 GetOrigin()
	{
		static int m_vecOrigin = g_NetVarManager->GetOffset("DT_BaseEntity", "m_vecOrigin");
		return getValue<Vector3>(m_vecOrigin);
	}

	Vector3 viewOffset( )
	{
		static int m_vecViewOffset = g_NetVarManager->GetOffset( "DT_BasePlayer", "localdata", "m_vecViewOffset[0]" );
		return getValue<Vector3>( m_vecViewOffset );
	}

	CBaseHandle viewModel( )
	{
		static int m_hViewModel = g_NetVarManager->GetOffset( "DT_BasePlayer", "m_hViewModel[0]" );

		return getValue<CBaseHandle>( m_hViewModel );
	}

	Vector3 velocity( )
	{
		static int m_vecVelocity = g_NetVarManager->GetOffset( "DT_BasePlayer", "localdata", "m_vecVelocity[0]" );
		return getValue<Vector3>( m_vecVelocity );
	}

	void setVelocity( Vector3 velocity )
	{
		static int m_vecVelocity = g_NetVarManager->GetOffset( "DT_BasePlayer", "localdata", "m_vecVelocity[0]" );

		*( Vector3* ) ( ( unsigned long ) this + m_vecVelocity ) = velocity;
	}

	ICollideable* collideable( )
	{
		return ( ICollideable* ) ( ( unsigned long ) this + 0x318 );
	}

	EntityFlags& flags( )
	{
		static int m_fFlags = g_NetVarManager->GetOffset( "DT_BasePlayer", "m_fFlags" );
		return *reinterpret_cast< EntityFlags* >( uintptr_t( this ) + m_fFlags );
	}

	void setFlags( int flags )
	{
		static int m_fFlags = g_NetVarManager->GetOffset( "DT_BasePlayer", "m_fFlags" );
		*reinterpret_cast< int* >( reinterpret_cast< uintptr_t >( this ) + m_fFlags ) = flags;
	}

	float_t duckAmount( )
	{
		static int m_flDuckAmount = g_NetVarManager->GetOffset( "DT_BasePlayer", "m_flDuckAmount" );
		return getValue<float_t>( m_flDuckAmount );
	}

	float* flashDuration( )
	{
		return ( float* ) ( ( uintptr_t ) this + g_NetVarManager->GetOffset( "DT_CSPlayer", "m_flFlashDuration" ) );
	}

	Vector3 eyePosition( void )
	{
		return vecOrigin( ) + viewOffset( );
	}

	int hitboxSet( )
	{
		static int m_nHitboxSet = g_NetVarManager->GetOffset( "DT_BasePlayer", "m_nHitboxSet" );
		return getValue<int>( m_nHitboxSet );
	}

	Vector3 hitboxPosition( int Hitbox, matrix3x4_t *Matrix, float *Radius );
	Vector3 hitboxPosition( int Hitbox, matrix3x4_t *Matrix );

	Vector3 getHitbox( int hitbox );

	void fixSetupBones( matrix3x4_t * Matrix, float simtime );

	matrix3x4_t GetBonesMatrix(int BoneID)
	{
		matrix3x4_t matrix;

		auto offset = *reinterpret_cast<uintptr_t*>(uintptr_t(this) + 0x26A8);
		if (offset)
			matrix = *reinterpret_cast<matrix3x4_t*>(offset + 0x30 * BoneID);

		return matrix;
	}

	bool IsPlayer() {
		using original_fn = bool(__thiscall*)(CBaseEntity*);
		return (*(original_fn * *)this)[155](this);
	}
	bool is_valid(CBaseEntity* local_player = nullptr, bool is_player = true, bool dormant = true, bool team_check = true) {
		if (!this)
			return false;

		if (dormant)
			if (this->IsDormant())
				return false;

		if (team_check && local_player)
			if (this->team() == local_player->team())
				return false;

		if (is_player)
			if (!this->IsPlayer())
				return false;

		if (this->health() <= 0)
			return false;

		return true;
	}
	Vector3 GetBonePosition(int iBone)
	{
		matrix3x4_t boneMatrixes[128];
		if (this->SetupBones(boneMatrixes, 128, 0x100, 0))
		{
			matrix3x4_t boneMatrix = boneMatrixes[iBone];
			return Vector3(boneMatrix.flMatVal[0][3], boneMatrix.flMatVal[1][3], boneMatrix.flMatVal[2][3]);
		}
		else
			return Vector3(0, 0, 0);
	}
};

typedef CSWeaponInfo& ( __thiscall * GetCSWpnDataFn )( void* );

class CBaseCombatWeapon : public CBaseEntity
{
private:
	template<class T>
	T getValue( const int offset )
	{
		return *reinterpret_cast< T* >( reinterpret_cast< std::uintptr_t >( this ) + offset );
	}

public:

	float inaccuracy( )
	{
		using OriginalFn = VFUNC_SIGNATURE( float );
		return ( ( OriginalFn ) g_InterfaceUtils->getVfuncTable( this )[ 476 ] )( this );
	}

	float spread( )
	{
		using OriginalFn = VFUNC_SIGNATURE( float );
		return ( ( OriginalFn ) g_InterfaceUtils->getVfuncTable( this )[ 446 ] )( this );
	}

	float accuracyPenalty( )
	{
		using OriginalFn = VFUNC_SIGNATURE( float );
		return ( ( OriginalFn ) g_InterfaceUtils->getVfuncTable( this )[ 477 ] )( this );
	}

	CSWeaponInfo* weaponData( )
	{
		if ( !this )
			return nullptr;

		using OriginalFn = VFUNC_SIGNATURE( CSWeaponInfo* );
		return ( ( OriginalFn ) g_InterfaceUtils->getVfuncTable( this )[ 454 ] )( this );
	}

	bool hasScope( )
	{
		int weapon_index = this->itemDefenitionIndex( );
		return weapon_index == weapon_g3sg1
			|| weapon_index == weapon_scar20
			|| weapon_index == weapon_awp
			|| weapon_index == weapon_aug
			|| weapon_index == weapon_sg553
			|| weapon_index == weapon_scout;
	}

	bool hasSniperScope( )
	{
		int weapon_index = this->itemDefenitionIndex( );
		return weapon_index == weapon_g3sg1
			|| weapon_index == weapon_scar20
			|| weapon_index == weapon_awp
			|| weapon_index == weapon_scout;
	}

	std::string name( )
	{
		return std::string( this->weaponData( )->weaponName );
	}

	int ammo( )
	{
		static int m_iClip1 = g_NetVarManager->GetOffset( "DT_BaseCombatWeapon", "m_iClip1" );
		return *( int* ) ( ( uintptr_t ) this + m_iClip1 );
	}

	float lastShotTime( )
	{
		static int m_fLastShotTime = g_NetVarManager->GetOffset( "DT_WeaponCSBase", "m_fLastShotTime" );
		return getValue<float>( m_fLastShotTime );
	}


	float m_flThrowStrength( )
	{
		static int m_flThrowStrength = g_NetVarManager->GetOffset( "DT_BaseCSGrenade", "m_flThrowStrength" );
		return getValue<float>( m_flThrowStrength );
	}

	bool m_bPinPulled( )
	{
		static int m_bPinPulled = g_NetVarManager->GetOffset( "DT_BaseCSGrenade", "m_bPinPulled" );
		return getValue<bool>( m_bPinPulled );
	}

	float nextPrimaryAttack( )
	{
		static int m_flNextPrimaryAttack = g_NetVarManager->GetOffset( "DT_BaseCombatWeapon", "LocalActiveWeaponData", "m_flNextPrimaryAttack" );
		return getValue<float>( m_flNextPrimaryAttack );
	}

	int zoomLevel( )
	{
		static int zoomLevel = g_NetVarManager->GetOffset( "DT_WeaponCSBaseGun", "m_zoomLevel" );
		return getValue<int>( zoomLevel );
	}

	float nextAttack( )
	{
		return *reinterpret_cast< float* >( uintptr_t( this ) + uintptr_t( 0x2D60 ) );
	}

	short itemDefenitionIndex( )
	{
		return *reinterpret_cast< short* >( uintptr_t( this ) + uintptr_t( 0x2FAA ) );
	}

	float postponeFireReadyTime( )
	{
		return *reinterpret_cast< float* >( uintptr_t( this ) + uintptr_t( 0x32C4 ) );
	}

	bool isReloading( );
	bool canFire( );
	bool canFirePostpone( );

	item_definition_indexes GetItemDefinitionIndex()
	{
		static int m_iItemDefinitionIndex = g_NetVarManager->GetOffset("DT_BaseAttributableItem", "m_AttributeManager", "m_Item", "m_iItemDefinitionIndex");
		return getValue<item_definition_indexes>(m_iItemDefinitionIndex);
	}

	bool IsPistol()
	{
		CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)this->activeWeapon();

		if (!pWeapon)
			return false;

		item_definition_indexes WeapID = pWeapon->GetItemDefinitionIndex();

		return WeapID == item_definition_indexes::weapon_deagle || WeapID == item_definition_indexes::weapon_duals || WeapID == item_definition_indexes::weapon_five7 || WeapID == item_definition_indexes::weapon_p250 ||
			WeapID == item_definition_indexes::weapon_glock || WeapID == item_definition_indexes::weapon_usps || WeapID == item_definition_indexes::weapon_cz75 || WeapID == item_definition_indexes::weapon_p2000 || WeapID == item_definition_indexes::weapon_tec9 || WeapID == item_definition_indexes::weapon_revolver;
	}

	bool IsShotgun()
	{
		CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)this->activeWeapon();

		if (!pWeapon)
			return false;

		item_definition_indexes WeapID = pWeapon->GetItemDefinitionIndex();

		return WeapID == item_definition_indexes::weapon_xm1014 || WeapID == item_definition_indexes::weapon_nova || WeapID == item_definition_indexes::weapon_sawedoff || WeapID == item_definition_indexes::weapon_mag7;
	}

	bool IsSniper()
	{
		CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)this->activeWeapon();

		if (!pWeapon)
			return false;

		item_definition_indexes WeapID = pWeapon->GetItemDefinitionIndex();

		return WeapID == item_definition_indexes::weapon_awp || WeapID == item_definition_indexes::weapon_scar20 || WeapID == item_definition_indexes::weapon_g3sg1 || WeapID == item_definition_indexes::weapon_scout;
	}
};

class IClientEntityListener
{
public:
	virtual void OnEntityCreated( CBaseEntity *pEntity ) { }
	virtual void OnEntityDeleted( CBaseEntity *pEntity ) { }
};

class IClientEntityList
{
public:
	virtual IClientNetworkable* GetClientNetworkable( int entnum ) = 0;
	virtual void* vtablepad0x1( void ) = 0;
	virtual void* vtablepad0x2( void ) = 0;
	virtual CBaseEntity* GetClientEntity( int entNum ) = 0;
	virtual IClientEntity* GetClientEntityFromHandle( CBaseHandle hEnt ) = 0;
	virtual int                   NumberOfEntities( bool bIncludeNonNetworkable ) = 0;
	virtual int                   GetHighestEntityIndex( void ) = 0;
	virtual void                  SetMaxEntities( int maxEnts ) = 0;
	virtual int                   GetMaxEntities( ) = 0;

	void AddListenerEntity( IClientEntityListener *pListener )
	{
		entityListeners.AddToTail( pListener );
	}

	void RemoveListenerEntity( IClientEntityListener *pListener )
	{
		entityListeners.FindAndRemove( pListener );
	}
private:
	CUtlVector< IClientEntityListener* > entityListeners;
};