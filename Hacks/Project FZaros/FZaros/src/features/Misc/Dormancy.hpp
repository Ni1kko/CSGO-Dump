#pragma once
#include "../../utilities/includes.h"
#include "../../sdk/interfaces/engineSound.hpp"

struct dormancyData
{
	CBaseEntity* entity;
	bool	prevstate;
	bool	set;
	int		flags;
	Vector3	oldpos = Vector3( 0.f, 0.f, 0.f );
	struct {
		int time;
	} sounddata;
};

class dormancyManager : public Singleton<dormancyManager> {
public:
	void positionCorrection( int stage );
	bool set( const int index );

	bool viable( CBaseEntity* entity );

	void soundDormancy( );
	void adjustPlayer( const int index );
	void restoreSoundDormancy( );
	bool validSound( soundInfo& sound );
private:
	CUtlVector<soundInfo> soundBuffer;
	CUtlVector<soundInfo> soundList;

	dormancyData players[ 65 ];
};