#include "Chams.h"
#include "offsets.h"
#include "SDK.h"
#include "Interfaces.h"


void InitKeyValues(KeyValues* keyValues, char* name)
{
	DWORD dwFunction = (DWORD)Offsets::Functions::KeyValues_KeyValues;
	__asm
	{
		push name
		mov ecx, keyValues
		call dwFunction
	}
}

void LoadFromBuffer(KeyValues* keyValues, char const* resourceName, const char* pBuffer)
{
	DWORD dwFunction = (DWORD)Offsets::Functions::KeyValues_LoadFromBuffer;

	__asm
	{
		push 0
		push 0
		push 0
		push pBuffer
		push resourceName
		mov ecx, keyValues
		call dwFunction
	}
}

/* "$basetexture" "vgui/white_additive"
  "$ignorez"      "0"
  "$envmap"       "env_cubemap"
  "$nofog"        "1"
  "$model"        "1"
  "$nocull"       "0"
  "$ambientocclusion"       "1"
  "$normalmapalphaenvmapmask"       "1"
  "$phong"       "1"
  "$phongboost"       "3"
  "$selfillum"    "1"
  "$halflambert"  "1"
	"$rimlight"               "1"
	"$rimlightexponent"       "2"
	"$rimlightboost"          ".2"
  "$znearer"      "0"
  "$flat"         "1"
  "$reflectivity" "[.89 .89 .89]"*/
IMaterial* CreateMaterialLit()
{
	static int created = 0;

	//i have no fucking idea why this keeps being flat

	static const char tmp[] =
	{
		"\"%s\"\
		\n{\
		\n\t\"$basetexture\" \"vgui/white_additive\"\
		\n\t\"$envmap\" \"\"\
		\n\t\"$model\" \"1\"\
		\n\t\"$nocull\" \"0\"\
		\n\t\"$ambientocclusion\" \"1\"\
		\n\t\"$selfillum\" \"1\"\
		\n\t\"$halflambert\" \"1\"\
        \n\t\"$flat\" \"0\"\
		\n\t\"$nofog\" \"1\"\
		\n\t\"$phong\" \"1\"\
		\n\t\"$phongboost\" \"1\"\
		\n\t\"$reflectivity\" \"[.89 .89 .89]\"\
		\n\t\"$ignorez\" \"%i\"\
		\n\t\"$znearer\" \"0\"\
		\n\t\"$wireframe\" \"%i\"\
        \n}\n"
	};

	char* baseType = "VertexLitGeneric";
	char material[512];
	sprintf_s(material, sizeof(material), tmp, baseType, 0, 0);

	char name[512];
	sprintf_s(name, sizeof(name), "#skeetus%i.vmt", created);
	++created;

	KeyValues* keyValues = (KeyValues*)malloc(sizeof(KeyValues));
	InitKeyValues(keyValues, baseType);
	LoadFromBuffer(keyValues, name, material);

	IMaterial* createdMaterial = interfaces::materialsystem->CreateMaterial(name, keyValues);
	createdMaterial->IncrementReferenceCount();

	return createdMaterial;
}

IMaterial* CreateMaterialIgnorez()
{
	static int created = 0;

	//i have no fucking idea why this keeps being flat

	static const char tmp[] =
	{
		"\"%s\"\
		\n{\
		\n\t\"$basetexture\" \"vgui/white_additive\"\
		\n\t\"$envmap\" \"\"\
		\n\t\"$model\" \"1\"\
		\n\t\"$nocull\" \"0\"\
		\n\t\"$ambientocclusion\" \"1\"\
		\n\t\"$selfillum\" \"1\"\
		\n\t\"$halflambert\" \"1\"\
        \n\t\"$flat\" \"0\"\
		\n\t\"$nofog\" \"1\"\
		\n\t\"$phong\" \"1\"\
		\n\t\"$phongboost\" \"1\"\
		\n\t\"$reflectivity\" \"[.89 .89 .89]\"\
		\n\t\"$ignorez\" \"%i\"\
		\n\t\"$znearer\" \"0\"\
		\n\t\"$wireframe\" \"%i\"\
        \n}\n"
	};

	char* baseType = "VertexLitGeneric";
	char material[512];
	sprintf_s(material, sizeof(material), tmp, baseType, 1, 0);

	char name[512];
	sprintf_s(name, sizeof(name), "#skootus_%i.vmt", created);
	++created;

	KeyValues* keyValues = (KeyValues*)malloc(sizeof(KeyValues));
	InitKeyValues(keyValues, baseType);
	LoadFromBuffer(keyValues, name, material);

	IMaterial* createdMaterial = interfaces::materialsystem->CreateMaterial(name, keyValues);
	createdMaterial->IncrementReferenceCount();

	return createdMaterial;
}

