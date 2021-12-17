#pragma once

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <SDKDDKVer.h>
#include <stdio.h>
#include <tchar.h>
#include <winhttp.h>
#include <string>
#include <vector>
#include <sstream>
#include <cstdio>
#include <time.h>
#include <cstring>
#include <iostream>
#include <TlHelp32.h> 
#include <intrin.h>
#include <memory>
#include <fstream>
#include "md5.h"

using namespace std;

template <int X> struct EnsureCompileTime {
	enum : int {
		Value = X
	};
};

//Use Compile-Time as seed
#define compile_seed (	(__TIME__[7] - '0') * 1  + (__TIME__[6] - '0') * 10		+	\
						(__TIME__[4] - '0') * 60   + (__TIME__[3] - '0') * 600	+	\
						(__TIME__[1] - '0') * 3600 + (__TIME__[0] - '0') * 36000	)

constexpr int LinearCongruentGenerator(int Rounds) {
	return 1013904223 + 1664525 * ((Rounds > 0) ? LinearCongruentGenerator(Rounds - 1) : compile_seed & 0xFFFFFFFF);
}

#define Random() EnsureCompileTime<LinearCongruentGenerator(10)>::Value //10 Rounds
#define RandomNumber(Min, Max) (Min + (Random() % (Max - Min + 1)))

template <int... Pack> struct IndexList {};

template <typename IndexList, int Right> struct Append;
template <int... Left, int Right> struct Append<IndexList<Left...>, Right> {
	typedef IndexList<Left..., Right> Result;
};

template <int N> struct ConstructIndexList {
	typedef typename Append<typename ConstructIndexList<N - 1>::Result, N - 1>::Result Result;
};

template <> struct ConstructIndexList<0> {
	typedef IndexList<> Result;
};

template <typename Char, typename IndexList> class XorStringT;
template <typename Char, int... Index> class XorStringT<Char, IndexList<Index...> > {

private:
	Char Value[sizeof...(Index)+1];

	static const Char XORKEY = static_cast<Char>(RandomNumber(0, 0xFFFF));

	template <typename Char>
	constexpr Char EncryptCharacterT(const Char Character, int Index) {
		return Character ^ (XORKEY + Index);
	}

public:
	__forceinline constexpr XorStringT(const Char* const String)
		: Value{ EncryptCharacterT(String[Index], Index)... } {}

	const Char *decrypt() {
		for (int t = 0; t < sizeof...(Index); t++) {
			Value[t] = Value[t] ^ (XORKEY + t);
		}

		Value[sizeof...(Index)] = static_cast<Char>(0);

		return Value;
	}

	const Char *get() {
		return Value;
	}
};

#define XORSTR( String ) (  XorStringT<char,  ConstructIndexList<sizeof( String ) - 1>::Result>( String ).decrypt() )
#define XORWSTR( String ) (  XorStringT<wchar_t,  ConstructIndexList<(sizeof( String ) - 1) / 2>::Result>( String ).decrypt() )


