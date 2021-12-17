#pragma once

#include "main.h"
#include <memory>
#include <functional>
#include <numeric>

#define g_pGrenadePrediction CGrenadePrediction::GetInstance()

#define IMPLEMENT_SINGLETON(classname)\
public:\
	static std::shared_ptr<classname> GetInstance() {\
		static std::shared_ptr<classname> instance(new classname);\
		return instance;\
				}\
private:\
	classname() {}\
	classname(classname const&) = delete;\
	void operator=(classname const&) = delete;

class CGrenadePrediction 
{
	IMPLEMENT_SINGLETON( CGrenadePrediction );

public:
	void DoNadePrediction(CBaseEntity * localPlayer, CBaseCombatWeapon * weapon);
	void Tick( int buttons );
	void PredictGrenade( CBaseEntity * pLocal );
	void PaintGrenadeTrajectory( CBaseEntity * pLocal );
	std::vector<std::pair<Vector, Vector>> OtherCollisions;
private:
	void Setup( CBaseEntity* pl, Vector& vecSrc, Vector& vecThrow, const Vector& angEyeAngles );
	void Simulate();
	int  Step( Vector& vecSrc, Vector& vecThrow, int tick, float interval );
	bool CheckDetonate( const Vector& vecThrow, const CEngineTraceClient::trace_t& tr, int tick, float interval );
	void AddGravityMove( Vector& move, Vector& vel, float frametime, bool onground );
	void HullTrace( Vector src, Vector end, CEngineTraceClient::ITraceFilter* filter, CEngineTraceClient::trace_t& tr );
	void PushEntity( Vector& src, const Vector& move, CEngineTraceClient::trace_t& tr );
	void ResolveFlyCollisionCustom( CEngineTraceClient::trace_t& tr, Vector& vecVelocity, float interval );
	unsigned char PhysicsClipVelocity( const Vector& in, const Vector& normal, Vector& out, float overbounce );
};
