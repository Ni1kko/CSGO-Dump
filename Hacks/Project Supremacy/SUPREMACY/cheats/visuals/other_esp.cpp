#include "other_esp.h"
#include "..\autowall\autowall.h"
#include "..\..\ragebot.h"
#include <chrono>
#include "..\..\Resolver_Remake.h"
struct ModColor
{
	float color[3];
	float alpha;

	void SetColorFloat(float r, float g, float b)
	{
		color[0] = r;
		color[1] = g;
		color[2] = b;
	}

	void SetColorFloat(float col[3])
	{
		color[0] = col[0];
		color[1] = col[1];
		color[2] = col[2];
	}

	void SetColor(Color col)
	{
		color[0] = col.r() / 255.f;
		color[1] = col.g() / 255.f;
		color[2] = col.b() / 255.f;
	}

	void GetColorFloat(float col[3])
	{
		col[0] = color[0];
		col[1] = color[1];
		col[2] = color[2];
	}

	Color GetColor()
	{
		return Color(color[0], color[1], color[2]);
	}

	ModColor(float r, float g, float b, float alph)
	{
		color[0] = r;
		color[1] = g;
		color[2] = b;
		alpha = alph;
	}

	ModColor(float col[3], float alph)
	{
		color[0] = col[0];
		color[1] = col[1];
		color[2] = col[2];
		alpha = alph;
	}
};

std::vector<impact_info> impacts;
std::vector<hitmarker_info> hitmarkers;
//std::vector<onetap_su> onetap;
std::unordered_map<MaterialHandle_t, std::pair<ModColor, ModColor>> vguiMaterials;
std::unordered_map<MaterialHandle_t, std::pair<ModColor, ModColor>> worldMaterials;
float r, g, b, a;
bool sprop = false;

void otheresp::antiaim_indicator() {
	auto local_player = static_cast<player_t *>(g_csgo.m_entitylist()->GetClientEntity(g_csgo.m_engine()->GetLocalPlayer()));

	static auto degrees_to_radians = [](float deg) -> float { return deg * (DirectX::XM_PI / 180.f); };

	auto rotated_position = [](Vector start, const float rotation, const float distance) -> Vector {
		const auto rad = degrees_to_radians(rotation);
		start.x += cosf(rad) * distance;
		start.y += sinf(rad) * distance;

		return start;
	};

	if (!g_ctx.available() || !local_player)
		return;

	static const auto real_color = Color(0, 255, 0, 255);
	static const auto fake_color = Color(255, 0, 0, 255);
	static const auto lby_color = Color(0, 0, 255, 255);

	if (!local_player->is_alive24())
		return;

	if (g_cfg.esp.antiaim_indicator) {
		auto client_viewangles = Vector();
		auto
			screen_width = render::get().viewport().right,
			screen_height = render::get().viewport().bottom;

		g_csgo.m_engine()->GetViewAngles(client_viewangles);

		constexpr auto radius = 80.f;

		const auto screen_center = Vector2D(screen_width / 2.f, screen_height / 2.f);
		const auto real_rot = degrees_to_radians(client_viewangles.y - g_ctx.m_globals.real_angles.y - 90);
		const auto fake_rot = degrees_to_radians(client_viewangles.y - g_ctx.m_globals.real_angles.y - 90);
		const auto lby_rot = degrees_to_radians(client_viewangles.y - g_ctx.m_local->m_flLowerBodyYawTarget() - 90);

		auto draw_arrow = [&](float rot, Color color) -> void {
			auto pos_one = Vector2D(screen_center.x + cosf(rot) * radius, screen_center.y + sinf(rot) * radius);
			auto pos_two = Vector2D(screen_center.x + cosf(rot + degrees_to_radians(8)) * (radius - 13.f),
				screen_center.y + sinf(rot + degrees_to_radians(8)) * (radius - 13.f));
			auto pos_three = Vector2D(screen_center.x + cosf(rot - degrees_to_radians(8)) * (radius - 13.f),//x
				screen_center.y + sinf(rot - degrees_to_radians(8)) * (radius - 13.f));//y

			render::get().triangle(
				Vector2D(pos_one.x, pos_one.y),
				Vector2D(pos_two.x, pos_two.y),
				Vector2D(pos_three.x, pos_three.y),
				color);
		};

		draw_arrow(real_rot, real_color);
		draw_arrow(fake_rot, fake_color);
		draw_arrow(lby_rot, lby_color);
	}
}

