#include "../includes.h"
#include "../SDK/CUserCmd.h"
#include "fake_walk.h"
#include <time.h>
#include <iostream>
#include "../SDK/CClientEntityList.h"
#include "../SDK/IEngine.h"

void c_fake_walk::do_fake_walk(SDK::CUserCmd* cmd)
{
	auto local_player = INTERFACES::ClientEntityList->GetClientEntity(INTERFACES::Engine->GetLocalPlayer());

	if (!local_player)
		return;

	if (!SETTINGS::settings.slow_walk || !GetAsyncKeyState(SETTINGS::settings.slow_walk_key))
		return;

	auto weapon_handle = reinterpret_cast<SDK::CBaseWeapon*>(INTERFACES::ClientEntityList->GetClientEntity(local_player->GetActiveWeaponIndex()));

	if (!weapon_handle)
		return;

	float amount = 0.0034f * SETTINGS::settings.fake_walk_speed;

	Vector velocity = local_player->GetVelocity();
	Vector direction;

	MATH::AngleVectors3(velocity, direction);

	float speed = velocity.Length2D();

	direction.y = cmd->viewangles.y - direction.y;

	Vector forward;

	MATH::AngleVectors3(direction, forward);

	Vector source = forward * -speed;

	if (speed >= (weapon_handle->GetWeaponInfo()->max_speed * amount))
	{
		cmd->forwardmove = source.x;
		cmd->sidemove = source.y;

	}
}

c_fake_walk* fakewalk = new c_fake_walk();