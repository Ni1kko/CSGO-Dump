#pragma once
//////////////////////////////////////////////////////////////
//				  WINAPI & C++ BASIC INCLUDES				//
//////////////////////////////////////////////////////////////
#include <WinSock2.h>
#include <Windows.h>
#include <string>
#include <iostream>
#include <map>
#include <Psapi.h>
#include <thread>
#include <vector>

#pragma comment(lib, "winmm.lib")

//////////////////////////////////////////////////////////////
//				    DIRECT X 9.0c INCLUDES					//
//////////////////////////////////////////////////////////////
#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3dx9.lib")

//////////////////////////////////////////////////////////////
//						DEFINED STUFF						//
//////////////////////////////////////////////////////////////
typedef void* ( *CreateInterfaceFn )( const char* pName, int* pReturnCode );

#define	SURF_SKY		0x0004
#define BONE_USED_BY_HITBOX	0x00000100
#define BONE_USED_BY_ANYTHING   0x0007FF00

#pragma warning(disable: 4838 4244 4390 4305)

typedef struct _UNICODE_STRING
{
	USHORT Length;
	USHORT MaximumLength;
	PWCH   Buffer;
} UNICODE_STRING;
typedef UNICODE_STRING* PUNICODE_STRING;

typedef struct _PEB_LDR_DATA
{
	ULONG           Length;
	BOOLEAN         Initialized;
	PVOID           SsHandle;
	LIST_ENTRY      InLoadOrderModuleList;
	LIST_ENTRY      InMemoryOrderModuleList;
	LIST_ENTRY      InInitializationOrderModuleList;
} PEB_LDR_DATA, *PPEB_LDR_DATA;

typedef struct _LDR_MODULE
{
	LIST_ENTRY      InLoadOrderModuleList;
	LIST_ENTRY      InMemoryOrderModuleList;
	LIST_ENTRY      InInitializationOrderModuleList;
	PVOID           BaseAddress;
	PVOID           EntryPoint;
	ULONG           SizeOfImage;
	UNICODE_STRING  FullDllName;
	UNICODE_STRING  BaseDllName;
	ULONG           Flags;
	SHORT           LoadCount;
	SHORT           TlsIndex;
	LIST_ENTRY      HashTableEntry;
	ULONG           TimeDateStamp;
} LDR_MODULE, *PLDR_MODULE;

#define HITGROUP_GENERIC    0
#define HITGROUP_HEAD       1
#define HITGROUP_CHEST      2
#define HITGROUP_STOMACH    3
#define HITGROUP_LEFTARM    4
#define HITGROUP_RIGHTARM   5
#define HITGROUP_LEFTLEG    6
#define HITGROUP_RIGHTLEG   7
#define HITGROUP_GEAR       10

#define VK_CHAR_0 0x30
#define VK_CHAR_1 0x31
#define VK_CHAR_2 0x32
#define VK_CHAR_3 0x33
#define VK_CHAR_4 0x34
#define VK_CHAR_5 0x35
#define VK_CHAR_6 0x36
#define VK_CHAR_7 0x37
#define VK_CHAR_8 0x38
#define VK_CHAR_9 0x39
#define VK_CHAR_A 0x41
#define VK_CHAR_B 0x42
#define VK_CHAR_C 0x43
#define VK_CHAR_D 0x44
#define VK_CHAR_E 0x45
#define VK_CHAR_F 0x46
#define VK_CHAR_G 0x47
#define VK_CHAR_H 0x48
#define VK_CHAR_I 0x49
#define VK_CHAR_G 0x4A
#define VK_CHAR_K 0x4B
#define VK_CHAR_L 0x4C
#define VK_CHAR_M 0x4D
#define VK_CHAR_N 0x4E
#define VK_CHAR_O 0x4F
#define VK_CHAR_P 0x50
#define VK_CHAR_Q 0x51
#define VK_CHAR_R 0x52
#define VK_CHAR_S 0x53
#define VK_CHAR_T 0x54
#define VK_CHAR_U 0x55
#define VK_CHAR_V 0x56
#define VK_CHAR_W 0x57
#define VK_CHAR_X 0x58
#define VK_CHAR_Y 0x59
#define VK_CHAR_Z 0x5A
