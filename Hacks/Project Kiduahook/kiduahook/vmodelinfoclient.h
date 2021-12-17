#pragma once

#include "sdk.h"

class RadianEuler
{
public:
	float x, y, z;
};

enum eHitboxes
{
	HITBOX_HEAD,
	HITBOX_NECK,
	HITBOX_PELVIS,
	HITBOX_BELLY,
	HITBOX_THORAX,
	HITBOX_LOWER_CHEST,
	HITBOX_UPPER_CHEST,
	HITBOX_RIGHT_THIGH,
	HITBOX_LEFT_THIGH,
	HITBOX_RIGHT_CALF,
	HITBOX_LEFT_CALF,
	HITBOX_RIGHT_FOOT,
	HITBOX_LEFT_FOOT,
	HITBOX_RIGHT_HAND,
	HITBOX_LEFT_HAND,
	HITBOX_RIGHT_UPPER_ARM,
	HITBOX_RIGHT_FOREARM,
	HITBOX_LEFT_UPPER_ARM,
	HITBOX_LEFT_FOREARM,
	HITBOX_MAX
};

struct mstudiobbox_t
{
	int bone;
	int group;
	Vector bbmin;
	Vector bbmax;
	int szhitboxnameindex;
	Vector rotation;
	float radius;
	int pad[4];

	const char* pszHitboxName() const
	{
		/*if (szhitboxnameindex == 0)
		return "";*/

		return ((const char*)this) + szhitboxnameindex;
	}
};

struct mstudiohitboxset_t
{
	int                                             sznameindex;
	inline char* const              pszName( void ) const { return ( ( char* )this ) + sznameindex; }
	int                                             numhitboxes;
	int                                             hitboxindex;
	inline mstudiobbox_t*   pHitbox( int i ) const { return ( mstudiobbox_t* )( ( ( BYTE* )this ) + hitboxindex ) + i; };
};

struct mstudiobone_t
{
	int sznameindex;

	inline char* const pszName( void ) const
	{
		return ( ( char * )this ) + sznameindex;
	}

	int parent; // parent bone
	int bonecontroller[ 6 ]; // bone controller index, -1 == none

							 // default values
	Vector pos;
	Quaternion quat;
	Vector rot;
	// compression scale
	Vector posscale;
	Vector rotscale;

	matrix3x4_t poseToBone;
	Quaternion qAlignment;
	int flags;
	int proctype;
	int procindex; // procedural rule
	mutable int physicsbone; // index into physically simulated bone
	inline void* pProcedure() const
	{
		if( procindex == 0 )
			return NULL;
		else
			return ( void * ) ( ( ( byte * )this ) + procindex );
	};
	int surfacepropidx; // index into string tablefor property name
	inline char* const pszSurfaceProp( void ) const
	{
		return ( ( char * )this ) + surfacepropidx;
	}

	int contents; // See BSPFlags.h for the contents flags

	int unused[ 8 ]; // remove as appropriate

	mstudiobone_t() {}
private:
	// No copy constructors allowed
	mstudiobone_t( const mstudiobone_t& vOther );
};

struct studiohdr_t
{
	int					id;
	int					version;

	int					checksum;		// this has to be the same in the phy and vtx files to load!

	char				name[ 64 ];
	int					length;


	Vector				eyeposition;	// ideal eye position

	Vector				illumposition;	// illumination center

	Vector				hull_min;		// ideal movement hull size
	Vector				hull_max;

	Vector				view_bbmin;		// clipping bounding box
	Vector				view_bbmax;

	int					flags;

	int					numbones;			// bones
	int					boneindex;
	int					RemapSeqBone( int iSequence, int iLocalBone ) const;	// maps local sequence bone to global bone
	int					RemapAnimBone( int iAnim, int iLocalBone ) const;		// maps local animations bone to global bone

	int					numbonecontrollers;		// bone controllers
	int					bonecontrollerindex;

	int					numhitboxsets;
	int					hitboxsetindex;

	// Look up hitbox set by index
	mstudiohitboxset_t	*pHitboxSet( int i ) const
	{
		Assert( i >= 0 && i < numhitboxsets );
		return ( mstudiohitboxset_t * )( ( ( BYTE * )this ) + hitboxsetindex ) + i;
	};

	inline mstudiobone_t* GetBone( int i ) const
	{
		return ( mstudiobone_t * ) ( ( ( byte * )this ) + boneindex ) + i;
	};

	// Calls through to hitbox to determine size of specified set
	inline mstudiobbox_t *pHitbox( int i, int set ) const
	{
		mstudiohitboxset_t const *s = pHitboxSet( set );
		if( !s )
			return NULL;

		return s->pHitbox( i );
	};

