#pragma once
#include <string>
#include "../xor_str.h"

namespace security
{
    class cheat_user
    {
    public:
        cheat_user ( );
    private:
        bool init ( );
        [[nodiscard]] std::string create_hwid ( ) const;

    private:
        std::string m_hwid = xor_str ( "" );
        std::string m_authkey = xor_str ( "" );
    };
}
