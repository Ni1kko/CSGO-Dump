#include "Utilities.h"
#include "../SDK/vmatrix.h"
#include "../Interfaces/Interfaces.h"
#include "../SDK/CBaseEntity.h"
#include "../SDK/GlobalDefenitions.h"
Cheat::CUtilities* Cheat::Utilities = new Cheat::CUtilities();

using namespace Cheat;

void CUtilities::Console_Create( std::string title )
{
	AllocConsole();
	AttachConsole( GetCurrentProcessId() );
	freopen_s( &m_pOutputFile, xorstr_("CONOUT$"), xorstr_("w"), stdout );
	m_hOutput = GetStdHandle( STD_OUTPUT_HANDLE );
	SetConsoleTitleA( title.c_str() );
	SetWindowPos( GetConsoleWindow(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
}

void CUtilities::Console_Close()
{
	if ( !m_pOutputFile || !m_hOutput || m_hOutput == INVALID_HANDLE_VALUE )
		return;

	fclose( m_pOutputFile );
	SetStdHandle( STD_OUTPUT_HANDLE, m_hOldOutput );
	FreeConsole();
}

void CUtilities::Console_Log( const char* text, ... )
{
	if ( !text )
		return;

	va_list va_args;
	char buffer[2048] = { 0 };
	va_start( va_args, text );
	_vsnprintf( buffer, sizeof( buffer ), text, va_args );
	va_end( va_args );
	Console_SetColor( ConsoleColor::MAGENTA );
	std::cout << xorstr_("source");
	Console_SetColor( ConsoleColor::DARKGRAY );
	std::cout << xorstr_(": ") << buffer << std::endl;
}

void CUtilities::Console_LogColored( const char* text, ConsoleColor color, ... )
{
	if ( !text )
		return;

	va_list va_args;
	char buffer[2048] = { 0 };
	va_start( va_args, text );
	_vsnprintf( buffer, sizeof( buffer ), text, va_args );
	va_end( va_args );
	Console_SetColor(ConsoleColor::MAGENTA);
	std::cout << xorstr_("source");
	Console_SetColor( ConsoleColor::DARKGRAY );
	std::cout << xorstr_(": ") << buffer << std::endl;
}

void CUtilities::Console_SetTitle( std::string title )
{
	SetConsoleTitleA( title.c_str() );
}

void CUtilities::Console_SetColor( CUtilities::ConsoleColor color )
{
	SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), ( WORD )color );
}

#define in_range(x, a, b) (x >= a && x <= b)
#define get_bits(x) (in_range((x & (~0x20)), 'A', 'F') ? ((x & (~0x20)) - 'A' + 0xA): (in_range(x, '0', '9') ? x - '0': 0))
#define get_byte(x) (get_bits(x[0]) << 4 | get_bits(x[1]))

