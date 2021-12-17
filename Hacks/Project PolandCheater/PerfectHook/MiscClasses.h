

#pragma once

#include "MiscDefinitions.h"

#include "Vector.h"
#include "bspflags.h"

// Entity List
class C_BaseEntityList
{
public:
	virtual void Function0();
	virtual void Function1();
	virtual void Function2();
	virtual C_BaseEntity *		GetClientEntity(int entnum);
	virtual C_BaseEntity *		GetClientEntityFromHandle(HANDLE hEnt) = 0;
	virtual int					NumberOfEntities(bool bIncludeNonNetworkable) = 0;
	virtual int					GetHighestEntityIndex(void);
	virtual void				SetMaxEntities(int maxents);
	virtual int					GetMaxEntities();

    C_BaseEntity *GetClientEntityFromHandle(CBaseHandle hEnt)
    {
        typedef C_BaseEntity* (__thiscall* OriginalFn)(PVOID, CBaseHandle);
        return call_vfunc<OriginalFn>(this, 4)(this, hEnt);
    }
};


// Panels
class IPanel
{
public:
	const char *GetName(unsigned int vguiPanel)
	{
		typedef const char* (__thiscall* OriginalFn)(PVOID, unsigned int);
		return call_vfunc<OriginalFn>(this, 36)(this, vguiPanel);
	}
};


// Debug Overlay
class IVDebugOverlay
{
public:
	virtual void AddEntityTextOverlay(int ent_index, int line_offset, float duration, int r, int g, int b, int a, const char *format, ...) = 0;
	virtual void AddBoxOverlay(const Vector& origin, const Vector& mins, const Vector& max, Vector const& orientation, int r, int g, int b, int a, float duration) = 0;
	virtual void AddSphereOverlay(const Vector& vOrigin, float flRadius, int nTheta, int nPhi, int r, int g, int b, int a, float flDuration) = 0;
	virtual void AddTriangleOverlay(const Vector& p1, const Vector& p2, const Vector& p3, int r, int g, int b, int a, bool noDepthTest, float duration) = 0;
	virtual void AddLineOverlay(const Vector& origin, const Vector& dest, int r, int g, int b, bool noDepthTest, float duration) = 0;
	virtual void AddTextOverlay(const Vector& origin, float duration, const char *format, ...) = 0;
	virtual void AddTextOverlay(const Vector& origin, int line_offset, float duration, const char *format, ...) = 0;
	virtual void AddScreenTextOverlay(float flXPos, float flYPos, float flDuration, int r, int g, int b, int a, const char *text) = 0;
	virtual void AddSweptBoxOverlay(const Vector& start, const Vector& end, const Vector& mins, const Vector& max, const Vector & angles, int r, int g, int b, int a, float flDuration) = 0;
	virtual void AddGridOverlay(const Vector& origin) = 0;
	virtual void AddCoordFrameOverlay(const matrix3x4& frame, float flScale, int vColorTable[3][3] = NULL) = 0;

	virtual int ScreenPosition(const Vector& point, Vector& screen) = 0;
	virtual int ScreenPosition(float flXPos, float flYPos, Vector& screen) = 0;

	virtual void *GetFirst(void) = 0;
	virtual void *GetNext(void *current) = 0;
	virtual void ClearDeadOverlays(void) = 0;
	virtual void ClearAllOverlays() = 0;

	virtual void AddTextOverlayRGB(const Vector& origin, int line_offset, float duration, float r, float g, float b, float alpha, const char *format, ...) = 0;
	virtual void AddTextOverlayRGB(const Vector& origin, int line_offset, float duration, int r, int g, int b, int a, const char *format, ...) = 0;

	virtual void AddLineOverlayAlpha(const Vector& origin, const Vector& dest, int r, int g, int b, int a, bool noDepthTest, float duration) = 0;
	virtual void AddBoxOverlay2(const Vector& origin, const Vector& mins, const Vector& max, Vector const& orientation, const Color& faceColor, const Color& edgeColor, float duration) = 0;

	virtual void PurgeTextOverlays() = 0;
};

// User Cmd's
class CUserCmd
{
public:
	virtual ~CUserCmd() {}; //Destructor 0
	CUserCmd()
	{
		Reset();
	}

	void Reset()
	{
		command_number = 0;
		tick_count = 0;
		viewangles.Init();
		forwardmove = 0.0f;
		sidemove = 0.0f;
		upmove = 0.0f;
		buttons = 0;
		impulse = 0;
		weaponselect = 0;
		weaponsubtype = 0;
		random_seed = 0;
		mousedx = 0;
		mousedy = 0;
		headangles.Init();
		headoffset.Init();

		hasbeenpredicted = false;
	}

	CUserCmd& operator =(const CUserCmd& src)
	{
		if (this == &src)
			return *this;

		command_number = src.command_number;
		tick_count = src.tick_count;
		viewangles = src.viewangles;
		forwardmove = src.forwardmove;
		sidemove = src.sidemove;
		upmove = src.upmove;
		buttons = src.buttons;
		impulse = src.impulse;
		weaponselect = src.weaponselect;
		weaponsubtype = src.weaponsubtype;
		random_seed = src.random_seed;
		mousedx = src.mousedx;
		mousedy = src.mousedy;

		hasbeenpredicted = src.hasbeenpredicted;
		headangles = src.headangles;
		headoffset = src.headoffset;
		return *this;
	}

	CUserCmd(const CUserCmd& src)
	{
		*this = src;
	}


	int        command_number;
	int        tick_count;
	Vector  viewangles;
	Vector    aimdirection;
	float    forwardmove;
	float    sidemove;
	float    upmove;
	int        buttons;
	BYTE    impulse;
	int        weaponselect;
	int        weaponsubtype;
	int        random_seed;
	short    mousedx;
	short    mousedy;
	bool    hasbeenpredicted;
	Vector  headangles;
	Vector    headoffset;
};


class CGlobalVarsBase
{
public:
	float    realtime;
	int      framecount;
	float    absoluteframetime;
	float    absoluteframestarttimestddev;
	float    curtime;
	float    frametime;
	int      maxClients;
	int      tickcount;
	float    interval_per_tick;
	float    interpolation_amount;
	int      simTicksThisFrame;
	int      network_protocol;
	void*    pSaveData;
	bool     m_bClient;
	int      nTimestampNetworkingBase;
	int      nTimestampRandomizeWindow;
};


struct Ray_t
{
	//__declspec(align(16)) Vector  m_Start;
	//__declspec(align(16)) Vector  m_Delta;
	//__declspec(align(16)) Vector  m_StartOffset;
	//__declspec(align(16)) Vector  m_Extents;
	////without your matrix3x4
	//const matrix3x4	*m_pWorldAxisTransform;

	//bool	m_IsRay;
	//bool	m_IsSwept;

	//void Init(Vector& vecStart, Vector& vecEnd)
	//{
	//	m_Delta = vecEnd - vecStart;
	//	m_IsSwept = (m_Delta.LengthSqr() != 0);
	//	m_Extents.x = m_Extents.y = m_Extents.z = 0.0f;
	//	m_IsRay = true;
	//	m_StartOffset.x = m_StartOffset.y = m_StartOffset.z = 0.0f;
	//	m_Start = vecStart;
	//}


	//Wrong structure000000
	VectorAligned		m_Start;					// starting point, centered within the extents
	VectorAligned		m_Delta;					// direction + length of the ray
	VectorAligned		m_StartOffset;				// Add this to m_Start to get the actual ray start
	VectorAligned		m_Extents;					// Describes an axis aligned box extruded along a ray
	const matrix3x4		*m_pWorldAxisTransform;
	bool				m_IsRay;					// are the extents zero00
	bool				m_IsSwept;					// is delta != 000

	Ray_t() : m_pWorldAxisTransform(NULL) {}

