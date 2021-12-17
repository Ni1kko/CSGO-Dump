#pragma once
#include <string>
#include "../security/xor_str.h"

namespace ctx
{
    struct cheat_user_t
    {
        cheat_user_t ( )
        {
        };

        cheat_user_t ( const std::string& username, const std::string& avatar_url ) : username ( username ), avatar_url ( avatar_url )
        {
            init = true;
        };

        std::string username = xor_str ( "" );
        std::string avatar_url = xor_str ( "" );
        bool init = false;
    };

    extern cheat_user_t cheat_user;
}
