#include "overrideconfig.h"

OverrideConfigFn oOverrideConfig;

bool __fastcall HOOKED_OverrideConfig( IMaterialSystem* this0, int edx, MaterialSystem_Config_t* config, bool forceUpdate )
{
	config->m_nFullbright = g_Var.Visuals.World.Fullbright ? 1 : 0;

/*	config->m_bDrawGray = g_Var.Visuals.World.WorldStyle[ 1 ] ? 1 : 0;

	config->m_bShowLowResImage = g_Var.Visuals.World.WorldStyle[ 2 ] ? 1 : 0;
	
	config->m_nShowMipLevels = g_Var.Visuals.World.WorldStyle[ 3 ] ? 1 : 0;*/

	return oOverrideConfig( this0, config, forceUpdate );
}