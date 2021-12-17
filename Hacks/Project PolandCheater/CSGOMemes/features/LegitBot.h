#pragma once

#include "../ValveSDK/math/Vector.hpp"
#include "../ValveSDK/math/QAngle.hpp"
#include "../ValveSDK/csgostructs.hpp"
#include "../singleton.hpp"
class legitbot
	: public singleton<legitbot>
{
public:
	legitbot();

	void weapon_settings(C_BaseCombatWeapon* weapon);

    void on_create_move(CUserCmd* cmd, C_BasePlayer* local, C_BaseCombatWeapon* weapon);
    bool hit_chance(C_BasePlayer* local, CUserCmd* cmd, C_BaseCombatWeapon* weapon, C_BasePlayer* target);

    void do_aimbot(C_BasePlayer *local, C_BaseCombatWeapon *weapon, CUserCmd *cmd);
    QAngle get_randomized_recoil(C_BasePlayer* local);
    QAngle get_randomized_angles(C_BasePlayer* local);
    void triggerbot(CUserCmd* cmd, C_BasePlayer* local, C_BaseCombatWeapon* weapon);

private:
    bool get_hitbox(C_BasePlayer *local, C_BasePlayer *entity, Vector &destination);
    int get_target(C_BasePlayer *local, C_BaseCombatWeapon *weapon, CUserCmd *cmd, Vector &destination);

private:
    int best_target;

    QAngle view_angle;
    QAngle aim_angle;
    QAngle delta_angle;
    QAngle final_angle;

    Vector hitbox_position;

    int aim_key;
    float aim_smooth;
    float aim_fov;
    float randomized_smooth;
    float recoil_min;
    float recoil_max;
    float randomized_angle;
    bool shoot;
};

