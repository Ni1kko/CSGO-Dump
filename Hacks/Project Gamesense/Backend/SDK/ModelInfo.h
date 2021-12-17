#pragma once
#include "IMaterial.h"
#include "vector2d.h"
#include "quaternion.h"
#include "vmatrix.h"

struct model_t;
struct mstudioanimdesc_t {};
struct mstudioseqdesc_t {};

enum HitboxList
{
	HITBOX_HEAD, // ебало
	HITBOX_NECK, // шейка
	HITBOX_PELVIS, // жопа
	HITBOX_BODY,  // животик
	HITBOX_THORAX, // ключица
	HITBOX_CHEST, // грудь
	HITBOX_UPPER_CHEST,  // сиськи
	HITBOX_RIGHT_THIGH, // правая ляжка
	HITBOX_LEFT_THIGH, // левая ляжка
	HITBOX_RIGHT_CALF, // правая икра
	HITBOX_LEFT_CALF, // левая икра
	HITBOX_RIGHT_FOOT,  // правая стопа
	HITBOX_LEFT_FOOT, // левая стопа
	HITBOX_RIGHT_HAND, // правая ладонь
	HITBOX_LEFT_HAND,  // левая ладонь
	HITBOX_RIGHT_UPPER_ARM, // правое плечо
	HITBOX_RIGHT_FOREARM, // правое предплечье
	HITBOX_LEFT_UPPER_ARM,  // левое плечо
	HITBOX_LEFT_FOREARM, // левое предплечье
	HITBOX_MAX
};

struct mstudiobbox_t
{
	int					bone;
	int					group;				// intersection group
	Vector				bbmin;				// bounding box
	Vector				bbmax;
	int					szhitboxnameindex;	// offset to the name of the hitbox.
	int					unused[3];
	float				radius;
	int					unused2[4];

	const char* pszHitboxName()
	{
		if (szhitboxnameindex == 0)
			return xorstr_("");

		return ((char*)this) + szhitboxnameindex;
	}

	mstudiobbox_t() {}

private:
	mstudiobbox_t(const mstudiobbox_t &vOther);
};

struct mstudiobone_t
{
	int					sznameindex;
	inline char* const pszName(void) const
	{
		return ((char*)this) + sznameindex;
	}
	int		 			parent;		// parent bone
	int					bonecontroller[6];	// bone controller index, -1 == none

	// default values
	Vector				pos;
	Quaternion			quat;
	RadianEuler			rot;
	// compression scale
	Vector				posscale;
	Vector				rotscale;

	matrix3x4_t			poseToBone;
	Quaternion			qAlignment;
	int					flags;
	int					proctype;
	int					procindex;		// procedural rule
	mutable int			physicsbone;	// index into physically simulated bone
	inline void* pProcedure() const
	{
		if (procindex == 0)
			return NULL;

		else
			return (void*)(((byte*)this) + procindex);
	};
	int					surfacepropidx;	// index into string tablefor property name
	inline char* const pszSurfaceProp(void) const
	{
		return ((char*)this) + surfacepropidx;
	}
	int					contents;		// See BSPFlags.h for the contents flags

	int					unused[8];		// remove as appropriate

	mstudiobone_t() {}
private:
	// No copy constructors allowed
	mstudiobone_t(const mstudiobone_t &vOther);
};

struct mstudiohitboxset_t
{
	int	sznameindex;
	int	numhitboxes;
	int	hitboxindex;

	inline mstudiobbox_t* GetHitbox(int i) const
	{
		return (mstudiobbox_t*)(((unsigned char*)this) + hitboxindex) + i;
	};
};

struct studiohdr_t
{
	int	id;
	int	version;

	long checksum;	// this has to be the same in the phy and vtx files to load!

	char name[64];
	int	length;

	Vector	eyeposition;	// ideal eye position

	Vector	illumposition;	// illumination center

	Vector	hull_min;	// ideal movement hull size
	Vector	hull_max;

	Vector	view_bbmin;	// clipping bounding box
	Vector	view_bbmax;

	int	flags;

	int	numbones;	// bones
	int	boneindex;
	inline mstudiobone_t* pBone(int i) const
	{
		return (mstudiobone_t*)(((byte*)this) + boneindex) + i;
	};

	int	numbonecontrollers;	// bone controllers
	int	bonecontrollerindex;
	inline void* pBonecontroller(int i) const
	{
		return (((byte*)this) + bonecontrollerindex) + i;
	};

	int	numhitboxsets;
	int	hitboxsetindex;

	// Look up hitbox set by index
	mstudiohitboxset_t*  pHitboxSet(int i) const
	{
		return (mstudiohitboxset_t*)(((byte*)this) + hitboxsetindex) + i;
	};

	// Calls through to hitbox to determine size of specified set
	inline mstudiobbox_t* pHitbox(int i, int set) const
	{
		mstudiohitboxset_t* s = pHitboxSet(set);

		if (!s)
			return NULL;

		return s->GetHitbox(i);
	};

	// Calls through to set to get hitbox count for set
	inline int  iHitboxCount(int set) const
	{
		mstudiohitboxset_t const* s = pHitboxSet(set);

		if (!s)
			return 0;

		return s->numhitboxes;
	};

