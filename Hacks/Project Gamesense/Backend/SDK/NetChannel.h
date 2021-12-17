#pragma once
#include <Windows.h>
#include "vmatrix.h"
#include "vfunc.hpp"
class NetChannel
{
	public:
		char pad_0x0000[0x18]; //0x0000
		__int32 m_nOutSequenceNr; //0x0018
		__int32 m_nInSequenceNr; //0x001C
		__int32 m_nOutSequenceNrAck; //0x0020
		__int32 m_nOutReliableState; //0x0024
		__int32 m_nInReliableState; //0x0028
		__int32 m_nChokedPackets; //0x002C

		void send_datagram()
		{
			using Fn = int(__thiscall *)(void *, void *);
			vfunc< Fn >(this, 46)(this, NULL);
		}
};
