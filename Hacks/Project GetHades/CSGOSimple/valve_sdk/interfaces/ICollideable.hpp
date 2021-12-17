#pragma once

enum SolidType_t;
class IHandleEntity;
struct Ray_t;
struct model_t;
class CGameTrace;
typedef CGameTrace trace_t;
class IClientUnknown;
class matrix3x4_t;

class ICollideable
{
public:
    virtual IHandleEntity* get_entity_handle ( ) = 0;
    virtual Vector& obb_mins ( ) const = 0;
    virtual Vector& obb_maxs ( ) const = 0;
    virtual void world_space_trigger_bounds ( Vector* pVecWorldMins, Vector* pVecWorldMaxs ) const = 0;
    virtual bool test_collision ( const Ray_t& ray, unsigned int fContentsMask, trace_t& tr ) = 0;
    virtual bool test_hitboxes ( const Ray_t& ray, unsigned int fContentsMask, trace_t& tr ) = 0;
    virtual int get_collision_model_index ( ) = 0;
    virtual const model_t* get_collision_model ( ) = 0;
    virtual const Vector& get_collision_origin ( ) const = 0;
    virtual const QAngle& get_collision_angles ( ) const = 0;
    virtual const matrix3x4_t& collision_to_world_transform ( ) const = 0;
    virtual SolidType_t get_solid ( ) const = 0;
    virtual int get_solid_flags ( ) const = 0;
    virtual IClientUnknown* get_iclient_unknown ( ) = 0;
    virtual int get_collision_group ( ) const = 0;
    virtual void world_space_surrounding_bounds ( Vector* pVecMins, Vector* pVecMaxs ) = 0;
    virtual bool should_touch_trigger ( int triggerSolidFlags ) const = 0;
    virtual const matrix3x4_t* get_root_parent_to_world_transform ( ) const = 0;
};
