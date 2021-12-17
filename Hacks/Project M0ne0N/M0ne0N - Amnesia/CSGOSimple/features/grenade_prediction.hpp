#pragma once

namespace GrenadePrediction
{
	void predict( CUserCmd *ucmd );
	bool detonated( C_BaseCombatWeapon* weapon, float time, trace_t &trace );
	void trace( CUserCmd *ucmd );
	void draw();
}