	void Init(Vector const& Start, Vector const& End) {
		m_Delta = End - Start;
		m_IsSwept = (m_Delta.LengthSqr() != 0);

		m_Extents.x = m_Extents.y = m_Extents.z = 0.0f;
		m_pWorldAxisTransform = NULL;
		m_IsRay = true;

		m_Extents.x = m_Extents.y = m_Extents.z = 0.0f;
		m_Start = Start;
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
};

struct cplane_t
{
	Vector  normal;
	float   dist;
	BYTE    type;
	BYTE    signbits;
	BYTE    pad[2];
};

class CBaseTrace
{
public:
	Vector                  startpos;
	Vector                  endpos;
	cplane_t                plane;
	float                   fraction;
	int                             contents;
	unsigned short  dispFlags;
	bool                    allsolid;
	bool                    startsolid;
};

struct csurface_t
{
	const char*             name;
	short                   surfaceProps;
	unsigned short  flags;
};

class CGameTrace : public CBaseTrace
{
public:
	bool                    DidHitWorld() const;
	bool                    DidHitNonWorldEntity() const;
	int                             GetEntityIndex() const;
	bool                    DidHit() const;
public:
	float                   fractionleftsolid;
	csurface_t              surface;
	int                             hitgroup;
	short                   physicsbone;
	unsigned short  worldSurfaceIndex;
	C_BaseEntity*               m_pEnt;
	int                             hitbox;
	char shit[0x24];
};

inline bool CGameTrace::DidHit() const
{
	return fraction < 1.0f || allsolid || startsolid;
}

typedef CGameTrace trace_t;

enum TraceType_t
{
	TRACE_EVERYTHING = 0,
	TRACE_WORLD_ONLY,
	TRACE_ENTITIES_ONLY,
	TRACE_EVERYTHING_FILTER_PROPS,
};
enum ItemDefinitionIndex
{
    ITEM_NONE = 0,
    WEAPON_DEAGLE = 1,
    WEAPON_DUALS = 2,
    WEAPON_FIVE7 = 3,
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
    WEAPON_UMP45 = 24,
    WEAPON_XM1014 = 25,
    WEAPON_BIZON = 26,
    WEAPON_MAG7 = 27,
    WEAPON_NEGEV = 28,
    WEAPON_SAWEDOFF = 29,
    WEAPON_TEC9 = 30,
    WEAPON_TASER = 31,
    WEAPON_P2000 = 32,
    WEAPON_MP7 = 33,
    WEAPON_MP9 = 34,
    WEAPON_NOVA = 35,
    WEAPON_P250 = 36,
    WEAPON_SCAR20 = 38,
    WEAPON_SG553 = 39,
    WEAPON_SCOUT = 40,
    WEAPON_KNIFE_T = 42,
    WEAPON_FLASH = 43,
    WEAPON_HE = 44,
    WEAPON_SMOKE = 45,
    WEAPON_MOLOTOV = 46,
    WEAPON_DECOY = 47,
    WEAPON_INC = 48,
    WEAPON_C4 = 49,
    WEAPON_KNIFE_CT = 59,
    WEAPON_M4A1S = 60,
    WEAPON_USPS = 61,
    WEAPON_CZ75 = 63,
    WEAPON_REVOLVER = 64,
    WEAPON_KNIFE_BAYONET = 500,
    WEAPON_KNIFE_FLIP = 505,
    WEAPON_KNIFE_GUT = 506,
    WEAPON_KNIFE_KARAMBIT = 507,
    WEAPON_KNIFE_M9BAYONET = 508,
    WEAPON_KNIFE_HUNTSMAN = 509,
    WEAPON_KNIFE_FALCHION = 512,
    WEAPON_KNIFE_BOWIE = 514,
    WEAPON_KNIFE_BUTTERFLY = 515,
    WEAPON_KNIFE_DAGGER = 516,
    WEAPON_MAX
};
class ITraceFilter
{
public:
	virtual bool            ShouldHitEntity(C_BaseEntity* pEntity, int contentsMask) = 0;
	virtual TraceType_t     GetTraceType() const = 0;
};

class CTraceFilter : public ITraceFilter
{
public:
	bool ShouldHitEntity(C_BaseEntity* pEntityHandle, int contentsMask)
	{
		return !(pEntityHandle == pSkip);
	}

	TraceType_t GetTraceType() const
	{
		return TRACE_EVERYTHING;
	}

	void* pSkip;
};

class CTraceFilterNoPlayer : public CTraceFilter
{
public:
	CTraceFilterNoPlayer() {}
	virtual bool ShouldHitEntity(C_BaseEntity *pServerEntity, int contentsMask)
	{
		IClientUnknown *pUnk = (IClientUnknown*)pServerEntity;
		C_BaseEntity *pEnt = pUnk->GetBaseEntity();
		return !pEnt->IsPlayer();
	}
};
class CTraceFilterWorldAndPropsOnly : public ITraceFilter
{
public:
    bool ShouldHitEntity(C_BaseEntity* /*pServerEntity*/, int /*contentsMask*/)
    {
        return false;
    }
    virtual TraceType_t GetTraceType() const
    {
        return TRACE_EVERYTHING;
    }
};

class IEngineTrace
{
public:
	int		GetPointContents(const Vector &vecAbsPosition, int contentsMask = MASK_ALL, C_BaseEntity** ppEntity = NULL)
	{
		typedef int(__thiscall* fnGetPointContents)(void*, const Vector&, int, C_BaseEntity**);
		return call_vfunc<fnGetPointContents>(this, 0)(this, vecAbsPosition, contentsMask, ppEntity);
	}

	void	ClipRayToEntity(const Ray_t &ray, unsigned int fMask, C_BaseEntity *pEnt, trace_t *pTrace)
	{
		typedef void(__thiscall* fnGetPointContents)(void*, const Ray_t&, unsigned int, C_BaseEntity*, trace_t*);
		return call_vfunc<fnGetPointContents>(this, 3)(this, ray, fMask, pEnt, pTrace);
	}

