#include "hooks.hpp"
#include <intrin.h>  

#include "render.hpp"
#include "menu.hpp"
#include "options.hpp"
#include "helpers/input.hpp"
#include "helpers/utils.hpp"
#include "features/bhop.hpp"
#include "features/chams.hpp"
#include "features/visuals.hpp"
#include "features/glow.hpp"
#include "features/aimbot.hpp"
#include "features/nosmoke.hpp"
#include "features/backtrack.hpp"
#include "features/engine_prediction.hpp"
#include "features/grenade_prediction.hpp"
#include "features/skins.hpp"
#pragma intrinsic(_ReturnAddress)  

float side = 1.0f;

float real_angle = 0.0f;
float view_angle = 0.0f;

namespace Hooks
{
  vfunc_hook direct3d_hook;
  vfunc_hook vguipanel_hook;
  vfunc_hook vguisurf_hook;
  vfunc_hook sound_hook;
  vfunc_hook mdlrender_hook;
  vfunc_hook clientmode_hook;
  vfunc_hook sv_cheats;
  vfunc_hook gameevents_hook;
  recv_prop_hook* sequence_hook;
  vfunc_hook bsp_query_hook;
  vfunc_hook partition_hook;
  vfunc_hook prediction_hook;
  vfunc_hook clientstate_hook;

  void Initialize( ) {
	 direct3d_hook.setup( g_D3DDevice9 );
	 vguipanel_hook.setup( g_VGuiPanel );
	 vguisurf_hook.setup( g_VGuiSurface );
	 sound_hook.setup( g_EngineSound );
	 mdlrender_hook.setup( g_StudioRender );
	 clientmode_hook.setup( g_ClientMode );
	 partition_hook.setup( g_SpatialPartition );
	 clientstate_hook.setup( ( void* )( ( uintptr_t )g_ClientState + 0x8 ) );

	 ConVar* sv_cheats_con = g_CVar->FindVar( "sv_cheats" );
	 sv_cheats.setup( sv_cheats_con );

	 gameevents_hook.setup( g_GameEvents );
	 gameevents_hook.hook_index( index::FireEvent, hkFireEvent );
	 sequence_hook = new recv_prop_hook( C_BaseViewModel::m_nSequence( ), hkRecvProxy );

	 bsp_query_hook.setup( g_EngineClient->GetBSPTreeQuery( ) );

	 direct3d_hook.hook_index( index::EndScene, hkEndScene );
	 direct3d_hook.hook_index( index::Reset, hkReset );

	 vguipanel_hook.hook_index( index::PaintTraverse, hkPaintTraverse );

	 sound_hook.hook_index( index::EmitSound1, hkEmitSound1 );
	 vguisurf_hook.hook_index( index::LockCursor, hkLockCursor );

	 mdlrender_hook.hook_index( index::DrawModelExecute, hkDrawModelExecute );

	 clientmode_hook.hook_index( index::DoPostScreenSpaceEffects, hkDoPostScreenEffects );
	 clientmode_hook.hook_index( index::OverrideView, hkOverrideView );
	 clientmode_hook.hook_index( index::ClientModeCreateMove, hkCreateMove );

	 partition_hook.hook_index( index::SuppressLists, hkSuppressLists );

	 sv_cheats.hook_index( index::SvCheatsGetBool, hkSvCheatsGetBool );
	 bsp_query_hook.hook_index( index::ListLeavesInBox, hkListLeavesInBox );

	 clientstate_hook.hook_index( index::ReadPacketEntities, hkReadPacketEntities );

	 g_GameEvents->AddListener( &Visuals::Get( ), "player_hurt", false );
  }
  //--------------------------------------------------------------------------------
  void Shutdown( ) {
	 direct3d_hook.unhook_all( );
	 vguipanel_hook.unhook_all( );
	 vguisurf_hook.unhook_all( );
	 mdlrender_hook.unhook_all( );
	 clientmode_hook.unhook_all( );
	 gameevents_hook.unhook_all( );
	 sequence_hook->~recv_prop_hook( );
	 sound_hook.unhook_all( );
	 sv_cheats.unhook_all( );
	 bsp_query_hook.unhook_all( );
	 partition_hook.unhook_all( );
	 clientstate_hook.unhook_all( );

	 g_GameEvents->RemoveListener( &Visuals::Get( ) );

	 Glow::Get( ).Shutdown( );
  }
  //--------------------------------------------------------------------------------
  long __stdcall hkEndScene( IDirect3DDevice9* pDevice ) {
	 auto oEndScene = direct3d_hook.get_original< EndScene >( index::EndScene );

	 static uintptr_t gameoverlay_return_address = 0;
	 if( !gameoverlay_return_address ) {
		MEMORY_BASIC_INFORMATION info;
		VirtualQuery( _ReturnAddress( ), &info, sizeof( MEMORY_BASIC_INFORMATION ) );

		char mod[MAX_PATH];
		GetModuleFileNameA( ( HMODULE )info.AllocationBase, mod, MAX_PATH );

		if( strstr( mod, "gameoverlay" ) )
		  gameoverlay_return_address = ( uintptr_t )( _ReturnAddress( ) );
	 }

	 if( gameoverlay_return_address != ( uintptr_t )( _ReturnAddress( ) ) )
		return oEndScene( pDevice );

	 IDirect3DVertexDeclaration9* vertDec;
	 IDirect3DVertexShader9* vertShader;
	 pDevice->GetVertexDeclaration( &vertDec );
	 pDevice->GetVertexShader( &vertShader );

	 pDevice->SetVertexDeclaration( nullptr );
	 pDevice->SetVertexShader( nullptr );

	 static auto r_modelAmbientMin = g_CVar->FindVar( "r_modelAmbientMin" );
	 static auto mat_force_tonemap_scale = g_CVar->FindVar( "mat_force_tonemap_scale" );
	 static auto mat_postprocess_enable = g_CVar->FindVar( "mat_postprocess_enable" );

	 static auto mat_ambient_light_r = g_CVar->FindVar( "mat_ambient_light_r" );
	 static auto mat_ambient_light_g = g_CVar->FindVar( "mat_ambient_light_g" );
	 static auto mat_ambient_light_b = g_CVar->FindVar( "mat_ambient_light_b" );

	 if( !g_Unload ) {
		r_modelAmbientMin->SetValue( g_Options.other_nightmode ? 1.f : 0.0f );
		mat_force_tonemap_scale->SetValue( g_Options.other_nightmode ? g_Options.other_nightmode_size : 1.0f );
		mat_postprocess_enable->SetValue( g_Options.other_nightmode ? 1 : 0 );

		if( g_Options.other_mat_ambient_light_rainbow ) {

		  static float rainbow; rainbow += 0.005f; if( rainbow > 1.f ) rainbow = 0.f;
		  auto rainbow_col = Color::FromHSB( rainbow, 1.0f, 1.0f );

		  mat_ambient_light_r->SetValue( rainbow_col.r( ) / 255.0f );
		  mat_ambient_light_g->SetValue( rainbow_col.g( ) / 255.0f );
		  mat_ambient_light_b->SetValue( rainbow_col.b( ) / 255.0f );
		}

		else {
		  mat_ambient_light_r->SetValue( g_Options.other_mat_ambient_light_r );
		  mat_ambient_light_g->SetValue( g_Options.other_mat_ambient_light_g );
		  mat_ambient_light_b->SetValue( g_Options.other_mat_ambient_light_b );
		}
	 } else {
		if( r_modelAmbientMin->GetFloat( ) != 0.0f )
		  r_modelAmbientMin->SetValue( 0.0f );

		if( mat_force_tonemap_scale->GetFloat( ) != 1.0f )
		  mat_force_tonemap_scale->SetValue( 1.0f );

		if( mat_postprocess_enable->GetInt( ) != 0 )
		  mat_postprocess_enable->SetValue( 0 );

		if( mat_ambient_light_r->GetFloat( ) != 0.0f )	mat_ambient_light_r->SetValue( 0.0f );
		if( mat_ambient_light_g->GetFloat( ) != 0.0f )	mat_ambient_light_g->SetValue( 0.0f );
		if( mat_ambient_light_b->GetFloat( ) != 0.0f )	mat_ambient_light_b->SetValue( 0.0f );
	 }

	 DWORD colorwrite, srgbwrite;
	 pDevice->GetRenderState( D3DRS_COLORWRITEENABLE, &colorwrite );
	 pDevice->GetRenderState( D3DRS_SRGBWRITEENABLE, &srgbwrite );

	 pDevice->SetRenderState( D3DRS_COLORWRITEENABLE, 0xffffffff );
	 pDevice->SetRenderState( D3DRS_SRGBWRITEENABLE, false ); // removes the source engine color correction

	 pDevice->SetSamplerState( NULL, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP );
	 pDevice->SetSamplerState( NULL, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP );
	 pDevice->SetSamplerState( NULL, D3DSAMP_ADDRESSW, D3DTADDRESS_WRAP );
	 pDevice->SetSamplerState( NULL, D3DSAMP_SRGBTEXTURE, NULL );

	 static auto misc_viewmodel_fov = g_CVar->FindVar( "viewmodel_fov" );

	 misc_viewmodel_fov->m_fnChangeCallbacks.m_Size = 0;
	 misc_viewmodel_fov->SetValue( g_Options.misc_viewmodel_fov );

	 ImGui_ImplDX9_NewFrame( );
	 ImGui_ImplWin32_NewFrame( );
	 ImGui::NewFrame( );

	 auto esp_drawlist = Render::Get( ).RenderScene( );
	 Menu::Get( ).Render( );
	 ImGui::Render( esp_drawlist );
	 ImGui_ImplDX9_RenderDrawData( ImGui::GetDrawData( ) );

	 pDevice->SetRenderState( D3DRS_COLORWRITEENABLE, colorwrite );
	 pDevice->SetRenderState( D3DRS_SRGBWRITEENABLE, srgbwrite );
	 pDevice->SetVertexDeclaration( vertDec );
	 pDevice->SetVertexShader( vertShader );

	 return oEndScene( pDevice );
  }
  //--------------------------------------------------------------------------------
  long __stdcall hkReset( IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* pPresentationParameters ) {
	 auto oReset = direct3d_hook.get_original<Reset>( index::Reset );

	 Menu::Get( ).OnDeviceLost( );

	 auto hr = oReset( device, pPresentationParameters );

	 if( hr >= 0 )
		Menu::Get( ).OnDeviceReset( );

	 return hr;
  }
  //--------------------------------------------------------------------------------
  float AngleDiff( float destAngle, float srcAngle ) {
	 float delta;

	 delta = fmodf( destAngle - srcAngle, 360.0f );
	 if( destAngle > srcAngle ) {
		if( delta >= 180 )
		  delta -= 360;
	 } else {
		if( delta <= -180 )
		  delta += 360;
	 }
	 return delta;
  }

