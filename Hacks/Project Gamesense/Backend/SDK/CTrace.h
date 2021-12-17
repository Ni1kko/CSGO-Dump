#pragma once
#include "../Include/GlobalIncludes.h"
#include "IHandleEntity.h"
#include "../Hashing/xorstr.h"
#include "vector3d.h"
#include "vmatrix.h"
class IHandleEntity;

#define	CONTENTS_EMPTY			0		/**< No contents. */
#define	CONTENTS_SOLID			0x1		/**< an eye is never valid in a solid . */
#define	CONTENTS_WINDOW			0x2		/**< translucent, but not watery (glass). */
#define	CONTENTS_AUX			0x4
#define	CONTENTS_GRATE			0x8		/**< alpha-tested xorstr_("grate") textures.  Bullets/sight pass through, but solids don't. */
#define	CONTENTS_SLIME			0x10
#define	CONTENTS_WATER			0x20
#define	CONTENTS_MIST			0x40
#define CONTENTS_OPAQUE			0x80		/**< things that cannot be seen through (may be non-solid though). */
#define	LAST_VISIBLE_CONTENTS	0x80
#define ALL_VISIBLE_CONTENTS 	(LAST_VISIBLE_CONTENTS | (LAST_VISIBLE_CONTENTS-1))
#define CONTENTS_TESTFOGVOLUME	0x100
#define CONTENTS_UNUSED5		0x200
#define CONTENTS_UNUSED6		0x4000
#define CONTENTS_TEAM1			0x800		/**< per team contents used to differentiate collisions. */
#define CONTENTS_TEAM2			0x1000		/**< between players and objects on different teams. */
#define CONTENTS_IGNORE_NODRAW_OPAQUE	0x2000		/**< ignore CONTENTS_OPAQUE on surfaces that have SURF_NODRAW. */
#define CONTENTS_MOVEABLE		0x4000		/**< hits entities which are MOVETYPE_PUSH (doors, plats, etc) */
#define	CONTENTS_AREAPORTAL		0x8000		/**< remaining contents are non-visible, and don't eat brushes. */
#define	CONTENTS_PLAYERCLIP		0x10000
#define	CONTENTS_MONSTERCLIP	0x20000

/**
    @section currents can be added to any other contents, and may be mixed
*/
#define	CONTENTS_CURRENT_0		0x40000
#define	CONTENTS_CURRENT_90		0x80000
#define	CONTENTS_CURRENT_180	0x100000
#define	CONTENTS_CURRENT_270	0x200000
#define	CONTENTS_CURRENT_UP		0x400000
#define	CONTENTS_CURRENT_DOWN	0x800000

/**
    @endsection
*/

#define	CONTENTS_ORIGIN			0x1000000	/**< removed before bsping an entity. */
#define	CONTENTS_MONSTER		0x2000000	/**< should never be on a brush, only in game. */
#define	CONTENTS_DEBRIS			0x4000000
#define	CONTENTS_DETAIL			0x8000000	/**< brushes to be added after vis leafs. */
#define	CONTENTS_TRANSLUCENT	0x10000000	/**< auto set if any surface has trans. */
#define	CONTENTS_LADDER			0x20000000
#define CONTENTS_HITBOX			0x40000000	/**< use accurate hitboxes on trace. */

/**
    @section Trace masks.
*/
#define	MASK_ALL				(0xFFFFFFFF)
#define	MASK_SOLID				(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_MONSTER|CONTENTS_GRATE) 			/**< everything that is normally solid */
#define	MASK_PLAYERSOLID		(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_PLAYERCLIP|CONTENTS_WINDOW|CONTENTS_MONSTER|CONTENTS_GRATE) 	/**< everything that blocks player movement */
#define	MASK_NPCSOLID			(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTERCLIP|CONTENTS_WINDOW|CONTENTS_MONSTER|CONTENTS_GRATE) /**< blocks npc movement */
#define	MASK_WATER				(CONTENTS_WATER|CONTENTS_MOVEABLE|CONTENTS_SLIME) 							/**< water physics in these contents */
#define	MASK_OPAQUE				(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_OPAQUE) 							/**< everything that blocks line of sight for AI, lighting, etc */
#define MASK_OPAQUE_AND_NPCS	(MASK_OPAQUE|CONTENTS_MONSTER)										/**< everything that blocks line of sight for AI, lighting, etc, but with monsters added. */
#define	MASK_VISIBLE			(MASK_OPAQUE|CONTENTS_IGNORE_NODRAW_OPAQUE) 								/**< everything that blocks line of sight for players */
#define MASK_VISIBLE_AND_NPCS	(MASK_OPAQUE_AND_NPCS|CONTENTS_IGNORE_NODRAW_OPAQUE) 							/**< everything that blocks line of sight for players, but with monsters added. */
#define	MASK_SHOT				(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTER|CONTENTS_WINDOW|CONTENTS_DEBRIS|CONTENTS_HITBOX) 	/**< bullets see these as solid */
#define MASK_SHOT_HULL			(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTER|CONTENTS_WINDOW|CONTENTS_DEBRIS|CONTENTS_GRATE) 	/**< non-raycasted weapons see this as solid (includes grates) */
#define MASK_SHOT_PORTAL		(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW) 							/**< hits solids (not grates) and passes through everything else */
#define MASK_SHOT_BRUSHONLY			(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_DEBRIS) // non-raycasted weapons see this as solid (includes grates)
#define MASK_SOLID_BRUSHONLY	(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_GRATE) 					/**< everything normally solid, except monsters (world+brush only) */
#define MASK_PLAYERSOLID_BRUSHONLY	(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_PLAYERCLIP|CONTENTS_GRATE) 			/**< everything normally solid for player movement, except monsters (world+brush only) */
#define MASK_NPCSOLID_BRUSHONLY	(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_MONSTERCLIP|CONTENTS_GRATE) 			/**< everything normally solid for npc movement, except monsters (world+brush only) */
#define MASK_NPCWORLDSTATIC		(CONTENTS_SOLID|CONTENTS_WINDOW|CONTENTS_MONSTERCLIP|CONTENTS_GRATE) 					/**< just the world, used for route rebuilding */
#define MASK_SPLITAREAPORTAL	(CONTENTS_WATER|CONTENTS_SLIME)

