#pragma once

#include "../singleton.hpp"
#include "../ValveSDK/math/Vector.hpp"
#include "../ValveSDK/math/QAngle.hpp"

class CUserCmd;
class C_BasePlayer;

QAngle GetAutostrafeView();

class auto_strafe
    : public singleton<auto_strafe>
{
public:
    void on_create_move(CUserCmd *pCmd, C_BasePlayer* local);
    void strafe(CUserCmd *pCmd, C_BasePlayer* local);
};