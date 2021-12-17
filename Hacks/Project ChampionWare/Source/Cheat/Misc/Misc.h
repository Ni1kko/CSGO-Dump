#pragma once

#include "../../Engine/Engine.h"

class CMisc
{
public:
//[swap_lines]
	void OnRender();
	void OnCreateMove( CUserCmd* pCmd );
	void OnDrawModelExecute();
	
	void KillSpam(IGameEvent* pEvent);
	//void OnPlaySound( const char* pszSoundName );
	void Emit_Sound(const char* sound_entry);
	//void OnOverrideView( CViewSetup* pSetup );
	//void OnGetViewModelFOV( float& fov );

	vector<int> GetObservervators( int playerId );
	void OnRenderSpectatorList();
//[/swap_lines]
};

void AutoAccept();
void Crosshair(IDirect3DDevice9* pDevice);