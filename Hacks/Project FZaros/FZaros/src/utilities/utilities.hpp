#pragma once
#include "includes.h"

class CUtils
{
public:

	static bool WorldToScreen(const Vector3 &origin, Vector2 &screen);
	Vector3 bonePosition(CBaseEntity* pPlayer, int Bone, matrix3x4_t MatrixArray[128]);
	const wchar_t* toWchar( const char* string );

	void loadNamedSky( const char* skyName );

}; extern CUtils* g_Utils;