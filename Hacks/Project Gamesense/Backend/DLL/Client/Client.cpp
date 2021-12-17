#include "Client.h"

Client::Client(CActiveSocket& s) {
	this->m_s = &s;
}

bool Client::WriteString(const char* s, int length) {
	return this->WriteInt(length) &&
		this->WriteRaw((const uint8*)s, length);
}

bool Client::WriteProtectedString(const char* s) {
	time_t t = time(NULL);
	tm pTime;
	localtime_s(&pTime, &t);

	const char* alphabet = "ABCDEFGHIJKLabcdefnopqr1234stuvwxyzMNOPQRSTUVWXYZ056789_-gijklm";
	char* rkey = new char[32];

	char* tmpKey = new char[129];
	for (int i = 0; i < 128; i++)
		tmpKey[i] = alphabet[(i ^ (pTime.tm_mon + 1)) * (i + 1) % strlen(alphabet)];
	tmpKey[128] = 0;

	char* tmpSha = new char[41];
	sha1(tmpKey).add("8cecaf525730a1ce5b45b788d1ac55c5").finalize().print_hex(tmpSha);
	std::string shrinked = std::string(tmpSha).substr(0, 32);
	rkey = (char*)shrinked.c_str();

	Xor x;
	char* xorEncrypted = x.EncryptDecrypt(s, rkey, strlen(s));
	return this->WriteString(xorEncrypted, strlen(s));
}

bool Client::WriteInt(int i) {
	uint8 tmp[4] = {};
	memcpy(tmp, &i, 4);

	return this->WriteRaw(tmp, 4);
}

bool Client::WriteRaw(const uint8* b, int size) {
	return this->m_s->Send(b, size);
}

char* Client::ReadString(int& len) {
	int d = this->ReadInt();
	len = d;
	return (char*)this->ReadBytes(d);
}

char* Client::ReadProtectedString() {
	int recvlen = 0;
	char* prot = this->ReadString(recvlen);

	time_t t = time(NULL);
	tm pTime;
	localtime_s(&pTime, &t);

	const char* alphabet = "ABCDEFGHIJKLabcdefnopqr1234stuvwxyzMNOPQRSTUVWXYZ056789_-gijklm";
	char* rkey = new char[32];

	char* tmpKey = new char[129];
	for (int i = 0; i < 128; i++)
		tmpKey[i] = alphabet[(i ^ (pTime.tm_mon + 1)) * (i + 1) % strlen(alphabet)];
	tmpKey[128] = 0;

	char* tmpSha = new char[41];
	sha1(tmpKey).add("8cecaf525730a1ce5b45b788d1ac55c5").finalize().print_hex(tmpSha);
	std::string shrinked = std::string(tmpSha).substr(0, 32);
	rkey = (char*)shrinked.c_str();

	Xor x;
	return x.EncryptDecrypt(prot, rkey, recvlen);
}

int Client::ReadInt() {
	int i = 0;
	memcpy(&i, this->ReadBytes(4), 4);

	return i;
}

uint8* Client::ReadBytes(int r) {
	try {
		char* data = new char[r + 1];
		memset(data, 0, r + 1);

		int maxBufferSize = 8192;

		auto bytesReceived = decltype(r){0};
		while (r > 0)
		{
			const auto bytesRequested = (r > maxBufferSize) ? maxBufferSize : r;
			const auto returnValue = recv(this->m_s->GetSocketDescriptor(), data + bytesReceived, bytesRequested, 0);
			if (returnValue == -1 || returnValue == 0)
				return (uint8*)data;

			bytesReceived += returnValue;
			r -= returnValue;
		}

		return (uint8*)data;
	}
	catch (...) {
		return 0;
	}
}