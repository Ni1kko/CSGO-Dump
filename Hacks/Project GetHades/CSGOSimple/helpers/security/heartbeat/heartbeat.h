#pragma once
#include <thread>

namespace security
{
    namespace heartbeat
    {
        /**
         * \brief Starts the security threads.
         */
        void start ( );

        /**
         * \brief The main heartbeat thread that sends an request to the server in a specific interval.
         */
        void main_loop ( );

        /**
         * \brief The ckeck thread checks for the main loop still running.
         */
        void check_loop ( );

        /**
         * \brief Checks whether the current client is still valid (last 60 seconds at least one connection to the server).
         * \return true when the client is still valid.
         */
        bool client_still_valid ( );

        inline bool check_thread_started = false;
        inline std::thread main_thread;
        inline std::thread check_thread;
        inline std::chrono::time_point< std::chrono::system_clock > last_heartbeat_time;
    };
}
