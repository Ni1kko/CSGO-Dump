#pragma once
#include "../Hooks/Hooks.h"

#include "../../Include/GlobalIncludes.h"
#include "../../Interfaces/Interfaces.h"
#include "../../SDK/CUserCmd.h"
#include "../../SDK/GlobalDefenitions.h"
#include "../../SDK/CBaseEntity.h"
#include "../../SDK/ISurface.h"
#include "../../Features/Visuals/Visuals.h"
#include "../../SDK/ModelRender.h"
#include "../../SDK/CMaterialHelper.h"
#include "../../DLL/Engine.h"


using SendDatagramfn = int( __thiscall* )( void*, void* );
SendDatagramfn oSendDataGram;
namespace Cheat
{
	namespace Hooked
	{
		int __fastcall SendDataGramHook( void* net_channel, void*, void* datagram )
		{
			if (Cheat::Settings->UnloadCheat)
			{
				Cheat::Settings->UnloadReadyTable.SendDataGram = 0;
				return oSendDataGram(net_channel, datagram);
			}
			Cheat::Settings->UnloadReadyTable.SendDataGram = (int)GetTickCount();

			if (!c_config::get()->b["misc_pingspike"] || !GetAsyncKeyState( c_config::get()->i["misc_pingspike_key"] ) )
				return oSendDataGram( net_channel, datagram );

			auto* channel = reinterpret_cast<NetChannel*>( net_channel );
			int instate = channel->m_nInReliableState;
			int insequencenr = channel->m_nInSequenceNr;
			//Backtrack->add_latency(channel);
			int ret = oSendDataGram( channel, datagram );
			channel->m_nInReliableState = instate;
			channel->m_nInSequenceNr = insequencenr;
			return ret;
		}
	}
}