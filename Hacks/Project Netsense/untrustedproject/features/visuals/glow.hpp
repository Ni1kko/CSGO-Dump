#pragma once
#include "../../valve_sdk/sdk.hpp"
#include "../../valve_sdk/csgostructs.hpp"


class Glow {
public:
    Glow();
    ~Glow();

public:
    void Run();
    void Shutdown();
};


extern Glow* g_Glow;