void otheresp::watermark()
{
	static int fps, old_tick_count;
	if ((g_csgo.m_globals()->m_tickcount - old_tick_count) > 50)
	{
		fps = static_cast<int>(1.f / g_csgo.m_globals()->m_frametime);
		old_tick_count = g_csgo.m_globals()->m_tickcount;
	}

	int screen_x, screen_y;
	g_csgo.m_engine()->GetScreenSize(screen_x, screen_y);

	static float bar_width = 226.8;

	std::stringstream ss;

	auto net_channel = g_csgo.m_engine()->GetNetChannelInfo();

	std::string incoming = g_ctx.m_local ? std::to_string((int)(net_channel->GetLatency(FLOW_INCOMING) * 1000)) : "0";
	std::string outgoing = g_ctx.m_local ? std::to_string((int)(net_channel->GetLatency(FLOW_OUTGOING) * 1000)) : "0";
	
	ss << "Supremacy | fps: " << fps << " | time: " << incoming.c_str() << "ms" << " | ping: " << outgoing.c_str() << "ms";
	int x, y, w, h;
	int textsize = render::get().text_width(fonts[rolexTEXT], ss.str().c_str());
	render::get().custom_box(screen_x - (textsize + 20), 4, textsize + 20, 18);
	render::get().text(fonts[rolexTEXT], screen_x - (textsize + 10), 9, Color(255, 255, 255, 200), HFONT_OFF_SCREEN, ss.str().c_str());
}

void otheresp::penetration_reticle() {
	auto local_player = static_cast<player_t *>(g_csgo.m_entitylist()->GetClientEntity(g_csgo.m_engine()->GetLocalPlayer()));

	if (!local_player->is_alive25())
		return;

	auto draw_xhair = [](float size, Color color) -> void {
		int x_1, x_2, y_1, y_2;

		int
			screen_w = render::get().viewport().right / 2,
			screen_h = render::get().viewport().bottom / 2;

		x_1 = screen_w - (size / 4);
		x_2 = screen_w + (size / 4) + 1;

		y_1 = screen_h - (size / 4);
		y_2 = screen_h + (size / 4) + 1;

		render::get().rect_filled(screen_w - 1, screen_h - 1, 3, 3, color);
	};

	if (!g_cfg.esp.penetration_reticle || !local_player->is_alive26())
		return;

	auto weapon = local_player->m_hActiveWeapon().Get();
	if (!weapon)
		return;

	if (weapon->is_non_aim())
		return;

	auto weapon_info = weapon->get_csweapon_info();
	if (!weapon_info)
		return;

	vec3_t view_angles;
	g_csgo.m_engine()->GetViewAngles(view_angles);

	vec3_t direction;
	math::angle_vectors(view_angles, direction);

	vec3_t
		start = local_player->get_eye_pos(),
		end = start + (direction * 8192.f);
	int
		screen_ww = render::get().viewport().right / 2,
		screen_hh = render::get().viewport().bottom / 2;
	Color color = Color(255, 0, 0);
	if (autowall::get().calculate_return_info(start, end, local_player).m_did_penetrate_wall) {
		color = Color(163, 225, 21);
	}		//render::get().text(fonts[INDICATORFONT], 5, screen_height - 450, culuryes, HFONT_OFF_SCREEN, "PING");
	//autowall::get().calculate_return_info(latency_based_eye_pos, left_eye_pos, g_ctx.m_local, e, 1).m_damage;
	draw_xhair(2, color);
}
int BreakLagCumpensation()
{
	Vector velocity = g_ctx.m_local->m_vecVelocity();
	velocity.z = 0;
	float speed = velocity.Length();
	if (speed > 0.f)
	{
		auto distance_per_tick = speed * g_csgo.m_globals()->m_interval_per_tick;
		int choked_ticks = std::ceilf(65.f / distance_per_tick);
		return std::min<int>(choked_ticks, 14);
	}
}
enum
{
	IND_desync_delta,
	IND_choke,
	IND_delta,
	IND_pinging,
	IND_LC
};

