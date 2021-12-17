
#include "nosmoke.hpp"
#include "../options.hpp"
//#include "ConfigSystem.h"


void NoSmoke::OnFrameStageNotify()
{
	static bool set = true;
	static std::vector<const char*> smoke_materials =
	{
		"particle/vistasmokev1/vistasmokev1_fire",
		"particle/vistasmokev1/vistasmokev1_smokegrenade",
		"particle/vistasmokev1/vistasmokev1_emods",
		"particle/vistasmokev1/vistasmokev1_emods_impactdust",
	};

	if (!g_Options.other_no_smoke)
	{
		if (set)
		{
			for (auto material_name : smoke_materials)
			{
				IMaterial* mat = g_MatSystem->FindMaterial(material_name, TEXTURE_GROUP_OTHER);
				mat->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, false);
			}
			set = false;
		}
		return;
	}

	set = true;
	for (auto material_name : smoke_materials)
	{
		IMaterial* mat = g_MatSystem->FindMaterial(material_name, TEXTURE_GROUP_OTHER);
		mat->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, true);
	}
}
