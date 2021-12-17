#pragma once

#define BACKTRACKING_ENABLED

#define	RFL_ONGROUND	(1 << 0)	
#define	RFL_MOVING		(1 << 1)
#define MAX_RECORDS 64
#define ACCELERATION_CALC_TIME 0.3f
#define ROTATION_CALC_TIME 0.5f
#define DIRECTION_CALC_TIME 0.5f
#define FLAG_CALC_TIME 0.9f
#define MAX_LAYER_RECORDS 15
#define MAXSTUDIOBONES 128		// total bones actually used
#define MAX_PLAYERS 65
#include <deque>
#include <array>
#include "../SDK/AnimLayer.h"
#include "../SDK/CUserCmd.h"
namespace SDK
{
	class CUserCmd;
	class CBaseEntity;
	class ConVar;
	class CBaseWeapon;

}

struct CIncomingSequence
{
	CIncomingSequence::CIncomingSequence(int instate, int outstate, int seqnr, float time)
	{
		inreliablestate = instate;
		outreliablestate = outstate;
		sequencenr = seqnr;
		curtime = time;
	}
	int inreliablestate;
	int outreliablestate;
	int sequencenr;
	float curtime;
};

struct legit_backtrackdata
{
	float simtime;
	Vector hitboxPos;
};


class VarMapEntry_t
{
public:
	unsigned short type;
	unsigned short m_bNeedsToInterpolate; // Set to false when this var doesn't
										  // need Interpolate() called on it anymore.
	void *data;
	void *watcher;
};

struct VarMapping_t
{
	VarMapping_t()
	{
		m_nInterpolatedEntries = 0;
	}

	VarMapEntry_t* m_Entries;
	int m_nInterpolatedEntries;
	float m_lastInterpolationTime;
};
extern SDK::CUserCmd*						UserCmdForBacktracking;
struct Incoming_Sequence_Record
{
	Incoming_Sequence_Record(int in_reliable, int out_reliable, int in_sequence, float realtime)
	{
		in_reliable_state = in_reliable;
		out_reliable_state = out_reliable;
		in_sequence_num = in_sequence;

		time = realtime;
	}

	int in_reliable_state;
	int out_reliable_state;
	int in_sequence_num;

	float time;
};
struct LayerRecord {
	int m_nSequence;
	float m_flCycle;
	float m_flWeight;
	int m_nOrder;

	LayerRecord() {
		m_nSequence = 0;
		m_flCycle = 0.f;
		m_flWeight = 0.f;
		m_nOrder = 0;
	}
};

struct LagRecord {
	LagRecord() {
		m_fFlags = 0;
		m_vecOrigin.Init(0, 0, 0);
		m_vecEyeAngles.Init(0, 0, 0);
		m_vecAbsAngles.Init(0, 0, 0);
		m_flSimulationTime = 0;
		m_flLowerBodyYawTarget = 0.f;
	}

	int						m_fFlags;
	Vector					m_vecOrigin;
	Vector					m_vecEyeAngles;
	Vector					m_vecAbsAngles;
	float					m_flLowerBodyYawTarget;

	float					m_flSimulationTime;
	float*					m_flPoseParameter;

	std::array<LayerRecord, MAX_LAYER_RECORDS>	m_layerRecords;

	VMatrix				m_bone_matrix[128];
};

struct resolver_info_t {
	int				flags_i;
	int				tick_count;
	LagRecord		lag_record;
	bool			resolved;
	int				sequence_activity;
	bool			valid;
};
struct resolver_player_t {
	int				index;
	SDK::CBaseEntity*	player;
	std::array<LayerRecord, MAX_LAYER_RECORDS> prevlayers;
	std::deque<resolver_info_t> storedTicks;

	bool			breakinglby = false;
	bool			breakinglbyu120 = false;
};
struct tick_record {
	Vector m_vecOrigin;
	bool bLowerBodyYawUpdated;
	bool bFakelagging;
	float m_flSimulationTime;
	float m_flAnimTime;
	int m_nSequence;
	float m_flCycle;
	Vector m_angEyeAngles;
	Vector m_vecAbsOrigin;
	int32_t m_nFlags;
	Vector m_absangles;
	Vector m_vecMins;
	Vector m_vecMax;
	std::array<float, 24> ragpos;


	float m_flUpdateTime;
	Vector m_vecVelocity;
	float m_flPoseParameter[24];
	float backtrack_time;

	bool needs_extrapolation = false;
	VMatrix boneMatrix[128];
	//	matrix3x4_t boneMatrix2[128];
};

struct player_record
{
	//std::vector<tick_record> records;
	std::vector<tick_record> records;
	Vector EyeAngles;
	float LowerBodyYawTarget;
	int Flags;

	SDK::CAnimationLayer currentLayer;
	SDK::CAnimationLayer previous_layer;

	float unknown;
};
struct simulation_record {
	Vector origin;
	Vector velocity;
	Vector acceleration;
	float simulation_time;
	float simulation_time_increasment_per_tick;
	float update_time;
};
class CBacktrack
{


public:
	player_record PlayerRecord[64];
	resolver_player_t * get_records(int index);
	void UpdateIncomingSequences();
	int last_incoming_sequence = 0;

	void ClearIncomingSequences();

	void AddLatency(SDK::NetChannel* net_channel, float latency);
	std::deque<Incoming_Sequence_Record> sequence_records;
	SDK::NetChannel* netchan;
	void backtrack_player(SDK::CUserCmd * cmd);
	int GetLatencyTicks();
	//bool IsTickValid(int tick);
	bool IsTickValid(tick_record record);
	//float GetEstimateServerTime();
	float GetLerpTime();
	simulation_record SimRecord[64][7];
	float GetNetworkLatency();
	void UpdateBacktrackRecords(SDK::CBaseEntity* pPlayer);
	void UpdateExtrapolationRecords(SDK::CBaseEntity* pPlayer);
	//void AnimationFix(ClientFrameStage_t stage);


	void run_legit(SDK::CUserCmd * cmd);
	void DisableInterpolation(SDK::CBaseEntity* pEntity)
	{
		VarMapping_t* map = GetVarMap(pEntity);
		if (!map) return;
		for (int i = 0; i < map->m_nInterpolatedEntries; i++)
		{
			VarMapEntry_t *e = &map->m_Entries[i];
			e->m_bNeedsToInterpolate = false;
		}
	}
private:
	VarMapping_t * GetVarMap(void* pBaseEntity)
	{
		return reinterpret_cast<VarMapping_t*>((DWORD)pBaseEntity + 0x24);
	}
	int latest_tick;
	Vector angle_vector(Vector meme);
	float point_to_line(Vector Point, Vector LineOrigin, Vector Dir);
	bool IsValid(SDK::CBaseEntity * player);
	resolver_player_t players[MAX_PLAYERS];
};

extern legit_backtrackdata headPositions[64][12];
extern CBacktrack* backtracking;
