#pragma once

#include "../singleton.hpp"

class C_BasePlayer;
class CUserCmd;

class fake_lag
    : public singleton<fake_lag>
{
public:
    void on_create_move(C_BasePlayer* local, CUserCmd* cmd, bool& bsendpacket);

};