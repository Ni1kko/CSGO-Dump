#pragma once

#include "main.h"

class CVMTHookManager
{
public:
	CVMTHookManager( void )
	{
		memset( this, 0, sizeof( CVMTHookManager ) );
	}

	CVMTHookManager( PDWORD* ppdwClassBase )
	{
		bInitialize( ppdwClassBase );
	}

	~CVMTHookManager( void )
	{
		UnHook();
	}

	bool bInitialize( PDWORD* ppdwClassBase )
	{
		m_ppdwClassBase = ppdwClassBase;
		m_pdwOldVMT = *ppdwClassBase;
		m_dwVMTSize = dwGetVMTCount( *ppdwClassBase );
		m_pdwNewVMT = new DWORD[ m_dwVMTSize ];
		memcpy( m_pdwNewVMT, m_pdwOldVMT, sizeof( DWORD ) * m_dwVMTSize );
		*ppdwClassBase = m_pdwNewVMT;
		return true;
	}

	bool bInitialize1( PDWORD** pppdwClassBase )
	{
		return bInitialize( *pppdwClassBase );
	}

	void UnHook( void )
	{
		if( m_ppdwClassBase )
		{
			*m_ppdwClassBase = m_pdwOldVMT;
		}
	}

	void ReHook( void )
	{
		if( m_ppdwClassBase )
		{
			*m_ppdwClassBase = m_pdwNewVMT;
		}
	}

	template< class T > T GetMethod( unsigned int nIndex ) const
	{
		return ( T )m_pdwOldVMT[ nIndex ];
	}

	int iGetFuncCount( void )
	{
		return ( int )m_dwVMTSize;
	}

	DWORD dwGetMethodAddress( int Index )
	{
		if( Index >= 0 && Index <= ( int )m_dwVMTSize && m_pdwOldVMT != NULL )
		{
			return m_pdwOldVMT[ Index ];
		}
		return NULL;
	}

	template<typename Fn>
	Fn GetFunctionAddress( int Index ) 
	{
		if( Index >= 0 && Index <= ( int ) m_dwVMTSize && m_pdwOldVMT != NULL )
		{
			return reinterpret_cast<Fn>( m_pdwOldVMT[ Index ] );
		}
		return NULL;
	}

	PDWORD pdwGetOldVMT( void )
	{
		return m_pdwOldVMT;
	}

	DWORD dwHookMethod( DWORD dwNewFunc, unsigned int iIndex )
	{
		if( m_pdwNewVMT && m_pdwOldVMT && iIndex <= m_dwVMTSize && iIndex >= 0 )
		{
			m_pdwNewVMT[ iIndex ] = dwNewFunc;
			return m_pdwOldVMT[ iIndex ];
		}
		return NULL;
	}

private:
    bool IsBadReadPtr( void* p )
    {
        MEMORY_BASIC_INFORMATION mbi = { 0 };
        if( ::VirtualQuery( p, &mbi, sizeof( mbi ) ) ) {
            auto mask = ( PAGE_READONLY | PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE
                | PAGE_EXECUTE_WRITECOPY );
            auto b = !( mbi.Protect & mask );

            if( mbi.Protect & ( PAGE_GUARD | PAGE_NOACCESS ) ) 
                b = true;

            return b;
        }
        return true;
    }

	DWORD dwGetVMTCount( PDWORD pdwVMT )
	{
		DWORD dwIndex = 0;

		for( dwIndex = 0; pdwVMT[ dwIndex ]; dwIndex++ )
		{
			if( IsBadReadPtr( ( FARPROC )pdwVMT[ dwIndex ] ) )
			{
				break;
			}
		}
		return dwIndex;
	}
	PDWORD*	m_ppdwClassBase;
	PDWORD	m_pdwNewVMT, m_pdwOldVMT;
	DWORD	m_dwVMTSize;
};

extern CVMTHookManager* g_pPaintTraverseHOOKED;
extern CVMTHookManager* g_pRunCommandHOOKED;
extern CVMTHookManager* g_pClientHOOKED;
extern CVMTHookManager* g_pDrawModelHOOKED;
extern CVMTHookManager* g_pClientModeHOOKED;
extern CVMTHookManager* g_pGameEventHOOKED;
extern CVMTHookManager* g_pEngineHOOKED;
extern CVMTHookManager* g_pMaterialSystemHOOKED;
extern CVMTHookManager* g_pRenderViewHOOKED;
extern CVMTHookManager* g_pSurfaceHOOKED;
extern CVMTHookManager* g_pEngineTraceHOOKED;

extern DWORD dwSvCheatsVMT;
extern DWORD dwModelStatsVMT;
extern DWORD dwCSMShadowVMT;
void HookConvar( void* src, void* dst, int index, DWORD& dwGetBool );

class VMTHook
{
public:
	VMTHook();

	VMTHook( PDWORD* ppdwClassBase );

	~VMTHook();

	bool bInitialize( PDWORD* ppdwClassBase );
	bool bInitialize( PDWORD** pppdwClassBase );

	void UnHook();

	void ReHook();

	int iGetFuncCount();

	template<typename Fn>
	Fn GetFuncAddress( int Index )
	{
		if( Index >= 0 && Index <= ( int ) m_VTSize && m_OldVT != NULL )
		{
			return reinterpret_cast<Fn>( m_OldVT[ Index ] );
		}
		return NULL;
	}

	PDWORD GetOldVT();

	DWORD HookFunction( DWORD dwNewFunc, unsigned int iIndex );

private:
	DWORD GetVTCount( PDWORD pdwVMT );

	PDWORD*	m_ClassBase;
	PDWORD	m_NewVT, m_OldVT;
	DWORD	m_VTSize;
};