	void TraceRay(const Ray_t &ray, unsigned int fMask, ITraceFilter* pTraceFilter, trace_t* pTrace)
	{
		typedef void(__thiscall* fnTraceRay)(void*, const Ray_t&, unsigned int, ITraceFilter*, trace_t*);
		call_vfunc<fnTraceRay>(this, 5)(this, ray, fMask, pTraceFilter, pTrace);
	}

};

struct mstudiobbox_t
{
    int bone;
    int group;
    Vector bbmin;
    Vector bbmax;
    int szhitboxnameindex;
    int m_iPad01[3];
    float m_flRadius;
    int m_iPad02[4];
};
struct mstudiohitboxset_t
{
	int	sznameindex;
	inline char* const GetName(void) const { return ((char*)this) + sznameindex; }
	int	numhitboxes;
	int	hitboxindex;
	inline mstudiobbox_t*   GetHitbox(int i) const { return (mstudiobbox_t*)(((byte*)this) + hitboxindex) + i; };
};
struct mstudiobone_t
{
	int sznameindex;
	inline char * const GetName(void) const { return ((char *)this) + sznameindex; }
	int parent;
	int bonecontroller[6];
	Vector pos;
	float quat[4];
	Vector rot;
	Vector posscale;
	Vector rotscale;
	matrix3x4 poseToBone;
	float qAlignment[4];
	int	flags;
	int	proctype;
	int	procindex;			  // procedural rule
	mutable int	physicsbone;	// index into physically simulated bone
	inline void * GetProcedure() const { if (procindex == 0) return NULL; else return  (void *)(((byte *)this) + procindex); };
	int	surfacepropidx; // index into string tablefor property name
	inline char * const GetSurfaceProps(void) const { return ((char *)this) + surfacepropidx; }
	int contents;			   // See BSPFlags.h for the contents flags
	int	unused[8];			  // remove as appropriate
};
struct studiohdr_t
{
	int id;
	int version;
	int checksum;
	char name[64];
	int length;
	Vector eyeposition;
	Vector illumposition;
	Vector hull_min;
	Vector hull_max;
	Vector view_bbmin;
	Vector view_bbmax;
	int flags;
	int numbones;
	int boneindex;
	inline mstudiobone_t *GetBone(int i) const { return (mstudiobone_t *)(((byte *)this) + boneindex) + i; };
	int numbonecontrollers;
	int bonecontrollerindex;
	int numhitboxsets;
	int hitboxsetindex;
	mstudiohitboxset_t* GetHitboxSet(int i) const
	{
		return (mstudiohitboxset_t*)(((byte*)this) + hitboxsetindex) + i;
	}
	inline mstudiobbox_t* GetHitbox(int i, int set) const
	{
		mstudiohitboxset_t const* s = GetHitboxSet(set);

		if (!s)
			return NULL;

		return s->GetHitbox(i);
	}
	inline int GetHitboxCount(int set) const
	{
		mstudiohitboxset_t const* s = GetHitboxSet(set);
		if (!s)
			return 0;
		return s->numhitboxes;
	}
	int numlocalanim;
	int localanimindex;
	int numlocalseq;
	int localseqindex;
	mutable int activitylistversion;
	mutable int eventsindexed;
	int numtextures;
	int textureindex;
	int numcdtextures;
	int cdtextureindex;
	int numskinref;
	int numskinfamilies;
	int skinindex;
	int numbodyparts;
	int bodypartindex;
	int numlocalattachments;
	int localattachmentindex;
	int numlocalnodes;
	int localnodeindex;
	int localnodenameindex;
	int numflexdesc;
	int flexdescindex;
	int numflexcontrollers;
	int flexcontrollerindex;
	int numflexrules;
	int flexruleindex;
	int numikchains;
	int ikchainindex;
	int nummouths;
	int mouthindex;
	int numlocalposeparameters;
	int localposeparamindex;
	int surfacepropindex;
	int keyvalueindex;
	int keyvaluesize;
	int numlocalikautoplaylocks;
	int localikautoplaylockindex;
	float mass;
	int contents;
	int numincludemodels;
	int includemodelindex;
	mutable void *virtualModel;
	int szanimblocknameindex;
	int numanimblocks;
	int animblockindex;
	mutable void *animblockModel;
	int bonetablebynameindex;
	void *pVertexBase;
	void *pIndexBase;
	byte constdirectionallightdot;
	byte rootLOD;
	byte numAllowedRootLODs;
	byte unused[1];
	int unused4;
	int numflexcontrollerui;
	int flexcontrolleruiindex;
	float flVertAnimFixedPointScale;
	int unused3[1];
	int studiohdr2index;
	int unused2[1];
};


struct surfacephysicsparams_t
{
	float    friction;
	float    elasticity; // collision elasticity - used to compute coefficient of restitution
	float    density;    // physical density (in kg / m^3)
	float    thickness;    // material thickness if not solid (sheet materials) in inches
	float    dampening;
};

struct surfaceaudioparams_t
{
	float    reflectivity;            // like elasticity, but how much sound should be reflected by this surface
	float    hardnessFactor;            // like elasticity, but only affects impact sound choices
	float    roughnessFactor;        // like friction, but only affects scrape sound choices   
	float    roughThreshold;            // surface roughness > this causes "rough" scrapes, < this causes "smooth" scrapes
	float    hardThreshold;            // surface hardness > this causes "hard" impacts, < this causes "soft" impacts
	float    hardVelocityThreshold;    // collision velocity > this causes "hard" impacts, < this causes "soft" impacts   
};

struct surfacesoundnames_t
{
	unsigned short    stepleft;
	unsigned short    stepright;
	unsigned short    impactSoft;
	unsigned short    impactHard;
	unsigned short    scrapeSmooth;
	unsigned short    scrapeRough;
	unsigned short    bulletImpact;
	unsigned short    rolling;
	unsigned short    breakSound;
	unsigned short    strainSound;
};


struct surfacegameprops_t
{
public:
	float    maxSpeedFactor; //0x0000
	float    jumpFactor; //0x0004
	char    pad00[0x4]; //0x0008
	float    flPenetrationModifier; //0x000C
	float    flDamageModifier; //0x0010
	unsigned short    material; //0x0014
	char    pad01[0x3];

};//Size=0x0019

struct surfacedata_t
{
	surfacephysicsparams_t    physics;
	surfaceaudioparams_t    audio;
	surfacesoundnames_t        sounds;
	surfacegameprops_t        game;
};

class IPhysicsSurfaceProps
{
public:

	surfacedata_t	*GetSurfaceData(int surfaceDataIndex)
	{
		typedef surfacedata_t*(__thiscall* fnGetSurfaceData)(void*, int);
		return call_vfunc<fnGetSurfaceData>(this, 5)(this, surfaceDataIndex);
	}
};



class bf_write;
class bf_read;


class IGameEvent {
public:
	const char* GetName() {
		return call_vfunc<const char*(__thiscall *)(void*)>(this, 1)(this);
	}

	int GetInt(const char* szKeyName, int nDefault = 0) {
		return call_vfunc<int(__thiscall *)(void*, const char*, int)>(this, 6)(this, szKeyName, nDefault);
	}

	const char* GetString(const char* szKeyName) {
		return call_vfunc<const char*(__thiscall *)(void*, const char*, int)>(this, 9)(this, szKeyName, 0);
	}

	void SetString(const char* szKeyName, const char* szValue) {
		return call_vfunc<void(__thiscall *)(void*, const char*, const char*)>(this, 16)(this, szKeyName, szValue);
	}
};


class IGameEventListener2 {
public:
	virtual	~IGameEventListener2(void) {};

	// FireEvent is called by EventManager if event just occured
	// KeyValue memory will be freed by manager if not needed anymore
	virtual void FireGameEvent(IGameEvent *event) = 0;

	virtual int	 GetEventDebugID(void) = 0;
};

class IGameEventManager2 {
public:
	virtual	~IGameEventManager2(void) {};

	// load game event descriptions from a file eg "resource\gameevents.res"
	virtual int LoadEventsFromFile(const char *filename) = 0;

	// removes all and anything
	virtual void  Reset() = 0;

	// adds a listener for a particular event
	virtual bool AddListener(IGameEventListener2 *listener, const char *name, bool bServerSide) = 0;

	// returns true if this listener is listens to given event
	virtual bool FindListener(IGameEventListener2 *listener, const char *name) = 0;

	// removes a listener 
	virtual void RemoveListener(IGameEventListener2 *listener) = 0;

	// create an event by name, but doesn't fire it. returns NULL is event is not
	// known or no listener is registered for it. bForce forces the creation even if no listener is active
	virtual IGameEvent *CreateEvent(const char *name, bool bForce = false, int *pCookie = NULL) = 0;

	// fires a server event created earlier, if bDontBroadcast is set, event is not send to clients
	virtual bool FireEvent(IGameEvent *event, bool bDontBroadcast = false) = 0;

	// fires an event for the local client only, should be used only by client code
	virtual bool FireEventClientSide(IGameEvent *event) = 0;

	// create a new copy of this event, must be free later
	virtual IGameEvent *DuplicateEvent(IGameEvent *event) = 0;

	// if an event was created but not fired for some reason, it has to bee freed, same UnserializeEvent
	virtual void FreeEvent(IGameEvent *event) = 0;

	// write/read event to/from bitbuffer
	virtual bool SerializeEvent(IGameEvent *event, bf_write *buf) = 0;
	virtual IGameEvent *UnserializeEvent(bf_read *buf) = 0; // create new KeyValues, must be deleted
};

struct ColorRGBExp32
{
	byte r, g, b;
	signed char exponent;
};

struct dlight_t
{
	int		flags;
	Vector	origin;
	float	radius;
	ColorRGBExp32	color;
	float	die;
	float	decay;
	float	minlight;
	int		key;
	int		style;
	Vector	m_Direction;
	float	m_InnerAngle;
	float	m_OuterAngle;
	float GetRadius() const
	{
		return radius;
	}
	float GetRadiusSquared() const
	{
		return radius * radius;
	}
	float IsRadiusGreaterThanZero() const
	{
		return radius > 0.0f;
	}
};


class IVEffects
{
public:
	dlight_t* CL_AllocDlight(int key)
	{
		typedef dlight_t*(__thiscall* OriginalFn)(PVOID, int);
		return call_vfunc<OriginalFn>(this, 4)(this, key);
	}
	dlight_t* CL_AllocElight(int key)
	{
		typedef dlight_t*(__thiscall* OriginalFn)(PVOID, int);
		return call_vfunc<OriginalFn>(this, 5)(this, key);
	}
	dlight_t* GetElightByKey(int key)
	{
		typedef dlight_t*(__thiscall* OriginalFn)(PVOID, int);
		return call_vfunc<OriginalFn>(this, 8)(this, key);
	}
};


class CMoveData {
public:
	bool			m_bFirstRunOfFunctions : 1;
	bool			m_bGameCodeMovedPlayer : 1;
	bool			m_bNoAirControl : 1;

	unsigned long	m_nPlayerHandle;
	int				m_nImpulseCommand;
	QAngle			m_vecViewAngles;
	QAngle			m_vecAbsViewAngles;
	int				m_nButtons;
	int				m_nOldButtons;
	float			m_flForwardMove;
	float			m_flSideMove;
	float			m_flUpMove;

