#include "CMaterialHelper.h"
CMaterialHelper* MaterialHelper = nullptr;

typedef void(__thiscall* orgGameFunc_InitKeyValues)(void* thisptr, const char* name);
orgGameFunc_InitKeyValues pInitKeyValues;

typedef void(__thiscall* orgGameFunc_LoadFromBuffer)(void* thisptr, const char* resourceName, const char* pBuffer, void* pFileSystem, const char* pPathID, void* pfnEvaluateSymbolProc);
orgGameFunc_LoadFromBuffer pLoadFromBuffer;
void CMaterialHelper::InitKeyValues(KeyValues* pKeyValues, const char* name)
{
	if (!pInitKeyValues)
		pInitKeyValues = reinterpret_cast<orgGameFunc_InitKeyValues>(offys.dwInitKeyValues);

	pInitKeyValues(pKeyValues, name);
}

void CMaterialHelper::LoadFromBuffer(KeyValues* pKeyValues, const char* resourceName, const char* pBuffer, void* pFileSystem, const char* pPathID, void* pfnEvaluateSymbolProc)
{
	if (!pLoadFromBuffer)
		pLoadFromBuffer = reinterpret_cast<orgGameFunc_LoadFromBuffer>(offys.dwLoadFromBuffer);

	pLoadFromBuffer(pKeyValues, resourceName, pBuffer, pFileSystem, pPathID, pfnEvaluateSymbolProc);
}

IMaterial* CMaterialHelper::CreateMaterial(bool throughwalls, bool textured, bool wireframe) //credits to ph0ne
{
	static int created = 0;
	static const char tmp[] =
	{
		"\"%s\"\
				\n{\
						\n\t\"$basetexture\" \"vgui/white_additive\"\
								\n\t\"$envmap\" \"\"\
										\n\t\"$model\" \"1\"\
												\n\t\"$flat\" \"1\"\
														\n\t\"$nocull\" \"0\"\
																\n\t\"$selfillum\" \"1\"\
																		\n\t\"$halflambert\" \"1\"\
																				\n\t\"$nofog\" \"0\"\
																						\n\t\"$ignorez\" \"%i\"\
																								\n\t\"$znearer\" \"0\"\
																										\n\t\"$wireframe\" \"%i\"\
																										        \n}\n"
	};
	char* baseType = (textured == true ? ("VertexLitGeneric") : ("UnlitGeneric"));
	char material[512];
	sprintf_s(material, sizeof(material), tmp, baseType, (throughwalls) ? 1 : 0, (wireframe) ? 1 : 0);
	/*  char material[1024];
		sprintf_s(material, sizeof(material), tmp, baseType, envmap, (wireframe) ? 1 : 0, (wireframe) ? 1 : 0, (wireframe) ? 1 : 0, (throughwalls) ? 1 : 0, 0);*/
	char name[512];
	sprintf_s(name, sizeof(name), ("#xuy%i.vmt"), created);
	++created;
	KeyValues* keyValues = new KeyValues();
	InitKeyValues(keyValues, baseType);
	LoadFromBuffer(keyValues, name, material, nullptr, NULL, nullptr);
	IMaterial* createdMaterial = Interfaces::MaterialSystem->CreateMaterial(name, keyValues);
	createdMaterial->IncrementReferenceCount();
	return createdMaterial;
}
void CMaterialHelper::ForceMaterial(CColor color, IMaterial* material, bool useColor, bool forceMaterial)
{
	if (useColor)
	{
		float temp[3] =
		{
			color.r(),
			color.g(),
			color.b()
		};
		temp[0] /= 255.f;
		temp[1] /= 255.f;
		temp[2] /= 255.f;
		float alpha = color.a();
		Interfaces::RenderView->SetBlend(alpha);
		Interfaces::RenderView->SetColorModulation(temp);
	}

	if (forceMaterial)
		Interfaces::ModelRender->ForcedMaterialOverride(material);

	else
		Interfaces::ModelRender->ForcedMaterialOverride(NULL);
}
CMaterialHelper::CMaterialHelper()
{
	std::ofstream("csgo\\materials\\simple_ignorez_reflective.vmt") << R"#("VertexLitGeneric"
{

  "$basetexture" "vgui/white_additive"
      "$ignorez" "1"
      "$envmap" "env_cubemap"
      "$normalmapalphaenvmapmask" "1"
      "$envmapcontrast"  "0.01"
      "$nofog" "1"
      "$model" "1"
      "$nocull" "0"
      "$selfillum" "1"
      "$halflambert" "1"
      "$znearer" "0"
      "$flat" "1" 
}
)#";

	std::ofstream("csgo\\materials\\simple_regular_reflective.vmt") << R"#("VertexLitGeneric"
{

  "$basetexture" "vgui/white_additive"
      "$ignorez" "0"
      "$envmap" "env_cubemap"
      "$normalmapalphaenvmapmask" "1"
      "$envmapcontrast"  "0.01"
      "$nofog" "1"
      "$model" "1"
      "$nocull" "0"
      "$selfillum" "1"
      "$halflambert" "1"
      "$znearer" "0"
      "$flat" "1" 
}
)#";
	std::ofstream("csgo\\materials\\models\\inventory_items\\trophy_majors\\gloss") << R"#("VertexLitGeneric"
{
	"$additive" "1"
    "$envmap"    "env_cubemap"
    "$envmapcontrast" "1"
    "$envmapsaturation" "1.0"
    "$normalmapalphaenvmask" "1"
}
)#";

	std::ofstream("csgo\\materials\\glowOverlay.vmt") << R"#("VertexLitGeneric" {
 
	"$additive" "1"
	"$envmap" "models/effects/cube_white"
	"$envmapfresnel" "1"
	"$envmapfresnelminmaxexp" "[0 1 2]"
	"$alpha" "0.8"
})#";

	std::ofstream("csgo\\materials\\simple_ignorez_shaded.vmt") << R"#("VertexLitGeneric"
{

  "$basetexture" "vgui/white_additive"
"$bumpmap" "effects\flat_normal"
"$ignorez" "1"
    "$phongtint" "[1 1 1]"
    "$phong" "1"
    "$phongexponent" "15.0"
    "$normalmapalphaenvmask" "1"
    "$phongboost"                "15.0"
      "$nofog" "1"
      "$model" "1"
      "$nocull" "0"
      "$selfillum" "1"
      "$halflambert" "1"
      "$znearer" "0"
      "$flat" "1" 
"$rimlight"               "1"
"$rimlightexponent" "5"
"$rimlightboost"          "5"
}
)#";

	std::ofstream("csgo\\materials\\simple_regular_shaded.vmt") << R"#("VertexLitGeneric"
{

"$basetexture" "vgui/white_additive"
"$bumpmap" "effects\flat_normal"
    "$phongtint" "[1 1 1]"
    "$phong" "1"
"$forcephong" "1"
    "$phongexponent" "15.0"
    "$normalmapalphaenvmask" "1"
    "$phongboost"                "15.0"
      "$nofog" "1"
      "$model" "1"
      "$nocull" "0"
      "$selfillum" "1"
      "$halflambert" "1"
      "$znearer" "0"
      "$flat" "1" 
"$rimlight"               "1"
"$rimlightexponent" "5"
"$rimlightboost"          "5"
}
)#";

	CoveredLit = CreateMaterial(true);
	OpenLit = CreateMaterial(false);
	CoveredFlat = CreateMaterial(true, false);
	OpenFlat = CreateMaterial(false, false);
	CoveredMetallic = CreateMaterial(true, true, true);
	OpenMetallic = CreateMaterial(false, true, true);
}
