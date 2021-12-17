#include "extrapolation.h"


template<class T, class U>
T fine(T in, U low, U high)
{
	if (in <= low)
		return low;

	if (in >= high)
		return high;

	return in;
}
void LinearExtrapolations::run()
{
	if (SETTINGS::settings.extrapolationbool) // [censored] extrapolation
	{
		auto m_local = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());
		
		if (m_local && m_local->IsAlive()) {
			for (int i = 1; i < INTERFACES::Globals->maxclients; i++)
			{
				auto m_entity = INTERFACES::ClientEntityList->GetClientEntity(i);
				if (!m_entity || m_entity == m_local || !m_entity->IsAlive()) continue;



				float simtime_delta = m_entity->GetSimTime() - m_entity->m_flOldSimulationTime();
				int choked_ticks = fine(TIME_TO_TICKS(simtime_delta), 1, 15);
				Vector lastOrig;

				if (lastOrig.Length() != m_entity->GetVecOrigin().Length())
					lastOrig = m_entity->GetVecOrigin();

				float delta_distance = (m_entity->GetVecOrigin() - lastOrig).LengthSqr();
				if (delta_distance > 4096.f)
				{
					Vector velocity_per_tick = m_entity->GetVelocity() * INTERFACES::Globals->interval_per_tick;
					auto new_origin = m_entity->GetVecOrigin() + (velocity_per_tick * choked_ticks);
					m_entity->SetAbsOrigin(new_origin);
				}
			}
		}
	}
}





















































