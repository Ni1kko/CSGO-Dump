#pragma once

class ServerClass
{
public:
	const char* m_pNetworkName;
	void** m_pTable;
	ServerClass* m_pNext;
	int							m_ClassID;
	int							m_InstanceBaselineIndex;
};