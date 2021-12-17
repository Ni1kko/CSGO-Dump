#include "Animations.hpp"

CAnimations g_Animations;

void CAnimations::init( ) {
	g_Interfaces->clientEntity->AddListenerEntity( this );
}

void CAnimations::remove( ) {
	g_Interfaces->clientEntity->RemoveListenerEntity( this );
}

void CAnimations::OnEntityCreated( CBaseEntity *ent ) {
	if ( !ent )
		return;

	int index = ent->EntIndex( );
	if ( index < 0 )
		return;

	CClientClass *cc = ent->GetClientClass( );
	if ( !cc )
		return;

	switch ( cc->iClassID ) {
	case 38: {
		m_track[ index ].m_index = index;
		m_track[ index ].m_vmt = std::make_unique< CVMTHook >( ent );
		m_track[ index ].m_vmt->hook_method( 193, Animations::DoExtraBonesProcessing );
		m_track[ index ].m_hooked = true;

		break;
	}
	default: {
		return;
	}
	}

}

void CAnimations::OnEntityDeleted( CBaseEntity *ent ) {
	if ( !ent )
		return;

	int index = ent->EntIndex( );
	if ( index < 0 )
		return;

	auto it = std::find_if( m_track.begin( ), m_track.end( ), [ & ] ( const Container &data ) {
		return data.m_index == index;
	} );

	if ( it == m_track.end( ) )
		return;

	if ( m_track[ it->m_index ].m_hooked )
		m_track[ it->m_index ].m_vmt->unhook_all( );
}