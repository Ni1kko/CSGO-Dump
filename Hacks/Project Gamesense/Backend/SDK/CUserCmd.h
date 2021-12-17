#pragma once
#include "../Include/GlobalIncludes.h"
#include "Checksum_CRC.h"
#include "vector3d.h"

class CUserCmd
{
	public:
		CUserCmd() {}

		CRC32_t GetChecksum( void )
		{
			CRC32_t crc;
			gCRC.CRC32_Init( &crc );
			gCRC.CRC32_ProcessBuffer( &crc, &command_number, sizeof( command_number ) );
			gCRC.CRC32_ProcessBuffer( &crc, &tick_count, sizeof( tick_count ) );
			gCRC.CRC32_ProcessBuffer( &crc, &viewangles, sizeof( viewangles ) );
			gCRC.CRC32_ProcessBuffer( &crc, &aimdirection, sizeof( aimdirection ) );
			gCRC.CRC32_ProcessBuffer( &crc, &forwardmove, sizeof( forwardmove ) );
			gCRC.CRC32_ProcessBuffer( &crc, &sidemove, sizeof( sidemove ) );
			gCRC.CRC32_ProcessBuffer( &crc, &upmove, sizeof( upmove ) );
			gCRC.CRC32_ProcessBuffer( &crc, &buttons, sizeof( buttons ) );
			gCRC.CRC32_ProcessBuffer( &crc, &impulse, sizeof( impulse ) );
			gCRC.CRC32_ProcessBuffer( &crc, &weaponselect, sizeof( weaponselect ) );
			gCRC.CRC32_ProcessBuffer( &crc, &weaponsubtype, sizeof( weaponsubtype ) );
			gCRC.CRC32_ProcessBuffer( &crc, &random_seed, sizeof( random_seed ) );
			gCRC.CRC32_ProcessBuffer( &crc, &mousedx, sizeof( mousedx ) );
			gCRC.CRC32_ProcessBuffer( &crc, &mousedy, sizeof( mousedy ) );
			gCRC.CRC32_Final( &crc );
			return crc;
		}

		char pad_0x0000[0x4];
		int		command_number;
		int		tick_count;
		Vector	viewangles;
		Vector	aimdirection;
		float	forwardmove;
		float	sidemove;
		float	upmove;
		int		buttons;
		byte    impulse;
		int		weaponselect;
		int		weaponsubtype;
		int		random_seed;
		short	mousedx;
		short	mousedy;
		bool	hasbeenpredicted;
		char	pad_0x4C[0x18];
};

class CVerifiedUserCmd
{
	public:
		CUserCmd	m_cmd;
		unsigned long	m_crc;
};
