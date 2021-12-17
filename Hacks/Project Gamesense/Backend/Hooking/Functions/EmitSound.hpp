#pragma once
#include "../../Configuration/Config.h"
#include "../../Interfaces/Interfaces.h"
#include "../../SDK/GlobalDefenitions.h"
#include "../../SDK/CViewSetup.h"
#include "../../SDK/CBaseEntity.h"

typedef void( __thiscall* fnEmitSound )( void* thisptr,
    void* filter, int iEntIndex, int iChannel, const char* pSoundEntry, unsigned int nSoundEntryHash,
    const char* pSample, float flVolume, float flAttenuation, int nSeed, int iFlags, int iPitch, const Vector* pOrigin,
    const Vector* pDirection, Vector* pUtlVecOrigins, bool bUpdatePositions, float soundtime, int speakerentity, void* params );
fnEmitSound oEmitSound;

void SetLocalPlayerReady()
{
	static auto SetLocalPlayerReadyFn = reinterpret_cast<bool( __stdcall* )( const char* )>( Utilities->Memory_PatternScan( "client.dll", "55 8B EC 83 E4 F8 8B 4D 08 BA ? ? ? ? E8 ? ? ? ? 85 C0 75 12" ) );

	if ( SetLocalPlayerReadyFn )
		SetLocalPlayerReadyFn( "deferred" );
}

namespace Cheat
{
	namespace Hooked
	{
		void __stdcall emit_sound(void* filter, int entity_index, int channel, const char* sound_entry, uint32_t sound_entry_hash, const char* sample, float volume, float attenuation, int seed, int flags, int pitch, const Vector* origin, const Vector* direction, Vector* utl_origins, bool update_positions, float sound_time, int speaker_entity, void* parameters)
		{
			if (Cheat::Settings->UnloadCheat)
			{
				Cheat::Settings->UnloadReadyTable.emit_sound = 0;
				oEmitSound(Interfaces::EngineSound, filter, entity_index, channel, sound_entry, sound_entry_hash, sample, volume, attenuation, seed, flags, pitch, origin, direction, utl_origins, update_positions, sound_time, speaker_entity, parameters);
				return;
			}
			Cheat::Settings->UnloadReadyTable.emit_sound = (int)GetTickCount();

			/*if (Interfaces::Engine->IsInGame() && Interfaces::Engine->IsConnected() && c_config::get()->b["vis_soundesp"] && origin && entity_index != Interfaces::Engine->GetLocalPlayerIndex())
				SoundEsp->AddStepManually(entity_index, *origin);*/
			if ( c_config::get()->b["misc_autoacceptmm"] )
			{
				if ( !strcmp(sound_entry, "UIPanorama.popup_accept_match_beep" ) )
					SetLocalPlayerReady();
			}

			for (auto hk : c_lua::get()->hooks->getHooks("on_emit_sound"))
				hk.func(entity_index, channel, sound_entry, sample, volume, attenuation, seed, flags, pitch, origin, direction, sound_time);

			oEmitSound( Interfaces::EngineSound, filter, entity_index, channel, sound_entry, sound_entry_hash, sample, volume, attenuation, seed, flags, pitch, origin, direction, utl_origins, update_positions, sound_time, speaker_entity, parameters);
		}
	}
}