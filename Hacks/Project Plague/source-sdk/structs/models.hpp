#pragma once
#include "../math/vector3d.hpp"

struct model_t {
	void* handle;
	char name[260];
	int loadFlags;
	int serverCount;
	int type;
	int flags;
	vec3_t  vecMins;
	vec3_t  vecMaxs;
	float   radius;
};

struct model_render_info_t {
	vec3_t origin;
	vec3_t angles;
	char pad[0x4]; // added this
	void* renderable; // this
	//const void *model; // and this
	const model_t* model;
	const matrix_t* model_to_world;
	const matrix_t* lighting_offset;
	const vec3_t* lighting_origin;
	int flags;
	int entity_index;
	int skin;
	int body;
	int hitboxset;
	unsigned short instance;

	model_render_info_t() {
		model_to_world = NULL;
		lighting_offset = NULL;
		lighting_origin = NULL;
	}
};

typedef unsigned short ClientShadowHandle_t;
typedef unsigned short ClientRenderHandle_t;
typedef unsigned short ModelInstanceHandle_t;

class player_t;

class i_client_unknown
{
public:
    virtual void* GetCollideable() = 0;
    virtual void* GetClientNetworkable() = 0;
    virtual void* GetClientRenderable() = 0;
    virtual void* GetIClientEntity() = 0;
    virtual player_t* GetBaseEntity() = 0;
    virtual void* GetClientThinkable() = 0;
    //virtual IClientModelRenderable*  GetClientModelRenderable() = 0;
    virtual void* GetClientAlphaProperty() = 0;

};


class i_client_renderable
{
public:
    virtual i_client_unknown* get_iclient_unknown() = 0;
    virtual vec3_t const& get_render_origin(void) = 0;
    virtual vec3_t const& get_render_angles(void) = 0;
    virtual bool should_draw(void) = 0;
    virtual int get_render_flags(void) = 0; // ERENDERFLAGS_xxx
    virtual void unused(void) const
    {
    }

    virtual ClientShadowHandle_t get_shadow_handle() const = 0;
    virtual ClientRenderHandle_t& render_handle() = 0;
    virtual const model_t* get_model() const = 0;
    virtual int draw_model(int flags, const int /*RenderableInstance_t*/& instance) = 0;
    virtual int get_body() = 0;
    virtual void get_color_modulation(float* color) = 0;
    virtual bool lod_test() = 0;
    virtual bool setup_bones(matrix3x4_t* bone_to_world_out, int max_bones, int bone_mask, float current_time) = 0;
    virtual void setup_weights(const matrix3x4_t* pBoneToWorld, int nFlexWeightCount, float* pFlexWeights, float* pFlexDelayedWeights) = 0;
    virtual void do_animation_events(void) = 0;
    virtual void* /*IPVSNotify*/ get_pvs_notify_interface() = 0;
    virtual void get_render_bounds(vec3_t& mins, vec3_t& maxs) = 0;
    virtual void get_render_bounds_worldspace(vec3_t& mins, vec3_t& maxs) = 0;
    virtual void get_shadow_render_bounds(vec3_t& mins, vec3_t& maxs, int /*ShadowType_t*/ shadowType) = 0;
    virtual bool should_receive_projected_textures(int flags) = 0;
    virtual bool get_shadow_cast_distance(float* pDist, int /*ShadowType_t*/ shadowType) const = 0;
    virtual bool get_shadow_cast_direction(vec3_t* pDirection, int /*ShadowType_t*/ shadowType) const = 0;
    virtual bool is_shadow_dirty() = 0;
    virtual void mark_shadow_dirty(bool bDirty) = 0;
    virtual i_client_renderable* get_shadow_parent() = 0;
    virtual i_client_renderable* first_shadow_child() = 0;
    virtual i_client_renderable* next_shadow_peer() = 0;
    virtual int /*ShadowType_t*/ shadow_cast_type() = 0;
    virtual void create_model_instance() = 0;
    virtual ModelInstanceHandle_t get_model_instance() = 0;
    virtual const matrix3x4_t& renderable_to_world_transform() = 0;
    virtual int lookup_attachment(const char* pAttachmentName) = 0;
    virtual bool get_attachment(int number, vec3_t& origin, vec3_t& angles) = 0;
    virtual bool get_attachment(int number, matrix3x4_t& matrix) = 0;
    virtual float* get_render_clip_plane(void) = 0;
    virtual int get_skin() = 0;
    virtual void on_threaded_draw_setup() = 0;
    virtual bool uses_flex_delayed_weights() = 0;
    virtual void record_tool_message() = 0;
    virtual bool should_draw_for_split_screen_user(int nSlot) = 0;
    virtual uint8_t override_alpha_modulation(uint8_t nAlpha) = 0;
    virtual uint8_t override_shadow_alpha_modulation(uint8_t nAlpha) = 0;
};
