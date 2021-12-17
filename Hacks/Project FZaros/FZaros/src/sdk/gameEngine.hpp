#pragma once
#include "../utilities/interfaceUtilities.hpp"
#include "vector.hpp"

#define FLOW_OUTGOING	0
#define FLOW_INCOMING	1
#define MAX_FLOWS		2

class Color;

struct playerinfo_t
{
	int64_t __pad0;

	union
	{
		int64_t xuid;
		struct
		{
			int xuidlow;
			int xuidhigh;
		};
	};

	char name[128];
	int userId;
	char guid[33];
	unsigned int friendsid;
	char friendsname[128];
	bool fakeplayer;
	bool ishltv;
	unsigned int customfiles[4];
	unsigned char filesdownloaded;
};

class INetChannelInfo
{
public:
	enum
	{
		GENERIC = 0,    // must be first and is default group
		LOCALPLAYER,    // bytes for local player entity update
		OTHERPLAYERS,   // bytes for other players update
		ENTITIES,       // all other entity bytes
		SOUNDS,         // game sounds
		EVENTS,         // event messages
		USERMESSAGES,   // user messages
		ENTMESSAGES,    // entity messages
		VOICE,          // voice data
		STRINGTABLE,    // a stringtable update
		MOVE,           // client move cmds
		STRINGCMD,      // string command
		SIGNON,         // various signondata
		TOTAL,          // must be last and is not a real group
	};

	virtual const char* GetName(void) const = 0;	            // get channel name
	virtual const char* GetAddress(void) const = 0;            // get channel IP address as string
	virtual float       GetTime(void) const = 0;	            // current net time
	virtual float       GetTimeConnected(void) const = 0;	    // get connection time in seconds
	virtual int         GetBufferSize(void) const = 0;	        // netchannel packet history size
	virtual int         GetDataRate(void) const = 0;            // send data rate in byte/sec

	virtual bool        IsLoopback(void) const = 0;             // true if loopback channel
	virtual bool        IsTimingOut(void) const = 0;            // true if timing out
	virtual bool        IsPlayback(void) const = 0;             // true if demo playback

	virtual float       GetLatency(int flow) const = 0;         // current latency (RTT), more accurate but jittering
	virtual float		GetAvgLatency(int flow) const = 0;      // average packet latency in seconds
	virtual float       GetAvgLoss(int flow) const = 0;         // avg packet loss[0..1]
	virtual float       GetAvgChoke(int flow) const = 0;        // avg packet choke[0..1]
	virtual float       GetAvgData(int flow) const = 0;         // data flow in bytes/sec
	virtual float       GetAvgPackets(int flow) const = 0;      // avg packets/sec
	virtual int         GetTotalData(int flow) const = 0;       // total flow in/out in bytes
	virtual int         GetSequenceNr(int flow) const = 0;      // last send seq number
	virtual bool        IsValidPacket(int flow, int frame_number) const = 0;                // true if packet was not lost/dropped/chocked/flushed
	virtual float       GetPacketTime(int flow, int frame_number) const = 0;                // time when packet was send
	virtual int         GetPacketBytes(int flow, int frame_number, int group) const = 0;    // group size of this packet
	virtual bool        GetStreamProgress(int flow, int* received, int* total) const = 0;   // TCP progress if transmitting
	virtual float       GetTimeSinceLastReceived(void) const = 0;// get time since last recieved packet in seconds
	virtual	float       GetCommandInterpolationAmount(int flow, int frame_number) const = 0;
	virtual void		GetPacketResponseLatency(int flow, int frame_number, int* pnLatencyMsecs, int* pnChoke) const = 0;
	virtual void		GetRemoteFramerate(float* pflFrameTime, float* pflFrameTimeStdDeviation) const = 0;

	virtual float		GetTimeoutSeconds() const = 0;
};

class NetChannel
{
public:

	int sendDataGram( )
	{
		using originalFn = VFUNC_SIGNATURE( int, void* );

		return ( ( originalFn ) g_InterfaceUtils->getVfuncTable( this )[ 46 ] )( this, NULL );
 	}

	uint8_t pad_0x0000[0x17];
	bool m_bShouldDelete;
	int m_nOutSequenceNr;
	int m_nInSequenceNr;
	int m_nOutSequenceNrAck;
	int m_nOutReliableState;
	int m_nInReliableState;
	int m_nChokedPackets;
};

class ClockDriftManager
{
public:
	float ClockOffsets[17];   //0x0000
	uint32_t CurrentClockOffsets; //0x0044
	uint32_t ServerTick;     //0x0048
	uint32_t ClientTick;     //0x004C
}; //Size: 0x0050


class CClientState
{
public:
	void FullUpdate()
	{
		*reinterpret_cast<int*>(uintptr_t(this) + 0x174) = -1;
	};

