#pragma once

#include "sdk.h"

class CEngineTraceClient
{
public:
	enum TraceType_t
	{
		TRACE_EVERYTHING = 0,
		TRACE_WORLD_ONLY,				// NOTE: This does *not* test static props!!!
		TRACE_ENTITIES_ONLY,			// NOTE: This version will *not* test static props
		TRACE_EVERYTHING_FILTER_PROPS,	// NOTE: This version will pass the IHandleEntity for props through the filter, unlike all other filters
	};

	struct cplane_t
	{
		Vector normal;
		float dist;
		BYTE type;
		BYTE signBits;
		BYTE pad[ 2 ];
	};

	struct csurface_t
	{
		const char*		name;
		short			surfaceProps;
		unsigned short	flags;
	};

	struct Ray_t
	{
		VectorAligned  m_Start;	// starting point, centered within the extents
		VectorAligned  m_Delta;	// direction + length of the ray
		VectorAligned  m_StartOffset;	// Add this to m_Start to get the actual ray start
		VectorAligned  m_Extents;	// Describes an axis aligned box extruded along a ray
		const matrix3x4_t *m_pWorldAxisTransform;
		bool	m_IsRay;	// are the extents zero?
		bool	m_IsSwept;	// is delta != 0?

		Ray_t() : m_pWorldAxisTransform( NULL )	{}

		void Init( Vector const& start, Vector const& end )
		{
			Assert( &end );
			Vectorsubtract( end, start, m_Delta );

			m_IsSwept = ( m_Delta.LengthSqr() != 0 );

			VectorClear( m_Extents );
			m_pWorldAxisTransform = NULL;
			m_IsRay = true;

			// Offset m_Start to be in the center of the box...
			VectorClear( m_StartOffset );
			VectorCopy( start, m_Start );
		}

		void Init( Vector const& start, Vector const& end, Vector const& mins, Vector const& maxs )
		{
			Assert( &end );
			Vectorsubtract( end, start, m_Delta );

			m_pWorldAxisTransform = NULL;
			m_IsSwept = ( m_Delta.LengthSqr() != 0 );

			Vectorsubtract( maxs, mins, m_Extents );
			m_Extents *= 0.5f;
			m_IsRay = ( m_Extents.LengthSqr() < 1e-6 );

			// Offset m_Start to be in the center of the box...
			VectorAdd( mins, maxs, m_StartOffset );
			m_StartOffset *= 0.5f;
			VectorAdd( start, m_StartOffset, m_Start );
			m_StartOffset *= -1.0f;
		}

		// compute inverse delta
		Vector InvDelta() const
		{
			Vector vecInvDelta;
			for( int iAxis = 0; iAxis < 3; ++iAxis )
			{
				if( m_Delta[ iAxis ] != 0.0f )
				{
					vecInvDelta[ iAxis ] = 1.0f / m_Delta[ iAxis ];
				}
				else
				{
					vecInvDelta[ iAxis ] = FLT_MAX;
				}
			}
			return vecInvDelta;
		}

	private:
	};
	struct trace_t
	{
		Vector	start;
		Vector	endpos;
		cplane_t	plane;
		float		fraction;
		int			contents;
		WORD		dispFlags;
		bool		allSolid;
		bool		startSolid;
		float		fractionLeftSolid;
		csurface_t	surface;
		int			hitgroup;
		short		physicsBone;
		WORD		m_nWorldSurfaceIndex;
		CBaseEntity* m_pEnt;
		int			hitbox;
		bool DidHit() const
		{
			return fraction < 1.0f || allSolid || startSolid;
		}
		bool DidHitNonWorldEntity() const
		{
			return m_pEnt != 0 && !DidHit();
		}
	};

	class ITraceFilter
	{
	public:
		virtual bool ShouldHitEntity( CBaseEntity *pEntity, int contentsMask ) = 0;
		virtual TraceType_t	GetTraceType() const = 0;
	};

	class CTraceFilter : public ITraceFilter
	{
	public:
		bool ShouldHitEntity( CBaseEntity* pEntityHandle, int contentsMask )
		{
			return !( pEntityHandle == pSkip );
		}

		virtual TraceType_t GetTraceType() const
		{
			return TRACE_EVERYTHING;
		}

		void* pSkip;
	};

	class CTraceFilterWorldAndPropsOnly : public ITraceFilter
	{
	public:
		bool ShouldHitEntity( CBaseEntity *pServerEntity, int contentsMask )
		{
			return false;
		}
		virtual TraceType_t	GetTraceType() const
		{
			return TRACE_EVERYTHING;
		}
	};

