#pragma once

#include <cstdint>

// Created with ReClass.NET by KN4CK3R
#pragma pack(push, 1)
class INetChannel
{
public:
    char pad_0000[20];           //0x0000
    bool m_bProcessingMessages;  //0x0014
    bool m_bShouldDelete;        //0x0015
    char pad_0016[2];            //0x0016
    int32_t m_nOutSequenceNr;    //0x0018 last send outgoing sequence number
    int32_t m_nInSequenceNr;     //0x001C last received incoming sequnec number
    int32_t m_nOutSequenceNrAck; //0x0020 last received acknowledge outgoing sequnce number
    int32_t m_nOutReliableState; //0x0024 state of outgoing reliable data (0/1) flip flop used for loss detection
    int32_t m_nInReliableState;  //0x0028 state of incoming reliable data
    int32_t m_nChokedPackets;    //0x002C number of choked packets
    char pad_0030[1044];         //0x0030
}; //Size: 0x0444

class CClockDriftMgr
{
public:
    float m_ClockOffsets[16];   //0x0000
    uint32_t m_iCurClockOffset; //0x0040
    int32_t m_nServerTick;      //0x0044
    int32_t m_nClientTick;      //0x0048
}; //Size: 0x004C

class CClientState
{
public:
    static void ForceFullUpdate();

    char pad_0000[156];             
    class INetChannel* m_NetChannel;
    uint32_t m_nChallengeNr;        
    double m_flConnectTime;         
    int32_t m_nRetryNumber;         
    char pad_00A8[88];              
    int32_t m_nSignonState;         
    char pad_0104[8];               
    float m_flNextCmdTime;          
    int32_t m_nServerCount;         
    uint32_t m_nCurrentSequence;    
    char pad_0118[8];               
    CClockDriftMgr m_ClockDriftMgr; 
    int32_t m_nDeltaTick;           
    bool m_bPaused;                 
    char pad_0171[3];               
    int32_t m_nViewEntity;          
    int32_t m_nPlayerSlot;          
    char pad_017C[4];               
    char m_szLevelName[260];        
    char m_szLevelNameShort[40];    
    char pad_02AC[92];              
    int32_t m_nMaxClients;          
    char pad_030C[4083];            
    uint32_t m_StringTableContainer;
    char pad_1303[14737];           
    float m_flLastServerTickTime;   
    bool insimulation;              
    char pad_4C99[3];               
    uint32_t oldtickcount;          
    float m_tickRemainder;          
    float m_frameTime;              
    int32_t lastoutgoingcommand;    
    int32_t chokedcommands;         
    int32_t last_command_ack;       
    int32_t command_ack;            
    int32_t m_nSoundSequence;       
    char pad_4CBC[80];              
    QAngle viewangles;              
}; //Size: 0x4D20
#pragma pack(pop)

C_ASSERT(FIELD_OFFSET(CClientState, m_NetChannel) == 0x9C);
C_ASSERT(FIELD_OFFSET(CClientState, m_nSignonState) == 0x108);
C_ASSERT(FIELD_OFFSET(CClientState, lastoutgoingcommand) == 0x4CB0);
C_ASSERT(FIELD_OFFSET(CClientState, viewangles) == 0x4D14);