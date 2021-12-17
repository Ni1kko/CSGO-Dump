#pragma once

#include <Windows.h>
#include "../bspflags.hpp"

#pragma region MASKS

#define   DISPSURF_FLAG_SURFACE           (1<<0)
#define   DISPSURF_FLAG_WALKABLE          (1<<1)
#define   DISPSURF_FLAG_BUILDABLE         (1<<2)
#define   DISPSURF_FLAG_SURFPROP1         (1<<3)
#define   DISPSURF_FLAG_SURFPROP2         (1<<4)

#define   CONTENTS_EMPTY                0

#define   CONTENTS_SOLID                0x1       
#define   CONTENTS_WINDOW               0x2
#define   CONTENTS_AUX                  0x4
#define   CONTENTS_GRATE                0x8
#define   CONTENTS_SLIME                0x10
#define   CONTENTS_WATER                0x20
#define   CONTENTS_BLOCKLOS             0x40 
#define   CONTENTS_OPAQUE               0x80 

#define   ALL_VISIBLE_CONTENTS            (LAST_VISIBLE_CONTENTS | (LAST_VISIBLE_CONTENTS-1))

#define   CONTENTS_TESTFOGVOLUME        0x100
#define   CONTENTS_UNUSED               0x200     
#define   CONTENTS_BLOCKLIGHT           0x400
#define   CONTENTS_TEAM1                0x800 
#define   CONTENTS_TEAM2                0x1000 
#define   CONTENTS_IGNORE_NODRAW_OPAQUE 0x2000
#define   CONTENTS_MOVEABLE             0x4000
#define   CONTENTS_AREAPORTAL           0x8000
#define   CONTENTS_PLAYERCLIP           0x10000
#define   CONTENTS_MONSTERCLIP          0x20000
#define   CONTENTS_CURRENT_0            0x40000
#define   CONTENTS_CURRENT_90           0x80000
#define   CONTENTS_CURRENT_180          0x100000
#define   CONTENTS_CURRENT_270          0x200000
#define   CONTENTS_CURRENT_UP           0x400000
#define   CONTENTS_CURRENT_DOWN         0x800000

#define   CONTENTS_ORIGIN               0x1000000 

#define   CONTENTS_MONSTER              0x2000000 
#define   CONTENTS_DEBRIS               0x4000000
#define   CONTENTS_DETAIL               0x8000000 
#define   CONTENTS_TRANSLUCENT          0x10000000
#define   CONTENTS_LADDER               0x20000000
#define   CONTENTS_HITBOX               0x40000000

#define   SURF_LIGHT                    0x0001 
#define   SURF_SKY2D                    0x0002 
#define   SURF_SKY                      0x0004 
#define   SURF_WARP                     0x0008 
#define   SURF_TRANS                    0x0010
#define   SURF_NOPORTAL                 0x0020 
#define   SURF_TRIGGER                  0x0040 
#define   SURF_NODRAW                   0x0080 

#define   SURF_HINT                     0x0100 

#define   SURF_SKIP                     0x0200   
#define   SURF_NOLIGHT                  0x0400   
#define   SURF_BUMPLIGHT                0x0800   
#define   SURF_NOSHADOWS                0x1000   
#define   SURF_NODECALS                 0x2000   
#define   SURF_NOPAINT                  SURF_NODECALS
#define   SURF_NOCHOP                   0x4000   
#define   SURF_HITBOX                   0x8000   


