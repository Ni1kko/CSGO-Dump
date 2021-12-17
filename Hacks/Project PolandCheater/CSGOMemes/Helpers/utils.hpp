#pragma once

#define NOMINMAX
#include <Windows.h>
#include <string>
#include <initializer_list>
#include "../ValveSDK/interfaces.hpp"

namespace utils
{
    void attach_console();

    void detach_console();

    bool console_print(_In_z_ _Printf_format_string_ const char* fmt, ...);

    std::wstring console_read();

    wchar_t console_read_key();

    int wait_for_modules(std::int32_t timeout, std::initializer_list<std::wstring> modules);

    std::uint8_t* pattern_scan(void* module, const char* signature);

    bool check_file_exists(const char* file);

    void set_clantag(const char* tag);

    void set_name(const char* name);

    float random_number_range(float min, float max);\

    void InitKeyValues(KeyValues* keyValues, const char* name);
    void LoadFromBuffer(KeyValues* keyValues, char const* resourceName, const char* pBuffer);
    IMaterial* CreateMaterial(std::string type, std::string texture, bool ignorez = false, bool nofog = false, bool model = false, bool nocull = false, bool halflambert = false);
    void ForceMaterial(Color color, IMaterial* material, bool useColor, bool forceMaterial);

   // void UTIL_TraceLine(const Vector& vecAbsStart, const Vector& vecAbsEnd, unsigned int mask,
   //     const IClientEntity *ignore, int collisionGroup, trace_t *ptr);

   // bool  is_visible(C_BasePlayer* local, C_BasePlayer* entity, int bone_id);
}