player_t* m_entity;
void otheresp::lby_indicator() {	//update make prettier c;
	bool LCbroken = false;
	auto local_player = static_cast<player_t *>(g_csgo.m_entitylist()->GetClientEntity(g_csgo.m_engine()->GetLocalPlayer()));
	Color culuryes(25, 225, 25);
	Color culurnop(225, 25, 25);
	Color culurclos(225, 100, 100);
	Color white(225, 225, 225);
	Color red(255, 0, 0);
	Color green(0, 185, 0);
	vec3_t view_angles;
	g_csgo.m_engine()->GetViewAngles(view_angles);

	vec3_t direction;
	math::angle_vectors(view_angles, direction);
	vec3_t
		start = local_player->get_eye_pos(),
		end = start + (direction * 8192.f);
	int screen_width, screen_height;
	g_csgo.m_engine()->GetScreenSize(screen_width, screen_height);

	float breaking_lby_fraction = fabs(math::normalize_yaw(g_ctx.m_globals.real_angles.y - local_player->m_flLowerBodyYawTarget())) / 180.f;
	color colour((1.f - breaking_lby_fraction) * 255.f, breaking_lby_fraction * 255.f, 0);
	Color clr(colour.RGBA[0], colour.RGBA[1], colour.RGBA[2]);

	Box box;

	Box n_box = {
		box.x + 1,
		box.y + box.h + 8,
		box.w - 1,
		2
	};

	//if (g_cfg.esp.indicatorsmulti[IND_desync_delta].enabled)
	//{
	//	//render::get().text(fonts[ESPNAME], 5, screen_height - 270, white, HFONT_OFF_SCREEN, "Desync: %.f", g_ctx.m_local->get_max_desync_delta());
	//	render::get().text(fonts[ESPNAME], 75, screen_height - 270, white, HFONT_OFF_SCREEN, ": %.f", g_ctx.m_local->get_max_desync_delta());
	//	render::get().text(fonts[ESPNAME], 5, screen_height - 260, white, HFONT_OFF_SCREEN, "Chocked Ticks: %.f", g_ctx.m_globals.fakelagamt); //g_ctx.m_globals.fakelagamt);
	//	render::get().rect(5, screen_height - 246, (14 * 3) * 3.0 + 3, 9, Color::Black);
	//	render::get().rect_filled(6, screen_height - 245, (g_ctx.m_globals.fakelagamt * 3) * 3.0, 7, g_cfg.menu.menu_theme[0]);
	//}

	//if (g_cfg.esp.indicatorsmulti[IND_delta].enabled)
	//{
	//	if (g_ctx.m_local->get_max_desync_delta() > 0)
	//	{
	//		render::get().text(fonts[ESPNAME], 5, screen_height - 270, green, HFONT_OFF_SCREEN, "Desync");
	//	}
	//	else
	//	{
	//		render::get().text(fonts[ESPNAME], 5, screen_height - 270, red, HFONT_OFF_SCREEN, "Desync");
	//	}
	//}

	//if (g_cfg.esp.indicatorsmulti[IND_LC].enabled)
	//{
	//	Vector velocity = g_ctx.m_local->m_vecVelocity();
	//	velocity.z = 0;
	//	float speed = velocity.Length();
	//	auto distance_per_tick = speed * g_csgo.m_globals()->m_interval_per_tick;
	//	int choked_ticks = std::ceilf(65.f / distance_per_tick);
	//	auto last_origin = local_player->abs_origin();

	//	Vector curpos = g_ctx.m_local->m_vecVelocity();
	//	static Vector originrecords[2];

	//	if (g_ctx.send_packet) {
	//		static bool flip = false;
	//		originrecords[flip ? 0 : 1] = curpos;
	//		flip = !flip;
	//	}

	//	if ((originrecords[0] - originrecords[1]).LengthSqr() > 4096)
	//	{
	//		LCbroken = true;
	//	}
	//	else if (local_player->abs_origin() != last_origin) {
	//		if (!local_player->m_fFlags() & FL_ONGROUND && local_player->abs_origin().LengthSqr() > 4096) {
	//			LCbroken = true;
	//			last_origin = local_player->abs_origin();
	//		}
	//	}
	//	else if (!local_player->m_fFlags() & FL_ONGROUND && local_player->m_vecVelocity().Length2D() > 125 && g_ctx.send_packet && BreakLagCumpensation()) {
	//		LCbroken = true;
	//	}
	//	else
	//	{
	//		LCbroken = false;
	//	}
	//	if (g_cfg.esp.indicatorsmulti[IND_LC].enabled)
	//	{
	//		if (LCbroken)
	//		{
	//			render::get().text(fonts[INDICATORFONT], 5, screen_height - 360, green, HFONT_OFF_SCREEN, "LC");
	//		}
	//		else
	//		{
	//			render::get().text(fonts[INDICATORFONT], 5, screen_height - 360, red, HFONT_OFF_SCREEN, "LC");
	//		}
	//	}
	//}

	/*if (g_cfg.esp.indicatorsmulti[IND_LBY].enabled)
	{
		render::get().text(fonts[INDICATORFONT2], 5, screen_height - 80, clr, HFONT_OFF_SCREEN, "LBY");;
	}

	if (g_ctx.m_globals.didp2cflick && g_cfg.esp.indicatorsmulti[IND_NEGATIVEFLICK].enabled)
	{
		render::get().text(fonts[INDICATORFONT], 5, screen_height - 420, clr, HFONT_OFF_SCREEN, "NEGATIVE FLICK");
	}*/

	//if (g_ctx.m_local->is_alive27())
	//{
	//	Vector velocity = g_ctx.m_local->m_vecVelocity();
	//	velocity.z = 0;
	//	float speed = velocity.Length();
	//	auto distance_per_tick = speed * g_csgo.m_globals()->m_interval_per_tick;
	//	int choked_ticks = std::ceilf(65.f / distance_per_tick);
	//	auto last_origin = local_player->abs_origin();

	//	Vector curpos = g_ctx.m_local->m_vecVelocity();
	//	static Vector originrecords[2];

	//	if (g_ctx.send_packet) {
	//		static bool flip = false;
	//		originrecords[flip ? 0 : 1] = curpos;
	//		flip = !flip;
	//	}

	//	if ((originrecords[0] - originrecords[1]).LengthSqr() > 4096)
	//	{
	//		LCbroken = true;
	//	}
	//	else if (local_player->abs_origin() != last_origin) {
	//		if (!local_player->m_fFlags() & FL_ONGROUND && local_player->abs_origin().LengthSqr() > 4096) {
	//			LCbroken = true;
	//			last_origin = local_player->abs_origin();
	//		}
	//	}
	//	else if (!local_player->m_fFlags() & FL_ONGROUND && local_player->m_vecVelocity().Length2D() > 125 && g_ctx.send_packet && BreakLagCumpensation()) {
	//		LCbroken = true;
	//	}
	//	else
	//	{
	//		LCbroken = false;
	//	}
	//	if (g_cfg.esp.indicatorsmulti[IND_LC].enabled)
	//	{
	//		if (LCbroken)
	//		{
	//			render::get().text(fonts[INDICATORFONT], 5, screen_height - 360, culuryes, HFONT_OFF_SCREEN, "LC");
	//		}
	//		else
	//		{
	//			render::get().text(fonts[INDICATORFONT], 5, screen_height - 360, culurnop, HFONT_OFF_SCREEN, "LC");
	//		}
	//	}
	//	if (g_cfg.esp.indicatorsmulti[IND_PING].enabled)
	//	{
	//		auto net_channel = g_csgo.m_engine()->GetNetChannelInfo();
	//		int ping = net_channel->GetLatency(FLOW_OUTGOING) * 1000;
	//		if (ping > 150)
	//			render::get().text(fonts[INDICATORFONT], 5, screen_height - 330, culurnop, HFONT_OFF_SCREEN, "PING");
	//		else if (ping > 100 && ping < 150)
	//		{
	//			render::get().text(fonts[INDICATORFONT], 5, screen_height - 330, culurclos, HFONT_OFF_SCREEN, "PING");
	//		}
	//		else
	//			render::get().text(fonts[INDICATORFONT], 5, screen_height - 330, culuryes, HFONT_OFF_SCREEN, "PING");
	//	}

	//	if (g_cfg.esp.indicatorsmulti[IND_AAMODE].enabled) {
	//		if (g_ctx.m_local->is_alive28()) {
	//			if (!g_ctx.send_packet) {
	//				//auto Angles = g_ctx.get_command()->m_viewangles.y;
	//				auto Angles = 180;
	//				if (Angles < 135 && Angles > 45)//left
	//				{
	//					render::get().text(fonts[INDICATORFONT], 5, screen_height - 300, culurclos, HFONT_OFF_SCREEN, "Real: Left");
	//				}
	//				if (Angles > -135 && Angles < -45)//right
	//				{
	//					render::get().text(fonts[INDICATORFONT], 5, screen_height - 300, culurclos, HFONT_OFF_SCREEN, "Real: Right");
	//				}
	//				if (Angles < -135 && Angles > -180 || Angles > 135 && Angles < 180)//back
	//				{
	//					render::get().text(fonts[INDICATORFONT], 5, screen_height - 300, culurclos, HFONT_OFF_SCREEN, "Real: Backwards");
	//				}
	//				if (Angles > 0 && Angles < 45 || Angles < 0 && Angles > -45)//forward
	//				{
	//					render::get().text(fonts[INDICATORFONT], 5, screen_height - 300, culurclos, HFONT_OFF_SCREEN, "Real: Forwards");
	//				}
	//			}
	//		}
	//	}

	//	if (g_cfg.esp.indicatorsmulti[IND_HEAD].enabled) {
	//		if (g_ctx.m_globals.headsave)
	//		{
	//			render::get().text(fonts[INDICATORFONT], 5, screen_height - 390, culuryes, HFONT_OFF_SCREEN, "Head");
	//		}
	//		else if (g_ctx.m_globals.headsavenotshure)
	//		{
	//			render::get().text(fonts[INDICATORFONT], 5, screen_height - 390, culurclos, HFONT_OFF_SCREEN, "Head");
	//		}
	//		else
	//		{
	//			render::get().text(fonts[INDICATORFONT], 5, screen_height - 390, culurnop, HFONT_OFF_SCREEN, "Head");
	//		}
	//	}
	//	if (g_cfg.esp.indicatorsmulti[IND_localinfo].enabled) {
	//		render::get().text(fonts[ESPNAME], 5, screen_height - 270, white, HFONT_OFF_SCREEN, "Desync Delta: %.f", g_ctx.m_local->get_max_desync_delta());
	//		render::get().text(fonts[ESPNAME], 5, screen_height - 260, white, HFONT_OFF_SCREEN, "Chocked Ticks: %.f", g_ctx.m_globals.fakelagamt); //g_ctx.m_globals.fakelagamt);
	//		render::get().rect(5, screen_height - 246, (14 * 3) * 2.5 + 3, 9, Color::Black);
	//		render::get().rect_filled(6, screen_height - 245, (g_ctx.m_globals.fakelagamt * 3) * 2.5, 7, g_cfg.menu.menu_theme[0]);
	//	}
	//}
}

