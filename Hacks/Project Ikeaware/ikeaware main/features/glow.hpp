#pragma once

class Glow
{
public:
    Glow();
    ~Glow();

    void Run();
    void Shutdown();
};

inline Glow* g_Glow;