#include "LegitBot.h"
#include "../Helpers/input_system.hpp"
#include "../Options.hpp"
#include "../Helpers/math.hpp"
#include "../Helpers/utils.hpp"
#include "../ValveSDK/math/QAngle.hpp"
static int custom_delay = 0;
#define TICK_INTERVAL			(g_GlobalVars->interval_per_tick)
#define TIME_TO_TICKS( dt )		( (int)( 0.5f + (float)(dt) / TICK_INTERVAL ) )
legitbot::legitbot()
{
    best_target = -1;

    view_angle = QAngle(0.0f, 0.0f, 0.0f);
    aim_angle = QAngle(0.0f, 0.0f, 0.0f);
    delta_angle = QAngle(0.0f, 0.0f, 0.0f);
    final_angle = QAngle(0.0f, 0.0f, 0.0f);

    hitbox_position = Vector(0.0f, 0.0f, 0.0f);

    aim_key = 0;
    aim_smooth = 1;
    aim_fov = 0;
    randomized_smooth = 0;
    recoil_min = 0;
    recoil_max = 0;
    randomized_angle = 0;
    shoot = false;
}

void legitbot::on_create_move(CUserCmd *cmd, C_BasePlayer* local, C_BaseCombatWeapon* weapon)
{

    do_aimbot(local, weapon, cmd);
    
    if (!PressedKeys[g_Options.triggerbot_key.get_int()]) custom_delay = 0;

    if (g_Options.triggerbot_enabled.get_bool() && g_Options.triggerbot_key.get_int() != 0 && PressedKeys[g_Options.triggerbot_key.get_int()])
        triggerbot(cmd, local, weapon);
}

void legitbot::triggerbot(CUserCmd *cmd, C_BasePlayer* local, C_BaseCombatWeapon* weapon)
{
    if (!local)
        return;
    if (!weapon)
        return;

    if (!local->is_alive())
        return;

    if (weapon) {
        if (weapon->m_iClip1() == 0)
            return;
        if (weapon->is_knife() || weapon->is_grenade()) return;
        if (weapon->m_Item().m_iItemDefinitionIndex() == 64) return;
    }



    QAngle ViewAngles = cmd->viewangles;


    Vector CrosshairForward;
    math::angle_vectors(ViewAngles, CrosshairForward);
    //CrosshairForward *= weapon->get_weapon_data()->m_flRange;
    CrosshairForward *= 8000.f;


    Vector TraceSource = local->get_eye_pos();
    Vector TraceDestination = TraceSource + CrosshairForward;

    Ray_t Ray;
    trace_t Trace;
    CTraceFilter Filter;

    Filter.pSkip = local;

    Ray.Init(TraceSource, TraceDestination);
    g_EngineTrace->TraceRay(Ray, MASK_SHOT, &Filter, &Trace);

    if (!Trace.hit_entity)
        return;
    if (!Trace.hit_entity->is_alive())
        return;
    if (Trace.hit_entity->m_iHealth() <= 0 || Trace.hit_entity->m_iHealth() > 100)
        return;
    if (Trace.hit_entity->m_bGunGameImmunity())
        return;


    if (local->m_iTeamNum() == Trace.hit_entity->m_iTeamNum())
        return;

    if (!hit_chance(local, cmd, weapon, Trace.hit_entity))
        return;

    bool didHit = false;
    if ((g_Options.triggerbot_Head.get_bool() && Trace.hitgroup == 1)
        || (g_Options.triggerbot_Chest.get_bool() && Trace.hitgroup == 2) 
        || (g_Options.triggerbot_Stomach.get_bool() && Trace.hitgroup == 3) 
        || (g_Options.triggerbot_Arms.get_bool() && (Trace.hitgroup == 4 || Trace.hitgroup == 5)) 
        || (g_Options.triggerbot_Legs.get_bool() && (Trace.hitgroup == 6 || Trace.hitgroup == 7))) 
    {
        didHit = true;
    }

    if (g_Options.triggerbot_delay.get_int() >= 1)
    {
        if (custom_delay >= g_Options.triggerbot_delay.get_int() / 30)
        {
            if (didHit)
            {
                custom_delay = 0;
                shoot = true;
                cmd->buttons |= IN_ATTACK;
            }
        }
        else
        {
            custom_delay++;
        }
    }

}


