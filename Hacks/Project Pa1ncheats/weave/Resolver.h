#pragma once
struct animation;
class CResolver
{
private:
    float GetLeftYaw(IBasePlayer);
    float GetRightYaw(IBasePlayer);
    bool TargetSide(IBasePlayer);
    void DetectSide(IBasePlayer, int); //you sould ignore visual studio. these functions dont have any problem
    void FixPitch(IBasePlayer pEnt);
    bool TargetJitter(IBasePlayer, bool);
    bool DoesHaveJitter(IBasePlayer, int);
    int FreestandSide[64];
public:
    float GetAngle(IBasePlayer);
    float GetForwardYaw(IBasePlayer);
    float GetBackwardYaw(IBasePlayer);
    bool DoesHaveJitter(IBasePlayer* player, int* new_side);
    void StoreAntifreestand();
    void Do(IBasePlayer* player);
    float GetAngle(IBasePlayer* player);
    float GetForwardYaw(IBasePlayer* player);
    float GetBackwardYaw(IBasePlayer* player);
    float GetLeftYaw(IBasePlayer* player);
    float GetRightYaw(IBasePlayer* player);
    bool TargetJitter(IBasePlayer* player, bool v2);
    bool TargetSide(IBasePlayer* player);
    void DetectSide(IBasePlayer* player, int* side);
};
extern CResolver* resolver;
extern std::string ResolverMode[65];