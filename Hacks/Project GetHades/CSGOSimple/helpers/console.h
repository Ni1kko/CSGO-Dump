#pragma once

#include "../valve_sdk/csgostructs.hpp"

namespace console
{
    /*
    * @brief prints c-strings to console
    *
    * Replacement to printf that works with the newly created console
    */
    bool print ( const char* fmt, ... );

    /*
     * @brief prints on one line to console
     *
     * Replacement to printf that works with the newly created console
     */
    void write ( std::string text, ... );
    /*
     * @brief prints with new line to console
     *
     * Replacement to printf that works with the newly created console
     */
    void write_line ( const std::string& text, ... );
    void write_line ( Vector vec, ... );
    void write_line ( QAngle ang, ... );
    void write_line ( int num );
    void write_line ( float num );
    void write_line ( Color clr );

    /*
     * @brief Create console
     *
     * Create and attach a console window to the current process
     */
    void attach ( );
    /*
     * @brief Detach console
     *
     * Detach and destroy the attached console
     */
    void detach ( );

    /*
     * @brief Blocks execution until a key is pressed on the console window
     *
     */
    char read_key ( );
};