  static int max_choke_ticks = 14;
  static CCSGOPlayerAnimState g_AnimState;
  bool __stdcall hkCreateMove( float input_sample_frametime, CUserCmd* cmd ) {
	 bool* send_packet = reinterpret_cast< bool* >( reinterpret_cast< uintptr_t >( _AddressOfReturnAddress( ) ) + 0x14 );
	 auto oCreateMove = clientmode_hook.get_original< CreateMoveClientMode >( index::ClientModeCreateMove );
	 bool result = oCreateMove( g_ClientMode, input_sample_frametime, cmd );

	 if( !cmd || !cmd->command_number || g_Unload )
		return result;

	 static int latency_ticks = 0;
	 float fl_latency = g_EngineClient->GetNetChannelInfo( )->GetLatency( FLOW_OUTGOING );
	 int latency = TIME_TO_TICKS( fl_latency );;
	 if( g_ClientState->chokedcommands <= 0 ) {
		latency_ticks = latency;
	 } else {
		latency_ticks = std::max( latency, latency_ticks );
	 }

	 if( g_GameRules->m_bIsValveDS( ) ) {
		if( fl_latency >= g_GlobalVars->interval_per_tick )
		  max_choke_ticks = 11 - latency_ticks;
		else
		  max_choke_ticks = 11;
	 } else {
		max_choke_ticks = 13 - latency_ticks;
	 }

	 static float SpawnTime = 0.0f;
	 if( g_LocalPlayer->m_flSpawnTime( ) != SpawnTime ) {
		g_AnimState.pBaseEntity = g_LocalPlayer;
		g_LocalPlayer->ResetAnimationState( &g_AnimState );
		SpawnTime = g_LocalPlayer->m_flSpawnTime( );
	 }

	 NoSmoke::Get( ).OnFrameStageNotify( );

	 if( Menu::Get( ).IsVisible( ) )
		cmd->buttons &= ~( IN_ATTACK | IN_ATTACK2 );

	 if( g_Options.misc_bhop )
		BunnyHop::Get( ).OnCreateMove( cmd );

	 if( g_Options.misc_autostrafe )
		BunnyHop::Get( ).AutoStrafe( cmd, cmd->viewangles );

	 if( g_Options.other_no_flash )
		g_LocalPlayer->m_flFlashDuration( ) = 0.f;

	 static float LastChangeTime = 0.f;

	 static bool restore = false;
	 if( g_Options.misc_clantag && g_LocalPlayer ) {
		if( g_GlobalVars->realtime - LastChangeTime >= 0.5f ) {
		  static std::string text = "Amnesia";

		  LastChangeTime = g_GlobalVars->realtime;

		  std::string temp = text;
		  text.erase( 0, 1 );
		  text += temp[0];

		  Utils::SetClantag( text.data( ) );
		  restore = true;
		}
	 } else if( restore ) {
		restore = false;
		Utils::SetClantag( "" );
	 }

	 /*
	 bool do_fakeduck = g_Options.misc_fakeduck_bind && g_Options.misc_fakeduck_ticks > 1 && InputSys::Get( ).IsKeyDown( g_Options.misc_fakeduck_bind )
		&& g_LocalPlayer->m_fFlags( ) & FL_ONGROUND;
	*/

	 int old_flags = g_LocalPlayer->m_fFlags( );

	 static float next_lby = 0.0f;

	 QAngle OldAngles = cmd->viewangles;
	 auto Desync = [OldAngles] ( CUserCmd* cmd, bool* send_packet ) {
		if( cmd->buttons & ( IN_ATTACK | IN_ATTACK2 | IN_USE ) ||
		  g_LocalPlayer->m_nMoveType( ) == MOVETYPE_LADDER || g_LocalPlayer->m_nMoveType( ) == MOVETYPE_NOCLIP
		  || !g_LocalPlayer->IsAlive( ) )
		  return;

		auto weapon = g_LocalPlayer->m_hActiveWeapon( ).Get( );
		if( !weapon )
		  return;

		auto weapon_index = weapon->m_Item( ).m_iItemDefinitionIndex( );
		if( ( weapon_index == WEAPON_GLOCK || weapon_index == WEAPON_FAMAS ) && weapon->m_flNextPrimaryAttack( ) >= g_GlobalVars->curtime )
		  return;

		auto weapon_data = weapon->GetCSWeaponData( );

		if( weapon_data->WeaponType == WEAPONTYPE_GRENADE ) {
		  if( !weapon->m_bPinPulled( ) ) {
			 float throwTime = weapon->m_fThrowTime( );
			 if( throwTime > 0.f )
				return;
		  }

		  if( ( cmd->buttons & IN_ATTACK ) || ( cmd->buttons & IN_ATTACK2 ) ) {
			 if( weapon->m_fThrowTime( ) > 0.f )
				return;
		  }
		}

		static bool broke_lby = false;

		if( InputSys::Get( ).WasKeyPressed( g_Options.misc_desync_bind ) ) {
		  side = -side;
		}

		if( g_Options.misc_desync == 1 ) {

			/*
			if( do_fakeduck )
				return;
			*/

		  float minimal_move = 2.0f;
		  if( g_LocalPlayer->m_fFlags( ) & FL_DUCKING )
			 minimal_move *= 3.f;

		  if( cmd->buttons & IN_WALK )
			 minimal_move *= 3.f;

		  bool should_move = g_LocalPlayer->m_vecVelocity( ).Length2D( ) <= 0.0f
			 || std::fabsf( g_LocalPlayer->m_vecVelocity( ).z ) <= 100.0f;

		  if( ( cmd->command_number % 2 ) == 1 ) {
			 cmd->viewangles.yaw += 120.0f * side;
			 if( should_move )
				cmd->sidemove -= minimal_move;
			 *send_packet = false;
		  } else if( should_move ) {
			 cmd->sidemove += minimal_move;
		  }
		} else {
		  if( next_lby >= g_GlobalVars->curtime ) {
			 if( !broke_lby && *send_packet && g_ClientState->chokedcommands > 0 )
				return;

			 broke_lby = false;
			 *send_packet = false;
			 cmd->viewangles.yaw += 120.0f * side;
		  } else {
			 broke_lby = true;
			 *send_packet = false;
			 cmd->viewangles.yaw += 120.0f * -side;
		  }
		}

		Math::FixAngles( cmd->viewangles );
		Math::MovementFix( cmd, OldAngles, cmd->viewangles );
	 };

	 auto Fakelag = [] ( CUserCmd* cmd, bool* send_packet ) {
		if( g_Options.fakelag_unducking &&
		  g_LocalPlayer->m_flDuckAmount( ) > 0.0f && g_LocalPlayer->m_flDuckAmount( ) < 1.0f ) {
		  *send_packet = !( g_ClientState->chokedcommands < max_choke_ticks );
		  return;
		}

		if( g_Options.fakelag_factor <= 0 )
		  return;

		int choke_factor = g_Options.misc_desync ? std::min( max_choke_ticks, g_Options.fakelag_factor ) : g_Options.fakelag_factor;

		auto LegitPeek = [choke_factor] ( CUserCmd* cmd, bool* send_packet ) {
		  static bool m_bIsPeeking = false;
		  if( m_bIsPeeking ) {
			 *send_packet = !( g_ClientState->chokedcommands < choke_factor );
			 if( *send_packet )
				m_bIsPeeking = false;
			 return;
		  }

		  auto speed = g_LocalPlayer->m_vecVelocity( ).Length( );
		  if( speed <= 100.0f )
			 return;

		  auto collidable = g_LocalPlayer->GetCollideable( );

		  Vector min, max;
		  min = collidable->OBBMins( );
		  max = collidable->OBBMaxs( );

		  min += g_LocalPlayer->m_vecOrigin( );
		  max += g_LocalPlayer->m_vecOrigin( );

		  Vector center = ( min + max ) * 0.5f;

		  for( int i = 1; i <= g_GlobalVars->maxClients; ++i ) {
			 auto player = C_BasePlayer::GetPlayerByIndex( i );
			 if( !player || !player->IsAlive( ) || player->IsDormant( ) )
				continue;
			 if( player == g_LocalPlayer || g_LocalPlayer->m_iTeamNum( ) == player->m_iTeamNum( ) )
				continue;

			 auto weapon = player->m_hActiveWeapon( ).Get( );
			 if( !weapon || weapon->m_iClip1( ) <= 0 )
				continue;

			 auto weapon_data = weapon->GetCSWeaponData( );
			 if( !weapon_data || weapon_data->WeaponType <= WEAPONTYPE_KNIFE || weapon_data->WeaponType >= WEAPONTYPE_C4 )
				continue;

			 auto eye_pos = player->GetEyePos( );

			 Vector direction;
			 Math::AngleVectors( player->m_angEyeAngles( ), direction );
			 direction.NormalizeInPlace( );

			 Vector hit_point;
			 bool hit = Math::IntersectionBoundingBox( eye_pos, direction, min, max, &hit_point );
			 if( hit && eye_pos.DistTo( hit_point ) <= weapon_data->flRange ) {
				Ray_t ray;
				trace_t tr;
				CTraceFilterSkipEntity filter( ( C_BasePlayer* )player );
				ray.Init( eye_pos, hit_point );

				g_EngineTrace->TraceRay( ray, MASK_SHOT_HULL | CONTENTS_HITBOX, &filter, &tr );
				if( tr.contents & CONTENTS_WINDOW ) { // skip windows
																							// at this moment, we dont care about local player
				  filter.pSkip = tr.hit_entity;
				  ray.Init( tr.endpos, hit_point );
				  g_EngineTrace->TraceRay( ray, MASK_SHOT_HULL | CONTENTS_HITBOX, &filter, &tr );
				}

				if( tr.fraction == 1.0f || tr.hit_entity == g_LocalPlayer ) {
				  m_bIsPeeking = true;
				  break;
				}
			 }
		  }
		};

		auto speed = g_LocalPlayer->m_vecVelocity( ).Length( );
		bool standing = speed <= 1.0f;
		if( !g_Options.fakelag_standing && standing ) {
		  return;
		}

		if( !g_Options.fakelag_moving && !standing ) {
		  return;
		}

		enum FakelagMode {
		  FakelagStatic = 0,
		  FakelagSwitch,
		  FakelagAdaptive,
		  FakelagRandom,
		  FakelagLegitPeek
		};

		float UnitsPerTick = 0.0f;

		int WishTicks = 0;
		int AdaptiveTicks = 2;
		static int LastRandomNumber = 5;
		static int randomSeed = 12345;

		switch( g_Options.fakelag_mode ) {
		case FakelagSwitch:
			// apply same logic as static fakelag
		  if( cmd->command_number % 30 > 15 )
			 break;
		case FakelagStatic:
		  *send_packet = !( g_ClientState->chokedcommands < choke_factor );
		  break;
		case FakelagAdaptive:
		  if( standing ) {
			 *send_packet = !( g_ClientState->chokedcommands < choke_factor );
			 break;
		  }

		  UnitsPerTick = g_LocalPlayer->m_vecVelocity( ).Length( ) * g_GlobalVars->interval_per_tick;
		  while( ( WishTicks * UnitsPerTick ) <= 68.0f ) {
			 if( ( ( AdaptiveTicks - 1 ) * UnitsPerTick ) > 68.0f ) {
				++WishTicks;
				break;
			 }
			 if( ( AdaptiveTicks * UnitsPerTick ) > 68.0f ) {
				WishTicks += 2;
				break;
			 }
			 if( ( ( AdaptiveTicks + 1 ) * UnitsPerTick ) > 68.0f ) {
				WishTicks += 3;
				break;
			 }
			 if( ( ( AdaptiveTicks + 2 ) * UnitsPerTick ) > 68.0f ) {
				WishTicks += 4;
				break;
			 }
			 AdaptiveTicks += 5;
			 WishTicks += 5;
			 if( AdaptiveTicks > 16 )
				break;
		  }

		  *send_packet = !( g_ClientState->chokedcommands < WishTicks );
		  break;
		case FakelagRandom:
		  if( g_ClientState->chokedcommands < LastRandomNumber ) {
			 *send_packet = false;
		  } else {
			 randomSeed = 0x41C64E6D * randomSeed + 12345;
			 LastRandomNumber = ( randomSeed / 0x10000 & 0x7FFFu ) % choke_factor;
			 *send_packet = true;
		  }
		  break;
		case FakelagLegitPeek:
		  LegitPeek( cmd, send_packet );
		  break;
		}
	 };

	 /*
	 if( do_fakeduck ) {
		int fakelag_limit = g_Options.misc_desync ? std::min( max_choke_ticks, g_Options.misc_fakeduck_ticks ) : g_Options.misc_fakeduck_ticks;
		int choked_goal = fakelag_limit / 2;
		bool should_crouch = g_ClientState->chokedcommands >= choked_goal;

		*send_packet = !( g_ClientState->chokedcommands < fakelag_limit );
		int choke_delta = choked_goal - g_ClientState->chokedcommands;

		cmd->buttons |= IN_BULLRUSH;
		if( should_crouch )
		  cmd->buttons |= IN_DUCK;
		else
		  cmd->buttons &= ~IN_DUCK;
	 }
	 */

	 if( g_Options.misc_slowwalk_bind && InputSys::Get( ).IsKeyDown( g_Options.misc_slowwalk_bind ) ) {
		float speed = g_Options.misc_slowwalk_speed * 0.01f;
		auto weapon = g_LocalPlayer->m_hActiveWeapon( ).Get( );
		if( weapon ) {
		  auto weapon_data = weapon->GetCSWeaponData( );
		  if( weapon_data ) {
			 float max_speed = weapon->m_weaponMode( ) == 0 ? weapon_data->flMaxPlayerSpeed : weapon_data->flMaxPlayerSpeedAlt;
			 float ratio = max_speed / 250.0f;
			 speed *= ratio;
		  }
		}

		cmd->forwardmove *= speed;
		cmd->sidemove *= speed;
	 }

	 EnginePrediction::Begin( cmd );
	 {

		/*
		if( !do_fakeduck && g_Options.fakelag_enabled ) {
		  Fakelag( cmd, send_packet );
		}

		if( g_Options.misc_block_ducked_attack && do_fakeduck && g_LocalPlayer->m_flDuckAmount( ) >= 0.1f && cmd->buttons & ( IN_ATTACK ) ) {
		  auto weapon = g_LocalPlayer->m_hActiveWeapon( ).Get( );
		  if( weapon ) {
			 auto weapon_data = weapon->GetCSWeaponData( );
			 if( weapon_data && weapon_data->WeaponType != WEAPONTYPE_GRENADE && weapon_data->WeaponType != WEAPONTYPE_C4 && weapon_data->WeaponType != WEAPONTYPE_KNIFE )
				cmd->buttons &= ~( IN_ATTACK );
		  }
		}
		*/

		if( g_Options.misc_edgejump && ( old_flags & FL_ONGROUND ) && !( g_LocalPlayer->m_fFlags( ) & FL_ONGROUND ) )
		  cmd->buttons |= IN_JUMP;

		g_Aimbot.OnMove( cmd );
		g_Backtrack.OnMove( cmd );

		if( g_Options.esp_grenade_prediction )
		  GrenadePrediction::trace( cmd );

		if( g_Options.misc_desync && std::fabsf( g_LocalPlayer->m_flSpawnTime( ) - g_GlobalVars->curtime ) > 1.0f )
		  Desync( cmd, send_packet );

		Math::FixAngles( cmd->viewangles );
		cmd->viewangles.yaw = std::remainderf( cmd->viewangles.yaw, 360.0f );

		if( g_Options.misc_desync && g_ClientState->chokedcommands >= max_choke_ticks ) {
		  *send_packet = true;
		  cmd->viewangles = g_ClientState->viewangles;
		}

		// from aimware dmp
		static ConVar* m_yaw = m_yaw = g_CVar->FindVar( "m_yaw" );
		static ConVar* m_pitch = m_pitch = g_CVar->FindVar( "m_pitch" );
		static ConVar* sensitivity = sensitivity = g_CVar->FindVar( "sensitivity" );

		static QAngle m_angOldViewangles = g_ClientState->viewangles;

		float delta_x = std::remainderf( cmd->viewangles.pitch - m_angOldViewangles.pitch, 360.0f );
		float delta_y = std::remainderf( cmd->viewangles.yaw - m_angOldViewangles.yaw, 360.0f );

		if( delta_x != 0.0f ) {
		  float mouse_y = -( ( delta_x / m_pitch->GetFloat( ) ) / sensitivity->GetFloat( ) );
		  short mousedy;
		  if( mouse_y <= 32767.0f ) {
			 if( mouse_y >= -32768.0f ) {
				if( mouse_y >= 1.0f || mouse_y < 0.0f ) {
				  if( mouse_y <= -1.0f || mouse_y > 0.0f )
					 mousedy = static_cast< short >( mouse_y );
				  else
					 mousedy = -1;
				} else {
				  mousedy = 1;
				}
			 } else {
				mousedy = 0x8000u;
			 }
		  } else {
			 mousedy = 0x7FFF;
		  }

		  cmd->mousedy = mousedy;
		}

		if( delta_y != 0.0f ) {
		  float mouse_x = -( ( delta_y / m_yaw->GetFloat( ) ) / sensitivity->GetFloat( ) );
		  short mousedx;
		  if( mouse_x <= 32767.0f ) {
			 if( mouse_x >= -32768.0f ) {
				if( mouse_x >= 1.0f || mouse_x < 0.0f ) {
				  if( mouse_x <= -1.0f || mouse_x > 0.0f )
					 mousedx = static_cast< short >( mouse_x );
				  else
					 mousedx = -1;
				} else {
				  mousedx = 1;
				}
			 } else {
				mousedx = 0x8000u;
			 }
		  } else {
			 mousedx = 0x7FFF;
		  }

		  cmd->mousedx = mousedx;
		}

		auto anim_state = g_LocalPlayer->GetPlayerAnimState( );
		if( anim_state ) {
		  CCSGOPlayerAnimState anim_state_backup = *anim_state;
		  *anim_state = g_AnimState;
		  *g_LocalPlayer->GetVAngles( ) = cmd->viewangles;
		  g_LocalPlayer->UpdateClientSideAnimation( );

		  if( anim_state->speed_2d > 0.1f || std::fabsf( anim_state->flUpVelocity ) ) {
			 next_lby = g_GlobalVars->curtime + 0.22f;
		  } else if( g_GlobalVars->curtime > next_lby ) {
			 if( std::fabsf( AngleDiff( anim_state->m_flGoalFeetYaw, anim_state->m_flEyeYaw ) ) > 35.0f ) {
				next_lby = g_GlobalVars->curtime + 1.1f;
			 }
		  }

		  g_AnimState = *anim_state;
		  *anim_state = anim_state_backup;
		}

		if( *send_packet ) {
		  real_angle = g_AnimState.m_flGoalFeetYaw;
		  view_angle = g_AnimState.m_flEyeYaw;
		}
	 }
	 EnginePrediction::End( );

	 return false;
  }
  //--------------------------------------------------------------------------------
  void __stdcall hkPaintTraverse( vgui::VPANEL panel, bool forceRepaint, bool allowForce ) {
	 static auto panelId = vgui::VPANEL{ 0 };
	 static auto oPaintTraverse = vguipanel_hook.get_original<PaintTraverse>( index::PaintTraverse );

	 oPaintTraverse( g_VGuiPanel, panel, forceRepaint, allowForce );

	 if( !panelId ) {
		const auto panelName = g_VGuiPanel->GetName( panel );
		if( !strcmp( panelName, "FocusOverlayPanel" ) ) {
		  panelId = panel;
		}
	 } else if( panelId == panel ) {
		 //Ignore 50% cuz it called very often
		static bool bSkip = false;
		bSkip = !bSkip;

		if( bSkip )
		  return;

		if( g_LocalPlayer && InputSys::Get( ).IsKeyDown( VK_TAB ) && g_Options.misc_showranks )
		  Utils::RankRevealAll( );

		Render::Get( ).BeginScene( );
	 }
  }
  //--------------------------------------------------------------------------------
  void __stdcall hkEmitSound1( IRecipientFilter& filter, int iEntIndex, int iChannel, const char* pSoundEntry, unsigned int nSoundEntryHash, const char *pSample, float flVolume, int nSeed, float flAttenuation, int iFlags, int iPitch, const Vector* pOrigin, const Vector* pDirection, void* pUtlVecOrigins, bool bUpdatePositions, float soundtime, int speakerentity, int unk ) {
	 static auto ofunc = sound_hook.get_original<EmitSound1>( index::EmitSound1 );

	 if( !strcmp( pSoundEntry, "UIPanorama.popup_accept_match_beep" ) ) {
		static auto fnAccept = reinterpret_cast< bool( __stdcall* )( const char* ) >( Utils::PatternScan( GetModuleHandleA( "client_panorama.dll" ), "55 8B EC 83 E4 F8 8B 4D 08 BA ? ? ? ? E8 ? ? ? ? 85 C0 75 12" ) );

		if( fnAccept ) {

		  fnAccept( "" );

		  //This will flash the CSGO window on the taskbar
		  //so we know a game was found (you cant hear the beep sometimes cause it auto-accepts too fast)
		  FLASHWINFO fi;
		  fi.cbSize = sizeof( FLASHWINFO );
		  fi.hwnd = InputSys::Get( ).GetMainWindow( );
		  fi.dwFlags = FLASHW_ALL | FLASHW_TIMERNOFG;
		  fi.uCount = 0;
		  fi.dwTimeout = 0;
		  FlashWindowEx( &fi );
		}
	 }

	 ofunc( g_EngineSound, filter, iEntIndex, iChannel, pSoundEntry, nSoundEntryHash, pSample, flVolume, nSeed, flAttenuation, iFlags, iPitch, pOrigin, pDirection, pUtlVecOrigins, bUpdatePositions, soundtime, speakerentity, unk );

  }
  //--------------------------------------------------------------------------------
  int __stdcall hkDoPostScreenEffects( int a1 ) {
	 auto oDoPostScreenEffects = clientmode_hook.get_original<DoPostScreenEffects>( index::DoPostScreenSpaceEffects );

	 if( g_LocalPlayer && g_Options.glow_enabled )
		Glow::Get( ).Run( );

	 return oDoPostScreenEffects( g_ClientMode, a1 );
  }
  //--------------------------------------------------------------------------------
  void __stdcall hkOverrideView( CViewSetup* vsView ) {
	 static auto ofunc = clientmode_hook.get_original<OverrideView>( index::OverrideView );

	 /*
	 if( g_LocalPlayer && g_LocalPlayer->IsAlive( ) ) {
		bool do_fakeduck = g_Options.misc_fakeduck_bind && g_Options.misc_fakeduck_ticks > 1 && InputSys::Get( ).IsKeyDown( g_Options.misc_fakeduck_bind )
		  && g_LocalPlayer->m_fFlags( ) & FL_ONGROUND;

		if( do_fakeduck ) {
		  vsView->origin.z = g_LocalPlayer->m_vecOrigin( ).z + 64.0f;
		}
	 }
	 */

	 if( g_EngineClient->IsInGame( ) && vsView ) {
		if( g_Options.misc_thirdperson_bind && InputSys::Get( ).WasKeyPressed( g_Options.misc_thirdperson_bind ) )
		  g_Options.misc_thirdperson = !g_Options.misc_thirdperson;

		Visuals::Get( ).ThirdPerson( );
	 }

	 if( g_LocalPlayer && g_EngineClient->IsInGame( ) && !g_LocalPlayer->m_bIsScoped( ) ) {
		vsView->fov = g_Options.misc_override_fov;
	 }

	 ofunc( g_ClientMode, vsView );
  }
  //--------------------------------------------------------------------------------
  void __stdcall hkLockCursor( ) {
	 static auto ofunc = vguisurf_hook.get_original<LockCursor_t>( index::LockCursor );

	 if( Menu::Get( ).IsVisible( ) ) {
		g_VGuiSurface->UnlockCursor( );
		g_InputSystem->ResetInputState( );
		return;
	 }
	 ofunc( g_VGuiSurface );
  }
  //--------------------------------------------------------------------------------
  void __fastcall hkDrawModelExecute( void* pEcx, void* pEdx, void* pResults, DrawModelInfo_t* pInfo, matrix3x4_t* pBoneToWorld, float* flpFlexWeights, float* flpFlexDelayedWeights, Vector& vrModelOrigin, int32_t iFlags ) {
	 static auto ofunc = mdlrender_hook.get_original<DrawModelExecute>( index::DrawModelExecute );
	 bool forced_mat = !g_MdlRender->IsForcedMaterialOverride( );
	 if( forced_mat )
		Chams::Get( ).OnDrawModelExecute( pResults, pInfo, pBoneToWorld, flpFlexWeights, flpFlexDelayedWeights, vrModelOrigin, iFlags );

	 ofunc( pEcx, pResults, pInfo, pBoneToWorld, flpFlexWeights, flpFlexDelayedWeights, vrModelOrigin, iFlags );

	 if( forced_mat )
		g_MdlRender->ForcedMaterialOverride( nullptr );
  }
  //--------------------------------------------------------------------------------
  auto dwCAM_Think = Utils::PatternScan( GetModuleHandleW( L"client_panorama.dll" ), "85 C0 75 30 38 86" );
  typedef bool( __thiscall *svc_get_bool_t )( PVOID );
  bool __fastcall hkSvCheatsGetBool( PVOID pConVar, void* edx ) {
	 static auto ofunc = sv_cheats.get_original<svc_get_bool_t>( 13 );
	 if( !ofunc )
		return false;

	 if( reinterpret_cast< DWORD >( _ReturnAddress( ) ) == reinterpret_cast< DWORD >( dwCAM_Think ) )
		return true;
	 return ofunc( pConVar );
  }
  //--------------------------------------------------------------------------------
  bool __stdcall hkFireEvent( IGameEvent* pEvent ) {
	 static auto oFireEvent = gameevents_hook.get_original<FireEvent>( index::FireEvent );
	 if( !strcmp( pEvent->GetName( ), "player_death" ) && g_EngineClient->GetPlayerForUserID( pEvent->GetInt( "attacker" ) ) == g_EngineClient->GetLocalPlayer( ) ) {
		auto& weapon = g_LocalPlayer->m_hActiveWeapon( );
		if( weapon && weapon->IsWeapon( ) ) {
		  auto& skin_data = g_Options.skins.m_items[weapon->m_Item( ).m_iItemDefinitionIndex( )];
		  if( skin_data.enabled && skin_data.stat_trak ) {
			 skin_data.stat_trak++;
			 weapon->m_nFallbackStatTrak( ) = skin_data.stat_trak;
			 weapon->GetClientNetworkable( )->PostDataUpdate( 0 );
			 weapon->GetClientNetworkable( )->OnDataChanged( 0 );
		  }
		}
		const auto icon_override = g_Options.skins.get_icon_override( pEvent->GetString( "weapon" ) );
		if( icon_override ) {
		  pEvent->SetString( "weapon", icon_override );
		}
	 }
	 return oFireEvent( g_GameEvents, pEvent );
  }
  //--------------------------------------------------------------------------------
  static auto random_sequence( const int low, const int high ) -> int {
	 return rand( ) % ( high - low + 1 ) + low;
  }

