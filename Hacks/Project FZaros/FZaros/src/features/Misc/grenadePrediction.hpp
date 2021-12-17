#pragma once 
#include "../../utilities/includes.h" 
#include <array> 

class CNadePoint {
public:
	CNadePoint( ) {
		m_valid = false;
	}

	CNadePoint( Vector3 start, Vector3 end, bool plane, bool valid, Vector3 normal, bool detonate ) {
		m_start = start;
		m_end = end;
		m_plane = plane;
		m_valid = valid;
		m_normal = normal;
		m_detonate = detonate;
	}

	Vector3 m_start, m_end, m_normal;
	bool m_valid, m_plane, m_detonate;
};

enum ENadeThrowAct {
	ACT_NONE,
	ACT_THROW,
	ACT_LOB,
	ACT_DROP
};

class CGrenadePrediction {
	std::array< CNadePoint, 500 >	_points{ };
	bool		 _predicted = false;
	void predict( CUserCmd *ucmd );
	bool detonated( CBaseCombatWeapon *weapon, float time, CGameTrace &trace );
public:
	void think( CUserCmd *ucmd );
	void draw( );
};

extern CGrenadePrediction g_GrenadePred;