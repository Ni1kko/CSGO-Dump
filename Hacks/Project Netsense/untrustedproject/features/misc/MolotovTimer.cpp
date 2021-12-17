#include "MolotovTimer.hpp"
#include "../../helpers/utils.hpp"
#include "../../helpers/math.hpp"
#include "../../render.hpp"
#include "misc.hpp"

MolotovTimer g_MolotovTimer;

void MolotovTimer::Draw() {
	if (!g_EngineClient->IsInGame() || !g_EngineClient->IsConnected())
		return;

	for (auto i = 0; i < g_EntityList->GetHighestEntityIndex(); i++)
	{
		auto ent = C_BaseEntity::GetEntityByIndex(i);
		if (!ent)
			continue;

		if (ent->GetClientClass()->m_ClassID != ClassId_CInferno)
			continue;

		auto inferno = reinterpret_cast<Inferno_t*>(ent);

		auto origin = inferno->m_vecOrigin();
		auto screen_origin = Vector();

		if (!Math::WorldToScreen(origin, screen_origin))
			return;

		const auto spawn_time = inferno->GetSpawnTime();
		const auto timer = (spawn_time + Inferno_t::GetExpireTime()) - g_GlobalVars->curtime;
		const auto factor = timer / Inferno_t::GetExpireTime();
		const auto l_spawn_time = *(float*)(uintptr_t(inferno) + 0x20);
		const auto l_factor = ((l_spawn_time + 7.03125f) - g_GlobalVars->curtime) / 7.03125f;




		Render::Get().RenderCircle3DFilled(origin, 200.f, g_Misc->colors.molotov_timer_color);

		Render::Get().RenderCircleFilled(screen_origin.x, screen_origin.y - 20, 32, (screen_origin.y + 10) + 4.f, Color::Black);

		Render::Get().RenderText(std::to_string((int)(round(timer + 1))) + "S.", screen_origin.x - 10, screen_origin.y - 20.f, 20.f, Color::White);

		if (timer <= 3)
			Render::Get().Arc(screen_origin.x, screen_origin.y - 20, 30, 32, -90, 360 * l_factor, Color::Green);
		
		else
			Render::Get().Arc(screen_origin.x, screen_origin.y - 20, 30, 32, -90, 360 * l_factor, Color::Red);

		

		//Render::Get().RenderCircle3D(origin, 50, 200.f, g_Misc->colors.molotov_timer_color);

	}
}