	int	numanim;	// animations/poses
	int	animdescindex;	// animation descriptions
	inline void* pAnimdesc(int i) const
	{
		return (((byte*)this) + animdescindex) + i;
	};

	int 	numanimgroup;
	int 	animgroupindex;
	inline  void* pAnimGroup(int i) const
	{
		return (((byte*)this) + animgroupindex) + i;
	};

	int 	numbonedesc;
	int 	bonedescindex;
	inline  void* pBoneDesc(int i) const
	{
		return (((byte*)this) + bonedescindex) + i;
	};

	int	numseq;		// sequences
	int	seqindex;
	inline mstudioseqdesc_t* pSeqdesc(int i) const
	{
		if (i < 0 || i >= numseq)
			i = 0;

		return (mstudioseqdesc_t*)(((byte*)this) + seqindex) + i;
	};
	int	sequencesindexed;	// initialization flag - have the sequences been indexed?

	int	numseqgroups;		// demand loaded sequences
	int	seqgroupindex;
	inline  void* pSeqgroup(int i) const
	{
		return (((byte*)this) + seqgroupindex) + i;
	};

	int	numtextures;		// raw textures
	int	textureindex;
	inline void* pTexture(int i) const
	{
		return (((byte*)this) + textureindex) + i;
	};

	int	numcdtextures;		// raw textures search paths
	int	cdtextureindex;
	inline char*			pCdtexture(int i) const
	{
		return (((char*)this) + *((int*)(((byte*)this) + cdtextureindex) + i));
	};

	int	numskinref;		// replaceable textures tables
	int	numskinfamilies;
	int	skinindex;
	inline short*		pSkinref(int i) const
	{
		return (short*)(((byte*)this) + skinindex) + i;
	};

	int	numbodyparts;
	int	bodypartindex;
	inline void*	pBodypart(int i) const
	{
		return (((byte*)this) + bodypartindex) + i;
	};

	int	numattachments;		// queryable attachable points
	int	attachmentindex;
	inline void*	pAttachment(int i) const
	{
		return (((byte*)this) + attachmentindex) + i;
	};

	int	numtransitions;		// animation node to animation node transition graph
	int	transitionindex;
	inline byte*	pTransition(int i) const
	{
		return (byte*)(((byte*)this) + transitionindex) + i;
	};

	int	numflexdesc;
	int	flexdescindex;
	inline void* pFlexdesc(int i) const
	{
		return (((byte*)this) + flexdescindex) + i;
	};

	int	numflexcontrollers;
	int	flexcontrollerindex;
	inline void* pFlexcontroller(int i) const
	{
		return (((byte*)this) + flexcontrollerindex) + i;
	};

	int	numflexrules;
	int	flexruleindex;
	inline void* pFlexRule(int i) const
	{
		return (((byte*)this) + flexruleindex) + i;
	};

	int	numikchains;
	int	ikchainindex;
	inline void* pIKChain(int i) const
	{
		return (((byte*)this) + ikchainindex) + i;
	};

	int	nummouths;
	int	mouthindex;
	inline void* pMouth(int i) const
	{
		return (((byte*)this) + mouthindex) + i;
	};

	int	numposeparameters;
	int	poseparamindex;
	inline void* pPoseParameter(int i) const
	{
		return (((byte*)this) + poseparamindex) + i;
	};

	int	surfacepropindex;
	inline char* const pszSurfaceProp(void) const
	{
		return ((char*)this) + surfacepropindex;
	}

	// Key values
	int	keyvalueindex;
	int	keyvaluesize;
	inline const char* KeyValueText(void) const
	{
		return keyvaluesize != 0 ? ((char*)this) + keyvalueindex : NULL;
	}

	int	numikautoplaylocks;
	int	ikautoplaylockindex;
	inline void* pIKAutoplayLock(int i) const
	{
		return (((byte*)this) + ikautoplaylockindex) + i;
	};

	float mass;		// The collision model mass that jay wanted
	int	contents;
	int	unused[9];	// remove as appropriate
};

#include "vfunc.hpp"

class CModelInfo
{
public:

	studiohdr_t* GetStudioModel(const model_t* Model)
	{
		typedef studiohdr_t* (__thiscall * Fn)(void*, const model_t*);
		return vfunc<Fn>(this, 32)(this, Model);
	}

	model_t* GetModel(int index) {
		typedef model_t*(__thiscall* Fn)(void*, int);
		return vfunc<Fn>(this, 1)(this, index);
	}

	int GetModelIndex(const char* name) {
		typedef int(__thiscall * Fn)(void*, const char*);
		return vfunc<Fn>(this, 2)(this, name);
	}

	char* GetModelName(const model_t* Model)
	{
		typedef char* (__thiscall * Fn)(void*, const model_t*);
		return vfunc<Fn>(this, 3)(this, Model);
	}

	void GetModelMaterials(const model_t* model, int count, IMaterial** ppMaterial)
	{
		typedef char* (__thiscall * Fn)(void*, const model_t*, int, IMaterial**);
		vfunc<Fn>(this, 19)(this, model, count, ppMaterial);
	}
};
