#pragma once
#include "../misc/vfunc.hpp"

class IBaseFileSystem
{
public:
    int read(void* output, int size, void* file) {
        typedef int(__thiscall* tRead)(void*, void*, int, void*);
        return CallVFunction<tRead>(this, 0)(this, output, size, file);
    }

    void* open(const char* file_name, const char* options, const char* path_id) {
        typedef void* (__thiscall* tOpen)(void*, const char*, const char*, const char*);
        return CallVFunction<tOpen>(this, 2)(this, file_name, options, path_id);
    }

    void close(void* file) {
        typedef void(__thiscall* tClose)(void*, void*);
        return CallVFunction<tClose>(this, 3)(this, file);
    }

    unsigned int size(void* file) {
        typedef unsigned int(__thiscall* tSize)(void*, void*);
        return CallVFunction<tSize>(this, 7)(this, file);
    }
};