#include "Resolver.h"

#define TIME_TO_TICKS( dt )		( (int)( 0.5 + (float)(dt) / Interfaces::Globals->interval_per_tick ) )
#define TICKS_TO_TIME( t )		( Interfaces::Globals->interval_per_tick *( t ) )
inline float clamp_yaw(float yaw)
{
	while (yaw > 180.f)
		yaw -= 360.f;
	while (yaw < -180.f)
		yaw += 360.f;
	return yaw;
}
void c_resolver::run() {
	if (!c_config::get()->b["rage_resolver"])
		return;

	for (int i = 0; i < 64; i++) {
		auto e = Interfaces::EntityList->GetClientEntity(i);
		if (e) {
			if (e->IsAlive() && e->GetHealth() > 0 && !e->IsDormant()) {
				if (e->GetVelocity().Length() > 110.f && e->GetFlags() & FL_ONGROUND) {
					e->GetEyeAnglesPtr()->y = e->GetLowerBodyYaw();
				}
				else if (e->GetFlags() & FL_ONGROUND) {

					if (G::MissedShots1[i] == 0) {
						if (this->ajusting_balance(e)) {
							stored_yaws[i] = e->GetLowerBodyYaw();
							did_store_yaw[i] = true;
						}

						if (did_store_yaw[i])
							e->GetEyeAnglesPtr()->y = stored_yaws[i];
					}
					else if (G::MissedShots1[i] == 2)
						e->GetEyeAnglesPtr()->y = stored_yaws[i];
					else if (G::MissedShots1[i] > 2) {
						auto f = stored_yaws[i];
						if (f > 0)
							e->GetEyeAnglesPtr()->y = f + (G::ResolverAdd[i] / 2); // òóò 
						else if (f < 0) {
							e->GetEyeAnglesPtr()->y = f - (G::ResolverAdd[i] / 2); // è òóò 
						}
						else {
							switch (G::MissedShots1[i] % 2) {
							case 0:
								e->GetEyeAnglesPtr()->y = f - (G::ResolverAdd[i] / 2);
								break;
							case 1:
								e->GetEyeAnglesPtr()->y = f + (G::ResolverAdd[i] / 2);
								break;
							}
						}
					}

				}
			}
			else {
				G::MissedShots1[i] = 0;
				G::ResolverAdd[i] = 0;

				did_store_yaw[i] = false;
			}
		}
		else {
			G::MissedShots1[i] = 0;
			G::ResolverAdd[i] = 0;

			did_store_yaw[i] = false;
		}
	}
}

void c_resolver::AnimationFix(CBaseEntity* pEnt)
{
	CBaseAnimState* state = pEnt->GetAnimState();
	if (state)
	{
		// backup
		const float curtime = Interfaces::Globals->curtime;
		const float frametime = Interfaces::Globals->frametime;

		Interfaces::Globals->frametime = Interfaces::Globals->interval_per_tick;
		Interfaces::Globals->curtime = pEnt->GetSimTime();

		int backup_eflags = pEnt->eflags();

		// SetLocalVelocity
		pEnt->eflags() &= ~0x1000; // InvalidatePhysicsRecursive(VELOCITY_CHANGED); EFL_DIRTY_ABSVELOCITY = 0x1000
		pEnt->abs_velocity() = pEnt->GetVelocity();

		// invalidates prior animations
		if (state->m_iLastClientSideAnimationUpdateFramecount == Interfaces::Globals->framecount)
			state->m_iLastClientSideAnimationUpdateFramecount = Interfaces::Globals->framecount - 1;

		*pEnt->clientsideanim() = true;

		// updates local animations + poses + calculates new abs angle based on eyeangles and other stuff
		pEnt->UpdateClientSideAnimation();

		*pEnt->clientsideanim() = false;

		// restore
		pEnt->eflags() = backup_eflags;

		Interfaces::Globals->curtime = curtime;
		Interfaces::Globals->frametime = frametime;

		pEnt->InvalidateBoneCache();
		pEnt->SetupBones(nullptr, -1, 0x7FF00, Interfaces::Globals->curtime);
	}

}

void c_resolver::Resolver(CBaseEntity* player)
{
	for (int i = 1; i <= Interfaces::Globals->maxclients; i++)
	{
		auto animstate = player->GetAnimState();
		if (player->IsAlive() && player->GetHealth() > 0 && !player->IsDormant()) {

			auto speedfraction = 0.0;
			if (animstate->m_flFeetSpeedForwardsOrSideWays < 0.0)
				speedfraction = 0.0;
			else
				speedfraction = fminf(animstate->m_flFeetSpeedForwardsOrSideWays, 1.0f);

			float v2 = (animstate->m_flStopToFullRunningFraction * -0.30000001 - 0.19999999) * speedfraction;
			float v18 = v2;
			float flYawModifier = v2 + 1.0f;
			float v29 = 0.0;
			if (animstate->m_fDuckAmount > 0.0)
			{
				v29 = 0.0;
				if (animstate->m_flFeetSpeedUnknownForwardOrSideways < 0.0)
					v29 = 0.0;
				else
					v29 = fminf(animstate->m_flFeetSpeedUnknownForwardOrSideways, 1.0f);
			}

			if (player) {
				float goal_feet_yaw = animstate->m_flGoalFeetYaw;

				CAnimationLayer layers[13];
				CAnimationLayer backup_layers[13];
				CAnimationLayer center_layers[13];
				CAnimationLayer left_layers[13];
				CAnimationLayer right_layers[13];

				int side = animstate->m_flGoalFeetYaw ? -1 : 1;

				if (!(layers[12].m_flWeight * 1000.f) && (layers[6].m_flWeight * 1000.f) == (layers[6].m_flWeight * 1000.f))
				{
					float delta_first = abs(layers[6].m_flPlaybackRate - center_layers[6].m_flPlaybackRate);
					float delta_second = abs(layers[6].m_flPlaybackRate - left_layers[6].m_flPlaybackRate);
					float delta_third = abs(layers[6].m_flPlaybackRate - right_layers[6].m_flPlaybackRate);

					if (delta_first < delta_second || delta_third <= delta_second || (delta_second * 1000.f))
					{
						if (delta_first >= delta_third && delta_second > delta_third && !(signed int)(float)(delta_third * 1000.0))
							side = 1;
					}
					else
						side = -1;
				}

				switch (G::MissedShots1[i] % 4)
				{
				case 0:
					side = +player->GetMaxDesyncDelta();
					break;
				case 1:
					side = -player->GetMaxDesyncDelta();
					break;
				case 2:
					side = +player->GetMaxDesyncDelta() / 2;
					break;
				case 3:
					side += player->GetMaxDesyncDelta() / 2;
					break;
				}

				animstate->m_flGoalFeetYaw = clamp_yaw(side);
			}
		}
	}
}


bool c_resolver::ajusting_balance(CBaseEntity* e) {

	for (int i = 0; i < e->GetNumAnimOverlays(); i++)
	{
		const int activity = e->GetSequenceActivity(e->GetAnimOverlay(i).m_nSequence);
		if (activity == 979)
			return true;
	}
	return false;
}