#define   MASK_ALL                      (0xFFFFFFFF)
#define   MASK_SOLID                    (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_MONSTER|CONTENTS_GRATE)
#define   MASK_PLAYERSOLID              (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_PLAYERCLIP|CONTENTS_WINDOW|CONTENTS_MONSTER|CONTENTS_GRATE)
#define   MASK_NPCSOLID                 (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTERCLIP|CONTENTS_WINDOW|CONTENTS_MONSTER|CONTENTS_GRATE)
#define   MASK_NPCFLUID                 (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTERCLIP|CONTENTS_WINDOW|CONTENTS_MONSTER)
#define   MASK_WATER                    (CONTENTS_WATER|CONTENTS_MOVEABLE|CONTENTS_SLIME)
#define   MASK_OPAQUE                   (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_OPAQUE)
#define   MASK_OPAQUE_AND_NPCS          (MASK_OPAQUE|CONTENTS_MONSTER)
#define   MASK_BLOCKLOS                 (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_BLOCKLOS)
#define   MASK_BLOCKLOS_AND_NPCS        (MASK_BLOCKLOS|CONTENTS_MONSTER)
#define   MASK_VISIBLE                  (MASK_OPAQUE|CONTENTS_IGNORE_NODRAW_OPAQUE)
#define   MASK_VISIBLE_AND_NPCS         (MASK_OPAQUE_AND_NPCS|CONTENTS_IGNORE_NODRAW_OPAQUE)
#define   MASK_SHOT                     (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTER|CONTENTS_WINDOW|CONTENTS_DEBRIS|CONTENTS_HITBOX)
#define   MASK_SHOT_BRUSHONLY           (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_DEBRIS)
#define   MASK_SHOT_HULL                (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTER|CONTENTS_WINDOW|CONTENTS_DEBRIS|CONTENTS_GRATE)
#define   MASK_SHOT_PORTAL              (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_MONSTER)
#define   MASK_SOLID_BRUSHONLY          (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_GRATE)
#define   MASK_PLAYERSOLID_BRUSHONLY    (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_PLAYERCLIP|CONTENTS_GRATE)
#define   MASK_NPCSOLID_BRUSHONLY       (CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_MONSTERCLIP|CONTENTS_GRATE)
#define   MASK_NPCWORLDSTATIC           (CONTENTS_SOLID|CONTENTS_WINDOW|CONTENTS_MONSTERCLIP|CONTENTS_GRATE)
#define   MASK_NPCWORLDSTATIC_FLUID     (CONTENTS_SOLID|CONTENTS_WINDOW|CONTENTS_MONSTERCLIP)
#define   MASK_SPLITAREAPORTAL          (CONTENTS_WATER|CONTENTS_SLIME)
#define   MASK_CURRENT                  (CONTENTS_CURRENT_0|CONTENTS_CURRENT_90|CONTENTS_CURRENT_180|CONTENTS_CURRENT_270|CONTENTS_CURRENT_UP|CONTENTS_CURRENT_DOWN)
#define   MASK_DEADSOLID                (CONTENTS_SOLID|CONTENTS_PLAYERCLIP|CONTENTS_WINDOW|CONTENTS_GRATE)
#pragma endregion

#include "../vector.hpp"
#include <cstdint>
#include "../baseClient.hpp"
#include "../baseEntity.hpp"


struct C_Ray
{
	VectorAligned m_Start; // starting point, centered within the extents
	VectorAligned m_Delta; // direction + length of the ray
	VectorAligned m_StartOffset; // Add this to m_Start to get the actual ray start
	VectorAligned m_Extents; // Describes an axis aligned box extruded along a ray
	const matrix3x4_t* m_pWorldAxisTransform;
	//const matrix3x4_t *m_pWorldAxisTransform;
	bool m_IsRay; // are the extents zero?
	bool m_IsSwept; // is delta != 0?

	C_Ray() : m_pWorldAxisTransform(nullptr), m_IsRay(false), m_IsSwept(false) {}

	C_Ray(Vector3 _start, Vector3 _end)
	{
		Init(_start, _end);
	}

	C_Ray(Vector3 _start, Vector3 _end, Vector3 _mins, Vector3 _maxs)
	{
		Init(_start, _end, _mins, _maxs);
	}

	void Init(const Vector3& vecStart, const Vector3& vecEnd)
	{
		m_Delta = vecEnd - vecStart;

		m_IsSwept = (m_Delta.LengthSqr() != 0);

		m_Extents.x = m_Extents.y = m_Extents.z = 0.0f;
		m_IsRay = true;

		m_StartOffset.x = m_StartOffset.y = m_StartOffset.z = 0.0f;

		m_Start = vecStart;
	}

