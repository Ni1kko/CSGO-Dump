#pragma once
#include "SDK.h"

void InitKeyValues(KeyValues* keyValues, char* name);

void LoadFromBuffer(KeyValues* keyValues, char const *resourceName, const char *pBuffer);

void ForceMaterial(Color color, IMaterial* material, bool useColor = true, bool forceMaterial = true);
IMaterial *CreateMaterial(bool shouldIgnoreZ, bool isLit = true, bool isWireframe = false);
IMaterial *CreateMaterialLit();
IMaterial *CreateMaterialIgnorez();

class Chams
{
public:
	IMaterial * materialRegular = nullptr;
	IMaterial* materialRegularIgnoreZ = nullptr;
	IMaterial* materialFlatIgnoreZ = nullptr;
	IMaterial* materialGLOW = nullptr;
	IMaterial* materialFlat = nullptr;

	void override_material(bool ignoreZ, bool flat, bool wireframe, const Color & rgba);

}; extern Chams* chams;