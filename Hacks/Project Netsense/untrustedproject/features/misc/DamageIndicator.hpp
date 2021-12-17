#include "../../valve_sdk/sdk.hpp"
#include "../../options.hpp"
#include "../../singleton.hpp"

struct DamageIndicator_t
{
	int iDamage;
	bool bInitialized;
	float flEraseTime;
	float flLastUpdate;
	C_BasePlayer* Player;
	Vector Position;
};


class DamageIndicators : public Singleton<DamageIndicators> {
	friend class Singleton<DamageIndicators>;
public:
	std::vector<DamageIndicator_t> DamageIndicator;
	void paint();
	void fireEvent(IGameEvent* pEvent);
};
