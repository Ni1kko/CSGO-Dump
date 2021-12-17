#pragma once
#include "../valve_sdk/csgostructs.hpp"

class IMatRenderContext;
struct DrawModelState_t;
struct ModelRenderInfo_t;
class matrix3x4_t;
class IMaterial;
class Color;

class c_material_helper
{
public:
    c_material_helper ( );
    ~c_material_helper ( );
    void init ( );
    void overwrite ( bool xqz, bool flat, bool wireframe, bool metallic, Color color ) const;
private:
    IMaterial* material_regular = nullptr;
    IMaterial* material_flat = nullptr;
    IMaterial* material_reflective = nullptr;
    IMaterial* material_regular_xqz = nullptr;
    IMaterial* material_flat_xqz = nullptr;
    IMaterial* material_reflective_xqz = nullptr;
};
