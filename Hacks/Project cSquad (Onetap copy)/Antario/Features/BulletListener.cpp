#include "BulletListener.h"

std::vector<impact_info> impacts;
std::vector<hitmarker_info> hitmarkers;

void bullettracers::draw_beam(Vector src, Vector end, Color color)
{
	BeamInfo_t info;
	info.m_nType = TE_BEAMPOINTS;
	info.m_pszModelName = "sprites/purplelaser1.vmt";
	//info.m_pszHaloName = -1;
	info.m_nHaloIndex = -1;
	info.m_flHaloScale = 0.f;
	info.m_flLife = 2;
	info.m_flWidth = 3;
	info.m_flEndWidth = 3;
	info.m_flFadeLength = 0;
	info.m_flAmplitude = 0.f;
	info.m_flBrightness = color.alpha;
	info.m_flSpeed = 1.f;
	info.m_nStartFrame = 0;
	info.m_flFrameRate = 0;
	info.m_flRed = color.red;
	info.m_flGreen = color.green;
	info.m_flBlue = color.blue;
	info.m_nSegments = -1;
	info.m_bRenderable = true;
	info.m_nFlags = FBEAM_ONLYNOISEONCE | FBEAM_NOTILE;
	info.m_vecStart = src; // Vector(0, 0, 5);
	info.m_vecEnd = end;

	Beam_t* beam = g_pBeam->CreateBeamPoints(info);

	if (beam)
		g_pBeam->DrawBeam(beam);
}

void bullettracers::events(IGameEvent* event)
{
	if (!strcmp(event->GetName(), "bullet_impact"))
	{
		auto index = g_pEngine->GetPlayerForUserID(event->GetInt("userid"));

		auto shooter = g_pEntityList->GetClientEntity(g_pEngine->GetPlayerForUserID(event->GetInt("userid")));
		if (!shooter)
			return;

		Vector position(event->GetFloat("x"), event->GetFloat("y"), event->GetFloat("z"));

		if (position == Vector(0, 0, 0))
			return;

		if (index == g_pEngine->GetLocalPlayer())
		{
			if (g_Settings.bBulletImpacts)
			{
				logs.push_back(trace_info(g::pLocalEntity->GetEyeOrigin(), position, g_pGlobalVars->realtime));
				//draw_beam(shooter->GetEyeOrigin(), position, Color(g_Settings.impact, g_Settings.impact[3] * 255));
				g_pDebugOverlay->AddBoxOverlay(position, Vector(-2, -2, -2), Vector(2, 2, 2), QAngle(0, 0, 0), g_Settings.impact[0] * 255, g_Settings.impact[1] * 255, g_Settings.impact[2] * 255, g_Settings.impact[3] * 255, 1.f);
			}
		}
		else if (index != g_pEngine->GetLocalPlayer() && shooter->GetTeam() != g::pLocalEntity->GetTeam())
		{
			//g_pDebugOverlay->AddTextOverlayRGB(position, 0, 4, 255, 255, 255, 255, "6ix9ine is gone");
			if (g_Settings.bBulletTracers)
			{
				logs.push_back(trace_info(shooter->GetEyeOrigin(), position, g_pGlobalVars->realtime));
				draw_beam(shooter->GetEyeOrigin(), position, Color(g_Settings.bullettracer, g_Settings.bullettracer[3] * 255));
			}
		}
	}
}



C_BaseEntity* get_player(int userid) {
	int i = g_pEngine->GetPlayerForUserID(userid);
	return static_cast<C_BaseEntity*>(g_pEntityList->GetClientEntity(i));
}

void otheresp::hitmarker_event(IGameEvent * event)
{
	auto local_player = static_cast<C_BaseEntity*>(g_pEntityList->GetClientEntity(g_pEngine->GetLocalPlayer()));

	if (!strcmp(event->GetName(), "player_hurt")) {

		auto attacker = event->GetInt("attacker");
		auto victim = get_player(event->GetInt("userid"));

		if (g_pEngine->GetPlayerForUserID(attacker) == g_pEngine->GetLocalPlayer())

			Globals::flHurtTime = g_pGlobalVars->realtime;
		impact_info best_impact;
		float best_impact_distance = -1;
		float time = g_pGlobalVars->curtime;


		for (int i = 0; i < impacts.size(); i++) {
			auto iter = impacts[i];
			if (time > iter.time + 1.f) {
				impacts.erase(impacts.begin() + i);
				continue;
			}
			Vector position = Vector(iter.x, iter.y, iter.z);
			Vector enemy_pos = victim->GetVecOrigin();
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

	if (strstr(event->GetName(), "bullet_impact"))
	{
		auto entity = get_player(event->GetInt("userid"));
		if (!entity || entity != local_player)
			return;

		impact_info info;
		info.x = event->GetFloat("x");
		info.y = event->GetFloat("y");
		info.z = event->GetFloat("z");

		info.time = g_pGlobalVars->curtime;

		impacts.push_back(info);
	}
}

void otheresp::hitmarkerdynamic_paint() {

	float time = g_pGlobalVars->curtime;

	for (int i = 0; i < hitmarkers.size(); i++) {
		bool expired = time >= hitmarkers.at(i).impact.time + 2.f;

		if (expired)
			hitmarkers.at(i).alpha -= 1;

		if (expired && hitmarkers.at(i).alpha <= 0) {
			hitmarkers.erase(hitmarkers.begin() + i);
			continue;
		}

		Vector pos3D = Vector(hitmarkers.at(i).impact.x, hitmarkers.at(i).impact.y, hitmarkers.at(i).impact.z), pos2D;
		if (!Utils::WorldToScreen(pos3D, pos2D))
			continue;

		auto linesize = 8;

		g_pSurface->DrawSetColor(255, 255, 255, hitmarkers.at(i).alpha);
		g_pSurface->DrawLine(pos2D.x - linesize, pos2D.y - linesize, pos2D.x - (linesize / 4), pos2D.y - (linesize / 4));
		g_pSurface->DrawLine(pos2D.x - linesize, pos2D.y + linesize, pos2D.x - (linesize / 4), pos2D.y + (linesize / 4));
		g_pSurface->DrawLine(pos2D.x + linesize, pos2D.y + linesize, pos2D.x + (linesize / 4), pos2D.y + (linesize / 4));
		g_pSurface->DrawLine(pos2D.x + linesize, pos2D.y - linesize, pos2D.x + (linesize / 4), pos2D.y - (linesize / 4));
	}
}