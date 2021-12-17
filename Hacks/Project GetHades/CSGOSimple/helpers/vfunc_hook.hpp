#pragma once
#define NOMINMAX
#include <Windows.h>

#include <cstdint>
#include <stdexcept>

namespace detail
{
    class protect_guard
    {
    public:
        protect_guard ( void* base, size_t len, std::uint32_t flags )
        {
            _base = base;
            _length = len;

            if ( !VirtualProtect ( base, len, flags, reinterpret_cast< PDWORD > ( &_old ) ) )
                throw std::runtime_error ( "Failed to protect region." );
        }

        ~protect_guard ( )
        {
            VirtualProtect ( _base, _length, _old, reinterpret_cast< PDWORD > ( &_old ) );
        }

    private:
        void* _base;
        size_t _length;
        std::uint32_t _old{ };
    };
}

class vfunc_hook
{
public:
    uintptr_t* search_free_data_page ( const char* module_name, std::size_t vmt_size );
    vfunc_hook ( );
    explicit vfunc_hook ( void* base );
    ~vfunc_hook ( );

    bool setup ( void* base, const char* moduleName = nullptr );

    template < typename T >
    void hook_index ( const int index, T fun )
    {
        //std::assert ( index >= 0 && index <= ( int )vftbl_len );

        if ( index >= 0 && index <= static_cast< int > ( vftbl_len ) )
            new_vftb1[ index + 1 ] = uintptr_t ( fun );
    }

    void unhook_index ( int index ) const
    {
        new_vftb1[ index ] = old_vftbl[ index ];
    }

    void unhook_all ( )
    {
        try
        {
            if ( old_vftbl != nullptr )
            {
                auto guard = detail::protect_guard{ class_base, sizeof ( std::uintptr_t ), PAGE_READWRITE };
                * static_cast< std::uintptr_t** > ( class_base ) = old_vftbl;
                old_vftbl = nullptr;
            }
        }
        catch ( ... )
        {
        }
    }

    template < typename t >
    t get_original ( const int index )
    {
        return reinterpret_cast< t > ( old_vftbl[ index ] );
    }

private:

    static inline std::size_t estimate_vftbl_length ( std::uintptr_t* vftbl_start );

    void* class_base;
    std::size_t vftbl_len;
    std::uintptr_t* new_vftb1;
    std::uintptr_t* old_vftbl;
    LPCVOID search_base = nullptr;
    bool wasAllocated = false;
};

class vftable_hook /*: public singleton<vftable_hook>*/
{
    vftable_hook ( const vftable_hook& ) = delete;
public:

    template < class Type >
    static Type hook_manual ( uintptr_t* instance, int offset, Type hook )
    {
        DWORD Dummy;
        Type fnOld = reinterpret_cast< Type > ( instance[ offset ] );
        //VirtualProtect ( ( void* ) ( instance + offset * 0x4 ), 0x4, PAGE_EXECUTE_READWRITE, &Dummy );
        VirtualProtect ( static_cast< void* > ( instance + offset * 0x4 ), 0x4, PAGE_EXECUTE_READWRITE, &Dummy );
        instance[ offset ] = ( uintptr_t )hook;
        VirtualProtect ( static_cast< void* > ( instance + offset * 0x4 ), 0x4, Dummy, &Dummy );
        return fnOld;
    }

private:

    static int unprotect ( void* region )
    {
        MEMORY_BASIC_INFORMATION mbi;
        VirtualQuery ( static_cast< LPCVOID > ( region ), &mbi, sizeof ( mbi ) );
        VirtualProtect ( mbi.BaseAddress, mbi.RegionSize, PAGE_EXECUTE_READWRITE, &mbi.Protect );
        return mbi.Protect;
    }

    static void protect ( void* region, int protection )
    {
        MEMORY_BASIC_INFORMATION mbi;
        VirtualQuery ( static_cast< LPCVOID > ( region ), &mbi, sizeof ( mbi ) );
        VirtualProtect ( mbi.BaseAddress, mbi.RegionSize, protection, &mbi.Protect );
    }
};
