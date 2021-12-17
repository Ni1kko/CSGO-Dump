#include "gameevents.h"

FireEventFn oFireEvent;

void __fastcall HOOKED_FireEvent( void* ecx, void* edx ) //59
{
	CBaseEntity* pLocal = g_pEntList->GetClientEntity( g_pEngine->GetLocalPlayer() );

	if( pLocal == nullptr || pLocal->GetHealth() <= 0 || !g_pEngine->IsInGame() )
		return oFireEvent( ecx );

	__try
	{
		g_Misc.PreserverFeed(pLocal);
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		g_pICvar->ConsoleColorPrintf(Color::Red(), "[KIDUAHOOK] PreserverFeed exception!\n");
	}

	//https://github.com/VSES/SourceEngine2007/blob/43a5c90a5ada1e69ca044595383be67f40b33c61/se2007/engine/cl_parse_event.cpp#L75
	auto cur_event = *reinterpret_cast< uintptr_t* >( reinterpret_cast< uintptr_t >( g_pClientState ) + 0x4DEC );

	// loop events
	while( cur_event )
	{
		// because inside event system, classID 0 is used to identify a non existing event, hence every real classID is shifted by 1
		uint16_t class_id = *reinterpret_cast< uint16_t* >( cur_event ) - 1;

		// remove delay
		*reinterpret_cast< float* >( cur_event + 0x4 ) = 0.f;

		// check next
		cur_event = *reinterpret_cast< uintptr_t* >( cur_event + 0x38 );
	}

	return oFireEvent( ecx );
}

std::string GetHitgroups( int group )
{
	switch( group )
	{
	case 1:
		return "Head";
		break;
	case 2:
	case 3:
		return "Body";
		break;
	case 4:
	case 5:
		return "Arm";
		break;
	case 6:
	case 7:
		return "Leg";
		break;
	}
	return "Body";
}

FireEventClientSide_t oFireEventClientSide;

bool __fastcall HOOKED_FireEventClientSide( void* thisptr, void* edx, IGameEvent* pEvent )
{
	if( !pEvent )
		return oFireEventClientSide( thisptr, pEvent );

	if( pEvent->GetName().empty() )
		return oFireEventClientSide( thisptr, pEvent );

	return oFireEventClientSide( thisptr, pEvent );
}