#include "chams.hpp"
#include <fstream>

#include "../options.hpp"
#include "../hooks.hpp"
#include "../helpers/input.hpp"

#include "backtrack.hpp"

Chams::Chams( ) {
  std::ofstream( "csgo\\materials\\simple_regular.vmt" ) << R"#("VertexLitGeneric"
{
  "$basetexture" "vgui/white_additive"
  "$ignorez"      "0"
  "$envmap"       ""
  "$nofog"        "1"
  "$model"        "1"
  "$nocull"       "0"
  "$selfillum"    "1"
  "$halflambert"  "1"
  "$znearer"      "0"
  "$flat"         "1"
}
)#";
  std::ofstream( "csgo\\materials\\simple_ignorez.vmt" ) << R"#("VertexLitGeneric"
{
  "$basetexture" "vgui/white_additive"
  "$ignorez"      "1"
  "$envmap"       ""
  "$nofog"        "1"
  "$model"        "1"
  "$nocull"       "0"
  "$selfillum"    "1"
  "$halflambert"  "1"
  "$znearer"      "0"
  "$flat"         "1"
}
)#";
  std::ofstream( "csgo\\materials\\simple_flat.vmt" ) << R"#("UnlitGeneric"
{
  "$basetexture" "vgui/white_additive"
  "$ignorez"      "0"
  "$envmap"       ""
  "$nofog"        "1"
  "$model"        "1"
  "$nocull"       "0"
  "$selfillum"    "1"
  "$halflambert"  "1"
  "$znearer"      "0"
  "$flat"         "1"
}
)#";
  std::ofstream( "csgo\\materials\\simple_flat_ignorez.vmt" ) << R"#("UnlitGeneric"
{
  "$basetexture" "vgui/white_additive"
  "$ignorez"      "1"
  "$envmap"       ""
  "$nofog"        "1"
  "$model"        "1"
  "$nocull"       "0"
  "$selfillum"    "1"
  "$halflambert"  "1"
  "$znearer"      "0"
  "$flat"         "1"
}
)#";

  materialRegular = g_MatSystem->FindMaterial( "simple_regular", TEXTURE_GROUP_MODEL );
  materialRegularIgnoreZ = g_MatSystem->FindMaterial( "simple_ignorez", TEXTURE_GROUP_MODEL );
  materialFlatIgnoreZ = g_MatSystem->FindMaterial( "simple_flat_ignorez", TEXTURE_GROUP_MODEL );
  materialFlat = g_MatSystem->FindMaterial( "simple_flat", TEXTURE_GROUP_MODEL );
}

Chams::~Chams( ) {
  std::remove( "csgo\\materials\\simple_regular.vmt" );
  std::remove( "csgo\\materials\\simple_ignorez.vmt" );
  std::remove( "csgo\\materials\\simple_flat.vmt" );
  std::remove( "csgo\\materials\\simple_flat_ignorez.vmt" );
}

void Chams::OverrideMaterial( bool ignoreZ, bool flat, bool wireframe, bool glass, const Color& rgba ) {
  IMaterial* material = nullptr;

  if( flat ) {
	 if( ignoreZ )
		material = materialFlatIgnoreZ;
	 else
		material = materialFlat;
  } else {
	 if( ignoreZ )
		material = materialRegularIgnoreZ;
	 else
		material = materialRegular;
  }


  if( glass ) {
	 material = materialFlat;
	 material->AlphaModulate( 0.45f );
  } else {
	 material->AlphaModulate(
		rgba.a( ) / 255.0f );
  }

  material->SetMaterialVarFlag( MATERIAL_VAR_WIREFRAME, wireframe );
  material->ColorModulate(
	 rgba.r( ) / 255.0f,
	 rgba.g( ) / 255.0f,
	 rgba.b( ) / 255.0f );

  g_MdlRender->ForcedMaterialOverride( material );
}

