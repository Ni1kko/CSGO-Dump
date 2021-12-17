#include "animation.h"
#include "../../SDK/CBaseEntity.h"
Animation* g_Animation = new Animation;
static int tickcount;
static float proper_abs;
CBaseAnimState* animstate = nullptr;
float_t m_flSpawnTime = 0.f;
CBaseHandle* m_ulEntHandle = nullptr;
float simtime = 0.f;
CAnimationLayer layers[13];
CAnimationLayer networked_layers[13];
static std::array<float, 24> pp;
#define TIME_TO_TICKS( dt )		( (int)( 0.5 + (float)(dt) / Interfaces::Globals->interval_per_tick ) )
/*void Animation::UpdatePlayerAnimations()
{
	if (G::LocalPlayer || G::LocalPlayer->IsAlive())
		return;

	static auto handle = const_cast<CBaseHandle*>(&G::LocalPlayer->GetRefEHandle());
	static auto spawn_time = G::LocalPlayer->m_flSpawnTime();

	bool allocate = (!G::LocalPlayer->GetAnimState()),
		change = (!allocate) && (const_cast<CBaseHandle*>(&G::LocalPlayer->GetRefEHandle()) != handle),
		reset = (!allocate && !change) && (G::LocalPlayer->m_flSpawnTime() != spawn_time);

	// player changed, free old animation state.
	if (change)
		Interfaces::MemAlloc->Free(G::LocalPlayer->GetAnimState());

	// need to reset? (on respawn)
	if (reset)
	{
		// reset animation state.
		CBaseEntity::ResetAnimationState(G::LocalPlayer->GetAnimState());

		// note new spawn time.
		spawn_time = G::LocalPlayer->m_flSpawnTime();
	}

	// need to allocate or create new due to player change.
	if (allocate || change)
	{
		CBaseAnimState *state = (CBaseAnimState*)Interfaces::MemAlloc->Alloc(sizeof(CBaseAnimState));

		if (state != nullptr)
			G::LocalPlayer->CreateAnimationState(state);

		// used to detect if we need to recreate / reset.
		handle = const_cast<CBaseHandle*>(&G::LocalPlayer->GetRefEHandle());
		spawn_time = G::LocalPlayer->m_flSpawnTime();

		// note anim state for future use.
		info.m_playerAnimState = state;
	}

	if (!info.m_playerAnimState)
		return;

	std::array<float_t, 24> backup_poses = player->m_flPoseParameter();

	CAnimationLayer backup_layers[15];
	std::memcpy(backup_layers, player->GetAnimOverlays(), (sizeof(CAnimationLayer) * player->GetNumAnimOverlays()));

	// fixing legs and few other things missing here
	CBaseEntity::UpdateAnimationState(info.m_playerAnimState, player->GetEyeAngles());

	info.m_flPoseParameters = player->m_flPoseParameter();
	std::memcpy(info.m_AnimationLayer, player->GetAnimOverlays(), (sizeof(CAnimationLayer) * player->GetNumAnimOverlays()));

	player->m_flPoseParameter() = backup_poses;
	std::memcpy(player->GetAnimOverlays(), backup_layers, (sizeof(CAnimationLayer) * player->GetNumAnimOverlays()));

}*/

void Animation::ManageLocalFake()
{
	static auto virtual_free = VirtualFree;

	if (!Interfaces::Engine->IsConnected() || !Interfaces::Engine->IsInGame() || !G::LocalPlayer) return;
		bool
		alloc = (animstate == nullptr),
//		change = (!alloc) && (&G::LocalPlayer->get_ref_ehandle() != m_ulEntHandle),
		reset = (!alloc) && (G::LocalPlayer->m_flSpawnTime() != m_flSpawnTime);

	/*if (change)
		virtual_free(animstate, sizeof(CBaseAnimState), MEM_RELEASE);*/

	if (reset) {
		G::LocalPlayer->ResetAnimationState(animstate);
		m_flSpawnTime = G::LocalPlayer->m_flSpawnTime();
	}

	if (alloc) {
		animstate = (CBaseAnimState*)Interfaces::MemAlloc->Alloc(sizeof(CBaseAnimState));
		if (animstate)
			G::LocalPlayer->CreateAnimationState(animstate);

		//m_ulEntHandle = (CBaseHandle*)(&G::LocalPlayer->get_ref_ehandle());
		m_flSpawnTime = G::LocalPlayer->m_flSpawnTime();
	}

	if (!alloc && !reset) {
		auto as = G::LocalPlayer->GetAnimState();

		auto pp123 = new float[24]{};
		auto layers123 = new CAnimationLayer[13]{};

		auto frametime = Interfaces::Globals->frametime;
		auto servertime = Interfaces::Globals->curtime;

		Interfaces::Globals->frametime = Interfaces::Globals->interval_per_tick;
		Interfaces::Globals->curtime = G::LocalPlayer->GetSimTime();

		auto vel = G::LocalPlayer->GetVelocity();

		memcpy(layers123, G::LocalPlayer->GetAnimOverlays(), sizeof(CAnimationLayer) * 13);

		if (G::LocalPlayer->GetOldSimTime() != G::LocalPlayer->GetSimTime()) {
			G::LocalPlayer->UpdateAnimationState(animstate, G::RealAngle);

			animstate->m_fDuckAmount = as->m_fDuckAmount;

			G::fake_obs = G::proper_obs;
			G::fake_obs -= animstate->m_flGoalFeetYaw;

			G::LocalPlayer->InvalidateBoneCache();
			G::LocalPlayer->SetupBones(G::FakeMatrix, 128, 0x7FF00, Interfaces::Globals->curtime);

			for (auto& i : G::FakeMatrix)
			{
				i[0][3] -= G::LocalPlayer->GetRenderOrigin().x;
				i[1][3] -= G::LocalPlayer->GetRenderOrigin().y;
				i[2][3] -= G::LocalPlayer->GetRenderOrigin().z;
			}
		}
		Interfaces::Globals->frametime = frametime;
		Interfaces::Globals->curtime = servertime;

		memcpy(G::LocalPlayer->GetAnimOverlays(), layers123, sizeof(CAnimationLayer) * 13);
	}
}
void Animation::update_local_ghetto(CBaseEntity* local)
{
	static auto pp = new float[24]{};
	static auto layers = new CAnimationLayer[13]{};
	static int tickcount = Interfaces::Globals->tickcount;


	if (!G::UserCmd)
		return;

	auto as = local->GetAnimState();

	if (tickcount != Interfaces::Globals->tickcount) {
		memcpy(layers, local->GetAnimOverlays(), sizeof(CAnimationLayer) * 13);

		local->clientsideanim();
		as->m_iLastClientSideAnimationUpdateFramecount = 0;

		local->UpdateAnimationState((CBaseAnimState*)as, G::UserCmd->viewangles);

		if (G::SendPacket) {
			G::proper_obs = as->m_flGoalFeetYaw;

			memcpy(pp, local->GetPoseParamaters(), sizeof(float) * 24);
		}

		tickcount = Interfaces::Globals->tickcount;
	}
	else
		as->m_iLastClientSideAnimationUpdateFramecount = Interfaces::Globals->framecount;

	local->SetAbsAngles(Vector(0, G::proper_obs, 0));
	as->m_flFeetYawRate = 0.f;

	memcpy(local->GetAnimOverlays(), layers, sizeof(CAnimationLayer) * 13);
	memcpy(local->GetPoseParamaters(), pp, sizeof(float) * 24);
}
AnimationInfo& Animation::GetPlayerAnimationInfo(int32_t idx)
{
	return arr_infos[idx];
}