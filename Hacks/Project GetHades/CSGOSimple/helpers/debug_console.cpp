#include "debug_console.h"

namespace debug_console
{
    void print ( const std::string& msg )
    {
        interfaces::cvar->console_color_printf ( Color ( 240, 240, 240 ), xor_str ( "[ " ) );
        interfaces::cvar->console_color_printf ( Color ( 255, 50, 0 ), xor_str ( "HADES" ) );
        interfaces::cvar->console_color_printf ( Color ( 240, 240, 240 ), xor_str ( " ] " ) );
        interfaces::cvar->console_color_printf ( Color ( 240, 240, 240 ), ( msg + xor_str ( "\n" ) ).data( ) );
    }

    void debug ( const std::string& msg )
    {
        interfaces::cvar->console_color_printf ( Color ( 240, 240, 240 ), xor_str ( "[ " ) );
        interfaces::cvar->console_color_printf ( Color ( 0, 50, 255 ), xor_str ( "HADES" ) );
        interfaces::cvar->console_color_printf ( Color ( 240, 240, 240 ), xor_str ( " ] " ) );
        interfaces::cvar->console_color_printf ( Color ( 240, 240, 240 ), ( msg + xor_str ( "\n" ) ).data( ) );
    }

    /*
    void debug ( std::string msg )
    {
        interfaces::cvar->console_color_printf ( Color ( 240, 240, 240 ), "[ " );
        interfaces::cvar->console_color_printf ( Color ( 0, 122, 222 ), "H" );
        interfaces::cvar->console_color_printf ( Color ( 240, 240, 240 ), "ADES" );
        interfaces::cvar->console_color_printf ( Color ( 240, 240, 240 ), " ] " );
        interfaces::cvar->console_color_printf ( Color ( 240, 240, 240 ), ( msg + "\n" ).data() );
    }

    void warning ( std::string msg )
    {
        interfaces::cvar->console_color_printf ( Color ( 240, 240, 240 ), "[ " );
        interfaces::cvar->console_color_printf ( Color ( 222, 177, 0 ), "H" );
        interfaces::cvar->console_color_printf ( Color ( 240, 240, 240 ), "ADES" );
        interfaces::cvar->console_color_printf ( Color ( 240, 240, 240 ), " ] " );
        interfaces::cvar->console_color_printf ( Color ( 240, 240, 240 ), ( msg + "\n" ).data() );
    }

    void success ( std::string msg )
    {
        interfaces::cvar->console_color_printf ( Color ( 240, 240, 240 ), "[ " );
        interfaces::cvar->console_color_printf ( Color ( 40, 200, 75 ), "H" );
        interfaces::cvar->console_color_printf ( Color ( 240, 240, 240 ), "ADES" );
        interfaces::cvar->console_color_printf ( Color ( 240, 240, 240 ), " ] " );
        interfaces::cvar->console_color_printf ( Color ( 240, 240, 240 ), ( msg + "\n" ).data() );
    }

    void info ( std::string msg )
    {
        interfaces::cvar->console_color_printf ( Color ( 240, 240, 240 ), "[ " );
        interfaces::cvar->console_color_printf ( Color ( 20, 160, 100 ), "H" );
        interfaces::cvar->console_color_printf ( Color ( 240, 240, 240 ), "ADES" );
        interfaces::cvar->console_color_printf ( Color ( 240, 240, 240 ), " ] " );
        interfaces::cvar->console_color_printf ( Color ( 240, 240, 240 ), ( msg + "\n" ).data() );
    }

    void color ( std::string msg, Color clr )
    {
        interfaces::cvar->console_color_printf ( Color ( 240, 240, 240 ), "[ " );
        interfaces::cvar->console_color_printf ( clr, "H" );
        interfaces::cvar->console_color_printf ( Color ( 240, 240, 240 ), "ADES" );
        interfaces::cvar->console_color_printf ( Color ( 240, 240, 240 ), " ] " );
        interfaces::cvar->console_color_printf ( Color ( 240, 240, 240 ), ( msg + "\n" ).data() );
    }
    */
}
