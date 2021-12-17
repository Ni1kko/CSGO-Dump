#include "Timer.h"

bool CTimer::delay( DWORD dwMsec )
{
	if ( !bEnable )
		return true;

	if ( !dwTime )
		dwTime = GetTickCount64();

	if ( dwTime + dwMsec < GetTickCount64() )
	{
		//dwTime = 0;
		bEnable = false;
		return true;
	}

	else
	{
		dwTime = 0;
		bEnable = true;
		return false;
	}
}

void CTimer::reset()
{
	dwTime = 0;
	bEnable = true;
}

void CTimer::stop()
{
	bEnable = false;
}