	float			m_flMaxSpeed;
	float			m_flClientMaxSpeed;

	Vector			m_vecVelocity;
	Vector			m_vecOldVelocity;
	float			somefloat;
	QAngle			m_vecAngles;
	QAngle			m_vecOldAngles;

	float			m_outStepHeight;
	Vector			m_outWishVel;
	Vector			m_outJumpVel;

	Vector			m_vecConstraintCenter;
	float			m_flConstraintRadius;
	float			m_flConstraintWidth;
	float			m_flConstraintSpeedFactor;
	bool			m_bConstraintPastRadius;

	void			SetAbsOrigin(const Vector &vec);
	const Vector	&GetAbsOrigin() const;

private:
	Vector			m_vecAbsOrigin;		// edict::origin
};

class IGameMovement {
public:
	virtual			~IGameMovement(void) {}

	virtual void	ProcessMovement(C_BaseEntity *pPlayer, CMoveData *pMove) = 0;
	virtual void	Reset(void) = 0;
	virtual void	StartTrackPredictionErrors(C_BaseEntity *pPlayer) = 0;
	virtual void	FinishTrackPredictionErrors(C_BaseEntity *pPlayer) = 0;
	virtual void	DiffPrint(char const *fmt, ...) = 0;

	virtual Vector const&	GetPlayerMins(bool ducked) const = 0;
	virtual Vector const&	GetPlayerMaxs(bool ducked) const = 0;
	virtual Vector const&   GetPlayerViewOffset(bool ducked) const = 0;

	virtual bool			IsMovingPlayerStuck(void) const = 0;
	virtual C_BaseEntity*	GetMovingPlayer(void) const = 0;
	virtual void			UnblockPusher(C_BaseEntity* pPlayer, C_BaseEntity *pPusher) = 0;

	virtual void SetupMovementBounds(CMoveData *pMove) = 0;
};



class IMoveHelper {
private:
	virtual void UnknownVirtual() = 0;
public:
	virtual void SetHost(C_BaseEntity* host) = 0;
};


enum ButtonCode_t
{
	BUTTON_CODE_INVALID = -1,
	BUTTON_CODE_NONE = 0,

	KEY_FIRST = 0,

	KEY_NONE = KEY_FIRST,
	KEY_0,
	KEY_1,
	KEY_2,
	KEY_3,
	KEY_4,
	KEY_5,
	KEY_6,
	KEY_7,
	KEY_8,
	KEY_9,
	KEY_A,
	KEY_B,
	KEY_C,
	KEY_D,
	KEY_E,
	KEY_F,
	KEY_G,
	KEY_H,
	KEY_I,
	KEY_J,
	KEY_K,
	KEY_L,
	KEY_M,
	KEY_N,
	KEY_O,
	KEY_P,
	KEY_Q,
	KEY_R,
	KEY_S,
	KEY_T,
	KEY_U,
	KEY_V,
	KEY_W,
	KEY_X,
	KEY_Y,
	KEY_Z,
	KEY_PAD_0,
	KEY_PAD_1,
	KEY_PAD_2,
	KEY_PAD_3,
	KEY_PAD_4,
	KEY_PAD_5,
	KEY_PAD_6,
	KEY_PAD_7,
	KEY_PAD_8,
	KEY_PAD_9,
	KEY_PAD_DIVIDE,
	KEY_PAD_MULTIPLY,
	KEY_PAD_MINUS,
	KEY_PAD_PLUS,
	KEY_PAD_ENTER,
	KEY_PAD_DECIMAL,
	KEY_LBRACKET,
	KEY_RBRACKET,
	KEY_SEMICOLON,
	KEY_APOSTROPHE,
	KEY_BACKQUOTE,
	KEY_COMMA,
	KEY_PERIOD,
	KEY_SLASH,
	KEY_BACKSLASH,
	KEY_MINUS,
	KEY_EQUAL,
	KEY_ENTER,
	KEY_SPACE,
	KEY_BACKSPACE,
	KEY_TAB,
	KEY_CAPSLOCK,
	KEY_NUMLOCK,
	KEY_ESCAPE,
	KEY_SCROLLLOCK,
	KEY_INSERT,
	KEY_DELETE,
	KEY_HOME,
	KEY_END,
	KEY_PAGEUP,
	KEY_PAGEDOWN,
	KEY_BREAK,
	KEY_LSHIFT,
	KEY_RSHIFT,
	KEY_LALT,
	KEY_RALT,
	KEY_LCONTROL,
	KEY_RCONTROL,
	KEY_LWIN,
	KEY_RWIN,
	KEY_APP,
	KEY_UP,
	KEY_LEFT,
	KEY_DOWN,
	KEY_RIGHT,
	KEY_F1,
	KEY_F2,
	KEY_F3,
	KEY_F4,
	KEY_F5,
	KEY_F6,
	KEY_F7,
	KEY_F8,
	KEY_F9,
	KEY_F10,
	KEY_F11,
	KEY_F12,
	KEY_CAPSLOCKTOGGLE,
	KEY_NUMLOCKTOGGLE,
	KEY_SCROLLLOCKTOGGLE,

	KEY_LAST = KEY_SCROLLLOCKTOGGLE,
	KEY_COUNT = KEY_LAST - KEY_FIRST + 1,

	// Mouse
	MOUSE_FIRST = KEY_LAST + 1,

	MOUSE_LEFT = MOUSE_FIRST,
	MOUSE_RIGHT,
	MOUSE_MIDDLE,
	MOUSE_4,
	MOUSE_5,
	MOUSE_WHEEL_UP,		// A fake button which is 'pressed' and 'released' when the wheel is moved up 
	MOUSE_WHEEL_DOWN,	// A fake button which is 'pressed' and 'released' when the wheel is moved down

	MOUSE_LAST = MOUSE_WHEEL_DOWN,
	MOUSE_COUNT = MOUSE_LAST - MOUSE_FIRST + 1,
};

class IInputSystem
{
public:
	void EnableInput(bool bEnable);
	void ResetInputState();
	bool IsButtonDown(ButtonCode_t code);
	void GetCursorPosition(int* m_pX, int* m_pY);
	ButtonCode_t VirtualKeyToButtonCode(int nVirtualKey);
	int ButtonCodeToVirtualKey(ButtonCode_t code);
};

inline void IInputSystem::EnableInput(bool bEnable)
{
	typedef void(__thiscall* OriginalFn)(void*, bool);
	return call_vfunc<OriginalFn>(this, 11)(this, bEnable);
}

inline void IInputSystem::ResetInputState()
{
	typedef void(__thiscall* OriginalFn)(void*);
	return call_vfunc<OriginalFn>(this, 39)(this);
}

inline bool IInputSystem::IsButtonDown(ButtonCode_t code)
{
	typedef bool(__thiscall* OriginalFn)(void*, ButtonCode_t);
	return call_vfunc<OriginalFn>(this, 15)(this, code);
}

inline void IInputSystem::GetCursorPosition(int* m_pX, int* m_pY)
{
	typedef void(__thiscall* OriginalFn)(void*, int*, int*);
	return call_vfunc<OriginalFn>(this, 56)(this, m_pX, m_pY);
}

inline ButtonCode_t IInputSystem::VirtualKeyToButtonCode(int nVirtualKey)
{
	typedef ButtonCode_t(__thiscall* OriginalFn)(void*, int);
	return call_vfunc<OriginalFn>(this, 44)(this, nVirtualKey);
}

inline int IInputSystem::ButtonCodeToVirtualKey(ButtonCode_t code)
{
	typedef int(__thiscall* OriginalFn)(void*, ButtonCode_t);
	return call_vfunc<OriginalFn>(this, 45)(this, code);
}


#define PRINTF_FORMAT_STRING _Printf_format_string_

class ConVar
{
public:
	Color GetColor()
	{
		using OriginalFn = Color(__thiscall *)(void *);
		return call_vfunc<OriginalFn>(this, 10)(this);
	}

	const char* GetString()
	{
		using OriginalFn = const char *(__thiscall *)(void *);
		return call_vfunc<OriginalFn>(this, 11)(this);
	}

	float GetFloat()
	{
		using OriginalFn = float(__thiscall *)(void *);
		return call_vfunc<OriginalFn>(this, 12)(this);
	}

	int GetInt()
	{
		using OriginalFn = int(__thiscall *)(void *);
		return call_vfunc<OriginalFn>(this, 13)(this);
	}

