#pragma once
#include <deque>
struct Point
{
	Vector vecPoint;
	int iHitbox;
};
struct tick_record
{
	Vector m_vecOrigin, m_vecAbsOrigin, m_vecRenderAngles;
	Vector headpos;
	bool bLowerBodyYawUpdated;
	float m_flLowerBodyYaw;
	bool bFakelagging;
	float m_flSimulationTime;
	float m_flAnimTime;
	int m_nSequence;
	Vector m_angEyeAngles, m_absAngles;
	string resolver;
	float m_flUpdateTime;
	Vector m_vecVelocity;
	float backtrack_time;
	float m_flCycle;
	float m_flWeight;
	int m_nFlags;
	CAnimationLayer AnimLayers[15];
	matrix boneMatrix[128];
	std::vector<Point> Points;
	CCSGOPlayerAnimState* animstate;
	float pose_params[24];
	Vector m_vecMins;
	Vector m_vecMax;

};
struct lby_record
{
	float simtime;
	Vector headpos;
};
struct CIncomingSequence
{
	CIncomingSequence(int instate22, int outstate22, int seqnr22, float ctime22)
	{
		inreliablestate = instate22;
		outreliablestate = outstate22;
		sequencenr = seqnr22;
		curtime = ctime22;
	}
	int inreliablestate;
	int outreliablestate;
	int sequencenr;
	float curtime;
};

struct ResolverInfo
{
	std::string resolvermode = "unknown";
	float backtrack_time = -1.f;
	float next_lby_update;
};

class CMBacktracking
{
public:
	void UpdateIncomingSequences();
	void ClearIncomingSequences();
	void AddLatencyToNetchan(INetChannel* netchan, float Latency);
	void DoPingSpike(INetChannel* net_channel);
private:
};

extern CMBacktracking* g_Backtrack;