#pragma once

#include "main.h"
#include "imaterial.h"

typedef void( __thiscall* DrawModelExecuteFn )( void*, void*, void*, const ModelRenderInfo_t&, matrix3x4_t* );
extern DrawModelExecuteFn oDrawModel;
extern bool m_bDrawingBacktrackChams, m_bDrawingFakeChams;

void __fastcall HOOKED_DrawModel( void*thisptr, int edx, void*context, void*state, const ModelRenderInfo_t &info, matrix3x4_t *customBoneToWorld );

int __fastcall HOOKED_ListLeavesInBox(void* bsp, void* edx, Vector& mins, Vector& maxs, unsigned short* pList, int listMax);

void HookAddRenderAble();