DWORD CUtilities::Memory_PatternScan( std::string moduleName, std::string pattern ) //more smaller
{
	MODULEINFO modInfo;
	GetModuleInformation( GetCurrentProcess(), GetModuleHandleA( moduleName.c_str() ), &modInfo, sizeof( MODULEINFO ) );
	DWORD startAddress = ( DWORD )modInfo.lpBaseOfDll;
	DWORD endAddress = startAddress + modInfo.SizeOfImage;
	const char* pat = pattern.c_str();
	DWORD firstMatch = 0;

	for ( DWORD pCur = startAddress; pCur < endAddress; pCur++ )
	{
		if ( !*pat )
			return firstMatch;

		if ( *( PBYTE )pat == ( '\?' ) || *( BYTE* )pCur == get_byte( pat ) )
		{
			if ( !firstMatch )
				firstMatch = pCur;

			if ( !pat[2] )
				return firstMatch;

			if ( *( PWORD )pat == ( '\?\?' ) || *( PBYTE )pat != ( '\?' ) )
				pat += 3;

			else
				pat += 2;
		}

		else
		{
			pat = pattern.c_str();
			firstMatch = 0;
		}
	}

	return NULL;
}
inline RECT viewport() {
	RECT viewport = { 0, 0, 0, 0 };

	int width, height;
	Interfaces::Engine->GetScreenSize(width, height);
	viewport.right = width;
	viewport.bottom = height;

	return viewport;
}
bool CUtilities::Game_WorldToScreen( const Vector &world, Vector &screen )
{
	
	auto find_point = [](Vector &point, int screen_w, int screen_h, int degrees) -> void {
		float x2 = screen_w * 0.5f;
		float y2 = screen_h * 0.5f;

		float d = sqrt(pow((point.x - x2), 2) + (pow((point.y - y2), 2))); //Distance
		float r = degrees / d; //Segment ratio

		point.x = r * point.x + (1 - r) * x2; //find point that divides the segment
		point.y = r * point.y + (1 - r) * y2; //into the ratio (1-r):r
	};
	float w = G::matrix[3][0] * world.x + G::matrix[3][1] * world.y + G::matrix[3][2] * world.z + G::matrix[3][3];
	auto
		screen_width = viewport().right,
		screen_height = viewport().bottom;

	float inverse_width = -1.0f / w;
	bool behind = true;

	if (w > 0.01) {
		inverse_width = 1.0f / w;
		behind = false;
	}

	screen.x = (float)((screen_width / 2) + (0.5 * ((G::matrix[0][0] * world.x
		+ G::matrix[0][1] * world.y
		+ G::matrix[0][2] * world.z
		+ G::matrix[0][3]) * inverse_width) * screen_width + 0.5));

	screen.y = (float)((screen_height / 2) - (0.5 * ((G::matrix[1][0] * world.x
		+ G::matrix[1][1] * world.y
		+ G::matrix[1][2] * world.z
		+ G::matrix[1][3]) * inverse_width) * screen_height + 0.5));

	if (screen.x > screen_width || screen.x < 0 || screen.y > screen_height || screen.y < 0 || behind) {
		find_point(screen, screen_width, screen_height, screen_height / 2);
		return false;
	}

	return !(behind);
	return true;

}


bool CUtilities::Game_ScreenTransform( const Vector &point, Vector &screen )
{
	const VMatrix &w2sMatrix = Interfaces::Engine->WorldToScreenMatrix();
	screen.x = w2sMatrix[0][0] * point.x + w2sMatrix[0][1] * point.y + w2sMatrix[0][2] * point.z + w2sMatrix[0][3];
	screen.y = w2sMatrix[1][0] * point.x + w2sMatrix[1][1] * point.y + w2sMatrix[1][2] * point.z + w2sMatrix[1][3];
	screen.z = 0.0f;
	float w = w2sMatrix[3][0] * point.x + w2sMatrix[3][1] * point.y + w2sMatrix[3][2] * point.z + w2sMatrix[3][3];

	if ( w < 0.001f )
	{
		screen.x *= 100000;
		screen.y *= 100000;
		return true;
	}

	float invw = 1.0f / w;
	screen.x *= invw;
	screen.y *= invw;
	return false;
}



void CUtilities::Game_TraceLine( Vector &start, Vector &end, unsigned int mask, CBaseEntity* ignore, trace_t* tr )
{
	Ray_t ray;
	ray.Init( start, end );
	CTraceFilter filter;
	filter.pSkip1 = ignore;
	Interfaces::Trace->TraceRay( ray, mask, &filter, tr );
}


#include "../Features/Visuals/EventLogger.h"
#include "../Configuration/Config.h"