player_t* get_player(int userid)
{
	int i = g_csgo.m_engine()->GetPlayerForUserID(userid);
	return static_cast<player_t *>(g_csgo.m_entitylist()->GetClientEntity(i));
}

void otheresp::hitmarker_event(IGameEvent * event)
{
	auto local_player = static_cast<player_t *>(g_csgo.m_entitylist()->GetClientEntity(g_csgo.m_engine()->GetLocalPlayer()));

	if (!strcmp(event->GetName(), "player_hurt"))
	{
		auto attacker = event->GetInt("attacker");
		auto victim = get_player(event->GetInt("userid"));

		if (g_csgo.m_engine()->GetPlayerForUserID(attacker) == g_csgo.m_engine()->GetLocalPlayer())
		{
			hurt_time = g_csgo.m_globals()->m_curtime;
			impact_info best_impact;
			float best_impact_distance = -1;
			float time = g_csgo.m_globals()->m_curtime;

			for (int i = 0; i < impacts.size(); i++) {
				auto iter = impacts[i];
				if (time > iter.time + 1.f) {
					impacts.erase(impacts.begin() + i);
					continue;
				}
				vec3_t position = vec3_t(iter.x, iter.y, iter.z);
				vec3_t enemy_pos = victim->m_vecOrigin();
				float distance = position.DistTo(enemy_pos);
				if (distance < best_impact_distance || best_impact_distance == -1) {
					best_impact_distance = distance;
					best_impact = iter;
				}
			}

			if (best_impact_distance == -1)
				return;

			hitmarker_info info;
			info.impact = best_impact;
			info.alpha = 255;
			hitmarkers.push_back(info);
		}
	}

	if (strstr(event->GetName(), "bullet_impact"))
	{
		auto entity = get_player(event->GetInt("userid"));
		if (!entity || entity != local_player)
			return;

		impact_info info;
		info.x = event->GetFloat("x");
		info.y = event->GetFloat("y");
		info.z = event->GetFloat("z");

		info.time = g_csgo.m_globals()->m_curtime;

		impacts.push_back(info);
	}
}
//
//void otheresp::hitmarker_paint()
//{
//	auto linesize = 8;
//	int sizeX, centerX;
//	int sizeY, centerY;
//
//	if (!g_ctx.m_local->is_alive29())
//		hurt_time = 0.f;
//	if (!g_csgo.m_engine()->IsInGame() || !g_csgo.m_engine()->IsConnected())
//		hurt_time = 0.f;
//
//	float time_negga = 4.f;
//	if (time_negga + 0.65f >= g_csgo.m_globals()->m_curtime)
//	{
//		g_csgo.m_engine()->GetScreenSize(sizeX, sizeY);
//		linesize -= static_cast<int>(static_cast<float>(((hurt_time + 0.65f) - g_csgo.m_globals()->m_curtime) / 1.2) * 45);
//
//		centerX = sizeX / 2;
//		centerY = sizeY / 2;
//
//		util::clamp_value(linesize, 0, 8);
//
//		g_csgo.m_surface()->DrawSetColor(255, 255, 255, 255);
//		g_csgo.m_surface()->DrawLine(centerX - linesize, centerY - linesize, centerX - (linesize / 4), centerY - (linesize / 4));
//		g_csgo.m_surface()->DrawLine(centerX - linesize, centerY + linesize, centerX - (linesize / 4), centerY + (linesize / 4));
//		g_csgo.m_surface()->DrawLine(centerX + linesize, centerY + linesize, centerX + (linesize / 4), centerY + (linesize / 4));
//		g_csgo.m_surface()->DrawLine(centerX + linesize, centerY - linesize, centerX + (linesize / 4), centerY - (linesize / 4));
//		if (g_cfg.esp.onetap_hit)
//		{
//
//			std::stringstream ss;
//			ss << "HITTING...";
//			int x, y, w, h;
//			int textsize = render::get().text_width(fonts[ZeusOk], ss.str().c_str());
//			int screen_width, screen_height;
//			g_csgo.m_engine()->GetScreenSize(screen_width, screen_height);
//			render::get().text(fonts[ZeusOk], 900, screen_height - 500, Color(39, 200, 50), HFONT_OFF_SCREEN, ss.str().c_str());
//		}
//	}
//}
//
//void otheresp::hitmarkerdynamic_paint()
//{
//	float time = g_csgo.m_globals()->m_curtime;
//
//	for (int i = 0; i < hitmarkers.size(); i++)
//	{
//		bool expired = time >= hitmarkers.at(i).impact.time + 1.f;
//
//		if (expired)
//			hitmarkers.at(i).alpha -= 1;
//
//		if (expired && hitmarkers.at(i).alpha <= 0)
//		{
//			hitmarkers.erase(hitmarkers.begin() + i);
//			continue;
//		}
//
//		Vector pos3D = Vector(hitmarkers.at(i).impact.x, hitmarkers.at(i).impact.y, hitmarkers.at(i).impact.z), pos2D;
//		if (!math::world_to_screen(pos3D, pos2D))
//			continue;
//
//		auto linesize = 8;
//
//		g_csgo.m_surface()->DrawSetColor(255, 255, 255, hitmarkers.at(i).alpha);
//		g_csgo.m_surface()->DrawLine(pos2D.x - linesize, pos2D.y - linesize, pos2D.x - (linesize / 4), pos2D.y - (linesize / 4));
//		g_csgo.m_surface()->DrawLine(pos2D.x - linesize, pos2D.y + linesize, pos2D.x - (linesize / 4), pos2D.y + (linesize / 4));
//		g_csgo.m_surface()->DrawLine(pos2D.x + linesize, pos2D.y + linesize, pos2D.x + (linesize / 4), pos2D.y + (linesize / 4));
//		g_csgo.m_surface()->DrawLine(pos2D.x + linesize, pos2D.y - linesize, pos2D.x + (linesize / 4), pos2D.y - (linesize / 4));
//		if (g_cfg.esp.onetap_hit)
//		{
//			std::stringstream ss;
//			ss << "HITTING...";
//			int x, y, w, h;
//			int textsize = render::get().text_width(fonts[ZeusOk], ss.str().c_str());
//			int screen_width, screen_height;
//			g_csgo.m_engine()->GetScreenSize(screen_width, screen_height);
//			render::get().text(fonts[ZeusOk], 900, screen_height - 500, Color(39, 200, 50), HFONT_OFF_SCREEN, ss.str().c_str());
//		}
//	}
//}


