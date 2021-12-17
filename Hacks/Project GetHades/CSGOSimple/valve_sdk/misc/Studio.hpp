#pragma once

#include "../math/Vector.hpp"

typedef float quaternion_t[4];
typedef float RadianEuler[3];

#define max_qpath  260

#define bone_calculate_mask             0x1F
#define bone_physically_simulated       0x01    // bone is physically simulated when physics are active
#define bone_physics_procedural         0x02    // procedural when physics is active
#define bone_always_procedural          0x04    // bone is always procedurally animated
#define bone_screen_align_sphere        0x08    // bone aligns to the screen, not constrained in motion.
#define bone_screen_align_cylinder      0x10    // bone aligns to the screen, constrained by it's own axis.

#define bone_used_mask                  0x0007FF00
#define bone_used_by_anything           0x0007FF00
#define bone_used_by_hitbox             0x00000100    // bone (or child) is used by a hit box
#define bone_used_by_attachment         0x00000200    // bone (or child) is used by an attachment point
#define bone_used_by_vertex_mask        0x0003FC00
#define bone_used_by_vertex_lod0        0x00000400    // bone (or child) is used by the toplevel model via skinned vertex
#define bone_used_by_vertex_lod1        0x00000800
#define bone_used_by_vertex_lod2        0x00001000
#define bone_used_by_vertex_lod3        0x00002000
#define bone_used_by_vertex_lod4        0x00004000
#define bone_used_by_vertex_lod5        0x00008000
#define bone_used_by_vertex_lod6        0x00010000
#define bone_used_by_vertex_lod7        0x00020000
#define bone_used_by_bone_merge         0x00040000    // bone is available for bone merge to occur against it

#define bone_used_by_vertex_at_lod(lod) ( BONE_USED_BY_VERTEX_LOD0 << (lod) )
#define bone_used_by_anything_at_lod(lod) ( ( BONE_USED_BY_ANYTHING & ~BONE_USED_BY_VERTEX_MASK ) | bone_used_by_vertex_at_lod(lod) )

#define max_num_lods 8
#define maxstudiobones		128		// total bones actually used

#define bone_type_mask                  0x00F00000
#define bone_fixed_alignment            0x00100000    // bone can't spin 360 degrees, all interpolation is normalized around a fixed orientation

#define bone_has_saveframe_pos          0x00200000    // Vector48
#define bone_has_saveframe_rot64        0x00400000    // Quaternion64
#define bone_has_saveframe_rot32        0x00800000    // Quaternion32

#define hitgroup_generic 0
#define hitgroup_head 1
#define hitgroup_chest 2
#define hitgroup_stomach 3
#define hitgroup_leftarm 4
#define hitgroup_rightarm 5
#define hitgroup_leftleg 6
#define hitgroup_rightleg 7
#define hitgroup_gear 10

enum modtype_t
{
    mod_bad = 0,
    mod_brush,
    mod_sprite,
    mod_studio
};

enum hitboxes_t
{
    hitbox_head,
    hitbox_neck,
    hitbox_pelvis,
    hitbox_stomach,
    hitbox_lower_chest,
    hitbox_chest,
    hitbox_upper_chest,
    hitbox_right_thigh,
    hitbox_left_thigh,
    hitbox_right_calf,
    hitbox_left_calf,
    hitbox_right_foot,
    hitbox_left_foot,
    hitbox_right_hand,
    hitbox_left_hand,
    hitbox_right_upper_arm,
    hitbox_right_forearm,
    hitbox_left_upper_arm,
    hitbox_left_forearm,
    hitbox_max
};

typedef unsigned short mdl_handle_t;

struct mstudiobone_t
{
    int sznameindex{ };

    inline char* name ( void ) const
    {
        return ( ( char* )this ) + sznameindex;
    }

    int parent{ };
    int bone_controller[6]{ }; // bone controller index, -1 == none
    Vector pos;
    quaternion_t quat{ };
    RadianEuler rot{ };
    // compression scale
    Vector posscale;
    Vector rotscale;