void Chams::OnDrawModelExecute( void * pResults, DrawModelInfo_t * pInfo, matrix3x4_t * pBoneToWorld,
  float * flpFlexWeights, float * flpFlexDelayedWeights, Vector & vrModelOrigin, int iFlags ) {
  static auto fnDME = Hooks::mdlrender_hook.get_original<Hooks::DrawModelExecute>( index::DrawModelExecute );

  if( !pInfo->m_pClientEntity || !g_LocalPlayer )
	 return;

  const auto mdl = pInfo->m_pClientEntity->GetModel( );

  //bool is_sleeve = strstr( mdl->szName, "sleeve" ) != nullptr;
  //bool is_weapon = strstr(mdl->szName, "weapons/v_")  != nullptr;

  if( g_Options.chams_player_enabled && strstr( mdl->szName, "models/player" ) != nullptr ) {
	 // 
	 // Draw player Chams.
	 // 
	 auto ent = ( C_BasePlayer* )( pInfo->m_pClientEntity->GetIClientUnknown( )->GetBaseEntity( ) );

	 if( ent && ent->IsPlayer( ) && ent->IsAlive( ) ) {
		const auto enemy = ent->m_iTeamNum( ) != g_LocalPlayer->m_iTeamNum( );
		if( !enemy && g_Options.chams_player_enemies_only )
		  return;

		if( g_Options.chams_backtrack && g_Backtrack.data.count( ent->EntIndex( ) ) > 0 ) {
		  auto& data = g_Backtrack.data.at( ent->EntIndex( ) );
		  if( data.size( ) > 0 ) {
			 if( g_Options.chams_backtrack == 2 ) {
				for( auto& record : data ) {
				  OverrideMaterial(
					 false,
					 g_Options.chams_backtrack_flat,
					 false,
					 false,
					 Color( g_Options.color_chams_backtrack ) );
				  fnDME( g_StudioRender, pResults, pInfo, record.boneMatrix, flpFlexWeights, flpFlexDelayedWeights, vrModelOrigin, iFlags );
				}
			 } else if( g_Options.chams_backtrack == 1 ) {
				auto& back = data.back( );
				OverrideMaterial(
				  false,
				  g_Options.chams_backtrack_flat,
				  false,
				  false,
				  Color( g_Options.color_chams_backtrack ) );
				fnDME( g_StudioRender, pResults, pInfo, back.boneMatrix, flpFlexWeights, flpFlexDelayedWeights, vrModelOrigin, iFlags );
			 }
		  }
		}

		const auto clr_front = enemy ? Color( g_Options.color_chams_player_enemy_visible ) : Color( g_Options.color_chams_player_ally_visible );
		const auto clr_back = enemy ? Color( g_Options.color_chams_player_enemy_occluded ) : Color( g_Options.color_chams_player_ally_occluded );

		if( g_Options.chams_player_ignorez ) {
		  OverrideMaterial(
			 true,
			 g_Options.chams_player_flat,
			 g_Options.chams_player_wireframe,
			 false,
			 clr_back );
		  fnDME( g_StudioRender, pResults, pInfo, pBoneToWorld, flpFlexWeights, flpFlexDelayedWeights, vrModelOrigin, iFlags );
		  OverrideMaterial(
			 false,
			 g_Options.chams_player_flat,
			 g_Options.chams_player_wireframe,
			 false,
			 clr_front );
		} else {
		  OverrideMaterial(
			 false,
			 g_Options.chams_player_flat,
			 g_Options.chams_player_wireframe,
			 g_Options.chams_player_glass,
			 clr_front );
		}
	 }
  } else if( strstr( mdl->szName, "arms" ) != nullptr ) {
	 auto material = g_MatSystem->FindMaterial( mdl->szName, TEXTURE_GROUP_MODEL );

	 if( !material )
		return;

	 if( g_Options.other_no_hands ) {
		material->SetMaterialVarFlag( MATERIAL_VAR_NO_DRAW, true );
		g_MdlRender->ForcedMaterialOverride( material );
	 }
  }
}
