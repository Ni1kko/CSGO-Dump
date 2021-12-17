#pragma once

#include "ItemSchema.h"
#include "../../Utility/Utilities.h"

class ItemSchema;

class Memory {
public:
    void initialize() noexcept;

    uintptr_t hud;
    int* (__thiscall* findHudElement)(uintptr_t, const char*);
    int(__thiscall* clearHudWeapon)(int*, int);
    std::add_pointer_t<ItemSchema* __cdecl()> itemSchema;
    int(__thiscall* equipWearable)(void* wearable, void* player);
};