	// Calls through to set to get hitbox count for set
	inline int			iHitboxCount( int set ) const
	{
		mstudiohitboxset_t const *s = pHitboxSet( set );
		if( !s )
			return 0;

		return s->numhitboxes;
	};

	// file local animations? and sequences
	//private:
	int					numlocalanim;			// animations/poses
	int					localanimindex;		// animation descriptions

	int					numlocalseq;				// sequences
	int					localseqindex;

	//public:
	bool				SequencesAvailable() const;
	int					GetNumSeq() const;
	int					iRelativeAnim( int baseseq, int relanim ) const;	// maps seq local anim reference to global anim index
	int					iRelativeSeq( int baseseq, int relseq ) const;		// maps seq local seq reference to global seq index

	//private:
	mutable int			activitylistversion;	// initialization flag - have the sequences been indexed?
	mutable int			eventsindexed;
	//public:
	int					GetSequenceActivity( int iSequence );
	void				SetSequenceActivity( int iSequence, int iActivity );
	int					GetActivityListVersion( void );
	void				SetActivityListVersion( int version ) const;
	int					GetEventListVersion( void );
	void				SetEventListVersion( int version );

	// raw textures
	int					numtextures;
	int					textureindex;


	// raw textures search paths
	int					numcdtextures;
	int					cdtextureindex;
	inline char			*pCdtexture( int i ) const { return ( ( ( char * )this ) + *( ( int * )( ( ( BYTE * )this ) + cdtextureindex ) + i ) ); };

	// replaceable textures tables
	int					numskinref;
	int					numskinfamilies;
	int					skinindex;
	inline short		*pSkinref( int i ) const { return ( short * )( ( ( BYTE * )this ) + skinindex ) + i; };

	int					numbodyparts;
	int					bodypartindex;

	// queryable attachable points
	//private:
	int					numlocalattachments;
	int					localattachmentindex;
	//public:
	int					GetNumAttachments( void ) const;
	int					GetAttachmentBone( int i );
	// used on my tools in hlmv, not persistant
	void				SetAttachmentBone( int iAttachment, int iBone );

	// animation node to animation node transition graph
	//private:
	int					numlocalnodes;
	int					localnodeindex;
	int					localnodenameindex;
	inline char			*pszLocalNodeName( int iNode ) const { Assert( iNode >= 0 && iNode < numlocalnodes ); return ( ( ( char * )this ) + *( ( int * )( ( ( BYTE * )this ) + localnodenameindex ) + iNode ) ); }
	inline BYTE			*pLocalTransition( int i ) const { Assert( i >= 0 && i < ( numlocalnodes * numlocalnodes ) ); return ( BYTE * )( ( ( BYTE * )this ) + localnodeindex ) + i; };

	//public:
	int					EntryNode( int iSequence );
	int					ExitNode( int iSequence );
	char				*pszNodeName( int iNode );
	int					GetTransition( int iFrom, int iTo ) const;

	int					numflexdesc;
	int					flexdescindex;

	int					numflexcontrollers;
	int					flexcontrollerindex;

	int					numflexrules;
	int					flexruleindex;

	int					numikchains;
	int					ikchainindex;

	int					nummouths;
	int					mouthindex;

	//private:
	int					numlocalposeparameters;
	int					localposeparamindex;
	//public:
	int					GetNumPoseParameters( void ) const;
	int					GetSharedPoseParameter( int iSequence, int iLocalPose ) const;

	int					surfacepropindex;
	inline char * const pszSurfaceProp( void ) const { return ( ( char * )this ) + surfacepropindex; }

	// Key values
	int					keyvalueindex;
	int					keyvaluesize;
	inline const char * KeyValueText( void ) const { return keyvaluesize != 0 ? ( ( char * )this ) + keyvalueindex : NULL; }

	int					numlocalikautoplaylocks;
	int					localikautoplaylockindex;

	int					GetNumIKAutoplayLocks( void ) const;
	int					CountAutoplaySequences() const;
	int					CopyAutoplaySequences( unsigned short *pOut, int outCount ) const;
	int					GetAutoplayList( unsigned short **pOut ) const;

	// The collision model mass that jay wanted
	float				mass;
	int					contents;

	// external animations, models, etc.
	int					numincludemodels;
	int					includemodelindex;
	// implementation specific call to get a named model
	const studiohdr_t	*FindModel( void **cache, char const *modelname ) const;

	// implementation specific back pointer to virtual data
	mutable void		*virtualModel;
	//virtualmodel_t		GetVirtualModel(void) const;

	// for demand loaded animation blocks
	int					szanimblocknameindex;
	inline char * const pszAnimBlockName( void ) const { return ( ( char * )this ) + szanimblocknameindex; }
	int					numanimblocks;
	int					animblockindex;
	mutable void		*animblockModel;
	BYTE *				GetAnimBlock( int i ) const;