void ForceMaterial(Color color, IMaterial* material, bool useColor, bool forceMaterial)
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
		interfaces::render_view->SetBlend(1.0f);
		interfaces::render_view->SetColorModulation(temp);
	}
	if (forceMaterial)
		interfaces::model_render->ForcedMaterialOverride(material);
	else
		interfaces::model_render->ForcedMaterialOverride(NULL);
}

IMaterial* metalic(bool shouldIgnoreZ, bool isLit, bool isWireframe)
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

	char* baseType = (isLit == true ? "VertexLitGeneric" : "UnlitGeneric");
	char material[512];
	sprintf_s(material, sizeof(material), tmp, baseType, (shouldIgnoreZ) ? 1 : 0, (isWireframe) ? 1 : 0);

	char name[512];
	sprintf_s(name, sizeof(name), "#avoz_%i.vmt", created);
	++created;

	KeyValues * keyValues = (KeyValues*)malloc(sizeof(KeyValues));
	InitKeyValues(keyValues, baseType);
	LoadFromBuffer(keyValues, name, material);

	IMaterial * createdMaterial = interfaces::materialsystem->CreateMaterial(name, keyValues);
	createdMaterial->IncrementReferenceCount();

	return createdMaterial;
}

IMaterial * CreateMaterial(bool shouldIgnoreZ, bool isLit, bool isWireframe)
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

	char* baseType = (isLit == true ? "VertexLitGeneric" : "UnlitGeneric");
	char material[512];
	sprintf_s(material, sizeof(material), tmp, baseType, (shouldIgnoreZ) ? 1 : 0, (isWireframe) ? 1 : 0);

	char name[512];
	sprintf_s(name, sizeof(name), "#avoz_%i.vmt", created);
	++created;

	KeyValues * keyValues = (KeyValues*)malloc(sizeof(KeyValues));
	InitKeyValues(keyValues, baseType);
	LoadFromBuffer(keyValues, name, material);

	IMaterial * createdMaterial = interfaces::materialsystem->CreateMaterial(name, keyValues);
	createdMaterial->IncrementReferenceCount();

	return createdMaterial;
}
Chams * chams;
void Chams::override_material(bool ignoreZ, bool flat, bool wireframe, const Color & rgba)
{
	static IMaterial* materialIGNOREZ = CreateMaterialIgnorez();
	static IMaterial* CoveredFlatIgnore = CreateMaterial(true, false, false);
	static IMaterial* CoveredFlat = CreateMaterial(false, false, false);
	static IMaterial* CoveredLit = CreateMaterial(true);
	IMaterial* material = nullptr;

	if (flat) {
		if (ignoreZ)
			material = CoveredFlatIgnore;
		else
			material = CoveredFlat;
	}
	else {
		if (ignoreZ)
			material = materialIGNOREZ;
		else
			material = CoveredLit;
	}
	material->SetMaterialVarFlag(MATERIAL_VAR_WIREFRAME, wireframe);
	material->ColorModulate(
		rgba.r() / 255.0f,
		rgba.g() / 255.0f,
		rgba.b() / 255.0f);

	interfaces::model_render->ForcedMaterialOverride(material);
}

























































































































































































