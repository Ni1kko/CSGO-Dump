#pragma once
class C_BasePlayer;
class CUserCmd;
class QAngle;
namespace BunnyHop
{
    void OnCreateMove(CUserCmd* cmd);
	void AutoStrafe(CUserCmd* cmd, QAngle oldangles);
	void FixMovement(CUserCmd * usercmd, QAngle & wish_angle);
	void AntiAim(CUserCmd * usercmd);
}