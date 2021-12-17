#include "material_helper.h"
#include "../options.h"

c_material_helper::c_material_helper ( )
{
}

c_material_helper::~c_material_helper ( )
{
    std::remove ( xor_str ( "csgo\\materials\\material_regular.vmt" ) );
    std::remove ( xor_str ( "csgo\\materials\\material_reflective.vmt" ) );
    std::remove ( xor_str ( "csgo\\materials\\material_flat.vmt" ) );
    std::remove ( xor_str ( "csgo\\materials\\material_xqz.vmt" ) );
    std::remove ( xor_str ( "csgo\\materials\\material_flat_xqz.vmt" ) );
    std::remove ( xor_str ( "csgo\\materials\\material_reflective_xqz.vmt" ) );
}

void c_material_helper::init ( )
{
    std::ofstream ( xor_str ( "csgo\\materials\\material_regular.vmt" ) ) << xor_str (
        R"#("VertexLitGeneric"
{
  "$basetexture" "vgui/white_additive"
  "$ignorez"      "0"
  "$nofog"        "1"
  "$model"        "1"
  "$nocull"       "0"
  "$selfillum"    "1"
  "$halflambert"  "1"
  "$znearer"      "0"
  "$flat"         "1"
  "$envmap"       ""
}
)#" );
    std::ofstream ( xor_str ( "csgo\\materials\\material_flat.vmt" ) ) << xor_str (
        R"#("UnlitGeneric"
{
  "$basetexture" "vgui/white_additive"
  "$ignorez"      "0"
  "$nofog"        "1"
  "$model"        "1"
  "$nocull"       "0"
  "$selfillum"    "1"
  "$halflambert"  "1"
  "$znearer"      "0"
  "$flat"         "1"
  "$envmap"       ""
}
)#" );
    std::ofstream ( xor_str ( "csgo\\materials\\material_reflective.vmt" ) ) << xor_str (
        R"#("VertexLitGeneric"
{
  "$basetexture" "vgui/white_additive"
  "$envmap"       "env_cubemap"
  "$model" "1"
  "$normalmapalphaenvmapmask"  "1"
  "$envmapcontrast" "1"
  "$flat" "1"
  "$nocull" "0"
  "$selfillum" "1"
  "$halflambert" "1"
  "$nofog" "0"
  "$ignorez" "0"
  "$znearer" "0"
  "$wireframe" "0"
}
)#" );
    std::ofstream ( xor_str ( "csgo\\materials\\material_xqz.vmt" ) ) << xor_str (
        R"#("VertexLitGeneric"
{
  "$basetexture" "vgui/white_additive"
  "$ignorez"      "1"
  "$nofog"        "1"
  "$model"        "1"
  "$nocull"       "0"
  "$selfillum"    "1"
  "$halflambert"  "1"
  "$znearer"      "0"
  "$flat"         "1"
  "$envmap"       ""
}
)#" );
    std::ofstream ( xor_str ( "csgo\\materials\\material_flat_xqz.vmt" ) ) << xor_str (
        R"#("UnlitGeneric"
{
  "$basetexture" "vgui/white_additive"
  "$ignorez"      "1"
  "$nofog"        "1"
  "$model"        "1"
  "$nocull"       "0"
  "$selfillum"    "1"
  "$halflambert"  "1"
  "$znearer"      "0"
  "$flat"         "1"
  "$envmap"       ""
}
)#" );
    std::ofstream ( xor_str ( "csgo\\materials\\material_reflective_xqz.vmt" ) ) << xor_str (
        R"#("VertexLitGeneric"
{
  "$basetexture" "vgui/white_additive"
  "$envmap"       "models/effects/crystal_cube_vertigo_hdr"
   "$reflectivity" "[1 1 1]"
  "$model" "1"
  "$normalmapalphaenvmapmask"  "1"
  "$envmapcontrast" "1"
  "$flat" "1"
  "$nocull" "0"
  "$selfillum" "1"
  "$halflambert" "1"
  "$nofog" "0"
  "$ignorez" "1"
  "$znearer" "0"
  "$wireframe" "0"
}
)#" );

    material_regular = interfaces::mat_system->find_material ( xor_str ( "material_regular" ), TEXTURE_GROUP_MODEL );
    material_reflective = interfaces::mat_system->find_material ( xor_str ( "material_reflective" ), TEXTURE_GROUP_MODEL );
    material_flat = interfaces::mat_system->find_material ( xor_str ( "material_flat" ), TEXTURE_GROUP_MODEL );
    material_regular_xqz = interfaces::mat_system->find_material ( xor_str ( "material_xqz" ), TEXTURE_GROUP_MODEL );
    material_flat_xqz = interfaces::mat_system->find_material ( xor_str ( "material_flat_xqz" ), TEXTURE_GROUP_MODEL );
    material_reflective_xqz = interfaces::mat_system->find_material ( xor_str ( "material_reflective_xqz" ), TEXTURE_GROUP_MODEL );
}

void c_material_helper::overwrite ( bool xqz, bool flat, bool wireframe, bool metallic, Color color ) const
{
    if ( !interfaces::mdl_render || !g_local || !interfaces::mat_system )
        return;

    IMaterial* mat;

    if ( !interfaces::engine_client->is_in_game( ) || !interfaces::engine_client->is_connected( ) || interfaces::engine_client->is_paused( ) || interfaces::
        engine_client->is_hltv( ) )
        return;

    if ( metallic )
    {
        if ( xqz )
            mat = material_reflective_xqz;
        else
            mat = material_reflective;
    }
    else if ( flat )
    {
        if ( xqz )
            mat = material_flat_xqz;
        else
            mat = material_flat;
    }
    else
    {
        if ( xqz )
            mat = material_flat_xqz;
        else
            mat = material_regular;
    }

    if ( !mat || mat == nullptr )
        return;

    //mat->set_material_var_flag(MATERIAL_VAR_IGNOREZ, xqz); // causing most crashes on join
    mat->set_material_var_flag ( MATERIAL_VAR_WIREFRAME, wireframe );
    mat->color_modulate ( color.r( ) / 255.f, color.g( ) / 255.f, color.b( ) / 255.f );

    interfaces::mdl_render->forced_material_override ( mat );
}
