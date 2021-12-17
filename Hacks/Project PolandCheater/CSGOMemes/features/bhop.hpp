#pragma once

#include "../singleton.hpp"

class C_BasePlayer;
class CUserCmd;

class bhop
    : public singleton<bhop>
{
public:
    void on_create_move(C_BasePlayer* local, CUserCmd* cmd);
};