#define CHAR_TEX_ANTLION        'A'
#define CHAR_TEX_BLOODYFLESH    'B'
#define    CHAR_TEX_CONCRETE        'C'
#define CHAR_TEX_DIRT            'D'
#define CHAR_TEX_EGGSHELL        'E' ///< the egg sacs in the tunnels in ep2.
#define CHAR_TEX_FLESH            'F'
#define CHAR_TEX_GRATE            'G'
#define CHAR_TEX_ALIENFLESH        'H'
#define CHAR_TEX_CLIP            'I'
//#define CHAR_TEX_UNUSED        'J'
//#define CHAR_TEX_UNUSED        'K'
#define CHAR_TEX_PLASTIC        'L'
#define CHAR_TEX_METAL            'M'
#define CHAR_TEX_SAND            'N'
#define CHAR_TEX_FOLIAGE        'O'
#define CHAR_TEX_COMPUTER        'P'
//#define CHAR_TEX_UNUSED        'Q'
//#define CHAR_TEX_UNUSED        'R'
#define CHAR_TEX_SLOSH            'S'
#define CHAR_TEX_TILE            'T'
#define CHAR_TEX_CARDBOARD        'U'
#define CHAR_TEX_VENT            'V'
#define CHAR_TEX_WOOD            'W'
//#define CHAR_TEX_UNUSED        'X'
#define CHAR_TEX_GLASS            'Y'
#define CHAR_TEX_WARPSHIELD        'Z' ///< wierd-looking jello effect for advisor shield.

class CBaseEntity;

class __declspec( align( 16 ) )VectorAligned : public Vector
{
	public:
		VectorAligned &operator=( const Vector &vOther )
		{
			Init( vOther.x, vOther.y, vOther.z );
			return *this;
		}
		float w;
};
struct Ray_t
{
	VectorAligned  m_Start;  // starting point, centered within the extents
	VectorAligned  m_Delta;  // direction + length of the ray
	VectorAligned  m_StartOffset; // Add this to m_Start to Get the actual ray start
	VectorAligned  m_Extents;     // Describes an axis aligned box extruded along a ray
	const matrix3x4_t *m_pWorldAxisTransform;
	bool m_IsRay;  // are the extents zero?
	bool m_IsSwept;     // is delta != 0?

	Ray_t() : m_pWorldAxisTransform(NULL) {}

	Ray_t(Vector const & start, Vector const & end) {
		m_Delta = end - start;

		m_IsSwept = (m_Delta.LengthSqr() != 0);

		m_Extents.Init();

		m_pWorldAxisTransform = NULL;
		m_IsRay = true;

		// Offset m_Start to be in the center of the box...
		m_StartOffset.Init();
		m_Start = start;
	}

	void Init(Vector const& start, Vector const& end)
	{
		m_Delta = end - start;

		m_IsSwept = (m_Delta.LengthSqr() != 0);

		m_Extents.Init();

		m_pWorldAxisTransform = NULL;
		m_IsRay = true;

		// Offset m_Start to be in the center of the box...
		m_StartOffset.Init();
		m_Start = start;
	}

	void Init(Vector const& start, Vector const& end, Vector const& mins, Vector const& maxs)
	{
		m_Delta = end - start;

		m_pWorldAxisTransform = NULL;
		m_IsSwept = (m_Delta.LengthSqr() != 0);

		m_Extents = maxs - mins;
		m_Extents *= 0.5f;
		m_IsRay = (m_Extents.LengthSqr() < 1e-6);

		// Offset m_Start to be in the center of the box...
		m_StartOffset = maxs + mins;
		m_StartOffset *= 0.5f;
		m_Start = start + m_StartOffset;
		m_StartOffset *= -1.0f;
	}
	Vector InvDelta() const
	{
		Vector vecInvDelta;
		for (int iAxis = 0; iAxis < 3; ++iAxis) {
			if (m_Delta[iAxis] != 0.0f) {
				vecInvDelta[iAxis] = 1.0f / m_Delta[iAxis];
			}
			else {
				vecInvDelta[iAxis] = FLT_MAX;
			}
		}
		return vecInvDelta;
	}

private:
};