  static auto fix_animation( const char* model, const int sequence ) -> int {
	 enum ESequence
	 {
		SEQUENCE_DEFAULT_DRAW = 0,
		SEQUENCE_DEFAULT_IDLE1 = 1,
		SEQUENCE_DEFAULT_IDLE2 = 2,
		SEQUENCE_DEFAULT_LIGHT_MISS1 = 3,
		SEQUENCE_DEFAULT_LIGHT_MISS2 = 4,
		SEQUENCE_DEFAULT_HEAVY_MISS1 = 9,
		SEQUENCE_DEFAULT_HEAVY_HIT1 = 10,
		SEQUENCE_DEFAULT_HEAVY_BACKSTAB = 11,
		SEQUENCE_DEFAULT_LOOKAT01 = 12,
		SEQUENCE_BUTTERFLY_DRAW = 0,
		SEQUENCE_BUTTERFLY_DRAW2 = 1,
		SEQUENCE_BUTTERFLY_LOOKAT01 = 13,
		SEQUENCE_BUTTERFLY_LOOKAT03 = 15,
		SEQUENCE_FALCHION_IDLE1 = 1,
		SEQUENCE_FALCHION_HEAVY_MISS1 = 8,
		SEQUENCE_FALCHION_HEAVY_MISS1_NOFLIP = 9,
		SEQUENCE_FALCHION_LOOKAT01 = 12,
		SEQUENCE_FALCHION_LOOKAT02 = 13,
		SEQUENCE_DAGGERS_IDLE1 = 1,
		SEQUENCE_DAGGERS_LIGHT_MISS1 = 2,
		SEQUENCE_DAGGERS_LIGHT_MISS5 = 6,
		SEQUENCE_DAGGERS_HEAVY_MISS2 = 11,
		SEQUENCE_DAGGERS_HEAVY_MISS1 = 12,
		SEQUENCE_BOWIE_IDLE1 = 1,
	 };
	 if( strstr( model, "models/weapons/v_knife_butterfly.mdl" ) ) {
		switch( sequence ) {
		case SEQUENCE_DEFAULT_DRAW:
		  return random_sequence( SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2 );
		case SEQUENCE_DEFAULT_LOOKAT01:
		  return random_sequence( SEQUENCE_BUTTERFLY_LOOKAT01, SEQUENCE_BUTTERFLY_LOOKAT03 );
		default:
		  return sequence + 1;
		}
	 } else if( strstr( model, "models/weapons/v_knife_falchion_advanced.mdl" ) ) {
		switch( sequence ) {
		case SEQUENCE_DEFAULT_IDLE2:
		  return SEQUENCE_FALCHION_IDLE1;
		case SEQUENCE_DEFAULT_HEAVY_MISS1:
		  return random_sequence( SEQUENCE_FALCHION_HEAVY_MISS1, SEQUENCE_FALCHION_HEAVY_MISS1_NOFLIP );
		case SEQUENCE_DEFAULT_LOOKAT01:
		  return random_sequence( SEQUENCE_FALCHION_LOOKAT01, SEQUENCE_FALCHION_LOOKAT02 );
		case SEQUENCE_DEFAULT_DRAW:
		case SEQUENCE_DEFAULT_IDLE1:
		  return sequence;
		default:
		  return sequence - 1;
		}
	 } else if( strstr( model, "models/weapons/v_knife_push.mdl" ) ) {
		switch( sequence ) {
		case SEQUENCE_DEFAULT_IDLE2:
		  return SEQUENCE_DAGGERS_IDLE1;
		case SEQUENCE_DEFAULT_LIGHT_MISS1:
		case SEQUENCE_DEFAULT_LIGHT_MISS2:
		  return random_sequence( SEQUENCE_DAGGERS_LIGHT_MISS1, SEQUENCE_DAGGERS_LIGHT_MISS5 );
		case SEQUENCE_DEFAULT_HEAVY_MISS1:
		  return random_sequence( SEQUENCE_DAGGERS_HEAVY_MISS2, SEQUENCE_DAGGERS_HEAVY_MISS1 );
		case SEQUENCE_DEFAULT_HEAVY_HIT1:
		case SEQUENCE_DEFAULT_HEAVY_BACKSTAB:
		case SEQUENCE_DEFAULT_LOOKAT01:
		  return sequence + 3;
		case SEQUENCE_DEFAULT_DRAW:
		case SEQUENCE_DEFAULT_IDLE1:
		  return sequence;
		default:
		  return sequence + 2;
		}
	 } else if( strstr( model, "models/weapons/v_knife_survival_bowie.mdl" ) ) {
		switch( sequence ) {
		case SEQUENCE_DEFAULT_DRAW:
		case SEQUENCE_DEFAULT_IDLE1:
		  return sequence;
		case SEQUENCE_DEFAULT_IDLE2:
		  return SEQUENCE_BOWIE_IDLE1;
		default:
		  return sequence - 1;
		}
	 } else {
		return sequence;
	 }
  }

