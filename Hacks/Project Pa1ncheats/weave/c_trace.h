
#include "matrix.h"
#include "i_base_player.h"
#include "Vector.h"

class IHandleEntity;
enum TraceType_t
{
	TRACE_EVERYTHING = 0,
	TRACE_WORLD_ONLY, // NOTE: This does *not* test static props!!!
	TRACE_ENTITIES_ONLY, // NOTE: This version will *not* test static props
	TRACE_EVERYTHING_FILTER_PROPS, // NOTE: This version will pass the IHandleEntity for props through the filter, unlike all other filters
};
inline void VectorSubtract(const Vector& a, const Vector& b, Vector& c)
{
	CHECK_VALID(a);
	CHECK_VALID(b);
	c.x = a.x - b.x;
	c.y = a.y - b.y;
	c.z = a.z - b.z;
}
inline void VectorAdd(const Vector& a, const Vector& b, Vector& c)
{
	CHECK_VALID(a);
	CHECK_VALID(b);
	c.x = a.x + b.x;
	c.y = a.y + b.y;
	c.z = a.z + b.z;
}
struct Ray_t
{
	__declspec(align(16)) Vector  m_Start;
	__declspec(align(16)) Vector  m_Delta;
	__declspec(align(16)) Vector  m_StartOffset;
	__declspec(align(16)) Vector  m_Extents;
	__declspec(align(16)) Vector  m_dt;
	//without your matrix3x4
	bool	m_IsRay;
	bool	m_IsSwept;
	const   matrix* m_pWorldAxisTransform;
	Ray_t() : m_pWorldAxisTransform(NULL) { }
	void init(Vector src, Vector end) {
		*reinterpret_cast<Vector*>(&m_Delta) = end - src;
		m_IsSwept = m_Delta.Length() != 0.f;
		m_Extents.x = m_Extents.y = m_Extents.z = 0.0f;
		m_pWorldAxisTransform = nullptr;
		m_StartOffset.x = m_StartOffset.y = m_StartOffset.z = 0.0f;
		*reinterpret_cast<Vector*>(&m_Start) = src;
	}
	void Init(const Vector &vecStart, const Vector &vecEnd)
	{
		m_Delta = vecEnd - vecStart;

		m_IsSwept = (m_Delta.LengthSqr() != 0);

		m_Extents.x = m_Extents.y = m_Extents.z = 0.0f;

		m_IsRay = true;

		m_StartOffset.x = m_StartOffset.y = m_StartOffset.z = 0.0f;

		m_Start = vecStart;
	}

	void Init(const Vector & start, const Vector & end, const Vector & mins, const Vector & maxs)
	{
		Assert(&end);
		VectorSubtract(end, start, m_Delta);

		m_pWorldAxisTransform = NULL;
		m_IsSwept = (m_Delta.LengthSqr() != 0);

		VectorSubtract(maxs, mins, m_Extents);
		m_Extents *= 0.5f;
		m_IsRay = (m_Extents.LengthSqr() < 1e-6);

		// Offset m_Start to be in the center of the box...
		VectorAdd(mins, maxs, m_StartOffset);
		m_StartOffset *= 0.5f;
		VectorAdd(start, m_StartOffset, m_Start);
		m_StartOffset *= -1.0f;
	}
};

struct cplane_t
{
	Vector normal;
	float dist;
	BYTE type;
	BYTE signbits;
	BYTE pad[2];
};

class ITraceFilter
{
public:
	virtual bool ShouldHitEntity(IBasePlayer* pEntityHandle, int contentsMask)
	{
		return !(pEntityHandle == pSkip);
	}
	virtual TraceType_t	GetTraceType()
	{
		return TRACE_EVERYTHING;
	}
	void* pSkip;
};


class CTraceFilter : public ITraceFilter
{
public:
	bool ShouldHitEntity(IBasePlayer* pCBaseEntityHandle, int contentsMask)
	{
		return !(pCBaseEntityHandle == pSkip);
	}

	virtual TraceType_t GetTraceType() const
	{
		return TRACE_EVERYTHING;
	}

