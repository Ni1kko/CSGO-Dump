#pragma once

#include "../other/singleton.hpp"

class IMatRenderContext;
struct DrawModelState_t;
struct ModelRenderInfo_t;
class matrix3x4_t;
class IMaterial;
class Color;

class Chams : public Singleton<Chams> {
    friend class Singleton<Chams>;

    Chams();
    ~Chams();

public:
    void OnDrawModelExecute(
        IMatRenderContext* ctx,
        const DrawModelState_t& state,
        const ModelRenderInfo_t& pInfo,
        matrix3x4_t* pCustomBoneToWorld);

private:
    void OverrideMaterial(bool ignoreZ, bool wireframe, int type, const Color& rgba);

    IMaterial* materialRegular = nullptr;
    IMaterial* materialFlat = nullptr;
    IMaterial* materialGlow = nullptr;

  constexpr auto ChangeMaterial(int material) {
      switch (material) {
      case 0: return materialRegular;
      case 1: return materialFlat;
      case 2: return materialGlow;
      }
  }
};
