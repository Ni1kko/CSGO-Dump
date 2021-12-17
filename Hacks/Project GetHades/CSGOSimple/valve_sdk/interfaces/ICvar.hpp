#pragma once

#include "IAppSystem.hpp"
#include "icon_var.hpp"

class con_command_base;
class con_command;
class con_var;

typedef int CVarDLLIdentifier_t;

class IConsoleDisplayFunc
{
public:
    virtual void colorPrint ( const uint8_t* clr, const char* pMessage ) = 0;
    virtual void print ( const char* pMessage ) = 0;
    virtual void dprint ( const char* pMessage ) = 0;
};

class ICvar : public IAppSystem
{
public:
    virtual CVarDLLIdentifier_t allocate_dll_identifier ( ) = 0; // 9
    virtual void register_con_command ( con_command_base* pCommandBase ) = 0; //10
    virtual void unregister_con_command ( con_command_base* pCommandBase ) = 0;
    virtual void unregister_con_commands ( CVarDLLIdentifier_t id ) = 0;
    virtual const char* get_command_line_value ( const char* pVariableName ) = 0;
    virtual con_command_base* find_command_base ( const char* name ) = 0;
    virtual const con_command_base* find_command_base ( const char* name ) const = 0;
    virtual con_var* find_var ( const char* var_name ) = 0; //16
    virtual const con_var* find_var ( const char* var_name ) const = 0;
    virtual con_command* find_command ( const char* name ) = 0;
    virtual const con_command* find_command ( const char* name ) const = 0;
    virtual void install_global_change_callback ( FnChangeCallback_t callback ) = 0;
    virtual void remove_global_change_callback ( FnChangeCallback_t callback ) = 0;
    virtual void call_global_change_callbacks ( con_var* var, const char* pOldString, float flOldValue ) = 0;
    virtual void install_console_display_func ( IConsoleDisplayFunc* pDisplayFunc ) = 0;
    virtual void remove_console_display_func ( IConsoleDisplayFunc* pDisplayFunc ) = 0;
    virtual void console_color_printf ( const Color& clr, const char* pFormat, ... ) const = 0;
    virtual void console_printf ( const char* pFormat, ... ) const = 0;
    virtual void console_dprintf ( const char* pFormat, ... ) const = 0;
    virtual void revert_flagged_con_vars ( int nFlag ) = 0;
};
