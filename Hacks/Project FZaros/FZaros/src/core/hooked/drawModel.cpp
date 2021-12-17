#include "../managers/hookManager.hpp"
#include "../../features/Aimbot/LagCompensation.hpp"

using drawModelExecuteFn = void( __thiscall* )( IVModelRender*, void*, void*, const ModelRenderInfo_t&, matrix3x4_t* );
void __stdcall  CHookManager::drawModelExecute( void* context, void* state, const ModelRenderInfo_t& info, matrix3x4_t* matrix ) {

	auto originalFn = g_Hooks->modelRenderHook->getOriginalFunction<drawModelExecuteFn>( 21 );

	if ( g_Interfaces->modelRender->IsForcedMaterialOverride( ) ) {
		return originalFn( g_Interfaces->modelRender, context, state, info, matrix );
	}

	static bool initialized = false;
	static IMaterial* material = nullptr;
	static IMaterial* lcMat = nullptr;
	static IMaterial* textured = nullptr;
	static IMaterial* metallic = nullptr;
	static IMaterial* dogtag = nullptr;
	static IMaterial* flat = nullptr;
	if ( !initialized ) {
		initialized = true;

		textured = g_Interfaces->materialSystem->FindMaterial("dopamine_material", TEXTURE_GROUP_MODEL, true, nullptr);
		metallic = g_Interfaces->materialSystem->FindMaterial("dopamine_reflective", TEXTURE_GROUP_MODEL, true, nullptr);
		dogtag = g_Interfaces->materialSystem->FindMaterial("models/inventory_items/dogtags/dogtags_outline", TEXTURE_GROUP_MODEL, true, nullptr);
		flat = g_Interfaces->materialSystem->FindMaterial("debug/debugdrawflat", TEXTURE_GROUP_MODEL, true, nullptr);
		lcMat = flat;
	}
	textured->IncrementReferenceCount();
	metallic->IncrementReferenceCount();
	dogtag->IncrementReferenceCount();
	flat->IncrementReferenceCount();
	lcMat->IncrementReferenceCount();

	switch (config->get_int("chamsMaterial")) {
	case 0:
		material = textured;
		break;
	case 1:
		material = flat;
		break;
	case 2:
		material = metallic;
		break;
	case 3:
		material = dogtag;
		break;
	}

	CBaseEntity* entity = reinterpret_cast< CBaseEntity* >( g_Interfaces->clientEntity->GetClientEntity( info.entity_index ) );
	std::string model_name = g_Interfaces->modelInfo->getModelName( ( model_t* )info.pModel );

	if ( g_Interfaces->gameEngine->inGame( ) ) {
		if ( Globals::localPlayer )
		{
			if (config->get_bool("espEnable") && config->get_bool("chamsMaterial") != 0 )
			{
				if ( model_name.find( "models/player" ) != std::string::npos )
				{
					//if ( material && materialIgnoreZ && materialDefault && materialDefaultIgnoreZ && materialFlat && materialFlatIgnoreZ && materialShaded && materialShadedIgnoreZ && materialMetallic && materialMetallicIgnoreZ && materialGlow && materialGlowIgnoreZ && materialBubble && materialBubbleIgnoreZ && lcMat )
					if ( material && textured && metallic && dogtag && flat && lcMat )
					{
						if ( entity )
						{
							if ( entity->team( ) != Globals::localPlayer->team( ) ) // enemy
							{
								// man we're so fucking smart, we better hook drawmodel instead of drawmodelexecute, because POLAK DOES IT. right? ~alpha
								if ( entity->health( ) > 0 )
								{
									auto& records = g_LagCompensation.PlayerRecord[ entity->EntIndex( ) ].records;
									if ( !records.empty( ) ) {
										matrix3x4_t matrix[ 128 ];
										std::memcpy( matrix, records.front( ).boneMatrix, sizeof( records.front( ).boneMatrix ) );

										// lag comp 
										if ( Globals::localPlayer->alive( ) && lcMat && config->get_bool("chamsShadow") && !entity->immunity( ) )
										{
											lcMat->AlphaModulate(config->get_color("colorChamsShadow").a() / 255.f);
											lcMat->ColorModulate(config->get_color("colorChamsShadow").r() / 255.f,
												config->get_color("colorChamsShadow").g() / 255.f,
												config->get_color("colorChamsShadow").b() / 255.f);
											lcMat->SetMaterialVarFlag( MATERIAL_VAR_IGNOREZ, true );
											g_Interfaces->modelRender->ForcedMaterialOverride( lcMat );
											originalFn( g_Interfaces->modelRender, context, state, info, matrix );
										}

									}

									// visible 
									if (config->get_bool("chamsEnemy")) // player
									{
										material->AlphaModulate(config->get_color("colorChamsEnemy").a() / 255.f);
										material->ColorModulate(config->get_color("colorChamsEnemy").r() / 255.f,
											config->get_color("colorChamsEnemy").g() / 255.f,
											config->get_color("colorChamsEnemy").b() / 255.f);
										material->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);

										g_Interfaces->modelRender->ForcedMaterialOverride(material);
										originalFn(g_Interfaces->modelRender, context, state, info, matrix);
									}

									//invisible
									if (config->get_bool("chamsEnemyXQZ")) // player (behind wall)
									{
										material->AlphaModulate(config->get_color("colorChamsEnemyXQZ").a() / 255.f);
										material->ColorModulate(config->get_color("colorChamsEnemyXQZ").r() / 255.f,
											config->get_color("colorChamsEnemyXQZ").g() / 255.f,
											config->get_color("colorChamsEnemyXQZ").b() / 255.f);
										material->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, true);

										g_Interfaces->modelRender->ForcedMaterialOverride(material);
										originalFn(g_Interfaces->modelRender, context, state, info, matrix);
									}
								}
							}
							else if (entity->team() == Globals::localPlayer->team()) // teammate
							{
								// visible 
								if (config->get_bool("chamsTeammates")) // teammate
								{
									material->AlphaModulate(config->get_color("colorChamsTeammates").a() / 255.f);
									material->ColorModulate(config->get_color("colorChamsTeammates").r() / 255.f,
										config->get_color("colorChamsTeammates").g() / 255.f,
										config->get_color("colorChamsTeammates").b() / 255.f);
									material->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);

									g_Interfaces->modelRender->ForcedMaterialOverride(material);
									originalFn(g_Interfaces->modelRender, context, state, info, matrix);
								}
								//invisible
								if (config->get_bool("chamsTeammatesXQZ")) // teammate (behind wall)
								{
									material->AlphaModulate(config->get_color("colorChamsTeammatesXQZ").a() / 255.f);
									material->ColorModulate(config->get_color("colorChamsTeammatesXQZ").r() / 255.f,
										config->get_color("colorChamsTeammatesXQZ").g() / 255.f,
										config->get_color("colorChamsTeammatesXQZ").b() / 255.f);
									material->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, true);

									g_Interfaces->modelRender->ForcedMaterialOverride(material);
									originalFn(g_Interfaces->modelRender, context, state, info, matrix);
								}
							}
							if ( entity == Globals::localPlayer && Globals::inThirdperson && Globals::localPlayer->alive( ) )
							{
								// i'm sorry 
								bool shouldBlend = Globals::localPlayer->scoped( );
								float blendAmt = 0.25f;

								if ( Globals::localPlayer->activeWeapon( ) )
								{
									auto alpha = config->get_color("colorChamsTeammatesXQZ").b() / 255.f;
									if ( Globals::localPlayer->activeWeapon( )->zoomLevel( ) == 1 )
										blendAmt = ( alpha ) * 0.1f;
									else if ( Globals::localPlayer->activeWeapon( )->zoomLevel( ) == 2 )
										blendAmt = ( alpha ) * 0.025f;
									else
										blendAmt = 1.0f;
								}

								// Fake angle chams
						
								if (config->get_bool("chamsLocalDesync"))
								{
									Vector3 bonePos;
									Vector3 outPos;
									matrix3x4_t BoneMatrix[ 128 ];
									for ( int i = 0; i < 128; i++ )
									{
										g_Math.angleMatrix( Globals::fakeAngle, BoneMatrix[ i ] );
										g_Math.matrixMultiply( BoneMatrix[ i ], matrix[ i ] );
										bonePos = Vector3( matrix[ i ][ 0 ][ 3 ], matrix[ i ][ 1 ][ 3 ], matrix[ i ][ 2 ][ 3 ] ) - info.origin;
										g_Math.vectorRotate( bonePos, Globals::fakeAngle, outPos );
										outPos += info.origin;
										BoneMatrix[ i ][ 0 ][ 3 ] = outPos.x;
										BoneMatrix[ i ][ 1 ][ 3 ] = outPos.y;
										BoneMatrix[ i ][ 2 ][ 3 ] = outPos.z;
									}
									
									
									material->AlphaModulate(config->get_color("colorDesyncModel").a() / 255.f);
									material->ColorModulate(config->get_color("colorDesyncModel").r() / 255.f,
										config->get_color("colorDesyncModel").g() / 255.f,
										config->get_color("colorDesyncModel").b() / 255.f);

									material->SetMaterialVarFlag( MATERIAL_VAR_IGNOREZ, false );
									//Globals::localPlayer->setAbsOrigin(Globals::fakePosition);
									g_Interfaces->modelRender->ForcedMaterialOverride( material );
									originalFn( g_Interfaces->modelRender, context, state, info, BoneMatrix );
									//Globals::localPlayer->setAbsOrigin(Globals::localPlayer->GetAbsOrigin());
								}

								// Regular chams
								if (config->get_bool("chamsLocal")) {
									material->AlphaModulate( shouldBlend ? blendAmt : config->get_color("colorChamsLocal").a() / 255.f);

									material->ColorModulate(config->get_color("colorChamsLocal").r() / 255.f,
										config->get_color("colorChamsLocal").g() / 255.f,
										config->get_color("colorChamsLocal").b() / 255.f);

									material->SetMaterialVarFlag( MATERIAL_VAR_IGNOREZ, false );
									g_Interfaces->modelRender->ForcedMaterialOverride( material );
									originalFn( g_Interfaces->modelRender, context, state, info, matrix );
								}
							}
						}
						if (model_name.find("arms") != std::string::npos)
						{
							// arms
							if (config->get_bool("chamsHands"))
							{
								material->AlphaModulate(config->get_color("colorChamsHands").a() / 255.f);
								material->ColorModulate(config->get_color("colorChamsHands").r() / 255.f,
									config->get_color("colorChamsHands").g() / 255.f,
									config->get_color("colorChamsHands").b() / 255.f);

								material->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);

								g_Interfaces->modelRender->ForcedMaterialOverride(material);
								originalFn(g_Interfaces->modelRender, context, state, info, matrix);
							}
						}
						else if (model_name.find("weapon") != std::string::npos)
						{
							if (!(model_name.find("arms") != std::string::npos))
							{
								// arms
								if (config->get_bool("chamsWeapon"))
								{
									material->AlphaModulate(config->get_color("colorChamsWeapon").a() / 255.f);
									material->ColorModulate(config->get_color("colorChamsWeapon").r() / 255.f,
										config->get_color("colorChamsWeapon").g() / 255.f,
										config->get_color("colorChamsWeapon").b() / 255.f);

									material->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);

									g_Interfaces->modelRender->ForcedMaterialOverride(material);
									originalFn(g_Interfaces->modelRender, context, state, info, matrix);
								}
							}
						}
					}
				}
			}
		}
	}

	originalFn( g_Interfaces->modelRender, context, state, info, matrix );
	g_Interfaces->modelRender->ForcedMaterialOverride( nullptr );
}