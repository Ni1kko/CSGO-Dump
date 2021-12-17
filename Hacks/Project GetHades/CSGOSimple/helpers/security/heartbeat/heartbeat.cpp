#include "heartbeat.h"
#include "../../../options.h"

namespace security
{
    using namespace std::chrono_literals;

    void heartbeat::start ( )
    {
        last_heartbeat_time = std::chrono::system_clock::now( );
        main_thread = std::thread ( main_loop );
        check_thread = std::thread ( check_loop );

        //std::chrono::time_point<std::chrono::system_clock> t = std::chrono::system_clock::now(  );
        //t.time_since_epoch(  )
    }

    void heartbeat::check_loop ( )
    {
        check_thread_started = true;

        while ( !g_unload )
        {
            if ( !main_thread.joinable( ) )
                std::terminate( );

            std::this_thread::sleep_for ( 1s );
        }
    }

    bool heartbeat::client_still_valid ( )
    {
        return last_heartbeat_time.time_since_epoch( ) <= 60s;
    }

    void heartbeat::main_loop ( )
    {
        while ( !check_thread_started )
            std::this_thread::sleep_for ( 500ms );

        while ( !g_unload )
        {
            if ( !check_thread.joinable( ) )
                std::terminate( );

            // todo run api request here

            last_heartbeat_time = std::chrono::system_clock::now( );

            std::this_thread::sleep_for ( 5s );
        }
    }
}