void legitbot::do_aimbot(C_BasePlayer *local, C_BaseCombatWeapon *weapon, CUserCmd *cmd)
{
    if (!g_Options.legitbot_enabled.get_bool())
        return;

    if (!local)
        return;

    if (!weapon)
        return;

    if (!local->is_alive())
        return;

    if (!weapon->has_bullets())
        return;

    weapon_settings(weapon);

	if (!PressedKeys[aim_key])
        return;

    best_target = get_target(local, weapon, cmd, hitbox_position);

    if (best_target == -1)
        return;
    C_BasePlayer* entity = (C_BasePlayer*)g_EntityList->GetClientEntity(best_target);
    if (!entity)
        return;



    if (math::get_distance(local->get_eye_pos(), hitbox_position) > 8192.0f)
        return;

    math::compute_angle(local->get_eye_pos(), hitbox_position, aim_angle);
    math::normalize_angles(aim_angle);

    aim_angle -= get_randomized_recoil(local);
    aim_angle += get_randomized_angles(local);

    math::normalize_angles(view_angle);

    delta_angle = view_angle - aim_angle;
    math::normalize_angles(delta_angle);

    float randomSmoothing = 1.0f;

    if (randomized_smooth > 1.0f)
        randomSmoothing = utils::random_number_range(randomized_smooth / 10.0f, 1.0f);

    final_angle = view_angle - delta_angle / aim_smooth * randomSmoothing;
    math::normalize_angles(final_angle);

    if (!math::sanitize_angles(final_angle))
        return;

    cmd->viewangles = final_angle;
    g_EngineClient->SetViewAngles(cmd->viewangles);
}

bool legitbot::hit_chance(C_BasePlayer* local, CUserCmd* cmd, C_BaseCombatWeapon* weapon, C_BasePlayer* target)
{
    Vector forward, right, up;

    constexpr auto max_traces = 150;

    math::angle_vectors(cmd->viewangles, forward, right, up);

    int total_hits = 0;
    int needed_hits = static_cast<int>(max_traces * (g_Options.triggerbot_hitchance.get_int() / 100.f));

    weapon->update_accuracy_penalty();

    auto eyes = local->get_eye_pos();
    auto flRange = weapon->get_weapon_data()->m_fRange;

    for (int i = 0; i < max_traces; i++) {
        RandomSeed(i + 1);

        float fRand1 = RandomFloat(0.f, 1.f);
        float fRandPi1 = RandomFloat(0.f, XM_2PI);
        float fRand2 = RandomFloat(0.f, 1.f);
        float fRandPi2 = RandomFloat(0.f, XM_2PI);
        
        float fRandInaccuracy = fRand1 * weapon->get_inaccuracy();
        float fRandSpread = fRand2 * weapon->get_spread();

        float fSpreadX = cos(fRandPi1) * fRandInaccuracy + cos(fRandPi2) * fRandSpread;
        float fSpreadY = sin(fRandPi1) * fRandInaccuracy + sin(fRandPi2) * fRandSpread;

        auto viewSpreadForward = (forward + fSpreadX * right + fSpreadY * up).Normalized();

        QAngle viewAnglesSpread;
        math::vector_angles(viewSpreadForward, viewAnglesSpread);
        math::normalize_angles(viewAnglesSpread);
        math::clamp_angles(viewAnglesSpread);

        Vector viewForward;
        math::angle_vectors(viewAnglesSpread, viewForward);
        viewForward.NormalizeInPlace();

        viewForward = eyes + (viewForward * flRange);

        trace_t tr;
        Ray_t ray;
        ray.Init(eyes, viewForward);

        g_EngineTrace->ClipRayToEntity(ray, MASK_SHOT | CONTENTS_GRATE, target, &tr);


        if (tr.hit_entity == target)
            total_hits++;

        if (total_hits >= needed_hits)
            return true;

        if ((max_traces - i + total_hits) < needed_hits)
            return false;
    }

    return false;
}

void legitbot::weapon_settings(C_BaseCombatWeapon* weapon)
{
    if (!weapon)
        return;

    if (weapon->is_sniper())
    {
        aim_key = g_Options.legitbot_key_sniper.get_int();
        aim_smooth = g_Options.legitbot_smooth_sniper.get_float();
        aim_fov = g_Options.legitbot_fov_sniper.get_float();
        randomized_smooth = g_Options.legitbot_randomized_smooth_sniper.get_float();
        recoil_min = g_Options.legitbot_randomized_recoil_min_sniper.get_float();
        recoil_max = g_Options.legitbot_randomized_recoil_max_sniper.get_float();
        randomized_angle = g_Options.legitbot_randomize_angle_sniper.get_float();

    }
    else if (weapon->is_pistol())
    {
        aim_key = g_Options.legitbot_key_pistol.get_int();
        aim_smooth = g_Options.legitbot_smooth_pistol.get_float();
        aim_fov = g_Options.legitbot_fov_pistol.get_float();
        randomized_smooth = g_Options.legitbot_randomized_smooth_pistol.get_float();
        recoil_min = g_Options.legitbot_randomized_recoil_min_pistol.get_float();
        recoil_max = g_Options.legitbot_randomized_recoil_max_pistol.get_float();
        randomized_angle = g_Options.legitbot_randomize_angle_pistol.get_float();

    }
    else
    {
        aim_key = g_Options.legitbot_key_rifle.get_int();
        aim_smooth = g_Options.legitbot_smooth_rifle.get_float();
        aim_fov = g_Options.legitbot_fov_rifle.get_float();
        randomized_smooth = g_Options.legitbot_randomized_smooth_rifle.get_float();
        recoil_min = g_Options.legitbot_randomized_recoil_min_rifle.get_float();
        recoil_max = g_Options.legitbot_randomized_recoil_max_rifle.get_float();
        randomized_angle = g_Options.legitbot_randomize_angle_rifle.get_float();

    }
}