void CUtilities::Game_Msg( const char* msg, ... )
{
	if ( msg == nullptr )
		return;

	char buffer[2048];
	va_list list;
	va_start( list, msg );
	vsprintf( buffer, msg, list );
	va_end( list );
	
	Features::EventLogger->AddLog(std::string(buffer).c_str());
	Interfaces::Var->ConsoleColorPrintf(CColor(159, 202, 43), xorstr_("[shattered] "));
	Interfaces::Var->ConsolePrintf(std::string(buffer + std::string(xorstr_("\n"))).c_str());

	this->Console_Log(buffer);
}




bool CUtilities::IsPointVisible( const Vector &start, const Vector &end, CBaseEntity* player )
{
	Ray_t ray;
	trace_t tr;
	ray.Init( start, end );
	CTraceFilter filter;
	filter.pSkip1 = G::LocalPlayer;
	Interfaces::Trace->TraceRay( ray, MASK_SHOT, &filter, &tr );

	if ( tr.m_pEnt == player || tr.fraction > 0.97f )
		return true;

	return false;
}
inline void clamp_angles( Vector &angles )
{
	if ( angles.x > 89.0f )
		angles.x = 89.0f;

	else
		if ( angles.x < -89.0f )
			angles.x = -89.0f;

	if ( angles.y > 180.0f )
		angles.y = 180.0f;

	else
		if ( angles.y < -180.0f )
			angles.y = -180.0f;

	angles.z = 0;
}
float CUtilities::Math_GetFov(Vector qAngles, Vector vecSource)
{
	Vector delta = qAngles - vecSource;
	clamp_angles(delta);
	return sqrtf(powf(delta.x, 2.0f) + powf(delta.y, 2.0f));
}
FORCEINLINE void vector_subtract(const Vector& a, const Vector& b, Vector& c)
{
	c.x = a.x - b.x;
	c.y = a.y - b.y;
	c.z = a.z - b.z;
}
void normalize(Vector &vIn, Vector &vOut)
{
	float flLen = vIn.Length();
	if (flLen == 0) {
		vOut.Init(0, 0, 1);
		return;
	}
	flLen = 1 / flLen;
	vOut.Init(vIn.x * flLen, vIn.y * flLen, vIn.z * flLen);
}
void angle_vectors(const Vector &angles, Vector& forward)
{
	Assert(s_bMathlibInitialized);
	Assert(forward);

	float	sp, sy, cp, cy;

	sy = sin(DEG2RAD(angles[1]));
	cy = cos(DEG2RAD(angles[1]));

	sp = sin(DEG2RAD(angles[0]));
	cp = cos(DEG2RAD(angles[0]));

	forward.x = cp * cy;
	forward.y = cp * sy;
	forward.z = -sp;
}
#define PI 3.14159265358979323846f
float CUtilities::Math_GetFov2(Vector ViewOffSet, Vector View, CBaseEntity* pEntity, int aHitBox)
{
	// Anything past 180 degrees is just going to wrap around
	CONST FLOAT MaxDegrees = 180.0f;

	// Get local angles
	Vector Angles = View;

	// Get local view / eye position
	Vector Origin = ViewOffSet;

	// Create and intiialize vectors for calculations below
	Vector Delta(0, 0, 0);
	//Vector Origin(0, 0, 0);
	Vector Forward(0, 0, 0);

	// Convert angles to normalized directional forward vector
	angle_vectors(Angles, Forward);

	Vector AimPos = G::LocalPlayer->get_hitbox_pos(pEntity, aHitBox); //pvs fix disabled

	vector_subtract(AimPos, Origin, Delta);
	//Delta = AimPos - Origin;

	// Normalize our delta vector
	normalize(Delta, Delta);

	// Get dot product between delta position and directional forward vectors
	FLOAT DotProduct = Forward.Dot(Delta);

	// Time to calculate the field of view
	return (acos(DotProduct) * (MaxDegrees / PI));
}
float CUtilities::Distance(Vector2D point1, Vector2D point2)
{
	float diffY = point1.y - point2.y;
	float diffX = point1.x - point2.x;
	return sqrt((diffY * diffY) + (diffX * diffX));
}
