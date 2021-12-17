#include "includes.h"
#include <ShlObj.h>

namespace Globals
{
	char documentsFolder[ MAX_PATH ];
	CUserCmd* oCmd;
	bool bSendPacket;
	int chokedPackets;
	matrix3x4_t fakeLagMatrix[ 128 ];
	Vector3 fakeLagOrigin;
	CBaseEntity* localPlayer;
	float autowallDmg;
	Vector3 originalViewAngle;
	Vector3 realAngle;
	Vector3 aimbotHitbox[ 65 ][ 28 ];
	Vector3 enemyEyeAngles[ 65 ];
	int targetIndex = -1;
	bool Shot[ 65 ];
	bool Hit[ 65 ];
	int MissedShots[65];
	bool inThirdperson;
	bool breakingLC;
	Vector3 fakeAngle;
	bool desyncing;
	bool inFakeDuck;
	bool isSlowWalking;
	matrix3x4_t aimbotHitMatrix[ 128 ];
	std::vector<EventInfo> eventInfo;

	bool fakeLagging;
	bool disableAA;
	Vector3 fakePosition;
	bool desyncEnabledSpecialChecks;

	// Here goes nothing..
	std::deque<Vector3> dequeBulletImpacts;

	bool bPlayerHurt[ 64 ]; //when player_hurt called
	bool bBulletImpact[ 64 ]; //when bullet_impact called
	bool bWeaponFire[ 64 ]; //when weapon_fire called
	bool shouldDormant = true;

	using msg_t = void( __cdecl* )( const char*, ... );
	msg_t Msg = reinterpret_cast< msg_t >( GetProcAddress( GetModuleHandleA( "tier0.dll" ), "Msg" ) );
}

std::vector<std::string> &split( const std::string &s, const std::string& delims, std::vector<std::string> &elems ) {
	char* tok;
	char* cchars = new char[ s.size( ) + 1 ];
	char* cstr = &cchars[ 0 ];
	strcpy( cstr, s.c_str( ) );
	tok = strtok( cstr, delims.c_str( ) );
	while ( tok != NULL ) {
		elems.push_back( tok );
		tok = strtok( NULL, delims.c_str( ) );
	}
	return elems;
}

std::vector<std::string> split( const std::string &s, const std::string& delims ) {
	std::vector<std::string> elems;
	return split( s, delims, elems );
}

bool validateHeader( std::string& response ) {
	if ( response.length( ) < 5 )
		return false;

	if ( response[ 0 ] == 'Z' ^ 66 &&
		response[ 1 ] == 'A' ^ 18 &&
		response[ 2 ] == 'R' ^ 12 &&
		response[ 3 ] == 'O' ^ 48 &&
		response[ 4 ] == 'S' ^ 22 )
	{
		response.erase( response.begin( ), response.begin( ) + 5 );
		return true;
	}

	return false;
}

/*bool c_config::save(std::string file_name) {
	CreateDirectory("C:\\dopamine\\configs\\", NULL); CreateDirectory("C:\dopamine\\configs\\", NULL);
	std::ofstream ofs("C:\\dopamine\\configs\\" + file_name + ".cfg");
}*/

/*bool c_config::load(std::string file_name)
{
	CreateDirectory("C:\\dopamine\\configs", NULL);

	std::string file_path = "C:\\dopamine\\configs\\" + file_name + ".cfg";

	std::fstream file;
	file.open(file_path, std::ios::out | std::ios::in);
	if (!file.is_open())
	{
		file.close();
		return false;
	}

	std::string line;
	while (file)
	{
		std::getline(file, line);

		const size_t settings_size = sizeof(c_config);
		if (line.size() > settings_size * 8)
		{
			file.close();
			return false;
		}
		for (int i = 0; i < settings_size; i++)
		{
			byte current_byte = *reinterpret_cast<byte*>(uintptr_t(this) + i);
			for (int x = 0; x < 8; x++)
			{
				if (line[(i * 8) + x] == '1')
					current_byte |= 1 << x;
				else
					current_byte &= ~(1 << x);
			}
			*reinterpret_cast<byte*>(uintptr_t(this) + i) = current_byte;
		}
	}

	file.close();

	return true;
}*/

void c_config::save(const std::string& file_name) {
	std::stringstream config_stream;

	for (auto& [k, v] : config_map) {
		config_stream << k << "=" << v << ";";
	}

	//long res = SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, Globals::documentsFolder);

	//if (res == S_OK)
	{
		auto mmeme = fs::path("C:\\dopamine\\configs") / file_name;

		std::ofstream file_stream(mmeme, std::ios::trunc | std::ios::binary);

		file_stream << uint8_t('Z' ^ 66) << uint8_t('A' ^ 18) << uint8_t('R' ^ 12)
			<< uint8_t('O' ^ 48) << uint8_t('S' ^ 22);

		file_stream << config_stream.str();
		file_stream.close();
	}
}

