#include "vmtmanager.h"

CVMTHookManager* g_pClientModeHOOKED = 0;
CVMTHookManager* g_pPaintTraverseHOOKED = 0;
CVMTHookManager* g_pRunCommandHOOKED = 0;
CVMTHookManager* g_pClientHOOKED = 0;
CVMTHookManager* g_pDrawModelHOOKED = 0;
CVMTHookManager* g_pGameEventHOOKED = 0;
CVMTHookManager* g_pEngineHOOKED = 0;
CVMTHookManager* g_pMaterialSystemHOOKED = 0;
CVMTHookManager* g_pRenderViewHOOKED = 0;
CVMTHookManager* g_pSurfaceHOOKED = 0;
CVMTHookManager* g_pClientStateHOOKED = 0;
CVMTHookManager* g_pEngineTraceHOOKED = 0;
VMTHook* g_pNetChannelHOOKED = 0;
VMTHook* g_pStudioRenderHOOKED = 0;
VMTHook* g_pPlayerRenderableHOOKED = 0;
VMTHook* g_pBSPTreeHOOKED = 0;

#define TABLE_SIZE(i) i*4
#define SIZE_ENTRY 4
DWORD dwSvCheatsVMT = NULL;
DWORD dwModelStatsVMT = NULL;
DWORD dwCSMShadowVMT = NULL;
uintptr_t* dwOriginalTPTable = 0;
void* pOriginalTablenz = 0;

void HookConvar( void* src, void* dst, int index, DWORD& dwGetBool )
{
	if( !src )
		return;

	pOriginalTablenz = src;

	void** vtbl = *( void*** ) src;

	if( !vtbl )
		return;

	int i = -1;
	do
	{
		i++;
	} while( vtbl[ i ] );

	if( i <= NULL )
		return;

	uintptr_t* table = ( uintptr_t* ) malloc( TABLE_SIZE( i ) + SIZE_ENTRY );

	if( !table )
		return;

	memcpy( &table[ 1 ], vtbl, TABLE_SIZE( i ) );

	// copy over rtti pointer.
	table[ 0 ] = ( uintptr_t ) vtbl[ -1 ];

	// change the classes vtable pointer to the copy we just created.
	dwOriginalTPTable = *( uintptr_t** ) src;
	*( uintptr_t** ) src = &table[ 1 ];

	// copy original function ptr.
	dwGetBool = ( uintptr_t ) vtbl[ index ];

	// set custom function ptr.
	table[ index + 1 ] = ( uintptr_t ) dst;
}

VMTHook::VMTHook()
{
	memset( this, 0, sizeof( VMTHook ) );
}

VMTHook::VMTHook( PDWORD* ppdwClassBase )
{
	bInitialize( ppdwClassBase );
}

VMTHook::~VMTHook()
{
	UnHook();
}

bool VMTHook::bInitialize( PDWORD* ppdwClassBase )
{
	m_ClassBase = ppdwClassBase;
	m_OldVT = *ppdwClassBase;
	m_VTSize = GetVTCount( *ppdwClassBase );
	m_NewVT = new DWORD[ m_VTSize + 1 ];
	memcpy( &m_NewVT[ 1 ], m_OldVT, sizeof( DWORD ) * m_VTSize );
	m_NewVT[ 0 ] = ( uintptr_t ) m_OldVT[ -1 ];
	DWORD old;
	VirtualProtect( ppdwClassBase, sizeof( DWORD ), PAGE_EXECUTE_READWRITE, &old );
	*ppdwClassBase = &m_NewVT[ 1 ];
	VirtualProtect( ppdwClassBase, sizeof( DWORD ), old, &old );
	return true;
}
bool VMTHook::bInitialize( PDWORD** pppdwClassBase )
{
	return bInitialize( *pppdwClassBase );
}

void VMTHook::UnHook()
{
	if( m_ClassBase )
	{
		DWORD old;
		VirtualProtect( m_ClassBase, sizeof( DWORD ), PAGE_EXECUTE_READWRITE, &old );
		*m_ClassBase = m_OldVT;
		VirtualProtect( m_ClassBase, sizeof( DWORD ), old, &old );
	}
}

void VMTHook::ReHook()
{
	if( m_ClassBase )
	{
		DWORD old;
		VirtualProtect( m_ClassBase, sizeof( DWORD ), PAGE_EXECUTE_READWRITE, &old );
		*m_ClassBase = &m_NewVT[ 1 ];
		VirtualProtect( m_ClassBase, sizeof( DWORD ), old, &old );
	}
}

int VMTHook::iGetFuncCount()
{
	return ( int ) m_VTSize;
}

PDWORD VMTHook::GetOldVT()
{
	return m_OldVT;
}

DWORD VMTHook::HookFunction( DWORD dwNewFunc, unsigned int iIndex )
{
	if( m_NewVT && m_OldVT && iIndex <= m_VTSize && iIndex >= 0 )
	{
		m_NewVT[ iIndex + 1 ] = dwNewFunc;
		return m_OldVT[ iIndex ];
	}

	return NULL;
}
DWORD VMTHook::GetVTCount( PDWORD pdwVMT )
{
	DWORD dwIndex = 0;

	for( dwIndex = 0; pdwVMT[ dwIndex ]; dwIndex++ )
	{
		if( IS_INTRESOURCE( ( FARPROC ) pdwVMT[ dwIndex ] ) )
		{
			break;
		}
	}
	return dwIndex;
}