void otheresp::spread_crosshair(IDirect3DDevice9* m_device) {
	if (!g_cfg.esp.show_spread)
		return;

	if (!g_ctx.m_local)
		return;

	if (!g_ctx.m_local->is_alive30())
		return;

	if (!g_csgo.m_engine()->IsConnected() && !g_csgo.m_engine()->IsInGame())
		return;

	auto weapon = g_ctx.m_local->m_hActiveWeapon().Get();

	if (!weapon)
		return;

	static float rot = 0.f;

	int w, h;

	g_csgo.m_engine()->GetScreenSize(w, h);

	w /= 2, h /= 2;

	int r, g, b;
	r = g_cfg.esp.show_spread_color.r();
	g = g_cfg.esp.show_spread_color.g();
	b = g_cfg.esp.show_spread_color.b();

	weapon->update_accuracy_penality();

	switch (g_cfg.esp.show_spread_type) {
	case 0:
		render::get().circle_dual_colour(w, h, weapon->get_innacuracy() * 500.0f, 0, 1, 50, D3DCOLOR_RGBA(r, g, b, 100), D3DCOLOR_RGBA(0, 0, 0, 0), m_device);
		break;
	case 1:
		render::get().circle_dual_colour(w, h, weapon->get_innacuracy() * 500.0f, rot, 1, 50, m_device);
		break;
	}

	rot += 0.5f;
	if (rot > 360.f)
		rot = 0.f;
}