    matrix3x4_t poseToBone;
    quaternion_t qAlignment{ };
    int flags{ };
    int proctype{ };
    int procindex{ };
    mutable int physicsbone{ };

    inline void* pProcedure ( ) const
    {
        if ( procindex == 0 )
            return nullptr;
        return static_cast< void* > ( ( ( byte* )this ) + procindex );
    };
    int surfacepropidx{ };

    inline char* const pszSurfaceProp ( void ) const
    {
        return ( ( char* )this ) + surfacepropidx;
    }

    inline int GetSurfaceProp ( void ) const
    {
        return surfacepropLookup;
    }

    int contents{ };
    int surfacepropLookup{ };
    int m_iPad01[7]{ };

    mstudiobone_t ( ) = default;
private:
    // No copy constructors allowed
    mstudiobone_t ( const mstudiobone_t& vOther ) = delete;
};

struct mstudiobbox_t
{
    int bone; // 4
    int group; // 8
    Vector bbmin; // 20
    Vector bbmax; // 32
    int szhitboxnameindex; // 36
    int32_t m_iPad01[3];
    float m_flRadius;
    int32_t m_iPad02[4];

    const char* get_name ( )
    {
        if ( !szhitboxnameindex )
            return nullptr;

        return reinterpret_cast< const char* > ( reinterpret_cast< uint8_t* > ( this ) + szhitboxnameindex );
    }
};

struct mstudiohitboxset_t
{
    int sznameindex;
    int numhitboxes;
    int hitboxindex;

    const char* get_name ( )
    {
        if ( !sznameindex )
            return nullptr;

        return reinterpret_cast< const char* > ( reinterpret_cast< uint8_t* > ( this ) + sznameindex );
    }

    mstudiobbox_t* get_hitbox ( int i )
    {
        if ( i > numhitboxes )
            return nullptr;

        return reinterpret_cast< mstudiobbox_t* > ( reinterpret_cast< uint8_t* > ( this ) + hitboxindex ) + i;
    }
};

struct model_t
{
    void* fnHandle; //0x0000
    char szName[260]; //0x0004
    __int32 nLoadFlags; //0x0108
    __int32 nServerCount; //0x010C
    __int32 type; //0x0110
    __int32 flags; //0x0114
    Vector vecMins; //0x0118
    Vector vecMaxs; //0x0124
    float radius; //0x0130
    char pad[0x1C]; //0x0134
}; //Size=0x0150

class studiohdr_t
{
public:
    __int32 id; //0x0000
    __int32 version; //0x0004
    long checksum; //0x0008
    char szName[64]; //0x000C
    __int32 length; //0x004C
    Vector vecEyePos; //0x0050
    Vector vecIllumPos; //0x005C
    Vector vecHullMin; //0x0068
    Vector vecHullMax; //0x0074
    Vector vecBBMin; //0x0080
    Vector vecBBMax; //0x008C
    __int32 flags; //0x0098
    __int32 numbones; //0x009C
    __int32 boneindex; //0x00A0
    __int32 numbonecontrollers; //0x00A4
    __int32 bonecontrollerindex; //0x00A8
    __int32 numhitboxsets; //0x00AC
    __int32 hitboxsetindex; //0x00B0
    __int32 numlocalanim; //0x00B4
    __int32 localanimindex; //0x00B8
    __int32 numlocalseq; //0x00BC
    __int32 localseqindex; //0x00C0
    __int32 activitylistversion; //0x00C4
    __int32 eventsindexed; //0x00C8
    __int32 numtextures; //0x00CC
    __int32 textureindex; //0x00D0

    mstudiohitboxset_t* get_hitbox_set ( int i )
    {
        if ( i > numhitboxsets )
            return nullptr;

        return reinterpret_cast< mstudiohitboxset_t* > ( reinterpret_cast< uint8_t* > ( this ) + hitboxsetindex ) + i;
    }

    mstudiobone_t* get_bone ( int i )
    {
        if ( i > numbones )
            return nullptr;

        return reinterpret_cast< mstudiobone_t* > ( reinterpret_cast< uint8_t* > ( this ) + boneindex ) + i;
    }
}; //Size=0x00D4