	char pad_0000[156];             //0x0000
	NetChannel* NetChannel;          //0x009C
	uint32_t ChallengeNumber;        //0x00A0
	char pad_00A4[100];             //0x00A4
	uint32_t SignonState;        //0x0108
	char pad_010C[8];               //0x010C
	float flNextCmdTime;          //0x0114
	uint32_t ServerCount;        //0x0118
	uint32_t CurrentSequence;    //0x011C
	char pad_0120[8];               //0x0120
	ClockDriftManager ClockDriftManager; //0x0128
	uint32_t DeltaTick;          //0x0178
	bool Paused;                 //0x017C
	char pad_017D[3];               //0x017D
	uint32_t ViewEntity;         //0x0180
	uint32_t PlayerSlot;         //0x0184
	char szLevelName[260];        //0x0188
	char szLevelNameShort[40];    //0x028C
	char szGroupName[40];         //0x02B4
	char pad_02DC[52];              //0x02DC
	uint32_t MaxClients;         //0x0310
	char pad_0314[18820];           //0x0314
	float LastServerTickTime;   //0x4C98
	bool InSimulation;              //0x4C9C
	char pad_4C9D[3];               //0x4C9D
	uint32_t OldTickCount;          //0x4CA0
	float TickRemainder;          //0x4CA4
	float FrameTime;              //0x4CA8
	uint32_t LastOutgoingCommand;   //0x4CAC
	uint32_t ChokedCommands;        //0x4CB0
	uint32_t LastCommandAck;      //0x4CB4
	uint32_t CommandAck;           //0x4CB8
	uint32_t SoundSequence;      //0x4CBC
	char pad_4CC0[80];              //0x4CC0
	Vector3 ViewAngles;              //0x4D10
}; //Size: 0x4D1C


class CGameEngine
{
public:

	virtual int                   unk31() = 0;
	virtual void*                unk32() = 0;
	virtual void*            unk3() = 0;
	virtual const char*           unk34() = 0;
	virtual bool                  unk33() = 0;
	virtual void                  unk35() = 0;
	virtual void                  unk36() = 0;
	virtual void                  unk37(const char *szCmdString) = 0;
	virtual bool                  getPlayerInfo2(int ent_num, playerinfo_t *pinfo) = 0;

	int getLocalPlayer()
	{
		using OriginalFn = VFUNC_SIGNATURE(int);

		return ((OriginalFn)g_InterfaceUtils->getVfuncTable(this)[12])(this);
	}

	int getPlayerIndex(int userId)
	{
		using OriginalFn = VFUNC_SIGNATURE(int, int);

		return ((OriginalFn)g_InterfaceUtils->getVfuncTable(this)[9])(this, userId);
	}

	void getPlayerInfo(int index, playerinfo_t* playerinfo)
	{
		using OriginalFn = VFUNC_SIGNATURE(void, int, playerinfo_t*);

		((OriginalFn)g_InterfaceUtils->getVfuncTable(this)[8])(this, index, playerinfo);
	}

	void getScreenSize(int &width, int &height)
	{
		using OriginalFn = VFUNC_SIGNATURE(void, int&, int&);

		((OriginalFn)g_InterfaceUtils->getVfuncTable(this)[5])(this, width, height);
	}

	void executeCmd(const char* cmd)
	{
		using OriginalFn = VFUNC_SIGNATURE(void, const char*);

		((OriginalFn)g_InterfaceUtils->getVfuncTable(this)[108])(this, cmd);
	}

	void setViewAngles(Vector3 &angle)
	{
		using OriginalFn = VFUNC_SIGNATURE(void, Vector3&);

		((OriginalFn)g_InterfaceUtils->getVfuncTable(this)[19])(this, angle);
	}

	void getViewAngles(Vector3 &angle)
	{
		using OriginalFn = VFUNC_SIGNATURE(void, Vector3&);

		((OriginalFn)g_InterfaceUtils->getVfuncTable(this)[18])(this, angle);
	}

	Vector3 getViewAngles()
	{
		Vector3 temp;

		using OriginalFn = VFUNC_SIGNATURE(Vector3, Vector3);
		((OriginalFn)g_InterfaceUtils->getVfuncTable(this)[18])(this, temp);

		return temp;
	}

	bool inGame()
	{
		using OriginalFn = VFUNC_SIGNATURE(bool);
		return ((OriginalFn)g_InterfaceUtils->getVfuncTable(this)[26])(this);
	}

	bool connected()
	{
		using OriginalFn = VFUNC_SIGNATURE(bool);
		return ((OriginalFn)g_InterfaceUtils->getVfuncTable(this)[27])(this);
	}

	const char* levelName()
	{
		using OriginalFn = VFUNC_SIGNATURE(const char*);
		return ((OriginalFn)g_InterfaceUtils->getVfuncTable(this)[52])(this);
	}

	INetChannelInfo* netchannelInfo()
	{
		using OriginalFn = VFUNC_SIGNATURE(INetChannelInfo*);

		return ((OriginalFn)g_InterfaceUtils->getVfuncTable(this)[78])(this);
	}

	NetChannel* getNetChannel()
	{
		using OriginalFn = VFUNC_SIGNATURE(NetChannel*);

		return ((OriginalFn)g_InterfaceUtils->getVfuncTable(this)[78])(this);
	}
};