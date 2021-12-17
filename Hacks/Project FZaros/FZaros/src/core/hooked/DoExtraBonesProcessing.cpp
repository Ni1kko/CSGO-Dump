#include "../../features/Misc/Animations.hpp"

void __fastcall Animations::DoExtraBonesProcessing( uintptr_t ecx, uintptr_t edx, CStudioHdr *hdr, Vector3 *pos, Quaternion *q, matrix3x4_t *matrix,
	CBoneBitList &bone_list, CIKContext *context )
{	
	auto player = ( CBaseEntity* )ecx;

	static auto originalFn = g_Animations.m_track[ player->EntIndex( ) ].m_vmt->get_old_method< originalFn::DoExtraBonesProcessing_t >( 195 );

	const auto state = player->animState( );

	if ( !state || !state->pBaseEntity )
		return originalFn( ecx, hdr, pos, q, matrix, bone_list, context );

	const auto bOnGround = state->m_bOnGround;

	state->m_bOnGround = false;

	originalFn( ecx, hdr, pos, q, matrix, bone_list, context );

	state->m_bOnGround = bOnGround;
}