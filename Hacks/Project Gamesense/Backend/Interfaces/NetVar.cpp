#include "NetVar.h"
#include "Interfaces.h"

using namespace Cheat;

CNetVarManager* Cheat::NetVarManager = nullptr;
offsets Cheat::offys;

void CNetVarManager::init()
{
	tables.clear();
	ClientClass* client = Interfaces::Client->GetAllClasses();

	if ( !client )
		return;

	while ( client )
	{
		auto recvTable = client->m_pRecvTable;

		if ( recvTable )
			tables.emplace( std::string( client->m_pNetworkName ), recvTable );

		client = client->m_pNext;
	}
}

int CNetVarManager::getOffs( const char* tableName, const char* propName )
{
	int offs = this->getProp( tableName, propName );

	if (!offs)
	{
		printf("%s %s\n", tableName, propName);
		return 0;
	}

	return offs;
}

bool CNetVarManager::hookProp( const char* tableName, const char* propName, RecvVarProxyFn func )
{
	RecvProp* prop = nullptr;
	this->getProp( tableName, propName, &prop );

	if ( !prop )
		return false;

	prop->m_ProxyFn = func;
	return true;
}

bool CNetVarManager::hookProp( const char* tableName, const char* propName, RecvVarProxyFn func, RecvVarProxyFn &orig )
{
	RecvProp* prop = nullptr;
	this->getProp( tableName, propName, &prop );

	if ( !prop )
		return false;

	orig = reinterpret_cast<RecvVarProxyFn>( prop->m_ProxyFn );
	prop->m_ProxyFn = func;
	return true;
}

int CNetVarManager::getProp( const char* tableName, const char* propName, RecvProp** prop )
{
	RecvTable* recvTable = this->getTable( tableName );

	if ( !recvTable )
		return 0;

	int offs = this->getProp( recvTable, propName, prop );

	if ( !offs )
		return 0;

	return offs;
}

int CNetVarManager::getProp( RecvTable* recvTable, const char* propName, RecvProp** prop )
{
	int extrOffs = 0;

	for ( int i = 0; i < recvTable->m_nProps; i++ )
	{
		auto* recvProp = &recvTable->m_pProps[i];
		auto recvChild = recvProp->m_pDataTable;

		if ( recvChild && ( recvChild->m_nProps > 0 ) )
		{
			int tmp = this->getProp( recvChild, propName, prop );

			if ( tmp )
				extrOffs += ( recvProp->m_Offset + tmp );
		}

		if ( strcmp( recvProp->m_pVarName, propName ) )
			continue;

		if ( prop )
			*prop = recvProp;

		return ( recvProp->m_Offset + extrOffs );
	}

	return extrOffs;
}

RecvTable* CNetVarManager::getTable( const char* tableName )
{
	if ( tables.empty() )
		return 0;

	for ( auto table : tables )
	{
		if ( strcmp( table.first.c_str(), tableName ) == 0 )
			return table.second;
	}

	return 0;
}
