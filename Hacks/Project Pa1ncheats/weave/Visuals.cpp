#include "Visuals.h"
#include "Autowall.h"
#include "Resolver.h"
#include "FakelagGraph.h"
#include "AntiAims.h"
#include "Ragebot.h"
#include "render.h"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/imgui_impl_dx9.h"




struct custom_cmd
{
	float			viewAngles;
	float			forwardmove;
	float			sidemove;
	float			upmove;
};

bool GetBox(IBasePlayer* entity, int& x, int& y, int& w, int& h, Vector origin)
{
	if (entity->GetClientClass() && entity->GetClientClass()->m_ClassID == ClassId->CCSPlayer) {
		auto min = entity->GetCollideable()->OBBMins();
		auto max = entity->GetCollideable()->OBBMaxs();

		Vector dir, vF, vR, vU;

		interfaces.engine->GetViewAngles(dir);
		dir.x = 0;
		dir.z = 0;
		//dir.Normalize();
		//printf("%.1f\n", dir.y);
		Math::AngleVectors(dir, &vF, &vR, &vU);

		auto zh = vU * max.z + vF * max.y + vR * min.x; // = Front left front
		auto e = vU * max.z + vF * max.y + vR * max.x; //  = Front right front
		auto d = vU * max.z + vF * min.y + vR * min.x; //  = Front left back
		auto c = vU * max.z + vF * min.y + vR * max.x; //  = Front right back

		auto g = vU * min.z + vF * max.y + vR * min.x; //  = Bottom left front
		auto f = vU * min.z + vF * max.y + vR * max.x; //  = Bottom right front
		auto a = vU * min.z + vF * min.y + vR * min.x; //  = Bottom left back
		auto b = vU * min.z + vF * min.y + vR * max.x; //  = Bottom right back*-

		Vector pointList[] = {
			a,
			b,
			c,
			d,
			e,
			f,
			g,
			zh,
		};

		Vector transformed[ARRAYSIZE(pointList)];

		for (int i = 0; i < ARRAYSIZE(pointList); i++)
		{
			pointList[i] += origin;

			if (!Math::WorldToScreen2(pointList[i], transformed[i]))
				return false;
		}

		float left = FLT_MAX;
		float top = -FLT_MAX;
		float right = -FLT_MAX;
		float bottom = FLT_MAX;
		for (int i = 0; i < ARRAYSIZE(pointList); i++) {
			if (left > transformed[i].x)
				left = transformed[i].x;
			if (top < transformed[i].y)
				top = transformed[i].y;
			if (right < transformed[i].x)
				right = transformed[i].x;
			if (bottom > transformed[i].y)
				bottom = transformed[i].y;
		}

		x = left;
		y = bottom;
		w = right - left;
		h = top - bottom;

		return true;
	}
	else
	{
		Vector vOrigin, min, max, flb, brt, blb, frt, frb, brb, blt, flt;
		//float left, top, right, bottom;

		auto collideable = entity->GetCollideable();

		if (!collideable)
			return false;

		min = collideable->OBBMins();
		max = collideable->OBBMaxs();

		auto& trans = entity->GetrgflCoordinateFrame();

		Vector points[] =
		{
			Vector(min.x, min.y, min.z),
			Vector(min.x, max.y, min.z),
			Vector(max.x, max.y, min.z),
			Vector(max.x, min.y, min.z),
			Vector(max.x, max.y, max.z),
			Vector(min.x, max.y, max.z),
			Vector(min.x, min.y, max.z),
			Vector(max.x, min.y, max.z)
		};

		Vector pointsTransformed[8];
		for (int i = 0; i < 8; i++) {
			Math::VectorTransform(points[i], trans, pointsTransformed[i]);
		}

		Vector pos = entity->GetRenderOrigin();

		if (!Math::WorldToScreen(pointsTransformed[3], flb) || !Math::WorldToScreen(pointsTransformed[5], brt)
			|| !Math::WorldToScreen(pointsTransformed[0], blb) || !Math::WorldToScreen(pointsTransformed[4], frt)
			|| !Math::WorldToScreen(pointsTransformed[2], frb) || !Math::WorldToScreen(pointsTransformed[1], brb)
			|| !Math::WorldToScreen(pointsTransformed[6], blt) || !Math::WorldToScreen(pointsTransformed[7], flt))
			return false;

		Vector arr[] = { flb, brt, blb, frt, frb, brb, blt, flt };
		//+1 for each cuz of borders at the original box
		float left = flb.x;        // left
		float top = flb.y;        // top
		float right = flb.x;    // right
		float bottom = flb.y;    // bottom

		for (int i = 1; i < 8; i++)
		{
			if (left > arr[i].x)
				left = arr[i].x;
			if (bottom < arr[i].y)
				bottom = arr[i].y;
			if (right < arr[i].x)
				right = arr[i].x;
			if (top > arr[i].y)
				top = arr[i].y;
		}

		x = (int)left;
		y = (int)top;
		w = (int)(right - left);
		h = (int)(bottom - top);

		return true;
	}
	return false;
}


void CVisuals::DrawAngleLine(Vector origin, float angle, color_t color)
{
	Vector src, dst, sc1, sc2, forward;

	src = origin;
	Math::AngleVectors(Vector(0, angle, 0), &forward);
	if (Math::WorldToScreen(src, sc1) && Math::WorldToScreen(src + (forward * 40), sc2))
	{
		Drawing::DrawLine(sc1.x, sc1.y, sc2.x, sc2.y, color);
		//g_Render->DrawLine(sc1.x, sc1.y, sc2.x, sc2.y, color, 1.f);
	}
}

void CVisuals::DrawLocalShit(IDirect3DDevice9* pDevice)
{
	if (csgo->local->isAlive() && csgo->weapon)
	{
		int dx = csgo->w, dy = csgo->h;

		if (vars.visuals.remove & 8)
		{
			if (csgo->weapon->isSniper() && csgo->local->IsScoped())
			{
				g_Render->DrawLine(dx / 2, 0, dx / 2, dy, color_t(0, 0, 0, 150), vars.visuals.scope_thickness);
				g_Render->DrawLine(0, dy / 2, dx, dy / 2, color_t(0, 0, 0, 150), vars.visuals.scope_thickness);
			}
		}

		dx /= 2;
		dy /= 2;

		if (vars.visuals.innacuracyoverlay)
		{
			if (vars.visuals.innacuracyoverlaytype == 0)
			{
				g_Render->circle_filled_radial_gradient(pDevice, Vector2D(dx, dy), csgo->weaponspread,
					vars.visuals.innacuracyoverlay_color, color_t(
						vars.visuals.innacuracyoverlay_color[0],
						vars.visuals.innacuracyoverlay_color[1],
						vars.visuals.innacuracyoverlay_color[2],
						100));
			}
			else
			{
				g_Render->circle_filled_rainbow(pDevice, Vector2D(dx, dy), csgo->weaponspread, true, vars.visuals.innacuracyoverlay_rotatingspeed / 100.f);
			}
		}

		static auto percent_col = [](int per) -> color_t {
			int red = per < 50 ? 255 : floorf(255 - (per * 2 - 100) * 255.f / 100.f);
			int green = per > 50 ? 255 : floorf((per * 2) * 255.f / 100.f);

			return color_t(red, green, 0);
		};

		int add = 0;

		auto anims = csgo->local->GetPlayerAnimState();

		if (vars.antiaim.enable && !csgo->ForceOffAA)
		{
			if (csgo->should_stop)
			{
				auto clr = percent_col(csgo->delta);
				g_Render->DrawString(10, dx - (50 + add), color_t(clr.get_alpha() - 126, clr.get_green() - 65, 10),
					render::outline, fonts::lby_indicator, "STOP"
				);
				//g_Render->DrawString(fonts::lby_indicator, 10, dx - (50 + add), false, false, true, color_t(clr.a() - 126, clr.g() - 65, 10), "FAKE");
				add += 30;
			}
			if (vars.visuals.indicators & 1 && vars.antiaim.enable)
			{
				auto clr = percent_col(csgo->delta);
				g_Render->DrawString(10, dx - (50 + add), color_t(clr.get_alpha() - 126, clr.get_green() - 65, 10),
					render::outline, fonts::lby_indicator, "FAKE"
				);
				//g_Render->DrawString(fonts::lby_indicator, 10, dx - (50 + add), false, false, true, color_t(clr.a() - 126, clr.g() - 65, 10), "FAKE");
				add += 30;
			}
			if (vars.visuals.indicators & 2 && vars.antiaim.fakelag > 0)
			{
				static color_t clr;
				if (csgo->canDrawLC)
				{
					clr = csgo->canBreakLC ? color_t(129, 190, 10) : color_t(255, 0, 0);
					g_Render->DrawString(10, dx - (50 + add), clr,
						render::outline, fonts::lby_indicator, "LC"
					);
					add += 30;
				}
			}
			if (vars.visuals.indicators & 8 && vars.antiaim.fakeduck->active)
			{
				//g_Render->DrawString(fonts::lby_indicator, 10, dx - (50 + add), false, false, true, color_t(129, 190, 10), "FD");
				g_Render->DrawString(10, dx - (50 + add), color_t(129, 190, 10),
					render::outline, fonts::lby_indicator, "FD"
				);
				add += 30;
			}
			/*		if (vars.antiaim.aa_override.enable && vars.visuals.indicators & 64)
			{
				static bool left, right, back;
				if (csgo->key_pressed[vars.antiaim.aa_override.left])
				{
					left = true;
					right = false;
					back = false;
				}
				else if (csgo->key_pressed[vars.antiaim.aa_override.right])
				{
					left = false;
					right = true;
					back = false;
				}
				else if (csgo->key_pressed[vars.antiaim.aa_override.back])
				{
					left = false;
					right = false;
					back = true;
				}

				if (left)
				{
					g_Render->DrawString(fonts::lby_indicator, 10, dx - (50 + add), false, false, true, color_t(129, 190, 10), "LEFT");
					g_Render->DrawString(10, dx - (50 + add), color_t(129, 190, 10),
						render::outline, fonts::lby_indicator, "LEFT"
					);
					add += 30;
				}
				if (right)
				{
					g_Render->DrawString(10, dx - (50 + add), color_t(129, 190, 10),
						render::outline, fonts::lby_indicator, "RIGHT"
					);
					add += 30;
				}
				if (back)
				{
				g_Render->DrawString(10, dx - (50 + add), color_t(129, 190, 10),
						render::outline, fonts::lby_indicator, "BACK"
					);
					add += 30;
				}
			} */
		}
		if (vars.ragebot.enable)
		{
			if (vars.visuals.indicators & 8 && vars.ragebot.override_dmg->active) {

				//g_Render->DrawString(fonts::lby_indicator, 10, dx - (50 + add), false, false, true, color_t(129, 190, 10), "DMG");
				g_Render->DrawString(10, dx - (50 + add), color_t(129, 190, 10),
					render::outline, fonts::lby_indicator, "DMG"
				);
				add += 30;
			}
			if (vars.visuals.indicators & 16 && vars.ragebot.force_body->active) {
				//g_Render->DrawString(fonts::lby_indicator, 10, dx - (50 + add), false, false, true, color_t(129, 190, 10), "BAIM");
				g_Render->DrawString(10, dx - (50 + add), color_t(129, 190, 10),
					render::outline, fonts::lby_indicator, "BAIM"
				);
				add += 30;
			}

			if (vars.visuals.indicators & 16 && vars.misc.airstuck->active) {
				g_Render->DrawString(10, dx - (50 + add), color_t(129, 190, 10),
					render::outline, fonts::lby_indicator, "AIRSTUCK"
				);
				add += 30;
			}

			if (vars.visuals.indicators & 32 && csgo->dt_charged) {
				//g_Render->DrawString(fonts::lby_indicator, 10, dx - (50 + add), false, false, true, color_t(129, 190, 10), "DT");
				if (CanDT() && !CMAntiAim::Get().did_shot) {
					g_Render->DrawString(10, dx - (50 + add), color_t(129, 190, 10),
						render::outline, fonts::lby_indicator, "DT"
					);
					add += 30;
				}
				if (CanHS()) {
					g_Render->DrawString(10, dx - (50 + add), color_t(129, 190, 10),
						render::outline, fonts::lby_indicator, "HS"
					);
					add += 30;
				}
			}
		}
	}
}

std::string str_toupper(std::string s) {
	std::transform(s.begin(), s.end(), s.begin(),
		[](unsigned char c) { return toupper(c); }
	);
	return s;
}

void DrawFovArrows(IBasePlayer* entity, float alpha, Vector origin)
{
	if (!vars.visuals.out_of_fov)
		return;

	auto idx = entity->GetIndex() - 1;
	Vector poopvec;

	Vector vEnemyOrigin = origin;
	Vector vLocalOrigin = csgo->origin;

	Vector screenPos;

	auto isOnScreen = [](Vector origin, Vector& screen) -> bool
	{
		if (!Math::WorldToScreen(origin, screen))
			return false;

		return (csgo->w > screen.x && screen.x > 0) && (csgo->h > screen.y && screen.y > 0);
	};

	if (!entity->IsDormant()) {
		if (isOnScreen(entity->GetBonePos(entity->GetBoneCache().Base(), 2), screenPos))
			return;
		else
			if (isOnScreen(vEnemyOrigin, screenPos))
				return;

	}
	Vector dir;
	interfaces.engine->GetViewAngles(dir);

	float view_angle = dir.y;

	if (view_angle < 0.f)
	{
		view_angle += 360.f;
	}
	view_angle = DEG2RAD(view_angle);

	auto entity_angle = Math::CalculateAngle(vLocalOrigin, vEnemyOrigin);
	entity_angle.Normalized();

	if (entity_angle.y < 0.f) {
		entity_angle.y += 360.f;
	}
	entity_angle.y = DEG2RAD(entity_angle.y);
	entity_angle.y -= view_angle;

	float dist = vars.visuals.out_of_fov_distance * 10;

	auto position = Vector2D(csgo->w / 2, csgo->h / 2);
	position.x -= dist;

	auto clr = entity->IsDormant() ? color_t(50, 50, 50, alpha - 50) : color_t(
		vars.visuals.out_of_fov_color[0],
		vars.visuals.out_of_fov_color[1],
		vars.visuals.out_of_fov_color[2],
		vars.visuals.out_of_fov_color[3] * (alpha / 255.f));

	Drawing::rotate_point(position, Vector2D(csgo->w / 2, csgo->h / 2), false, entity_angle.y);

	auto size = vars.visuals.out_of_fov_size / 2;


	Drawing::filled_tilted_triangle(position, Vector2D(size - 1, size), position, true, -entity_angle.y,
		(entity->IsDormant() && alpha > 255.f ? color_t(130, 130, 130, alpha) : clr));

}


void CVisuals::Draw()
{
	if (!interfaces.engine->IsConnected() && !interfaces.engine->IsInGame())
		return;

	RecordInfo();


	if (vars.visuals.enable)
	{
		for (auto entity : players) {

			if (entity == nullptr
				|| !entity->IsPlayer()
				|| entity == csgo->local
				|| entity->GetTeam() == csgo->local->GetTeam())
				continue;

			bool dormant = entity->IsDormant();
			if (vars.visuals.dormant)
			{
				if (!entity->isAlive())
				{

					csgo->EntityAlpha[entity->GetIndex()] = 0;
					continue;
				}
				strored_origin[entity->GetIndex()] = entity->GetRenderOrigin();
				if (dormant)
				{
					if (csgo->EntityAlpha[entity->GetIndex()] > 140)
						csgo->EntityAlpha[entity->GetIndex()]--;
					else if ((csgo->LastSeenTime[entity->GetIndex()] + 3.f) <= csgo->get_absolute_time() && csgo->EntityAlpha[entity->GetIndex()] > 0)
						csgo->EntityAlpha[entity->GetIndex()]--;

					if (csgo->EntityAlpha[entity->GetIndex()] <= 0.f)
						continue;
				}
				else
				{
					csgo->EntityAlpha[entity->GetIndex()] = 255;
					csgo->LastSeenTime[entity->GetIndex()] = csgo->get_absolute_time();
				}

				csgo->EntityAlpha[entity->GetIndex()] = std::clamp(csgo->EntityAlpha[entity->GetIndex()], 0.f, 255.f);
			}
			else
			{
				if (dormant || !entity->isAlive())
					continue;

				strored_origin[entity->GetIndex()] = entity->GetRenderOrigin();
			}

			DrawFovArrows(entity, csgo->EntityAlpha[entity->GetIndex()], strored_origin[entity->GetIndex()]);

			int alpha = vars.visuals.dormant ? csgo->EntityAlpha[entity->GetIndex()] : 255;

			auto weapon = entity->GetWeapon();
			if (!weapon)
				continue;

			//	DrawFovArrows(entity, (float)alpha, strored_origin[entity->GetIndex()]);

			BOX box;
			if (!GetBox(entity, box.x, box.y, box.w, box.h, strored_origin[entity->GetIndex()]))
				continue;

			if (vars.visuals.box)
			{
				auto clr = dormant ? color_t(255, 255, 255, alpha - 50) : color_t(
					vars.visuals.box_color[0],
					vars.visuals.box_color[1],
					vars.visuals.box_color[2],
					alpha - 50);

				Drawing::DrawOutlinedRect(box.x, box.y, box.w, box.h, clr);
				Drawing::DrawOutlinedRect(box.x - 1, box.y - 1, box.w + 2, box.h + 2, color_t(10, 10, 10, (alpha * 0.8f)));
				Drawing::DrawOutlinedRect(box.x + 1, box.y + 1, box.w - 2, box.h - 2, color_t(10, 10, 10, (alpha * 0.8f)));
			}

			int count = 0;
			if (vars.visuals.name)
			{
				auto text_size = Drawing::GetTextSize(fonts::esp_name, entity->GetName().c_str());

				int r = vars.visuals.name_color[0],
					g = vars.visuals.name_color[1],
					b = vars.visuals.name_color[2],
					a = vars.visuals.name_color[3] * (alpha / 255.f);

				auto clr = dormant ? color_t(255, 255, 255, alpha) : color_t(r, g, b, a);

				Drawing::DrawString(fonts::esp_name, box.x + box.w / 2 - text_size.right / 2, box.y - 13, clr, FONT_LEFT, entity->GetName().c_str());
			}
			if (!dormant) {
				auto record = g_Animfix->get_latest_animation(entity);
				if (record.has_value() && record.value()->player) {
					auto clr = color_t(
						vars.visuals.skeleton_color[0],
						vars.visuals.skeleton_color[1],
						vars.visuals.skeleton_color[2],
						alpha - 50);
					if (vars.visuals.shot_multipoint) {

						auto hitboxes = Ragebot::Get().GetHitboxesToScan(record.value()->player);
						auto priority = Ragebot::Get().GetCurrentPriorityHitbox(record.value()->player);
						for (auto i : hitboxes) {
							auto points = Ragebot::Get().GetMultipoints(record.value()->player, i, record.value()->bones);
							for (auto p : points) {
								Vector world;
								if (Math::WorldToScreen(p, world)) {
									Drawing::DrawBox(world.x - 2, world.y - 2, 4, 4, color_t(0, 0, 0, alpha));
									Drawing::DrawBox(world.x - 1, world.y - 1, 2, 2, priority == i ? color_t(84, 25, 120, alpha) : color_t(255, 255, 255, alpha));

								}
							}
						}
					}
					if (vars.visuals.skeleton) {
						studiohdr_t* pStudioHdr = interfaces.models.model_info->GetStudioModel(entity->GetModel());

						if (!pStudioHdr)
							return;

						Vector vParent, vChild, sParent, sChild;

						for (int j = 0; j < pStudioHdr->numbones; j++)
						{
							mstudiobone_t* pBone = pStudioHdr->pBone(j);

							if (pBone && (pBone->flags & 0x100) && (pBone->parent != -1))
							{
								vChild = entity->GetBonePos(record.value()->bones, j);
								vParent = entity->GetBonePos(record.value()->bones, pBone->parent);

								if (Math::WorldToScreen(vParent, sParent) && Math::WorldToScreen(vChild, sChild))
								{
									Drawing::DrawLine(sParent[0], sParent[1], sChild[0], sChild[1], clr);
								}
							}
						}
					}
				}
			}

			if (vars.visuals.healthbar)
			{
				int hp = entity->GetHealth();

				if (hp > 100)
					hp = 100;

				int hp_percent = box.h - (int)((box.h * hp) / 100);

				int width = (box.w * (hp / 100.f));

				int red = 255 - (hp * 2.55);
				int green = hp * 2.55;

				char hps[10] = "";

				sprintf_s(hps, "%i", hp);

				auto text_size = Drawing::GetTextSize(fonts::esp_info, hps);

				auto clr = dormant ? color_t(200, 200, 200, alpha) : color_t(255, 255, 255, alpha);

				auto hp_color = dormant ? color_t(200, 200, 200, alpha) : vars.visuals.override_hp ? color_t(
					vars.visuals.hp_color[0],
					vars.visuals.hp_color[1],
					vars.visuals.hp_color[2],
					vars.visuals.hp_color[3] * (alpha / 255.f)) : color_t(red, green, 0, alpha);

				Drawing::DrawRect(box.x - 6, box.y - 1, 4, box.h + 2, color_t(80, 80, 80, alpha * 0.49f));
				Drawing::DrawOutlinedRect(box.x - 6, box.y - 1, 4, box.h + 2, color_t(10, 10, 10, (alpha * 0.8f)));
				Drawing::DrawRect(box.x - 5, box.y + hp_percent, 2, box.h - hp_percent, hp_color);

				if (hp <= 99) // draws hp count at the same pos as top of bar
					Drawing::DrawString(fonts::esp_info, box.x - text_size.left, box.y + hp_percent - 1,
						color_t(255, 255, 255, alpha), FONT_LEFT, hps);

			}
			/*DrawAngleLine(strored_origin[entity->GetIndex()], resolver->GetAngle(entity), color_t(255, 255, 255, alpha));
			DrawAngleLine(strored_origin[entity->GetIndex()], resolver->GetForwardYaw(entity), color_t(255, 0, 0, alpha));
			DrawAngleLine(strored_origin[entity->GetIndex()], resolver->GetBackwardYaw(entity), color_t(0, 255, 0, alpha));*/

			if (weapon && !dormant)
			{
				if (weapon->IsGun())
				{

					auto ammo = weapon->GetAmmo(false);
					auto max_ammo = weapon->GetAmmo(true);
					if (vars.visuals.ammo)
					{
						auto clr = color_t(
							vars.visuals.ammo_color[0],
							vars.visuals.ammo_color[1],
							vars.visuals.ammo_color[2],
							vars.visuals.ammo_color[3] * (alpha / 255.f));

						int hp_percent = box.w - (int)((box.w * ammo) / 100);

						int width = (box.w * (ammo / float(max_ammo)));

						char ammostr[10];
						sprintf_s(ammostr, "%d", ammo);

						const auto text_size = Drawing::GetTextSize(fonts::esp_info, ammostr);

						Drawing::DrawRect(box.x, box.y + 2 + box.h, box.w + 1, 4, color_t(80, 80, 80, alpha * 0.49f));
						Drawing::DrawOutlinedRect(box.x, box.y + 2 + box.h, box.w + 1, 4, color_t(10, 10, 10, (alpha * 0.8f)));
						Drawing::DrawRect(box.x + 1, box.y + 3 + box.h, width - 1, 2, clr);

						if (ammo < (max_ammo / 2) && ammo > 0)
							Drawing::DrawString(fonts::esp_info, box.x + width - 1 - text_size.right, box.y + box.h,
								color_t(255, 255, 255, alpha - 55.f), FONT_LEFT, ammostr);

						count++;
					}
				}
				if (vars.visuals.weapon)
				{
					auto clr = vars.visuals.weapon_color;

					auto weap_info = entity->GetWeapon()->GetCSWpnData();

					char wpn_name[100] = "";

					sprintf_s(wpn_name, "%s", str_toupper(weap_info->m_szWeaponName).c_str() + 7);

					if (entity->GetWeapon()->GetItemDefinitionIndex() == 64)
						strcpy_s(wpn_name, "REVOLVER");

					auto wpn_name_size = Drawing::GetTextSize(fonts::esp_info, wpn_name);

					Drawing::DrawString(fonts::esp_info, box.x + box.w / 2 - wpn_name_size.right / 2, box.y + 1 + box.h + (count++ * 6.f),
						clr, FONT_LEFT, wpn_name);
				}

			}

			int offset = 0;
			if (weapon && !dormant)
			{
				auto clr = vars.visuals.flags_color;

				if (vars.visuals.flags & 1 && entity->GetArmor() > 0) {
					if (entity->HasHelmet())
						Drawing::DrawString(fonts::esp_info, box.x + box.w + 3, box.y + offset++ * 9, clr, FONT_LEFT,
							"HK");
					else
						Drawing::DrawString(fonts::esp_info, box.x + box.w + 3, box.y + offset++ * 9, clr, FONT_LEFT, "K");
				}
				if (vars.visuals.flags & 2 && entity->IsScoped()) {
					Drawing::DrawString(fonts::esp_info, box.x + box.w + 3, box.y + offset++ * 9, clr, FONT_LEFT, "ZOOM");
				}
				if (vars.visuals.flags & 4 && entity->IsFlashed() > 0) {
					Drawing::DrawString(fonts::esp_info, box.x + box.w + 3, box.y + offset++ * 9, clr, FONT_LEFT, "BLIND");
				}
				if (vars.visuals.flags & 8 && csgo->hitchance) {
					Drawing::DrawString(fonts::esp_info, box.x + box.w + 3, box.y + offset++ * 9, clr, FONT_LEFT, "HIT");
					csgo->hitchance = false;
				}

				if (vars.visuals.flags & 16 && vars.ragebot.resolver && ResolverMode[entity->EntIndex()] != "") {
					Drawing::DrawString(fonts::esp_info, box.x + box.w + 3, box.y + offset++ * 9, clr, FONT_LEFT, ResolverMode[entity->EntIndex()].c_str());
				}
				if (vars.visuals.flags & 32 && vars.ragebot.resolver) {
					auto str = "CHOKE: " + std::to_string(entity->GetChokedPackets());
					Drawing::DrawString(fonts::esp_info, box.x + box.w + 3, box.y + offset++ * 9, clr, FONT_LEFT, str.c_str());
				}
#ifdef _DEBUG
				if (vars.visuals.dormant && dormant)
				{
					Drawing::DrawString(fonts::esp_info, box.x + box.w + 3, box.y + offset++ * 9, clr, FONT_LEFT, "DORMANT");
				}
#endif
			}
		}
		//DrawAngleLine(csgo->local->GetRenderOrigin(), csgo->local->GetLBY(), color_t(255, 255, 255, 255));
	}


}

bool CVisuals::IsValidSound(SndInfo_t& sound) {
	// We don't want the sound to keep following client's predicted origin.
	for (int iter = 0; iter < m_utlvecSoundBuffer.Count(); iter++)
	{
		SndInfo_t& cached_sound = m_utlvecSoundBuffer[iter];
		if (cached_sound.m_nGuid == sound.m_nGuid)
		{
			return false;
		}
	}

	return true;
}

void CVisuals::OnDormant() {
	if (!interfaces.engine->IsInGame() || !csgo->local || !csgo->local->isAlive() || csgo->disable_dt)
		return;

	m_utlCurSoundList.RemoveAll();
	interfaces.engine_sound->GetActiveSounds(m_utlCurSoundList);

	// No active sounds.
	if (!m_utlCurSoundList.Count())
		return;

	// Accumulate sounds for esp correction
	for (int iter = 0; iter < m_utlCurSoundList.Count(); iter++)
	{
		SndInfo_t& sound = m_utlCurSoundList[iter];
		if (sound.m_nSoundSource == 0 || // World
			sound.m_nSoundSource > 64)   // Most likely invalid
			continue;

		auto player = interfaces.ent_list->GetClientEntity(sound.m_nSoundSource);

		if (!player || player == csgo->local || sound.m_pOrigin->IsZero())
			continue;

		if (!IsValidSound(sound))
			continue;

		AdjustDormant(player, sound);

		csgo->LastSeenTime[player->GetIndex()] = csgo->get_absolute_time();

		m_cSoundPlayers[sound.m_nSoundSource - 1].Override(sound);
	}

	m_utlvecSoundBuffer = m_utlCurSoundList;
}