	void* pSkip;
};
class CTraceFilterOneEntity : public ITraceFilter
{
public:
	bool ShouldHitEntity(IBasePlayer* pEntityHandle, int contentsMask)
	{
		return (pEntityHandle == pEntity);
	}

	TraceType_t GetTraceType() const
	{
		return TRACE_EVERYTHING;
	}

	void* pEntity;
};
class CTraceFilterNoPlayers : public CTraceFilter
{
public:
	CTraceFilterNoPlayers() {}
	virtual bool ShouldHitEntity(IBasePlayer* pEntityHandle, int contentsMask) override
	{
		return false;
	}
};
class CTraceCBaseEntity : public ITraceFilter
{
public:
	bool ShouldHitEntity(IBasePlayer* pCBaseEntityHandle, int contentsMask)
	{
		return (pCBaseEntityHandle == pHit);
	}
	virtual TraceType_t	GetTraceType() const
	{
		return TRACE_ENTITIES_ONLY;
	}
	void* pHit;
};
class CTraceFilterNoPlayer : public CTraceFilter
{
public:
	CTraceFilterNoPlayer() {}
	virtual bool ShouldHitEntity(IBasePlayer *pServerCBaseEntity, int contentsMask)
	{
		if (pServerCBaseEntity)
			return !pServerCBaseEntity->IsPlayer();
		return false;
	}
};
class CTraceFilterEntity : public ITraceFilter
{
public:
	bool ShouldHitEntity(IBasePlayer* pEntityHandle, int contentsMask)
	{
		return pEntityHandle == pHit;
	}

	TraceType_t GetTraceType() const
	{
		return TRACE_ENTITIES_ONLY;
	}

	void* pHit;
};
class CTraceFilterSkipTwoEntities : public ITraceFilter
{
public:
	CTraceFilterSkipTwoEntities() {}
	CTraceFilterSkipTwoEntities(void *pPassEnt1, void *pPassEnt2)
	{
		pPassEntity1 = pPassEnt1;
		pPassEntity2 = pPassEnt2;
	}

	virtual bool ShouldHitEntity(IBasePlayer *pEntityHandle, int contentsMask)
	{
		return !(pEntityHandle == pPassEntity1 || pEntityHandle == pPassEntity2);
	}

	virtual TraceType_t GetTraceType() const
	{
		return TRACE_EVERYTHING;
	}

	void *pPassEntity1;
	void *pPassEntity2;
};


class CBaseTrace
{
public:
	Vector startpos;
	Vector endpos;
	cplane_t plane;
	float fraction;
	int contents;
	unsigned short dispFlags;
	bool allsolid;
	bool startsolid;
};

struct csurface_t
{
	const char* name;
	short surfaceProps;
	unsigned short flags;
};

class CGameTrace : public CBaseTrace
{
public:
	bool DidHitWorld() const;

	bool DidHitNonWorldEntity() const;

	int GetEntityIndex() const;

	bool DidHit() const;

	bool IsVisible() const;

public:
	float fractionleftsolid;
	csurface_t surface;
	int hitgroup;
	short physicsbone;
	unsigned short worldSurfaceIndex;
	IBasePlayer* m_pEnt;
	int hitbox;
};

typedef CGameTrace trace_t;

inline bool CGameTrace::DidHit() const
{
	return fraction < 1.0f || allsolid || startsolid;
}
inline bool CGameTrace::IsVisible() const
{
	return fraction > 0.97f;
}


class CTraceFilterWorldAndPropsOnly : public ITraceFilter
{
public:
	bool ShouldHitEntity(IBasePlayer* pServerEntity, int contentsMask)
	{
		return false;
	}
	virtual TraceType_t	GetTraceType() const
	{
		return TRACE_EVERYTHING;
	}
};

class CTraceWorldOnly : public ITraceFilter
{
public:
	bool ShouldHitEntity(IBasePlayer* pCBaseEntityHandle, int contentsMask)
	{
		return false;
	}

	TraceType_t GetTraceType() const
	{
		return TRACE_WORLD_ONLY;
	}
};
