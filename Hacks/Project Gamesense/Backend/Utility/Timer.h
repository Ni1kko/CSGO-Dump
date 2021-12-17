#pragma once
#include "../Include/GlobalIncludes.h"

class CTimer
{
	public:
		ULONGLONG dwTime;
		bool bEnable;

		CTimer()
		{
			dwTime = 0;
			bEnable = true;
		}

		bool delay( DWORD dwMsec );
		void reset();
		void stop();
};
