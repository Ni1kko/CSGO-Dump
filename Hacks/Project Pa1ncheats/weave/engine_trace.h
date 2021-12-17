#pragma once
#include "common.h"
#include "c_trace.h"
#include "bspflags.h"

struct Ray_t;
#define INVALID_EHANDLE_INDEX	0xFFFFFFFF
class IHandleEntity;
struct virtualmeshlist_t;


class ITraceListData
{
public:
	virtual ~ITraceListData()
	{}

	virtual void Reset() = 0;
	virtual bool IsEmpty() = 0;
	// CanTraceRay will return true if the current volume encloses the ray
	// NOTE: The leaflist trace will NOT check this.  Traces are intersected
	// against the culled volume exclusively.
	virtual bool CanTraceRay(const Ray_t& ray) = 0;
};

class IEntityEnumerator
{
public:
	// This gets called with each handle
	virtual bool EnumEntity(IHandleEntity* pHandleEntity) = 0;
};

struct BrushSideInfo_t
{
	Vector plane; // The plane of the brush side
	float planec;
	unsigned short bevel; // Bevel plane?
	unsigned short thin; // Thin?
};

enum DebugTraceCounterBehavior_t
{
	kTRACE_COUNTER_SET = 0,
	kTRACE_COUNTER_INC,
};
class CTraceFilter;
#define	MASK_ALL					(0xFFFFFFFF)

class IEngineTrace
{
public:
	virtual int   GetPointContents(const Vector &vecAbsPosition, int contentsMask = MASK_ALL, IHandleEntity** ppEntity = nullptr) = 0;
	virtual int   GetPointContents_WorldOnly(const Vector &vecAbsPosition, int contentsMask = MASK_ALL) = 0;
	virtual int   GetPointContents_Collideable(ICollideable *pCollide, const Vector &vecAbsPosition) = 0;
	virtual void  ClipRayToEntity(const Ray_t &ray, unsigned int fMask, IHandleEntity *pEnt, CGameTrace *pTrace) = 0;
	virtual void  ClipRayToCollideable(const Ray_t &ray, unsigned int fMask, ICollideable *pCollide, CGameTrace *pTrace) = 0;
	virtual void  TraceRay(const Ray_t &ray, unsigned int fMask, ITraceFilter *pTraceFilter, CGameTrace *pTrace) = 0;
};