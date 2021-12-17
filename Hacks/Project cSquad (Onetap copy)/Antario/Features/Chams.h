#pragma once
#include "..\Settings.h"
#include "..\Singleton.h"
#include "..\SDK\Materials.h"
#include "fstream"

class IMatRenderContext;
struct DrawModelState_t;
struct ModelRenderInfo_t;
class matrix3x4_t;
class IMaterial;
class Color;

class MaterialManager : public Singleton<MaterialManager>
{
public:
	MaterialManager();
	~MaterialManager();
	void OverrideMaterial(bool ignoreZ, bool flat, bool wireframe, bool glass, bool metallic, bool onetap, const Color rgba);
	void OverrideMaterial(bool ignoreZ, bool flat, bool wireframe, bool glass, bool metallic, bool onetapsu);
private:
	IMaterial* materialRegular = nullptr;
	IMaterial* materialRegularIgnoreZ = nullptr;
	IMaterial* materialFlatIgnoreZ = nullptr;
	IMaterial* materialFlat = nullptr;
	IMaterial* materialReflective = nullptr;
	IMaterial* materialReflectiveIgnoreZ = nullptr;
	IMaterial* materialOneTapSu = nullptr;
};