	void SetValue(const char *value)
	{
		using OriginalFn = void(__thiscall *)(void *, const char *);
		call_vfunc<OriginalFn>(this, 14)(this, value);
	}

	void SetValue(float value)
	{
		using OriginalFn = void(__thiscall *)(void *, float);
		call_vfunc<OriginalFn>(this, 15)(this, value);
	}

	void SetValue(int value)
	{
		using OriginalFn = void(__thiscall *)(void *, int);
		call_vfunc<OriginalFn>(this, 16)(this, value);
	}

	void SetValue(Color value)
	{
		using OriginalFn = void(__thiscall *)(void *, Color);
		call_vfunc<OriginalFn>(this, 17)(this, value);
	}

	char pad_0x0000[0x4]; //0x0000
	ConVar *pNext; //0x0004
	int32_t bRegistered; //0x0008
	char *pszName; //0x000C
	char *pszHelpString; //0x0010
	int32_t nFlags; //0x0014
	char pad_0x0018[0x4]; //0x0018
	ConVar *pParent; //0x001C
	char *pszDefaultValue; //0x0020
	char *strString; //0x0024
	int32_t StringLength; //0x0028
	float fValue; //0x002C
	int32_t nValue; //0x0030
	int32_t bHasMin; //0x0034
	float fMinVal; //0x0038
	int32_t bHasMax; //0x003C
	float fMaxVal; //0x0040
	void *fnChangeCallback; //0x0044
};

class ICVar {
public:
	virtual void Func0();
	virtual void Func1();
	virtual void Func2();
	virtual void Func3();
	virtual void Func4();
	virtual void Func5();
	virtual void Func6();
	virtual void Func7();
	virtual void Func8();
	virtual void Func9();
	virtual void			RegisterConCommand(ConVar *pCommandBase) = 0;
	virtual void			UnregisterConCommand(ConVar *pCommandBase) = 0;
	virtual void Func12();
	virtual void Func13();
	virtual void Func14();
	virtual void Func15();
	virtual ConVar* FindVar(const char* getVar);
	virtual void Func17();
	virtual void Func18();
	virtual void Func19();
	virtual void Func20();
	virtual void Func21();
	virtual void Func22();
	virtual void Func23();
	virtual void Func24();
	virtual void ConsoleColorPrintf(const Color& clr, const char *format, ...);
	virtual void ConsolePrintf(const char *format, ...);
	virtual void ConsoleDPrintf(PRINTF_FORMAT_STRING const char *pFormat, ...);
	virtual void Func28();
	virtual void Func29();
	virtual void Func30();
	virtual void Func31();
	virtual void Func32();
	virtual void Func33();
	virtual void Func34();
	virtual void Func35();
	virtual void Func36();
	virtual void Func37();
};
struct CServerConfirmedReservationCheckCallback
{
	char pad[0x2200];
};
typedef ULONG CRC32_t;


#define LittleLong( val )( val )

static const CRC32_t pulCRCTable[256] =
{
	0x00000000, 0x77073096, 0xee0e612c, 0x990951ba,
	0x076dc419, 0x706af48f, 0xe963a535, 0x9e6495a3,
	0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988,
	0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91,
	0x1db71064, 0x6ab020f2, 0xf3b97148, 0x84be41de,
	0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7,
	0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec,
	0x14015c4f, 0x63066cd9, 0xfa0f3d63, 0x8d080df5,
	0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172,
	0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b,
	0x35b5a8fa, 0x42b2986c, 0xdbbbc9d6, 0xacbcf940,
	0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59,
	0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116,
	0x21b4f4b5, 0x56b3c423, 0xcfba9599, 0xb8bda50f,
	0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924,
	0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d,
	0x76dc4190, 0x01db7106, 0x98d220bc, 0xefd5102a,
	0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433,
	0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818,
	0x7f6a0dbb, 0x086d3d2d, 0x91646c97, 0xe6635c01,
	0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e,
	0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457,
	0x65b0d9c6, 0x12b7e950, 0x8bbeb8ea, 0xfcb9887c,
	0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65,
	0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2,
	0x4adfa541, 0x3dd895d7, 0xa4d1c46d, 0xd3d6f4fb,
	0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0,
	0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9,
	0x5005713c, 0x270241aa, 0xbe0b1010, 0xc90c2086,
	0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f,
	0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4,
	0x59b33d17, 0x2eb40d81, 0xb7bd5c3b, 0xc0ba6cad,
	0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a,
	0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683,
	0xe3630b12, 0x94643b84, 0x0d6d6a3e, 0x7a6a5aa8,
	0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1,
	0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe,
	0xf762575d, 0x806567cb, 0x196c3671, 0x6e6b06e7,
	0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc,
	0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5,
	0xd6d6a3e8, 0xa1d1937e, 0x38d8c2c4, 0x4fdff252,
	0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b,
	0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60,
	0xdf60efc3, 0xa867df55, 0x316e8eef, 0x4669be79,
	0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236,
	0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f,
	0xc5ba3bbe, 0xb2bd0b28, 0x2bb45a92, 0x5cb36a04,
	0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d,
	0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a,
	0x9c0906a9, 0xeb0e363f, 0x72076785, 0x05005713,
	0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38,
	0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21,
	0x86d3d2d4, 0xf1d4e242, 0x68ddb3f8, 0x1fda836e,
	0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777,
	0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c,
	0x8f659eff, 0xf862ae69, 0x616bffd3, 0x166ccf45,
	0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2,
	0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db,
	0xaed16a4a, 0xd9d65adc, 0x40df0b66, 0x37d83bf0,
	0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9,
	0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6,
	0xbad03605, 0xcdd70693, 0x54de5729, 0x23d967bf,
	0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94,
	0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d
};

inline void CRC32_Init(CRC32_t *pulCRC)
{
	*pulCRC = 0xFFFFFFFFUL;
}

inline void CRC32_Final(CRC32_t *pulCRC)
{
	*pulCRC ^= 0xFFFFFFFFUL;
}

inline void CRC32_ProcessBuffer(CRC32_t *pulCRC, const void *pBuffer, int nBuffer)
{
	CRC32_t ulCrc = *pulCRC;
	unsigned char *pb = (unsigned char *)pBuffer;
	unsigned int nFront;
	int nMain;

JustAfew:
	switch (nBuffer)
	{
	case 7:
		ulCrc = pulCRCTable[*pb++ ^ (unsigned char)ulCrc] ^ (ulCrc >> 8);

	case 6:
		ulCrc = pulCRCTable[*pb++ ^ (unsigned char)ulCrc] ^ (ulCrc >> 8);

	case 5:
		ulCrc = pulCRCTable[*pb++ ^ (unsigned char)ulCrc] ^ (ulCrc >> 8);

	case 4:
		ulCrc ^= LittleLong(*(CRC32_t *)pb);
		ulCrc = pulCRCTable[(unsigned char)ulCrc] ^ (ulCrc >> 8);
		ulCrc = pulCRCTable[(unsigned char)ulCrc] ^ (ulCrc >> 8);
		ulCrc = pulCRCTable[(unsigned char)ulCrc] ^ (ulCrc >> 8);
		ulCrc = pulCRCTable[(unsigned char)ulCrc] ^ (ulCrc >> 8);
		*pulCRC = ulCrc;
		return;

	case 3:
		ulCrc = pulCRCTable[*pb++ ^ (unsigned char)ulCrc] ^ (ulCrc >> 8);

	case 2:
		ulCrc = pulCRCTable[*pb++ ^ (unsigned char)ulCrc] ^ (ulCrc >> 8);

	case 1:
		ulCrc = pulCRCTable[*pb++ ^ (unsigned char)ulCrc] ^ (ulCrc >> 8);

	case 0:
		*pulCRC = ulCrc;
		return;
	}

	nFront = ((unsigned int)pb) & 3;
	nBuffer -= nFront;

	switch (nFront)
	{
	case 3:
		ulCrc = pulCRCTable[*pb++ ^ (unsigned char)ulCrc] ^ (ulCrc >> 8);
	case 2:
		ulCrc = pulCRCTable[*pb++ ^ (unsigned char)ulCrc] ^ (ulCrc >> 8);
	case 1:
		ulCrc = pulCRCTable[*pb++ ^ (unsigned char)ulCrc] ^ (ulCrc >> 8);
	}

	nMain = nBuffer >> 3;

	while (nMain--)
	{
		ulCrc ^= LittleLong(*(CRC32_t *)pb);
		ulCrc = pulCRCTable[(unsigned char)ulCrc] ^ (ulCrc >> 8);
		ulCrc = pulCRCTable[(unsigned char)ulCrc] ^ (ulCrc >> 8);
		ulCrc = pulCRCTable[(unsigned char)ulCrc] ^ (ulCrc >> 8);
		ulCrc = pulCRCTable[(unsigned char)ulCrc] ^ (ulCrc >> 8);
		ulCrc ^= LittleLong(*(CRC32_t *)(pb + 4));
		ulCrc = pulCRCTable[(unsigned char)ulCrc] ^ (ulCrc >> 8);
		ulCrc = pulCRCTable[(unsigned char)ulCrc] ^ (ulCrc >> 8);
		ulCrc = pulCRCTable[(unsigned char)ulCrc] ^ (ulCrc >> 8);
		ulCrc = pulCRCTable[(unsigned char)ulCrc] ^ (ulCrc >> 8);
		pb += 8;
	}

	nBuffer &= 7;

	goto JustAfew;
}
inline CRC32_t CRC32_ProcessSingleBuffer(const void *p, int len)
{
	CRC32_t crc;

	CRC32_Init(&crc);
	CRC32_ProcessBuffer(&crc, p, len);
	CRC32_Final(&crc);

	return crc;
}


class CInput
{
public:
	class CUserCmd
	{
	public:
		CRC32_t GetChecksum() const
		{
			CRC32_t crc;
			CRC32_Init(&crc);
			CRC32_ProcessBuffer(&crc, &command_number, sizeof(command_number));
			CRC32_ProcessBuffer(&crc, &tick_count, sizeof(tick_count));
			CRC32_ProcessBuffer(&crc, &viewangles, sizeof(viewangles));
			CRC32_ProcessBuffer(&crc, &aimdirection, sizeof(aimdirection));
			CRC32_ProcessBuffer(&crc, &forwardmove, sizeof(forwardmove));
			CRC32_ProcessBuffer(&crc, &sidemove, sizeof(sidemove));
			CRC32_ProcessBuffer(&crc, &upmove, sizeof(upmove));
			CRC32_ProcessBuffer(&crc, &buttons, sizeof(buttons));
			CRC32_ProcessBuffer(&crc, &impulse, sizeof(impulse));
			CRC32_ProcessBuffer(&crc, &weaponselect, sizeof(weaponselect));
			CRC32_ProcessBuffer(&crc, &weaponsubtype, sizeof(weaponsubtype));
			CRC32_ProcessBuffer(&crc, &random_seed, sizeof(random_seed));
			CRC32_ProcessBuffer(&crc, &mousedx, sizeof(mousedx));
			CRC32_ProcessBuffer(&crc, &mousedy, sizeof(mousedy));
			CRC32_Final(&crc);
			return crc;
		}

