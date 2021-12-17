#pragma once

#include "bspflags.h"

#define CHAR_TEX_ANTLION		'A'
#define CHAR_TEX_BLOODYFLESH	'B'
#define	CHAR_TEX_CONCRETE		'C'
#define CHAR_TEX_DIRT			'D'
#define CHAR_TEX_EGGSHELL		'E' ///< the egg sacs in the tunnels in ep2.
#define CHAR_TEX_FLESH			'F'
#define CHAR_TEX_GRATE			'G'
#define CHAR_TEX_ALIENFLESH		'H'
#define CHAR_TEX_CLIP			'I'
//#define CHAR_TEX_UNUSED		'J'
#define CHAR_TEX_SNOW			'K'
#define CHAR_TEX_PLASTIC		'L'
#define CHAR_TEX_METAL			'M'
#define CHAR_TEX_SAND			'N'
#define CHAR_TEX_FOLIAGE		'O'
#define CHAR_TEX_COMPUTER		'P'
//#define CHAR_TEX_UNUSED		'Q'
#define CHAR_TEX_REFLECTIVE		'R'
#define CHAR_TEX_SLOSH			'S'
#define CHAR_TEX_TILE			'T'
#define CHAR_TEX_CARDBOARD		'U'
#define CHAR_TEX_VENT			'V'
#define CHAR_TEX_WOOD			'W'
//#define CHAR_TEX_UNUSED		'X' ///< do not use - "fake" materials use this (ladders, wading, clips, etc)
#define CHAR_TEX_GLASS			'Y'
#define CHAR_TEX_WARPSHIELD		'Z' ///< wierd-looking jello effect for advisor shield.

#define CHAR_TEX_STEAM_PIPE		11

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


	C_Ray(Vector _start, Vector _end)
	{
		Init(_start, _end);
	}

	C_Ray(Vector _start, Vector _end, Vector _mins, Vector _maxs)
	{
		Init(_start, _end, _mins, _maxs);
	}

	void Init(const Vector& vecStart, const Vector& vecEnd)
	{
		m_Delta = vecEnd - vecStart;

		m_IsSwept = (m_Delta.LengthSqr() != 0);

		m_Extents.x = m_Extents.y = m_Extents.z = 0.0f;
		m_IsRay = true;

		m_StartOffset.x = m_StartOffset.y = m_StartOffset.z = 0.0f;

		m_Start = vecStart;
	}

	void Init(Vector& vecStart, Vector& vecEnd, Vector min, Vector max)
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
	Vector normal;
	float dist;
	byte type;
	byte signbits;
	byte pad[2];
};

struct C_Trace
{
	bool DidHitWorld() const
	{
		return m_pEnt && m_pEnt->EntIndex() == 0;
	}

	bool DidHitNonWorldEntity() const
	{
		return m_pEnt != NULL && !DidHitWorld();
	}

	bool DidHit() const
	{
		return flFraction < 1.f;
	}

	Vector start;
	Vector end;
	cplane_t plane;
	float flFraction;
	int contents;
	WORD dispFlags;
	bool allsolid;
	bool startSolid;
	float fractionLeftSolid;
	csurface_t surface;
	int hitGroup;
	short physicsBone;
	C_BaseEntity* m_pEnt;
	int hitbox;
};

enum TraceType_t
{
	TRACE_EVERYTHING = 0,
	TRACE_WORLD_ONLY, // NOTE: This does *not* test static props!!!
	TRACE_ENTITIES_ONLY, // NOTE: This version will *not* test static props
	TRACE_EVERYTHING_FILTER_PROPS, // NOTE: This version will pass the IHandleEntity for props through the filter, unlike all other filters
};

class ITraceFilter
{
public:
	virtual bool ShouldHitEntity(void* pEntity, int contentsMask) = 0;
	virtual TraceType_t GetTraceType() const = 0;
};

class C_TraceFilter : public ITraceFilter
{
public:

	explicit C_TraceFilter(C_BaseEntity* entity, TraceType_t tracetype = TRACE_EVERYTHING)
	{
		pSkip1 = entity;
	}

	bool ShouldHitEntity(void* pEntityHandle, int contentsMask)
	{
		return (pEntityHandle != pSkip1);
	}

	TraceType_t GetTraceType() const
	{
		return TRACE_EVERYTHING;
	}

	void* pSkip1;
};

class CTraceFilterSkipEntity : public ITraceFilter {
public:
	void *m_skip;

	CTraceFilterSkipEntity(void *pEntityHandle) {
		m_skip = pEntityHandle;
	}

	bool ShouldHitEntity(void *pEntityHandle, int /*contentsMask*/) override {
		return !(pEntityHandle == m_skip);
	}

	TraceType_t GetTraceType() const override {
		return TraceType_t::TRACE_EVERYTHING;
	}
};


class CTraceFilterOneEntity : public ITraceFilter
{
public:

	explicit CTraceFilterOneEntity(C_BaseEntity* entity, TraceType_t tracetype = TRACE_EVERYTHING)
	{
		pEntity = entity;
	}

	bool ShouldHitEntity(void* pEntityHandle, int contentsMask)
	{
		return (pEntityHandle == pEntity);
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

	explicit CTraceFilterOneEntity2(C_BaseEntity* entity, TraceType_t tracetype = TRACE_EVERYTHING)
	{
		pEntity = entity;
	}

	bool ShouldHitEntity(void* pEntityHandle, int contentsMask)
	{
		return (pEntityHandle == pEntity);
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
	CTraceFilterSkipTwoEntities(void* pPassEnt1, void* pPassEnt2)
	{
		passentity1 = pPassEnt1;
		passentity2 = pPassEnt2;
	}

	virtual bool ShouldHitEntity(void* pEntityHandle, int contentsMask)
	{
		return !(pEntityHandle == passentity1 || pEntityHandle == passentity2);
	}

	virtual TraceType_t GetTraceType() const
	{
		return TRACE_EVERYTHING;
	}

	void* passentity1;
	void* passentity2;
};

class CTraceEntity : public ITraceFilter
{
public:

	explicit CTraceEntity(C_BaseEntity* entity, TraceType_t tracetype = TRACE_EVERYTHING)
	{
		pSkip1 = entity;
	}

	bool ShouldHitEntity(void* pEntityHandle, int contentsMask)
	{
		return !(pEntityHandle == pSkip1);
	}

	TraceType_t GetTraceType() const
	{
		return TRACE_ENTITIES_ONLY;
	}

	void* pSkip1;
};

class CTraceFilter : public ITraceFilter
{
public:

	bool ShouldHitEntity(C_BaseEntity* pEntityHandle, int contentsMask)
	{
		return !(pEntityHandle == pSkip);
	}

	TraceType_t	GetTraceType()
	{
		return TRACE_EVERYTHING;
	}
	void* pSkip;
};

class CTraceWorldOnly : public ITraceFilter
{
public:

	explicit CTraceWorldOnly(C_BaseEntity* entity, TraceType_t tracetype = TRACE_EVERYTHING)
	{
		pSkip1 = entity;
	}

	bool ShouldHitEntity(void* pEntityHandle, int contentsMask)
	{
		return false;
	}

	TraceType_t GetTraceType() const
	{
		return TRACE_EVERYTHING;
	}

	void* pSkip1;
};

class CTraceFilterWorldOnly : public ITraceFilter
{
public:
	bool ShouldHitEntity(void* pEntityHandle, int contentsMask)
	{
		return false;
	}
	virtual TraceType_t GetTraceType() const
	{
		return TRACE_EVERYTHING;
	}
};

class i_trace_filter {
public:
	virtual bool ShouldHitEntity(void* pEntity, int contentsMask) = 0;
	virtual TraceType_t GetTraceType() const = 0;
};

class trace_filter : public i_trace_filter {
public:
	bool ShouldHitEntity(void* pEntityHandle, int contentsMask) {
		return (pEntityHandle != skip);
	}

	TraceType_t GetTraceType() const {
		return TRACE_EVERYTHING;
	}

	void* skip;
};

class trace_filter_one_entity : public i_trace_filter {
public:
	bool ShouldHitEntity(void* pEntityHandle, int contentsMask) {
		return (pEntityHandle == pEntity);
	}

	TraceType_t GetTraceType() const {
		return TRACE_EVERYTHING;
	}

	void* pEntity;
};

class trace_filter_one_entity2 : public i_trace_filter {
public:
	bool ShouldHitEntity(void* pEntityHandle, int contentsMask) {
		return (pEntityHandle == pEntity);
	}

	TraceType_t GetTraceType() const {
		return TRACE_ENTITIES_ONLY;
	}

	void* pEntity;
};

class trace_filter_skip_two_entities : public i_trace_filter {
public:
	trace_filter_skip_two_entities(void* pPassEnt1, void* pPassEnt2) {
		passentity1 = pPassEnt1;
		passentity2 = pPassEnt2;
	}

	virtual bool ShouldHitEntity(void* pEntityHandle, int contentsMask) {
		return !(pEntityHandle == passentity1 || pEntityHandle == passentity2);
	}

	virtual TraceType_t GetTraceType() const {
		return TRACE_EVERYTHING;
	}

	void* passentity1;
	void* passentity2;
};

class trace_filter_skip_one_entity : public i_trace_filter {
public:
	trace_filter_skip_one_entity(void* pPassEnt1) {
		passentity1 = pPassEnt1;
	}

	virtual bool ShouldHitEntity(void* pEntityHandle, int contentsMask) {
		return !(pEntityHandle == passentity1);
	}

	virtual TraceType_t GetTraceType() const {
		return TRACE_EVERYTHING;
	}

	void* passentity1;
};


class trace_entity : public i_trace_filter {
public:
	bool ShouldHitEntity(void* pEntityHandle, int contentsMask) {
		return !(pEntityHandle == pSkip1);
	}

	TraceType_t GetTraceType() const {
		return TRACE_ENTITIES_ONLY;
	}

	void* pSkip1;
};

class trace_world_only : public i_trace_filter {
public:
	bool ShouldHitEntity(void* pEntityHandle, int contentsMask) {
		return false;
	}

	TraceType_t GetTraceType() const {
		return TRACE_EVERYTHING;
	}

	void* pSkip1;
};

class IEngineTrace
{
public:
	// Returns the contents mask + entity at a particular world-space position
	virtual int		GetPointContents(const Vector &vecAbsPosition, int contentsMask = mask_all, IHandleEntity** ppEntity = NULL) = 0;

	// Returns the contents mask of the world only @ the world-space position (static props are ignored)
	virtual int		GetPointContents_WorldOnly(const Vector &vecAbsPosition, int contentsMask = mask_all) = 0;

	// Get the point contents, but only test the specific entity. This works
	// on static props and brush models.
	//
	// If the entity isn't a static prop or a brush model, it returns CONTENTS_EMPTY and sets
	// bFailed to true if bFailed is non-null.
	virtual int		GetPointContents_Collideable(ICollideable *pCollide, const Vector &vecAbsPosition) = 0;

	// Traces a ray against a particular entity
	virtual void	ClipRayToEntity(const C_Ray &ray, unsigned int fMask, C_BaseEntity *pEnt, C_Trace *pTrace) = 0;

	// Traces a ray against a particular entity
	virtual void	ClipRayToCollideable(const C_Ray &ray, unsigned int fMask, ICollideable *pCollide, C_Trace *pTrace) = 0;

	// A version that simply accepts a ray (can work as a traceline or tracehull)
	virtual void	TraceRay(const C_Ray &ray, unsigned int fMask, ITraceFilter *pTraceFilter, C_Trace *pTrace) = 0;
};

struct ray_t {
	Vector m_start; // starting point, centered within the extents
	Vector m_delta; // direction + length of the ray
	Vector m_start_offset; // Add this to m_Start to get the actual ray start
	Vector m_extents; // Describes an axis aligned box extruded along a ray
	const matrix3x4_t* m_world_axis_transform;
	//const matrix_t *m_pWorldAxisTransform;
	bool m_is_ray; // are the extents zero?
	bool m_is_swept; // is delta != 0?

	void initialize(const Vector& start, const Vector& end) {
		m_delta = end - start;

		m_is_swept = (m_delta.LengthSqr() != 0);

		m_extents.x = m_extents.y = m_extents.z = 0.0f;
		m_is_ray = true;

		m_start_offset.x = m_start_offset.y = m_start_offset.z = 0.0f;

		m_start = start;
	}

	void initialize(Vector& vecStart, Vector& vecEnd, Vector min, Vector max) {
		m_delta = vecEnd - vecStart;

		m_is_swept = (m_delta.LengthSqr() != 0);

		m_extents.x = (max.x - min.x);
		m_extents.y = (max.y - min.y);
		m_extents.z = (max.z - min.z);
		m_is_ray = false;

		m_start_offset.x = m_start_offset.y = m_start_offset.z = 0.0f;

		m_start = vecStart + ((max + min) * 0.5f);
	}

private:
};
struct trace_t {
	bool did_hit() const {
		return flFraction < 1.f;
	}

	Vector start;
	Vector end;
	cplane_t plane;
	float flFraction;
	int contents;
	unsigned short dispFlags;
	bool allsolid;
	bool startSolid;
	float fractionLeftSolid;
	csurface_t surface;
	int hitGroup;
	short physicsBone;
	C_BaseEntity* entity;
	int hitbox;

	bool DidHitWorld() const {
		return false;
	}

	bool DidHitNonWorldEntity() const {
		return entity != NULL && !DidHitWorld();
	}
};

class collideable_t;
class trace {
public:
	virtual int get_point_contents(const Vector &pos, int mask = mask_all,
		IHandleEntity **ent = nullptr) = 0;
	virtual int get_point_contents_world(const Vector &pos, int mask = mask_all) = 0;
	virtual int get_point_contents_collideable(collideable_t *collide, const Vector &pos) = 0;
	virtual void clip_ray_to_entity(const ray_t &ray, unsigned int mask, C_BaseEntity *ent, trace_t *trace) = 0;
	virtual void clip_ray_to_collideable(const ray_t &ray, unsigned int mask, collideable_t *collide, trace_t *trace) =
		0;
	virtual void trace_ray(const ray_t &ray, unsigned int mask, i_trace_filter *filter, trace_t *trace) = 0;
};
extern trace* g_pTraceV2;
extern IEngineTrace* g_pTrace;