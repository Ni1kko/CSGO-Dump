#pragma once
#include "../Hooks/Hooks.h"

#include "../../Include/GlobalIncludes.h"
#include "../../Interfaces/Interfaces.h"
#include "../../SDK/CUserCmd.h"
#include "../../SDK/GlobalDefenitions.h"
#include "../../SDK/CBaseEntity.h"
#include "../../SDK/ISurface.h"
#include "../../Features/Visuals/Visuals.h"
#include "../../Features/Visuals/Drawing.h"
#include "../../Features//Ragebot/AntiAims.h"
#include "../../Features/Visuals/EventLogger.h"
#include "../../Features/Ragebot/CAutoWall.h"
#include "../../Lua/CLua.h"

typedef void( __thiscall* fnPaintTraverse )( void*, unsigned int, bool, bool );
fnPaintTraverse oPaintTraverse = nullptr;

using namespace Graphics;

namespace Cheat
{
	namespace Hooked
	{
		void __fastcall PaintTraverseHook( void* thisptr, void*, unsigned int vguiPanel, bool forceRepaint, bool allowForce )
		{
			if (Cheat::Settings->UnloadCheat)
			{
				Cheat::Settings->UnloadReadyTable.PaintTraverse = 0;
				oPaintTraverse(thisptr, vguiPanel, forceRepaint, allowForce);
				return;
			}
			Cheat::Settings->UnloadReadyTable.PaintTraverse = (int)GetTickCount();

			const char* panel_name = Interfaces::GuiPanel->GetName( vguiPanel );
			Graphics::Drawing->Initialize();

			if ( !( strstr( panel_name, "HudZoom" ) && c_config::get()->b["vis_noscope"]) )
			{
				oPaintTraverse( thisptr, vguiPanel, forceRepaint, allowForce );

				if ( strstr( panel_name, "FocusOverlayPanel" ) )
				{
					Graphics::Drawing->Initialize();

					if ( Interfaces::Engine->IsInGame() && Interfaces::Engine->IsConnected() && G::LocalPlayer )
					{
						static auto svcheatsspoof = new SpoofedConvar( "sv_cheats" );
						svcheatsspoof->SetInt( 1 );
						for (int i = 0; i < 4; i++) {
							for (int j = 0; j < 4; j++) {
								G::matrix[i][j] = Interfaces::Engine->WorldToScreenMatrix()[i][j];
							}
						}

						Visuals->Run();
						static int olddpi = 0;
						if (c_config::get()->i["menu_dpiscale"] != olddpi) {
							olddpi = c_config::get()->i["menu_dpiscale"];
							Graphics::Drawing->Initialize();
						}

						if ( G::LocalPlayer && G::LocalPlayer->GetHealth() > 0 && G::LocalPlayer->IsAlive() )
						{
							auto recoilspoof = Interfaces::Var->FindVar( "cl_crosshair_recoil" );

							if ( c_config::get()->b["vis_recoiloverlay"] )
								recoilspoof->SetValue( 1 );

							else
								recoilspoof->SetValue( 0 );

							if (c_config::get()->b["vis_soundesp"])
								SoundEsp->DrawSteps();

							if (c_config::get()->b["vis_foot_trails"]){
								//SoundEsp->DrawBeamPaw(src,end, color);
								//SoundEsp->AddTrails();
							}
							//if ( c_config::get()->b["vis_bullettracer"] )
							//	BulletTracer->DrawTracers();
							if (c_config::get()->b["vis_spectators"]) {
								Visuals->RunSpectators();
							}
							if (c_config::get()->b["vis_zeus_range"]) {
								Visuals->DrawZeusRange();
							}
							//if (c_config::get()->b["rage_flip"]) {
							///	Visuals->desyncdraw();
							//}

							if ( c_config::get()->b["vis_noscope"] && G::LocalPlayer->IsScoped() )
							{
								int w, h = 0;
								Interfaces::Engine->GetScreenSize( w, h );

								int gOffy = h / 2 / 2;
								Drawing->Line(w / 2, 0, w / 2, h, CColor(0, 0, 0, 255));
								Drawing->Line(0, h / 2, w, h / 2, CColor(0, 0, 0, 255));
							}

							if (c_config::get()->b["rage_quickpeek"] && c_keyhandler::get()->auto_check("rage_quickpeek_key")) {
								if (G::quickpeekstartpos != Vector(0, 0, 0)) {
									static int screen_x_line_old;
									static int screen_y_line_old;
									for (int i = 0; i < 360; i += 1) {
										float rot_temp = DEG2RAD(i);
										int lineX = 15.f * cos(rot_temp) + G::quickpeekstartpos.x;
										int lineY = 15.f * sin(rot_temp) + G::quickpeekstartpos.y;
										Vector coords;
										auto clr = c_config::get()->c["rage_quickpeek_color"];
										if (Utilities->Game_WorldToScreen(Vector(lineX, lineY, G::quickpeekstartpos.z), coords) && screen_x_line_old && screen_y_line_old)
											Drawing->Line(coords.x, coords.y, screen_x_line_old, screen_y_line_old, CColor(clr[0], clr[1], clr[2], clr[3]));

										screen_x_line_old = coords.x;
										screen_y_line_old = coords.y;
									}
								}
							}
							
							if (c_config::get()->b["vis_penetrationreticle"])
							{
								Vector direction, EyeAng, dst, forward, angles;
								Interfaces::Engine->GetViewAngles(&EyeAng);

								Utilities->Math_AngleVectors(EyeAng, forward);
								dst = G::LocalPlayer->GetEyePosition() + (forward * 8912.f);

								static auto percent_col = [](int per) -> CColor {
									int red = per < 50 ? 255 : floorf(255 - (per * 2 - 100) * 255.f / 100.f);
									int green = per > 50 ? 255 : floorf((per * 2) * 255.f / 100.f);

									return CColor(red, green, 0);
								};

								int w, h = 0;
								Interfaces::Engine->GetScreenSize(w, h);

								auto dmg = c_autowall::get()->get_estimated_point_damage(dst);
								auto max_dmg = 50.f;

								if (c_config::get()->b["rage_automaticpenetration"])
									max_dmg = float(c_config::get()->i["rage_automaticpenetration_damage"]);
								if (max_dmg == 0.f)
									max_dmg = 50.f;

								CColor col;

								if (dmg >= 90) {
									col = CColor(0, 0, 255);
								}
								else if (dmg > 10) {
									col = CColor(0, 255, 0);
								}
								else {
									col = CColor(255, 0, 0);
								}
								if (G::LocalPlayer->GetActiveWeapon()->is_knife() == false && G::LocalPlayer->GetActiveWeapon()->is_grenade() == false) {
									Drawing->Rect(w / 2 - 1, h / 2 - 1, 1, 1, CColor(0, 0, 0, 150));
									Drawing->Rect(w / 2 - 1, h / 2 + 1, 1, 1, CColor(0, 0, 0, 150));
									Drawing->Rect(w / 2 + 1, h / 2 - 1, 1, 1, CColor(0, 0, 0, 150));
									Drawing->Rect(w / 2 + 1, h / 2 + 1, 1, 1, CColor(0, 0, 0, 150));

									Drawing->Line(w / 2 - 1, h / 2, w / 2 + 1, h / 2, col);
									Drawing->Line(w / 2, h / 2 - 1, w / 2, h / 2 + 1, col);
								}
								
							}
						}
					}

					Features::grenade_prediction::get()->Paint();
					Features::EventLogger->Draw();

					for (auto hk : c_lua::get()->hooks->getHooks("on_paint"))
						hk.func();
				}
			}
			SkinChanger::model_indexes.clear();
			SkinChanger::player_model_indexes.clear();
		}
	}
}