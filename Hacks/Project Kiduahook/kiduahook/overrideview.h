#pragma once

#include "main.h"

class CViewSetup
{
public:
	char _0x0000[ 16 ];
	__int32 x; //0x0010 
	__int32 x_old; //0x0014 
	__int32 y; //0x0018 
	__int32 y_old; //0x001C
	__int32 width; //0x0020
	__int32	width_old; //0x0024
	__int32 height; //0x0028
	__int32	height_old; //0x002C
	char _0x0030[ 128 ];
	float fov; //0x00B0 
	float fovViewmodel; //0x00B4 
	Vector origin; //0x00B8 
	Vector angles; //0x00C4
	float zNear; //0x00D0 
	float zFar; //0x00D4 
	float zNearViewmodel; //0x00D8 
	float zFarViewmodel; //0x00DC 
	float m_flAspectRatio; //0x00E0 
	float m_flNearBlurDepth; //0x00E4 
	float m_flNearFocusDepth; //0x00E8 
	float m_flFarFocusDepth; //0x00EC 
	float m_flFarBlurDepth; //0x00F0 
	float m_flNearBlurRadius; //0x00F4 
	float m_flFarBlurRadius; //0x00F8 
	float m_nDoFQuality; //0x00FC 
	__int32 m_nMotionBlurMode; //0x0100 
	char _0x0104[ 68 ];
	__int32 m_EdgeBlur; //0x0148 

};//Size=0x014C

typedef void( __thiscall* OverrideViewFn_t )( void*, CViewSetup* );
extern OverrideViewFn_t oOverrideView;

void __fastcall HOOKED_OverrideView( void* thisptr, void* edx, CViewSetup* setup );