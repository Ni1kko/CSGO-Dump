#pragma once

#include <fstream>
#include <string>
#include <optional>
#include <map>
#include "../misc/vfunc.hpp"

class IBaseFileSystem
{
public:

    int read(void* output, int size, void* file)
    {
        return CallVFunction<int(__thiscall*)(void*, void*, int, void*)>(this, 0)(this, output, size, file);
    }

    void* open(const char* file_name, const char* options, const char* path_id)
    {
        return CallVFunction<void* (__thiscall*)(void*, const char*, const char*, const char*)>(this, 2)(this, file_name, options, path_id);
    }

    void close(void* file)
    {
        return CallVFunction<void(__thiscall*)(void*, void*)>(this, 3)(this, file);
    }

    unsigned int size(void* file)
    {
        return CallVFunction<unsigned int(__thiscall*)(void*, void*)>(this, 7)(this, file);
    }
};