		BYTE u1[4];
		int command_number;
		int	tick_count;
		QAngle viewangles;
		Vector aimdirection;
		float forwardmove;
		float sidemove;
		float upmove;
		int buttons;
		BYTE impulse;
		int weaponselect;
		int weaponsubtype;
		int random_seed;
		short mousedx;
		short mousedy;
		bool hasbeenpredicted;
		Vector headangles;
		Vector headoffset;
	};

	class CVerifiedUserCmd
	{
	public:
		CUserCmd m_cmd;
		unsigned long m_crc;
	};

	CInput::CUserCmd *GetUserCmd(int slot, int seq)
	{
		using OriginalFn = CUserCmd *(__thiscall *)(void *, int, int);
		return GetVirtualFunction<OriginalFn>(this, 8)(this, slot, seq);
	}


	CUserCmd* GetUserCmd2(int sequence_number)
	{
		return &m_pCommands[sequence_number % 150];
	}

	CInput::CUserCmd* GetUserCmdz(signed int sequence_number)
	{
		int result; // eax@3

		result = *(DWORD*)&m_pCommands[sequence_number % 150];
		if (*(DWORD *)(result + 4) != sequence_number)
			result = 0;
		return (CInput::CUserCmd*)result;
	}




public:
	void*               pvftable;                     //0x00
	bool                m_fTrackIRAvailable;          //0x04
	bool                m_fMouseInitialized;          //0x05
	bool                m_fMouseActive;               //0x06
	bool                m_fJoystickAdvancedInit;      //0x07
	char                pad_0x08[0x2C];               //0x08
	void*               m_pKeys;                      //0x34
	char                pad_0x38[0x64];               //0x38
	int					pad_0x41;
	int					pad_0x42;
	bool                m_fCameraInterceptingMouse;   //0x9C
	bool                m_fCameraInThirdPerson;       //0x9D
	bool                m_fCameraMovingWithMouse;     //0x9E
	Vector				m_vecCameraOffset;            //0xA0
	bool                m_fCameraDistanceMove;        //0xAC
	int                 m_nCameraOldX;                //0xB0
	int                 m_nCameraOldY;                //0xB4
	int                 m_nCameraX;                   //0xB8
	int                 m_nCameraY;                   //0xBC
	bool                m_CameraIsOrthographic;       //0xC0
	Vector              m_angPreviousViewAngles;      //0xC4
	Vector              m_angPreviousViewAnglesTilt;  //0xD0
	float               m_flLastForwardMove;          //0xDC
	int                 m_nClearInputState;           //0xE0
	char                pad_0xE4[0x8];                //0xE4
	CInput::CUserCmd*           m_pCommands;                  //0xEC
	CVerifiedUserCmd*   m_pVerifiedCommands;          //0xF0

};


class CGlowObjectManager
{
public:

	int RegisterGlowObject(C_BaseEntity *pEntity, const Vector &vGlowColor, float flGlowAlpha, bool bRenderWhenOccluded, bool bRenderWhenUnoccluded, int nSplitScreenSlot)
	{
		int nIndex;
		if (m_nFirstFreeSlot == GlowObjectDefinition_t::END_OF_FREE_LIST)
		{
			nIndex = m_GlowObjectDefinitions.AddToTail();
		}
		else
		{
			nIndex = m_nFirstFreeSlot;
			m_nFirstFreeSlot = m_GlowObjectDefinitions[nIndex].m_nNextFreeSlot;
		}

		m_GlowObjectDefinitions[nIndex].m_hEntity = pEntity;
		m_GlowObjectDefinitions[nIndex].m_vGlowColor = vGlowColor;
		m_GlowObjectDefinitions[nIndex].m_flGlowAlpha = flGlowAlpha;
		m_GlowObjectDefinitions[nIndex].flUnk = 0.0f;
		m_GlowObjectDefinitions[nIndex].m_flBloomAmount = 1.0f;
		m_GlowObjectDefinitions[nIndex].localplayeriszeropoint3 = 0.0f;
		m_GlowObjectDefinitions[nIndex].m_bRenderWhenOccluded = bRenderWhenOccluded;
		m_GlowObjectDefinitions[nIndex].m_bRenderWhenUnoccluded = bRenderWhenUnoccluded;
		m_GlowObjectDefinitions[nIndex].m_bFullBloomRender = false;
		m_GlowObjectDefinitions[nIndex].m_nFullBloomStencilTestValue = 0;
		m_GlowObjectDefinitions[nIndex].m_nSplitScreenSlot = nSplitScreenSlot;
		m_GlowObjectDefinitions[nIndex].m_nNextFreeSlot = GlowObjectDefinition_t::ENTRY_IN_USE;

		return nIndex;
	}

	void UnregisterGlowObject(int nGlowObjectHandle)
	{
		Assert(!m_GlowObjectDefinitions[nGlowObjectHandle].IsUnused());

		m_GlowObjectDefinitions[nGlowObjectHandle].m_nNextFreeSlot = m_nFirstFreeSlot;
		m_GlowObjectDefinitions[nGlowObjectHandle].m_hEntity = NULL;
		m_nFirstFreeSlot = nGlowObjectHandle;
	}

	int HasGlowEffect(C_BaseEntity *pEntity) const
	{
		for (int i = 0; i < m_GlowObjectDefinitions.Count(); ++i)
		{
			if (!m_GlowObjectDefinitions[i].IsUnused() && m_GlowObjectDefinitions[i].m_hEntity == pEntity)
			{
				return i;
			}
		}

		return NULL;
	}