struct csurface_t
{
	const char* name;
	short surfaceProps;
	unsigned short flags;
};

struct cplane_t
{
	Vector normal;
	float dist;
	byte type;
	byte signbits;
	byte pad[2];
};
struct trace_t
{
	Vector start;
	Vector end;
	cplane_t plane;
	float fraction;
	int contents;
	WORD dispFlags;
	bool allsolid;
	bool startSolid;
	float fractionLeftSolid;
	csurface_t surface;
	int hitGroup;
	short physicsBone;
	CBaseEntity* m_pEnt;
	int hitbox;
	bool DidHit() const
	{
		return fraction < 1 || allsolid || startSolid;
	};
	bool DidHitNonWorldEntity() const
	{
		return m_pEnt && !DidHit();
	}
};
enum TraceType_t
{
	TRACE_EVERYTHING = 0,
	TRACE_WORLD_ONLY,				// NOTE: This does *not* test static props!!!
	TRACE_ENTITIES_ONLY,			// NOTE: This version will *not* test static props
	TRACE_EVERYTHING_FILTER_PROPS,	// NOTE: This version will pass the IHandleEntity for props through the filter, unlike all other filters
};
class ITraceFilter
{
	public:
		virtual bool ShouldHitEntity( void* pEntity, int contentsMask ) = 0;
		virtual TraceType_t GetTraceType() const = 0;
};


class CTraceFilter : public ITraceFilter
{
	public:
		bool ShouldHitEntity( void* pEntityHandle, int contentsMask )
		{
			return ( pEntityHandle != pSkip1 );
		}

		TraceType_t GetTraceType() const
		{
			return TRACE_EVERYTHING;
		}
		inline void SetIgnoreClass( char* Class )
		{
			ccIgnore = Class;
		}

		void* pSkip1;
		char* ccIgnore = "";
};
class CTraceFilterOneEntity : public ITraceFilter
{
	public:
		bool ShouldHitEntity( void* pEntityHandle, int contentsMask )
		{
			return ( pEntityHandle == pEntity );
		}

		TraceType_t GetTraceType() const
		{
			return TRACE_EVERYTHING;
		}

		void* pEntity;
};
class CTraceFilterOneEntity2 : public ITraceFilter
{
	public:
		bool ShouldHitEntity( void* pEntityHandle, int contentsMask )
		{
			return ( pEntityHandle == pEntity );
		}

		TraceType_t GetTraceType() const
		{
			return TRACE_ENTITIES_ONLY;
		}

		void* pEntity;
};

class CTraceFilterSkipTwoEntities : public ITraceFilter
{
	public:
		CTraceFilterSkipTwoEntities( void* pPassEnt1, void* pPassEnt2 )
		{
			passentity1 = pPassEnt1;
			passentity2 = pPassEnt2;
		}

		virtual bool ShouldHitEntity( void* pEntityHandle, int contentsMask )
		{
			return !( pEntityHandle == passentity1 || pEntityHandle == passentity2 );
		}

		virtual TraceType_t    GetTraceType() const
		{
			return TRACE_EVERYTHING;
		}

		void* passentity1;
		void* passentity2;
};

class CTraceEntity : public ITraceFilter
{
	public:
		bool ShouldHitEntity( void* pEntityHandle, int contentsMask )
		{
			return !( pEntityHandle == pSkip1 );
		}

		TraceType_t GetTraceType() const
		{
			return TRACE_ENTITIES_ONLY;
		}

		void* pSkip1;
};
class CTraceWorldOnly : public ITraceFilter
{
	public:
		bool ShouldHitEntity( void* pEntityHandle, int contentsMask )
		{
			return false;
		}

		TraceType_t GetTraceType() const
		{
			return TRACE_EVERYTHING;
		}

		void* pSkip1;
};

#include "vfunc.hpp"

class CTrace
{
	public:
		int GetPointContents( const Vector &vecAbsPosition, int contentsMask = MASK_ALL, IHandleEntity** ppEntity = NULL )
		{
			typedef int( __thiscall * fn )( void*, const Vector &, int, IHandleEntity** );

			if ( this )
				return vfunc<fn>( this, 0 )( this, vecAbsPosition, contentsMask, ppEntity );
		}
		void TraceRay( const Ray_t &ray, unsigned int fMask, ITraceFilter* filter, trace_t* trace )
		{
			typedef void( __thiscall * fn )( void*, const Ray_t &, unsigned int, ITraceFilter*, trace_t* );

			if ( this )
				vfunc<fn>( this, 5 )( this, ray, fMask, filter, trace );
		}
		void ClipRayToEntity( const Ray_t &ray, unsigned int fMask, CBaseEntity* pEnt, trace_t* pTrace )
		{
			typedef void( __thiscall * OriginalFn )( void*, const Ray_t &, unsigned int, CBaseEntity*, trace_t* );

			if ( this )
				return vfunc<OriginalFn>( this, 3 )( this, ray, fMask, pEnt, pTrace );
		}
};