	void Init(Vector3& vecStart, Vector3& vecEnd, Vector3 min, Vector3 max)
	{
		m_Delta = vecEnd - vecStart;

		m_IsSwept = (m_Delta.LengthSqr() != 0);

		m_Extents.x = (max.x - min.x);
		m_Extents.y = (max.y - min.y);
		m_Extents.z = (max.z - min.z);
		m_IsRay = false;

		m_StartOffset.x = m_StartOffset.y = m_StartOffset.z = 0.0f;

		m_Start = vecStart + ((max + min) * 0.5f);
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
	Vector3 normal;
	float dist;
	uint8_t type;
	uint8_t signbits;
	uint8_t pad[2];
};

class CBaseTrace
{
public:
	bool IsDispSurface(void) { return ((dispFlags & DISPSURF_FLAG_SURFACE) != 0); }
	bool IsDispSurfaceWalkable(void) { return ((dispFlags & DISPSURF_FLAG_WALKABLE) != 0); }
	bool IsDispSurfaceBuildable(void) { return ((dispFlags & DISPSURF_FLAG_BUILDABLE) != 0); }
	bool IsDispSurfaceProp1(void) { return ((dispFlags & DISPSURF_FLAG_SURFPROP1) != 0); }
	bool IsDispSurfaceProp2(void) { return ((dispFlags & DISPSURF_FLAG_SURFPROP2) != 0); }

public:

	// these members are aligned!!
	Vector3         start;            // start position
	Vector3         end;              // final position
	cplane_t       plane;               // surface normal at impact

	float          flFraction;            // time completed, 1.0 = didn't hit anything

	int            contents;            // contents on other side of surface hit
	unsigned short dispFlags;           // displacement flags for marking surfaces with data

	bool           allSolid;            // if true, plane is not valid
	bool           startSolid;          // if true, the initial point was in a solid area

	CBaseTrace() {}

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

	float               fractionleftsolid;  // time we left a solid, only valid if we started in solid
	csurface_t          surface;            // surface hit (impact surface)
	int                 hitGroup;           // 0 == generic, non-zero is specific body part
	short               physicsbone;        // physics bone hit by trace in studio
	unsigned short      worldSurfaceIndex;  // Index of the msurface2_t, if applicable
	CBaseEntity*      m_pEnt;
	int                 hitbox;                       // box hit by trace in studio

	CGameTrace() {}

private:
	// No copy constructors allowed
	CGameTrace(const CGameTrace& other) :
		fractionleftsolid(other.fractionleftsolid),
		surface(other.surface),
		hitGroup(other.hitGroup),
		physicsbone(other.physicsbone),
		worldSurfaceIndex(other.worldSurfaceIndex),
		m_pEnt(other.m_pEnt),
		hitbox(other.hitbox)
	{
		start = other.start;
		end = other.end;
		plane = other.plane;
		flFraction = other.flFraction;
		contents = other.contents;
		dispFlags = other.dispFlags;
		allSolid = other.allSolid;
		startSolid = other.startSolid;
	}
};

inline bool CGameTrace::DidHit() const
{
	return flFraction < 1 || allSolid || startSolid;
}

inline bool CGameTrace::IsVisible() const
{
	return flFraction > 0.97f;
}

enum class TraceType_t
{
	TRACE_EVERYTHING = 0,
	TRACE_WORLD_ONLY, // NOTE: This does *not* test static props!!!
	TRACE_ENTITIES_ONLY, // NOTE: This version will *not* test static props
	TRACE_EVERYTHING_FILTER_PROPS, // NOTE: This version will pass the IHandleEntity for props through the filter, unlike all other filters
};

class ITraceFilter
{
public:
	virtual bool ShouldHitEntity(IHandleEntity *pEntity, int contentsMask) = 0;
	virtual TraceType_t GetTraceType() const = 0;
};


class CTraceFilter : public ITraceFilter
{
public:

	bool ShouldHitEntity(IHandleEntity* pEntityHandle, int /*contentsMask*/)
	{
		CClientClass* pEntCC = ((IClientEntity*)pEntityHandle)->GetClientClass();
		if (pEntCC && strcmp(ccIgnore, ""))
		{
			if (pEntCC->NetworkName == ccIgnore)
				return false;
		}

		return !(pEntityHandle == pSkip1);
	}

	virtual TraceType_t GetTraceType() const
	{
		return type;
	}

	inline void SetIgnoreClass(char* Class)
	{
		ccIgnore = Class;
	}


	void* pSkip1;
	const char* ccIgnore = "";
	TraceType_t type;
};

class CTraceFilterSkipEntity : public ITraceFilter
{
public:
	CTraceFilterSkipEntity(IHandleEntity* pEntityHandle)
	{
		pSkip = pEntityHandle;
	}

