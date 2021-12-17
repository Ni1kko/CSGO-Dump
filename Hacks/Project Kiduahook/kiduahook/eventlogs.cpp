#include "eventlogs.h"

CEventLogs g_EventLogs;

void CEventText::Draw( int x, int y, unsigned int font )
{
	for( int i = 0; i < szTexts.size(); i++ )
	{
		g_Draw.DrawString( font, false, x, y, vColor[ i ].r(), vColor[ i ].g(), vColor[ i ].b(), vColor[ i ].a(), szTexts[ i ].c_str() );
		x += g_Draw.getWidth( font, szTexts[ i ].c_str() );
	}
}

void CEventLogs::Init()
{
	if( vLog.size() > iMaxLines )
		vLog.erase( vLog.begin() + iMaxLines, vLog.end() );

	for( int i = 0; i < vLog.size(); i++ )
	{
		CEventLogs::sLog EventLogs = vLog[ i ];

		float flDeltaTime = fabs( g_pGlobalVars->curtime - EventLogs.flTime );

		if( flDeltaTime > flTextTime )
		{
			vLog.erase( vLog.begin() + i );
			break;
		}

		int iHeight = iIdealHeight + ( 16 * i );

		if( flDeltaTime > flTextTime - flOutSpeed )
			iHeight = iHeight + ( ( ( flOutSpeed - ( flTextTime - flDeltaTime ) ) / flOutSpeed ) * flOutDistance );

		if( flDeltaTime > flTextTime - flFadeOut )
			EventLogs.EventText.ChangeAlpha( 255 - ( ( ( flDeltaTime - ( flTextTime - flFadeOut ) ) / flFadeOut ) * 255.f ) );

		if( flDeltaTime < flFadeIn )
			EventLogs.EventText.ChangeAlpha( ( flDeltaTime / flFadeIn ) * 255.f );

		int iWidth = iIdealWidth;

		if( flDeltaTime < flFadeIn )
			iHeight = ( flDeltaTime / flFadeIn ) * static_cast<float>( flInDistance ) + ( iIdealWidth - flInDistance );

		if( flDeltaTime > flTextTime - flFadeOut )
			iHeight = iIdealWidth - ( ( ( flDeltaTime - ( flTextTime - flFadeOut ) ) / flFadeOut ) * static_cast<float>( flOutDistance ) );

		EventLogs.EventText.Draw( iWidth, iHeight, g_Draw.keyfont );
	}
}