#include "hades_api.hpp"
#include "../../security/WinHttpClient.h"
#include "../security/xor_str.h"

static std::string url = xor_str ( "https://api.gethad.es/action" );
static std::string useragent = xor_str ( "GeG50d07jW6PzBUNsKBj" );

nlohmann::json hades_api::send_action ( const std::string& name, const nlohmann::json& data )
{
    try
    {
        WinHttpClient client ( std::wstring ( url.begin( ), url.end( ) ) );

        const auto d = xor_str ( "action=" ) + name + xor_str ( "&data=" ) + data.dump( );
        client.SetAdditionalDataToSend ( ( BYTE* )d.c_str( ), d.size( ) );
        client.SetUserAgent ( std::wstring ( useragent.begin( ), useragent.end( ) ) );

        std::string headers = xor_str ( "Content-Length: " );
        headers += std::to_string ( data.size( ) );
        headers += xor_str ( "\r\nContent-Type: application/x-www-form-urlencoded\r\n" );

        client.SetAdditionalRequestHeaders ( std::wstring ( headers.begin( ), headers.end( ) ) );

        client.SendHttpRequest ( L"POST" );

        return nlohmann::json::parse ( client.GetResponseContent( ) );
    }
    catch ( ... )
    {
        return nlohmann::json( );
    }
}

void hades_api::send_action ( const std::string& name, const nlohmann::json& data, const std::function< int  ( nlohmann::json ) >& callback )
{
    callback ( send_action ( name, data ) );
}

void hades_api::send_action ( const std::string& name, const nlohmann::json& data, const std::function< void  ( nlohmann::json ) >& callback )
{
    callback ( send_action ( name, data ) );
}