	bool ShouldHitEntity(IHandleEntity* pEntityHandle, int /*contentsMask*/)
	{
		return !(pEntityHandle == pSkip);
	}
	virtual TraceType_t GetTraceType() const
	{
		return TraceType_t::TRACE_EVERYTHING;
	}
	void* pSkip;
};

class CTraceFilterEntitiesOnly : public ITraceFilter
{
public:
	bool ShouldHitEntity(IHandleEntity* pEntityHandle, int /*contentsMask*/)
	{
		return true;
	}
	virtual TraceType_t GetTraceType() const
	{
		return TraceType_t::TRACE_ENTITIES_ONLY;
	}
};


//-----------------------------------------------------------------------------
// Classes need not inherit from these
//-----------------------------------------------------------------------------
class CTraceFilterWorldOnly : public ITraceFilter
{
public:
	bool ShouldHitEntity(IHandleEntity* /*pServerEntity*/, int /*contentsMask*/)
	{
		return false;
	}
	virtual TraceType_t GetTraceType() const
	{
		return TraceType_t::TRACE_WORLD_ONLY;
	}
};

class CTraceFilterWorldAndPropsOnly : public ITraceFilter
{
public:
	bool ShouldHitEntity(IHandleEntity* /*pServerEntity*/, int /*contentsMask*/)
	{
		return false;
	}
	virtual TraceType_t GetTraceType() const
	{
		return TraceType_t::TRACE_EVERYTHING;
	}
};

class CTraceFilterPlayersOnlySkipOne : public ITraceFilter
{
public:
	CTraceFilterPlayersOnlySkipOne(void* ent)
	{
		pEnt = ent;
	}
	bool ShouldHitEntity(IHandleEntity* pEntityHandle, int /*contentsMask*/)
	{
		return pEntityHandle != pEnt && ((CBaseEntity*)pEntityHandle)->GetClientClass()->iClassID == 35;
	}
	virtual TraceType_t GetTraceType() const
	{
		return TraceType_t::TRACE_ENTITIES_ONLY;
	}

private:
	void * pEnt;
};

class CTraceFilterSkipTwoEntities : public ITraceFilter
{
public:
	CTraceFilterSkipTwoEntities(void* ent1, void* ent2)
	{
		pEnt1 = ent1;
		pEnt2 = ent2;
	}
	bool ShouldHitEntity(IHandleEntity* pEntityHandle, int /*contentsMask*/)
	{
		return !(pEntityHandle == pEnt1 || pEntityHandle == pEnt2);
	}
	virtual TraceType_t GetTraceType() const
	{
		return TraceType_t::TRACE_EVERYTHING;
	}

private:
	void * pEnt1;
	void * pEnt2;
};

class CTraceFilterHitAll : public CTraceFilter
{
public:
	virtual bool ShouldHitEntity(IHandleEntity* /*pServerEntity*/, int /*contentsMask*/)
	{
		return true;
	}
};


enum class DebugTraceCounterBehavior_t
{
	kTRACE_COUNTER_SET = 0,
	kTRACE_COUNTER_INC,
};

class IEngineTrace
{
public:
	// Returns the contents mask + entity at a particular world-space position
	virtual int		GetPointContents(const Vector3 &vecAbsPosition, int contentsMask = mask_all, IHandleEntity** ppEntity = NULL) = 0;

	// Returns the contents mask of the world only @ the world-space position (static props are ignored)
	virtual int		GetPointContents_WorldOnly(const Vector3 &vecAbsPosition, int contentsMask = mask_all) = 0;

	// Get the point contents, but only test the specific entity. This works
	// on static props and brush models.
	//
	// If the entity isn't a static prop or a brush model, it returns CONTENTS_EMPTY and sets
	// bFailed to true if bFailed is non-null.
	virtual int		GetPointContents_Collideable(ICollideable *pCollide, const Vector3 &vecAbsPosition) = 0;

	// Traces a ray against a particular entity
	virtual void	ClipRayToEntity(const C_Ray &ray, unsigned int fMask, IHandleEntity *pEnt, CGameTrace *pTrace) = 0;

	// Traces a ray against a particular entity
	virtual void	ClipRayToCollideable(const C_Ray &ray, unsigned int fMask, ICollideable *pCollide, CGameTrace *pTrace) = 0;

	// A version that simply accepts a ray (can work as a traceline or tracehull)
	virtual void	TraceRay(const C_Ray &ray, unsigned int fMask, ITraceFilter *pTraceFilter, CGameTrace *pTrace) = 0;
};
