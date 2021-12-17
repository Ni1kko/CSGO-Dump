#include "utilities.hpp"

CUtils* g_Utils;

bool CUtils::WorldToScreen( const Vector3 &origin, Vector2 &screen )
{
	const auto screenTransform = [ &origin, &screen ]( ) -> bool
	{
		static std::uintptr_t pViewMatrix;
		if ( !pViewMatrix )
		{
			pViewMatrix = static_cast< std::uintptr_t >( g_Memory->findPattern( "client.dll", "0F 10 05 ? ? ? ? 8D 85 ? ? ? ? B9" ) );
			pViewMatrix += 3;
			pViewMatrix = *reinterpret_cast< std::uintptr_t* >( pViewMatrix );
			pViewMatrix += 176;
		}

		const VMatrix& w2sMatrix = *reinterpret_cast< VMatrix* >( pViewMatrix );
		screen.x = w2sMatrix.m[ 0 ][ 0 ] * origin.x + w2sMatrix.m[ 0 ][ 1 ] * origin.y + w2sMatrix.m[ 0 ][ 2 ] * origin.z + w2sMatrix.m[ 0 ][ 3 ];
		screen.y = w2sMatrix.m[ 1 ][ 0 ] * origin.x + w2sMatrix.m[ 1 ][ 1 ] * origin.y + w2sMatrix.m[ 1 ][ 2 ] * origin.z + w2sMatrix.m[ 1 ][ 3 ];

		float w = w2sMatrix.m[ 3 ][ 0 ] * origin.x + w2sMatrix.m[ 3 ][ 1 ] * origin.y + w2sMatrix.m[ 3 ][ 2 ] * origin.z + w2sMatrix.m[ 3 ][ 3 ];

		if ( w < 0.001f )
		{
			screen.x *= 100000;
			screen.y *= 100000;
			return true;
		}

		float invw = 1.f / w;
		screen.x *= invw;
		screen.y *= invw;

		return false;
	};

	if ( !screenTransform( ) )
	{
		int iScreenWidth, iScreenHeight;
		g_Interfaces->gameEngine->getScreenSize( iScreenWidth, iScreenHeight );

		screen.x = ( iScreenWidth * 0.5f ) + ( screen.x * iScreenWidth ) * 0.5f;
		screen.y = ( iScreenHeight * 0.5f ) - ( screen.y * iScreenHeight ) * 0.5f;

		return true;
	}
	return false;
}

Vector3 CUtils::bonePosition( CBaseEntity* pPlayer, int Bone, matrix3x4_t MatrixArray[ 128 ] )
{
	Vector3 pos = Vector3( );

	matrix3x4_t & HitboxMatrix = MatrixArray[ Bone ];

	pos = Vector3( HitboxMatrix[ 0 ][ 3 ], HitboxMatrix[ 1 ][ 3 ], HitboxMatrix[ 2 ][ 3 ] );

	return pos;
}

const wchar_t * CUtils::toWchar( const char * string )
{
	va_list va_alist;
	char buf[ 1024 ];
	va_start( va_alist, string );
	_vsnprintf_s( buf, sizeof( buf ), string, va_alist );
	va_end( va_alist );
	wchar_t wbuf[ 1024 ];
	MultiByteToWideChar( CP_UTF8, 0, buf, 256, wbuf, 256 );

	return wbuf;
}

void CUtils::loadNamedSky( const char* skyName )
{
	static auto fnLoadNamedSkys = ( void( __fastcall* )( const char* ) )g_Memory->findPattern( "engine.dll", "55 8B EC 81 EC ? ? ? ? 56 57 8B F9 C7 45" );

	fnLoadNamedSkys( skyName );
}