//class my_license
//{
//	typedef void(_stdcall *GSI)(LPSYSTEM_INFO);
//public:
//	const char
//		*user_name = new char,
//		*user_password = new char,
//		*user_key = new char;
//
//	virtual std::string generate_key();
//	virtual std::string GetRec(std::string, int);
//
//	virtual void win32_copy_to_clipboardW(char*);
//	virtual bool check_license(std::string, std::string);
//	std::string
//		host = "weave.su/",
//		path = "cheat/",
//		host_gate_login = "license-test.php?login=",
//		host_gate_password = "&password=",
//		host_gate_system_key = "&binding=",
//		host_gate_token = "&token=",
//		host_gate_inject = "&inject=1",
//		base64_chars =
//		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
//		"abcdefghijklmnopqrstuvwxyz"
//		"0123456789+/";
//
//private:
//	virtual DWORD get_system_info();
//	virtual char* string_to_hex(const char * input);
//	virtual char* convert_key_to_char(const char*, const char*, DWORD);
//
//	virtual std::string get_hash_system_key();
//	virtual std::string str_to_hex(const std::string);
//	virtual std::string get_hash_text(const void*, const size_t);
//	virtual std::string base64_encode(char const*, unsigned int);
//
//	SYSTEM_INFO sysinfo;
//	DWORD system_key = NULL;
//	HGLOBAL hMem = NULL;
//	UINT uFormat = NULL;
//};
//unique_ptr<my_license> my_lic(new my_license());
//
//class MD5
//{
//public:
//	typedef unsigned int size_type;
//
//	MD5();
//	MD5(const std::string&);
//	virtual MD5& finalize();
//	virtual void update(const unsigned char *, size_type);
//	virtual void update(const char *, size_type);
//	virtual 	std::string hexdigest() const;
//	friend std::ostream& operator<<(std::ostream&, MD5);
//
//private:
//	void init();
//	typedef unsigned char uint1;
//	typedef unsigned int uint4;
//	enum { blocksize = 64 };
//
//	virtual void transform(const uint1 block[blocksize]);
//	static void decode(uint4 output[], const uint1 input[], size_type len);
//	static void encode(uint1 output[], const uint4 input[], size_type len);
//
//	bool finalized;
//	uint1 buffer[blocksize];
//	uint4 count[2];
//	uint4 state[4];
//	uint1 digest[16];
//
//
//	static inline uint4 F(uint4 x, uint4 y, uint4 z);
//	static inline uint4 G(uint4 x, uint4 y, uint4 z);
//	static inline uint4 H(uint4 x, uint4 y, uint4 z);
//	static inline uint4 I(uint4 x, uint4 y, uint4 z);
//	static inline uint4 rotate_left(uint4 x, int n);
//	static inline void FF(uint4 &a, uint4 b, uint4 c, uint4 d, uint4 x, uint4 s, uint4 ac);
//	static inline void GG(uint4 &a, uint4 b, uint4 c, uint4 d, uint4 x, uint4 s, uint4 ac);
//	static inline void HH(uint4 &a, uint4 b, uint4 c, uint4 d, uint4 x, uint4 s, uint4 ac);
//	static inline void II(uint4 &a, uint4 b, uint4 c, uint4 d, uint4 x, uint4 s, uint4 ac);
//};
//
//std::string md5(const std::string str);
//
//
//class CHTTPSession
//{
//private:
//	HINTERNET hSession, hConnect, hRequest;
//	LPCWSTR wcUserAgent;
//	LPCWSTR wcHTTPVerb;
//
//	int iHTTPMethod;
//	std::wstring wsDomain, wsDocument;
//	std::string sPOSTData, sPOSTFile;
//	int iSESSION_PROTOCOL, iSESSION_PORT;
//private:
//	virtual bool SetHTTPSecuirty(int);
//	virtual bool CreateSession();
//	virtual	bool SetServer(LPCWSTR, INTERNET_PORT);
//	virtual bool SetRequest(LPCWSTR);
//	virtual bool SendRequest();
//	virtual bool SetResponseHandle();
//	virtual bool HTTPPhraseData(std::string, std::string);
//
//	void CloseHandles();
//
//public:
//	virtual std::wstring GetResponseHeader();
//	virtual int GetHTTPStatusCode();
//	virtual bool ReciveData(std::string&);
//	virtual bool SetHTTPMethod(int);
//
//	virtual bool Send(LPCWSTR, LPCWSTR);
//	virtual bool Recv(int&, std::string&, std::string&);
//	virtual int Transmit(int, int, std::string, std::string&, std::string, int);
//
//
//	CHTTPSession()
//	{
//		wcUserAgent = L"MyUserAgent/1.0";
//	}
//
//	CHTTPSession(LPCWSTR lwcUserAgent)
//	{
//		wcUserAgent = lwcUserAgent;
//	}
//
//	~CHTTPSession()
//	{
//		CloseHandles();
//	}
//
//	void AddPOSTFile(std::string sData)
//	{
//		sPOSTFile = sData;
//	}
//};
//
//enum CHTTP_PROTOCOLID
//{
//	HTTP_CLEAR = 10,
//	HTTP_SSL
//};
//
//enum CHTTP_METHODID
//{
//	HTTP_GET = 10,
//	HTTP_POST
//};
//
//class get_information
//{
//public:
//	virtual ~get_information() = 0 {}
//	virtual std::string get_time(std::string host, std::string path) = 0;
//	virtual std::string get_inject(std::string host, std::string path) = 0;
//	virtual std::string get_download_url(std::string url, std::string host_gate_inject) = 0;
//	virtual void gen_key(std::string login, std::string password) = 0;
//	virtual std::string gen_token(std::string login, std::string password, std::string key, std::string url_time) = 0;
//	virtual	std::string get_gen_url(std::string host, std::string  path, std::string host_gate_login, std::string user_name, std::string host_gate_password, std::string user_password,
//		std::string host_gate_system_key, std::string generate_key, std::string host_gate_token, std::string get_token) = 0;
//
//	virtual int URLDownloadBinary(LPCSTR lpszURL, BYTE **ppBuffer, DWORD *pdwSize) = 0;
//};
//
//class get_info : public get_information
//{
//public:
//	auto get_time(std::string host, std::string path) -> std::string
//	{
//		std::string url_time = "";
//		return  url_time = host + path + XORSTR("time.php");
//	}
//
//
//	auto get_download_url(std::string url, std::string host_gate_inject)-> std::string
//	{
//		string add;
//		std::string	download_dll = "http://" + url + host_gate_inject;
//		return  download_dll;
//	}
//
//	auto get_inject(std::string url, std::string host_gate_inject)-> std::string
//	{
//		std::string url_inject = "";
//		return  url_inject = XORSTR("http://") + url + host_gate_inject;
//	}
//
//	auto gen_key(std::string login, std::string password) -> void
//	{
//		std::string g_key = "";
//
//		my_lic->user_name = login.c_str();
//		my_lic->user_password = login.c_str();
//		g_key = my_lic->generate_key();
//
//	}
//
//	auto gen_token(std::string login, std::string password, std::string key, std::string url_time)-> std::string
//	{
//		std::string	_tkn_one = md5(login).substr(0, 15),
//			_tkn_two = md5(password).substr(0, 15),
//			_tkn_three = md5(key).substr(0, 15),
//			_tkn_four = md5(my_lic->GetRec(url_time, HTTP_CLEAR)).substr(0, 15);
//
//		std::string _general_tkn = _tkn_one + _tkn_two + _tkn_three + _tkn_four;
//		_general_tkn = md5(_general_tkn);
//
//		return _general_tkn;
//	}
//
//
//	auto get_gen_url(std::string host, std::string  path, std::string host_gate_login, std::string user_name, std::string host_gate_password, std::string user_password,
//		std::string host_gate_system_key, std::string generate_key, std::string host_gate_token, std::string get_token)-> std::string
//	{
//
//		std::string gen_url = host + path + host_gate_login + user_name + host_gate_password + user_password + host_gate_system_key + generate_key + host_gate_token + get_token;
//
//		return gen_url;
//	}
//
//	auto URLDownloadBinary(LPCSTR lpszURL, BYTE **ppBuffer, DWORD *pdwSize) -> int
//	{
//
//		LPSTREAM lpStream = 0;
//		BYTE *pResult = 0;
//		int nResult = 0;
//
//		if (lpszURL && SUCCEEDED(URLOpenBlockingStream(NULL, lpszURL, &lpStream, 0, NULL)))
//		{
//			STATSTG statStream;
//			if (SUCCEEDED(lpStream->Stat(&statStream, STATFLAG_NONAME)))
//			{
//				DWORD dwSize = statStream.cbSize.LowPart;
//				pResult = (BYTE *)malloc(dwSize);
//				if (pResult)
//				{
//					LARGE_INTEGER liPos;
//					ZeroMemory(&liPos, sizeof(liPos));
//					lpStream->Seek(liPos, STREAM_SEEK_SET, NULL);
//					lpStream->Read(pResult, dwSize, NULL);
//					*ppBuffer = pResult;
//					*pdwSize = dwSize;
//					nResult = 1;
//				}
//			}
//			lpStream->Release();
//		}
//
//		return nResult;
//	}
//
//};
//
//unique_ptr<get_information> g_info(new get_info());