#pragma once
#include "main.h"

class Client {
public:
	Client(CActiveSocket& s);

	bool WriteProtectedString(const char* str);
	bool WriteString(const char* str, int length);
	bool WriteInt(int i);
	bool WriteRaw(const uint8* bytes, int size);

	char* ReadProtectedString();
	char* ReadString(int& len);
	int ReadInt();
	uint8* ReadBytes(int size);

private:
	CActiveSocket* m_s;
};