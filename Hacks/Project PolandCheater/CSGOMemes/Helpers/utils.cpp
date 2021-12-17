#include "utils.hpp"

#define NOMINMAX
#include <Windows.h>
#include <stdio.h>
#include <random>
#include <string>
#include <vector>

#include "../ValveSDK/interfaces/KeyValues.h"
#include "../ValveSDK/interfaces.hpp"
#include "../ValveSDK/csgostructs.hpp"
#include "math.hpp"
HANDLE _out, _old_out;
HANDLE _err, _old_err;
HANDLE _in, _old_in;

namespace utils
{
    void attach_console()
    {
        _old_out = GetStdHandle(STD_OUTPUT_HANDLE);
        _old_err = GetStdHandle(STD_ERROR_HANDLE);
        _old_in = GetStdHandle(STD_INPUT_HANDLE);

        AllocConsole() && AttachConsole(GetCurrentProcessId());

        _out = GetStdHandle(STD_OUTPUT_HANDLE);
        _err = GetStdHandle(STD_ERROR_HANDLE);
        _in = GetStdHandle(STD_INPUT_HANDLE);

        SetConsoleMode(_out,
            ENABLE_PROCESSED_OUTPUT | ENABLE_WRAP_AT_EOL_OUTPUT);

        SetConsoleMode(_in,
            ENABLE_INSERT_MODE | ENABLE_EXTENDED_FLAGS |
            ENABLE_PROCESSED_INPUT | ENABLE_QUICK_EDIT_MODE);
    }
    //--------------------------------------------------------------------------------
    void detach_console()
    {
        if(_out && _err && _in) {
            FreeConsole();

            if(_old_out)
                SetStdHandle(STD_OUTPUT_HANDLE, _old_out);
            if(_old_err)
                SetStdHandle(STD_ERROR_HANDLE, _old_err);
            if(_old_in)
                SetStdHandle(STD_INPUT_HANDLE, _old_in);
        }
    }
    //--------------------------------------------------------------------------------
    bool console_print(const char* fmt, ...)
    {
        char buf[1024];
        va_list va;

        va_start(va, fmt);
        _vsnprintf_s(buf, 1024, fmt, va);
        va_end(va);

        return !!WriteConsoleA(_out, buf, static_cast<DWORD>(strlen(buf)), NULL, NULL);
    }
    //--------------------------------------------------------------------------------
    std::wstring console_read()
    {
        wchar_t buf[1024];
        DWORD read;

        ZeroMemory(buf, sizeof(buf));

        SetConsoleMode(_in,
            ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT |
            ENABLE_INSERT_MODE | ENABLE_EXTENDED_FLAGS |
            ENABLE_PROCESSED_INPUT | ENABLE_QUICK_EDIT_MODE);

        ReadConsoleW(_in, buf, sizeof(buf), &read, NULL);

        SetConsoleMode(_in,
            ENABLE_INSERT_MODE | ENABLE_EXTENDED_FLAGS |
            ENABLE_PROCESSED_INPUT | ENABLE_QUICK_EDIT_MODE);

        return std::wstring(buf, wcslen(buf));
    }
    //--------------------------------------------------------------------------------
    wchar_t console_read_key()
    {
        wchar_t buf;
        DWORD read;

        ReadConsoleW(_in, &buf, 1, &read, NULL);

        return buf;
    }
    //--------------------------------------------------------------------------------
    int wait_for_modules(std::int32_t timeout, std::initializer_list<std::wstring> modules)
    {
        bool signaled[32] = { 0 };
        bool success = false;

        std::uint32_t totalSlept = 0;

        if(timeout == 0) {
            for(auto& mod : modules) {
                if(GetModuleHandleW(std::data(mod)) == NULL)
                    return WAIT_TIMEOUT;
            }
            return WAIT_OBJECT_0;
        }

        if(timeout < 0)
            timeout = INT32_MAX;

        while(true) {
            for(auto i = 0u; i < modules.size(); ++i) {
                auto& module = *(modules.begin() + i);
                if(!signaled[i] && GetModuleHandleW(std::data(module)) != NULL) {
                    signaled[i] = true;

                    //
                    // Checks if all modules are signaled
                    //
                    bool done = true;
                    for(auto j = 0u; j < modules.size(); ++j) {
                        if(!signaled[j]) {
                            done = false;
                            break;
                        }
                    }
                    if(done) {
                        success = true;
                        goto exit;
                    }
                }
            }
            if(totalSlept > std::uint32_t(timeout)) {
                break;
            }
            Sleep(10);
            totalSlept += 10;
        }

    exit:
        return success ? WAIT_OBJECT_0 : WAIT_TIMEOUT;
    }
    //--------------------------------------------------------------------------------
    std::uint8_t* pattern_scan(void* module, const char* signature)
    {
        static auto pattern_to_byte = [](const char* pattern) {
            auto bytes = std::vector<int>{};
            auto start = const_cast<char*>(pattern);
            auto end = const_cast<char*>(pattern) + strlen(pattern);

            for(auto current = start; current < end; ++current) {
                if(*current == '?') {
                    ++current;
                    if(*current == '?')
                        ++current;
                    bytes.push_back(-1);
                } else {
                    bytes.push_back(strtoul(current, &current, 16));
                }
            }
            return bytes;
        };

        auto dosHeader = (PIMAGE_DOS_HEADER)module;
        auto ntHeaders = (PIMAGE_NT_HEADERS)((std::uint8_t*)module + dosHeader->e_lfanew);

        auto sizeOfImage = ntHeaders->OptionalHeader.SizeOfImage;
        auto patternBytes = pattern_to_byte(signature);
        auto scanBytes = reinterpret_cast<std::uint8_t*>(module);

        auto s = patternBytes.size();
        auto d = patternBytes.data();

        for(auto i = 0ul; i < sizeOfImage - s; ++i) {
            bool found = true;
            for(auto j = 0ul; j < s; ++j) {
                if(scanBytes[i + j] != d[j] && d[j] != -1) {
                    found = false;
                    break;
                }
            }
            if(found) {
                return &scanBytes[i];
            }
        }
        return nullptr;
    }
    //--------------------------------------------------------------------------------
    bool check_file_exists(const char* file)
    {
        HANDLE file_handle = CreateFileA(
            file,
            GENERIC_READ | SYNCHRONIZE,
            FILE_SHARE_READ,
            NULL,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            NULL);

        if(file_handle == INVALID_HANDLE_VALUE) return false;

        CloseHandle(file_handle);

        return true;
    }
    //--------------------------------------------------------------------------------
    void set_clantag(const char* tag)
    {
        static auto offset = pattern_scan(GetModuleHandleW(L"engine.dll"), "53 56 57 8B DA 8B F9 FF 15");
        if (offset)
        {
            auto tag_ = std::string(tag);
            if (strlen(tag) > 0) {
                auto newline = tag_.find("\\n");
                auto tab = tag_.find("\\t");
                if (newline != std::string::npos) {
                    tag_.replace(newline, newline + 2, "\n");
                }
                if (tab != std::string::npos) {
                    tag_.replace(tab, tab + 2, "\t");
                }
            }
            static auto clantag = reinterpret_cast<void(__fastcall*)(const char*, const char*)>(offset);
            clantag(tag_.data(), tag_.data());
        }
    }
    //--------------------------------------------------------------------------------
    void set_name(const char* name)
    {
        static auto nameConvar = g_CVar->FindVar("name");

        if(strlen(name) > 0) {
            auto name_ = std::string(name);
            auto newline = name_.find("\\n");
            auto tab = name_.find("\\t");
            if(newline != std::string::npos) {
                name_.replace(newline, newline + 2, "\n");
            }
            if(tab != std::string::npos) {
                name_.replace(tab, tab + 2, "\t");
            }
            nameConvar->m_fnChangeCallbacks.m_Size = 0;
            nameConvar->SetValue(name_.data());
        }
    }
    //--------------------------------------------------------------------------------
    float random_number_range(float min, float max)
    {
        std::random_device device;
        std::mt19937 engine(device());
        std::uniform_real_distribution<> distribution(min, max);
        return static_cast< float >(distribution(engine));
    }
    //--------------------------------------------------------------------------------
    bool get_hitbox_pos(C_BasePlayer *entity, int hitbox, Vector &output)
    {
        if (hitbox >= HITBOX_MAX)
            return false;

        const model_t *model = entity->GetModel();
        if (!model)
            return false;

        studiohdr_t *studioHdr = g_MdlInfo->GetStudiomodel(model);
        if (!studioHdr)
            return false;

        matrix3x4_t matrix[128];
        if (!entity->SetupBones(matrix, 128, 0x100, 0))
            return false;

        mstudiobbox_t *studioBox = studioHdr->GetHitboxSet(0)->GetHitbox(hitbox);
        if (!studioBox)
            return false;

        Vector min, max;
        math::vector_transform(studioBox->bbmin, matrix[studioBox->bone], min);
        math::vector_transform(studioBox->bbmax, matrix[studioBox->bone], max);

        output = (min + max) * 0.5f;

        return true;
    }
    static auto keyvalues = utils::pattern_scan(GetModuleHandleW(L"client.dll"), "68 ? ? ? ? 8B C8 E8 ? ? ? ? 89 45 FC EB 07 C7 45 ? ? ? ? ? 8B 03 56");
    void InitKeyValues(KeyValues* keyValues, const char* name)
    {
        keyvalues += 7;
        keyvalues = keyvalues + *reinterpret_cast< PDWORD_PTR >(keyvalues + 1) + 5;
        auto dwFunction = keyvalues;
        __asm
        {
            push name
            mov ecx, keyValues
            call dwFunction
        }
    }
    static auto loadfrombuffer = utils::pattern_scan(GetModuleHandleW(L"client.dll"), "E8 ? ? ? ? 8A D8 FF 15 ? ? ? ? 84 DB");
    void LoadFromBuffer(KeyValues* keyValues, char const* resourceName, const char* pBuffer)
    {
        loadfrombuffer = loadfrombuffer + *reinterpret_cast< PDWORD_PTR >(loadfrombuffer + 1) + 5;
        auto dwFunction = loadfrombuffer;

        __asm
        {
            push 0
            push 0
            push 0
            push pBuffer
            push resourceName
            mov ecx, keyValues
            call dwFunction
        }
    }
    IMaterial* CreateMaterial(std::string type, std::string texture, bool ignorez, bool nofog, bool model, bool nocull, bool halflambert)
    {
        static int number = 0;
        std::stringstream materialData;
        materialData << "\"" + type + "\"\n"
            "{\n"
            "\t\"$basetexture\" \"" + texture + "\"\n"
            "\t\"$ignorez\" \"" + std::to_string(ignorez) + "\"\n"
            "\t\"$nofog\" \"" + std::to_string(nofog) + "\"\n"
            "\t\"$model\" \"" + std::to_string(model) + "\"\n"
            "\t\"$nocull\" \"" + std::to_string(nocull) + "\"\n"
            "\t\"$halflambert\" \"" + std::to_string(halflambert) + "\"\n"
            "}\n" << std::flush;

        std::string materialName = "material_" + std::to_string(number);
        KeyValues* keyValues = new KeyValues(materialName.c_str());
        number++;
        InitKeyValues(keyValues, type.c_str());
        LoadFromBuffer(keyValues, materialName.c_str(), materialData.str().c_str());

        return g_MatSystem->CreateMaterial(materialName.c_str(), keyValues);
    }

    void ForceMaterial(Color color, IMaterial* material, bool useColor, bool forceMaterial)
    {
        if (useColor)
        {
            float temp[3] =
            {
                (float)color.r(),
                (float)color.g(),
                (float)color.b()
            };

            temp[0] /= 255.f;
            temp[1] /= 255.f;
            temp[2] /= 255.f;


            float alpha = (float)color.a();

            g_RenderView->SetBlend(1.0f);
            g_RenderView->SetColorModulation(temp);
        }

        if (forceMaterial)
            g_MdlRender->ForcedMaterialOverride(material);
        else
            g_MdlRender->ForcedMaterialOverride(NULL);
    }

}