void CVisuals::AdjustDormant(IBasePlayer* player, SndInfo_t& sound)
{
	Vector src3D, dst3D;
	trace_t tr;
	Ray_t ray;
	CTraceFilter filter;

	filter.pSkip = player;
	src3D = (*sound.m_pOrigin) + Vector(0, 0, 1); // So they dont dig into ground incase shit happens /shrug
	dst3D = src3D - Vector(0, 0, 100);
	ray.Init(src3D, dst3D);

	interfaces.trace->TraceRay(ray, MASK_PLAYERSOLID, &filter, &tr);

	// step = (tr.fraction < 0.20)
	// shot = (tr.fraction > 0.20)
	// stand = (tr.fraction > 0.50)
	// crouch = (tr.fraction < 0.50)

	/* Corrects origin and important flags. */

	// Player stuck, idk how this happened
	if (tr.allsolid)
	{
		m_cSoundPlayers[sound.m_nSoundSource - 1].m_iReceiveTime = -1;
	}

	*sound.m_pOrigin = ((tr.fraction < 0.97) ? tr.endpos : *sound.m_pOrigin);
	m_cSoundPlayers[sound.m_nSoundSource - 1].m_nFlags = player->GetFlags();
	m_cSoundPlayers[sound.m_nSoundSource - 1].m_nFlags |= (tr.fraction < 0.50f ? FL_DUCKING : 0) | (tr.fraction != 1 ? FL_ONGROUND : 0);   // Turn flags on
	m_cSoundPlayers[sound.m_nSoundSource - 1].m_nFlags &= (tr.fraction > 0.50f ? ~FL_DUCKING : 0) | (tr.fraction == 1 ? ~FL_ONGROUND : 0); // Turn flags off
}

void CVisuals::RecordInfo()
{
	players.clear();
	for (auto i = 0; i < interfaces.global_vars->maxClients; i++)
	{
		IBasePlayer* entity = interfaces.ent_list->GetClientEntity(i);
		if (!entity)
			continue;

		players.push_back(entity);
	}
}

void CVisuals::agent_changer(ClientFrameStage_t stage) {

	if (stage != FRAME_RENDER_START && stage != FRAME_RENDER_END)
		return;

	static int originalIdx = 0;

	if (!csgo->local) {
		originalIdx = 0;
		return;
	}


	constexpr auto getModel = [](int team) constexpr noexcept -> const char* {
		constexpr std::array models{
("models/player/custom_player/legacy/ctm_fbi_variantb.mdl"), //Agent Ava
("models/player/custom_player/legacy/ctm_fbi_variantf.mdl"), //Operator
("models/player/custom_player/legacy/ctm_fbi_variantg.mdl"), //Markus Delrow
("models/player/custom_player/legacy/ctm_fbi_varianth.mdl"), //Michael Syfers
("models/player/custom_player/legacy/ctm_st6_variante.mdl"), //Seal Team 6 Soldier
("models/player/custom_player/legacy/ctm_st6_variantg.mdl"), //Buckshot
("models/player/custom_player/legacy/ctm_st6_varianti.mdl"), //Ricksaw
("models/player/custom_player/legacy/ctm_st6_variantk.mdl"), //3rd Commando Company
("models/player/custom_player/legacy/ctm_st6_variantm.mdl"), //McCoy
("models/player/custom_player/legacy/ctm_sas_variantf.mdl"), //B Squadron Officer
("models/player/custom_player/legacy/tm_leet_variantf.mdl"), //Mr Muhlik
("models/player/custom_player/legacy/tm_leet_varianti.mdl"), //Prof. Shahmat
("models/player/custom_player/legacy/tm_leet_varianth.mdl"), //Osiris
("models/player/custom_player/legacy/tm_leet_variantg.mdl"), //Ground Rebel
("models/player/custom_player/legacy/tm_balkan_variantf.mdl"), //Dragomir
("models/player/custom_player/legacy/tm_balkan_variantg.mdl"), //Rezan The Ready
("models/player/custom_player/legacy/tm_balkan_varianth.mdl"), //Doctor Romanov
("models/player/custom_player/legacy/tm_balkan_varianti.mdl"), //Maximus
("models/player/custom_player/legacy/tm_balkan_variantj.mdl"), //Blackwolf
("models/player/custom_player/legacy/tm_phoenix_variantf.mdl"), //Enforcer
("models/player/custom_player/legacy/tm_phoenix_variantg.mdl"), //Slingshot
("models/player/custom_player/legacy/tm_phoenix_varianth.mdl"), //Soldier Phoenix
("models/player/custom_player/legacy/ctm_heavy.mdl"), //Heavy Soldier
		};

		switch (team) {
		case 2: return static_cast<std::size_t>(vars.visuals.agents_t - 1) < models.size() ? models[vars.visuals.agents_t - 1] : nullptr;
		case 3: return static_cast<std::size_t>(vars.visuals.agents_ct - 1) < models.size() ? models[vars.visuals.agents_ct - 1] : nullptr;
		default: return nullptr;
		}
	};

	if (const auto model = getModel(csgo->local->GetTeam())) {
		if (stage == FRAME_RENDER_START)
			originalIdx = csgo->local->m_nModelIndex();

		const auto idx = stage == FRAME_RENDER_END && originalIdx ? originalIdx : interfaces.models.model_info->GetModelIndex(model);

		csgo->local->SetModelIndex(idx);


		if (const auto ragdoll = interfaces.ent_list->GetClientEntityFromHandle(csgo->local->ragdoll()))
			ragdoll->SetModelIndex(idx);
	}

}

void CGrenadePrediction::Tick(int buttons)
{
	if (!vars.visuals.nadepred)
		return;
	bool in_attack = buttons & IN_ATTACK;
	bool in_attack2 = buttons & IN_ATTACK2;

	//g_pICvar->Consolecolor_tPrintf(SDK::color_t(255, 255, 255, 255), "OView");

	act = (in_attack && in_attack2) ? ACT_LOB :
		(in_attack2) ? ACT_DROP :
		(in_attack) ? ACT_THROW :
		ACT_NONE;
}
void CGrenadePrediction::View(CViewSetup* setup)
{
	if (!vars.visuals.nadepred)
		return;

	if (csgo->local && csgo->local->isAlive())
	{
		IBaseCombatWeapon* weapon = csgo->local->GetWeapon();

		if (weapon && weapon->IsNade() && act != ACT_NONE)
		{
			type = weapon->GetItemDefinitionIndex();
			Simulate(setup);
		}
		else
		{
			type = 0;
		}
	}
}

void CGrenadePrediction::Paint()
{
	if (!vars.visuals.nadepred)
		return;

	if ((type) && !(path.empty()))
	{
		Vector nadeStart, nadeEnd;
		Vector nadeStart1, nadeEnd1;

		Vector prev = path[0];

		for (auto it = path.begin(), end = path.end(); it != end; ++it)
		{
			if (Math::WorldToScreen2(prev, nadeStart) && Math::WorldToScreen2(*it, nadeEnd))
			{
				Drawing::DrawLine((int)nadeStart.x, (int)nadeStart.y, (int)nadeEnd.x, (int)nadeEnd.y, vars.visuals.nadepred_color);
			}
			prev = *it;
		}

		Drawing::DrawFilledCircle(nadeEnd.x, nadeEnd.y, 5, 100, color_t(0, 0, 0, 200));
		Drawing::DrawFilledCircle(nadeEnd.x, nadeEnd.y, 4, 100, color_t(255, 0, 0, 200));
	}
}
const auto PIRAD = 0.01745329251f;
void angle_vectors2(const Vector& angles, Vector* forward, Vector* right, Vector* up)
{
	float sr, sp, sy, cr, cp, cy;

	sp = static_cast<float>(sin(double(angles.x) * PIRAD));
	cp = static_cast<float>(cos(double(angles.x) * PIRAD));
	sy = static_cast<float>(sin(double(angles.y) * PIRAD));
	cy = static_cast<float>(cos(double(angles.y) * PIRAD));
	sr = static_cast<float>(sin(double(angles.z) * PIRAD));
	cr = static_cast<float>(cos(double(angles.z) * PIRAD));

	if (forward)
	{
		forward->x = cp * cy;
		forward->y = cp * sy;
		forward->z = -sp;
	}

	if (right)
	{
		right->x = (-1 * sr * sp * cy + -1 * cr * -sy);
		right->y = (-1 * sr * sp * sy + -1 * cr * cy);
		right->z = -1 * sr * cp;
	}

	if (up)
	{
		up->x = (cr * sp * cy + -sr * -sy);
		up->y = (cr * sp * sy + -sr * cy);
		up->z = cr * cp;
	}
}
void CGrenadePrediction::Setup(Vector& vecSrc, Vector& vecThrow, Vector viewangles)
{
	if (!vars.visuals.nadepred)
		return;

	Vector angThrow = viewangles;
	float pitch = angThrow.x;

	if (pitch <= 90.0f)
	{
		if (pitch < -90.0f)
		{
			pitch += 360.0f;
		}
	}
	else
	{
		pitch -= 360.0f;
	}
	float a = pitch - (90.0f - fabs(pitch)) * 10.0f / 90.0f;
	angThrow.x = a;

	// Gets ThrowVelocity from weapon files
	// Clamped to [15,750]
	float flVel = 750.0f * 0.9f;

	// Do magic on member of grenade object [esi+9E4h]
	// m1=1  m1+m2=0.5  m2=0
	static const float power[] = { 1.0f, 1.0f, 0.5f, 0.0f };
	float b = power[act];
	// Clamped to [0,1]
	b = b * 0.7f;
	b = b + 0.3f;
	flVel *= b;

	Vector vForward, vRight, vUp;
	angle_vectors2(angThrow, &vForward, &vRight, &vUp); //angThrow.ToVector(vForward, vRight, vUp);

	vecSrc = csgo->unpred_eyepos;
	float off = (power[act] * 12.0f) - 12.0f;
	vecSrc.z += off;

	// Game calls UTIL_TraceHull here with hull and assigns vecSrc tr.endpos
	trace_t tr;
	Vector vecDest = vecSrc;
	vecDest += vForward * 22.0f; //vecDest.MultAdd(vForward, 22.0f);

	TraceHull(vecSrc, vecDest, tr);

	// After the hull trace it moves 6 units back along vForward
	// vecSrc = tr.endpos - vForward * 6
	Vector vecBack = vForward; vecBack *= 6.0f;
	vecSrc = tr.endpos;
	vecSrc -= vecBack;

	// Finally calculate velocity
	vecThrow = csgo->local->GetVelocity(); vecThrow *= 1.25f;
	vecThrow += vForward * flVel; //	vecThrow.MultAdd(vForward, flVel);
}

void CGrenadePrediction::Simulate(CViewSetup* setup)
{
	if (!vars.visuals.nadepred)
		return;
	Vector vecSrc, vecThrow;
	Vector angles; interfaces.engine->GetViewAngles(angles);
	Setup(vecSrc, vecThrow, angles);

	float interval = interfaces.global_vars->interval_per_tick;

	// Log positions 20 times per sec
	int logstep = static_cast<int>(0.05f / interval);
	int logtimer = 0;


	path.clear();
	for (unsigned int i = 0; i < path.max_size() - 1; ++i)
	{
		if (!logtimer)
			path.push_back(vecSrc);

		int s = Step(vecSrc, vecThrow, i, interval);
		if ((s & 1)) break;

		// Reset the log timer every logstep OR we bounced
		if ((s & 2) || logtimer >= logstep) logtimer = 0;
		else ++logtimer;
	}
	path.push_back(vecSrc);
}

int CGrenadePrediction::Step(Vector& vecSrc, Vector& vecThrow, int tick, float interval)
{

	// Apply gravity
	Vector move;
	AddGravityMove(move, vecThrow, interval, false);

	// Push entity
	trace_t tr;
	PushEntity(vecSrc, move, tr);

	int result = 0;
	// Check ending conditions
	if (CheckDetonate(vecThrow, tr, tick, interval))
	{
		result |= 1;
	}

	// Resolve collisions
	if (tr.fraction != 1.0f)
	{
		result |= 2; // Collision!
		ResolveFlyCollisionCustom(tr, vecThrow, interval);
	}

	// Set new position
	vecSrc = tr.endpos;

	return result;
}


bool CGrenadePrediction::CheckDetonate(const Vector& vecThrow, const trace_t& tr, int tick, float interval)
{
	switch (type)
	{
	case WEAPON_SMOKEGRENADE:
	case WEAPON_DECOY:
		// Velocity must be <0.1, this is only checked every 0.2s
		if (vecThrow.Length2D() < 0.1f)
		{
			int det_tick_mod = static_cast<int>(0.2f / interval);
			return !(tick % det_tick_mod);
		}
		return false;

	case WEAPON_MOLOTOV:
	case WEAPON_INC:
		// Detonate when hitting the floor
		if (tr.fraction != 1.0f && tr.plane.normal.z > 0.7f)
			return true;
		// OR we've been flying for too long

	case WEAPON_FLASHBANG:
	case WEAPON_HEGRENADE:
		// Pure timer based, detonate at 1.5s, checked every 0.2s
		return static_cast<float>(tick) * interval > 1.5f && !(tick % static_cast<int>(0.2f / interval));

	default:
		assert(false);
		return false;
	}
}

void CGrenadePrediction::TraceHull(Vector& src, Vector& end, trace_t& tr)
{
	if (!vars.visuals.nadepred)
		return;
	Ray_t ray;
	ray.Init(src, end, Vector(-2.0f, -2.0f, -2.0f), Vector(2.0f, 2.0f, 2.0f));

	CTraceFilterWorldAndPropsOnly filter;
	//filter.SetIgnoreClass("BaseCSGrenadeProjectile");
	//filter.bShouldHitPlayers = false;

	interfaces.trace->TraceRay(ray, 0x200400B, &filter, &tr);
}

void CGrenadePrediction::AddGravityMove(Vector& move, Vector& vel, float frametime, bool onground)
{
	if (!vars.visuals.nadepred)
		return;
	Vector basevel(0.0f, 0.0f, 0.0f);

	move.x = (vel.x + basevel.x) * frametime;
	move.y = (vel.y + basevel.y) * frametime;

	if (onground)
	{
		move.z = (vel.z + basevel.z) * frametime;
	}
	else
	{
		// Game calls GetActualGravity( this );
		float gravity = 800.0f * 0.4f;

		float newZ = vel.z - (gravity * frametime);
		move.z = ((vel.z + newZ) / 2.0f + basevel.z) * frametime;

		vel.z = newZ;
	}
}

void CGrenadePrediction::PushEntity(Vector& src, const Vector& move, trace_t& tr)
{
	if (!vars.visuals.nadepred)
		return;
	Vector vecAbsEnd = src;
	vecAbsEnd += move;

	// Trace through world
	TraceHull(src, vecAbsEnd, tr);
}

void CGrenadePrediction::ResolveFlyCollisionCustom(trace_t& tr, Vector& vecVelocity, float interval)
{
	if (!vars.visuals.nadepred)
		return;
	// Calculate elasticity
	float flSurfaceElasticity = 1.0;  // Assume all surfaces have the same elasticity
	float flGrenadeElasticity = 0.45f; // GetGrenadeElasticity()
	float flTotalElasticity = flGrenadeElasticity * flSurfaceElasticity;
	if (flTotalElasticity > 0.9f) flTotalElasticity = 0.9f;
	if (flTotalElasticity < 0.0f) flTotalElasticity = 0.0f;

	// Calculate bounce
	Vector vecAbsVelocity;
	PhysicsClipVelocity(vecVelocity, tr.plane.normal, vecAbsVelocity, 2.0f);
	vecAbsVelocity *= flTotalElasticity;

	// Stop completely once we move too slow
	float flSpeedSqr = vecAbsVelocity.LengthSqr();
	static const float flMinSpeedSqr = 20.0f * 20.0f; // 30.0f * 30.0f in CSS
	if (flSpeedSqr < flMinSpeedSqr)
	{
		//vecAbsVelocity.Zero();
		vecAbsVelocity.x = 0.0f;
		vecAbsVelocity.y = 0.0f;
		vecAbsVelocity.z = 0.0f;
	}

	// Stop if on ground
	if (tr.plane.normal.z > 0.7f)
	{
		vecVelocity = vecAbsVelocity;
		vecAbsVelocity *= ((1.0f - tr.fraction) * interval); //vecAbsVelocity.Mult((1.0f - tr.fraction) * interval);
		PushEntity(tr.endpos, vecAbsVelocity, tr);
	}
	else
	{
		vecVelocity = vecAbsVelocity;
	}
}

int CGrenadePrediction::PhysicsClipVelocity(const Vector& in, const Vector& normal, Vector& out, float overbounce)
{
	static const float STOP_EPSILON = 0.1f;

	float    backoff;
	float    change;
	float    angle;
	int        i, blocked;

	blocked = 0;

	angle = normal[2];

	if (angle > 0)
	{
		blocked |= 1;        // floor
	}
	if (!angle)
	{
		blocked |= 2;        // step
	}

	backoff = in.Dot(normal) * overbounce;

	for (i = 0; i < 3; i++)
	{
		change = normal[i] * backoff;
		out[i] = in[i] - change;
		if (out[i] > -STOP_EPSILON && out[i] < STOP_EPSILON)
		{
			out[i] = 0;
		}
	}

	return blocked;
}

void CVisuals::NightMode()
{
	static auto load_named_sky = reinterpret_cast<void(__fastcall*)(const char*)>(
		csgo->Utils.FindPatternIDA(GetModuleHandleA(g_Modules[fnv::hash(hs::engine_dll::s().c_str())].c_str()),
			hs::load_named_sky::s().c_str()));

	static auto sv_skyname = interfaces.cvars->FindVar(hs::sv_skyname::s().c_str());
	sv_skyname->m_fnChangeCallbacks.m_Size = 0;

	static auto r_3dsky = interfaces.cvars->FindVar(hs::r_3dsky::s().c_str());
	r_3dsky->m_fnChangeCallbacks.m_Size = 0;
	r_3dsky->SetValue(0);

	static auto backup = sv_skyname->GetString();

	const auto reset = [&]()
	{
		load_named_sky(hs::sky_dust::s().c_str());

		for (auto i = interfaces.material_system->FirstMaterial(); i != interfaces.material_system->InvalidMaterial(); i = interfaces.material_system->NextMaterial(i))
		{
			auto mat = interfaces.material_system->GetMaterial(i);
			if (!mat)
				continue;

			if (mat->IsErrorMaterial())
				continue;

			std::string name = mat->GetName();
			auto tex_name = mat->GetTextureGroupName();


			if (strstr(tex_name, hs::World::s().c_str()) || strstr(tex_name, hs::StaticProp::s().c_str()) || strstr(tex_name, hs::SkyBox::s().c_str()))
			{
				mat->ColorModulate(1.f, 1.f, 1.f);
				mat->AlphaModulate(1.f);
			}
		}
	};

	static bool Reset = false;
	static bool Reset2 = false;

	if (vars.visuals.nightmode)
	{
		Reset = false;

		load_named_sky(hs::sky_csgo_night02::s().c_str());

		static float OldClr[3];
		static float OldClr1[3];
		static float OldClr2[3];

		if (csgo->mapChanged || !Reset2 || OldClr[0] != vars.visuals.nightmode_color[0]
			|| OldClr[1] != vars.visuals.nightmode_color[1]
			|| OldClr[2] != vars.visuals.nightmode_color[2])
		{
			for (auto i = interfaces.material_system->FirstMaterial(); i != interfaces.material_system->InvalidMaterial(); i = interfaces.material_system->NextMaterial(i))
			{
				auto mat = interfaces.material_system->GetMaterial(i);
				if (!mat)
					continue;

				auto tex_name = mat->GetTextureGroupName();

				if (strstr(tex_name, hs::World::s().c_str()))
				{
					mat->ColorModulate(vars.visuals.nightmode_color[0] / 255.f,
						vars.visuals.nightmode_color[1] / 255.f, vars.visuals.nightmode_color[2] / 255.f);
				}
			}

			OldClr[0] = vars.visuals.nightmode_color[0];
			OldClr[1] = vars.visuals.nightmode_color[1];
			OldClr[2] = vars.visuals.nightmode_color[2];
			csgo->mapChanged = false;
			Reset2 = true;
		}

		if (csgo->mapChanged || !Reset2 || OldClr1[0] != vars.visuals.nightmode_prop_color[0]
			|| OldClr1[1] != vars.visuals.nightmode_prop_color[1]
			|| OldClr1[2] != vars.visuals.nightmode_prop_color[2])
		{
			for (auto i = interfaces.material_system->FirstMaterial(); i != interfaces.material_system->InvalidMaterial(); i = interfaces.material_system->NextMaterial(i))
			{
				auto mat = interfaces.material_system->GetMaterial(i);
				if (!mat)
					continue;

				auto tex_name = mat->GetTextureGroupName();

				if (strstr(tex_name, hs::StaticProp::s().c_str()))
				{
					mat->ColorModulate(vars.visuals.nightmode_prop_color[0] / 255.f,
						vars.visuals.nightmode_prop_color[1] / 255.f, vars.visuals.nightmode_prop_color[2] / 255.f);
				}
			}

			OldClr1[0] = vars.visuals.nightmode_prop_color[0];
			OldClr1[1] = vars.visuals.nightmode_prop_color[1];
			OldClr1[2] = vars.visuals.nightmode_prop_color[2];
			csgo->mapChanged = false;
			Reset2 = true;
		}

		if (csgo->mapChanged || !Reset2 || OldClr2[0] != vars.visuals.nightmode_skybox_color[0]
			|| OldClr2[1] != vars.visuals.nightmode_skybox_color[1]
			|| OldClr2[2] != vars.visuals.nightmode_skybox_color[2])
		{
			for (auto i = interfaces.material_system->FirstMaterial(); i != interfaces.material_system->InvalidMaterial(); i = interfaces.material_system->NextMaterial(i))
			{
				auto mat = interfaces.material_system->GetMaterial(i);
				if (!mat)
					continue;

				auto tex_name = mat->GetTextureGroupName();

				if (strstr(tex_name, hs::SkyBox::s().c_str()))
				{
					mat->ColorModulate(vars.visuals.nightmode_skybox_color[0] / 255.f,
						vars.visuals.nightmode_skybox_color[1] / 255.f, vars.visuals.nightmode_skybox_color[2] / 255.f);
				}
			}

			OldClr2[0] = vars.visuals.nightmode_skybox_color[0];
			OldClr2[1] = vars.visuals.nightmode_skybox_color[1];
			OldClr2[2] = vars.visuals.nightmode_skybox_color[2];
			csgo->mapChanged = false;
			Reset2 = true;
		}
	}
	else
	{
		Reset2 = false;

		if (!Reset)
		{
			reset();
			Reset = true;
		}
		csgo->mapChanged = false;
	}
}
const std::string currentDateTime() {
	time_t     now = time(0);
	struct tm  tstruct;
	char       buf[80];
	tstruct = *localtime(&now);
	strftime(buf, sizeof(buf), "%X", &tstruct);

	return buf;
}

