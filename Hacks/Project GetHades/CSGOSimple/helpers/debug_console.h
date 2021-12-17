#pragma once

#include "../valve_sdk/csgostructs.hpp"

namespace debug_console
{
    void print ( const std::string& msg );
#ifdef _DEBUG
    void debug ( const std::string& msg );
#endif // _DEBUG

};
