#include "antiaim.h"

void NormalizeNum(Vector &vIn, Vector &vOut)
{
	float flLen = vIn.Length();
	if (flLen == 0) {
		vOut.Init(0, 0, 1);
		return;
	}
	flLen = 1 / flLen;
	vOut.Init(vIn.x * flLen, vIn.y * flLen, vIn.z * flLen);
}

float antiaim_helper::fov_ent(Vector ViewOffSet, Vector View, IClientEntity* entity, int hitbox)
{
	const float MaxDegrees = 180.0f;
	Vector Angles = View, Origin = ViewOffSet;
	Vector Delta(0, 0, 0), Forward(0, 0, 0);
	Vector AimPos = GetHitboxPosition(entity, hitbox);
	AngleVectors(Angles, &Forward);
	VectorSubtract(AimPos, Origin, Delta);
	NormalizeNum(Delta, Delta);
	float DotProduct = Forward.Dot(Delta);
	return (acos(DotProduct) * (MaxDegrees / PI));
}

int antiaim_helper::closest()
{
	int index = -1;
	float lowest_fov = 180.f; // maybe??

	IClientEntity* local_player = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

	if (!local_player)
		return -1;

	if (!local_player->IsAlive())
		return -1;

	Vector local_position = local_player->GetAbsOrigin() + local_player->GetViewOffset();
	Vector angles;
	Interfaces::Engine->GetViewAngles(angles);
	for (int i = 1; i <= Interfaces::Globals->maxClients; i++)
	{
		IClientEntity *entity = Interfaces::EntList->GetClientEntity(i);

		if (!entity || entity->GetHealth() <= 0 || entity->team() == local_player->team() || entity->is_dormant() || entity == local_player)
			continue;

		float fov = fov_ent(local_position, angles, entity, 0);
		if (fov < lowest_fov)
		{
			lowest_fov = fov;
			index = i;
			closest_entity = i;
		}

	}
	return index;

}