	int					bonetablebynameindex;
	inline const BYTE	*GetBoneTableSortedByName() const { return ( BYTE * )this + bonetablebynameindex; }

	// used by tools only that don't cache, but persist mdl's peer data
	// engine uses virtualModel to back link to cache pointers
	void				*pVertexBase;
	void				*pIndexBase;

	// if STUDIOHDR_FLAGS_CONSTANT_DIRECTIONAL_LIGHT_DOT is set,
	// this value is used to calculate directional components of lighting 
	// on static props
	BYTE				constdirectionallightdot;

	// set during load of mdl data to track *desired* lod configuration (not actual)
	// the *actual* clamped root lod is found in studiohwdata
	// this is stored here as a global store to ensure the staged loading matches the rendering
	BYTE				rootLOD;

	// set in the mdl data to specify that lod configuration should only allow first numAllowRootLODs
	// to be set as root LOD:
	//	numAllowedRootLODs = 0	means no restriction, any lod can be set as root lod.
	//	numAllowedRootLODs = N	means that lod0 - lod(N-1) can be set as root lod, but not lodN or lower.
	BYTE				numAllowedRootLODs;

	BYTE				unused[ 1 ];

	int					unused4; // zero out if version < 47

	int					numflexcontrollerui;
	int					flexcontrolleruiindex;
	int					unused3[ 2 ];

	// FIXME: Remove when we up the model version. Move all fields of studiohdr2_t into studiohdr_t.
	int					studiohdr2index;


	// NOTE: No room to add stuff? Up the .mdl file format version 
	// [and move all fields in studiohdr2_t into studiohdr_t and kill studiohdr2_t],
	// or add your stuff to studiohdr2_t. See NumSrcBoneTransforms/SrcBoneTransform for the pattern to use.
	int					unused2[ 1 ];

	studiohdr_t() {}

private:
	// No copy constructors allowed
	studiohdr_t( const studiohdr_t& vOther );

	friend struct virtualmodel_t;
};

struct DrawModelState_t
{
	studiohdr_t*			m_pStudioHdr;
	unsigned long			m_pStudioHWData;
	CBaseEntity*		m_pRenderable;
	const matrix3x4_t		*m_pModelToWorld;
	unsigned long		m_decals;
	int						m_drawFlags;
	int						m_lod;
};

class VModelInfoClient
{
public:
	int GetModelIndex( const char* ModelName )
	{
		typedef int( __thiscall* OriginalFn )( PVOID, const char* );
		return getvfunc<OriginalFn>( this, 2 )( this, ModelName );
	}

	const char* GetModelName( const model_t* pModel )
	{
		typedef const char* ( __thiscall* OriginalFn )( PVOID, const model_t* );
		return getvfunc<OriginalFn>( this, 3 )( this, pModel );
	}

	int GetInt( const char *keyName, int defaultValue = 0 )
	{
		typedef int( __thiscall* OriginalFn )( PVOID, const char *, int );
		return getvfunc<OriginalFn>( this, 6 )( this, keyName, defaultValue );
	}

	void GetModelMaterials( const model_t *model, int count, IMaterial** ppMaterial )
	{
		typedef void( __thiscall* OriginalFn )( PVOID, const model_t*, int, IMaterial** );
		getvfunc<OriginalFn>( this, 17 )( this, model, count, ppMaterial );
	}

	studiohdr_t     *GetStudiomodel( const model_t *mod )
	{
		typedef studiohdr_t* ( __thiscall* OriginalFn )( PVOID, const model_t* );
		return getvfunc<OriginalFn>( this, 32 )( this, mod );
	}
};
extern VModelInfoClient* g_pModel;

class VEngineModel
{
public:
	void DrawModelExecute( void* context, DrawModelState_t &state, const ModelRenderInfo_t &pInfo, matrix3x4_t *pCustomBoneToWorld )
	{
		typedef void( __thiscall* OriginalFn )( PVOID, void* context, DrawModelState_t &state, const ModelRenderInfo_t &pInfo, matrix3x4_t *pCustomBoneToWorld );
		getvfunc<OriginalFn>( this, 21 )( this, context, state, pInfo, pCustomBoneToWorld );
	}
	void ForcedMaterialOverride( IMaterial *newMaterial, int nOverrideType = 0, int iUnknown = 0 )
	{
		typedef void( __thiscall* original )( void*, IMaterial*, int, int );
		getvfunc<original>( this, 1 )( this, newMaterial, nOverrideType, iUnknown );
	}
};
extern VEngineModel* g_pEngineModel;