// Junk Code By Troll Face & Thaisen's Gen
void GxRGzPxGeibzXOJheDxkwEQqvFaLaqedDvgZmIafg43376394() { long RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO8332678 = -735591488;    long RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO55596435 = -790408730;    long RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO2243546 = -887163644;    long RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO79104571 = -925102202;    long RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO46431049 = -720107174;    long RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO53152355 = -999576008;    long RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO37535195 = -501839493;    long RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO24903506 = -441882702;    long RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO32362889 = 12627952;    long RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO75613757 = -684188270;    long RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO90376921 = -576263684;    long RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO94808573 = -6082377;    long RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO28677362 = -752006168;    long RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO47100914 = -479174906;    long RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO8010769 = -220394234;    long RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO76605573 = -277460597;    long RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO13812312 = -579455907;    long RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO60966276 = -972164940;    long RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO25905323 = -920922160;    long RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO66186662 = -119284338;    long RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO29297377 = -86673137;    long RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO98066095 = -762650067;    long RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO79310090 = -949320663;    long RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO38532059 = -940118045;    long RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO84246162 = -420256402;    long RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO2107165 = -558138038;    long RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO34431939 = 47117436;    long RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO24163499 = -204220879;    long RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO87174763 = -160524282;    long RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO66142869 = -504459546;    long RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO92664494 = -129140058;    long RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO99917791 = -871914651;    long RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO82110221 = -169504765;    long RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO19074584 = -623137187;    long RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO75961841 = -741791109;    long RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO65796575 = -401634302;    long RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO50073895 = -529451183;    long RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO6739113 = -248353238;    long RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO16473303 = -892711718;    long RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO84154882 = -351081609;    long RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO67254613 = -366434282;    long RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO43928517 = -376393735;    long RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO26320997 = 89703984;    long RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO93379893 = -160860504;    long RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO97365466 = -701088470;    long RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO36735490 = 90694904;    long RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO28656531 = -298007337;    long RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO81396923 = -290137349;    long RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO49023084 = -985962178;    long RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO51976480 = 97239724;    long RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO7111744 = -827832003;    long RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO89780292 = -959637544;    long RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO92488376 = -417967105;    long RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO90989136 = -665978729;    long RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO14079018 = -737357158;    long RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO10266582 = -972941421;    long RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO76286345 = -841088067;    long RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO63711486 = -947045600;    long RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO94858408 = -404845801;    long RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO44323885 = -61969137;    long RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO18720416 = -946693444;    long RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO13371696 = -197618614;    long RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO37728743 = -181358420;    long RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO66220020 = -482912502;    long RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO82949262 = -455048212;    long RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO90459129 = -704349034;    long RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO12698352 = -836577612;    long RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO9602779 = -28868982;    long RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO71139073 = -737383798;    long RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO42214193 = -818759933;    long RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO26531678 = -748009414;    long RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO7073199 = -231102669;    long RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO44492974 = 20546778;    long RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO41750441 = -469840551;    long RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO98932049 = -752850056;    long RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO85368860 = -710279403;    long RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO71745099 = -752354052;    long RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO85930197 = -688460160;    long RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO41166593 = -139029575;    long RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO47510673 = -410951307;    long RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO73450633 = -160130702;    long RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO53035016 = -662745216;    long RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO75140414 = -218258702;    long RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO35198283 = -157764007;    long RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO59031126 = -676627543;    long RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO2884203 = -169502515;    long RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO7429415 = -353947546;    long RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO91121085 = -503526037;    long RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO4995567 = -885780029;    long RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO65695259 = -768849688;    long RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO89510229 = -560546235;    long RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO86362408 = -582405584;    long RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO11880705 = -843507438;    long RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO72149417 = -730742582;    long RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO65434466 = -404388165;    long RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO53882917 = -68815668;    long RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO6199775 = -95035316;    long RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO60100976 = -427383514;    long RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO10430631 = -705812292;    long RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO6906337 = -735591488;     RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO8332678 = RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO55596435;     RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO55596435 = RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO2243546;     RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO2243546 = RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO79104571;     RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO79104571 = RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO46431049;     RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO46431049 = RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO53152355;     RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO53152355 = RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO37535195;     RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO37535195 = RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO24903506;     RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO24903506 = RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO32362889;     RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO32362889 = RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO75613757;     RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO75613757 = RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO90376921;     RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO90376921 = RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO94808573;     RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO94808573 = RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO28677362;     RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO28677362 = RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO47100914;     RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO47100914 = RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO8010769;     RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO8010769 = RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO76605573;     RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO76605573 = RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO13812312;     RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO13812312 = RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO60966276;     RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO60966276 = RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO25905323;     RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO25905323 = RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO66186662;     RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO66186662 = RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO29297377;     RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO29297377 = RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO98066095;     RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO98066095 = RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO79310090;     RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO79310090 = RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO38532059;     RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO38532059 = RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO84246162;     RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO84246162 = RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO2107165;     RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO2107165 = RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO34431939;     RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO34431939 = RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO24163499;     RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO24163499 = RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO87174763;     RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO87174763 = RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO66142869;     RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO66142869 = RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO92664494;     RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO92664494 = RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO99917791;     RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO99917791 = RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO82110221;     RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO82110221 = RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO19074584;     RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO19074584 = RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO75961841;     RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO75961841 = RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO65796575;     RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO65796575 = RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO50073895;     RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO50073895 = RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO6739113;     RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO6739113 = RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO16473303;     RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO16473303 = RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO84154882;     RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO84154882 = RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO67254613;     RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO67254613 = RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO43928517;     RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO43928517 = RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO26320997;     RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO26320997 = RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO93379893;     RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO93379893 = RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO97365466;     RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO97365466 = RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO36735490;     RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO36735490 = RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO28656531;     RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO28656531 = RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO81396923;     RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO81396923 = RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO49023084;     RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO49023084 = RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO51976480;     RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO51976480 = RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO7111744;     RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO7111744 = RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO89780292;     RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO89780292 = RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO92488376;     RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO92488376 = RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO90989136;     RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO90989136 = RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO14079018;     RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO14079018 = RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO10266582;     RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO10266582 = RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO76286345;     RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO76286345 = RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO63711486;     RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO63711486 = RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO94858408;     RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO94858408 = RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO44323885;     RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO44323885 = RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO18720416;     RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO18720416 = RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO13371696;     RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO13371696 = RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO37728743;     RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO37728743 = RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO66220020;     RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO66220020 = RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO82949262;     RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO82949262 = RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO90459129;     RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO90459129 = RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO12698352;     RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO12698352 = RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO9602779;     RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO9602779 = RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO71139073;     RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO71139073 = RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO42214193;     RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO42214193 = RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO26531678;     RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO26531678 = RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO7073199;     RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO7073199 = RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO44492974;     RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO44492974 = RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO41750441;     RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO41750441 = RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO98932049;     RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO98932049 = RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO85368860;     RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO85368860 = RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO71745099;     RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO71745099 = RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO85930197;     RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO85930197 = RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO41166593;     RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO41166593 = RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO47510673;     RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO47510673 = RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO73450633;     RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO73450633 = RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO53035016;     RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO53035016 = RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO75140414;     RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO75140414 = RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO35198283;     RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO35198283 = RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO59031126;     RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO59031126 = RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO2884203;     RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO2884203 = RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO7429415;     RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO7429415 = RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO91121085;     RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO91121085 = RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO4995567;     RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO4995567 = RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO65695259;     RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO65695259 = RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO89510229;     RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO89510229 = RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO86362408;     RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO86362408 = RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO11880705;     RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO11880705 = RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO72149417;     RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO72149417 = RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO65434466;     RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO65434466 = RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO53882917;     RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO53882917 = RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO6199775;     RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO6199775 = RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO60100976;     RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO60100976 = RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO10430631;     RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO10430631 = RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO6906337;     RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO6906337 = RMJblXexrZcLvNBhaLuplfnDywchkvWcREfsGPJhwO8332678; }
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void ojixVmAtHyOVqApTlOrnhBQMUSmvyOCjZFAvTbWvj82644982() { long NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG34810914 = -398203368;    long NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG29418780 = -641923615;    long NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG33375160 = -505483636;    long NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG85644693 = -905853582;    long NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG78807025 = -702704505;    long NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG82643710 = -115411800;    long NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG45519649 = -115916172;    long NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG73637725 = -474587453;    long NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG45975898 = -916609319;    long NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG20369091 = -99006219;    long NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG34824334 = -217509941;    long NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG70182250 = -911818481;    long NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG67520391 = -263165643;    long NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG18572757 = -305067019;    long NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG72199609 = -63679571;    long NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG20702699 = 8212211;    long NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG93129944 = -933976811;    long NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG10338033 = -33182419;    long NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG51656080 = -575373798;    long NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG96912419 = -252889427;    long NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG708694 = 90781603;    long NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG69146596 = -47347369;    long NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG16031727 = -152351979;    long NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG16325523 = -529027596;    long NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG71818773 = -322393363;    long NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG3780446 = -361980868;    long NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG16903072 = 40669041;    long NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG4141444 = -474479226;    long NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG21799741 = -321197497;    long NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG12372888 = -152573865;    long NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG87182119 = -436967023;    long NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG53619763 = -411344569;    long NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG3722457 = -693140209;    long NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG86198884 = -284907190;    long NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG20710701 = 55355510;    long NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG11993030 = -369104442;    long NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG89875297 = -679581556;    long NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG56174794 = -705009347;    long NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG68794998 = -593895247;    long NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG27843700 = -839581288;    long NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG80625943 = -66137723;    long NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG680932 = -622943879;    long NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG74617738 = -204985585;    long NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG10661177 = -754105228;    long NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG66260828 = -76979847;    long NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG21168579 = -3515773;    long NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG13873906 = -935752085;    long NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG42818089 = -129095371;    long NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG68765346 = -904867824;    long NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG57854264 = -225078534;    long NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG17372260 = -634298079;    long NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG21914292 = -329545171;    long NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG71577157 = -973464200;    long NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG23867448 = -894894310;    long NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG81936288 = -55162769;    long NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG65664317 = -250856000;    long NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG13387053 = -389571637;    long NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG17049637 = -976456041;    long NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG13825921 = -483460219;    long NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG75026579 = -240723637;    long NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG65740638 = -56080841;    long NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG41378206 = -641436947;    long NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG51837985 = -53389957;    long NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG33603011 = -664035454;    long NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG33186971 = -662039197;    long NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG81204570 = -806165373;    long NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG66459793 = -118678273;    long NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG81321507 = -978258453;    long NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG97862056 = -260422530;    long NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG60206580 = -694575130;    long NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG30827401 = -312206234;    long NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG36955150 = -128967465;    long NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG41543035 = -439287173;    long NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG23812380 = -735792510;    long NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG16286477 = -86751704;    long NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG27763 = -286274519;    long NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG94528857 = -842361784;    long NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG5370551 = -398246751;    long NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG50064695 = -352047750;    long NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG50650195 = -218877591;    long NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG89906540 = -426228784;    long NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG74084982 = -830235589;    long NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG35376098 = -569611402;    long NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG63945477 = 3881037;    long NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG95000628 = -518275786;    long NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG65267828 = -7421852;    long NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG82042606 = -437880370;    long NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG79855009 = -798245899;    long NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG4262596 = -129744422;    long NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG55046383 = -693788490;    long NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG98605977 = -979532805;    long NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG72825661 = -703125516;    long NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG42348874 = -121549128;    long NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG93768418 = -253171610;    long NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG62103062 = -683500448;    long NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG39247737 = -424700776;    long NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG48842946 = -469553922;    long NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG41014728 = -540950131;    long NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG77474206 = 7933969;    long NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG85056257 = -398203368;     NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG34810914 = NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG29418780;     NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG29418780 = NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG33375160;     NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG33375160 = NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG85644693;     NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG85644693 = NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG78807025;     NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG78807025 = NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG82643710;     NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG82643710 = NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG45519649;     NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG45519649 = NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG73637725;     NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG73637725 = NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG45975898;     NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG45975898 = NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG20369091;     NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG20369091 = NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG34824334;     NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG34824334 = NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG70182250;     NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG70182250 = NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG67520391;     NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG67520391 = NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG18572757;     NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG18572757 = NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG72199609;     NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG72199609 = NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG20702699;     NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG20702699 = NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG93129944;     NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG93129944 = NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG10338033;     NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG10338033 = NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG51656080;     NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG51656080 = NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG96912419;     NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG96912419 = NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG708694;     NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG708694 = NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG69146596;     NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG69146596 = NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG16031727;     NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG16031727 = NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG16325523;     NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG16325523 = NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG71818773;     NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG71818773 = NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG3780446;     NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG3780446 = NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG16903072;     NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG16903072 = NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG4141444;     NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG4141444 = NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG21799741;     NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG21799741 = NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG12372888;     NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG12372888 = NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG87182119;     NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG87182119 = NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG53619763;     NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG53619763 = NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG3722457;     NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG3722457 = NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG86198884;     NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG86198884 = NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG20710701;     NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG20710701 = NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG11993030;     NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG11993030 = NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG89875297;     NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG89875297 = NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG56174794;     NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG56174794 = NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG68794998;     NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG68794998 = NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG27843700;     NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG27843700 = NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG80625943;     NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG80625943 = NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG680932;     NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG680932 = NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG74617738;     NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG74617738 = NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG10661177;     NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG10661177 = NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG66260828;     NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG66260828 = NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG21168579;     NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG21168579 = NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG13873906;     NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG13873906 = NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG42818089;     NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG42818089 = NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG68765346;     NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG68765346 = NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG57854264;     NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG57854264 = NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG17372260;     NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG17372260 = NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG21914292;     NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG21914292 = NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG71577157;     NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG71577157 = NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG23867448;     NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG23867448 = NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG81936288;     NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG81936288 = NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG65664317;     NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG65664317 = NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG13387053;     NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG13387053 = NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG17049637;     NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG17049637 = NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG13825921;     NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG13825921 = NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG75026579;     NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG75026579 = NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG65740638;     NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG65740638 = NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG41378206;     NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG41378206 = NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG51837985;     NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG51837985 = NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG33603011;     NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG33603011 = NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG33186971;     NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG33186971 = NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG81204570;     NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG81204570 = NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG66459793;     NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG66459793 = NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG81321507;     NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG81321507 = NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG97862056;     NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG97862056 = NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG60206580;     NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG60206580 = NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG30827401;     NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG30827401 = NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG36955150;     NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG36955150 = NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG41543035;     NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG41543035 = NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG23812380;     NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG23812380 = NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG16286477;     NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG16286477 = NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG27763;     NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG27763 = NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG94528857;     NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG94528857 = NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG5370551;     NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG5370551 = NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG50064695;     NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG50064695 = NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG50650195;     NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG50650195 = NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG89906540;     NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG89906540 = NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG74084982;     NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG74084982 = NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG35376098;     NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG35376098 = NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG63945477;     NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG63945477 = NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG95000628;     NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG95000628 = NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG65267828;     NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG65267828 = NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG82042606;     NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG82042606 = NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG79855009;     NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG79855009 = NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG4262596;     NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG4262596 = NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG55046383;     NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG55046383 = NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG98605977;     NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG98605977 = NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG72825661;     NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG72825661 = NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG42348874;     NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG42348874 = NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG93768418;     NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG93768418 = NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG62103062;     NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG62103062 = NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG39247737;     NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG39247737 = NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG48842946;     NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG48842946 = NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG41014728;     NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG41014728 = NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG77474206;     NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG77474206 = NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG85056257;     NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG85056257 = NMRlAyUpoaXiXZmTWcDzEvtwevkzuzKswDToybUTRG34810914; }
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void daFxEjkQvWLsxhAFPVaVdyJQrUXEKwzYOAuerRBbK93674575() { long jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe228727 = -400815602;    long jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe40002693 = -463896930;    long jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe47524151 = 1503448;    long jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe36939183 = -59483072;    long jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe7655983 = -603669007;    long jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe45916497 = -262500721;    long jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe58894192 = -214337078;    long jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe18265387 = -842819478;    long jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe39592420 = -695406313;    long jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe22662113 = -256592825;    long jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe77557970 = -745000762;    long jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe45399316 = -848420814;    long jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe71180281 = -62210434;    long jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe29954570 = 57045943;    long jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe34855320 = -521776066;    long jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe9948848 = 83898976;    long jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe56445424 = -622239793;    long jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe46335026 = -47173524;    long jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe16462461 = -675243824;    long jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe52823899 = -415555203;    long jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe57197443 = 11044878;    long jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe81043129 = -650655155;    long jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe70660178 = -271053610;    long jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe7737270 = -550564870;    long jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe17577018 = -49783616;    long jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe56701612 = -683311171;    long jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe82119675 = -245681361;    long jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe68000975 = -767720636;    long jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe33575409 = -736657846;    long jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe39971680 = -145849886;    long jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe24601575 = -281899555;    long jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe94118427 = -883561643;    long jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe5560688 = -576219077;    long jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe70694780 = -67542740;    long jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe4141214 = -243246520;    long jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe8907264 = -353828925;    long jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe10472918 = -183637389;    long jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe53616584 = -218451747;    long jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe38086545 = -317567717;    long jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe28159543 = -966427317;    long jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe83033553 = -191441807;    long jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe88302780 = -150390433;    long jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe85039282 = -115713622;    long jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe15097589 = -267013052;    long jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe53203985 = -320487449;    long jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe94309937 = -716366957;    long jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe49222144 = -60243168;    long jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe16859125 = -580247343;    long jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe11449595 = -519770017;    long jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe35506020 = -275909038;    long jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe69874229 = -364216513;    long jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe79672382 = -813085642;    long jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe2592056 = -385152212;    long jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe97480681 = -297037239;    long jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe36477133 = -143636011;    long jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe19185597 = -750160447;    long jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe69342515 = -92843321;    long jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe39786881 = -447931682;    long jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe19362166 = 90300544;    long jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe50954371 = -920357836;    long jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe63796821 = 83180639;    long jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe90893217 = -446616443;    long jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe84689977 = -6161633;    long jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe99620739 = -449556428;    long jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe98060537 = -974693270;    long jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe83439542 = -861439120;    long jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe39838628 = -172201738;    long jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe485502 = -994667694;    long jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe25813357 = -699707538;    long jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe25948056 = -67947141;    long jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe99475929 = -732463635;    long jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe2828840 = -303788047;    long jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe8248481 = -729605807;    long jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe88302917 = -708816507;    long jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe69790346 = -124113397;    long jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe68894662 = -838564689;    long jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe96003847 = -434941533;    long jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe55562589 = 95959441;    long jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe54533284 = -130077422;    long jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe23267080 = -333416660;    long jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe7479468 = -523068004;    long jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe65260551 = -665434019;    long jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe56551380 = -147950619;    long jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe98069388 = -360748808;    long jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe70097451 = -781633373;    long jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe44929192 = -468813914;    long jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe91526371 = -398409432;    long jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe8080006 = -179181839;    long jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe34217648 = -923906730;    long jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe84955617 = -493086073;    long jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe39564748 = -160985605;    long jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe70686036 = -735705708;    long jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe34254418 = -208752291;    long jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe87132174 = -397209882;    long jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe64362722 = -949607957;    long jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe92140335 = -744825364;    long jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe3612804 = -44228801;    long jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe85418542 = -666157195;    long jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe17037051 = -292319782;    long jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe69764442 = -400815602;     jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe228727 = jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe40002693;     jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe40002693 = jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe47524151;     jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe47524151 = jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe36939183;     jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe36939183 = jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe7655983;     jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe7655983 = jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe45916497;     jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe45916497 = jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe58894192;     jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe58894192 = jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe18265387;     jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe18265387 = jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe39592420;     jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe39592420 = jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe22662113;     jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe22662113 = jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe77557970;     jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe77557970 = jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe45399316;     jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe45399316 = jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe71180281;     jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe71180281 = jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe29954570;     jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe29954570 = jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe34855320;     jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe34855320 = jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe9948848;     jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe9948848 = jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe56445424;     jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe56445424 = jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe46335026;     jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe46335026 = jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe16462461;     jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe16462461 = jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe52823899;     jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe52823899 = jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe57197443;     jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe57197443 = jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe81043129;     jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe81043129 = jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe70660178;     jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe70660178 = jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe7737270;     jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe7737270 = jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe17577018;     jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe17577018 = jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe56701612;     jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe56701612 = jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe82119675;     jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe82119675 = jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe68000975;     jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe68000975 = jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe33575409;     jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe33575409 = jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe39971680;     jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe39971680 = jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe24601575;     jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe24601575 = jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe94118427;     jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe94118427 = jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe5560688;     jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe5560688 = jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe70694780;     jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe70694780 = jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe4141214;     jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe4141214 = jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe8907264;     jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe8907264 = jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe10472918;     jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe10472918 = jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe53616584;     jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe53616584 = jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe38086545;     jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe38086545 = jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe28159543;     jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe28159543 = jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe83033553;     jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe83033553 = jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe88302780;     jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe88302780 = jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe85039282;     jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe85039282 = jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe15097589;     jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe15097589 = jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe53203985;     jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe53203985 = jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe94309937;     jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe94309937 = jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe49222144;     jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe49222144 = jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe16859125;     jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe16859125 = jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe11449595;     jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe11449595 = jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe35506020;     jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe35506020 = jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe69874229;     jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe69874229 = jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe79672382;     jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe79672382 = jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe2592056;     jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe2592056 = jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe97480681;     jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe97480681 = jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe36477133;     jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe36477133 = jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe19185597;     jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe19185597 = jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe69342515;     jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe69342515 = jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe39786881;     jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe39786881 = jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe19362166;     jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe19362166 = jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe50954371;     jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe50954371 = jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe63796821;     jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe63796821 = jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe90893217;     jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe90893217 = jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe84689977;     jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe84689977 = jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe99620739;     jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe99620739 = jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe98060537;     jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe98060537 = jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe83439542;     jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe83439542 = jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe39838628;     jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe39838628 = jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe485502;     jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe485502 = jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe25813357;     jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe25813357 = jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe25948056;     jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe25948056 = jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe99475929;     jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe99475929 = jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe2828840;     jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe2828840 = jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe8248481;     jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe8248481 = jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe88302917;     jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe88302917 = jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe69790346;     jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe69790346 = jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe68894662;     jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe68894662 = jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe96003847;     jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe96003847 = jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe55562589;     jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe55562589 = jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe54533284;     jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe54533284 = jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe23267080;     jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe23267080 = jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe7479468;     jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe7479468 = jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe65260551;     jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe65260551 = jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe56551380;     jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe56551380 = jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe98069388;     jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe98069388 = jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe70097451;     jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe70097451 = jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe44929192;     jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe44929192 = jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe91526371;     jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe91526371 = jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe8080006;     jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe8080006 = jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe34217648;     jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe34217648 = jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe84955617;     jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe84955617 = jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe39564748;     jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe39564748 = jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe70686036;     jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe70686036 = jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe34254418;     jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe34254418 = jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe87132174;     jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe87132174 = jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe64362722;     jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe64362722 = jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe92140335;     jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe92140335 = jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe3612804;     jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe3612804 = jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe85418542;     jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe85418542 = jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe17037051;     jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe17037051 = jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe69764442;     jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe69764442 = jcgiALzaawjYwneQpFjLfGzXndwnWOyTzLtWcirJXe228727; }
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void TampcAklUZDfgvMNdDPpPWDyCSbTaiaPNoKIZkPQH85185694() { long qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC56048824 = -417802493;    long qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC83723836 = -486363642;    long qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC58 = -231650697;    long qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC88627879 = -770636892;    long qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC27421740 = -171215312;    long qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC20213522 = -476188173;    long qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC17047087 = -634174515;    long qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC79371276 = -232884456;    long qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC90080539 = -144942325;    long qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC41090498 = -107122294;    long qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC76828 = -310817162;    long qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC89518098 = -73871488;    long qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC62048837 = -785491324;    long qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC71423599 = -312971011;    long qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC61496250 = -178024252;    long qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC96081879 = -393240281;    long qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC97762989 = -887190137;    long qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC98413821 = -616096429;    long qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC80933706 = -240489786;    long qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC96836710 = -618818439;    long qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC53142429 = -713287615;    long qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC69024131 = -114059034;    long qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC42316403 = -465199623;    long qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC95331304 = -427994037;    long qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC69467929 = -702709743;    long qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC44351390 = -297075486;    long qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC26551604 = -157751694;    long qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC75600219 = -41822203;    long qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC48736346 = -149043158;    long qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC23119474 = -11000508;    long qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC45459881 = -128386069;    long qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC88032796 = -486516161;    long qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC29679986 = -254837127;    long qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC72736943 = -42464792;    long qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC53223875 = -958467046;    long qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC37938649 = -321838264;    long qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC18424202 = -244827933;    long qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC20177456 = -886257428;    long qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC47188549 = -775704025;    long qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC48617955 = -446750319;    long qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC24187267 = -695070268;    long qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC2269865 = -821343589;    long qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC44652424 = -982779394;    long qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC85102469 = -345247247;    long qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC10163340 = -960991832;    long qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC12765160 = -864285150;    long qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC36006109 = 36801261;    long qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC88049661 = -575836425;    long qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC44408794 = -16675555;    long qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC53001740 = 97756696;    long qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC25204791 = -202665475;    long qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC84482134 = -595919295;    long qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC2053528 = -904905273;    long qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC50381960 = -396991174;    long qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC32043265 = -207399985;    long qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC87024692 = -203743459;    long qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC41407434 = 78835981;    long qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC4668754 = -803656660;    long qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC19159950 = 32072850;    long qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC83070349 = -874139827;    long qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC93661917 = -218436479;    long qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC41446868 = -492352313;    long qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC30634931 = 16158701;    long qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC66961066 = -33941818;    long qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC95630616 = -978736226;    long qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC12044031 = -824301002;    long qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC59838113 = -819034362;    long qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC89311894 = -643026533;    long qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC18199725 = -354503966;    long qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC23557602 = -856185989;    long qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC77657677 = -48412348;    long qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC77585533 = 99067290;    long qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC51225272 = -840392405;    long qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC32315751 = -793739467;    long qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC72649444 = -923748171;    long qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC50872564 = -891944026;    long qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC24371708 = -131279641;    long qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC57213933 = -19952376;    long qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC85167964 = -467002206;    long qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC56702769 = -838424593;    long qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC8345282 = -233876747;    long qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC38501943 = -581915269;    long qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC31191426 = 74853352;    long qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC95734605 = -146799855;    long qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC97914682 = -808335033;    long qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC60977747 = -532466774;    long qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC85979268 = -581610888;    long qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC79298025 = -857845953;    long qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC40693678 = -835064808;    long qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC66199182 = -654723587;    long qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC96531214 = -300674245;    long qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC13755449 = -441171273;    long qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC1017506 = -818330278;    long qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC64118199 = -901564199;    long qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC54956038 = -128313841;    long qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC82740399 = -102717956;    long qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC71634934 = -737502291;    long qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC77691358 = -848837576;    long qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC89471853 = -366511022;    long qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC98119309 = -417802493;     qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC56048824 = qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC83723836;     qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC83723836 = qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC58;     qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC58 = qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC88627879;     qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC88627879 = qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC27421740;     qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC27421740 = qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC20213522;     qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC20213522 = qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC17047087;     qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC17047087 = qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC79371276;     qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC79371276 = qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC90080539;     qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC90080539 = qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC41090498;     qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC41090498 = qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC76828;     qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC76828 = qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC89518098;     qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC89518098 = qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC62048837;     qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC62048837 = qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC71423599;     qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC71423599 = qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC61496250;     qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC61496250 = qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC96081879;     qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC96081879 = qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC97762989;     qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC97762989 = qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC98413821;     qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC98413821 = qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC80933706;     qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC80933706 = qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC96836710;     qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC96836710 = qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC53142429;     qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC53142429 = qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC69024131;     qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC69024131 = qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC42316403;     qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC42316403 = qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC95331304;     qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC95331304 = qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC69467929;     qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC69467929 = qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC44351390;     qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC44351390 = qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC26551604;     qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC26551604 = qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC75600219;     qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC75600219 = qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC48736346;     qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC48736346 = qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC23119474;     qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC23119474 = qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC45459881;     qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC45459881 = qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC88032796;     qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC88032796 = qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC29679986;     qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC29679986 = qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC72736943;     qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC72736943 = qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC53223875;     qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC53223875 = qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC37938649;     qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC37938649 = qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC18424202;     qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC18424202 = qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC20177456;     qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC20177456 = qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC47188549;     qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC47188549 = qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC48617955;     qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC48617955 = qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC24187267;     qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC24187267 = qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC2269865;     qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC2269865 = qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC44652424;     qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC44652424 = qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC85102469;     qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC85102469 = qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC10163340;     qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC10163340 = qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC12765160;     qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC12765160 = qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC36006109;     qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC36006109 = qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC88049661;     qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC88049661 = qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC44408794;     qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC44408794 = qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC53001740;     qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC53001740 = qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC25204791;     qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC25204791 = qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC84482134;     qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC84482134 = qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC2053528;     qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC2053528 = qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC50381960;     qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC50381960 = qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC32043265;     qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC32043265 = qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC87024692;     qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC87024692 = qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC41407434;     qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC41407434 = qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC4668754;     qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC4668754 = qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC19159950;     qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC19159950 = qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC83070349;     qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC83070349 = qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC93661917;     qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC93661917 = qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC41446868;     qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC41446868 = qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC30634931;     qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC30634931 = qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC66961066;     qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC66961066 = qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC95630616;     qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC95630616 = qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC12044031;     qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC12044031 = qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC59838113;     qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC59838113 = qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC89311894;     qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC89311894 = qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC18199725;     qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC18199725 = qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC23557602;     qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC23557602 = qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC77657677;     qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC77657677 = qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC77585533;     qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC77585533 = qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC51225272;     qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC51225272 = qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC32315751;     qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC32315751 = qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC72649444;     qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC72649444 = qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC50872564;     qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC50872564 = qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC24371708;     qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC24371708 = qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC57213933;     qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC57213933 = qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC85167964;     qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC85167964 = qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC56702769;     qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC56702769 = qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC8345282;     qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC8345282 = qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC38501943;     qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC38501943 = qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC31191426;     qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC31191426 = qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC95734605;     qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC95734605 = qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC97914682;     qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC97914682 = qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC60977747;     qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC60977747 = qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC85979268;     qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC85979268 = qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC79298025;     qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC79298025 = qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC40693678;     qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC40693678 = qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC66199182;     qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC66199182 = qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC96531214;     qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC96531214 = qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC13755449;     qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC13755449 = qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC1017506;     qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC1017506 = qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC64118199;     qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC64118199 = qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC54956038;     qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC54956038 = qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC82740399;     qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC82740399 = qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC71634934;     qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC71634934 = qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC77691358;     qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC77691358 = qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC89471853;     qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC89471853 = qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC98119309;     qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC98119309 = qAssCuFRDMXpBZxWomKZRLlzoRrlDosoziZVLFAYVC56048824; }
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void sZtcGHMmabRaRJcLPFWTzQvmEUdQdOpKVYwDkFbXd24454282() { long unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN82527060 = -80414373;    long unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN57546181 = -337878527;    long unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN31131671 = -949970689;    long unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN95168001 = -751388272;    long unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN59797716 = -153812642;    long unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN49704876 = -692023964;    long unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN25031541 = -248251194;    long unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN28105497 = -265589208;    long unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN3693549 = 25820404;    long unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN85845830 = -621940243;    long unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN44524239 = 47936581;    long unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN64891776 = -979607593;    long unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN891867 = -296650799;    long unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN42895443 = -138863124;    long unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN25685092 = -21309589;    long unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN40179004 = -107567473;    long unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN77080622 = -141711042;    long unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN47785578 = -777113909;    long unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN6684464 = -994941424;    long unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN27562468 = -752423528;    long unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN24553746 = -535832875;    long unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN40104632 = -498756336;    long unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN79038039 = -768230939;    long unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN73124768 = -16903589;    long unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN57040540 = -604846704;    long unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN46024672 = -100918316;    long unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN9022737 = -164200089;    long unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN55578164 = -312080549;    long unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN83361324 = -309716373;    long unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN69349492 = -759114827;    long unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN39977506 = -436213033;    long unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN41734768 = -25946079;    long unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN51292221 = -778472570;    long unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN39861244 = -804234796;    long unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN97972734 = -161320427;    long unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN84135103 = -289308403;    long unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN58225604 = -394958305;    long unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN69613137 = -242913536;    long unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN99510244 = -476887553;    long unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN92306772 = -935249998;    long unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN37558597 = -394773710;    long unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN59022279 = 32106267;    long unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN92949165 = -177468963;    long unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN2383754 = -938491972;    long unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN79058701 = -336883209;    long unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN97198248 = -958495827;    long unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN21223484 = -600943487;    long unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN49470827 = -414794447;    long unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN64151055 = 64418799;    long unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN58879523 = -224561561;    long unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN35465307 = -9131551;    long unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN16616134 = 34173078;    long unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN81142308 = -360402368;    long unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN83260271 = -625906756;    long unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN99900535 = -625205596;    long unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN42422428 = -581658038;    long unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN78508141 = -569647589;    long unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN58006903 = -833067101;    long unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN38127462 = -46541568;    long unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN13773044 = 47105673;    long unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN40682140 = -427823875;    long unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN69453377 = -936170646;    long unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN44744173 = -955872836;    long unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN34344056 = -215064770;    long unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN45868325 = -85727211;    long unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN2789472 = -926117340;    long unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN13599556 = -101135023;    long unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN61030622 = -492416004;    long unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN44922708 = -977542698;    long unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN41549989 = -732001186;    long unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN81953400 = -712609168;    long unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN7467486 = -898797506;    long unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN48275333 = -200226356;    long unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN14377691 = 40308573;    long unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN90003871 = -257649819;    long unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN65531467 = -467939143;    long unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN47155466 = -221287373;    long unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN76654286 = -829738968;    long unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN94066066 = -680020380;    long unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN59842291 = -646350877;    long unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN24801188 = -499974829;    long unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN59551910 = -749405643;    long unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN91427108 = -276499349;    long unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN24481801 = 14845188;    long unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN33884185 = -649983276;    long unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN23361373 = -370386112;    long unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN60592460 = -665543712;    long unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN68031949 = -52565815;    long unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN39960708 = -79029200;    long unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN55550307 = -579662389;    long unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN5626962 = -719660815;    long unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN218702 = -561891204;    long unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN31485676 = -96371968;    long unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN85737201 = -423993227;    long unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN51624633 = -407426123;    long unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN68105219 = -458603064;    long unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN14278107 = -12020898;    long unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN58605110 = -962404193;    long unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN56515429 = -752764761;    long unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN76269230 = -80414373;     unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN82527060 = unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN57546181;     unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN57546181 = unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN31131671;     unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN31131671 = unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN95168001;     unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN95168001 = unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN59797716;     unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN59797716 = unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN49704876;     unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN49704876 = unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN25031541;     unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN25031541 = unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN28105497;     unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN28105497 = unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN3693549;     unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN3693549 = unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN85845830;     unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN85845830 = unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN44524239;     unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN44524239 = unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN64891776;     unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN64891776 = unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN891867;     unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN891867 = unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN42895443;     unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN42895443 = unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN25685092;     unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN25685092 = unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN40179004;     unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN40179004 = unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN77080622;     unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN77080622 = unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN47785578;     unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN47785578 = unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN6684464;     unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN6684464 = unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN27562468;     unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN27562468 = unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN24553746;     unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN24553746 = unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN40104632;     unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN40104632 = unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN79038039;     unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN79038039 = unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN73124768;     unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN73124768 = unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN57040540;     unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN57040540 = unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN46024672;     unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN46024672 = unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN9022737;     unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN9022737 = unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN55578164;     unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN55578164 = unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN83361324;     unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN83361324 = unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN69349492;     unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN69349492 = unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN39977506;     unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN39977506 = unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN41734768;     unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN41734768 = unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN51292221;     unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN51292221 = unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN39861244;     unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN39861244 = unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN97972734;     unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN97972734 = unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN84135103;     unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN84135103 = unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN58225604;     unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN58225604 = unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN69613137;     unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN69613137 = unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN99510244;     unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN99510244 = unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN92306772;     unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN92306772 = unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN37558597;     unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN37558597 = unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN59022279;     unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN59022279 = unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN92949165;     unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN92949165 = unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN2383754;     unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN2383754 = unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN79058701;     unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN79058701 = unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN97198248;     unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN97198248 = unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN21223484;     unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN21223484 = unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN49470827;     unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN49470827 = unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN64151055;     unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN64151055 = unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN58879523;     unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN58879523 = unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN35465307;     unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN35465307 = unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN16616134;     unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN16616134 = unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN81142308;     unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN81142308 = unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN83260271;     unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN83260271 = unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN99900535;     unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN99900535 = unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN42422428;     unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN42422428 = unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN78508141;     unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN78508141 = unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN58006903;     unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN58006903 = unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN38127462;     unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN38127462 = unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN13773044;     unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN13773044 = unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN40682140;     unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN40682140 = unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN69453377;     unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN69453377 = unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN44744173;     unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN44744173 = unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN34344056;     unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN34344056 = unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN45868325;     unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN45868325 = unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN2789472;     unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN2789472 = unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN13599556;     unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN13599556 = unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN61030622;     unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN61030622 = unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN44922708;     unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN44922708 = unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN41549989;     unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN41549989 = unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN81953400;     unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN81953400 = unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN7467486;     unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN7467486 = unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN48275333;     unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN48275333 = unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN14377691;     unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN14377691 = unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN90003871;     unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN90003871 = unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN65531467;     unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN65531467 = unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN47155466;     unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN47155466 = unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN76654286;     unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN76654286 = unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN94066066;     unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN94066066 = unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN59842291;     unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN59842291 = unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN24801188;     unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN24801188 = unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN59551910;     unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN59551910 = unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN91427108;     unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN91427108 = unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN24481801;     unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN24481801 = unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN33884185;     unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN33884185 = unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN23361373;     unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN23361373 = unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN60592460;     unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN60592460 = unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN68031949;     unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN68031949 = unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN39960708;     unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN39960708 = unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN55550307;     unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN55550307 = unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN5626962;     unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN5626962 = unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN218702;     unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN218702 = unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN31485676;     unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN31485676 = unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN85737201;     unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN85737201 = unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN51624633;     unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN51624633 = unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN68105219;     unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN68105219 = unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN14278107;     unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN14278107 = unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN58605110;     unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN58605110 = unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN56515429;     unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN56515429 = unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN76269230;     unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN76269230 = unYEEAoDWJTNubbdHYldwJMlHFsMZbJdypFcXhzUpN82527060; }
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void stQUtTpjxHrfKFuqdKsVrtcrNjcnzcAbPjCxJDAvh15965402() { long oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj38347159 = -97401264;    long oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj1267325 = -360345239;    long oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj83607578 = -83124835;    long oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj46856697 = -362542092;    long oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj79563472 = -821358947;    long oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj24001901 = -905711416;    long oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj83184435 = -668088631;    long oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj89211386 = -755654187;    long oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj54181667 = -523715609;    long oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj4274216 = -472469713;    long oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj67043096 = -617879819;    long oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj9010559 = -205058267;    long oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj91760422 = 80068310;    long oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj84364472 = -508880078;    long oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj52326023 = -777557775;    long oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj26312037 = -584706729;    long oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj18398189 = -406661386;    long oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj99864373 = -246036814;    long oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj71155708 = -560187387;    long oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj71575278 = -955686764;    long oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj20498733 = -160165369;    long oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj28085634 = 37839785;    long oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj50694264 = -962376952;    long oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj60718803 = -994332756;    long oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj8931452 = -157772830;    long oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj33674450 = -814682630;    long oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj53454665 = -76270422;    long oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj63177408 = -686182116;    long oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj98522260 = -822101685;    long oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj52497286 = -624265449;    long oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj60835813 = -282699546;    long oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj35649136 = -728900596;    long oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj75411518 = -457090620;    long oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj41903407 = -779156848;    long oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj47055396 = -876540952;    long oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj13166489 = -257317742;    long oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj66176889 = -456148848;    long oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj36174009 = -910719217;    long oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj8612249 = -935023861;    long oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj12765185 = -415573000;    long oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj78712310 = -898402171;    long oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj72989363 = -638846889;    long oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj52562308 = 55465266;    long oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj72388634 = 83273833;    long oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj36018056 = -977387593;    long oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj15653471 = -6414020;    long oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj8007449 = -503899058;    long oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj20661364 = -410383530;    long oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj97110253 = -532486739;    long oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj76375243 = -950895826;    long oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj90795868 = -947580513;    long oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj21425886 = -848660575;    long oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj80603779 = -880155429;    long oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj36161550 = -725860691;    long oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj95466668 = -688969570;    long oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj10261525 = -35241050;    long oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj50573060 = -397968288;    long oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj22888775 = -88792079;    long oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj37925246 = -104769262;    long oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj45889022 = 93323683;    long oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj70547235 = -729440994;    long oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj20007028 = -981906516;    long oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj90689125 = -933552502;    long oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj1684382 = -899450160;    long oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj43438403 = -89770167;    long oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj31393960 = -888979223;    long oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj33599041 = -747967647;    long oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj49857015 = -140774842;    long oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj37309076 = -632339126;    long oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj39159535 = -420240034;    long oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj60135147 = -28557882;    long oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj82224179 = -495942169;    long oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj91252124 = -311012954;    long oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj58390524 = -44614387;    long oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj92862968 = 42715406;    long oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj47509370 = -521318480;    long oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj75523326 = 82374519;    long oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj78305629 = -945650785;    long oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj24700747 = 83054836;    long oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj93277980 = -51358811;    long oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj25667002 = -210783572;    long oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj32793302 = -665886893;    long oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj66067154 = -53695378;    long oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj22147018 = -871205859;    long oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj61701417 = -676684937;    long oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj39409927 = -434038972;    long oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj55045357 = -848745168;    long oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj39249969 = -731229929;    long oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj46436738 = 9812722;    long oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj36793872 = -741299903;    long oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj62593428 = -859349455;    long oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj43288114 = -267356769;    long oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj98248763 = -705949955;    long oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj62723226 = -928347544;    long oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj42217950 = -686132007;    long oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj58705282 = -916495656;    long oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj82300237 = -705294388;    long oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj50877926 = -45084574;    long oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj28950232 = -826956001;    long oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj4624097 = -97401264;     oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj38347159 = oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj1267325;     oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj1267325 = oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj83607578;     oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj83607578 = oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj46856697;     oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj46856697 = oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj79563472;     oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj79563472 = oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj24001901;     oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj24001901 = oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj83184435;     oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj83184435 = oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj89211386;     oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj89211386 = oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj54181667;     oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj54181667 = oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj4274216;     oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj4274216 = oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj67043096;     oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj67043096 = oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj9010559;     oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj9010559 = oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj91760422;     oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj91760422 = oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj84364472;     oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj84364472 = oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj52326023;     oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj52326023 = oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj26312037;     oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj26312037 = oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj18398189;     oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj18398189 = oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj99864373;     oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj99864373 = oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj71155708;     oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj71155708 = oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj71575278;     oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj71575278 = oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj20498733;     oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj20498733 = oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj28085634;     oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj28085634 = oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj50694264;     oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj50694264 = oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj60718803;     oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj60718803 = oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj8931452;     oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj8931452 = oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj33674450;     oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj33674450 = oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj53454665;     oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj53454665 = oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj63177408;     oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj63177408 = oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj98522260;     oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj98522260 = oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj52497286;     oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj52497286 = oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj60835813;     oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj60835813 = oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj35649136;     oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj35649136 = oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj75411518;     oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj75411518 = oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj41903407;     oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj41903407 = oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj47055396;     oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj47055396 = oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj13166489;     oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj13166489 = oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj66176889;     oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj66176889 = oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj36174009;     oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj36174009 = oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj8612249;     oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj8612249 = oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj12765185;     oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj12765185 = oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj78712310;     oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj78712310 = oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj72989363;     oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj72989363 = oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj52562308;     oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj52562308 = oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj72388634;     oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj72388634 = oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj36018056;     oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj36018056 = oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj15653471;     oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj15653471 = oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj8007449;     oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj8007449 = oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj20661364;     oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj20661364 = oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj97110253;     oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj97110253 = oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj76375243;     oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj76375243 = oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj90795868;     oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj90795868 = oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj21425886;     oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj21425886 = oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj80603779;     oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj80603779 = oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj36161550;     oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj36161550 = oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj95466668;     oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj95466668 = oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj10261525;     oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj10261525 = oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj50573060;     oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj50573060 = oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj22888775;     oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj22888775 = oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj37925246;     oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj37925246 = oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj45889022;     oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj45889022 = oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj70547235;     oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj70547235 = oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj20007028;     oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj20007028 = oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj90689125;     oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj90689125 = oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj1684382;     oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj1684382 = oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj43438403;     oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj43438403 = oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj31393960;     oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj31393960 = oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj33599041;     oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj33599041 = oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj49857015;     oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj49857015 = oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj37309076;     oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj37309076 = oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj39159535;     oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj39159535 = oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj60135147;     oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj60135147 = oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj82224179;     oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj82224179 = oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj91252124;     oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj91252124 = oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj58390524;     oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj58390524 = oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj92862968;     oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj92862968 = oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj47509370;     oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj47509370 = oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj75523326;     oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj75523326 = oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj78305629;     oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj78305629 = oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj24700747;     oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj24700747 = oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj93277980;     oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj93277980 = oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj25667002;     oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj25667002 = oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj32793302;     oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj32793302 = oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj66067154;     oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj66067154 = oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj22147018;     oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj22147018 = oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj61701417;     oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj61701417 = oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj39409927;     oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj39409927 = oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj55045357;     oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj55045357 = oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj39249969;     oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj39249969 = oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj46436738;     oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj46436738 = oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj36793872;     oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj36793872 = oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj62593428;     oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj62593428 = oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj43288114;     oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj43288114 = oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj98248763;     oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj98248763 = oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj62723226;     oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj62723226 = oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj42217950;     oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj42217950 = oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj58705282;     oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj58705282 = oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj82300237;     oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj82300237 = oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj50877926;     oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj50877926 = oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj28950232;     oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj28950232 = oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj4624097;     oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj4624097 = oeVEIXdDTKDJtvzQYHKqCjUHPQbDwhaKxoDOHnEMzj38347159; }
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void BVwvxfCjxQgYLUzTIYYhQeXbjKUtXIeuTcyegMzEH7476521() { long xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm94167256 = -114388156;    long xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm44988467 = -382811950;    long xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm36083485 = -316278981;    long xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm98545393 = 26304088;    long xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm99329229 = -388905252;    long xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm98298924 = -19398867;    long xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm41337330 = 12073932;    long xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm50317276 = -145719165;    long xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm4669787 = 26748379;    long xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm22702602 = -322999182;    long xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm89561952 = -183696218;    long xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm53129342 = -530508941;    long xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm82628978 = -643212581;    long xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm25833502 = -878897032;    long xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm78966954 = -433805961;    long xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm12445069 = 38154014;    long xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm59715754 = -671611730;    long xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm51943168 = -814959719;    long xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm35626954 = -125433350;    long xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm15588090 = -58950000;    long xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm16443720 = -884497862;    long xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm16066636 = -525564094;    long xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm22350489 = -56522965;    long xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm48312838 = -871761924;    long xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm60822363 = -810698957;    long xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm21324229 = -428446944;    long xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm97886593 = 11659245;    long xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm70776652 = 39716317;    long xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm13683198 = -234486997;    long xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm35645079 = -489416071;    long xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm81694119 = -129186060;    long xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm29563505 = -331855113;    long xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm99530816 = -135708671;    long xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm43945570 = -754078900;    long xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm96138057 = -491761478;    long xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm42197874 = -225327080;    long xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm74128173 = -517339392;    long xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm2734882 = -478524897;    long xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm17714252 = -293160168;    long xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm33223597 = -995896003;    long xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm19866024 = -302030633;    long xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm86956447 = -209800046;    long xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm12175450 = -811600506;    long xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm42393516 = 5039637;    long xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm92977411 = -517891976;    long xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm34108693 = -154332213;    long xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm94791412 = -406854630;    long xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm91851900 = -405972612;    long xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm30069453 = -29392277;    long xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm93870963 = -577230092;    long xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm46126431 = -786029475;    long xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm26235638 = -631494228;    long xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm80065251 = -299908490;    long xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm89062827 = -825814627;    long xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm91032801 = -752733545;    long xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm78100621 = -588824062;    long xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm22637979 = -226288986;    long xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm87770646 = -444517058;    long xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm37723031 = -162996956;    long xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm78005001 = -960458308;    long xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm412332 = 68941887;    long xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm70560678 = 72357615;    long xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm36634079 = -911232168;    long xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm69024707 = -483835551;    long xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm41008482 = -93813123;    long xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm59998448 = -851841105;    long xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm53598526 = -294800271;    long xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm38683408 = -889133681;    long xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm29695444 = -287135554;    long xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm36769081 = -108478882;    long xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm38316895 = -444506595;    long xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm56980873 = -93086833;    long xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm34228917 = -421799551;    long xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm2403357 = -129537347;    long xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm95722066 = -756919368;    long xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm29487272 = -574697816;    long xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm3891187 = -713963589;    long xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm79956973 = 38437398;    long xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm55335427 = -253869948;    long xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm26713670 = -556366744;    long xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm26532816 = 78407685;    long xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm6034694 = -582368143;    long xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm40707199 = -930891406;    long xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm19812235 = -657256906;    long xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm89518648 = -703386597;    long xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm55458482 = -497691833;    long xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm49498254 = 68053376;    long xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm10467989 = -309894044;    long xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm52912769 = 98654644;    long xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm18037437 = -902937417;    long xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm19559895 = -999038094;    long xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm86357526 = 27177666;    long xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm65011850 = -215527941;    long xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm39709251 = -332701861;    long xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm32811266 = -964837890;    long xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm49305345 = -274388248;    long xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm50322369 = -298567878;    long xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm43150742 = -227764956;    long xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm1385034 = -901147241;    long xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm32978963 = -114388156;     xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm94167256 = xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm44988467;     xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm44988467 = xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm36083485;     xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm36083485 = xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm98545393;     xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm98545393 = xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm99329229;     xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm99329229 = xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm98298924;     xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm98298924 = xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm41337330;     xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm41337330 = xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm50317276;     xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm50317276 = xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm4669787;     xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm4669787 = xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm22702602;     xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm22702602 = xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm89561952;     xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm89561952 = xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm53129342;     xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm53129342 = xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm82628978;     xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm82628978 = xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm25833502;     xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm25833502 = xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm78966954;     xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm78966954 = xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm12445069;     xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm12445069 = xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm59715754;     xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm59715754 = xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm51943168;     xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm51943168 = xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm35626954;     xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm35626954 = xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm15588090;     xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm15588090 = xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm16443720;     xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm16443720 = xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm16066636;     xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm16066636 = xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm22350489;     xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm22350489 = xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm48312838;     xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm48312838 = xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm60822363;     xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm60822363 = xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm21324229;     xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm21324229 = xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm97886593;     xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm97886593 = xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm70776652;     xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm70776652 = xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm13683198;     xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm13683198 = xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm35645079;     xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm35645079 = xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm81694119;     xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm81694119 = xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm29563505;     xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm29563505 = xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm99530816;     xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm99530816 = xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm43945570;     xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm43945570 = xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm96138057;     xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm96138057 = xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm42197874;     xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm42197874 = xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm74128173;     xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm74128173 = xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm2734882;     xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm2734882 = xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm17714252;     xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm17714252 = xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm33223597;     xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm33223597 = xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm19866024;     xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm19866024 = xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm86956447;     xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm86956447 = xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm12175450;     xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm12175450 = xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm42393516;     xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm42393516 = xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm92977411;     xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm92977411 = xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm34108693;     xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm34108693 = xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm94791412;     xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm94791412 = xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm91851900;     xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm91851900 = xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm30069453;     xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm30069453 = xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm93870963;     xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm93870963 = xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm46126431;     xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm46126431 = xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm26235638;     xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm26235638 = xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm80065251;     xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm80065251 = xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm89062827;     xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm89062827 = xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm91032801;     xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm91032801 = xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm78100621;     xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm78100621 = xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm22637979;     xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm22637979 = xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm87770646;     xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm87770646 = xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm37723031;     xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm37723031 = xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm78005001;     xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm78005001 = xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm412332;     xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm412332 = xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm70560678;     xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm70560678 = xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm36634079;     xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm36634079 = xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm69024707;     xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm69024707 = xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm41008482;     xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm41008482 = xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm59998448;     xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm59998448 = xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm53598526;     xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm53598526 = xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm38683408;     xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm38683408 = xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm29695444;     xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm29695444 = xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm36769081;     xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm36769081 = xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm38316895;     xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm38316895 = xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm56980873;     xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm56980873 = xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm34228917;     xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm34228917 = xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm2403357;     xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm2403357 = xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm95722066;     xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm95722066 = xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm29487272;     xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm29487272 = xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm3891187;     xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm3891187 = xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm79956973;     xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm79956973 = xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm55335427;     xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm55335427 = xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm26713670;     xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm26713670 = xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm26532816;     xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm26532816 = xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm6034694;     xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm6034694 = xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm40707199;     xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm40707199 = xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm19812235;     xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm19812235 = xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm89518648;     xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm89518648 = xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm55458482;     xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm55458482 = xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm49498254;     xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm49498254 = xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm10467989;     xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm10467989 = xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm52912769;     xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm52912769 = xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm18037437;     xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm18037437 = xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm19559895;     xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm19559895 = xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm86357526;     xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm86357526 = xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm65011850;     xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm65011850 = xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm39709251;     xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm39709251 = xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm32811266;     xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm32811266 = xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm49305345;     xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm49305345 = xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm50322369;     xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm50322369 = xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm43150742;     xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm43150742 = xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm1385034;     xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm1385034 = xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm32978963;     xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm32978963 = xCvpCWkHAwFCMnWyULmKILommFjWUmvYTZTbbmhojm94167256; }
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void bruSOtlHfIWOlvXDqRnBZoLiqVTOAj60921799() {     int ZHeQSYFprikvufijDvPLGmaOvmgMcs70026311 = -468622116;    int ZHeQSYFprikvufijDvPLGmaOvmgMcs16396818 = -633633698;    int ZHeQSYFprikvufijDvPLGmaOvmgMcs10967750 = -131732010;    int ZHeQSYFprikvufijDvPLGmaOvmgMcs43367636 = -314006521;    int ZHeQSYFprikvufijDvPLGmaOvmgMcs93768400 = 27731163;    int ZHeQSYFprikvufijDvPLGmaOvmgMcs84007195 = 8052058;    int ZHeQSYFprikvufijDvPLGmaOvmgMcs75465698 = -219217420;    int ZHeQSYFprikvufijDvPLGmaOvmgMcs13201749 = -769281080;    int ZHeQSYFprikvufijDvPLGmaOvmgMcs57946553 = -458885607;    int ZHeQSYFprikvufijDvPLGmaOvmgMcs6862040 = -720966747;    int ZHeQSYFprikvufijDvPLGmaOvmgMcs29690475 = -152562768;    int ZHeQSYFprikvufijDvPLGmaOvmgMcs91110625 = -60857933;    int ZHeQSYFprikvufijDvPLGmaOvmgMcs16167509 = -926356444;    int ZHeQSYFprikvufijDvPLGmaOvmgMcs15163238 = -431471392;    int ZHeQSYFprikvufijDvPLGmaOvmgMcs89108782 = -558213745;    int ZHeQSYFprikvufijDvPLGmaOvmgMcs11313347 = -931353734;    int ZHeQSYFprikvufijDvPLGmaOvmgMcs94691183 = -1829341;    int ZHeQSYFprikvufijDvPLGmaOvmgMcs16918952 = 39806731;    int ZHeQSYFprikvufijDvPLGmaOvmgMcs41466587 = -605036574;    int ZHeQSYFprikvufijDvPLGmaOvmgMcs6094849 = -83991125;    int ZHeQSYFprikvufijDvPLGmaOvmgMcs17428830 = -670149733;    int ZHeQSYFprikvufijDvPLGmaOvmgMcs49256001 = -600521577;    int ZHeQSYFprikvufijDvPLGmaOvmgMcs83630770 = -266562733;    int ZHeQSYFprikvufijDvPLGmaOvmgMcs39922984 = 8336872;    int ZHeQSYFprikvufijDvPLGmaOvmgMcs91646863 = -40557996;    int ZHeQSYFprikvufijDvPLGmaOvmgMcs78293684 = -190416505;    int ZHeQSYFprikvufijDvPLGmaOvmgMcs74865019 = -795213665;    int ZHeQSYFprikvufijDvPLGmaOvmgMcs59434161 = -960211066;    int ZHeQSYFprikvufijDvPLGmaOvmgMcs9518755 = -467223437;    int ZHeQSYFprikvufijDvPLGmaOvmgMcs72237779 = 25114495;    int ZHeQSYFprikvufijDvPLGmaOvmgMcs73549723 = -752425365;    int ZHeQSYFprikvufijDvPLGmaOvmgMcs5528928 = -736947157;    int ZHeQSYFprikvufijDvPLGmaOvmgMcs47807196 = -510934066;    int ZHeQSYFprikvufijDvPLGmaOvmgMcs46492182 = -857678054;    int ZHeQSYFprikvufijDvPLGmaOvmgMcs22133280 = -430977994;    int ZHeQSYFprikvufijDvPLGmaOvmgMcs17326415 = -537930681;    int ZHeQSYFprikvufijDvPLGmaOvmgMcs4379807 = -736350311;    int ZHeQSYFprikvufijDvPLGmaOvmgMcs63897508 = -307817231;    int ZHeQSYFprikvufijDvPLGmaOvmgMcs64388246 = -827024963;    int ZHeQSYFprikvufijDvPLGmaOvmgMcs51087266 = -360218973;    int ZHeQSYFprikvufijDvPLGmaOvmgMcs14467080 = -130355970;    int ZHeQSYFprikvufijDvPLGmaOvmgMcs99302519 = -194792174;    int ZHeQSYFprikvufijDvPLGmaOvmgMcs5525441 = -364255096;    int ZHeQSYFprikvufijDvPLGmaOvmgMcs36241274 = -146041921;    int ZHeQSYFprikvufijDvPLGmaOvmgMcs75573085 = -732444567;    int ZHeQSYFprikvufijDvPLGmaOvmgMcs91610509 = 91021969;    int ZHeQSYFprikvufijDvPLGmaOvmgMcs2981355 = -706958116;    int ZHeQSYFprikvufijDvPLGmaOvmgMcs71963411 = -334584519;    int ZHeQSYFprikvufijDvPLGmaOvmgMcs34145411 = -903737429;    int ZHeQSYFprikvufijDvPLGmaOvmgMcs363646 = -607250291;    int ZHeQSYFprikvufijDvPLGmaOvmgMcs22434772 = -915379694;    int ZHeQSYFprikvufijDvPLGmaOvmgMcs23510940 = -987991955;    int ZHeQSYFprikvufijDvPLGmaOvmgMcs11353769 = -76980098;    int ZHeQSYFprikvufijDvPLGmaOvmgMcs40819987 = -783790351;    int ZHeQSYFprikvufijDvPLGmaOvmgMcs43492970 = -764323340;    int ZHeQSYFprikvufijDvPLGmaOvmgMcs20770310 = -868100539;    int ZHeQSYFprikvufijDvPLGmaOvmgMcs32766048 = -267070966;    int ZHeQSYFprikvufijDvPLGmaOvmgMcs71044766 = -40068883;    int ZHeQSYFprikvufijDvPLGmaOvmgMcs51720773 = -173448525;    int ZHeQSYFprikvufijDvPLGmaOvmgMcs15474717 = -781852332;    int ZHeQSYFprikvufijDvPLGmaOvmgMcs9142177 = -196734278;    int ZHeQSYFprikvufijDvPLGmaOvmgMcs16031537 = -259006355;    int ZHeQSYFprikvufijDvPLGmaOvmgMcs3682994 = -202057643;    int ZHeQSYFprikvufijDvPLGmaOvmgMcs85708774 = -384000103;    int ZHeQSYFprikvufijDvPLGmaOvmgMcs33312317 = -968541383;    int ZHeQSYFprikvufijDvPLGmaOvmgMcs24161547 = -415615612;    int ZHeQSYFprikvufijDvPLGmaOvmgMcs43303429 = -549923867;    int ZHeQSYFprikvufijDvPLGmaOvmgMcs69675327 = 31321610;    int ZHeQSYFprikvufijDvPLGmaOvmgMcs93029958 = 99506601;    int ZHeQSYFprikvufijDvPLGmaOvmgMcs71782367 = 79716935;    int ZHeQSYFprikvufijDvPLGmaOvmgMcs6933540 = -95003423;    int ZHeQSYFprikvufijDvPLGmaOvmgMcs30793675 = -694012111;    int ZHeQSYFprikvufijDvPLGmaOvmgMcs52530706 = -133168306;    int ZHeQSYFprikvufijDvPLGmaOvmgMcs90379321 = -144817601;    int ZHeQSYFprikvufijDvPLGmaOvmgMcs91627769 = -953635156;    int ZHeQSYFprikvufijDvPLGmaOvmgMcs18126311 = -375357559;    int ZHeQSYFprikvufijDvPLGmaOvmgMcs43730561 = -136266482;    int ZHeQSYFprikvufijDvPLGmaOvmgMcs47389496 = -20520813;    int ZHeQSYFprikvufijDvPLGmaOvmgMcs64349898 = -259218561;    int ZHeQSYFprikvufijDvPLGmaOvmgMcs36354 = -31579966;    int ZHeQSYFprikvufijDvPLGmaOvmgMcs75312329 = -483458390;    int ZHeQSYFprikvufijDvPLGmaOvmgMcs2901608 = -360629146;    int ZHeQSYFprikvufijDvPLGmaOvmgMcs25288750 = 43526363;    int ZHeQSYFprikvufijDvPLGmaOvmgMcs9155110 = -859973146;    int ZHeQSYFprikvufijDvPLGmaOvmgMcs49803008 = -59505811;    int ZHeQSYFprikvufijDvPLGmaOvmgMcs50038783 = -764433411;    int ZHeQSYFprikvufijDvPLGmaOvmgMcs94175158 = -559967060;    int ZHeQSYFprikvufijDvPLGmaOvmgMcs6987209 = -727143716;    int ZHeQSYFprikvufijDvPLGmaOvmgMcs2999213 = 6645285;    int ZHeQSYFprikvufijDvPLGmaOvmgMcs1362970 = -562877455;    int ZHeQSYFprikvufijDvPLGmaOvmgMcs84560367 = -170859716;    int ZHeQSYFprikvufijDvPLGmaOvmgMcs33335040 = -596281429;    int ZHeQSYFprikvufijDvPLGmaOvmgMcs12176736 = -34368706;    int ZHeQSYFprikvufijDvPLGmaOvmgMcs48913530 = 54827368;    int ZHeQSYFprikvufijDvPLGmaOvmgMcs41945089 = -63484695;    int ZHeQSYFprikvufijDvPLGmaOvmgMcs98435542 = -871349615;    int ZHeQSYFprikvufijDvPLGmaOvmgMcs95619525 = -992734532;    int ZHeQSYFprikvufijDvPLGmaOvmgMcs19816667 = -980254994;    int ZHeQSYFprikvufijDvPLGmaOvmgMcs2928957 = -177500538;    int ZHeQSYFprikvufijDvPLGmaOvmgMcs51411539 = -468622116;     ZHeQSYFprikvufijDvPLGmaOvmgMcs70026311 = ZHeQSYFprikvufijDvPLGmaOvmgMcs16396818;     ZHeQSYFprikvufijDvPLGmaOvmgMcs16396818 = ZHeQSYFprikvufijDvPLGmaOvmgMcs10967750;     ZHeQSYFprikvufijDvPLGmaOvmgMcs10967750 = ZHeQSYFprikvufijDvPLGmaOvmgMcs43367636;     ZHeQSYFprikvufijDvPLGmaOvmgMcs43367636 = ZHeQSYFprikvufijDvPLGmaOvmgMcs93768400;     ZHeQSYFprikvufijDvPLGmaOvmgMcs93768400 = ZHeQSYFprikvufijDvPLGmaOvmgMcs84007195;     ZHeQSYFprikvufijDvPLGmaOvmgMcs84007195 = ZHeQSYFprikvufijDvPLGmaOvmgMcs75465698;     ZHeQSYFprikvufijDvPLGmaOvmgMcs75465698 = ZHeQSYFprikvufijDvPLGmaOvmgMcs13201749;     ZHeQSYFprikvufijDvPLGmaOvmgMcs13201749 = ZHeQSYFprikvufijDvPLGmaOvmgMcs57946553;     ZHeQSYFprikvufijDvPLGmaOvmgMcs57946553 = ZHeQSYFprikvufijDvPLGmaOvmgMcs6862040;     ZHeQSYFprikvufijDvPLGmaOvmgMcs6862040 = ZHeQSYFprikvufijDvPLGmaOvmgMcs29690475;     ZHeQSYFprikvufijDvPLGmaOvmgMcs29690475 = ZHeQSYFprikvufijDvPLGmaOvmgMcs91110625;     ZHeQSYFprikvufijDvPLGmaOvmgMcs91110625 = ZHeQSYFprikvufijDvPLGmaOvmgMcs16167509;     ZHeQSYFprikvufijDvPLGmaOvmgMcs16167509 = ZHeQSYFprikvufijDvPLGmaOvmgMcs15163238;     ZHeQSYFprikvufijDvPLGmaOvmgMcs15163238 = ZHeQSYFprikvufijDvPLGmaOvmgMcs89108782;     ZHeQSYFprikvufijDvPLGmaOvmgMcs89108782 = ZHeQSYFprikvufijDvPLGmaOvmgMcs11313347;     ZHeQSYFprikvufijDvPLGmaOvmgMcs11313347 = ZHeQSYFprikvufijDvPLGmaOvmgMcs94691183;     ZHeQSYFprikvufijDvPLGmaOvmgMcs94691183 = ZHeQSYFprikvufijDvPLGmaOvmgMcs16918952;     ZHeQSYFprikvufijDvPLGmaOvmgMcs16918952 = ZHeQSYFprikvufijDvPLGmaOvmgMcs41466587;     ZHeQSYFprikvufijDvPLGmaOvmgMcs41466587 = ZHeQSYFprikvufijDvPLGmaOvmgMcs6094849;     ZHeQSYFprikvufijDvPLGmaOvmgMcs6094849 = ZHeQSYFprikvufijDvPLGmaOvmgMcs17428830;     ZHeQSYFprikvufijDvPLGmaOvmgMcs17428830 = ZHeQSYFprikvufijDvPLGmaOvmgMcs49256001;     ZHeQSYFprikvufijDvPLGmaOvmgMcs49256001 = ZHeQSYFprikvufijDvPLGmaOvmgMcs83630770;     ZHeQSYFprikvufijDvPLGmaOvmgMcs83630770 = ZHeQSYFprikvufijDvPLGmaOvmgMcs39922984;     ZHeQSYFprikvufijDvPLGmaOvmgMcs39922984 = ZHeQSYFprikvufijDvPLGmaOvmgMcs91646863;     ZHeQSYFprikvufijDvPLGmaOvmgMcs91646863 = ZHeQSYFprikvufijDvPLGmaOvmgMcs78293684;     ZHeQSYFprikvufijDvPLGmaOvmgMcs78293684 = ZHeQSYFprikvufijDvPLGmaOvmgMcs74865019;     ZHeQSYFprikvufijDvPLGmaOvmgMcs74865019 = ZHeQSYFprikvufijDvPLGmaOvmgMcs59434161;     ZHeQSYFprikvufijDvPLGmaOvmgMcs59434161 = ZHeQSYFprikvufijDvPLGmaOvmgMcs9518755;     ZHeQSYFprikvufijDvPLGmaOvmgMcs9518755 = ZHeQSYFprikvufijDvPLGmaOvmgMcs72237779;     ZHeQSYFprikvufijDvPLGmaOvmgMcs72237779 = ZHeQSYFprikvufijDvPLGmaOvmgMcs73549723;     ZHeQSYFprikvufijDvPLGmaOvmgMcs73549723 = ZHeQSYFprikvufijDvPLGmaOvmgMcs5528928;     ZHeQSYFprikvufijDvPLGmaOvmgMcs5528928 = ZHeQSYFprikvufijDvPLGmaOvmgMcs47807196;     ZHeQSYFprikvufijDvPLGmaOvmgMcs47807196 = ZHeQSYFprikvufijDvPLGmaOvmgMcs46492182;     ZHeQSYFprikvufijDvPLGmaOvmgMcs46492182 = ZHeQSYFprikvufijDvPLGmaOvmgMcs22133280;     ZHeQSYFprikvufijDvPLGmaOvmgMcs22133280 = ZHeQSYFprikvufijDvPLGmaOvmgMcs17326415;     ZHeQSYFprikvufijDvPLGmaOvmgMcs17326415 = ZHeQSYFprikvufijDvPLGmaOvmgMcs4379807;     ZHeQSYFprikvufijDvPLGmaOvmgMcs4379807 = ZHeQSYFprikvufijDvPLGmaOvmgMcs63897508;     ZHeQSYFprikvufijDvPLGmaOvmgMcs63897508 = ZHeQSYFprikvufijDvPLGmaOvmgMcs64388246;     ZHeQSYFprikvufijDvPLGmaOvmgMcs64388246 = ZHeQSYFprikvufijDvPLGmaOvmgMcs51087266;     ZHeQSYFprikvufijDvPLGmaOvmgMcs51087266 = ZHeQSYFprikvufijDvPLGmaOvmgMcs14467080;     ZHeQSYFprikvufijDvPLGmaOvmgMcs14467080 = ZHeQSYFprikvufijDvPLGmaOvmgMcs99302519;     ZHeQSYFprikvufijDvPLGmaOvmgMcs99302519 = ZHeQSYFprikvufijDvPLGmaOvmgMcs5525441;     ZHeQSYFprikvufijDvPLGmaOvmgMcs5525441 = ZHeQSYFprikvufijDvPLGmaOvmgMcs36241274;     ZHeQSYFprikvufijDvPLGmaOvmgMcs36241274 = ZHeQSYFprikvufijDvPLGmaOvmgMcs75573085;     ZHeQSYFprikvufijDvPLGmaOvmgMcs75573085 = ZHeQSYFprikvufijDvPLGmaOvmgMcs91610509;     ZHeQSYFprikvufijDvPLGmaOvmgMcs91610509 = ZHeQSYFprikvufijDvPLGmaOvmgMcs2981355;     ZHeQSYFprikvufijDvPLGmaOvmgMcs2981355 = ZHeQSYFprikvufijDvPLGmaOvmgMcs71963411;     ZHeQSYFprikvufijDvPLGmaOvmgMcs71963411 = ZHeQSYFprikvufijDvPLGmaOvmgMcs34145411;     ZHeQSYFprikvufijDvPLGmaOvmgMcs34145411 = ZHeQSYFprikvufijDvPLGmaOvmgMcs363646;     ZHeQSYFprikvufijDvPLGmaOvmgMcs363646 = ZHeQSYFprikvufijDvPLGmaOvmgMcs22434772;     ZHeQSYFprikvufijDvPLGmaOvmgMcs22434772 = ZHeQSYFprikvufijDvPLGmaOvmgMcs23510940;     ZHeQSYFprikvufijDvPLGmaOvmgMcs23510940 = ZHeQSYFprikvufijDvPLGmaOvmgMcs11353769;     ZHeQSYFprikvufijDvPLGmaOvmgMcs11353769 = ZHeQSYFprikvufijDvPLGmaOvmgMcs40819987;     ZHeQSYFprikvufijDvPLGmaOvmgMcs40819987 = ZHeQSYFprikvufijDvPLGmaOvmgMcs43492970;     ZHeQSYFprikvufijDvPLGmaOvmgMcs43492970 = ZHeQSYFprikvufijDvPLGmaOvmgMcs20770310;     ZHeQSYFprikvufijDvPLGmaOvmgMcs20770310 = ZHeQSYFprikvufijDvPLGmaOvmgMcs32766048;     ZHeQSYFprikvufijDvPLGmaOvmgMcs32766048 = ZHeQSYFprikvufijDvPLGmaOvmgMcs71044766;     ZHeQSYFprikvufijDvPLGmaOvmgMcs71044766 = ZHeQSYFprikvufijDvPLGmaOvmgMcs51720773;     ZHeQSYFprikvufijDvPLGmaOvmgMcs51720773 = ZHeQSYFprikvufijDvPLGmaOvmgMcs15474717;     ZHeQSYFprikvufijDvPLGmaOvmgMcs15474717 = ZHeQSYFprikvufijDvPLGmaOvmgMcs9142177;     ZHeQSYFprikvufijDvPLGmaOvmgMcs9142177 = ZHeQSYFprikvufijDvPLGmaOvmgMcs16031537;     ZHeQSYFprikvufijDvPLGmaOvmgMcs16031537 = ZHeQSYFprikvufijDvPLGmaOvmgMcs3682994;     ZHeQSYFprikvufijDvPLGmaOvmgMcs3682994 = ZHeQSYFprikvufijDvPLGmaOvmgMcs85708774;     ZHeQSYFprikvufijDvPLGmaOvmgMcs85708774 = ZHeQSYFprikvufijDvPLGmaOvmgMcs33312317;     ZHeQSYFprikvufijDvPLGmaOvmgMcs33312317 = ZHeQSYFprikvufijDvPLGmaOvmgMcs24161547;     ZHeQSYFprikvufijDvPLGmaOvmgMcs24161547 = ZHeQSYFprikvufijDvPLGmaOvmgMcs43303429;     ZHeQSYFprikvufijDvPLGmaOvmgMcs43303429 = ZHeQSYFprikvufijDvPLGmaOvmgMcs69675327;     ZHeQSYFprikvufijDvPLGmaOvmgMcs69675327 = ZHeQSYFprikvufijDvPLGmaOvmgMcs93029958;     ZHeQSYFprikvufijDvPLGmaOvmgMcs93029958 = ZHeQSYFprikvufijDvPLGmaOvmgMcs71782367;     ZHeQSYFprikvufijDvPLGmaOvmgMcs71782367 = ZHeQSYFprikvufijDvPLGmaOvmgMcs6933540;     ZHeQSYFprikvufijDvPLGmaOvmgMcs6933540 = ZHeQSYFprikvufijDvPLGmaOvmgMcs30793675;     ZHeQSYFprikvufijDvPLGmaOvmgMcs30793675 = ZHeQSYFprikvufijDvPLGmaOvmgMcs52530706;     ZHeQSYFprikvufijDvPLGmaOvmgMcs52530706 = ZHeQSYFprikvufijDvPLGmaOvmgMcs90379321;     ZHeQSYFprikvufijDvPLGmaOvmgMcs90379321 = ZHeQSYFprikvufijDvPLGmaOvmgMcs91627769;     ZHeQSYFprikvufijDvPLGmaOvmgMcs91627769 = ZHeQSYFprikvufijDvPLGmaOvmgMcs18126311;     ZHeQSYFprikvufijDvPLGmaOvmgMcs18126311 = ZHeQSYFprikvufijDvPLGmaOvmgMcs43730561;     ZHeQSYFprikvufijDvPLGmaOvmgMcs43730561 = ZHeQSYFprikvufijDvPLGmaOvmgMcs47389496;     ZHeQSYFprikvufijDvPLGmaOvmgMcs47389496 = ZHeQSYFprikvufijDvPLGmaOvmgMcs64349898;     ZHeQSYFprikvufijDvPLGmaOvmgMcs64349898 = ZHeQSYFprikvufijDvPLGmaOvmgMcs36354;     ZHeQSYFprikvufijDvPLGmaOvmgMcs36354 = ZHeQSYFprikvufijDvPLGmaOvmgMcs75312329;     ZHeQSYFprikvufijDvPLGmaOvmgMcs75312329 = ZHeQSYFprikvufijDvPLGmaOvmgMcs2901608;     ZHeQSYFprikvufijDvPLGmaOvmgMcs2901608 = ZHeQSYFprikvufijDvPLGmaOvmgMcs25288750;     ZHeQSYFprikvufijDvPLGmaOvmgMcs25288750 = ZHeQSYFprikvufijDvPLGmaOvmgMcs9155110;     ZHeQSYFprikvufijDvPLGmaOvmgMcs9155110 = ZHeQSYFprikvufijDvPLGmaOvmgMcs49803008;     ZHeQSYFprikvufijDvPLGmaOvmgMcs49803008 = ZHeQSYFprikvufijDvPLGmaOvmgMcs50038783;     ZHeQSYFprikvufijDvPLGmaOvmgMcs50038783 = ZHeQSYFprikvufijDvPLGmaOvmgMcs94175158;     ZHeQSYFprikvufijDvPLGmaOvmgMcs94175158 = ZHeQSYFprikvufijDvPLGmaOvmgMcs6987209;     ZHeQSYFprikvufijDvPLGmaOvmgMcs6987209 = ZHeQSYFprikvufijDvPLGmaOvmgMcs2999213;     ZHeQSYFprikvufijDvPLGmaOvmgMcs2999213 = ZHeQSYFprikvufijDvPLGmaOvmgMcs1362970;     ZHeQSYFprikvufijDvPLGmaOvmgMcs1362970 = ZHeQSYFprikvufijDvPLGmaOvmgMcs84560367;     ZHeQSYFprikvufijDvPLGmaOvmgMcs84560367 = ZHeQSYFprikvufijDvPLGmaOvmgMcs33335040;     ZHeQSYFprikvufijDvPLGmaOvmgMcs33335040 = ZHeQSYFprikvufijDvPLGmaOvmgMcs12176736;     ZHeQSYFprikvufijDvPLGmaOvmgMcs12176736 = ZHeQSYFprikvufijDvPLGmaOvmgMcs48913530;     ZHeQSYFprikvufijDvPLGmaOvmgMcs48913530 = ZHeQSYFprikvufijDvPLGmaOvmgMcs41945089;     ZHeQSYFprikvufijDvPLGmaOvmgMcs41945089 = ZHeQSYFprikvufijDvPLGmaOvmgMcs98435542;     ZHeQSYFprikvufijDvPLGmaOvmgMcs98435542 = ZHeQSYFprikvufijDvPLGmaOvmgMcs95619525;     ZHeQSYFprikvufijDvPLGmaOvmgMcs95619525 = ZHeQSYFprikvufijDvPLGmaOvmgMcs19816667;     ZHeQSYFprikvufijDvPLGmaOvmgMcs19816667 = ZHeQSYFprikvufijDvPLGmaOvmgMcs2928957;     ZHeQSYFprikvufijDvPLGmaOvmgMcs2928957 = ZHeQSYFprikvufijDvPLGmaOvmgMcs51411539;     ZHeQSYFprikvufijDvPLGmaOvmgMcs51411539 = ZHeQSYFprikvufijDvPLGmaOvmgMcs70026311;}
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void SxtPXJTrYgFDPTfCAnWATxkpcpVeaN76090601() {     int RBMIdvCibUVvukeVBUJRAteqPUNDni93585600 = -909961245;    int RBMIdvCibUVvukeVBUJRAteqPUNDni58895526 = 17516475;    int RBMIdvCibUVvukeVBUJRAteqPUNDni30167077 = -262026773;    int RBMIdvCibUVvukeVBUJRAteqPUNDni53548219 = -104924632;    int RBMIdvCibUVvukeVBUJRAteqPUNDni28416639 = 9918418;    int RBMIdvCibUVvukeVBUJRAteqPUNDni24207508 = -237660124;    int RBMIdvCibUVvukeVBUJRAteqPUNDni36126292 = -454613024;    int RBMIdvCibUVvukeVBUJRAteqPUNDni51552343 = -425197153;    int RBMIdvCibUVvukeVBUJRAteqPUNDni35083229 = -32118218;    int RBMIdvCibUVvukeVBUJRAteqPUNDni37782511 = -398629409;    int RBMIdvCibUVvukeVBUJRAteqPUNDni90819290 = -230174924;    int RBMIdvCibUVvukeVBUJRAteqPUNDni14075967 = -715151449;    int RBMIdvCibUVvukeVBUJRAteqPUNDni78374017 = -243155875;    int RBMIdvCibUVvukeVBUJRAteqPUNDni52436368 = -271940473;    int RBMIdvCibUVvukeVBUJRAteqPUNDni77088808 = 27894839;    int RBMIdvCibUVvukeVBUJRAteqPUNDni77370208 = -727205318;    int RBMIdvCibUVvukeVBUJRAteqPUNDni30147578 = -250845107;    int RBMIdvCibUVvukeVBUJRAteqPUNDni84377043 = -520222775;    int RBMIdvCibUVvukeVBUJRAteqPUNDni25570593 = -501590847;    int RBMIdvCibUVvukeVBUJRAteqPUNDni38915625 = -682498217;    int RBMIdvCibUVvukeVBUJRAteqPUNDni413112 = -7420010;    int RBMIdvCibUVvukeVBUJRAteqPUNDni71503367 = -992255779;    int RBMIdvCibUVvukeVBUJRAteqPUNDni85232089 = -691088075;    int RBMIdvCibUVvukeVBUJRAteqPUNDni8938329 = -853123814;    int RBMIdvCibUVvukeVBUJRAteqPUNDni54882716 = -615435942;    int RBMIdvCibUVvukeVBUJRAteqPUNDni96431432 = -336160330;    int RBMIdvCibUVvukeVBUJRAteqPUNDni94174862 = -709226499;    int RBMIdvCibUVvukeVBUJRAteqPUNDni84292330 = -505748237;    int RBMIdvCibUVvukeVBUJRAteqPUNDni78996128 = -297550678;    int RBMIdvCibUVvukeVBUJRAteqPUNDni94017472 = -894142651;    int RBMIdvCibUVvukeVBUJRAteqPUNDni53580627 = -844262270;    int RBMIdvCibUVvukeVBUJRAteqPUNDni41269019 = -188654739;    int RBMIdvCibUVvukeVBUJRAteqPUNDni99856864 = 39566219;    int RBMIdvCibUVvukeVBUJRAteqPUNDni12984303 = -499924414;    int RBMIdvCibUVvukeVBUJRAteqPUNDni30583111 = -732777758;    int RBMIdvCibUVvukeVBUJRAteqPUNDni57895938 = -493574052;    int RBMIdvCibUVvukeVBUJRAteqPUNDni19124948 = -580408807;    int RBMIdvCibUVvukeVBUJRAteqPUNDni1189563 = -974056683;    int RBMIdvCibUVvukeVBUJRAteqPUNDni35715135 = -591577285;    int RBMIdvCibUVvukeVBUJRAteqPUNDni95872104 = -709025719;    int RBMIdvCibUVvukeVBUJRAteqPUNDni40563096 = -936668041;    int RBMIdvCibUVvukeVBUJRAteqPUNDni79405675 = -922329853;    int RBMIdvCibUVvukeVBUJRAteqPUNDni19405231 = -719215879;    int RBMIdvCibUVvukeVBUJRAteqPUNDni37463977 = -90995657;    int RBMIdvCibUVvukeVBUJRAteqPUNDni56513591 = -576315429;    int RBMIdvCibUVvukeVBUJRAteqPUNDni22404444 = -358291034;    int RBMIdvCibUVvukeVBUJRAteqPUNDni57891212 = -679985934;    int RBMIdvCibUVvukeVBUJRAteqPUNDni88941732 = -600294864;    int RBMIdvCibUVvukeVBUJRAteqPUNDni563359 = -886865615;    int RBMIdvCibUVvukeVBUJRAteqPUNDni50056026 = -548560635;    int RBMIdvCibUVvukeVBUJRAteqPUNDni98261640 = -773134424;    int RBMIdvCibUVvukeVBUJRAteqPUNDni98242542 = -304433159;    int RBMIdvCibUVvukeVBUJRAteqPUNDni81953292 = -636674923;    int RBMIdvCibUVvukeVBUJRAteqPUNDni86341782 = -731618760;    int RBMIdvCibUVvukeVBUJRAteqPUNDni75677490 = -815983470;    int RBMIdvCibUVvukeVBUJRAteqPUNDni22082234 = -917705467;    int RBMIdvCibUVvukeVBUJRAteqPUNDni73663436 = -291395451;    int RBMIdvCibUVvukeVBUJRAteqPUNDni21228749 = -408902960;    int RBMIdvCibUVvukeVBUJRAteqPUNDni98665502 = -489488690;    int RBMIdvCibUVvukeVBUJRAteqPUNDni31985206 = -653921253;    int RBMIdvCibUVvukeVBUJRAteqPUNDni30032646 = -528433626;    int RBMIdvCibUVvukeVBUJRAteqPUNDni51833962 = -948864787;    int RBMIdvCibUVvukeVBUJRAteqPUNDni72556214 = -27646475;    int RBMIdvCibUVvukeVBUJRAteqPUNDni41065756 = -137975567;    int RBMIdvCibUVvukeVBUJRAteqPUNDni84201883 = -554367140;    int RBMIdvCibUVvukeVBUJRAteqPUNDni49550271 = 58479814;    int RBMIdvCibUVvukeVBUJRAteqPUNDni14219103 = -654717668;    int RBMIdvCibUVvukeVBUJRAteqPUNDni65389714 = -743231462;    int RBMIdvCibUVvukeVBUJRAteqPUNDni21853258 = -539162716;    int RBMIdvCibUVvukeVBUJRAteqPUNDni19192871 = -478531109;    int RBMIdvCibUVvukeVBUJRAteqPUNDni58245260 = -46796511;    int RBMIdvCibUVvukeVBUJRAteqPUNDni28958015 = -276788424;    int RBMIdvCibUVvukeVBUJRAteqPUNDni48661908 = -928645491;    int RBMIdvCibUVvukeVBUJRAteqPUNDni29698488 = -792565129;    int RBMIdvCibUVvukeVBUJRAteqPUNDni98352529 = -745830177;    int RBMIdvCibUVvukeVBUJRAteqPUNDni21007436 = -85090158;    int RBMIdvCibUVvukeVBUJRAteqPUNDni52098136 = -173039900;    int RBMIdvCibUVvukeVBUJRAteqPUNDni47768113 = -500092419;    int RBMIdvCibUVvukeVBUJRAteqPUNDni52424737 = -176808385;    int RBMIdvCibUVvukeVBUJRAteqPUNDni32478273 = -157144909;    int RBMIdvCibUVvukeVBUJRAteqPUNDni38540220 = -656174397;    int RBMIdvCibUVvukeVBUJRAteqPUNDni5233130 = -8931635;    int RBMIdvCibUVvukeVBUJRAteqPUNDni83728971 = -618882623;    int RBMIdvCibUVvukeVBUJRAteqPUNDni28940103 = -748990044;    int RBMIdvCibUVvukeVBUJRAteqPUNDni95755832 = -21008228;    int RBMIdvCibUVvukeVBUJRAteqPUNDni55338085 = -439829112;    int RBMIdvCibUVvukeVBUJRAteqPUNDni59315727 = -551979816;    int RBMIdvCibUVvukeVBUJRAteqPUNDni13515083 = -228815022;    int RBMIdvCibUVvukeVBUJRAteqPUNDni37306812 = -683940944;    int RBMIdvCibUVvukeVBUJRAteqPUNDni8500877 = -815072291;    int RBMIdvCibUVvukeVBUJRAteqPUNDni84232501 = -102178602;    int RBMIdvCibUVvukeVBUJRAteqPUNDni97896199 = -71505848;    int RBMIdvCibUVvukeVBUJRAteqPUNDni2524061 = -384567994;    int RBMIdvCibUVvukeVBUJRAteqPUNDni3729930 = -937656033;    int RBMIdvCibUVvukeVBUJRAteqPUNDni65839459 = -80592093;    int RBMIdvCibUVvukeVBUJRAteqPUNDni88729133 = -987803255;    int RBMIdvCibUVvukeVBUJRAteqPUNDni6849461 = -794683379;    int RBMIdvCibUVvukeVBUJRAteqPUNDni78339475 = -481240313;    int RBMIdvCibUVvukeVBUJRAteqPUNDni53262093 = -536628517;    int RBMIdvCibUVvukeVBUJRAteqPUNDni6963321 = -909961245;     RBMIdvCibUVvukeVBUJRAteqPUNDni93585600 = RBMIdvCibUVvukeVBUJRAteqPUNDni58895526;     RBMIdvCibUVvukeVBUJRAteqPUNDni58895526 = RBMIdvCibUVvukeVBUJRAteqPUNDni30167077;     RBMIdvCibUVvukeVBUJRAteqPUNDni30167077 = RBMIdvCibUVvukeVBUJRAteqPUNDni53548219;     RBMIdvCibUVvukeVBUJRAteqPUNDni53548219 = RBMIdvCibUVvukeVBUJRAteqPUNDni28416639;     RBMIdvCibUVvukeVBUJRAteqPUNDni28416639 = RBMIdvCibUVvukeVBUJRAteqPUNDni24207508;     RBMIdvCibUVvukeVBUJRAteqPUNDni24207508 = RBMIdvCibUVvukeVBUJRAteqPUNDni36126292;     RBMIdvCibUVvukeVBUJRAteqPUNDni36126292 = RBMIdvCibUVvukeVBUJRAteqPUNDni51552343;     RBMIdvCibUVvukeVBUJRAteqPUNDni51552343 = RBMIdvCibUVvukeVBUJRAteqPUNDni35083229;     RBMIdvCibUVvukeVBUJRAteqPUNDni35083229 = RBMIdvCibUVvukeVBUJRAteqPUNDni37782511;     RBMIdvCibUVvukeVBUJRAteqPUNDni37782511 = RBMIdvCibUVvukeVBUJRAteqPUNDni90819290;     RBMIdvCibUVvukeVBUJRAteqPUNDni90819290 = RBMIdvCibUVvukeVBUJRAteqPUNDni14075967;     RBMIdvCibUVvukeVBUJRAteqPUNDni14075967 = RBMIdvCibUVvukeVBUJRAteqPUNDni78374017;     RBMIdvCibUVvukeVBUJRAteqPUNDni78374017 = RBMIdvCibUVvukeVBUJRAteqPUNDni52436368;     RBMIdvCibUVvukeVBUJRAteqPUNDni52436368 = RBMIdvCibUVvukeVBUJRAteqPUNDni77088808;     RBMIdvCibUVvukeVBUJRAteqPUNDni77088808 = RBMIdvCibUVvukeVBUJRAteqPUNDni77370208;     RBMIdvCibUVvukeVBUJRAteqPUNDni77370208 = RBMIdvCibUVvukeVBUJRAteqPUNDni30147578;     RBMIdvCibUVvukeVBUJRAteqPUNDni30147578 = RBMIdvCibUVvukeVBUJRAteqPUNDni84377043;     RBMIdvCibUVvukeVBUJRAteqPUNDni84377043 = RBMIdvCibUVvukeVBUJRAteqPUNDni25570593;     RBMIdvCibUVvukeVBUJRAteqPUNDni25570593 = RBMIdvCibUVvukeVBUJRAteqPUNDni38915625;     RBMIdvCibUVvukeVBUJRAteqPUNDni38915625 = RBMIdvCibUVvukeVBUJRAteqPUNDni413112;     RBMIdvCibUVvukeVBUJRAteqPUNDni413112 = RBMIdvCibUVvukeVBUJRAteqPUNDni71503367;     RBMIdvCibUVvukeVBUJRAteqPUNDni71503367 = RBMIdvCibUVvukeVBUJRAteqPUNDni85232089;     RBMIdvCibUVvukeVBUJRAteqPUNDni85232089 = RBMIdvCibUVvukeVBUJRAteqPUNDni8938329;     RBMIdvCibUVvukeVBUJRAteqPUNDni8938329 = RBMIdvCibUVvukeVBUJRAteqPUNDni54882716;     RBMIdvCibUVvukeVBUJRAteqPUNDni54882716 = RBMIdvCibUVvukeVBUJRAteqPUNDni96431432;     RBMIdvCibUVvukeVBUJRAteqPUNDni96431432 = RBMIdvCibUVvukeVBUJRAteqPUNDni94174862;     RBMIdvCibUVvukeVBUJRAteqPUNDni94174862 = RBMIdvCibUVvukeVBUJRAteqPUNDni84292330;     RBMIdvCibUVvukeVBUJRAteqPUNDni84292330 = RBMIdvCibUVvukeVBUJRAteqPUNDni78996128;     RBMIdvCibUVvukeVBUJRAteqPUNDni78996128 = RBMIdvCibUVvukeVBUJRAteqPUNDni94017472;     RBMIdvCibUVvukeVBUJRAteqPUNDni94017472 = RBMIdvCibUVvukeVBUJRAteqPUNDni53580627;     RBMIdvCibUVvukeVBUJRAteqPUNDni53580627 = RBMIdvCibUVvukeVBUJRAteqPUNDni41269019;     RBMIdvCibUVvukeVBUJRAteqPUNDni41269019 = RBMIdvCibUVvukeVBUJRAteqPUNDni99856864;     RBMIdvCibUVvukeVBUJRAteqPUNDni99856864 = RBMIdvCibUVvukeVBUJRAteqPUNDni12984303;     RBMIdvCibUVvukeVBUJRAteqPUNDni12984303 = RBMIdvCibUVvukeVBUJRAteqPUNDni30583111;     RBMIdvCibUVvukeVBUJRAteqPUNDni30583111 = RBMIdvCibUVvukeVBUJRAteqPUNDni57895938;     RBMIdvCibUVvukeVBUJRAteqPUNDni57895938 = RBMIdvCibUVvukeVBUJRAteqPUNDni19124948;     RBMIdvCibUVvukeVBUJRAteqPUNDni19124948 = RBMIdvCibUVvukeVBUJRAteqPUNDni1189563;     RBMIdvCibUVvukeVBUJRAteqPUNDni1189563 = RBMIdvCibUVvukeVBUJRAteqPUNDni35715135;     RBMIdvCibUVvukeVBUJRAteqPUNDni35715135 = RBMIdvCibUVvukeVBUJRAteqPUNDni95872104;     RBMIdvCibUVvukeVBUJRAteqPUNDni95872104 = RBMIdvCibUVvukeVBUJRAteqPUNDni40563096;     RBMIdvCibUVvukeVBUJRAteqPUNDni40563096 = RBMIdvCibUVvukeVBUJRAteqPUNDni79405675;     RBMIdvCibUVvukeVBUJRAteqPUNDni79405675 = RBMIdvCibUVvukeVBUJRAteqPUNDni19405231;     RBMIdvCibUVvukeVBUJRAteqPUNDni19405231 = RBMIdvCibUVvukeVBUJRAteqPUNDni37463977;     RBMIdvCibUVvukeVBUJRAteqPUNDni37463977 = RBMIdvCibUVvukeVBUJRAteqPUNDni56513591;     RBMIdvCibUVvukeVBUJRAteqPUNDni56513591 = RBMIdvCibUVvukeVBUJRAteqPUNDni22404444;     RBMIdvCibUVvukeVBUJRAteqPUNDni22404444 = RBMIdvCibUVvukeVBUJRAteqPUNDni57891212;     RBMIdvCibUVvukeVBUJRAteqPUNDni57891212 = RBMIdvCibUVvukeVBUJRAteqPUNDni88941732;     RBMIdvCibUVvukeVBUJRAteqPUNDni88941732 = RBMIdvCibUVvukeVBUJRAteqPUNDni563359;     RBMIdvCibUVvukeVBUJRAteqPUNDni563359 = RBMIdvCibUVvukeVBUJRAteqPUNDni50056026;     RBMIdvCibUVvukeVBUJRAteqPUNDni50056026 = RBMIdvCibUVvukeVBUJRAteqPUNDni98261640;     RBMIdvCibUVvukeVBUJRAteqPUNDni98261640 = RBMIdvCibUVvukeVBUJRAteqPUNDni98242542;     RBMIdvCibUVvukeVBUJRAteqPUNDni98242542 = RBMIdvCibUVvukeVBUJRAteqPUNDni81953292;     RBMIdvCibUVvukeVBUJRAteqPUNDni81953292 = RBMIdvCibUVvukeVBUJRAteqPUNDni86341782;     RBMIdvCibUVvukeVBUJRAteqPUNDni86341782 = RBMIdvCibUVvukeVBUJRAteqPUNDni75677490;     RBMIdvCibUVvukeVBUJRAteqPUNDni75677490 = RBMIdvCibUVvukeVBUJRAteqPUNDni22082234;     RBMIdvCibUVvukeVBUJRAteqPUNDni22082234 = RBMIdvCibUVvukeVBUJRAteqPUNDni73663436;     RBMIdvCibUVvukeVBUJRAteqPUNDni73663436 = RBMIdvCibUVvukeVBUJRAteqPUNDni21228749;     RBMIdvCibUVvukeVBUJRAteqPUNDni21228749 = RBMIdvCibUVvukeVBUJRAteqPUNDni98665502;     RBMIdvCibUVvukeVBUJRAteqPUNDni98665502 = RBMIdvCibUVvukeVBUJRAteqPUNDni31985206;     RBMIdvCibUVvukeVBUJRAteqPUNDni31985206 = RBMIdvCibUVvukeVBUJRAteqPUNDni30032646;     RBMIdvCibUVvukeVBUJRAteqPUNDni30032646 = RBMIdvCibUVvukeVBUJRAteqPUNDni51833962;     RBMIdvCibUVvukeVBUJRAteqPUNDni51833962 = RBMIdvCibUVvukeVBUJRAteqPUNDni72556214;     RBMIdvCibUVvukeVBUJRAteqPUNDni72556214 = RBMIdvCibUVvukeVBUJRAteqPUNDni41065756;     RBMIdvCibUVvukeVBUJRAteqPUNDni41065756 = RBMIdvCibUVvukeVBUJRAteqPUNDni84201883;     RBMIdvCibUVvukeVBUJRAteqPUNDni84201883 = RBMIdvCibUVvukeVBUJRAteqPUNDni49550271;     RBMIdvCibUVvukeVBUJRAteqPUNDni49550271 = RBMIdvCibUVvukeVBUJRAteqPUNDni14219103;     RBMIdvCibUVvukeVBUJRAteqPUNDni14219103 = RBMIdvCibUVvukeVBUJRAteqPUNDni65389714;     RBMIdvCibUVvukeVBUJRAteqPUNDni65389714 = RBMIdvCibUVvukeVBUJRAteqPUNDni21853258;     RBMIdvCibUVvukeVBUJRAteqPUNDni21853258 = RBMIdvCibUVvukeVBUJRAteqPUNDni19192871;     RBMIdvCibUVvukeVBUJRAteqPUNDni19192871 = RBMIdvCibUVvukeVBUJRAteqPUNDni58245260;     RBMIdvCibUVvukeVBUJRAteqPUNDni58245260 = RBMIdvCibUVvukeVBUJRAteqPUNDni28958015;     RBMIdvCibUVvukeVBUJRAteqPUNDni28958015 = RBMIdvCibUVvukeVBUJRAteqPUNDni48661908;     RBMIdvCibUVvukeVBUJRAteqPUNDni48661908 = RBMIdvCibUVvukeVBUJRAteqPUNDni29698488;     RBMIdvCibUVvukeVBUJRAteqPUNDni29698488 = RBMIdvCibUVvukeVBUJRAteqPUNDni98352529;     RBMIdvCibUVvukeVBUJRAteqPUNDni98352529 = RBMIdvCibUVvukeVBUJRAteqPUNDni21007436;     RBMIdvCibUVvukeVBUJRAteqPUNDni21007436 = RBMIdvCibUVvukeVBUJRAteqPUNDni52098136;     RBMIdvCibUVvukeVBUJRAteqPUNDni52098136 = RBMIdvCibUVvukeVBUJRAteqPUNDni47768113;     RBMIdvCibUVvukeVBUJRAteqPUNDni47768113 = RBMIdvCibUVvukeVBUJRAteqPUNDni52424737;     RBMIdvCibUVvukeVBUJRAteqPUNDni52424737 = RBMIdvCibUVvukeVBUJRAteqPUNDni32478273;     RBMIdvCibUVvukeVBUJRAteqPUNDni32478273 = RBMIdvCibUVvukeVBUJRAteqPUNDni38540220;     RBMIdvCibUVvukeVBUJRAteqPUNDni38540220 = RBMIdvCibUVvukeVBUJRAteqPUNDni5233130;     RBMIdvCibUVvukeVBUJRAteqPUNDni5233130 = RBMIdvCibUVvukeVBUJRAteqPUNDni83728971;     RBMIdvCibUVvukeVBUJRAteqPUNDni83728971 = RBMIdvCibUVvukeVBUJRAteqPUNDni28940103;     RBMIdvCibUVvukeVBUJRAteqPUNDni28940103 = RBMIdvCibUVvukeVBUJRAteqPUNDni95755832;     RBMIdvCibUVvukeVBUJRAteqPUNDni95755832 = RBMIdvCibUVvukeVBUJRAteqPUNDni55338085;     RBMIdvCibUVvukeVBUJRAteqPUNDni55338085 = RBMIdvCibUVvukeVBUJRAteqPUNDni59315727;     RBMIdvCibUVvukeVBUJRAteqPUNDni59315727 = RBMIdvCibUVvukeVBUJRAteqPUNDni13515083;     RBMIdvCibUVvukeVBUJRAteqPUNDni13515083 = RBMIdvCibUVvukeVBUJRAteqPUNDni37306812;     RBMIdvCibUVvukeVBUJRAteqPUNDni37306812 = RBMIdvCibUVvukeVBUJRAteqPUNDni8500877;     RBMIdvCibUVvukeVBUJRAteqPUNDni8500877 = RBMIdvCibUVvukeVBUJRAteqPUNDni84232501;     RBMIdvCibUVvukeVBUJRAteqPUNDni84232501 = RBMIdvCibUVvukeVBUJRAteqPUNDni97896199;     RBMIdvCibUVvukeVBUJRAteqPUNDni97896199 = RBMIdvCibUVvukeVBUJRAteqPUNDni2524061;     RBMIdvCibUVvukeVBUJRAteqPUNDni2524061 = RBMIdvCibUVvukeVBUJRAteqPUNDni3729930;     RBMIdvCibUVvukeVBUJRAteqPUNDni3729930 = RBMIdvCibUVvukeVBUJRAteqPUNDni65839459;     RBMIdvCibUVvukeVBUJRAteqPUNDni65839459 = RBMIdvCibUVvukeVBUJRAteqPUNDni88729133;     RBMIdvCibUVvukeVBUJRAteqPUNDni88729133 = RBMIdvCibUVvukeVBUJRAteqPUNDni6849461;     RBMIdvCibUVvukeVBUJRAteqPUNDni6849461 = RBMIdvCibUVvukeVBUJRAteqPUNDni78339475;     RBMIdvCibUVvukeVBUJRAteqPUNDni78339475 = RBMIdvCibUVvukeVBUJRAteqPUNDni53262093;     RBMIdvCibUVvukeVBUJRAteqPUNDni53262093 = RBMIdvCibUVvukeVBUJRAteqPUNDni6963321;     RBMIdvCibUVvukeVBUJRAteqPUNDni6963321 = RBMIdvCibUVvukeVBUJRAteqPUNDni93585600;}
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void BUxROQrCDVZOfXhGjrTKwGsRtBUkba67279351() {     float gzletiilKHsEQArmGaRnnsJGXucfXM31668125 = -259464863;    float gzletiilKHsEQArmGaRnnsJGXucfXM69196025 = -667918340;    float gzletiilKHsEQArmGaRnnsJGXucfXM40591261 = -883758149;    float gzletiilKHsEQArmGaRnnsJGXucfXM50475063 = -406178954;    float gzletiilKHsEQArmGaRnnsJGXucfXM31089889 = -640272616;    float gzletiilKHsEQArmGaRnnsJGXucfXM23899887 = -494464642;    float gzletiilKHsEQArmGaRnnsJGXucfXM49634653 = -307758177;    float gzletiilKHsEQArmGaRnnsJGXucfXM68110016 = -173366708;    float gzletiilKHsEQArmGaRnnsJGXucfXM88237750 = -254546114;    float gzletiilKHsEQArmGaRnnsJGXucfXM44558932 = -787823969;    float gzletiilKHsEQArmGaRnnsJGXucfXM69648037 = -944327549;    float gzletiilKHsEQArmGaRnnsJGXucfXM75775271 = -227889691;    float gzletiilKHsEQArmGaRnnsJGXucfXM39941392 = -442503607;    float gzletiilKHsEQArmGaRnnsJGXucfXM9640242 = -535217037;    float gzletiilKHsEQArmGaRnnsJGXucfXM81097617 = -980031904;    float gzletiilKHsEQArmGaRnnsJGXucfXM89938273 = -427433334;    float gzletiilKHsEQArmGaRnnsJGXucfXM63363125 = -26826387;    float gzletiilKHsEQArmGaRnnsJGXucfXM40326409 = -173771174;    float gzletiilKHsEQArmGaRnnsJGXucfXM90302010 = -131032932;    float gzletiilKHsEQArmGaRnnsJGXucfXM26102030 = -887511484;    float gzletiilKHsEQArmGaRnnsJGXucfXM89401446 = -541198126;    float gzletiilKHsEQArmGaRnnsJGXucfXM80643938 = -774831365;    float gzletiilKHsEQArmGaRnnsJGXucfXM17723621 = -815115298;    float gzletiilKHsEQArmGaRnnsJGXucfXM54198526 = -326291333;    float gzletiilKHsEQArmGaRnnsJGXucfXM13318015 = -964736786;    float gzletiilKHsEQArmGaRnnsJGXucfXM24449722 = 9106349;    float gzletiilKHsEQArmGaRnnsJGXucfXM71173480 = -544154402;    float gzletiilKHsEQArmGaRnnsJGXucfXM65781768 = 93827547;    float gzletiilKHsEQArmGaRnnsJGXucfXM86671017 = -505476483;    float gzletiilKHsEQArmGaRnnsJGXucfXM44510898 = -886328905;    float gzletiilKHsEQArmGaRnnsJGXucfXM6913167 = -898625083;    float gzletiilKHsEQArmGaRnnsJGXucfXM97176700 = -394900751;    float gzletiilKHsEQArmGaRnnsJGXucfXM24682686 = -92684112;    float gzletiilKHsEQArmGaRnnsJGXucfXM50505799 = -13351725;    float gzletiilKHsEQArmGaRnnsJGXucfXM49709093 = -696590564;    float gzletiilKHsEQArmGaRnnsJGXucfXM64682138 = -179007920;    float gzletiilKHsEQArmGaRnnsJGXucfXM42297033 = -8818053;    float gzletiilKHsEQArmGaRnnsJGXucfXM29539833 = -544612922;    float gzletiilKHsEQArmGaRnnsJGXucfXM23763854 = -190127225;    float gzletiilKHsEQArmGaRnnsJGXucfXM99035866 = -793507056;    float gzletiilKHsEQArmGaRnnsJGXucfXM99810589 = -290626918;    float gzletiilKHsEQArmGaRnnsJGXucfXM30812272 = -388250467;    float gzletiilKHsEQArmGaRnnsJGXucfXM34680884 = -308604075;    float gzletiilKHsEQArmGaRnnsJGXucfXM36443062 = -413269202;    float gzletiilKHsEQArmGaRnnsJGXucfXM18487283 = 29861516;    float gzletiilKHsEQArmGaRnnsJGXucfXM18413963 = -526803249;    float gzletiilKHsEQArmGaRnnsJGXucfXM19624836 = -942549103;    float gzletiilKHsEQArmGaRnnsJGXucfXM70498590 = -448572346;    float gzletiilKHsEQArmGaRnnsJGXucfXM70399211 = -831607540;    float gzletiilKHsEQArmGaRnnsJGXucfXM80343729 = 36837400;    float gzletiilKHsEQArmGaRnnsJGXucfXM79781052 = -697638866;    float gzletiilKHsEQArmGaRnnsJGXucfXM79493630 = -183122824;    float gzletiilKHsEQArmGaRnnsJGXucfXM66414625 = -868472718;    float gzletiilKHsEQArmGaRnnsJGXucfXM94059975 = -25718260;    float gzletiilKHsEQArmGaRnnsJGXucfXM77877905 = -952626469;    float gzletiilKHsEQArmGaRnnsJGXucfXM51024187 = -484633499;    float gzletiilKHsEQArmGaRnnsJGXucfXM51472405 = -852803042;    float gzletiilKHsEQArmGaRnnsJGXucfXM86392734 = -457466817;    float gzletiilKHsEQArmGaRnnsJGXucfXM37157049 = -441442169;    float gzletiilKHsEQArmGaRnnsJGXucfXM6640167 = -549378965;    float gzletiilKHsEQArmGaRnnsJGXucfXM52726406 = -950310241;    float gzletiilKHsEQArmGaRnnsJGXucfXM83852885 = -301585725;    float gzletiilKHsEQArmGaRnnsJGXucfXM81438999 = -667890225;    float gzletiilKHsEQArmGaRnnsJGXucfXM43726852 = -368217209;    float gzletiilKHsEQArmGaRnnsJGXucfXM37645766 = -889198886;    float gzletiilKHsEQArmGaRnnsJGXucfXM72471336 = -449426799;    float gzletiilKHsEQArmGaRnnsJGXucfXM51092586 = -35205579;    float gzletiilKHsEQArmGaRnnsJGXucfXM89435593 = -329151882;    float gzletiilKHsEQArmGaRnnsJGXucfXM59931149 = -838626473;    float gzletiilKHsEQArmGaRnnsJGXucfXM16415480 = -701023984;    float gzletiilKHsEQArmGaRnnsJGXucfXM47641240 = -318615281;    float gzletiilKHsEQArmGaRnnsJGXucfXM33823293 = -482213466;    float gzletiilKHsEQArmGaRnnsJGXucfXM16562555 = -983643950;    float gzletiilKHsEQArmGaRnnsJGXucfXM91266144 = -337525877;    float gzletiilKHsEQArmGaRnnsJGXucfXM26291440 = -496884566;    float gzletiilKHsEQArmGaRnnsJGXucfXM58589175 = -52947660;    float gzletiilKHsEQArmGaRnnsJGXucfXM45963055 = -366227290;    float gzletiilKHsEQArmGaRnnsJGXucfXM81280558 = -301846097;    float gzletiilKHsEQArmGaRnnsJGXucfXM35711244 = -256152849;    float gzletiilKHsEQArmGaRnnsJGXucfXM94904052 = -337933537;    float gzletiilKHsEQArmGaRnnsJGXucfXM4824886 = -48344549;    float gzletiilKHsEQArmGaRnnsJGXucfXM674891 = 4417943;    float gzletiilKHsEQArmGaRnnsJGXucfXM95382556 = -74564913;    float gzletiilKHsEQArmGaRnnsJGXucfXM6327288 = -442313883;    float gzletiilKHsEQArmGaRnnsJGXucfXM64729846 = -88690040;    float gzletiilKHsEQArmGaRnnsJGXucfXM27419537 = -615502260;    float gzletiilKHsEQArmGaRnnsJGXucfXM30762075 = -526428034;    float gzletiilKHsEQArmGaRnnsJGXucfXM30622710 = 33034147;    float gzletiilKHsEQArmGaRnnsJGXucfXM72627893 = -60725257;    float gzletiilKHsEQArmGaRnnsJGXucfXM98684906 = -111957066;    float gzletiilKHsEQArmGaRnnsJGXucfXM13209733 = -326204879;    float gzletiilKHsEQArmGaRnnsJGXucfXM55904299 = -551351237;    float gzletiilKHsEQArmGaRnnsJGXucfXM92382783 = -3170754;    float gzletiilKHsEQArmGaRnnsJGXucfXM17123687 = -640748260;    float gzletiilKHsEQArmGaRnnsJGXucfXM46309461 = -843196815;    float gzletiilKHsEQArmGaRnnsJGXucfXM15957705 = -989041194;    float gzletiilKHsEQArmGaRnnsJGXucfXM49373273 = -720360242;    float gzletiilKHsEQArmGaRnnsJGXucfXM90954032 = -940386867;    float gzletiilKHsEQArmGaRnnsJGXucfXM98797296 = -524070316;    float gzletiilKHsEQArmGaRnnsJGXucfXM46015946 = -259464863;     gzletiilKHsEQArmGaRnnsJGXucfXM31668125 = gzletiilKHsEQArmGaRnnsJGXucfXM69196025;     gzletiilKHsEQArmGaRnnsJGXucfXM69196025 = gzletiilKHsEQArmGaRnnsJGXucfXM40591261;     gzletiilKHsEQArmGaRnnsJGXucfXM40591261 = gzletiilKHsEQArmGaRnnsJGXucfXM50475063;     gzletiilKHsEQArmGaRnnsJGXucfXM50475063 = gzletiilKHsEQArmGaRnnsJGXucfXM31089889;     gzletiilKHsEQArmGaRnnsJGXucfXM31089889 = gzletiilKHsEQArmGaRnnsJGXucfXM23899887;     gzletiilKHsEQArmGaRnnsJGXucfXM23899887 = gzletiilKHsEQArmGaRnnsJGXucfXM49634653;     gzletiilKHsEQArmGaRnnsJGXucfXM49634653 = gzletiilKHsEQArmGaRnnsJGXucfXM68110016;     gzletiilKHsEQArmGaRnnsJGXucfXM68110016 = gzletiilKHsEQArmGaRnnsJGXucfXM88237750;     gzletiilKHsEQArmGaRnnsJGXucfXM88237750 = gzletiilKHsEQArmGaRnnsJGXucfXM44558932;     gzletiilKHsEQArmGaRnnsJGXucfXM44558932 = gzletiilKHsEQArmGaRnnsJGXucfXM69648037;     gzletiilKHsEQArmGaRnnsJGXucfXM69648037 = gzletiilKHsEQArmGaRnnsJGXucfXM75775271;     gzletiilKHsEQArmGaRnnsJGXucfXM75775271 = gzletiilKHsEQArmGaRnnsJGXucfXM39941392;     gzletiilKHsEQArmGaRnnsJGXucfXM39941392 = gzletiilKHsEQArmGaRnnsJGXucfXM9640242;     gzletiilKHsEQArmGaRnnsJGXucfXM9640242 = gzletiilKHsEQArmGaRnnsJGXucfXM81097617;     gzletiilKHsEQArmGaRnnsJGXucfXM81097617 = gzletiilKHsEQArmGaRnnsJGXucfXM89938273;     gzletiilKHsEQArmGaRnnsJGXucfXM89938273 = gzletiilKHsEQArmGaRnnsJGXucfXM63363125;     gzletiilKHsEQArmGaRnnsJGXucfXM63363125 = gzletiilKHsEQArmGaRnnsJGXucfXM40326409;     gzletiilKHsEQArmGaRnnsJGXucfXM40326409 = gzletiilKHsEQArmGaRnnsJGXucfXM90302010;     gzletiilKHsEQArmGaRnnsJGXucfXM90302010 = gzletiilKHsEQArmGaRnnsJGXucfXM26102030;     gzletiilKHsEQArmGaRnnsJGXucfXM26102030 = gzletiilKHsEQArmGaRnnsJGXucfXM89401446;     gzletiilKHsEQArmGaRnnsJGXucfXM89401446 = gzletiilKHsEQArmGaRnnsJGXucfXM80643938;     gzletiilKHsEQArmGaRnnsJGXucfXM80643938 = gzletiilKHsEQArmGaRnnsJGXucfXM17723621;     gzletiilKHsEQArmGaRnnsJGXucfXM17723621 = gzletiilKHsEQArmGaRnnsJGXucfXM54198526;     gzletiilKHsEQArmGaRnnsJGXucfXM54198526 = gzletiilKHsEQArmGaRnnsJGXucfXM13318015;     gzletiilKHsEQArmGaRnnsJGXucfXM13318015 = gzletiilKHsEQArmGaRnnsJGXucfXM24449722;     gzletiilKHsEQArmGaRnnsJGXucfXM24449722 = gzletiilKHsEQArmGaRnnsJGXucfXM71173480;     gzletiilKHsEQArmGaRnnsJGXucfXM71173480 = gzletiilKHsEQArmGaRnnsJGXucfXM65781768;     gzletiilKHsEQArmGaRnnsJGXucfXM65781768 = gzletiilKHsEQArmGaRnnsJGXucfXM86671017;     gzletiilKHsEQArmGaRnnsJGXucfXM86671017 = gzletiilKHsEQArmGaRnnsJGXucfXM44510898;     gzletiilKHsEQArmGaRnnsJGXucfXM44510898 = gzletiilKHsEQArmGaRnnsJGXucfXM6913167;     gzletiilKHsEQArmGaRnnsJGXucfXM6913167 = gzletiilKHsEQArmGaRnnsJGXucfXM97176700;     gzletiilKHsEQArmGaRnnsJGXucfXM97176700 = gzletiilKHsEQArmGaRnnsJGXucfXM24682686;     gzletiilKHsEQArmGaRnnsJGXucfXM24682686 = gzletiilKHsEQArmGaRnnsJGXucfXM50505799;     gzletiilKHsEQArmGaRnnsJGXucfXM50505799 = gzletiilKHsEQArmGaRnnsJGXucfXM49709093;     gzletiilKHsEQArmGaRnnsJGXucfXM49709093 = gzletiilKHsEQArmGaRnnsJGXucfXM64682138;     gzletiilKHsEQArmGaRnnsJGXucfXM64682138 = gzletiilKHsEQArmGaRnnsJGXucfXM42297033;     gzletiilKHsEQArmGaRnnsJGXucfXM42297033 = gzletiilKHsEQArmGaRnnsJGXucfXM29539833;     gzletiilKHsEQArmGaRnnsJGXucfXM29539833 = gzletiilKHsEQArmGaRnnsJGXucfXM23763854;     gzletiilKHsEQArmGaRnnsJGXucfXM23763854 = gzletiilKHsEQArmGaRnnsJGXucfXM99035866;     gzletiilKHsEQArmGaRnnsJGXucfXM99035866 = gzletiilKHsEQArmGaRnnsJGXucfXM99810589;     gzletiilKHsEQArmGaRnnsJGXucfXM99810589 = gzletiilKHsEQArmGaRnnsJGXucfXM30812272;     gzletiilKHsEQArmGaRnnsJGXucfXM30812272 = gzletiilKHsEQArmGaRnnsJGXucfXM34680884;     gzletiilKHsEQArmGaRnnsJGXucfXM34680884 = gzletiilKHsEQArmGaRnnsJGXucfXM36443062;     gzletiilKHsEQArmGaRnnsJGXucfXM36443062 = gzletiilKHsEQArmGaRnnsJGXucfXM18487283;     gzletiilKHsEQArmGaRnnsJGXucfXM18487283 = gzletiilKHsEQArmGaRnnsJGXucfXM18413963;     gzletiilKHsEQArmGaRnnsJGXucfXM18413963 = gzletiilKHsEQArmGaRnnsJGXucfXM19624836;     gzletiilKHsEQArmGaRnnsJGXucfXM19624836 = gzletiilKHsEQArmGaRnnsJGXucfXM70498590;     gzletiilKHsEQArmGaRnnsJGXucfXM70498590 = gzletiilKHsEQArmGaRnnsJGXucfXM70399211;     gzletiilKHsEQArmGaRnnsJGXucfXM70399211 = gzletiilKHsEQArmGaRnnsJGXucfXM80343729;     gzletiilKHsEQArmGaRnnsJGXucfXM80343729 = gzletiilKHsEQArmGaRnnsJGXucfXM79781052;     gzletiilKHsEQArmGaRnnsJGXucfXM79781052 = gzletiilKHsEQArmGaRnnsJGXucfXM79493630;     gzletiilKHsEQArmGaRnnsJGXucfXM79493630 = gzletiilKHsEQArmGaRnnsJGXucfXM66414625;     gzletiilKHsEQArmGaRnnsJGXucfXM66414625 = gzletiilKHsEQArmGaRnnsJGXucfXM94059975;     gzletiilKHsEQArmGaRnnsJGXucfXM94059975 = gzletiilKHsEQArmGaRnnsJGXucfXM77877905;     gzletiilKHsEQArmGaRnnsJGXucfXM77877905 = gzletiilKHsEQArmGaRnnsJGXucfXM51024187;     gzletiilKHsEQArmGaRnnsJGXucfXM51024187 = gzletiilKHsEQArmGaRnnsJGXucfXM51472405;     gzletiilKHsEQArmGaRnnsJGXucfXM51472405 = gzletiilKHsEQArmGaRnnsJGXucfXM86392734;     gzletiilKHsEQArmGaRnnsJGXucfXM86392734 = gzletiilKHsEQArmGaRnnsJGXucfXM37157049;     gzletiilKHsEQArmGaRnnsJGXucfXM37157049 = gzletiilKHsEQArmGaRnnsJGXucfXM6640167;     gzletiilKHsEQArmGaRnnsJGXucfXM6640167 = gzletiilKHsEQArmGaRnnsJGXucfXM52726406;     gzletiilKHsEQArmGaRnnsJGXucfXM52726406 = gzletiilKHsEQArmGaRnnsJGXucfXM83852885;     gzletiilKHsEQArmGaRnnsJGXucfXM83852885 = gzletiilKHsEQArmGaRnnsJGXucfXM81438999;     gzletiilKHsEQArmGaRnnsJGXucfXM81438999 = gzletiilKHsEQArmGaRnnsJGXucfXM43726852;     gzletiilKHsEQArmGaRnnsJGXucfXM43726852 = gzletiilKHsEQArmGaRnnsJGXucfXM37645766;     gzletiilKHsEQArmGaRnnsJGXucfXM37645766 = gzletiilKHsEQArmGaRnnsJGXucfXM72471336;     gzletiilKHsEQArmGaRnnsJGXucfXM72471336 = gzletiilKHsEQArmGaRnnsJGXucfXM51092586;     gzletiilKHsEQArmGaRnnsJGXucfXM51092586 = gzletiilKHsEQArmGaRnnsJGXucfXM89435593;     gzletiilKHsEQArmGaRnnsJGXucfXM89435593 = gzletiilKHsEQArmGaRnnsJGXucfXM59931149;     gzletiilKHsEQArmGaRnnsJGXucfXM59931149 = gzletiilKHsEQArmGaRnnsJGXucfXM16415480;     gzletiilKHsEQArmGaRnnsJGXucfXM16415480 = gzletiilKHsEQArmGaRnnsJGXucfXM47641240;     gzletiilKHsEQArmGaRnnsJGXucfXM47641240 = gzletiilKHsEQArmGaRnnsJGXucfXM33823293;     gzletiilKHsEQArmGaRnnsJGXucfXM33823293 = gzletiilKHsEQArmGaRnnsJGXucfXM16562555;     gzletiilKHsEQArmGaRnnsJGXucfXM16562555 = gzletiilKHsEQArmGaRnnsJGXucfXM91266144;     gzletiilKHsEQArmGaRnnsJGXucfXM91266144 = gzletiilKHsEQArmGaRnnsJGXucfXM26291440;     gzletiilKHsEQArmGaRnnsJGXucfXM26291440 = gzletiilKHsEQArmGaRnnsJGXucfXM58589175;     gzletiilKHsEQArmGaRnnsJGXucfXM58589175 = gzletiilKHsEQArmGaRnnsJGXucfXM45963055;     gzletiilKHsEQArmGaRnnsJGXucfXM45963055 = gzletiilKHsEQArmGaRnnsJGXucfXM81280558;     gzletiilKHsEQArmGaRnnsJGXucfXM81280558 = gzletiilKHsEQArmGaRnnsJGXucfXM35711244;     gzletiilKHsEQArmGaRnnsJGXucfXM35711244 = gzletiilKHsEQArmGaRnnsJGXucfXM94904052;     gzletiilKHsEQArmGaRnnsJGXucfXM94904052 = gzletiilKHsEQArmGaRnnsJGXucfXM4824886;     gzletiilKHsEQArmGaRnnsJGXucfXM4824886 = gzletiilKHsEQArmGaRnnsJGXucfXM674891;     gzletiilKHsEQArmGaRnnsJGXucfXM674891 = gzletiilKHsEQArmGaRnnsJGXucfXM95382556;     gzletiilKHsEQArmGaRnnsJGXucfXM95382556 = gzletiilKHsEQArmGaRnnsJGXucfXM6327288;     gzletiilKHsEQArmGaRnnsJGXucfXM6327288 = gzletiilKHsEQArmGaRnnsJGXucfXM64729846;     gzletiilKHsEQArmGaRnnsJGXucfXM64729846 = gzletiilKHsEQArmGaRnnsJGXucfXM27419537;     gzletiilKHsEQArmGaRnnsJGXucfXM27419537 = gzletiilKHsEQArmGaRnnsJGXucfXM30762075;     gzletiilKHsEQArmGaRnnsJGXucfXM30762075 = gzletiilKHsEQArmGaRnnsJGXucfXM30622710;     gzletiilKHsEQArmGaRnnsJGXucfXM30622710 = gzletiilKHsEQArmGaRnnsJGXucfXM72627893;     gzletiilKHsEQArmGaRnnsJGXucfXM72627893 = gzletiilKHsEQArmGaRnnsJGXucfXM98684906;     gzletiilKHsEQArmGaRnnsJGXucfXM98684906 = gzletiilKHsEQArmGaRnnsJGXucfXM13209733;     gzletiilKHsEQArmGaRnnsJGXucfXM13209733 = gzletiilKHsEQArmGaRnnsJGXucfXM55904299;     gzletiilKHsEQArmGaRnnsJGXucfXM55904299 = gzletiilKHsEQArmGaRnnsJGXucfXM92382783;     gzletiilKHsEQArmGaRnnsJGXucfXM92382783 = gzletiilKHsEQArmGaRnnsJGXucfXM17123687;     gzletiilKHsEQArmGaRnnsJGXucfXM17123687 = gzletiilKHsEQArmGaRnnsJGXucfXM46309461;     gzletiilKHsEQArmGaRnnsJGXucfXM46309461 = gzletiilKHsEQArmGaRnnsJGXucfXM15957705;     gzletiilKHsEQArmGaRnnsJGXucfXM15957705 = gzletiilKHsEQArmGaRnnsJGXucfXM49373273;     gzletiilKHsEQArmGaRnnsJGXucfXM49373273 = gzletiilKHsEQArmGaRnnsJGXucfXM90954032;     gzletiilKHsEQArmGaRnnsJGXucfXM90954032 = gzletiilKHsEQArmGaRnnsJGXucfXM98797296;     gzletiilKHsEQArmGaRnnsJGXucfXM98797296 = gzletiilKHsEQArmGaRnnsJGXucfXM46015946;     gzletiilKHsEQArmGaRnnsJGXucfXM46015946 = gzletiilKHsEQArmGaRnnsJGXucfXM31668125;}
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void ddhoGQGqPeRgdszXdHyBVWunlcvPzw51801529() {     long fVowaaPKCIhOQFQKCHxcqVxTmHiuCe42155563 = -968931056;    long fVowaaPKCIhOQFQKCHxcqVxTmHiuCe82770155 = -84759615;    long fVowaaPKCIhOQFQKCHxcqVxTmHiuCe51173389 = -488039681;    long fVowaaPKCIhOQFQKCHxcqVxTmHiuCe3438980 = -382353525;    long fVowaaPKCIhOQFQKCHxcqVxTmHiuCe7627112 = -43543517;    long fVowaaPKCIhOQFQKCHxcqVxTmHiuCe42760443 = -338437306;    long fVowaaPKCIhOQFQKCHxcqVxTmHiuCe91793579 = -520415352;    long fVowaaPKCIhOQFQKCHxcqVxTmHiuCe19962908 = -655645285;    long fVowaaPKCIhOQFQKCHxcqVxTmHiuCe61019951 = -323643357;    long fVowaaPKCIhOQFQKCHxcqVxTmHiuCe52340276 = -543540966;    long fVowaaPKCIhOQFQKCHxcqVxTmHiuCe30136978 = -116556377;    long fVowaaPKCIhOQFQKCHxcqVxTmHiuCe54075414 = -168807728;    long fVowaaPKCIhOQFQKCHxcqVxTmHiuCe30921435 = -926007853;    long fVowaaPKCIhOQFQKCHxcqVxTmHiuCe36387262 = -927031466;    long fVowaaPKCIhOQFQKCHxcqVxTmHiuCe47618404 = -617976018;    long fVowaaPKCIhOQFQKCHxcqVxTmHiuCe16709747 = -969140854;    long fVowaaPKCIhOQFQKCHxcqVxTmHiuCe92806935 = -26147420;    long fVowaaPKCIhOQFQKCHxcqVxTmHiuCe14000726 = -12115303;    long fVowaaPKCIhOQFQKCHxcqVxTmHiuCe15137317 = -946125909;    long fVowaaPKCIhOQFQKCHxcqVxTmHiuCe43601871 = -212091964;    long fVowaaPKCIhOQFQKCHxcqVxTmHiuCe60181972 = -212681023;    long fVowaaPKCIhOQFQKCHxcqVxTmHiuCe63284997 = -484695585;    long fVowaaPKCIhOQFQKCHxcqVxTmHiuCe52092884 = -678089048;    long fVowaaPKCIhOQFQKCHxcqVxTmHiuCe55920713 = -140920272;    long fVowaaPKCIhOQFQKCHxcqVxTmHiuCe4076163 = -37188658;    long fVowaaPKCIhOQFQKCHxcqVxTmHiuCe20192293 = -891954893;    long fVowaaPKCIhOQFQKCHxcqVxTmHiuCe77152170 = -409469588;    long fVowaaPKCIhOQFQKCHxcqVxTmHiuCe55822962 = -170391903;    long fVowaaPKCIhOQFQKCHxcqVxTmHiuCe16372737 = -105966041;    long fVowaaPKCIhOQFQKCHxcqVxTmHiuCe69914360 = -591374748;    long fVowaaPKCIhOQFQKCHxcqVxTmHiuCe12651645 = -519797653;    long fVowaaPKCIhOQFQKCHxcqVxTmHiuCe52634964 = -725708273;    long fVowaaPKCIhOQFQKCHxcqVxTmHiuCe45553014 = 76068655;    long fVowaaPKCIhOQFQKCHxcqVxTmHiuCe46638180 = 84968879;    long fVowaaPKCIhOQFQKCHxcqVxTmHiuCe68579835 = -11046500;    long fVowaaPKCIhOQFQKCHxcqVxTmHiuCe95383685 = -235481363;    long fVowaaPKCIhOQFQKCHxcqVxTmHiuCe1070071 = -821128461;    long fVowaaPKCIhOQFQKCHxcqVxTmHiuCe69242584 = 4177735;    long fVowaaPKCIhOQFQKCHxcqVxTmHiuCe13008314 = -704849302;    long fVowaaPKCIhOQFQKCHxcqVxTmHiuCe99734887 = -713275285;    long fVowaaPKCIhOQFQKCHxcqVxTmHiuCe21212369 = -730917741;    long fVowaaPKCIhOQFQKCHxcqVxTmHiuCe10058693 = -179151545;    long fVowaaPKCIhOQFQKCHxcqVxTmHiuCe64727488 = -997914656;    long fVowaaPKCIhOQFQKCHxcqVxTmHiuCe96978545 = -851254177;    long fVowaaPKCIhOQFQKCHxcqVxTmHiuCe67394596 = -896641662;    long fVowaaPKCIhOQFQKCHxcqVxTmHiuCe11346850 = -19421715;    long fVowaaPKCIhOQFQKCHxcqVxTmHiuCe67486927 = 89999187;    long fVowaaPKCIhOQFQKCHxcqVxTmHiuCe14349274 = -304291088;    long fVowaaPKCIhOQFQKCHxcqVxTmHiuCe8384398 = -715544;    long fVowaaPKCIhOQFQKCHxcqVxTmHiuCe2590266 = -655466387;    long fVowaaPKCIhOQFQKCHxcqVxTmHiuCe83706414 = 63806684;    long fVowaaPKCIhOQFQKCHxcqVxTmHiuCe44591017 = -816730963;    long fVowaaPKCIhOQFQKCHxcqVxTmHiuCe46707403 = -244033219;    long fVowaaPKCIhOQFQKCHxcqVxTmHiuCe83876469 = -57718978;    long fVowaaPKCIhOQFQKCHxcqVxTmHiuCe91619556 = -798500569;    long fVowaaPKCIhOQFQKCHxcqVxTmHiuCe78870565 = -384235471;    long fVowaaPKCIhOQFQKCHxcqVxTmHiuCe30677272 = -406670567;    long fVowaaPKCIhOQFQKCHxcqVxTmHiuCe95252676 = -247119410;    long fVowaaPKCIhOQFQKCHxcqVxTmHiuCe99362816 = -245164868;    long fVowaaPKCIhOQFQKCHxcqVxTmHiuCe87434818 = -151588625;    long fVowaaPKCIhOQFQKCHxcqVxTmHiuCe65608273 = -928967719;    long fVowaaPKCIhOQFQKCHxcqVxTmHiuCe35970618 = -250023449;    long fVowaaPKCIhOQFQKCHxcqVxTmHiuCe3590171 = -449679244;    long fVowaaPKCIhOQFQKCHxcqVxTmHiuCe91105591 = -732268609;    long fVowaaPKCIhOQFQKCHxcqVxTmHiuCe39688631 = 76256687;    long fVowaaPKCIhOQFQKCHxcqVxTmHiuCe77502013 = -390848104;    long fVowaaPKCIhOQFQKCHxcqVxTmHiuCe8522401 = -144876384;    long fVowaaPKCIhOQFQKCHxcqVxTmHiuCe84283255 = -910976732;    long fVowaaPKCIhOQFQKCHxcqVxTmHiuCe67807426 = -815984966;    long fVowaaPKCIhOQFQKCHxcqVxTmHiuCe52234718 = -282494656;    long fVowaaPKCIhOQFQKCHxcqVxTmHiuCe15639677 = -48012394;    long fVowaaPKCIhOQFQKCHxcqVxTmHiuCe23564351 = 69674845;    long fVowaaPKCIhOQFQKCHxcqVxTmHiuCe992413 = -307266002;    long fVowaaPKCIhOQFQKCHxcqVxTmHiuCe15402429 = -132850625;    long fVowaaPKCIhOQFQKCHxcqVxTmHiuCe22389502 = -481174223;    long fVowaaPKCIhOQFQKCHxcqVxTmHiuCe50123280 = 66470522;    long fVowaaPKCIhOQFQKCHxcqVxTmHiuCe98557509 = -486780930;    long fVowaaPKCIhOQFQKCHxcqVxTmHiuCe55114338 = -826834872;    long fVowaaPKCIhOQFQKCHxcqVxTmHiuCe88526117 = -244278610;    long fVowaaPKCIhOQFQKCHxcqVxTmHiuCe92729313 = 82233057;    long fVowaaPKCIhOQFQKCHxcqVxTmHiuCe52705366 = -881954081;    long fVowaaPKCIhOQFQKCHxcqVxTmHiuCe62802896 = -5178500;    long fVowaaPKCIhOQFQKCHxcqVxTmHiuCe47438565 = -69676360;    long fVowaaPKCIhOQFQKCHxcqVxTmHiuCe13782471 = -450499655;    long fVowaaPKCIhOQFQKCHxcqVxTmHiuCe86207946 = -555181433;    long fVowaaPKCIhOQFQKCHxcqVxTmHiuCe68060628 = -703066690;    long fVowaaPKCIhOQFQKCHxcqVxTmHiuCe5927561 = -381675054;    long fVowaaPKCIhOQFQKCHxcqVxTmHiuCe61676544 = -866212368;    long fVowaaPKCIhOQFQKCHxcqVxTmHiuCe55018623 = -116530553;    long fVowaaPKCIhOQFQKCHxcqVxTmHiuCe89709269 = -626811030;    long fVowaaPKCIhOQFQKCHxcqVxTmHiuCe64706414 = -828810797;    long fVowaaPKCIhOQFQKCHxcqVxTmHiuCe5817394 = -474009051;    long fVowaaPKCIhOQFQKCHxcqVxTmHiuCe69879768 = -750657398;    long fVowaaPKCIhOQFQKCHxcqVxTmHiuCe25573495 = -453260678;    long fVowaaPKCIhOQFQKCHxcqVxTmHiuCe34126615 = -784307566;    long fVowaaPKCIhOQFQKCHxcqVxTmHiuCe85241751 = -380894293;    long fVowaaPKCIhOQFQKCHxcqVxTmHiuCe6468522 = -729472302;    long fVowaaPKCIhOQFQKCHxcqVxTmHiuCe73621897 = -165646047;    long fVowaaPKCIhOQFQKCHxcqVxTmHiuCe57289914 = -827510864;    long fVowaaPKCIhOQFQKCHxcqVxTmHiuCe89892582 = -968931056;     fVowaaPKCIhOQFQKCHxcqVxTmHiuCe42155563 = fVowaaPKCIhOQFQKCHxcqVxTmHiuCe82770155;     fVowaaPKCIhOQFQKCHxcqVxTmHiuCe82770155 = fVowaaPKCIhOQFQKCHxcqVxTmHiuCe51173389;     fVowaaPKCIhOQFQKCHxcqVxTmHiuCe51173389 = fVowaaPKCIhOQFQKCHxcqVxTmHiuCe3438980;     fVowaaPKCIhOQFQKCHxcqVxTmHiuCe3438980 = fVowaaPKCIhOQFQKCHxcqVxTmHiuCe7627112;     fVowaaPKCIhOQFQKCHxcqVxTmHiuCe7627112 = fVowaaPKCIhOQFQKCHxcqVxTmHiuCe42760443;     fVowaaPKCIhOQFQKCHxcqVxTmHiuCe42760443 = fVowaaPKCIhOQFQKCHxcqVxTmHiuCe91793579;     fVowaaPKCIhOQFQKCHxcqVxTmHiuCe91793579 = fVowaaPKCIhOQFQKCHxcqVxTmHiuCe19962908;     fVowaaPKCIhOQFQKCHxcqVxTmHiuCe19962908 = fVowaaPKCIhOQFQKCHxcqVxTmHiuCe61019951;     fVowaaPKCIhOQFQKCHxcqVxTmHiuCe61019951 = fVowaaPKCIhOQFQKCHxcqVxTmHiuCe52340276;     fVowaaPKCIhOQFQKCHxcqVxTmHiuCe52340276 = fVowaaPKCIhOQFQKCHxcqVxTmHiuCe30136978;     fVowaaPKCIhOQFQKCHxcqVxTmHiuCe30136978 = fVowaaPKCIhOQFQKCHxcqVxTmHiuCe54075414;     fVowaaPKCIhOQFQKCHxcqVxTmHiuCe54075414 = fVowaaPKCIhOQFQKCHxcqVxTmHiuCe30921435;     fVowaaPKCIhOQFQKCHxcqVxTmHiuCe30921435 = fVowaaPKCIhOQFQKCHxcqVxTmHiuCe36387262;     fVowaaPKCIhOQFQKCHxcqVxTmHiuCe36387262 = fVowaaPKCIhOQFQKCHxcqVxTmHiuCe47618404;     fVowaaPKCIhOQFQKCHxcqVxTmHiuCe47618404 = fVowaaPKCIhOQFQKCHxcqVxTmHiuCe16709747;     fVowaaPKCIhOQFQKCHxcqVxTmHiuCe16709747 = fVowaaPKCIhOQFQKCHxcqVxTmHiuCe92806935;     fVowaaPKCIhOQFQKCHxcqVxTmHiuCe92806935 = fVowaaPKCIhOQFQKCHxcqVxTmHiuCe14000726;     fVowaaPKCIhOQFQKCHxcqVxTmHiuCe14000726 = fVowaaPKCIhOQFQKCHxcqVxTmHiuCe15137317;     fVowaaPKCIhOQFQKCHxcqVxTmHiuCe15137317 = fVowaaPKCIhOQFQKCHxcqVxTmHiuCe43601871;     fVowaaPKCIhOQFQKCHxcqVxTmHiuCe43601871 = fVowaaPKCIhOQFQKCHxcqVxTmHiuCe60181972;     fVowaaPKCIhOQFQKCHxcqVxTmHiuCe60181972 = fVowaaPKCIhOQFQKCHxcqVxTmHiuCe63284997;     fVowaaPKCIhOQFQKCHxcqVxTmHiuCe63284997 = fVowaaPKCIhOQFQKCHxcqVxTmHiuCe52092884;     fVowaaPKCIhOQFQKCHxcqVxTmHiuCe52092884 = fVowaaPKCIhOQFQKCHxcqVxTmHiuCe55920713;     fVowaaPKCIhOQFQKCHxcqVxTmHiuCe55920713 = fVowaaPKCIhOQFQKCHxcqVxTmHiuCe4076163;     fVowaaPKCIhOQFQKCHxcqVxTmHiuCe4076163 = fVowaaPKCIhOQFQKCHxcqVxTmHiuCe20192293;     fVowaaPKCIhOQFQKCHxcqVxTmHiuCe20192293 = fVowaaPKCIhOQFQKCHxcqVxTmHiuCe77152170;     fVowaaPKCIhOQFQKCHxcqVxTmHiuCe77152170 = fVowaaPKCIhOQFQKCHxcqVxTmHiuCe55822962;     fVowaaPKCIhOQFQKCHxcqVxTmHiuCe55822962 = fVowaaPKCIhOQFQKCHxcqVxTmHiuCe16372737;     fVowaaPKCIhOQFQKCHxcqVxTmHiuCe16372737 = fVowaaPKCIhOQFQKCHxcqVxTmHiuCe69914360;     fVowaaPKCIhOQFQKCHxcqVxTmHiuCe69914360 = fVowaaPKCIhOQFQKCHxcqVxTmHiuCe12651645;     fVowaaPKCIhOQFQKCHxcqVxTmHiuCe12651645 = fVowaaPKCIhOQFQKCHxcqVxTmHiuCe52634964;     fVowaaPKCIhOQFQKCHxcqVxTmHiuCe52634964 = fVowaaPKCIhOQFQKCHxcqVxTmHiuCe45553014;     fVowaaPKCIhOQFQKCHxcqVxTmHiuCe45553014 = fVowaaPKCIhOQFQKCHxcqVxTmHiuCe46638180;     fVowaaPKCIhOQFQKCHxcqVxTmHiuCe46638180 = fVowaaPKCIhOQFQKCHxcqVxTmHiuCe68579835;     fVowaaPKCIhOQFQKCHxcqVxTmHiuCe68579835 = fVowaaPKCIhOQFQKCHxcqVxTmHiuCe95383685;     fVowaaPKCIhOQFQKCHxcqVxTmHiuCe95383685 = fVowaaPKCIhOQFQKCHxcqVxTmHiuCe1070071;     fVowaaPKCIhOQFQKCHxcqVxTmHiuCe1070071 = fVowaaPKCIhOQFQKCHxcqVxTmHiuCe69242584;     fVowaaPKCIhOQFQKCHxcqVxTmHiuCe69242584 = fVowaaPKCIhOQFQKCHxcqVxTmHiuCe13008314;     fVowaaPKCIhOQFQKCHxcqVxTmHiuCe13008314 = fVowaaPKCIhOQFQKCHxcqVxTmHiuCe99734887;     fVowaaPKCIhOQFQKCHxcqVxTmHiuCe99734887 = fVowaaPKCIhOQFQKCHxcqVxTmHiuCe21212369;     fVowaaPKCIhOQFQKCHxcqVxTmHiuCe21212369 = fVowaaPKCIhOQFQKCHxcqVxTmHiuCe10058693;     fVowaaPKCIhOQFQKCHxcqVxTmHiuCe10058693 = fVowaaPKCIhOQFQKCHxcqVxTmHiuCe64727488;     fVowaaPKCIhOQFQKCHxcqVxTmHiuCe64727488 = fVowaaPKCIhOQFQKCHxcqVxTmHiuCe96978545;     fVowaaPKCIhOQFQKCHxcqVxTmHiuCe96978545 = fVowaaPKCIhOQFQKCHxcqVxTmHiuCe67394596;     fVowaaPKCIhOQFQKCHxcqVxTmHiuCe67394596 = fVowaaPKCIhOQFQKCHxcqVxTmHiuCe11346850;     fVowaaPKCIhOQFQKCHxcqVxTmHiuCe11346850 = fVowaaPKCIhOQFQKCHxcqVxTmHiuCe67486927;     fVowaaPKCIhOQFQKCHxcqVxTmHiuCe67486927 = fVowaaPKCIhOQFQKCHxcqVxTmHiuCe14349274;     fVowaaPKCIhOQFQKCHxcqVxTmHiuCe14349274 = fVowaaPKCIhOQFQKCHxcqVxTmHiuCe8384398;     fVowaaPKCIhOQFQKCHxcqVxTmHiuCe8384398 = fVowaaPKCIhOQFQKCHxcqVxTmHiuCe2590266;     fVowaaPKCIhOQFQKCHxcqVxTmHiuCe2590266 = fVowaaPKCIhOQFQKCHxcqVxTmHiuCe83706414;     fVowaaPKCIhOQFQKCHxcqVxTmHiuCe83706414 = fVowaaPKCIhOQFQKCHxcqVxTmHiuCe44591017;     fVowaaPKCIhOQFQKCHxcqVxTmHiuCe44591017 = fVowaaPKCIhOQFQKCHxcqVxTmHiuCe46707403;     fVowaaPKCIhOQFQKCHxcqVxTmHiuCe46707403 = fVowaaPKCIhOQFQKCHxcqVxTmHiuCe83876469;     fVowaaPKCIhOQFQKCHxcqVxTmHiuCe83876469 = fVowaaPKCIhOQFQKCHxcqVxTmHiuCe91619556;     fVowaaPKCIhOQFQKCHxcqVxTmHiuCe91619556 = fVowaaPKCIhOQFQKCHxcqVxTmHiuCe78870565;     fVowaaPKCIhOQFQKCHxcqVxTmHiuCe78870565 = fVowaaPKCIhOQFQKCHxcqVxTmHiuCe30677272;     fVowaaPKCIhOQFQKCHxcqVxTmHiuCe30677272 = fVowaaPKCIhOQFQKCHxcqVxTmHiuCe95252676;     fVowaaPKCIhOQFQKCHxcqVxTmHiuCe95252676 = fVowaaPKCIhOQFQKCHxcqVxTmHiuCe99362816;     fVowaaPKCIhOQFQKCHxcqVxTmHiuCe99362816 = fVowaaPKCIhOQFQKCHxcqVxTmHiuCe87434818;     fVowaaPKCIhOQFQKCHxcqVxTmHiuCe87434818 = fVowaaPKCIhOQFQKCHxcqVxTmHiuCe65608273;     fVowaaPKCIhOQFQKCHxcqVxTmHiuCe65608273 = fVowaaPKCIhOQFQKCHxcqVxTmHiuCe35970618;     fVowaaPKCIhOQFQKCHxcqVxTmHiuCe35970618 = fVowaaPKCIhOQFQKCHxcqVxTmHiuCe3590171;     fVowaaPKCIhOQFQKCHxcqVxTmHiuCe3590171 = fVowaaPKCIhOQFQKCHxcqVxTmHiuCe91105591;     fVowaaPKCIhOQFQKCHxcqVxTmHiuCe91105591 = fVowaaPKCIhOQFQKCHxcqVxTmHiuCe39688631;     fVowaaPKCIhOQFQKCHxcqVxTmHiuCe39688631 = fVowaaPKCIhOQFQKCHxcqVxTmHiuCe77502013;     fVowaaPKCIhOQFQKCHxcqVxTmHiuCe77502013 = fVowaaPKCIhOQFQKCHxcqVxTmHiuCe8522401;     fVowaaPKCIhOQFQKCHxcqVxTmHiuCe8522401 = fVowaaPKCIhOQFQKCHxcqVxTmHiuCe84283255;     fVowaaPKCIhOQFQKCHxcqVxTmHiuCe84283255 = fVowaaPKCIhOQFQKCHxcqVxTmHiuCe67807426;     fVowaaPKCIhOQFQKCHxcqVxTmHiuCe67807426 = fVowaaPKCIhOQFQKCHxcqVxTmHiuCe52234718;     fVowaaPKCIhOQFQKCHxcqVxTmHiuCe52234718 = fVowaaPKCIhOQFQKCHxcqVxTmHiuCe15639677;     fVowaaPKCIhOQFQKCHxcqVxTmHiuCe15639677 = fVowaaPKCIhOQFQKCHxcqVxTmHiuCe23564351;     fVowaaPKCIhOQFQKCHxcqVxTmHiuCe23564351 = fVowaaPKCIhOQFQKCHxcqVxTmHiuCe992413;     fVowaaPKCIhOQFQKCHxcqVxTmHiuCe992413 = fVowaaPKCIhOQFQKCHxcqVxTmHiuCe15402429;     fVowaaPKCIhOQFQKCHxcqVxTmHiuCe15402429 = fVowaaPKCIhOQFQKCHxcqVxTmHiuCe22389502;     fVowaaPKCIhOQFQKCHxcqVxTmHiuCe22389502 = fVowaaPKCIhOQFQKCHxcqVxTmHiuCe50123280;     fVowaaPKCIhOQFQKCHxcqVxTmHiuCe50123280 = fVowaaPKCIhOQFQKCHxcqVxTmHiuCe98557509;     fVowaaPKCIhOQFQKCHxcqVxTmHiuCe98557509 = fVowaaPKCIhOQFQKCHxcqVxTmHiuCe55114338;     fVowaaPKCIhOQFQKCHxcqVxTmHiuCe55114338 = fVowaaPKCIhOQFQKCHxcqVxTmHiuCe88526117;     fVowaaPKCIhOQFQKCHxcqVxTmHiuCe88526117 = fVowaaPKCIhOQFQKCHxcqVxTmHiuCe92729313;     fVowaaPKCIhOQFQKCHxcqVxTmHiuCe92729313 = fVowaaPKCIhOQFQKCHxcqVxTmHiuCe52705366;     fVowaaPKCIhOQFQKCHxcqVxTmHiuCe52705366 = fVowaaPKCIhOQFQKCHxcqVxTmHiuCe62802896;     fVowaaPKCIhOQFQKCHxcqVxTmHiuCe62802896 = fVowaaPKCIhOQFQKCHxcqVxTmHiuCe47438565;     fVowaaPKCIhOQFQKCHxcqVxTmHiuCe47438565 = fVowaaPKCIhOQFQKCHxcqVxTmHiuCe13782471;     fVowaaPKCIhOQFQKCHxcqVxTmHiuCe13782471 = fVowaaPKCIhOQFQKCHxcqVxTmHiuCe86207946;     fVowaaPKCIhOQFQKCHxcqVxTmHiuCe86207946 = fVowaaPKCIhOQFQKCHxcqVxTmHiuCe68060628;     fVowaaPKCIhOQFQKCHxcqVxTmHiuCe68060628 = fVowaaPKCIhOQFQKCHxcqVxTmHiuCe5927561;     fVowaaPKCIhOQFQKCHxcqVxTmHiuCe5927561 = fVowaaPKCIhOQFQKCHxcqVxTmHiuCe61676544;     fVowaaPKCIhOQFQKCHxcqVxTmHiuCe61676544 = fVowaaPKCIhOQFQKCHxcqVxTmHiuCe55018623;     fVowaaPKCIhOQFQKCHxcqVxTmHiuCe55018623 = fVowaaPKCIhOQFQKCHxcqVxTmHiuCe89709269;     fVowaaPKCIhOQFQKCHxcqVxTmHiuCe89709269 = fVowaaPKCIhOQFQKCHxcqVxTmHiuCe64706414;     fVowaaPKCIhOQFQKCHxcqVxTmHiuCe64706414 = fVowaaPKCIhOQFQKCHxcqVxTmHiuCe5817394;     fVowaaPKCIhOQFQKCHxcqVxTmHiuCe5817394 = fVowaaPKCIhOQFQKCHxcqVxTmHiuCe69879768;     fVowaaPKCIhOQFQKCHxcqVxTmHiuCe69879768 = fVowaaPKCIhOQFQKCHxcqVxTmHiuCe25573495;     fVowaaPKCIhOQFQKCHxcqVxTmHiuCe25573495 = fVowaaPKCIhOQFQKCHxcqVxTmHiuCe34126615;     fVowaaPKCIhOQFQKCHxcqVxTmHiuCe34126615 = fVowaaPKCIhOQFQKCHxcqVxTmHiuCe85241751;     fVowaaPKCIhOQFQKCHxcqVxTmHiuCe85241751 = fVowaaPKCIhOQFQKCHxcqVxTmHiuCe6468522;     fVowaaPKCIhOQFQKCHxcqVxTmHiuCe6468522 = fVowaaPKCIhOQFQKCHxcqVxTmHiuCe73621897;     fVowaaPKCIhOQFQKCHxcqVxTmHiuCe73621897 = fVowaaPKCIhOQFQKCHxcqVxTmHiuCe57289914;     fVowaaPKCIhOQFQKCHxcqVxTmHiuCe57289914 = fVowaaPKCIhOQFQKCHxcqVxTmHiuCe89892582;     fVowaaPKCIhOQFQKCHxcqVxTmHiuCe89892582 = fVowaaPKCIhOQFQKCHxcqVxTmHiuCe42155563;}
// Junk Finished

