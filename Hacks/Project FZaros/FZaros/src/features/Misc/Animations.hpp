#pragma once
#include "../../utilities/includes.h"
#include <array>

class CBoneBitList;
class CIKContext;

namespace Animations {
	void __fastcall DoExtraBonesProcessing( uintptr_t ecx, uintptr_t edx, CStudioHdr *hdr, Vector3 *pos, Quaternion *q, matrix3x4_t *matrix,
		CBoneBitList &bone_list, CIKContext *context );
	
	namespace originalFn {
		using DoExtraBonesProcessing_t = void( __thiscall * )( uintptr_t, CStudioHdr *, Vector3 *, Quaternion *, matrix3x4_t *,
			CBoneBitList &, CIKContext * );
	}
}

struct Container {
	std::unique_ptr< CVMTHook > m_vmt;
	int m_index;
	bool m_hooked;
};

class CAnimations : public IClientEntityListener {
public:
	std::array< Container, 64 > m_track;
	void init( );
	void remove( );
	void OnEntityCreated( CBaseEntity * ent );
	void OnEntityDeleted( CBaseEntity * ent );
};

extern CAnimations g_Animations;