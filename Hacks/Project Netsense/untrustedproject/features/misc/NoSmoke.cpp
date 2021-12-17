#include "NoSmoke.hpp"
#include "../../options.hpp"
#include "../../valve_sdk/sdk.hpp"
#include "../../helpers/utils.hpp"
#include "misc.hpp"


namespace NoSmoke {
	void OnFrameStageNotify() {
		static auto linegoesthrusmoke = Utils::PatternScan(GetModuleHandleA("client.dll"), "A3 ? ? ? ? 57 8B CB");
		static bool set = true;
		std::vector<const char*> vistasmoke_wireframe =
		{
			"particle/vistasmokev1/vistasmokev1_smokegrenade",
		};

		std::vector<const char*> vistasmoke_nodraw =
		{
			"particle/vistasmokev1/vistasmokev1_fire",
			"particle/vistasmokev1/vistasmokev1_emods",
			"particle/vistasmokev1/vistasmokev1_emods_impactdust",
		};

		if (!g_Misc->options.nosmoke)
		{
			if (set)
			{
				for (auto material_name : vistasmoke_wireframe)
				{
					IMaterial* mat = g_MatSystem->FindMaterial(material_name, TEXTURE_GROUP_OTHER);
					mat->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, false);
				}
				for (auto material_name : vistasmoke_nodraw) 
				{
					IMaterial* mat = g_MatSystem->FindMaterial(material_name, TEXTURE_GROUP_OTHER);
					mat->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, false);
				}
				set = false;
			}
			return;
		}

		set = true;

		for (auto mat_s : vistasmoke_wireframe)
		{
			IMaterial* mat = g_MatSystem->FindMaterial(mat_s, TEXTURE_GROUP_OTHER);
			mat->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, true);
		}

		for (auto mat_n : vistasmoke_nodraw)
		{
			IMaterial* mat = g_MatSystem->FindMaterial(mat_n, TEXTURE_GROUP_OTHER);
			mat->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
		}

		static auto smokecout = *(DWORD*)(linegoesthrusmoke + 0x1);
		*(int*)(smokecout) = 0;
		
	}
}