	class CTraceFilterOneEntity : public ITraceFilter
	{
	public:
		bool ShouldHitEntity( CBaseEntity* pEntityHandle, int contentsMask )
		{
			return ( pEntityHandle == pEntity );
		}

		TraceType_t GetTraceType() const
		{
			return TRACE_EVERYTHING;
		}

		void* pEntity;
	};

    class CTraceWorldOnly : public ITraceFilter
    {
    public:
        bool ShouldHitEntity( CBaseEntity* pEntityHandle, int contentsMask )
        {
            return false;
        }

        virtual TraceType_t GetTraceType() const
        {
            return TRACE_WORLD_ONLY;
        }
    };

	class CTraceEntity : public ITraceFilter
	{
	public:
		bool ShouldHitEntity( CBaseEntity* pEntityHandle, int contentsMask )
		{
			return ( pEntityHandle == pHit );
		}
		virtual TraceType_t	GetTraceType() const
		{
			return TRACE_ENTITIES_ONLY;
		}
		void* pHit;
	};

	class CTraceFilterEntities : public ITraceFilter
	{
	public:
		bool ShouldHitEntity( CBaseEntity *pEntity, int contentsMask )
		{
			return !( pEntity == pSkip );
		}

		TraceType_t GetTraceType() const
		{
			return TRACE_ENTITIES_ONLY;
		}

		void* pSkip;
	};

	class CTraceFilterIgnoreGrenades 
	{
	public:
		virtual bool ShouldHitEntity( CBaseEntity* pEntityHandle, int contentsMask ) 
		{
			if( !pEntityHandle ) 
			{
				return !( pEntityHandle == pSkip );
			}

			return !( pEntityHandle == pSkip );
		}

		virtual TraceType_t    GetTraceType() const 
		{
			return TRACE_EVERYTHING;
		}

		CBaseEntity* pSkip;
		int m_icollisionGroup;
	};

	class CGrenadePredictTraceFilter 
	{
	public:
		virtual bool ShouldHitEntity( CBaseEntity* pEntityHandle, int contentsMask ) 
		{
			if( !pEntityHandle )
				return !( pEntityHandle == pSkip );

			if( pEntityHandle == pSkip )
				return false;

			return !( pEntityHandle == pSkip );
		}

		virtual TraceType_t    GetTraceType() const 
		{
			return TRACE_EVERYTHING;
		}

		CBaseEntity* pSkip;
		int m_icollisionGroup;
	};

	class CTraceFilterskipTwoEntities : public ITraceFilter
	{
	public:
		CTraceFilterskipTwoEntities( void *pPassEnt1, void *pPassEnt2 )
		{
			passentity1 = pPassEnt1;
			passentity2 = pPassEnt2;
		}

		virtual  bool ShouldHitEntity( CBaseEntity* pEntityHandle, int contentsMask )
		{
			return !( pEntityHandle == passentity1 || pEntityHandle == passentity2 );
		}

		virtual TraceType_t    GetTraceType() const
		{
			return TRACE_EVERYTHING;
		}

		void *passentity1;
		void *passentity2;
	};

	int GetPointContents( const Vector &vecAbsPosition, unsigned int fMask = MASK_ALL, CBaseEntity** ppEntity = NULL )
	{
		typedef int( __thiscall* OriginalFn )( PVOID, const Vector &, unsigned int, CBaseEntity** );
		return getvfunc<OriginalFn>( this, 0 )( this, vecAbsPosition, fMask, ppEntity );
	}

	void ClipRayToEntity( const Ray_t &ray, unsigned int fMask, CBaseEntity *pEnt, trace_t *pTrace )
	{
		typedef void( __thiscall* OriginalFn )( PVOID, const Ray_t &, unsigned int, CBaseEntity *, trace_t * );
		getvfunc<OriginalFn>( this, 3 )( this, ray, fMask, pEnt, pTrace );
	}

	void TraceRay( const Ray_t &ray, unsigned int flMask, ITraceFilter *pTraceFilter, trace_t *ptrace )
	{
		typedef void( __thiscall* OriginalFn )( PVOID, const Ray_t &, unsigned int, ITraceFilter *, trace_t * );
		getvfunc<OriginalFn>( this, 5 )( this, ray, flMask, pTraceFilter, ptrace );
	}
};

extern CEngineTraceClient* g_pEngineTrace;