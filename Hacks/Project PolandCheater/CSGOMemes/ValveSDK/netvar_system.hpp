#pragma once

#include <memory>
#include <iostream>
#include <unordered_map>

#include "misc/Recv.hpp"

class netvar_system
{
    struct netvar_table
    {
        std::string               name;
        RecvProp*                 prop;
        uint32_t                  offset;
        std::vector<RecvProp*>    child_props;
        std::vector<netvar_table> child_tables;
    };

    netvar_system(const netvar_system&) = delete;
    netvar_system& operator=(const netvar_system&) = delete;

public:
    netvar_system() = default;
    ~netvar_system() = default;

    void initialize();
    void dump();
    void dump(std::ostream& stream);

    uint32_t      find(const std::string& tableName, const std::string& propName);
    RecvProp*     get_prop(const std::string& tableName, const std::string& propName);
    RecvVarProxyFn     hook_prop(const std::string& tableName, const std::string& propName, void* hkFunc, RecvVarProxyFn oldFn = 0);
private:
    netvar_table  load_table(RecvTable* clientClass);
    void          dump_table(std::ostream& stream, const netvar_table& table, uint32_t indentation);
    uint32_t      find(const netvar_table& table, const std::string& propName);
    RecvProp*     get_prop(const netvar_table& table, const std::string& propName);

private:
    std::vector<netvar_table>  database;
};

extern netvar_system netvars;