#pragma once

#include "../singleton.hpp"

#include "../valve_sdk/csgostructs.hpp"

class Chams
  : public Singleton<Chams>
{
  friend class Singleton<Chams>;

  Chams( );
  ~Chams( );

public:
  void OnDrawModelExecute( void* pResults, DrawModelInfo_t* pInfo, matrix3x4_t* pBoneToWorld, float* flpFlexWeights,
	 float* flpFlexDelayedWeights, Vector& vrModelOrigin, int iFlags );

private:
  void OverrideMaterial( bool ignoreZ, bool flat, bool wireframe, bool glass, const Color& rgba );

  IMaterial* materialRegular = nullptr;
  IMaterial* materialRegularIgnoreZ = nullptr;
  IMaterial* materialFlatIgnoreZ = nullptr;
  IMaterial* materialFlat = nullptr;
};
