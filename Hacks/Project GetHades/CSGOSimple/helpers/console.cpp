#include "console.h"

HANDLE out = nullptr, old_out = nullptr;
HANDLE err = nullptr, old_err = nullptr;
HANDLE in = nullptr, old_in = nullptr;

namespace console
{
    bool print ( const char* fmt, ... )
    {
        if ( !out )
            return false;

        char buf[1024];
        va_list va;

        va_start ( va, fmt );
        _vsnprintf_s ( buf, 1024, fmt, va );
        va_end ( va );

        return WriteConsoleA ( out, buf, static_cast< DWORD > ( strlen ( buf ) ), nullptr, nullptr );
    }

    void write ( std::string text, ... )
    {
        print ( text.data( ) );
    }

    void write_line ( const std::string& text, ... )
    {
        print ( ( text + xor_str ( "\n" ) ).data( ) );
    }

    void write_line ( const Vector vec, ... )
    {
        write_line (
            xor_str ( "Vector(" ) + std::to_string ( vec.x ) + xor_str ( ", " ) + std::to_string ( vec.y ) + xor_str ( ", " ) + std::to_string ( vec.z ) +
            xor_str ( ")" ) );
    }

    void write_line ( const QAngle ang, ... )
    {
        write_line (
            xor_str ( "QAngle(" ) + std::to_string ( ang.pitch ) + xor_str ( ", " ) + std::to_string ( ang.yaw ) + xor_str ( ", " ) + std::
            to_string ( ang.roll ) + xor_str ( ")" ) );
    }

    void write_line ( const float num )
    {
        write_line ( std::to_string ( num ) );
    }

    void write_line ( Color clr )
    {
        write_line (
            xor_str ( "Color(" ) + std::to_string ( clr.r( ) ) + xor_str ( ", " ) + std::to_string ( clr.g( ) ) + xor_str ( ", " ) + std::
            to_string ( clr.b( ) ) + xor_str ( ", " ) + std::to_string ( clr.a( ) ) + xor_str ( ")" ) );
    }

    void write_line ( const int num )
    {
        write_line ( std::to_string ( num ) );
    }

    void attach ( )
    {
        old_out = GetStdHandle ( STD_OUTPUT_HANDLE );
        old_err = GetStdHandle ( STD_ERROR_HANDLE );
        old_in = GetStdHandle ( STD_INPUT_HANDLE );

        ::AllocConsole( ) && ::AttachConsole ( GetCurrentProcessId( ) );

        out = GetStdHandle ( STD_OUTPUT_HANDLE );
        err = GetStdHandle ( STD_ERROR_HANDLE );
        in = GetStdHandle ( STD_INPUT_HANDLE );

        SetConsoleMode ( out,
                         ENABLE_PROCESSED_OUTPUT | ENABLE_WRAP_AT_EOL_OUTPUT );

        SetConsoleMode ( in,
                         ENABLE_INSERT_MODE | ENABLE_EXTENDED_FLAGS |
                         ENABLE_PROCESSED_INPUT | ENABLE_QUICK_EDIT_MODE );
    }

    void detach ( )
    {
        if ( out && err && in )
        {
            FreeConsole( );

            if ( old_out )
                SetStdHandle ( STD_OUTPUT_HANDLE, old_out );

            if ( old_err )
                SetStdHandle ( STD_ERROR_HANDLE, old_err );

            if ( old_in )
                SetStdHandle ( STD_INPUT_HANDLE, old_in );
        }
    }

    char read_key ( )
    {
        if ( !in )
            return false;

        auto key = char{ 0 };
        auto keysread = DWORD{ 0 };

        ReadConsoleA ( in, &key, 1, &keysread, nullptr );

        return key;
    }
}
