#pragma once
#include "../valve_sdk/csgostructs.hpp"

enum class chams_modes_t : int
{
    none,
    normal,
    flat,
    metallic
};

class c_chams
{
public:
    void on_draw_model_execute ( IMatRenderContext* ctx, const DrawModelState_t& state, const ModelRenderInfo_t& pinfo, matrix3x4_t* matrix ) const;
};
