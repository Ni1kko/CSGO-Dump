#include "singleton.hpp"
#include "valve_sdk/csgostructs.hpp"

#pragma once
class HitPossitionHelper : public Singleton<HitPossitionHelper>
{
public:
	void OnFireEvent(IGameEvent* event);
	void ShotTracer(Vector shot_pos, Vector hit_pos);
private:
	void PlayerHurt(IGameEvent* event);
	void BulletImpact(IGameEvent* event);
	c_base_player* GetPlayer(int i);
};