	class GlowObjectDefinition_t
	{
	public:
		void set(float r, float g, float b, float a)
		{
			m_vGlowColor = Vector(r, g, b);
			m_flGlowAlpha = a;
			m_bRenderWhenOccluded = true;
			m_bRenderWhenUnoccluded = false;
			m_flBloomAmount = 1.0f;
		}

		C_BaseEntity* getEnt()
		{
			return m_hEntity;
		}

		bool IsUnused() const { return m_nNextFreeSlot != GlowObjectDefinition_t::ENTRY_IN_USE; }

	public:
		C_BaseEntity*    m_hEntity;
		Vector            m_vGlowColor;
		float            m_flGlowAlpha;

		char            unknown[4]; //pad 
		float            flUnk; //confirmed to be treated as a float while reversing glow functions 
		float            m_flBloomAmount;
		float            localplayeriszeropoint3;


		bool            m_bRenderWhenOccluded;
		bool            m_bRenderWhenUnoccluded;
		bool            m_bFullBloomRender;
		char            unknown1[1]; //pad 


		int                m_nFullBloomStencilTestValue; // 0x28 
		int                iUnk; //appears like it needs to be zero  
		int                m_nSplitScreenSlot; //Should be -1 

											   // Linked list of free slots 
		int                m_nNextFreeSlot;

		// Special values for GlowObjectDefinition_t::m_nNextFreeSlot 
		static const int END_OF_FREE_LIST = -1;
		static const int ENTRY_IN_USE = -2;
	};

	CUtlVector< GlowObjectDefinition_t > m_GlowObjectDefinitions;
	int m_nFirstFreeSlot;
};

class SpoofedConvar {
public:
	SpoofedConvar();
	SpoofedConvar(const char* szCVar);
	SpoofedConvar(ConVar* pCVar, char* newname);

	~SpoofedConvar();

	bool           IsSpoofed();
	void           Spoof();

	void           SetFlags(int flags);
	int            GetFlags();

	void           SetBool(bool bValue);
	void           SetInt(int iValue);
	void           SetFloat(float flValue);
	void           SetString(const char* szValue);

private:
	ConVar* m_pOriginalCVar = NULL;
	ConVar* m_pDummyCVar = NULL;

	char m_szDummyName[128];
	char m_szDummyValue[128];
	char m_szOriginalName[128];
	char m_szOriginalValue[128];
	int m_iOriginalFlags;
	char* m_szReplacementName;
};

class MinspecCvar
{
public:
	MinspecCvar(const char* szCVar, char* newname, float newvalue);
	~MinspecCvar();

	bool ValidCvar();
	void Spoof();

	template<typename T>
	void SetValue(T value);

	int	GetInt();
	float GetFloat();
	const char* GetString();
private:
	ConVar* m_pConVar;

	char* m_szOriginalName;
	char* m_szReplacementName;
	float m_OriginalValue;
	float m_newvalue;
};

class IAppSystem
{
public:
	virtual void func0() = 0;
	virtual void func1() = 0;
	virtual void func2() = 0;
	virtual void func3() = 0;
	virtual void func4() = 0;
	virtual void func5() = 0;
	virtual void func6() = 0;
	virtual void func7() = 0;
	virtual void func8() = 0;
	virtual void func9() = 0;
};

typedef unsigned short MDLHandle_t;

enum
{
	MDLHANDLE_INVALID = (MDLHandle_t)~0
};

enum MDLCacheDataType_t
{
	// Callbacks to get called when data is loaded or unloaded for these:
	MDLCACHE_STUDIOHDR = 0,
	MDLCACHE_STUDIOHWDATA,
	MDLCACHE_VCOLLIDE,

	// Callbacks NOT called when data is loaded or unloaded for these:
	MDLCACHE_ANIMBLOCK,
	MDLCACHE_VIRTUALMODEL,
	MDLCACHE_VERTEXES,
	MDLCACHE_DECODEDANIMBLOCK,
};

class IMDLCacheNotify
{
public:
	// Called right after the data is loaded
	virtual void OnDataLoaded(MDLCacheDataType_t type, MDLHandle_t handle) = 0;

	// Called right before the data is unloaded
	virtual void OnDataUnloaded(MDLCacheDataType_t type, MDLHandle_t handle) = 0;
};

struct studiohdr_t;
struct studiohwdata_t;
struct vcollide_t;
struct virtualmodel_t;
struct vertexFileHeader_t;

enum MDLCacheFlush_t
{
	MDLCACHE_FLUSH_STUDIOHDR = 0x01,
	MDLCACHE_FLUSH_STUDIOHWDATA = 0x02,
	MDLCACHE_FLUSH_VCOLLIDE = 0x04,
	MDLCACHE_FLUSH_ANIMBLOCK = 0x08,
	MDLCACHE_FLUSH_VIRTUALMODEL = 0x10,
	MDLCACHE_FLUSH_AUTOPLAY = 0x20,
	MDLCACHE_FLUSH_VERTEXES = 0x40,

	MDLCACHE_FLUSH_IGNORELOCK = 0x80000000,
	MDLCACHE_FLUSH_ALL = 0xFFFFFFFF
};

class IMDLCache : public IAppSystem
{
public:
	// Used to install callbacks for when data is loaded + unloaded
	// Returns the prior notify
	virtual void SetCacheNotify(IMDLCacheNotify *pNotify) = 0;

	// NOTE: This assumes the "GAME" path if you don't use
	// the UNC method of specifying files. This will also increment
	// the reference count of the MDL
	virtual MDLHandle_t FindMDL(const char *pMDLRelativePath) = 0;

	// Reference counting
	virtual int AddRef(MDLHandle_t handle) = 0;
	virtual int Release(MDLHandle_t handle) = 0;
	virtual int GetRef(MDLHandle_t handle) = 0;

	// Gets at the various data associated with a MDL
	virtual studiohdr_t *GetStudioHdr(MDLHandle_t handle) = 0;
	virtual studiohwdata_t *GetHardwareData(MDLHandle_t handle) = 0;
	virtual vcollide_t *GetVCollide(MDLHandle_t handle) = 0;
	virtual unsigned char *GetAnimBlock(MDLHandle_t handle, int nBlock) = 0;
	virtual virtualmodel_t *GetVirtualModel(MDLHandle_t handle) = 0;
	virtual int GetAutoplayList(MDLHandle_t handle, unsigned short **pOut) = 0;
	virtual vertexFileHeader_t *GetVertexData(MDLHandle_t handle) = 0;

	// Brings all data associated with an MDL into memory
	virtual void TouchAllData(MDLHandle_t handle) = 0;

	// Gets/sets user data associated with the MDL
	virtual void SetUserData(MDLHandle_t handle, void* pData) = 0;
	virtual void *GetUserData(MDLHandle_t handle) = 0;

	// Is this MDL using the error model?
	virtual bool IsErrorModel(MDLHandle_t handle) = 0;

	// Flushes the cache, force a full discard
	virtual void Flush(MDLCacheFlush_t nFlushFlags = MDLCACHE_FLUSH_ALL) = 0;

	// Flushes a particular model out of memory
	virtual void Flush(MDLHandle_t handle, int nFlushFlags = MDLCACHE_FLUSH_ALL) = 0;

	// Returns the name of the model (its relative path)
	virtual const char *GetModelName(MDLHandle_t handle) = 0;

	// faster access when you already have the studiohdr
	virtual virtualmodel_t *GetVirtualModelFast(const studiohdr_t *pStudioHdr, MDLHandle_t handle) = 0;

	// all cache entries that subsequently allocated or successfully checked 
	// are considered "locked" and will not be freed when additional memory is needed
	virtual void BeginLock() = 0;

	// reset all protected blocks to normal
	virtual void EndLock() = 0;

	// returns a pointer to a counter that is incremented every time the cache has been out of the locked state (EVIL)
	virtual int *GetFrameUnlockCounterPtrOLD() = 0;

	// Finish all pending async operations
	virtual void FinishPendingLoads() = 0;

	virtual vcollide_t *GetVCollideEx(MDLHandle_t handle, bool synchronousLoad = true) = 0;
	virtual bool GetVCollideSize(MDLHandle_t handle, int *pVCollideSize) = 0;

	virtual bool GetAsyncLoad(MDLCacheDataType_t type) = 0;
	virtual bool SetAsyncLoad(MDLCacheDataType_t type, bool bAsync) = 0;

