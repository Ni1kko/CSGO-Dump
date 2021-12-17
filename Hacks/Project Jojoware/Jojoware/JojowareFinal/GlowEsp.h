#pragma once
//change: Glow.cpp , glow.h
#include "Includes.h"
#include "ReadProcessMemory.h"
#include "HackProcess.h"
#include "CheatManager.h"
class Glow
{
public:
	void GlowEsp();
	struct GlowStruct
	{
		float r;
		float g;
		float b;
		float a;
		char m_unk[4];
		float m_flUnk;
		float m_flBloomAmount;
		float m_flUnk1;
		bool m_bRenderWhenOccluded;
		bool m_bRenderWhenUnoccluded;
		bool m_bFullBloomRender;
		char m_unk1;
		int m_nFullBloomStencilTestValue;
		int m_nGlowStyle;
		int m_nSplitScreenSlot;
		int m_nNextFreeSlot;
		
		
	};
	GlowStruct EnemyGlow;
	void SetGlowStyle(int s)
	{
		EnemyGlow.m_nGlowStyle = s;
	}
	void ChangeYellowGlow()
	{
		EnemyGlow.r = 1;
		EnemyGlow.g = 1;
		EnemyGlow.b = 0;
	}
	void ChangeGreenGlow()
	{
		EnemyGlow.r = 0;
		EnemyGlow.g = 1;
		EnemyGlow.b = 0;
	}
	void ChangeRedGlow()
	{
		EnemyGlow.r = 1;
		EnemyGlow.g = 0;
		EnemyGlow.b = 0;
	}
}; extern Glow G;