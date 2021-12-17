#include "hooks.hpp"
#include <intrin.h>
#include "../options.h"

#include "../helpers/input.hpp"
#include "../helpers/console.h"

void __stdcall hooks::hk_emit_sound1 ( IRecipientFilter& filter, int iEntIndex, int iChannel, const char* pSoundEntry, unsigned int nSoundEntryHash,
                                       const char* pSample, float flVolume, int nSeed, float flAttenuation, int iFlags, int iPitch, const Vector* pOrigin,
                                       const Vector* pDirection, void* pUtlVecOrigins, bool bUpdatePositions, float soundtime, int speakerentity, int unk )
{
    static auto ofunc = sound_hook.get_original< emit_sound1 > ( index::emit_sound1 );

    //if ( g_Unload ) return ofunc( interfaces::engine_sound, filter, iEntIndex, iChannel, pSoundEntry, nSoundEntryHash, pSample, flVolume, nSeed, flAttenuation, iFlags, iPitch, pOrigin, pDirection, pUtlVecOrigins, bUpdatePositions, soundtime, speakerentity, unk );

    if ( !strcmp ( pSoundEntry, xor_str ( "UIPanorama.popup_accept_match_beep" ) ) )
    {
        static auto fnAccept = reinterpret_cast< bool ( __stdcall*) ( const char* ) > ( g_utils.pattern_scan ( xor_str ( "client_panorama.dll" ),
                                                                                                               xor_str (
                                                                                                                   "55 8B EC 83 E4 F8 8B 4D 08 BA ? ? ? ? E8 ? ? ? ? 85 C0 75 12" ) )
        );

        if ( fnAccept )
        {
            fnAccept ( "" );
            //This will flash the CSGO window on the taskbar
            //so we know a game was found (you cant hear the beep sometimes cause it auto-accepts too fast)
            FLASHWINFO fi;
            fi.cbSize = sizeof ( FLASHWINFO );
            fi.hwnd = g_inputsys.get_main_window( );
            fi.dwFlags = FLASHW_ALL | FLASHW_TIMERNOFG;
            fi.uCount = 0;
            fi.dwTimeout = 0;
            FlashWindowEx ( &fi );
        }
    }

    ofunc ( interfaces::engine_sound, filter, iEntIndex, iChannel, pSoundEntry, nSoundEntryHash, pSample, flVolume, nSeed, flAttenuation, iFlags, iPitch,
            pOrigin, pDirection, pUtlVecOrigins, bUpdatePositions, soundtime, speakerentity, unk );
}