void otheresp::spread_crosshair() {
	if (g_cfg.esp.show_spread_type != 2)
		return;

	if (!g_ctx.m_local)
		return;

	if (!g_ctx.m_local->is_alive31())
		return;

	if (!g_csgo.m_engine()->IsConnected() && !g_csgo.m_engine()->IsInGame())
		return;

	auto weapon = g_ctx.m_local->m_hActiveWeapon().Get();

	if (!weapon)
		return;

	if (weapon) {
		int screen_w, screen_h;
		g_csgo.m_engine()->GetScreenSize(screen_w, screen_h);
		int cross_x = screen_w / 2, cross_y = screen_h / 2;

		float recoil_step = screen_h / 180;

		cross_x -= (int)(g_ctx.m_local->m_aimPunchAngle().y * recoil_step);
		cross_y += (int)(g_ctx.m_local->m_aimPunchAngle().x * recoil_step);

		weapon->update_accuracy_penality();
		float inaccuracy = weapon->get_innacuracy();
		float spread = weapon->get_spread();

		float cone = inaccuracy * spread;
		cone *= screen_h * 0.7f;
		cone *= 90.f / 180;

		for (int seed{ }; seed < 512; ++seed) {
			math::random_seed(math::random_float(0, 512));

			float	rand_a = math::random_float(0.f, 1.f);
			float	pi_rand_a = math::random_float(0.f, 2.f * DirectX::XM_PI);
			float	rand_b = math::random_float(0.0f, 1.f);
			float	pi_rand_b = math::random_float(0.f, 2.f * DirectX::XM_PI);

			float spread_x = cos(pi_rand_a) * (rand_a * inaccuracy) + cos(pi_rand_b) * (rand_b * spread);
			float spread_y = sin(pi_rand_a) * (rand_a * inaccuracy) + sin(pi_rand_b) * (rand_b * spread);

			float max_x = cos(pi_rand_a) * cone + cos(pi_rand_b) * cone;
			float max_y = sin(pi_rand_a) * cone + sin(pi_rand_b) * cone;

			float step = screen_h / 180 * 90.f;
			int screen_spread_x = (int)(spread_x * step * 0.7f);
			int screen_spread_y = (int)(spread_y * step * 0.7f);

			float percentage = (rand_a * inaccuracy + rand_b * spread) / (inaccuracy + spread);

			render::get().rect(cross_x + screen_spread_x, cross_y + screen_spread_y, 1, 1,
				g_cfg.esp.show_spread_color);
		}
	}
}