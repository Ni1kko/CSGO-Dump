#pragma once

#include "valve_sdk/csgostructs.hpp"
#include "MaterialManager.h"
#include "singleton.hpp"
#include "IStudioRender.h"
#include <deque>

enum class ChamsModes : int
{
	NORMAL,
	FLAT,
	WIREFRAME,
	GLASS,
	METALLIC,
	XQZ,
	METALLIC_XQZ,
	FLAT_XQZ
};

#pragma once
class Chams : public Singleton<Chams>
{
	friend class Singleton<Chams>;

	  Chams();
	 ~Chams();

public:
	void OnSceneEnd();
	void OverrideMaterial(bool ignoreZ, bool flat, bool wireframe, bool glass, const Color& rgba);
	void OnDrawModelExecute(void* pResults, DrawModelInfo_t* pInfo, matrix3x4_t* pBoneToWorld, float* flpFlexWeights, float* flpFlexDelayedWeights, Vector& vrModelOrigin, int iFlags);
private:

	IMaterial* materialRegular = nullptr;
	IMaterial* materialRegularIgnoreZ = nullptr;
	IMaterial* materialFlatIgnoreZ = nullptr;
	IMaterial* materialFlat = nullptr;
};

