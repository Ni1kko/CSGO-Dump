#pragma once
#include <Windows.h>
#include "matrix.hpp"

struct mstudiobbox_t
{
	int		bone;				// 0x0000 
	int		group;				// 0x0004 
	Vector3	min;				// 0x0008 
	Vector3	max;				// 0x0014 
	int		nameidx;			// 0x0020 
	char	pad_0x0024[0xC];	// 0x0024
	float	radius;				// 0x0030 
	char	pad_0x0034[0x10];	// 0x0034
};
struct mstudiohitboxset_t
{
	int                                             sznameindex;
	inline char* const              getName(void) const { return ((char*)this) + sznameindex; }
	int                                             numhitboxes;
	int                                             hitboxindex;
	inline mstudiobbox_t*   getHitbox(int i) const { return (mstudiobbox_t*)(((BYTE*)this) + hitboxindex) + i; };
};

struct mstudiobone_t
{
	int                                     sznameindex;
	inline char * const getName(void) const { return ((char *)this) + sznameindex; }
	int                                     parent;
	int                                     bonecontroller[6];

	Vector3                          pos;
	float                           quat[4];
	Vector3                          rot;
	Vector3                          posscale;
	Vector3                          rotscale;

	matrix3x4_t                     poseToBone;
	float                           qAlignment[4];
	int                                     flags;
	int                                     proctype;
	int                                     procindex;              // procedural rule
	mutable int                     physicsbone;    // index into physically simulated bone
	inline void *           getProcedure() const { if (procindex == 0) return NULL; else return  (void *)(((BYTE *)this) + procindex); };
	int                                     surfacepropidx; // index into string tablefor property name
	inline char * const getSurfaceProps(void) const { return ((char *)this) + surfacepropidx; }
	int                                     contents;               // See BSPFlags.h for the contents flags

	int                                     unused[8];              // remove as appropriate
};

struct studiohdr_t
{
	int                                     id;
	int                                     version;

	int                                     checksum;

	char                            name[64];
	int                                     length;


	Vector3                          eyeposition;

	Vector3                          illumposition;

	Vector3                          hull_min;
	Vector3                          hull_max;

	Vector3                          view_bbmin;
	Vector3                          view_bbmax;

	int                                     flags;

	int                                     numbones;
	int                                     boneindex;

	inline mstudiobone_t *getBone(int i) const { return (mstudiobone_t *)(((BYTE *)this) + boneindex) + i; };
	//	inline mstudiobone_t *pBone(int i) const { Assert(i >= 0 && i < numbones); return (mstudiobone_t *)(((byte *)this) + boneindex) + i; };

	int                                     numbonecontrollers;
	int                                     bonecontrollerindex;

	int                                     numhitboxsets;
	int                                     hitboxsetindex;

	mstudiohitboxset_t* getHitboxSet(int i) const
	{
		return (mstudiohitboxset_t*)(((BYTE*)this) + hitboxsetindex) + i;
	}

	inline mstudiobbox_t* getHitbox(int i, int set) const
	{
		mstudiohitboxset_t const* s = getHitboxSet(set);

		if (!s)
			return NULL;

		return s->getHitbox(i);
	}

	inline int getHitboxCount(int set) const
	{
		mstudiohitboxset_t const* s = getHitboxSet(set);

		if (!s)
			return 0;

		return s->numhitboxes;
	}

	int                                     numlocalanim;
	int                                     localanimindex;

	int                                     numlocalseq;
	int                                     localseqindex;

	mutable int                     activitylistversion;
	mutable int                     eventsindexed;

	int                                     numtextures;
	int                                     textureindex;

	int                                     numcdtextures;
	int                                     cdtextureindex;

	int                                     numskinref;
	int                                     numskinfamilies;
	int                                     skinindex;

	int                                     numbodyparts;
	int                                     bodypartindex;

	int                                     numlocalattachments;
	int                                     localattachmentindex;

	int                                     numlocalnodes;
	int                                     localnodeindex;
	int                                     localnodenameindex;

	int                                     numflexdesc;
	int                                     flexdescindex;

	int                                     numflexcontrollers;
	int                                     flexcontrollerindex;

	int                                     numflexrules;
	int                                     flexruleindex;

	int                                     numikchains;
	int                                     ikchainindex;

	int                                     nummouths;
	int                                     mouthindex;

	int                                     numlocalposeparameters;
	int                                     localposeparamindex;

	int                                     surfacepropindex;

	int                                     keyvalueindex;
	int                                     keyvaluesize;


	int                                     numlocalikautoplaylocks;
	int                                     localikautoplaylockindex;

	float                           mass;
	int                                     contents;

	int                                     numincludemodels;
	int                                     includemodelindex;

	mutable void            *virtualModel;

	int                                     szanimblocknameindex;
	int                                     numanimblocks;
	int                                     animblockindex;

	mutable void            *animblockModel;

	int                                     bonetablebynameindex;

	void                            *pVertexBase;
	void                            *pIndexBase;

	BYTE                            constdirectionallightdot;

	BYTE                            rootLOD;

	BYTE                            numAllowedRootLODs;

	BYTE                            unused[1];

	int                                     unused4;

	int                                     numflexcontrollerui;
	int                                     flexcontrolleruiindex;
	float                           flVertAnimFixedPointScale;
	int                                     unused3[1];
	int                                     studiohdr2index;
	int                                     unused2[1];
};

struct model_t;

class IVModelInfo // gucci
{
public:
	inline void* getModel(int Index) {
		using Fn = void*(__thiscall*)(IVModelInfo*, int);
		return (*(Fn**)this)[1](this, Index);
	}
	int	getModelIndex(const char *name)
	{
		using Fn = int(__thiscall*)(IVModelInfo*, const char*);
		return (*(Fn**)this)[2](this, name);
	}
	const char *getModelName(const model_t *mod)
	{
		using Fn = char*(__thiscall*)(IVModelInfo*, const model_t *);
		return (*(Fn**)this)[3](this, mod);
	}
	studiohdr_t	*getStudiomodel(const model_t *mod)
	{
		using Fn = studiohdr_t*(__thiscall*)(IVModelInfo*, const model_t *);
		return (*(Fn**)this)[32](this, mod);
	}
};