void c_config::load(const std::string& file_name) {
	//long res = SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, Globals::documentsFolder);

	//if (res == S_OK)
	{
		if (fs::exists(fs::path("C:\\dopamine\\configs") / file_name)) {
			std::string loaderConfig;
			std::ifstream(fs::path("C:\\dopamine\\configs") / file_name, std::ios::binary) >> loaderConfig;

			if (validateHeader(loaderConfig)) {
				config_map.clear();

				auto config_items = split(loaderConfig, ";");
				for (auto item : config_items) {
					auto a = split(item, "=");
					//config_map.at( a[ 0 ] ) = a[ 1 ];
					config_map.insert({ a[0],  a[1] });
				}
			}
		}
	}
}

void c_config::set( const std::string &key, bool value ) {
	this->set( key, value ? 1 : 0 );
}

void c_config::set( const std::string &key, int value ) {
	this->config_map[ key ] = std::to_string( value );
}

void c_config::set( const std::string &key, float value ) {
	this->config_map[ key ] = std::to_string( value );
}

void c_config::set( const std::string &key, double value ) {
	this->config_map[ key ] = std::to_string( value );
}

void c_config::set( const std::string &key, const std::string &value ) {
	this->config_map[ key ] = value;
}

void c_config::set( const std::string &key, const Color &value ) {
	this->set( key, ( int )D3DCOLOR_ARGB( value.a( ), value.r( ), value.g( ), value.b( ) ) );
}

void c_config::set( const std::string &key, const Vector2 &value ) {
	this->config_map[ key + "_x" ] = value.x;
	this->config_map[ key + "_y" ] = value.y;
}

void c_config::set( const std::string &key, const Vector3 &value ) {
	this->config_map[ key + "_x" ] = value.x;
	this->config_map[ key + "_y" ] = value.y;
	this->config_map[ key + "_z" ] = value.z;
}

bool c_config::get_bool( const std::string &key, bool default_value ) {
	if ( this->config_map.find( key ) == this->config_map.end( ) ) {
		this->set( key, default_value );
		return default_value;
	}

	return this->get_int( key ) == 1;
}

int c_config::get_int( const std::string &key, int default_value ) {
	if ( this->config_map.find( key ) == this->config_map.end( ) ) {
		this->set( key, default_value );
		return default_value;
	}

	return std::stoi( this->config_map[ key ] );
}

float c_config::get_float( const std::string &key, float default_value ) {
	if ( this->config_map.find( key ) == this->config_map.end( ) ) {
		this->set( key, default_value );
		return default_value;
	}

	return std::stof( this->config_map[ key ] );
}

double c_config::get_double( const std::string &key, double default_value ) {
	if ( this->config_map.find( key ) == this->config_map.end( ) ) {
		this->set( key, default_value );
		return default_value;
	}

	return std::stod( this->config_map[ key ] );
}

std::string c_config::get_string( const std::string &key, const std::string &default_value ) {
	if ( this->config_map.find( key ) == this->config_map.end( ) ) {
		this->set( key, default_value );
		return default_value;
	}

	return this->config_map[ key ];
}

Color c_config::get_color( const std::string &key, const Color &default_value ) {
	if ( this->config_map.find( key ) == this->config_map.end( ) ) {
		this->set( key, default_value );
		return default_value;
	}

	int color_int = std::stoi( this->config_map[ key ] );
	Color result;

	result[ 0 ] = ( color_int >> 16 ) & 255;
	result[ 1 ] = ( color_int >> 8 ) & 255;
	result[ 2 ] = color_int & 255;
	result[ 3 ] = ( color_int >> 24 ) & 255;

	return result;
}

Vector2 c_config::get_vector2( const std::string &key, const Vector2 &default_value ) {
	if ( this->config_map.find( key ) == this->config_map.end( ) ) {
		this->set( key, default_value );
		return default_value;
	}

	return Vector2(
		this->get_float( key + "_x" ),
		this->get_float( key + "_y" )
	);
}

Vector3 c_config::get_vector3( const std::string &key, const Vector3 &default_value ) {
	if ( this->config_map.find( key ) == this->config_map.end( ) ) {
		this->set( key, default_value );
		return default_value;
	}

	return Vector3(
		this->get_float( key + "_x" ),
		this->get_float( key + "_y" ),
		this->get_float( key + "_z" )
	);
}

c_config *config = new c_config( );