#include "sound_esp.h"

/* declarations */
soundesp::player_hurt_event player_hurt_listener;
soundesp::player_footstep_event player_footstep_listener;

/* sound esp */
soundesp g_soundesp;

/* sound logs */
std::vector<sound_info> sound_logs;

/* sound esp draw */
void soundesp::draw_circle(CColor color, Vector position)
{
	SDK::BeamInfo_t beam_info;
	beam_info.m_nType = SDK::TE_BEAMRINGPOINT;
	beam_info.m_pszModelName = "sprites/purplelaser1.vmt";
	beam_info.m_nModelIndex = INTERFACES::ModelInfo->GetModelIndex("sprites/purplelaser1.vmt");
	//beam_info.m_pszHaloName   = "sprites/purplelaser1.vmt";
	beam_info.m_nHaloIndex = -1;
	beam_info.m_flHaloScale = 5;
	beam_info.m_flLife = .50f;
	beam_info.m_flWidth = 10.f;
	beam_info.m_flFadeLength = 1.0f;
	beam_info.m_flAmplitude = 0.f;
	beam_info.m_flRed = color.r();
	beam_info.m_flGreen = color.g();
	beam_info.m_flBlue = color.b();
	beam_info.m_flBrightness = color.a();
	beam_info.m_flSpeed = 0.f;
	beam_info.m_nStartFrame = 0.f;
	beam_info.m_flFrameRate = 60.f;
	beam_info.m_nSegments = -1;
	//beam_info.m_bRenderable   = true;
	beam_info.m_nFlags = SDK::FBEAM_FADEOUT;
	beam_info.m_vecCenter = position + Vector(0, 0, 5);
	beam_info.m_flStartRadius = 20.f;
	beam_info.m_flEndRadius = 640.f;

	auto beam = INTERFACES::ViewRenderBeams->CreateBeamRingPoint(beam_info);

	if (beam)
		INTERFACES::ViewRenderBeams->DrawBeam(beam);
}

/* listeners */
void soundesp::listener()
{
	INTERFACES::GameEventManager->AddListener(&player_footstep_listener, "player_footstep", false);

	INTERFACES::GameEventManager->AddListener(&player_hurt_listener, "player_hurt", false);
}

/* footstep */
void soundesp::player_footstep_event::FireGameEvent(SDK::IGameEvent * game_event)
{
	/* check if we are in game */
	if (!INTERFACES::Engine->IsInGame() || !INTERFACES::Engine->IsConnected())
		return;

	/* check if we have the sound esp enabled */
	if (!SETTINGS::settings.vis_sound)
		return;

	/* return if not event */
	if (!game_event)
		return;

	/* local player */
	auto localplayer = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

	/* footstep */
	if (strstr(game_event->GetName(), "player_footstep"))
	{
		/* get the walker */
		auto walker = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetPlayerForUserID(game_event->GetInt("userid")));

		/* return if walker is nullptr */
		if (walker == nullptr)
			return;

		/* return if walker is dormant */
		if (walker->GetIsDormant())
			return;

		/* clock */
		static int timer;

		timer += 1;

		if (timer > 1)
			timer = 0;

		if (walker->GetTeam() != localplayer->GetTeam())
		{
			if (walker && timer < 1)
			{
				sound_logs.push_back(sound_info(walker->GetVecOrigin(), INTERFACES::Globals->curtime, game_event->GetInt("userid")));
			}
		}
	}
}

/* player hurt */
void soundesp::player_hurt_event::FireGameEvent(SDK::IGameEvent * game_event)
{
	/* if we are connected */
	if (!INTERFACES::Engine->IsConnected() || !INTERFACES::Engine->IsInGame())
		return;

	/* check if we have the sound esp enabled */
	if (!SETTINGS::settings.vis_sound)
		return;

	/* return if not event */
	if (!game_event)
		return;

	/* local player */
	auto localplayer = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

	/* get the attacker */
	auto attacker = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetPlayerForUserID(game_event->GetInt("attacker")));

	/* get the victim */
	auto victim = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetPlayerForUserID(game_event->GetInt("userid")));


	/* clock */
	static int timer;

	timer += 1;

	if (timer > 2)
		timer = 0;

	/* check if we are attacker */
	if (attacker == localplayer)
	{
		if (timer < 1)
			sound_logs.push_back(sound_info(victim->GetVecOrigin(), INTERFACES::Globals->curtime, game_event->GetInt("userid")));
	}
}

/* draw */
void soundesp::draw()
{
	/* if footstep esp is enabled */
	if (!SETTINGS::settings.vis_sound)
		return;

	/* if we are connected */
	if (!INTERFACES::Engine->IsConnected() || !INTERFACES::Engine->IsInGame())
		return;

	for (unsigned int i = 0; i < sound_logs.size(); i++)
	{
		/* draw the beam */
		g_soundesp.draw_circle(SETTINGS::settings.vis_soundcolor, sound_logs[i].position);

		sound_logs.erase(sound_logs.begin() + i);
	}
}