	virtual void BeginMapLoad() = 0;
	virtual void EndMapLoad() = 0;
	virtual void MarkAsLoaded(MDLHandle_t handle) = 0;

	virtual void InitPreloadData(bool rebuild) = 0;
	virtual void ShutdownPreloadData() = 0;

	virtual bool IsDataLoaded(MDLHandle_t handle, MDLCacheDataType_t type) = 0;

	virtual int *GetFrameUnlockCounterPtr(MDLCacheDataType_t type) = 0;

	virtual studiohdr_t *LockStudioHdr(MDLHandle_t handle) = 0;
	virtual void UnlockStudioHdr(MDLHandle_t handle) = 0;

	virtual bool PreloadModel(MDLHandle_t handle) = 0;

	// Hammer uses this. If a model has an error loading in GetStudioHdr, then it is flagged
	// as an error model and any further attempts to load it will just get the error model.
	// That is, until you call this function. Then it will load the correct model.
	virtual void ResetErrorModelStatus(MDLHandle_t handle) = 0;

	virtual void MarkFrame() = 0;
};

#pragma pack(push, 1)
class INetChannel
{
public:
	char pad_0000[20];           //0x0000
	bool m_bProcessingMessages;  //0x0014
	bool m_bShouldDelete;        //0x0015
	char pad_0016[2];            //0x0016
	int32_t m_nOutSequenceNr;    //0x0018 last send outgoing sequence number
	int32_t m_nInSequenceNr;     //0x001C last received incoming sequnec number
	int32_t m_nOutSequenceNrAck; //0x0020 last received acknowledge outgoing sequnce number
	int32_t m_nOutReliableState; //0x0024 state of outgoing reliable data (0/1) flip flop used for loss detection
	int32_t m_nInReliableState;  //0x0028 state of incoming reliable data
	int32_t m_nChokedPackets;    //0x002C number of choked packets
	char pad_0030[1044];         //0x0030
}; //Size: 0x0444

class CClockDriftMgr
{
public:
	float m_ClockOffsets[16];   //0x0000
	uint32_t m_iCurClockOffset; //0x0040
	int32_t m_nServerTick;      //0x0044
	int32_t m_nClientTick;      //0x0048
}; //Size: 0x004C

class CClientState
{
public:
	static void ForceFullUpdate();

	char pad_0000[148];              //0x0000
	INetChannel* m_NetChannel; //0x0094
	uint32_t m_nChallengeNr;         //0x0098
	double m_flConnectTime;          //0x009C
	int32_t m_nRetryNumber;          //0x00A4
	char pad_00A8[88];               //0x00A8
	int32_t m_nSignonState;          //0x0100
	char pad_0104[8];                //0x0104
	float m_flNextCmdTime;           //0x010C
	int32_t m_nServerCount;          //0x0110
	uint32_t m_nCurrentSequence;     //0x0114
	char pad_0118[8];                //0x0118
	CClockDriftMgr m_ClockDriftMgr;  //0x0120
	int32_t m_nDeltaTick;            //0x016C
	bool m_bPaused;                  //0x0170
	char pad_0171[3];                //0x0171
	int32_t m_nViewEntity;           //0x0174
	int32_t m_nPlayerSlot;           //0x0178
	char pad_017C[4];                //0x017C
	char m_szLevelName[260];         //0x0180
	char m_szLevelNameShort[40];     //0x0284
	char pad_02AC[92];               //0x02AC
	int32_t m_nMaxClients;           //0x0308
	char pad_030C[4083];             //0x030C
	uint32_t m_StringTableContainer; //0x12FF
	char pad_1303[14737];            //0x1303
	float m_flLastServerTickTime;    //0x4C94
	bool insimulation;               //0x4C98
	char pad_4C99[3];                //0x4C99
	uint32_t oldtickcount;           //0x4C9C
	float m_tickRemainder;           //0x4CA0
	float m_frameTime;               //0x4CA4
	int32_t lastoutgoingcommand;     //0x4CA8
	int32_t chokedcommands;          //0x4CAC
	int32_t last_command_ack;        //0x4CB0
	int32_t command_ack;             //0x4CB4
	int32_t m_nSoundSequence;        //0x4CB8
	char pad_4CBC[80];               //0x4CBC
	QAngle viewangles;               //0x4D0C
}; //Size: 0x4D18
#pragma pack(pop)
#include <winnt.h>
C_ASSERT(FIELD_OFFSET(CClientState, m_NetChannel) == 0x0094);
C_ASSERT(FIELD_OFFSET(CClientState, m_nSignonState) == 0x0100);
C_ASSERT(FIELD_OFFSET(CClientState, lastoutgoingcommand) == 0x4CA8);
C_ASSERT(FIELD_OFFSET(CClientState, viewangles) == 0x4D0C);

class IPrediction {
private:
	virtual void UnknownVirtual0() = 0;
	virtual void UnknownVirtual1() = 0;
	virtual void UnknownVirtual2() = 0;
	virtual void UnknownVirtual3() = 0;
	virtual void UnknownVirtual4() = 0;
	virtual void UnknownVirtual5() = 0;
	virtual void UnknownVirtual6() = 0;
	virtual void UnknownVirtual7() = 0;
	virtual void UnknownVirtual8() = 0;
	virtual void UnknownVirtual9() = 0;
	virtual void UnknownVirtual10() = 0;
	virtual void UnknownVirtual11() = 0;
	virtual void UnknownVirtual12() = 0;
	virtual void UnknownVirtual13() = 0;
	virtual void UnknownVirtual14() = 0;
	virtual void UnknownVirtual15() = 0;
	virtual void UnknownVirtual16() = 0;
	virtual void UnknownVirtual17() = 0;
	virtual void UnknownVirtual18() = 0;
	virtual void UnknownVirtual19() = 0;
public:
	virtual void SetupMove(C_BaseEntity* player, CInput::CUserCmd* ucmd, IMoveHelper* pHelper, CMoveData* move) = 0; //20
	virtual void FinishMove(C_BaseEntity* player, CInput::CUserCmd* ucmd, CMoveData* move) = 0;
};
class C_TEFireBullets
{
public:
    char    pad[12];
    int		m_iPlayer; //12
    int     m_iItemDefinitionIndex;
    Vector	m_vecOrigin;
    Vector	m_vecAngles;
    int		m_iWeaponID;
    int		m_iMode;
    int		m_iSeed;
    float	m_flSpread;
};
class CBaseViewport;
class CBaseHudWeaponSelection;
typedef unsigned long HCursor;
class CHudChat;
class IClientMode
{
public:
    virtual             ~IClientMode() {}
    virtual int         ClientModeCSNormal(void *) = 0;
    virtual void        Init() = 0;
    virtual void        InitViewport() = 0;
    virtual void        Shutdown() = 0;
    virtual void        Enable() = 0;
    virtual void        Disable() = 0;
    virtual void        Layout() = 0;
    virtual IPanel*     GetViewport() = 0;
    virtual void*       GetViewportAnimationController() = 0;
    virtual void        ProcessInput(bool bActive) = 0;
    virtual bool        ShouldDrawDetailObjects() = 0;
    virtual bool        ShouldDrawEntity(C_BaseEntity *pEnt) = 0;
    virtual bool        ShouldDrawLocalPlayer(C_BaseEntity *pPlayer) = 0;
    virtual bool        ShouldDrawParticles() = 0;
    virtual bool        ShouldDrawFog(void) = 0;
    virtual void        OverrideView(CViewSetup *pSetup) = 0;
    virtual int         KeyInput(int down, int keynum, const char *pszCurrentBinding) = 0;
    virtual void        StartMessageMode(int iMessageModeType) = 0;
    virtual IPanel*     GetMessagePanel() = 0;
    virtual void        OverrideMouseInput(float *x, float *y) = 0;
    virtual bool        CreateMove(float flInputSampleTime, void* usercmd) = 0;
    virtual void        LevelInit(const char *newmap) = 0;
    virtual void        LevelShutdown(void) = 0;
public:
    void* ptr1;
    void* ptr2;
    CBaseViewport			*m_pViewport;
    // Message mode handling
    // All modes share a common chat interface
    CHudChat			*m_pChatElement;
    HCursor			m_CursorNone;
    CBaseHudWeaponSelection *m_pWeaponSelection;
    int						m_nRootSize[2];
};