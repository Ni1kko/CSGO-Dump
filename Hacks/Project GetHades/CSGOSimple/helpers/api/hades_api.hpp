#pragma once
#include "../../include/json.h"

namespace hades_api
{
    /**
    * \brief Sends an request to the hades api
    * \param name The name of the action
    * \param data The data to send to the server
    * \return The response of the server
    */
    nlohmann::json send_action ( const std::string& name, const nlohmann::json& data );

    /**
    * \brief Sends an request to the hades api and calls the given callback
    * \param name The name of the action
    * \param data The data to send to the server
    * \param callback The callback to call with the server response
    */
    void send_action ( const std::string& name, const nlohmann::json& data, const std::function< int  ( nlohmann::json ) >& callback );

    /**
    * \brief Sends an request to the hades api and calls the given callback
    * \param name The name of the action
    * \param data The data to send to the server
    * \param callback The callback to call with the server response
    */
    void send_action ( const std::string& name, const nlohmann::json& data, const std::function< void  ( nlohmann::json ) >& callback );
} // namespace hades_api
