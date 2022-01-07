#include "lin_extp.h"

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

	auto m_local = hackManager.pLocal();
	if (m_local && m_local->IsAlive()) {
		for (int i = 1; i < interfaces::globals->maxClients; i++)
		{
			auto m_entity = interfaces::ent_list->get_client_entity(i);
			if (!m_entity || m_entity == m_local || m_entity->GetClientClass()->m_ClassID != (int)CSGOClassID::CCSPlayer || !m_entity->IsAlive())
				continue;

			float simtime_delta = m_entity->GetSimulationTime() - m_entity->m_flOldSimulationTime();
			int choked_ticks = fine(TIME_TO_TICKS(simtime_delta), 1, 15);
			Vector lastOrig;

			if (lastOrig.Length() != m_entity->GetOrigin().Length())
				lastOrig = m_entity->GetOrigin();

			float delta_distance = (m_entity->GetOrigin() - lastOrig).LengthSqr();
			if (delta_distance > 4096.f)
			{
				Vector velocity_per_tick = m_entity->GetVelocity() * interfaces::globals->interval_per_tick;
				auto new_origin = m_entity->GetOrigin() + (velocity_per_tick * choked_ticks);
				m_entity->SetAbsOriginal(new_origin);
			}
		}
	}

}

























































































































































































