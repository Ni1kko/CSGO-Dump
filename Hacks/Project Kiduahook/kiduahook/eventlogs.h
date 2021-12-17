#pragma once

#include "sdk.h"

class CEventText
{
public:
	CEventText() {}

	CEventText( std::string text, Color color = Color( 255, 255, 255 ) )
	{
		szTexts.push_back( text );
		vColor.push_back( color );
	}

	void PushBack( std::string text, Color color = Color( 255, 255, 255 ) )
	{
		szTexts.push_back( text );
		vColor.push_back( color );
	}

	void PushFront( std::string text, Color color = Color( 255, 255, 255 ) )
	{
		szTexts.insert( szTexts.begin(), text );
		vColor.insert( vColor.begin(), color );
	}

	void ChangeAlpha( int alpha )
	{
		for( auto& color : vColor )
			color._color[ 3 ] = alpha;
	}

	void Draw( int x, int y, unsigned int font );

	std::string Text()
	{
		std::string szText;
		for( const auto szString : szTexts )
			szText += szString;

		return szText;
	}

private:
	std::vector<std::string> szTexts;
	std::vector<Color> vColor;
};

class CEventLogs
{
public:
	struct sLog
	{
		sLog()
		{
			flTime = g_pGlobalVars->curtime;
		}

		sLog( std::string text, Color color = Color( 255, 255, 255 ) )
		{
			EventText = CEventText( text, color );
			flTime = g_pGlobalVars->curtime;
		}

		CEventText EventText;
		float flTime;
	};

public:
	void Init();

	void AddLog( sLog log )
	{
		vLog.insert( vLog.begin(), log );
	}

private:
	std::vector<sLog> vLog;

private:
	const float flTextTime = 5.f;
	const float flFadeIn = 0.3f;
	const float flFadeOut = 0.2f;

	const int iMaxLines = 10;

	const int iIdealHeight = 5;
	const int iIdealWidth = 5;

	const int flInDistance = 20;
	const int flOutDistance = 20;
	const float flOutSpeed = 0.2f;
};

extern CEventLogs g_EventLogs;