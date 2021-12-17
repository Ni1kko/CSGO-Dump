#include "Chams.h"

MaterialManager::MaterialManager()
{
	std::ofstream("csgo\\materials\\simple_regular.vmt") << R"#("VertexLitGeneric"
{
  "$basetexture" "vgui/white_additive"
  "$ignorez"      "0"
  "$envmap"       ""
  "$nofog"        "1"
  "$model"        "1"
  "$nocull"       "0"
  "$selfillum"    "1"
  "$halflambert"  "1"
  "$znearer"      "0"
  "$flat"         "1"
}
)#";
	std::ofstream("csgo\\materials\\simple_reflective.vmt") << R"#("VertexLitGeneric"
{
  "$basetexture" "vgui/white_additive"
  "$envmap"       "env_cubemap"
  "$model" "1"
  "$normalmapalphaenvmapmask"  "1"
  "$envmapcontrast" "1"
  "$flat" "1"
  "$nocull" "0"
  "$selfillum" "1"
  "$halflambert" "1"
  "$nofog" "0"
  "$ignorez" "0"
  "$znearer" "0"
  "$wireframe" "0"
}
)#";
	std::ofstream("csgo\\materials\\simple_reflectiveignorez.vmt") << R"#("VertexLitGeneric"
{
  "$basetexture" "vgui/white_additive"
  "$envmap"       "env_cubemap"
  "$model" "1"
  "$normalmapalphaenvmapmask"  "1"
  "$envmapcontrast" "1"
  "$flat" "1"
  "$nocull" "0"
  "$selfillum" "1"
  "$halflambert" "1"
  "$nofog" "0"
  "$ignorez" "1"
  "$znearer" "0"
  "$wireframe" "0"
}
)#";
	std::ofstream("csgo\\materials\\simple_ignorez.vmt") << R"#("VertexLitGeneric"
{
  "$basetexture" "vgui/white_additive"
  "$ignorez"      "1"
  "$envmap"       ""
  "$nofog"        "1"
  "$model"        "1"
  "$nocull"       "0"
  "$selfillum"    "1"
  "$halflambert"  "1"
  "$znearer"      "0"
  "$flat"         "1"
}
)#";
	std::ofstream("csgo\\materials\\simple_flat.vmt") << R"#("UnlitGeneric"
{
  "$basetexture" "vgui/white_additive"
  "$ignorez"      "0"
  "$envmap"       ""
  "$nofog"        "1"
  "$model"        "1"
  "$nocull"       "0"
  "$selfillum"    "1"
  "$halflambert"  "1"
  "$znearer"      "0"
  "$flat"         "1"
}
)#";
	std::ofstream("csgo\\materials\\simple_flat_ignorez.vmt") << R"#("UnlitGeneric"
{
  "$basetexture" "vgui/white_additive"
  "$ignorez"      "1"
  "$envmap"       ""
  "$nofog"        "1"
  "$model"        "1"
  "$nocull"       "0"
  "$selfillum"    "1"
  "$halflambert"  "1"
  "$znearer"      "0"
  "$flat"         "1"
}
)#";

	materialRegular = g_pMaterialSystem->FindMaterial("simple_regular", TEXTURE_GROUP_MODEL);
	materialRegularIgnoreZ = g_pMaterialSystem->FindMaterial("simple_ignorez", TEXTURE_GROUP_MODEL);
	materialFlatIgnoreZ = g_pMaterialSystem->FindMaterial("simple_flat_ignorez", TEXTURE_GROUP_MODEL);
	materialFlat = g_pMaterialSystem->FindMaterial("simple_flat", TEXTURE_GROUP_MODEL);
	materialReflective = g_pMaterialSystem->FindMaterial("simple_reflective", TEXTURE_GROUP_MODEL);
	materialReflectiveIgnoreZ = g_pMaterialSystem->FindMaterial("simple_reflectiveignorez", TEXTURE_GROUP_MODEL);
	materialOneTapSu = g_pMaterialSystem->FindMaterial("models/inventory_items/dogtags/dogtags_outline", TEXTURE_GROUP_MODEL);
}

MaterialManager::~MaterialManager()
{
	std::remove("csgo\\materials\\simple_regular.vmt");
	std::remove("csgo\\materials\\simple_ignorez.vmt");
	std::remove("csgo\\materials\\simple_flat.vmt");
	std::remove("csgo\\materials\\simple_flat_ignorez.vmt");
	std::remove("csgo\\materials\\regular_reflective.vmt");
	std::remove("csgo\\materials\\simple_reflective.vmt");
}

void MaterialManager::OverrideMaterial(bool ignoreZ, bool flat, bool wireframe, bool glass, bool metallic, bool onetap, const Color rgba)
{
	IMaterial* material = nullptr;

	if (flat)
	{
		if (ignoreZ)
		{
			material = materialFlatIgnoreZ;
		}
		else
		{
			material = materialFlat;
		}
	}
	else
	{
		if (ignoreZ && !metallic)
		{
			material = materialRegularIgnoreZ;
		}
		else if (ignoreZ && metallic)
		{
			material = materialReflectiveIgnoreZ;
		}
		else if (metallic)
		{
			material = materialReflective;
		}
		else if (onetap)
		{
			material = materialOneTapSu;
		}
		else
		{
			material = materialRegular;
		}
	}


	if (glass)
	{
		material = materialFlat;
		material->AlphaModulate(0.45f);
	}
	else
	{
		material->AlphaModulate(
			rgba.alpha / 255.0f);
	}

	material->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, wireframe);
	material->ColorModulate(
		rgba.red / 255.0f,
		rgba.green / 255.0f,
		rgba.blue / 255.0f);

	g_pModelRender->ForcedMaterialOverride(material);
}

void MaterialManager::OverrideMaterial(bool ignoreZ, bool flat, bool wireframe, bool glass, bool metallic, bool onetapsu)
{
	IMaterial* material = nullptr;

	if (flat)
	{
		if (ignoreZ)
		{
			material = materialFlatIgnoreZ;
		}
		else
		{
			material = materialFlat;
		}
	}
	else
	{
		if (ignoreZ && !metallic && !flat)
		{
			material = materialRegularIgnoreZ;
		}
		else if (ignoreZ && metallic && !flat)
		{
			material = materialReflectiveIgnoreZ;
		}
		else if (metallic)
		{
			material = materialReflective;
		}
		else if (onetapsu)
		{
			material = materialOneTapSu;
		}
		else
		{
			material = materialRegular;
		}
	}


	if (glass)
	{
		material = materialFlat;
		material->AlphaModulate(0.45f);
	}

	material->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, wireframe);

	g_pModelRender->ForcedMaterialOverride(material);
}

MaterialManager * pMatManager;