  void hkRecvProxy( const CRecvProxyData* pData, void* entity, void* output ) {
	 static auto original_fn = sequence_hook->get_original_function( );
	 const auto local = static_cast< C_BasePlayer* >( g_EntityList->GetClientEntity( g_EngineClient->GetLocalPlayer( ) ) );
	 if( local && local->IsAlive( ) ) {
		const auto proxy_data = const_cast< CRecvProxyData* >( pData );
		const auto view_model = static_cast< C_BaseViewModel* >( entity );
		if( view_model && view_model->m_hOwner( ) && view_model->m_hOwner( ).IsValid( ) ) {
		  const auto owner = static_cast< C_BasePlayer* >( g_EntityList->GetClientEntityFromHandle( view_model->m_hOwner( ) ) );
		  if( owner == g_EntityList->GetClientEntity( g_EngineClient->GetLocalPlayer( ) ) ) {
			 const auto view_model_weapon_handle = view_model->m_hWeapon( );
			 if( view_model_weapon_handle.IsValid( ) ) {
				const auto view_model_weapon = static_cast< C_BaseAttributableItem* >( g_EntityList->GetClientEntityFromHandle( view_model_weapon_handle ) );
				if( view_model_weapon ) {
				  if( k_weapon_info.count( view_model_weapon->m_Item( ).m_iItemDefinitionIndex( ) ) ) {
					 auto original_sequence = proxy_data->m_Value.m_Int;
					 const auto override_model = k_weapon_info.at( view_model_weapon->m_Item( ).m_iItemDefinitionIndex( ) ).model;
					 proxy_data->m_Value.m_Int = fix_animation( override_model, proxy_data->m_Value.m_Int );
				  }
				}
			 }
		  }
		}
	 }
	 original_fn( pData, entity, output );
  }
  //--------------------------------------------------------------------------------
  struct RenderableInfo_t {
	 IClientRenderable* m_pRenderable;
	 void* m_pAlphaProperty;
	 int m_EnumCount;
	 int m_nRenderFrame;
	 unsigned short m_FirstShadow;
	 unsigned short m_LeafList;
	 short m_Area;
	 uint16_t m_Flags;   // 0x0016
	 uint16_t m_Flags2; // 0x0018
	 Vector m_vecBloatedAbsMins;
	 Vector m_vecBloatedAbsMaxs;
	 Vector m_vecAbsMins;
	 Vector m_vecAbsMaxs;
	 int pad;
  };

#define MAX_COORD_FLOAT ( 16384.0f )
#define MIN_COORD_FLOAT ( -MAX_COORD_FLOAT )
  int __fastcall hkListLeavesInBox( void* bsp, void* edx, Vector& mins, Vector& maxs, unsigned short* pList, int listMax ) {
	 typedef int( __thiscall * ListLeavesInBox )( void*, const Vector&, const Vector&, unsigned short*, int );
	 static auto ofunc = bsp_query_hook.get_original< ListLeavesInBox >( index::ListLeavesInBox );

	 // occulusion getting updated on player movement/angle change,
	 // in RecomputeRenderableLeaves ( https://github.com/pmrowla/hl2sdk-csgo/blob/master/game/client/clientleafsystem.cpp#L674 );
	 // check for return in CClientLeafSystem::InsertIntoTree
	 if( !g_Options.chams_player_enabled || !g_Options.chams_disable_occulusion || *( uint32_t* )_ReturnAddress( ) != 0x8B087D8B )
		return ofunc( bsp, mins, maxs, pList, listMax );

	// get current renderable info from stack ( https://github.com/pmrowla/hl2sdk-csgo/blob/master/game/client/clientleafsystem.cpp#L1470 )
	 auto info = *( RenderableInfo_t** )( ( uintptr_t )_AddressOfReturnAddress( ) + 0x14 );
	 if( !info || !info->m_pRenderable )
		return ofunc( bsp, mins, maxs, pList, listMax );

	// check if disabling occulusion for players ( https://github.com/pmrowla/hl2sdk-csgo/blob/master/game/client/clientleafsystem.cpp#L1491 )
	 auto base_entity = info->m_pRenderable->GetIClientUnknown( )->GetBaseEntity( );
	 if( !base_entity || !base_entity->IsPlayer( ) )
		return ofunc( bsp, mins, maxs, pList, listMax );

	// extend world space bounds to maximum ( https://github.com/pmrowla/hl2sdk-csgo/blob/master/game/client/clientleafsystem.cpp#L707 )
	 static const Vector map_min = Vector( MIN_COORD_FLOAT, MIN_COORD_FLOAT, MIN_COORD_FLOAT );
	 static const Vector map_max = Vector( MAX_COORD_FLOAT, MAX_COORD_FLOAT, MAX_COORD_FLOAT );
	 auto count = ofunc( bsp, map_min, map_max, pList, listMax );
	 return count;
  }
  //--------------------------------------------------------------------------------
  void __stdcall hkSuppressLists( int a2, bool a3 ) {
	 static auto ofunc = partition_hook.get_original< SuppressLists >( index::SuppressLists );

	 static auto OnRenderStart_Return = Utils::PatternScan( GetModuleHandleA( "client_panorama.dll" ), "FF 50 40 8B 1D ? ? ? ?" ) + 0x3;
	 static auto FrameNetUpdateEnd_Return = Utils::PatternScan( GetModuleHandleA( "client_panorama.dll" ), "5F 5E 5D C2 04 00 83 3D ? ? ? ? ?" );

	 if( g_LocalPlayer && g_LocalPlayer->IsAlive( ) ) {
		if( _ReturnAddress( ) == OnRenderStart_Return ) {
		  static auto set_abs_angles = Utils::PatternScan( GetModuleHandleA( "client_panorama.dll" ), "55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1 E8" );
		  reinterpret_cast< void( __thiscall* )( void*, const QAngle& ) >( set_abs_angles )( g_LocalPlayer, QAngle( 0.0f, g_AnimState.m_flGoalFeetYaw, 0.0f ) );
		} else if( _ReturnAddress( ) == FrameNetUpdateEnd_Return ) {
		  Skins::OnFrameStageNotify( true );
		}
	 }

	 ofunc( g_SpatialPartition, a2, a3 );
  }
  void __fastcall hkPostEntityPacketReceived( void * pPrediction, void * pEDX ) {
	 static auto original = prediction_hook.get_original<PostEntityPacketReceived>( index::PostEntityPacketReceived );

	 //FRAME_NET_UPDATE_POSTDATAUPDATE_END
	 static auto FramePostDataUpdateEnd = Utils::PatternScan( GetModuleHandleA( "client_panorama.dll" ), "FF 50 14 5F C6 05 ? ? ? ? ?" ) + 0x3;

	 //if( _ReturnAddress( ) == FramePostDataUpdateEnd )
	 //	Skins::OnFrameStageNotify( false );

	 original( pPrediction );
  }
  void __fastcall hkReadPacketEntities( void * pClientState, void * pEDX, void * pEntInfo ) {
	 static auto original = clientstate_hook.get_original<ReadPacketEntities>( index::ReadPacketEntities );

	 original( pClientState, pEntInfo );
	 Skins::OnFrameStageNotify( false );
  }
  //--------------------------------------------------------------------------------
}
