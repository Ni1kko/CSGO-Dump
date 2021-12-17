#include "EventLogger.h"
#include "Drawing.h"

#include "../../Include/GlobalIncludes.h"
#include "../../Interfaces/Interfaces.h"
#include "../../Configuration/Config.h"

#include <algorithm>

using namespace Features;
using namespace Graphics;

CEventLogger* Features::EventLogger = new CEventLogger();

void CEventLogger::AddLog( const char* str, ... )
{
	if ( !str )
		return;

	if (logs.size() > 5)
		logs.erase(logs.begin());

	va_list va_args;
	char buffer[2048] = { 0 };
	va_start( va_args, str );
	_vsnprintf( buffer, sizeof( buffer ), str, va_args );
	va_end( va_args );
	logs.push_back( { Interfaces::Globals->realtime, -15, 0, std::string( buffer ) } );
}

void CEventLogger::Draw()
{
	for ( int i = 0; i < logs.size(); i++ )
	{
		auto &l = logs.at( i );

		if (l.alpha == 0 && Interfaces::Globals->realtime - l.time >= 5)
			logs.erase(logs.begin() + i);

		if ( l.yOffset < 0 && l.alpha < 255 && Interfaces::Globals->realtime - l.time < 5 )
		{
			l.yOffset += 1;
			l.alpha += 17;
			continue;
		}

		if ( l.yOffset <= 0 && l.alpha <= 255 && l.alpha > 0 && Interfaces::Globals->realtime - l.time >= 5 )
		{
			l.yOffset -= 1;
			l.alpha -= 17;
			continue;
		}
	}

	auto offset = 0;
	auto textSize = 13;

	for ( auto &l : logs )
	{
		auto addy = ( c_config::get()->b["misc_showprefix"] ? Drawing->TextSize( "[shattered] ", Drawing->fonts.esp_main ).right : 0 );


		/*if ( c_config::get()->b["misc_showprefix"] )
			Drawing->Text( 5, 5 + offset + l.yOffset, "[shattered] ", Drawing->fonts.esp_main, c_config::get()->imcolor_to_ccolor(c_config::get()->c["misc_
			"]));*/

		Drawing->Text( 5 + addy, 5 + offset + l.yOffset, l.text.c_str(), Drawing->fonts.eventlogs, CColor( 255, 255, 255, l.alpha ) );
		offset += textSize + l.yOffset;
	}
}