#pragma once

#include "singleton.hpp"

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
	void OverrideMaterial(bool ignoreZ, bool flat, bool wireframe, bool glass, bool metallic, const Color rgba);
	void OverrideMaterial(bool ignoreZ, bool flat, bool wireframe, bool glass, bool metallic);
private:
	IMaterial* materialRegular = nullptr;
	IMaterial* materialRegularIgnoreZ = nullptr;
	IMaterial* materialFlatIgnoreZ = nullptr;
	IMaterial* materialFlat = nullptr;
	IMaterial* materialReflective = nullptr;
	IMaterial* materialReflectiveIgnoreZ = nullptr;
};