// Junk Code By Troll Face & Thaisen's Gen
void wdmUxlcbVCSnuXPqOwxDZJIveosEJe30445090() {     double okJUmvJiGOMRtxCDyQTwhdvOhmUrpI60769918 = 94696538;    double okJUmvJiGOMRtxCDyQTwhdvOhmUrpI40046791 = -224316433;    double okJUmvJiGOMRtxCDyQTwhdvOhmUrpI44760581 = -358142441;    double okJUmvJiGOMRtxCDyQTwhdvOhmUrpI81718675 = -867033584;    double okJUmvJiGOMRtxCDyQTwhdvOhmUrpI9853846 = -67462135;    double okJUmvJiGOMRtxCDyQTwhdvOhmUrpI16860748 = -704757057;    double okJUmvJiGOMRtxCDyQTwhdvOhmUrpI60470063 = 69745725;    double okJUmvJiGOMRtxCDyQTwhdvOhmUrpI71270347 = -310006874;    double okJUmvJiGOMRtxCDyQTwhdvOhmUrpI53487676 = -917879874;    double okJUmvJiGOMRtxCDyQTwhdvOhmUrpI81187974 = -927149479;    double okJUmvJiGOMRtxCDyQTwhdvOhmUrpI54742460 = -397718702;    double okJUmvJiGOMRtxCDyQTwhdvOhmUrpI12280106 = -782243265;    double okJUmvJiGOMRtxCDyQTwhdvOhmUrpI2117152 = -700998308;    double okJUmvJiGOMRtxCDyQTwhdvOhmUrpI85175094 = -648022304;    double okJUmvJiGOMRtxCDyQTwhdvOhmUrpI91100752 = -227818970;    double okJUmvJiGOMRtxCDyQTwhdvOhmUrpI63062163 = -206176936;    double okJUmvJiGOMRtxCDyQTwhdvOhmUrpI20060369 = -730354019;    double okJUmvJiGOMRtxCDyQTwhdvOhmUrpI45887340 = 85091017;    double okJUmvJiGOMRtxCDyQTwhdvOhmUrpI80233855 = -198363234;    double okJUmvJiGOMRtxCDyQTwhdvOhmUrpI50176974 = -799568775;    double okJUmvJiGOMRtxCDyQTwhdvOhmUrpI36714236 = -140246813;    double okJUmvJiGOMRtxCDyQTwhdvOhmUrpI57443486 = -855467959;    double okJUmvJiGOMRtxCDyQTwhdvOhmUrpI16411408 = -504541653;    double okJUmvJiGOMRtxCDyQTwhdvOhmUrpI83459242 = 22456202;    double okJUmvJiGOMRtxCDyQTwhdvOhmUrpI51307972 = -92189392;    double okJUmvJiGOMRtxCDyQTwhdvOhmUrpI43443120 = -860892996;    double okJUmvJiGOMRtxCDyQTwhdvOhmUrpI70222658 = -579246560;    double okJUmvJiGOMRtxCDyQTwhdvOhmUrpI86520725 = -507082840;    double okJUmvJiGOMRtxCDyQTwhdvOhmUrpI82302668 = -299980700;    double okJUmvJiGOMRtxCDyQTwhdvOhmUrpI35700932 = -492122308;    double okJUmvJiGOMRtxCDyQTwhdvOhmUrpI36180099 = -708834373;    double okJUmvJiGOMRtxCDyQTwhdvOhmUrpI68963334 = -367605267;    double okJUmvJiGOMRtxCDyQTwhdvOhmUrpI58902731 = -600973909;    double okJUmvJiGOMRtxCDyQTwhdvOhmUrpI25403169 = -223855226;    double okJUmvJiGOMRtxCDyQTwhdvOhmUrpI47775481 = 87843499;    double okJUmvJiGOMRtxCDyQTwhdvOhmUrpI66103949 = -717346623;    double okJUmvJiGOMRtxCDyQTwhdvOhmUrpI55858705 = -822661631;    double okJUmvJiGOMRtxCDyQTwhdvOhmUrpI56110380 = -589169688;    double okJUmvJiGOMRtxCDyQTwhdvOhmUrpI33743707 = -938984416;    double okJUmvJiGOMRtxCDyQTwhdvOhmUrpI81616913 = -801225525;    double okJUmvJiGOMRtxCDyQTwhdvOhmUrpI38977806 = -675090323;    double okJUmvJiGOMRtxCDyQTwhdvOhmUrpI49606156 = -506175933;    double okJUmvJiGOMRtxCDyQTwhdvOhmUrpI8869900 = -639501055;    double okJUmvJiGOMRtxCDyQTwhdvOhmUrpI12572092 = -724455061;    double okJUmvJiGOMRtxCDyQTwhdvOhmUrpI49339876 = -13341754;    double okJUmvJiGOMRtxCDyQTwhdvOhmUrpI52895241 = -750602509;    double okJUmvJiGOMRtxCDyQTwhdvOhmUrpI47230671 = -258870659;    double okJUmvJiGOMRtxCDyQTwhdvOhmUrpI95750300 = 30718745;    double okJUmvJiGOMRtxCDyQTwhdvOhmUrpI37084659 = 68481770;    double okJUmvJiGOMRtxCDyQTwhdvOhmUrpI91979627 = -309056007;    double okJUmvJiGOMRtxCDyQTwhdvOhmUrpI90790257 = -554332827;    double okJUmvJiGOMRtxCDyQTwhdvOhmUrpI9393245 = -318067769;    double okJUmvJiGOMRtxCDyQTwhdvOhmUrpI61343589 = -799722053;    double okJUmvJiGOMRtxCDyQTwhdvOhmUrpI18664634 = -457224841;    double okJUmvJiGOMRtxCDyQTwhdvOhmUrpI93730854 = -60584932;    double okJUmvJiGOMRtxCDyQTwhdvOhmUrpI3326432 = -49835504;    double okJUmvJiGOMRtxCDyQTwhdvOhmUrpI23635383 = -719774780;    double okJUmvJiGOMRtxCDyQTwhdvOhmUrpI61301338 = -280598643;    double okJUmvJiGOMRtxCDyQTwhdvOhmUrpI30410703 = -674844193;    double okJUmvJiGOMRtxCDyQTwhdvOhmUrpI66410725 = -206569140;    double okJUmvJiGOMRtxCDyQTwhdvOhmUrpI46638090 = -25510497;    double okJUmvJiGOMRtxCDyQTwhdvOhmUrpI73949337 = -423171436;    double okJUmvJiGOMRtxCDyQTwhdvOhmUrpI88967678 = 89973826;    double okJUmvJiGOMRtxCDyQTwhdvOhmUrpI17786744 = -325757567;    double okJUmvJiGOMRtxCDyQTwhdvOhmUrpI45007875 = -118315107;    double okJUmvJiGOMRtxCDyQTwhdvOhmUrpI85779125 = 69886565;    double okJUmvJiGOMRtxCDyQTwhdvOhmUrpI53377374 = -81269356;    double okJUmvJiGOMRtxCDyQTwhdvOhmUrpI76713982 = -377143083;    double okJUmvJiGOMRtxCDyQTwhdvOhmUrpI37399613 = -635865804;    double okJUmvJiGOMRtxCDyQTwhdvOhmUrpI24996803 = -510472348;    double okJUmvJiGOMRtxCDyQTwhdvOhmUrpI7203458 = -383515305;    double okJUmvJiGOMRtxCDyQTwhdvOhmUrpI63949989 = -41184331;    double okJUmvJiGOMRtxCDyQTwhdvOhmUrpI12143633 = 24075433;    double okJUmvJiGOMRtxCDyQTwhdvOhmUrpI98616941 = -397137709;    double okJUmvJiGOMRtxCDyQTwhdvOhmUrpI11199168 = -24478452;    double okJUmvJiGOMRtxCDyQTwhdvOhmUrpI87108079 = -634070881;    double okJUmvJiGOMRtxCDyQTwhdvOhmUrpI48573586 = -115966904;    double okJUmvJiGOMRtxCDyQTwhdvOhmUrpI3839316 = -780086593;    double okJUmvJiGOMRtxCDyQTwhdvOhmUrpI34119367 = -964202045;    double okJUmvJiGOMRtxCDyQTwhdvOhmUrpI98412730 = -341586884;    double okJUmvJiGOMRtxCDyQTwhdvOhmUrpI96212449 = -502022338;    double okJUmvJiGOMRtxCDyQTwhdvOhmUrpI74472357 = -509965305;    double okJUmvJiGOMRtxCDyQTwhdvOhmUrpI49436066 = -475564610;    double okJUmvJiGOMRtxCDyQTwhdvOhmUrpI90323040 = -990924693;    double okJUmvJiGOMRtxCDyQTwhdvOhmUrpI44910675 = -937789481;    double okJUmvJiGOMRtxCDyQTwhdvOhmUrpI26786855 = -290766605;    double okJUmvJiGOMRtxCDyQTwhdvOhmUrpI7619745 = -567883215;    double okJUmvJiGOMRtxCDyQTwhdvOhmUrpI40238098 = -43749068;    double okJUmvJiGOMRtxCDyQTwhdvOhmUrpI31672315 = -63270295;    double okJUmvJiGOMRtxCDyQTwhdvOhmUrpI44449050 = -862320997;    double okJUmvJiGOMRtxCDyQTwhdvOhmUrpI42468566 = -997571843;    double okJUmvJiGOMRtxCDyQTwhdvOhmUrpI94557366 = -442062989;    double okJUmvJiGOMRtxCDyQTwhdvOhmUrpI25699677 = -914325496;    double okJUmvJiGOMRtxCDyQTwhdvOhmUrpI67332982 = -632415277;    double okJUmvJiGOMRtxCDyQTwhdvOhmUrpI34978824 = -675715028;    double okJUmvJiGOMRtxCDyQTwhdvOhmUrpI65028469 = -151918887;    double okJUmvJiGOMRtxCDyQTwhdvOhmUrpI60638477 = -496149760;    double okJUmvJiGOMRtxCDyQTwhdvOhmUrpI91083155 = -213743488;    double okJUmvJiGOMRtxCDyQTwhdvOhmUrpI67564217 = -506139955;    double okJUmvJiGOMRtxCDyQTwhdvOhmUrpI63560750 = 94696538;     okJUmvJiGOMRtxCDyQTwhdvOhmUrpI60769918 = okJUmvJiGOMRtxCDyQTwhdvOhmUrpI40046791;     okJUmvJiGOMRtxCDyQTwhdvOhmUrpI40046791 = okJUmvJiGOMRtxCDyQTwhdvOhmUrpI44760581;     okJUmvJiGOMRtxCDyQTwhdvOhmUrpI44760581 = okJUmvJiGOMRtxCDyQTwhdvOhmUrpI81718675;     okJUmvJiGOMRtxCDyQTwhdvOhmUrpI81718675 = okJUmvJiGOMRtxCDyQTwhdvOhmUrpI9853846;     okJUmvJiGOMRtxCDyQTwhdvOhmUrpI9853846 = okJUmvJiGOMRtxCDyQTwhdvOhmUrpI16860748;     okJUmvJiGOMRtxCDyQTwhdvOhmUrpI16860748 = okJUmvJiGOMRtxCDyQTwhdvOhmUrpI60470063;     okJUmvJiGOMRtxCDyQTwhdvOhmUrpI60470063 = okJUmvJiGOMRtxCDyQTwhdvOhmUrpI71270347;     okJUmvJiGOMRtxCDyQTwhdvOhmUrpI71270347 = okJUmvJiGOMRtxCDyQTwhdvOhmUrpI53487676;     okJUmvJiGOMRtxCDyQTwhdvOhmUrpI53487676 = okJUmvJiGOMRtxCDyQTwhdvOhmUrpI81187974;     okJUmvJiGOMRtxCDyQTwhdvOhmUrpI81187974 = okJUmvJiGOMRtxCDyQTwhdvOhmUrpI54742460;     okJUmvJiGOMRtxCDyQTwhdvOhmUrpI54742460 = okJUmvJiGOMRtxCDyQTwhdvOhmUrpI12280106;     okJUmvJiGOMRtxCDyQTwhdvOhmUrpI12280106 = okJUmvJiGOMRtxCDyQTwhdvOhmUrpI2117152;     okJUmvJiGOMRtxCDyQTwhdvOhmUrpI2117152 = okJUmvJiGOMRtxCDyQTwhdvOhmUrpI85175094;     okJUmvJiGOMRtxCDyQTwhdvOhmUrpI85175094 = okJUmvJiGOMRtxCDyQTwhdvOhmUrpI91100752;     okJUmvJiGOMRtxCDyQTwhdvOhmUrpI91100752 = okJUmvJiGOMRtxCDyQTwhdvOhmUrpI63062163;     okJUmvJiGOMRtxCDyQTwhdvOhmUrpI63062163 = okJUmvJiGOMRtxCDyQTwhdvOhmUrpI20060369;     okJUmvJiGOMRtxCDyQTwhdvOhmUrpI20060369 = okJUmvJiGOMRtxCDyQTwhdvOhmUrpI45887340;     okJUmvJiGOMRtxCDyQTwhdvOhmUrpI45887340 = okJUmvJiGOMRtxCDyQTwhdvOhmUrpI80233855;     okJUmvJiGOMRtxCDyQTwhdvOhmUrpI80233855 = okJUmvJiGOMRtxCDyQTwhdvOhmUrpI50176974;     okJUmvJiGOMRtxCDyQTwhdvOhmUrpI50176974 = okJUmvJiGOMRtxCDyQTwhdvOhmUrpI36714236;     okJUmvJiGOMRtxCDyQTwhdvOhmUrpI36714236 = okJUmvJiGOMRtxCDyQTwhdvOhmUrpI57443486;     okJUmvJiGOMRtxCDyQTwhdvOhmUrpI57443486 = okJUmvJiGOMRtxCDyQTwhdvOhmUrpI16411408;     okJUmvJiGOMRtxCDyQTwhdvOhmUrpI16411408 = okJUmvJiGOMRtxCDyQTwhdvOhmUrpI83459242;     okJUmvJiGOMRtxCDyQTwhdvOhmUrpI83459242 = okJUmvJiGOMRtxCDyQTwhdvOhmUrpI51307972;     okJUmvJiGOMRtxCDyQTwhdvOhmUrpI51307972 = okJUmvJiGOMRtxCDyQTwhdvOhmUrpI43443120;     okJUmvJiGOMRtxCDyQTwhdvOhmUrpI43443120 = okJUmvJiGOMRtxCDyQTwhdvOhmUrpI70222658;     okJUmvJiGOMRtxCDyQTwhdvOhmUrpI70222658 = okJUmvJiGOMRtxCDyQTwhdvOhmUrpI86520725;     okJUmvJiGOMRtxCDyQTwhdvOhmUrpI86520725 = okJUmvJiGOMRtxCDyQTwhdvOhmUrpI82302668;     okJUmvJiGOMRtxCDyQTwhdvOhmUrpI82302668 = okJUmvJiGOMRtxCDyQTwhdvOhmUrpI35700932;     okJUmvJiGOMRtxCDyQTwhdvOhmUrpI35700932 = okJUmvJiGOMRtxCDyQTwhdvOhmUrpI36180099;     okJUmvJiGOMRtxCDyQTwhdvOhmUrpI36180099 = okJUmvJiGOMRtxCDyQTwhdvOhmUrpI68963334;     okJUmvJiGOMRtxCDyQTwhdvOhmUrpI68963334 = okJUmvJiGOMRtxCDyQTwhdvOhmUrpI58902731;     okJUmvJiGOMRtxCDyQTwhdvOhmUrpI58902731 = okJUmvJiGOMRtxCDyQTwhdvOhmUrpI25403169;     okJUmvJiGOMRtxCDyQTwhdvOhmUrpI25403169 = okJUmvJiGOMRtxCDyQTwhdvOhmUrpI47775481;     okJUmvJiGOMRtxCDyQTwhdvOhmUrpI47775481 = okJUmvJiGOMRtxCDyQTwhdvOhmUrpI66103949;     okJUmvJiGOMRtxCDyQTwhdvOhmUrpI66103949 = okJUmvJiGOMRtxCDyQTwhdvOhmUrpI55858705;     okJUmvJiGOMRtxCDyQTwhdvOhmUrpI55858705 = okJUmvJiGOMRtxCDyQTwhdvOhmUrpI56110380;     okJUmvJiGOMRtxCDyQTwhdvOhmUrpI56110380 = okJUmvJiGOMRtxCDyQTwhdvOhmUrpI33743707;     okJUmvJiGOMRtxCDyQTwhdvOhmUrpI33743707 = okJUmvJiGOMRtxCDyQTwhdvOhmUrpI81616913;     okJUmvJiGOMRtxCDyQTwhdvOhmUrpI81616913 = okJUmvJiGOMRtxCDyQTwhdvOhmUrpI38977806;     okJUmvJiGOMRtxCDyQTwhdvOhmUrpI38977806 = okJUmvJiGOMRtxCDyQTwhdvOhmUrpI49606156;     okJUmvJiGOMRtxCDyQTwhdvOhmUrpI49606156 = okJUmvJiGOMRtxCDyQTwhdvOhmUrpI8869900;     okJUmvJiGOMRtxCDyQTwhdvOhmUrpI8869900 = okJUmvJiGOMRtxCDyQTwhdvOhmUrpI12572092;     okJUmvJiGOMRtxCDyQTwhdvOhmUrpI12572092 = okJUmvJiGOMRtxCDyQTwhdvOhmUrpI49339876;     okJUmvJiGOMRtxCDyQTwhdvOhmUrpI49339876 = okJUmvJiGOMRtxCDyQTwhdvOhmUrpI52895241;     okJUmvJiGOMRtxCDyQTwhdvOhmUrpI52895241 = okJUmvJiGOMRtxCDyQTwhdvOhmUrpI47230671;     okJUmvJiGOMRtxCDyQTwhdvOhmUrpI47230671 = okJUmvJiGOMRtxCDyQTwhdvOhmUrpI95750300;     okJUmvJiGOMRtxCDyQTwhdvOhmUrpI95750300 = okJUmvJiGOMRtxCDyQTwhdvOhmUrpI37084659;     okJUmvJiGOMRtxCDyQTwhdvOhmUrpI37084659 = okJUmvJiGOMRtxCDyQTwhdvOhmUrpI91979627;     okJUmvJiGOMRtxCDyQTwhdvOhmUrpI91979627 = okJUmvJiGOMRtxCDyQTwhdvOhmUrpI90790257;     okJUmvJiGOMRtxCDyQTwhdvOhmUrpI90790257 = okJUmvJiGOMRtxCDyQTwhdvOhmUrpI9393245;     okJUmvJiGOMRtxCDyQTwhdvOhmUrpI9393245 = okJUmvJiGOMRtxCDyQTwhdvOhmUrpI61343589;     okJUmvJiGOMRtxCDyQTwhdvOhmUrpI61343589 = okJUmvJiGOMRtxCDyQTwhdvOhmUrpI18664634;     okJUmvJiGOMRtxCDyQTwhdvOhmUrpI18664634 = okJUmvJiGOMRtxCDyQTwhdvOhmUrpI93730854;     okJUmvJiGOMRtxCDyQTwhdvOhmUrpI93730854 = okJUmvJiGOMRtxCDyQTwhdvOhmUrpI3326432;     okJUmvJiGOMRtxCDyQTwhdvOhmUrpI3326432 = okJUmvJiGOMRtxCDyQTwhdvOhmUrpI23635383;     okJUmvJiGOMRtxCDyQTwhdvOhmUrpI23635383 = okJUmvJiGOMRtxCDyQTwhdvOhmUrpI61301338;     okJUmvJiGOMRtxCDyQTwhdvOhmUrpI61301338 = okJUmvJiGOMRtxCDyQTwhdvOhmUrpI30410703;     okJUmvJiGOMRtxCDyQTwhdvOhmUrpI30410703 = okJUmvJiGOMRtxCDyQTwhdvOhmUrpI66410725;     okJUmvJiGOMRtxCDyQTwhdvOhmUrpI66410725 = okJUmvJiGOMRtxCDyQTwhdvOhmUrpI46638090;     okJUmvJiGOMRtxCDyQTwhdvOhmUrpI46638090 = okJUmvJiGOMRtxCDyQTwhdvOhmUrpI73949337;     okJUmvJiGOMRtxCDyQTwhdvOhmUrpI73949337 = okJUmvJiGOMRtxCDyQTwhdvOhmUrpI88967678;     okJUmvJiGOMRtxCDyQTwhdvOhmUrpI88967678 = okJUmvJiGOMRtxCDyQTwhdvOhmUrpI17786744;     okJUmvJiGOMRtxCDyQTwhdvOhmUrpI17786744 = okJUmvJiGOMRtxCDyQTwhdvOhmUrpI45007875;     okJUmvJiGOMRtxCDyQTwhdvOhmUrpI45007875 = okJUmvJiGOMRtxCDyQTwhdvOhmUrpI85779125;     okJUmvJiGOMRtxCDyQTwhdvOhmUrpI85779125 = okJUmvJiGOMRtxCDyQTwhdvOhmUrpI53377374;     okJUmvJiGOMRtxCDyQTwhdvOhmUrpI53377374 = okJUmvJiGOMRtxCDyQTwhdvOhmUrpI76713982;     okJUmvJiGOMRtxCDyQTwhdvOhmUrpI76713982 = okJUmvJiGOMRtxCDyQTwhdvOhmUrpI37399613;     okJUmvJiGOMRtxCDyQTwhdvOhmUrpI37399613 = okJUmvJiGOMRtxCDyQTwhdvOhmUrpI24996803;     okJUmvJiGOMRtxCDyQTwhdvOhmUrpI24996803 = okJUmvJiGOMRtxCDyQTwhdvOhmUrpI7203458;     okJUmvJiGOMRtxCDyQTwhdvOhmUrpI7203458 = okJUmvJiGOMRtxCDyQTwhdvOhmUrpI63949989;     okJUmvJiGOMRtxCDyQTwhdvOhmUrpI63949989 = okJUmvJiGOMRtxCDyQTwhdvOhmUrpI12143633;     okJUmvJiGOMRtxCDyQTwhdvOhmUrpI12143633 = okJUmvJiGOMRtxCDyQTwhdvOhmUrpI98616941;     okJUmvJiGOMRtxCDyQTwhdvOhmUrpI98616941 = okJUmvJiGOMRtxCDyQTwhdvOhmUrpI11199168;     okJUmvJiGOMRtxCDyQTwhdvOhmUrpI11199168 = okJUmvJiGOMRtxCDyQTwhdvOhmUrpI87108079;     okJUmvJiGOMRtxCDyQTwhdvOhmUrpI87108079 = okJUmvJiGOMRtxCDyQTwhdvOhmUrpI48573586;     okJUmvJiGOMRtxCDyQTwhdvOhmUrpI48573586 = okJUmvJiGOMRtxCDyQTwhdvOhmUrpI3839316;     okJUmvJiGOMRtxCDyQTwhdvOhmUrpI3839316 = okJUmvJiGOMRtxCDyQTwhdvOhmUrpI34119367;     okJUmvJiGOMRtxCDyQTwhdvOhmUrpI34119367 = okJUmvJiGOMRtxCDyQTwhdvOhmUrpI98412730;     okJUmvJiGOMRtxCDyQTwhdvOhmUrpI98412730 = okJUmvJiGOMRtxCDyQTwhdvOhmUrpI96212449;     okJUmvJiGOMRtxCDyQTwhdvOhmUrpI96212449 = okJUmvJiGOMRtxCDyQTwhdvOhmUrpI74472357;     okJUmvJiGOMRtxCDyQTwhdvOhmUrpI74472357 = okJUmvJiGOMRtxCDyQTwhdvOhmUrpI49436066;     okJUmvJiGOMRtxCDyQTwhdvOhmUrpI49436066 = okJUmvJiGOMRtxCDyQTwhdvOhmUrpI90323040;     okJUmvJiGOMRtxCDyQTwhdvOhmUrpI90323040 = okJUmvJiGOMRtxCDyQTwhdvOhmUrpI44910675;     okJUmvJiGOMRtxCDyQTwhdvOhmUrpI44910675 = okJUmvJiGOMRtxCDyQTwhdvOhmUrpI26786855;     okJUmvJiGOMRtxCDyQTwhdvOhmUrpI26786855 = okJUmvJiGOMRtxCDyQTwhdvOhmUrpI7619745;     okJUmvJiGOMRtxCDyQTwhdvOhmUrpI7619745 = okJUmvJiGOMRtxCDyQTwhdvOhmUrpI40238098;     okJUmvJiGOMRtxCDyQTwhdvOhmUrpI40238098 = okJUmvJiGOMRtxCDyQTwhdvOhmUrpI31672315;     okJUmvJiGOMRtxCDyQTwhdvOhmUrpI31672315 = okJUmvJiGOMRtxCDyQTwhdvOhmUrpI44449050;     okJUmvJiGOMRtxCDyQTwhdvOhmUrpI44449050 = okJUmvJiGOMRtxCDyQTwhdvOhmUrpI42468566;     okJUmvJiGOMRtxCDyQTwhdvOhmUrpI42468566 = okJUmvJiGOMRtxCDyQTwhdvOhmUrpI94557366;     okJUmvJiGOMRtxCDyQTwhdvOhmUrpI94557366 = okJUmvJiGOMRtxCDyQTwhdvOhmUrpI25699677;     okJUmvJiGOMRtxCDyQTwhdvOhmUrpI25699677 = okJUmvJiGOMRtxCDyQTwhdvOhmUrpI67332982;     okJUmvJiGOMRtxCDyQTwhdvOhmUrpI67332982 = okJUmvJiGOMRtxCDyQTwhdvOhmUrpI34978824;     okJUmvJiGOMRtxCDyQTwhdvOhmUrpI34978824 = okJUmvJiGOMRtxCDyQTwhdvOhmUrpI65028469;     okJUmvJiGOMRtxCDyQTwhdvOhmUrpI65028469 = okJUmvJiGOMRtxCDyQTwhdvOhmUrpI60638477;     okJUmvJiGOMRtxCDyQTwhdvOhmUrpI60638477 = okJUmvJiGOMRtxCDyQTwhdvOhmUrpI91083155;     okJUmvJiGOMRtxCDyQTwhdvOhmUrpI91083155 = okJUmvJiGOMRtxCDyQTwhdvOhmUrpI67564217;     okJUmvJiGOMRtxCDyQTwhdvOhmUrpI67564217 = okJUmvJiGOMRtxCDyQTwhdvOhmUrpI63560750;     okJUmvJiGOMRtxCDyQTwhdvOhmUrpI63560750 = okJUmvJiGOMRtxCDyQTwhdvOhmUrpI60769918;}
// Junk Finished
