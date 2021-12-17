#include "Xor.h"
#include <string>

char* Xor::EncryptDecrypt(const char v[], const char* k, int length) {
	int size = length;
	int keySize = strlen(k);

	char* n = new char[size];
	memset(n, 0, size);

	for (int i = 0; i < size; i++)
		n[i] = (v[i] ^ k[i % keySize]);

	return n;
}