QAngle legitbot::get_randomized_recoil(C_BasePlayer *local)
{
    QAngle compensatedAngles = (local->m_aimPunchAngle() * 2.0f) * (utils::random_number_range(recoil_min, recoil_max) / 100.0f);
    math::normalize_angles(compensatedAngles);

    return (local->m_iShotsFired() > 1 ? compensatedAngles : QAngle(0.0f, 0.0f, 0.0f));
}

QAngle legitbot::get_randomized_angles(C_BasePlayer *local)
{
    QAngle randomizedValue = QAngle(0.0f, 0.0f, 0.0f);

    float randomRate = utils::random_number_range(-randomized_angle, randomized_angle);
    float randomDeviation = utils::random_number_range(-randomized_angle, randomized_angle);

    switch (rand() % 2)
    {
    case 0:
        randomizedValue.pitch = (randomRate * cos(randomDeviation));
        randomizedValue.yaw = (randomRate * cos(randomDeviation));
        randomizedValue.roll = (randomRate * cos(randomDeviation));
        break;
    case 1:
        randomizedValue.pitch = (randomRate * sin(randomDeviation));
        randomizedValue.yaw = (randomRate * sin(randomDeviation));
        randomizedValue.roll = (randomRate * sin(randomDeviation));
        break;
    }

    math::normalize_angles(randomizedValue);

    return (local->m_iShotsFired() > 1 ? randomizedValue : QAngle(0.0f, 0.0f, 0.0f));
}

bool legitbot::get_hitbox(C_BasePlayer *local, C_BasePlayer *entity, Vector &destination)
{
    int bestHitbox = -1;
    float best_fov = aim_fov;

    static const std::vector<int> hitboxes = { HITBOX_HEAD, HITBOX_CHEST };

    for (auto hitbox : hitboxes)
    {
        Vector temp;
        if (!entity->get_hitbox_pos(hitbox, temp))
            continue;

        float fov = math::get_fov(view_angle, math::compute_angle(local->get_eye_pos(), temp));
        if (fov < best_fov)
        {
            best_fov = fov;
            bestHitbox = hitbox;
        }
    }

    if (bestHitbox != -1)
    {
        if (!entity->get_hitbox_pos(bestHitbox, destination))
            return true;
    }

    return false;
}

int legitbot::get_target(C_BasePlayer *local, C_BaseCombatWeapon *weapon, CUserCmd *cmd, Vector &destination)
{
    int best_target = -1;
    float best_fov = FLT_MAX;

    g_EngineClient->GetViewAngles(view_angle);

    for (int i = 1; i <= g_GlobalVars->maxClients; i++)
    {
        C_BasePlayer *entity = (C_BasePlayer*)g_EntityList->GetClientEntity(i);
        if (!entity
            || entity == local
            || entity->IsDormant()
            || entity->m_lifeState() != LIFE_ALIVE
            || entity->m_bGunGameImmunity()
            || entity->GetClientClass()->m_ClassID != CCSPlayer
            || !(entity->m_fFlags() & FL_ONGROUND))
            continue;

        if (checkteams)
        {
            if (entity->m_iTeamNum() == local->m_iTeamNum())
                continue;
        }

        Vector hitbox;
        if (get_hitbox(local, entity, hitbox))
            continue;

        float fov = math::get_fov(view_angle + (local->m_aimPunchAngle() * 2.0f), math::compute_angle(local->get_eye_pos(), hitbox));
        if (fov < best_fov && fov < aim_fov)
        {
            if (local->can_see_player(entity, hitbox))
            {
                best_fov = fov;
                destination = hitbox;
                best_target = i;
            }
        }
        /*
         * Aimbot with BackTracking
         */
        /*
        Vector hitboxPos = entity->get_hitbox_pos(0);


        float FOVDistance = math::get_fov(view_angle + (local->m_aimPunchAngle() * 2.0f), math::compute_angle(local->get_eye_pos(), hitboxPos));

        if (FOVDistance < best_fov && FOVDistance < aim_fov)
        {
            best_fov = FOVDistance;
            best_target = i;
        }
    }
    float bestTargetSimTime;
    if (best_target != -1)
    {
        float tempFloat = FLT_MAX;


        QAngle test = view_angle + (local->m_aimPunchAngle() * 2.0f);
        for (int t = 0; t <= 12; ++t)
        {
            float tempFOVDistance = math::get_fov(test, math::compute_angle(local->get_eye_pos(), headPositions[best_target][t].hitboxPos));

            if (tempFOVDistance < tempFloat && tempFOVDistance < aim_fov)
            {
                tempFloat = tempFOVDistance;
                bestTargetSimTime = headPositions[best_target][t].simtime;
                destination = headPositions[best_target][t].hitboxPos;
            }
        }
        if (cmd->buttons & IN_ATTACK)
        {
            cmd->tick_count = TIME_TO_TICKS(bestTargetSimTime);

        }*/
    }

    return best_target;
}