extern VMTHook* g_pStudioRenderHOOKED;
extern VMTHook* g_pPlayerRenderableHOOKED;
extern VMTHook* g_pNetChannelHOOKED;
extern CVMTHookManager* g_pClientStateHOOKED;
extern VMTHook* g_pBSPTreeHOOKED;

class VTable
{
public:
	VTable() : count( 0 ), pCopyTable( nullptr ), pObject( nullptr ), pOrgTable( nullptr )
	{
	}

	explicit VTable( void* Interface, std::string moduleName )
	{
		this->Init( Interface, moduleName );
	}

	/*Inits the function*/
	bool Init( void* Interface, std::string moduleName )
	{
		pOrgTable = *static_cast< void** >( Interface );
		this->count = this->GetCount();
		auto space = search_free_data_page( moduleName.c_str(), sizeof( void* ) * count );
		if( !space ) {
			exit( 1 ); // ABOOOOOOOOOOOOOOORT we cant find free space shit
		}
		pCopyTable = space;
		memcpy( pCopyTable, pOrgTable, sizeof( void* ) * count );
		pObject = ( DWORD* ) Interface;
		return true;
	}

	/*Hook/Unhook*/
	void HookTable( bool hooked ) const
	{
		if( hooked ) {
			*pObject = reinterpret_cast< DWORD >( pCopyTable );
		}
		else {
			*pObject = reinterpret_cast< DWORD >( pOrgTable );
		}
	}

	/*Hooks function*/
	template <typename T>
	T HookFunction( int Index, void* hkFunction ) const
	{
		if( Index < this->count && Index >= 0 ) {
			static_cast< DWORD* >( pCopyTable ) [ Index ] = reinterpret_cast< DWORD >( hkFunction );
			return reinterpret_cast< T >( static_cast< DWORD* >( pOrgTable ) [ Index ] );
		}
		return nullptr;
	}

	/* Return Method Adress of Index */
	DWORD GetMethodAddress( int iIndex )
	{
		if( iIndex >= 0 && iIndex <= this->count && pOrgTable != nullptr ) {
			auto table = static_cast< void** >( pOrgTable );
			return reinterpret_cast< DWORD >( table [ iIndex ] );
		}
	}

	/* Get Free Space in the data Section for our VMT Hook */
	void* search_free_data_page( const char* module_name, const std::size_t vmt_size )
	{
		auto check_data_section = [ & ] ( LPCVOID address, const std::size_t vmt_size ) -> const bool
		{
			const DWORD DataProtection = ( PAGE_EXECUTE_READWRITE | PAGE_READWRITE ); // Section .data has the flag WRITE
			MEMORY_BASIC_INFORMATION mbi = { 0 };

			if( VirtualQuery( address, &mbi, sizeof( mbi ) ) == sizeof( mbi ) && mbi.AllocationBase && mbi.BaseAddress &&
				mbi.State == MEM_COMMIT && !( mbi.Protect & PAGE_GUARD ) && mbi.Protect != PAGE_GUARD ) {
				return ( ( mbi.Protect & DataProtection ) && mbi.RegionSize >= vmt_size ) ? true : false;
			}
			return false;
		};

		const auto module_addr = GetModuleHandleA( module_name );

		if( module_addr == nullptr ) // Module not found
			return nullptr;

		const auto dos_header = reinterpret_cast< PIMAGE_DOS_HEADER > ( module_addr );
		const auto nt_headers = reinterpret_cast< PIMAGE_NT_HEADERS > ( reinterpret_cast< std::uint8_t* >( module_addr ) + dos_header->e_lfanew );

		const auto module_end = reinterpret_cast< std::uintptr_t >( module_addr ) + nt_headers->OptionalHeader.SizeOfImage - sizeof( std::uintptr_t );

		for( auto current_address = reinterpret_cast< std::uintptr_t >( module_addr ); current_address < module_end; current_address += sizeof( std::uintptr_t ) ) {
			if( *reinterpret_cast< std::uintptr_t* >( current_address ) == 0 && check_data_section( reinterpret_cast< LPCVOID >( current_address ), vmt_size ) ) {
				bool is_good_vmt = true;
				auto page_count = 0u;

				for( ; page_count < vmt_size && is_good_vmt; page_count += sizeof( std::uintptr_t ) ) {
					if( *reinterpret_cast< std::uintptr_t* >( current_address + page_count ) != 0 )
						is_good_vmt = false;
				}

				if( is_good_vmt && page_count >= vmt_size )
					return &current_address;
			}
		}
		return nullptr; // no space found rip
	}

	/*Deconstructor*/
	~VTable()
	{
		this->HookTable( false );
	}

private:
	/*Returns if you can read the pointer*/
	static bool CanReadPointer( void* table )
	{
		MEMORY_BASIC_INFORMATION mbi;
		if( table == nullptr ) return false;
		if( !VirtualQuery( table, &mbi, sizeof( mbi ) ) ) return false;
		if( mbi.Protect & ( PAGE_GUARD | PAGE_NOACCESS ) ) return false;
		return ( mbi.Protect & ( PAGE_READWRITE | PAGE_EXECUTE_READWRITE | PAGE_READONLY | PAGE_WRITECOPY | PAGE_EXECUTE_READ | PAGE_EXECUTE_WRITECOPY ) );
	}

	/*Gets VMT count*/
	int GetCount() const
	{
		auto index = 0;
		auto table = static_cast< void** >( pOrgTable );
		for( void* fn; ( fn = table [ index ] ) != nullptr; index++ ) {
			if( !CanReadPointer( fn ) ) break;
		}
		return index;
	}

	int count;
	void* pCopyTable;
	DWORD* pObject;
	void* pOrgTable;
};

extern VTable* g_pClientvmtHOOKED;