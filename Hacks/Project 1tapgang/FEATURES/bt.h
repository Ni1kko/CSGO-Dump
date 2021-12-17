#pragma once
struct tick_record
{
	Vector m_vecOrigin;//0 //4 // 8
	bool bLowerBodyYawUpdated;
	bool bFakelagging;
	float m_flSimulationTime; //12
	float m_flAnimTime;
	int m_nSequence;//16
	float m_flCycle; //20
	Vector m_angEyeAngles;//x->24
						  //y->28
						  //z->32
	float m_flUpdateTime;//36
	Vector m_vecVelocity;//40 //44 //48
	float m_flPoseParameter[24];//52
	float backtrack_time;

	bool needs_extrapolation = false;
	/*my addition*/
	VMatrix boneMatrix[128];
};
template<class T, class U>
inline T clamp(T in, U low, U high)
{
	if (in <= low)
		return low;
	else if (in >= high)
		return high;
	else
		return in;
}
#define TICK_INTERVAL			( INTERFACES::Globals->interval_per_tick )
#define TIME_TO_TICKS( dt )		( floorf(( 0.5f + (float)(dt) / TICK_INTERVAL ) ) )
#define TICKS_TO_TIME( t )		( TICK_INTERVAL *( t ) )
struct player_record
{
	//std::vector<tick_record> records;
	std::vector<tick_record> records;
	Vector EyeAngles;
	float LowerBodyYawTarget;
	int Flags;
	float unknown;
};

class CBacktrackHelper
{
public:
	player_record PlayerRecord[64];
	
	float GetLerpTime();

	float GetNetworkLatency();
	int GetLatencyTicks();
	bool IsTickValid(tick_record record);
	float GetEstimateServerTime();
	void UpdateBacktrackRecords(SDK::CBaseEntity* pPlayer);

}; extern CBacktrackHelper* g_BacktrackHelper;

