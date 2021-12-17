#pragma once
#include "../Include/GlobalIncludes.h"
#include "../Interfaces/Interfaces.h"
#include "CUserCmd.h"
#include "GlobalDefenitions.h"
#include "CBaseEntity.h"
#include "ISurface.h"
#include "../Features/Visuals/Visuals.h"
#include "ModelRender.h"
#include <fstream>
class CMaterialHelper
{
	public:
		CMaterialHelper();
		IMaterial* CoveredLit = nullptr;
		IMaterial* OpenLit = nullptr;
		IMaterial* CoveredFlat = nullptr;
		IMaterial* OpenFlat = nullptr;
		IMaterial* CoveredMetallic = nullptr;
		IMaterial* OpenMetallic = nullptr;
		void ForceMaterial( CColor color, IMaterial* material, bool useColor = true, bool forceMaterial = true );
		IMaterial* CreateMaterial( bool shouldIgnoreZ, bool isLit = true, bool isWireframe = false );
		void InitKeyValues( KeyValues* pKeyValues, const char* name );
		void LoadFromBuffer( KeyValues* pKeyValues, const char* resourceName, const char* pBuffer, void* pFileSystem = nullptr, const char* pPathID = NULL, void* pfnEvaluateSymbolProc = nullptr );
};
extern CMaterialHelper* MaterialHelper;
