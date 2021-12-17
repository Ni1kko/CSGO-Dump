#include "gui.h"

CGui g_Gui;
bool bKeyPressed [ 256 ];

CGui::CGui()
{
	for( int i = 0; i <= 60; i++ )
	{
		sColorPicker [ i ].bOpen = false;
		sColorPicker [ i ].bActive = true;
		sComboBox [ i ].bOpen = false;
		sComboBox [ i ].bActive = true;
		sMultiComboBox[ i ].bOpen = false;
		sMultiComboBox[ i ].bActive = true;
		sKeyBox[ i ].bOpen = false;
		sKeyBox[ i ].bActive = true;
		sNumberInput[ i ].bOpen = false;
		sNumberInput[ i ].bActive = true;
	}

	bCursorActive = false;
	bInComboBox = false;
	bInMultiCombo = false;
	bInColorPick = false;
	bInGetText = false;
	bInDetectKey = false;
	bInNumberInput = false;

	iCursorPos [ 0 ] = 0;
	iCursorPos [ 1 ] = 0;

	bLClick = false;
	bLeftClickReleased = false;
	bRClick = false;
	bRightClickReleased = false;

	bUpScroll = false;
	bDownScroll = false;

	bSecUpScroll = false;
	bSecDownScroll = false;
}

void CGui::DrawMouse()
{
	if( !bCursorActive )
		return;

	g_pSurface->SurfaceGetCursorPos( iCursorPos [ 0 ], iCursorPos [ 1 ] );

	int r, g, b;

	if( bLClick )
		r = g_Var.Color.Menu [ 0 ], g = g_Var.Color.Menu [ 1 ], b = g_Var.Color.Menu [ 2 ];
	else
		r = 255, g = 255, b = 255;

	g_Draw.FillRGBA( iCursorPos [ 0 ] + 6, iCursorPos [ 1 ] + 6, 1, 6, r, g, b, 255 );
	g_Draw.FillRGBA( iCursorPos [ 0 ] + 7, iCursorPos [ 1 ] + 7, 1, 5, r, g, b, 255 );

	for( int i = 0; i < 4; i++ )
	{
		g_Draw.FillRGBA( iCursorPos [ 0 ] + 2 + i, iCursorPos [ 1 ] + 2 + i, 1, 14 - ( i * 2 ), r, g, b, 255 );
		g_Draw.FillRGBA( iCursorPos [ 0 ] + 8 + i, iCursorPos [ 1 ] + 8 + i, 1, 4 - i, r, g, b, 255 );
	}
}

bool IsMenuKey( int iKey )
{
	if( GetAsyncKeyState( iKey ) )
	{
		if( !bKeyPressed [ iKey ] )
		{
			bKeyPressed [ iKey ] = true;
			return true;
		}
	}
	else
		bKeyPressed [ iKey ] = false;

	return false;
}

void CGui::GetMouse()
{
	if( !bCursorActive )
		return;

	if( GetAsyncKeyState( VK_LBUTTON ) )
		bLClick = true;
	else
	{
		if( bLClick )
			bLeftClickReleased = true;
		else
			bLeftClickReleased = false;

		bLClick = false;
	}

	if( GetAsyncKeyState( VK_RBUTTON ) )
		bRClick = true;
	else
	{
		if( bRClick )
			bRightClickReleased = true;
		else
			bRightClickReleased = false;

		bRClick = false;
	}
}

bool CGui::MouseOver( int x, int y, int w, int h )
{
	if( ( iCursorPos [ 0 ] > x ) && ( iCursorPos [ 0 ] < x + w ) && ( iCursorPos [ 1 ] > y ) && ( iCursorPos [ 1 ] < y + h ) )
		return true;

	return false;
}

bool CGui::LeftClick( int x, int y, int w, int h )
{
	if( bLeftClickReleased && ( iCursorPos [ 0 ] > x ) && ( iCursorPos [ 0 ] < x + w ) && ( iCursorPos [ 1 ] > y ) && ( iCursorPos [ 1 ] < y + h ) )
		return true;

	return false;
}

void CGui::Save()
{

}

void CGui::Load()
{

}

void CGui::DrawCheckbox( int x, int y, int& value, const char* text, int alpha, int distance )
{
	g_Draw.DrawString( g_Draw.visualsfont, false, x, y, 200, 200, 200, clamp( alpha, 0, 150 ), text );

	if( LeftClick( x + distance + 1, y + 1, 10, 10 ) && !bInComboBox && !bInDetectKey && !bInColorPick && !bInMultiCombo && !bInNumberInput )
	{
		if( value != 1 )
			value = 1;
		else
			value = 0;
	}

	if( value )
		g_Draw.DrawGradient( x + distance + ( float )3.5, y + 3, 8, 8, g_Var.Color.Menu [ 0 ], g_Var.Color.Menu [ 1 ], g_Var.Color.Menu [ 2 ], alpha, g_Var.Color.Menu [ 0 ] / 2, g_Var.Color.Menu [ 1 ] / 3, g_Var.Color.Menu [ 2 ] / 4, alpha );

	g_Draw.DrawOutlineRect( x + distance, y, 14, 14, 70, 70, 70, clamp( alpha, 0, 180 ) );

	if( MouseOver( x + distance + 1, y + 1, 10, 10 ) && !bLClick )
		g_Draw.DrawOutlineRect( x + distance + 1, y + 1, 12, 12, g_Var.Color.Menu [ 0 ], g_Var.Color.Menu [ 1 ], g_Var.Color.Menu [ 2 ], alpha );
}

Color GetColorFromPos( int x, int y, int w, int h )
{
	int iX = ( g_Gui.iCursorPos [ 0 ] - x );
	int iY = ( g_Gui.iCursorPos [ 1 ] - y );
	int div = w / 6;
	int phase = iX / div;
	float t = ( iX % div ) / ( float ) div;
	int r, g, b;

	switch( phase )
	{
	case( 0 ):
		r = 255;
		g = 255 * t;
		b = 0;
		break;
	case( 1 ):
		r = 255 * ( 1.f - t );
		g = 255;
		b = 0;
		break;
	case( 2 ):
		r = 0;
		g = 255;
		b = 255 * t;
		break;
	case( 3 ):
		r = 0;
		g = 255 * ( 1.f - t );
		b = 255;
		break;
	case( 4 ):
		r = 255 * t;
		g = 0;
		b = 255;
		break;
	case( 5 ):
		r = 255;
		g = 0;
		b = 255 * ( 1.f - t );
		break;
	}

	float sat = iY / ( float ) h;
	return Color( r + sat * ( 255 - r ), g + sat * ( 255 - g ), b + sat * ( 255 - b ) );
}

void CGui::DrawColorPicker( int x, int y, int w, int& r, int& g, int& b, int num, const char* text, int distancecolor, int alpha )
{
	static bool bInit;
	static float flX [ 40 ], flY [ 40 ], flYBright [ 40 ];
	static bool bDidPos [ 40 ];

	g_Draw.DrawString( g_Draw.visualsfont, false, x, y, 200, 200, 200, clamp( alpha, 0, 150 ), text );

	g_Draw.DrawGradient( x + distancecolor + ( float )3.5, y + 3, w - 2, 8, r, g, b, alpha, r / 2, g / 3, b / 4, alpha );
	g_Draw.DrawOutlineRect( x + distancecolor, y, w + 4, 14, 70, 70, 70, clamp( alpha, 0, 180 ) );

	if( MouseOver( x + distancecolor, y, w, 10 ) && !bLClick )
		g_Draw.DrawOutlineRect( x + distancecolor + 1, y + 1, w + 2, 12, g_Var.Color.Menu [ 0 ], g_Var.Color.Menu [ 1 ], g_Var.Color.Menu [ 2 ], alpha );

	if( sColorPicker [ num ].bActive )
	{
		if( !bInDetectKey && !bInComboBox && !bInMultiCombo && !bInNumberInput )
		{
			if( MouseOver( x + distancecolor, y, w, 10 ) && bLeftClickReleased && !bInColorPick )
			{
				sColorPicker [ num ].bOpen = true;
				sColorPicker [ num ].bActive = false;
			}
		}
	}
	else
	{
		if( sColorPicker [ num ].bOpen )
		{
			bInColorPick = true;

			static int r2, g2, b2;
			int sizew = 120, sizeh = 80;

			if( !bDidPos [ num ] )
			{
				flX [ num ] = x - 1000;
				flY [ num ] = y - 1000;

				flYBright [ num ] = y - 1000;
				bDidPos [ num ] = true;
			}

			g_Draw.FillRGBA( x + distancecolor - sizew - 1 + w, y + 16 - 1, sizew + 11, sizeh + 2, 30, 30, 30, alpha );

			Color color = GetColorFromPos( x + distancecolor - sizew + w, y + 16, sizew, sizeh );
			g_Draw.DrawColorPalette( x + distancecolor - sizew + w, y + 16, sizew, sizeh );

			g_Draw.DrawOutlineRect( x + distancecolor - sizew - 1 + w, y + 16 - 1, sizew + 11, sizeh + 2, 70, 70, 70, clamp( alpha, 0, 180 ) );

			if( bInit )
			{
				r2 = r;
				g2 = g;
				b2 = b;
				bInit = false;
			}

			if( MouseOver( x + distancecolor - sizew + w, y + 16, sizew, sizeh ) && bLClick )
			{
				r = color.r();
				g = color.g();
				b = color.b();
				r2 = color.r();
				g2 = color.g();
				b2 = color.b();
				flX [ num ] = clamp( iCursorPos [ 0 ] - x, iCursorPos [ 0 ] - x, iCursorPos [ 0 ] - x + sizew );
				flY [ num ] = clamp( iCursorPos [ 1 ] - y, iCursorPos [ 1 ] - y, iCursorPos [ 1 ] - y + sizeh );
			}

			g_Draw.DrawOutlineRect( x + flX [ num ], y + flY [ num ], 4, 4, 30, 30, 30, alpha );

			int r1, g1, b1;
			for( int i = 0; i < sizeh; i++ )
			{
				int div = sizeh / 2;
				int phase = i / div;
				float t = ( i % div ) / ( float ) div;

				switch( phase )
				{
					/*		case( 0 ): // white -> color ?
					r1 = r2 * t;
					g1 = g2 * t;
					b1 = b2 * t;
					break;*/
				case( 0 ):
					r1 = r2;
					g1 = g2;
					b1 = b2;
					break;
				case( 1 ):
					r1 = r2 * ( 1.f - t );
					g1 = g2 * ( 1.f - t );
					b1 = b2 * ( 1.f - t );
					break;
				}

				g_Draw.FillRGBA( x + distancecolor + w + 4, y + 16 + i, 5, 1, r1, g1, b1, alpha );

				if( MouseOver( x + distancecolor + w + 3, y + 15 + i, 7, sizeh + 2 ) && bLClick )
				{
					r = r1;
					g = g1;
					b = b1;
					flYBright [ num ] = clamp( iCursorPos [ 1 ] - y, iCursorPos [ 1 ] - y, iCursorPos [ 1 ] - y + sizeh );
				}
			}

			g_Draw.DrawOutlineRect( x + distancecolor + w + 3, clamp( y + flYBright [ num ], y, y + sizeh + 14 ), 7, 4, 30, 30, 30, alpha );

			if( !MouseOver( x + distancecolor - sizew + w, y + 16, sizew + 10, sizeh ) && bLeftClickReleased )
			{
				sColorPicker [ num ].bOpen = false;
				sColorPicker [ num ].bActive = true;
				bInColorPick = false;
				bInit = true;
			}
		}
	}
}

void CGui::DrawSlider( int x, int y, int w, int h, float& value, int min, int max, const char* text, int alpha, bool style )
{
	bool bNegate = ( min <= -1 );
	int distance = 111;
	g_Draw.DrawString( g_Draw.visualsfont, false, x, y, 200, 200, 200, clamp( alpha, 0, 150 ), text );

	std::string szValue = std::to_string( value );

	float flSliderpos;

	float iDistance = x + distance;

	if( MouseOver( iDistance - 2, y + 2, w + 6, h + 2 ) && bLClick && !bInComboBox && !bInDetectKey && !bInColorPick && !bInMultiCombo && !bInNumberInput )
		flSliderpos = ( iCursorPos [ 0 ] - iDistance );
	else
		flSliderpos = value * w / max;

	value = clamp( max * flSliderpos / w, min, max );

	if( LeftClick( iDistance - 13, y + 3, 10, h + 4 ) && !bInComboBox && !bInDetectKey && !bInColorPick && !bInMultiCombo )
		value -= style ? 0.1 : 1;

	if( LeftClick( iDistance + w + 3, y + 3, 10, h + 4 ) && !bInComboBox && !bInDetectKey && !bInColorPick && !bInMultiCombo )
		value += style ? 0.1 : 1;

	if( MouseOver( iDistance - 2, y + 2, w + 6, h + 2 ) )
	{
		if( bDownScroll )
		{
			value -= style ? 0.1 : 1;
			bDownScroll = false;
		}
		else
			if( bUpScroll )
			{
				value += style ? 0.1 : 1;
				bUpScroll = false;
			}
	}

	g_Draw.DrawString( g_Draw.visualsfont, false, iDistance + w + 5, y + 1, g_Var.Color.Menu [ 0 ], g_Var.Color.Menu [ 1 ], g_Var.Color.Menu [ 2 ], clamp( alpha, 0, 120 ), "+" );
	g_Draw.DrawString( g_Draw.visualsfont, false, iDistance - 10, y + 1, g_Var.Color.Menu [ 0 ], g_Var.Color.Menu [ 1 ], g_Var.Color.Menu [ 2 ], clamp( alpha, 0, 120 ), "-" );

	g_Draw.DrawGradient( iDistance, y + 5, flSliderpos, h, g_Var.Color.Menu [ 0 ], g_Var.Color.Menu [ 1 ], g_Var.Color.Menu [ 2 ], alpha, g_Var.Color.Menu [ 0 ] / 2, g_Var.Color.Menu [ 1 ] / 3, g_Var.Color.Menu [ 2 ] / 4, alpha );
	g_Draw.DrawOutlineRect( iDistance, y + 5, w + 1, h, 70, 70, 70, clamp( alpha, 0, 180 ) );

	int iValue = g_Draw.getWidth( g_Draw.visualsfont, szValue.c_str() ) / 2;

	if( style )
	{
		g_Draw.FillRGBA( iDistance + ( w - iValue * 1.5 ) / 2, y + 5, iValue * 1.5, h, 36, 36, 36, clamp( alpha, 0, 200 ) );
		g_Draw.DrawString( g_Draw.visualsfont, true, iDistance + w / 2, y + 1, 200, 200, 200, clamp( alpha, 0, 150 ), "%3.2f", bNegate ? value - 90 : value );
	}
	else
	{
		g_Draw.FillRGBA( iDistance + ( w - iValue ) / 2, y + 5, iValue, h, 36, 36, 36, clamp( alpha, 0, 200 ) );
		g_Draw.DrawString( g_Draw.visualsfont, true, iDistance + w / 2, y + 1, 200, 200, 200, clamp( alpha, 0, 150 ), "%i", bNegate ? ( int ) value - 90 : ( int ) value );
	}
}

void CGui::DrawKeybox( int x, int y, int& value, int num, const char* text, int alpha, int distance )
{
	int dist = distance - g_Draw.getWidth( g_Draw.visualsfont, GetKeyName( value ) );

	g_Draw.DrawString( g_Draw.visualsfont, false, x, y + ( float )1.5, 200, 200, 200, clamp( alpha, 0, 150 ), text );

	//g_Draw.DrawOutlineRect( x + distance, y, g_Draw.getWidth( g_Draw.visualsfont, GetKeyName( value ) ) + 12, 15, 255, 0, 0, alpha );
//	if( LeftClick( x + dist, y, g_Draw.getWidth( g_Draw.visualsfont, GetKeyName( value ) ) + 12, 15 ) && !bInComboBox && !bInColorPick && !bInMultiCombo )
//		bInDetectKey = true;

	if( sKeyBox[ num ].bActive )
	{
		g_Draw.DrawString( g_Draw.keyfont, false, x + dist, y, 70, 70, 70, clamp( alpha, 0, 180 ), "[" );
		g_Draw.DrawString( g_Draw.keyfont, false, x + dist + g_Draw.getWidth( g_Draw.keyfont, GetKeyName( value ) ) + 7, y, 70, 70, 70, clamp( alpha, 0, 180 ), "]" );

		g_Draw.DrawString( g_Draw.visualsfont, false, x + 5 + dist, y, 200, 200, 200, clamp( alpha, 0, 150 ), GetKeyName( value ) );

		if( !bInComboBox && !bInColorPick && !bInMultiCombo && !bInNumberInput )
		{
			if( MouseOver( x + dist, y, g_Draw.getWidth( g_Draw.keyfont, GetKeyName( value ) ), 10 ) && !bInComboBox && !bLClick )
			{
				g_Draw.DrawString( g_Draw.keyfont, false, x + dist, y, g_Var.Color.Menu[ 0 ], g_Var.Color.Menu[ 1 ], g_Var.Color.Menu[ 2 ], clamp( alpha, 0, 180 ), "[" );
				g_Draw.DrawString( g_Draw.keyfont, false, x + dist + g_Draw.getWidth( g_Draw.keyfont, GetKeyName( value ) ) + 7, y, g_Var.Color.Menu[ 0 ], g_Var.Color.Menu[ 1 ], g_Var.Color.Menu[ 2 ], clamp( alpha, 0, 180 ), "]" );
			}

			if( LeftClick( x + dist, y, g_Draw.getWidth( g_Draw.visualsfont, GetKeyName( value ) ) + 12, 15 ) && !bInDetectKey )
			{
				sKeyBox[ num ].bOpen = true;
				sKeyBox[ num ].bActive = false;
			}
		}
	}
	else
	{
		if( sKeyBox[ num ].bOpen )
		{
			bInDetectKey = true;

			g_Draw.DrawString( g_Draw.visualsfont, false, x + distance - g_Draw.getWidth( g_Draw.visualsfont, "SELECT" ), y, g_Var.Color.Menu[ 0 ], g_Var.Color.Menu[ 1 ], g_Var.Color.Menu[ 2 ], clamp( alpha, 0, 180 ), "[" );
			g_Draw.DrawString( g_Draw.visualsfont, false, x + distance - g_Draw.getWidth( g_Draw.visualsfont, "SELECT" ) + 5, y, g_Var.Color.Menu[ 0 ], g_Var.Color.Menu[ 1 ], g_Var.Color.Menu[ 2 ], clamp( alpha, 0, 200 ), "SELECT" );
			g_Draw.DrawString( g_Draw.visualsfont, false, x + distance + 7, y, g_Var.Color.Menu[ 0 ], g_Var.Color.Menu[ 1 ], g_Var.Color.Menu[ 2 ], clamp( alpha, 0, 180 ), "]" );

			for( int i = 1; i < 192; i++ )
			{
				if( GetAsyncKeyState( i ) & 0x8000 )
				{
					if( i == VK_ESCAPE || i == VK_BACK )
						value = 0;
					else
						value = i;

					sKeyBox[ num ].bOpen = false;
					sKeyBox[ num ].bActive = true;
					bInDetectKey = false;
				}
			}
		}
	}
}

void CGui::DrawCombobox( int x, int y, int& value, int num, int max, int w, int h, std::string* structs, const char* text, int alpha, int distance = 179 )
{
	sComboBox [ num ].Name = const_cast< char* >( structs [ value ].c_str() );

	g_Draw.DrawOutlineRect( x + distance, y - 1, w, h, 70, 70, 70, clamp( alpha, 0, 180 ) );
	//g_Draw.DrawOutlineRect( x + distance + 1, y, w - 2, h - 2, 36, 36, 36, alpha );

	if( MouseOver( x + distance, y, w - 2, h - 2 ) && !sComboBox[ num ].bOpen )
		g_Draw.DrawOutlineRect( x + distance + 1, y, w - 2, h - 2, g_Var.Color.Menu[ 0 ], g_Var.Color.Menu[ 1 ], g_Var.Color.Menu[ 2 ], alpha );

	if( sComboBox [ num ].bActive )
	{
		if( !bInDetectKey && !bInColorPick && !bInMultiCombo && !bInNumberInput )
		{
			sComboBox [ num ].iSelect = value;

			if( MouseOver( x + distance, y, w, h ) && bLeftClickReleased && !bInComboBox )
			{
				sComboBox [ num ].bOpen = true;
				sComboBox [ num ].bActive = false;
			}
		}

		g_Draw.DrawString( g_Draw.visualsfont, true, x + distance + w / 2, y, 200, 200, 200, clamp( alpha, 0, 150 ), sComboBox [ num ].Name );
	}
	else
	{
		if( sComboBox [ num ].bOpen )
		{
			g_Draw.DrawString( g_Draw.visualsfont, true, x + distance + w / 2, y, 200, 200, 200, clamp( alpha, 0, 150 ), sComboBox [ num ].Name );

			bInComboBox = true;

			for( int i = 0; i <= max; i++ )
			{
				g_Draw.FillRGBA( x + distance, y + 20 + ( i * h ), w, h, 27, 27, 27, alpha );

				g_Draw.LineRGBA( x + distance - 1, y + 20 - 1, x + distance + w, y + 20 - 1, 70, 70, 70, clamp( alpha, 0, 180 ) );
				g_Draw.LineRGBA( x + distance - 1, y + 20 - 1, x + distance - 1, y + 20 + ( i * h ) + 15, 70, 70, 70, clamp( alpha, 0, 180 ) );
				g_Draw.LineRGBA( x + distance + w, y + 20 - 1, x + distance + w, y + 20 + ( i * h ) + 16, 70, 70, 70, clamp( alpha, 0, 180 ) );
				g_Draw.LineRGBA( x + distance - 1, y + 20 + ( i * h ) + 15, x + distance + w, y + 20 + ( i * h ) + 15, 70, 70, 70, clamp( alpha, 0, 180 ) );

				if( MouseOver( x + distance + 4, y + 20 + ( i * h ), w, h ) )
				{
					g_Draw.DrawOutlineRect( x + distance, y + 20 + ( i * h ), w, h, g_Var.Color.Menu [ 0 ], g_Var.Color.Menu [ 1 ], g_Var.Color.Menu [ 2 ], alpha );

					if( bLeftClickReleased )
					{
						sComboBox [ num ].iSelect = i;
						sComboBox [ num ].Name = const_cast< char* >( structs [ sComboBox [ num ].iSelect ].c_str() );
						value = sComboBox [ num ].iSelect;
						sComboBox [ num ].bOpen = false;
						sComboBox [ num ].bActive = true;
						bInComboBox = false;
					}
				}

				g_Draw.DrawString( g_Draw.visualsfont, true, x + distance + w / 2, y + 20 + ( i * h ), 200, 200, 200, clamp( alpha, 0, 150 ), structs [ i ].c_str() );
			}

			if( !MouseOver( x + distance, y - 2, w, h ) && bLeftClickReleased )
			{
				sComboBox [ num ].bOpen = false;
				sComboBox [ num ].bActive = true;
				bInComboBox = false;
			}
		}
	}

	g_Draw.DrawString( g_Draw.visualsfont, false, x, y, 200, 200, 200, clamp( alpha, 0, 150 ), text );
}

void CGui::DrawMultiCombobox( int x, int y, int* value, int num, int max, int w, int h, std::string* structs, const char* text, int alpha )
{
	int distance = 179;
	std::string szEntrys = "";

	for( int i = 0; i <= max; i++ )
		szEntrys.append( value[ i ] ? structs[ i ] + ", " : "" );

	if( szEntrys.length() <= 1 )
		szEntrys = "None";

	if( szEntrys != "None" )
		szEntrys.erase( szEntrys.size() - 2 );

	while( g_Draw.getWidth( g_Draw.visualsfont, szEntrys.c_str() ) > w - 10 )
		szEntrys.erase( szEntrys.size() - 1 );

	if( g_Draw.getWidth( g_Draw.visualsfont, szEntrys.c_str() ) >= w - 16 )
	{
		szEntrys.erase( szEntrys.size() - 2 );
		szEntrys.append( ".." );
	}
	
	g_Draw.DrawOutlineRect( x + distance, y - 1, w, h, 70, 70, 70, clamp( alpha, 0, 180 ) );

	if( MouseOver( x + distance, y, w - 2, h - 2 ) && !sMultiComboBox [ num ].bOpen )
		g_Draw.DrawOutlineRect( x + distance + 1, y, w - 2, h - 2, g_Var.Color.Menu [ 0 ], g_Var.Color.Menu [ 1 ], g_Var.Color.Menu [ 2 ], alpha );

	if( sMultiComboBox [ num ].bActive )
	{
		if( !bInDetectKey && !bInColorPick && !bInComboBox && !bInNumberInput )
		{
			if( MouseOver( x + distance, y, w, h ) && bLeftClickReleased && !bInMultiCombo )
			{
				sMultiComboBox [ num ].bOpen = true;
				sMultiComboBox [ num ].bActive = false;
			}
		}

		g_Draw.DrawString( g_Draw.visualsfont, true, x + distance + w / 2, y, 200, 200, 200, clamp( alpha, 0, 150 ), szEntrys.c_str() );
	}
	else
	{
		if( sMultiComboBox [ num ].bOpen )
		{
			g_Draw.DrawString( g_Draw.visualsfont, true, x + distance + w / 2, y, 200, 200, 200, clamp( alpha, 0, 150 ), szEntrys.c_str() );

			bInMultiCombo = true;

			for( int i = 0; i <= max; i++ )
			{
				g_Draw.FillRGBA( x + distance, y + 20 + ( i * h ), w, h, 27, 27, 27, alpha );

				g_Draw.LineRGBA( x + distance - 1, y + 20 - 1, x + distance + w, y + 20 - 1, 70, 70, 70, clamp( alpha, 0, 180 ) );
				g_Draw.LineRGBA( x + distance - 1, y + 20 - 1, x + distance - 1, y + 20 + ( i * h ) + 15, 70, 70, 70, clamp( alpha, 0, 180 ) );
				g_Draw.LineRGBA( x + distance + w, y + 20 - 1, x + distance + w, y + 20 + ( i * h ) + 16, 70, 70, 70, clamp( alpha, 0, 180 ) );
				g_Draw.LineRGBA( x + distance - 1, y + 20 + ( i * h ) + 15, x + distance + w, y + 20 + ( i * h ) + 15, 70, 70, 70, clamp( alpha, 0, 180 ) );

				if( MouseOver( x + distance, y + 20 + ( i * h ), w, h ) )
				{
					g_Draw.DrawOutlineRect( x + distance, y + 20 + ( i * h ), w, h, g_Var.Color.Menu [ 0 ], g_Var.Color.Menu [ 1 ], g_Var.Color.Menu [ 2 ], alpha );

					if( bLeftClickReleased )
					{
						if( value[ i ] != 1 )
							value[ i ] = 1;
						else
							value[ i ] = 0;
					}
				}

				if( value[ i ] )
					g_Draw.DrawString( g_Draw.visualsfont, true, x + distance + w / 2, y + 20 + ( i * h ), g_Var.Color.Menu[ 0 ], g_Var.Color.Menu[ 1 ], g_Var.Color.Menu[ 2 ], clamp( alpha, 0, 150 ), structs[ i ].c_str() );
				else
					g_Draw.DrawString( g_Draw.visualsfont, true, x + distance + w / 2, y + 20 + ( i * h ), 200, 200, 200, clamp( alpha, 0, 150 ), structs[ i ].c_str() );
			}

			if( !MouseOver( x + distance, y, w, h + ( 21 * max ) ) && bLeftClickReleased )
			{
				sMultiComboBox [ num ].bOpen = false;
				sMultiComboBox [ num ].bActive = true;
				bInMultiCombo = false;
			}
		}
	}

	g_Draw.DrawString( g_Draw.visualsfont, false, x, y, 200, 200, 200, clamp( alpha, 0, 150 ), text );
}

void CGui::DrawNumberBox( int x, int y, int& value, int num, int w, int h, const char* text, int alpha, int distance = 179 )
{
	static std::string szInput[ 60 ];
	static int iPos[ 60 ];
	static bool Draw = false;
	static int iTick = 0;

	g_Draw.DrawOutlineRect( x + distance, y - 1, w, h, 70, 70, 70, clamp( alpha, 0, 180 ) );

	if( MouseOver( x + distance, y, w - 2, h - 2 ) && !sNumberInput[ num ].bOpen )
		g_Draw.DrawOutlineRect( x + distance + 1, y, w - 2, h - 2, g_Var.Color.Menu[ 0 ], g_Var.Color.Menu[ 1 ], g_Var.Color.Menu[ 2 ], alpha );

	if( sNumberInput[ num ].bActive )
	{
		szInput[ num ] = std::to_string( value );

		if( !bInDetectKey && !bInColorPick && !bInComboBox && !bInMultiCombo )
		{
			if( MouseOver( x + distance, y, w, h ) && bLeftClickReleased && !bInNumberInput )
			{
				sNumberInput[ num ].bOpen = true;
				sNumberInput[ num ].bActive = false;
			}
		}
	}
	else
	{
		if( sNumberInput[ num ].bOpen )
		{
			bInNumberInput = true;

			if( g_Draw.getWidth( g_Draw.visualsfont, szInput[ num ].c_str() ) > ( w - 5 ) )
				szInput[ num ].erase( szInput[ num ].end() - 1 );

			if( GetAsyncKeyState( eKEY_0 ) & 1 )
				szInput[ num ].insert( szInput[ num ].end() - iPos[ num ], '0' );

			if( GetAsyncKeyState( eKEY_1 ) & 1 )
				szInput[ num ].insert( szInput[ num ].end() - iPos[ num ], '1' );

			if( GetAsyncKeyState( eKEY_2 ) & 1 )
				szInput[ num ].insert( szInput[ num ].end() - iPos[ num ], '2' );

			if( GetAsyncKeyState( eKEY_3 ) & 1 )
				szInput[ num ].insert( szInput[ num ].end() - iPos[ num ], '3' );

			if( GetAsyncKeyState( eKEY_4 ) & 1 )
				szInput[ num ].insert( szInput[ num ].end() - iPos[ num ], '4' );

			if( GetAsyncKeyState( eKEY_5 ) & 1 )
				szInput[ num ].insert( szInput[ num ].end() - iPos[ num ], '5' );

			if( GetAsyncKeyState( eKEY_6 ) & 1 )
				szInput[ num ].insert( szInput[ num ].end() - iPos[ num ], '6' );

			if( GetAsyncKeyState( eKEY_7 ) & 1 )
				szInput[ num ].insert( szInput[ num ].end() - iPos[ num ], '7' );

			if( GetAsyncKeyState( eKEY_8 ) & 1 )
				szInput[ num ].insert( szInput[ num ].end() - iPos[ num ], '8' );

			if( GetAsyncKeyState( eKEY_9 ) & 1 )
				szInput[ num ].insert( szInput[ num ].end() - iPos[ num ], '9' );

			if( GetAsyncKeyState( VK_BACK ) & 1 && iPos[ num ] < szInput[ num ].length() && szInput[ num ].length() > 0 )
				szInput[ num ].erase( szInput[ num ].end() - iPos[ num ] - 1 );

			sscanf( szInput[ num ].c_str(), "%d", &value );
		
			if( GetAsyncKeyState( VK_LEFT ) & 1 )
			{
				if( iPos[ num ] + 1 <= szInput[ num ].size() )
					iPos[ num ]++;
			}
			else
				if( GetAsyncKeyState( VK_RIGHT ) & 1 )
				{
					if( iPos[ num ] - 1 >= 0 )
						iPos[ num ]--;
				}

			iTick++;

			if( iTick > 75 )
			{
				Draw = !Draw;
				iTick = 0;
			}

			std::string sub = szInput[ num ].substr( 0, szInput[ num ].size() - iPos[ num ] );

			g_Draw.DrawString( g_Draw.visualsfont, false, x + distance + g_Draw.getWidth( g_Draw.visualsfont, sub.c_str() ) + 4, y - 1, 200, 200, 200, clamp( alpha, 0, 150 ), Draw ? "|" : "" );


			if( GetAsyncKeyState( VK_RETURN ) || GetAsyncKeyState( VK_ESCAPE ) || !MouseOver( x + distance, y, w - 2, h - 2 ) && bLeftClickReleased )
			{
				sNumberInput[ num ].bOpen = false;
				sNumberInput[ num ].bActive = true;
				bInNumberInput = false;
			}
		}
	}

	if( szInput[ num ].length() > 0 )
		g_Draw.DrawString( g_Draw.visualsfont, false, x + distance + 4, y, 200, 200, 200, clamp( alpha, 0, 150 ), "%i", value );

	g_Draw.DrawString( g_Draw.visualsfont, false, x + 50, y, 200, 200, 200, clamp( alpha, 0, 150 ), text );
}

void LoadButton( int x, int y, int w, int h, int a )
{
	if( g_Gui.MouseOver( x, y, w, h ) )
		g_Draw.DrawGradient( x + 1, y + 1, w - 2, h - 2, 17, 17, 17, a, 23, 23, 23, a );

	if( g_Gui.MouseOver( x, y, w, h ) && g_Gui.bLClick && !g_Gui.bInComboBox && !g_Gui.bInMultiCombo )
	{
		g_Draw.DrawGradient( x + 1, y + 1, w - 2, h - 2, 23, 23, 23, a, 17, 17, 17, a );

		g_ConfigFile.LoadConfig();
		g_ConfigFile.LoadSkinConfig();
		//LoadSkinConfig();
	}

	g_Draw.DrawString( g_Draw.visualsfont, true, x + w / 2, y + 3, 200, 200, 200, clamp( a, 0, 200 ), "LOAD" );

	g_Draw.DrawOutlineRect( x, y, w, h, 40, 40, 40, a );
	g_Draw.DrawOutlineRect( x + 1, y + 1, w - 2, h - 2, 36, 36, 36, a );
}

void SaveButton( int x, int y, int w, int h, int a )
{
	if( g_Gui.MouseOver( x, y, w, h ) )
		g_Draw.DrawGradient( x + 1, y + 1, w - 2, h - 2, 17, 17, 17, a, 23, 23, 23, a );

	if( g_Gui.MouseOver( x, y, w, h ) && g_Gui.bLClick && !g_Gui.bInComboBox && !g_Gui.bInMultiCombo )
	{
		g_Draw.DrawGradient( x + 1, y + 1, w - 2, h - 2, 23, 23, 23, a, 17, 17, 17, a );

		g_ConfigFile.SaveConfig();
		g_ConfigFile.SaveSkinConfig();
		//SaveSkinConfig();
	}

	g_Draw.DrawString( g_Draw.visualsfont, true, x + w / 2, y + 3, 200, 200, 200, clamp( a, 0, 200 ), "SAVE" );

	g_Draw.DrawOutlineRect( x, y, w, h, 40, 40, 40, a );
	g_Draw.DrawOutlineRect( x + 1, y + 1, w - 2, h - 2, 36, 36, 36, a );
}

void UpdateButton( int x, int y, int w, int h, int a )
{
	if( g_Gui.MouseOver( x, y, w, h ) )
		g_Draw.DrawGradient( x + 1, y + 1, w - 2, h - 2, 17, 17, 17, a, 23, 23, 23, a );

	if( g_Gui.MouseOver( x, y, w, h ) && g_Gui.bLClick && !g_Gui.bInComboBox && !g_Gui.bInMultiCombo )
	{
		g_Draw.DrawGradient( x + 1, y + 1, w - 2, h - 2, 23, 23, 23, a, 17, 17, 17, a );

		*( PINT ) ( ( DWORD ) g_pClientState + 0x174 ) = -1;
	}

	g_Draw.DrawString( g_Draw.visualsfont, true, x + w / 2, y + 1, 200, 200, 200, clamp( a, 0, 200 ), "UPDATE" );

	g_Draw.DrawOutlineRect( x, y, w, h, 40, 40, 40, a );
	g_Draw.DrawOutlineRect( x + 1, y + 1, w - 2, h - 2, 36, 36, 36, a );
}

void TabSlider( int x, int y, int w, int h, float maxvalue, int& var, int r, int g, int b, int a )
{
	if( var >= maxvalue / 2 )
	{
		if( g_Gui.LeftClick( x, y, 20, 15 ) )
			var = maxvalue / 2 - 1;

		if( g_Gui.MouseOver( x, y, 20, 15 ) && !GetAsyncKeyState( 1 ) )
		{
			g_Draw.DrawFilledTriangle( Vector2D( x + 3, y + 7 ), Vector2D( x + 9, y + 1 ), Vector2D( x + 9, y + 13 ), g_Var.Color.Menu[ 0 ], g_Var.Color.Menu[ 1 ], g_Var.Color.Menu[ 2 ], clamp( a, 0, 200 ) );
			g_Draw.FillRGBA( x + 9, y + 4, 6, 6, g_Var.Color.Menu[ 0 ], g_Var.Color.Menu[ 1 ], g_Var.Color.Menu[ 2 ], clamp( a, 0, 200 ) );
		}
		else
		{
			g_Draw.DrawFilledTriangle( Vector2D( x + 3, y + 7 ), Vector2D( x + 9, y + 1 ), Vector2D( x + 9, y + 13 ), 200, 200, 200, clamp( a, 0, 200 ) );
			g_Draw.FillRGBA( x + 9, y + 4, 6, 6, 200, 200, 200, clamp( a, 0, 200 ) );
		}

	}
	else
	{
		if( g_Gui.LeftClick( x + w, y, 20, 15 ) )
			var = maxvalue / 2 + 1;

		if( g_Gui.MouseOver( x + w, y, 20, 15 ) && !GetAsyncKeyState( 1 ) )
		{
			g_Draw.DrawFilledTriangle( Vector2D( x + w + 9, y ), Vector2D( x + w + 15, y + 7 ), Vector2D( x + w + 9, y + 14 ), g_Var.Color.Menu[ 0 ], g_Var.Color.Menu[ 1 ], g_Var.Color.Menu[ 2 ], clamp( a, 0, 200 ) );
			g_Draw.FillRGBA( x + w + 3, y + 4, 6, 6, g_Var.Color.Menu[ 0 ], g_Var.Color.Menu[ 1 ], g_Var.Color.Menu[ 2 ], clamp( a, 0, 200 ) );
		}
		else
		{
			g_Draw.DrawFilledTriangle( Vector2D( x + w + 9, y ), Vector2D( x + w + 15, y + 7 ), Vector2D( x + w + 9, y + 14 ), 200, 200, 200, clamp( a, 0, 200 ) );
			g_Draw.FillRGBA( x + w + 3, y + 4, 6, 6, 200, 200, 200, clamp( a, 0, 200 ) );
		}
	}

	if( var > maxvalue / 2 )
		var += 5;
	else
		var -= 5;

	if( var < 0 )
		var = 0;

	if( var > maxvalue )
		var = maxvalue;
}

std::string sHitbox[ 4 ] = {
	"Head",
	"Body",
	"Arms",
	"Legs"
};

std::string sChams[ 5 ] = {
	"Off",
	"Normal",
	"Flat",
	"Ghost",
	"Color Bars"
};

std::string sChamsHands[ 5 ] = {
	"Off",
	"Normal",
	"Ghost",
	"Net",
	"TV Anim"
};

std::string sViewChams[ 4 ] = {
	"Off",
	"Ghost",
	"Net",
	"Cloud"
};

std::string sChamsWeap[ 3 ] = {
	"Off",
	"Normal",
	"Pulse"
};

std::string sAutoStop[4] = {
	"Off",
	"Full Stop",
	"Shot Stop",
	"Accurate"
};

std::string sAimspot[ 4 ] = {
	"Head",
	"Neck",
	"Pelvis",
	"Chest"
};

std::string sCrosshair[ 4 ] = {
	"Spread",
	"Recoil",
	"Autowall",
	"Sniper"
};

std::string sMultibox[ 5 ] = {
	"Head",
	"Body",
	"Arms",
	"Legs",
	"Feet"
};

std::string sPosAdjust[ 5 ] = {
	"Off",
	"Priority",
	"Normal",
	"High",
	"Very high"
};

std::string sTextIcon[ 3 ] = {
	"Off",
	"Text",
	"Icon"
};

std::string sPitch[ 5 ] = {
	"Off",
	"Down",
	"Up",
	"Zero",
	"Jitter"
};

std::string sYaw[ 3 ] = {
	"Off",
	"Default",
	"Dynamic"
};

std::string sEnemyBased[ 3 ] = {
	"Off",
	"Distance",
	"Fov"
};

std::string sAddStyle[ 3 ] = {
	"Off",
	"Jitter",
	"Spin"
};

std::string sBreakLby[ 5 ] = {
	"Off",
	"Switch",
	"Predict",
	"Desync",
	"Shift"
};

std::string sResolver[ 3 ] = {
	"Off",
	"Default",
	"Alternative"
};

std::string sLagstyle[ 3 ] = {
	"Default",
	"Burst",
	"Adaptive"
};

std::string sBulletHit[ 4 ] = {
	"Tracer",
	"Impacts",
	"Hits",
	"Hitbox"
};

std::string sLagFlags[ 5 ] = {
	"In Stand",
	"In Air",
	"In Move",
	"On Land",
	"On Sight"
};

std::string sRemovals[ 6 ] = {
	"Flash",
	"Smoke",
	"Scope",
	"Recoil",
	"First Zoom",
	"Post Proc.."
};

std::string sLocalFlag[ 5 ] = {
	"Name",
	"Health",
	"Border",
	"Weapon",
	"Ammo"
};

std::string sEventLogs[ 3 ] = {
	"Damage",
	"Purchase",
	"Bomb"
};

std::string sHitmarker[ 3 ] = {
	"Off",
	"Default",
	"Button"
};

std::string sZeusKnife[2] = {
	"Automatic",
	"Show Range"
};

std::string sGrenades[ 3 ] = {
	"Text",
	"Icon",
	"Prediction"
};

std::string sWorldStyle[ 4 ] = {
	"Draw Grey",
	"Fullbright",
	"LowRes",
	"MipLevel"
};

std::string sIndicator[ 6 ] = {
	"AA Arrow",
	"Fake",
	"FakeLag",
	"Bomb Time",
	"Angle Lines",
	"Lag Comp"
};

std::string sBone[ 3 ] = {
	"Off",
	"Basic",
	"History"
};

std::string sDebugAim[ 3 ] = {
	"Off",
	"Points",
	"Damage"
};

std::string sRagdoll[ 3 ] = {
	"Off",
	"Low Gravity",
	"High Punch"
};

std::string sTargetSelection[ 4 ] = {
	"Distance",
	"Fov",
	"Random",
	"Lowest HP"
};

std::string sMultiPoint[ 3 ] = {
	"Off",
	"Dynamic",
	"Default"
};

std::string sIgnoreLimbs[ 2 ] = {
	"In Move",
	"In Air"
};

std::string sPriorityRec[4] = {
	"On Shot",
	"No Weapon",
	"In Air",
	"Running"
};

std::string sIgnoreHead[ 3 ] = {
	"In Air",
	"Unresolved",
	"High Fake"
};

std::string sGlow[ 3 ] = {
	"Off",
	"Default",
	"FullBloom"
};

std::string sFakeChams[5] = {
	"Off",
	"Normal",
	"Flat",
	"Ghost",
	"Pulse"
};

std::string sPrimary[ 6 ] = {
	"None",
	"Ak/M4",
	"Sg/Aug",
	"Scar/G3",
	"Scout",
	"Awp"
};

std::string sSecondary[ 4 ] = {
	"None",
	"Duals",
	"P250",
	"Deag/R8"
};

std::string sGear[ 3 ] = {
	"Kev/Helm",
	"Defuser",
	"Taser"
};

std::string sConfig[ 3 ] = {
	"Default",
	"Slot 1",
	"Slot 2"
};

std::string sGrenade[ 5 ] = {
	"Grenade",
	"Smoke",
	"Flash",
	"Molotov",
	"Decoy"
};

std::string sKnives[ 11 ] = {
	"Default",
	"Bayonet",
	"Flip Knife",
	"Gut Knife",
	"Karambit",
	"M9 Bayonet",
	"Huntsman",
	"Butterfly",
	"Falchion",
	"Daggers",
	"Bowie Knife"
};

std::string sGloves[ 8 ] = {
	"Off",
	"Bloodhound",
	"Sport",
	"Driver",
	"Hand Wraps",
	"Moto",
	"Specialist",
	"Hydra"
};

void CGui::DrawMenu()
{
	static int a = 0;

	CBaseEntity* pLocal = g_pEntList->GetClientEntity( g_pEngine->GetLocalPlayer() );

	if( !pLocal || pLocal && pLocal->GetHealth() <= 0 )
		g_pInputSystem->EnableInput( !bCursorActive );
	else
		g_pInputSystem->EnableInput( !bInNumberInput );

	if( IsMenuKey( VK_INSERT ) || IsMenuKey( VK_DELETE ) )
	{
		bCursorActive = !bCursorActive;

		if( !pLocal || pLocal && pLocal->GetHealth() <= 0 )
			g_pInputSystem->ResetInputState();

		bUpScroll = false;
		bDownScroll = false;

		bSecUpScroll = false;
		bSecDownScroll = false;
	}

	if( bCursorActive )
	{
		a += 8;

		if( a > 255 )
			a = 255;
	}
	else
	{
		a -= 8;

		if( a < 0 )
			a = 0;
	}

	if( !bCursorActive )
	{
		if( a == 0 )
			return;
	}

	static int iOldMouseX = iCursorPos [ 0 ], iOldMouseY = iCursorPos [ 1 ];

	if( iOldMouseX != iCursorPos [ 0 ] || iOldMouseY != iCursorPos [ 1 ] )
	{
		iOldMouseX = iCursorPos [ 0 ], iOldMouseY = iCursorPos [ 1 ];
		bDownScroll = false;
		bUpScroll = false;
		bSecSliderUp = false;
		bSecSliderDown = false;
	}

	static int x1, y1;

	int w = 557;
	int h = 410;

	static int x = 250, y = 300;

	if( MouseOver( x, y, w, 20 ) && bLClick )
	{
		if( x1 == -1 || y1 == -1 )
		{
			x1 = iCursorPos [ 0 ] - x;
			y1 = iCursorPos [ 1 ] - y;
		}
		x += iCursorPos [ 0 ] - ( x1 + x );
		y += iCursorPos [ 1 ] - ( y1 + y );
	}
	else
	{
		x1 = -1;
		y1 = -1;
	}

	g_Draw.NewMenu( x, y, w, h, a );

	int r = g_Var.Color.Menu [ 0 ], g = g_Var.Color.Menu [ 1 ], b = g_Var.Color.Menu [ 2 ];

	static int iTab = 1;
	static int iWpnTab = 0;
	static int iWpnGroup = 0;
	static int iWorldSlide, iPlayerSlide, iLocalSlide, iChamSlide;

	if( LeftClick( x + 10, y + 20, 120, 20 ) )
		iTab = 1;
	else
		if( LeftClick( x + 128, y + 20, 140, 20 ) )
			iTab = 2;
		else
			if( LeftClick( x + 271, y + 20, 140, 20 ) )
				iTab = 3;
			else
				if( LeftClick( x + 414, y + 20, 140, 20 ) )
					iTab = 4;

	static int iCheck = iTab;

	if( iCheck != iTab )
	{
		for( int i = 0; i <= 60; i++ )
		{
			sColorPicker[ i ].bOpen = false;
			sColorPicker[ i ].bActive = true;
			sComboBox[ i ].bOpen = false;
			sComboBox[ i ].bActive = true;
			sMultiComboBox[ i ].bOpen = false;
			sMultiComboBox[ i ].bActive = true;
			sKeyBox[ i ].bOpen = false;
			sKeyBox[ i ].bActive = true;
			sNumberInput[ i ].bOpen = false;
			sNumberInput[ i ].bActive = true;
		}
		bUpScroll = false, bDownScroll = false, bSecUpScroll = false, bSecDownScroll = false, bSliderUp = false, bSliderDown = false, bSecSliderUp = false, bSecSliderDown = false
			, bInComboBox = false, bInMultiCombo = false, bInDetectKey = false, bInGetText = false, bInColorPick = false, bInNumberInput = false;
		iCheck = iTab;
	}

	g_Draw.DrawString( g_Draw.tabfont, true, x + 70, y + 23, 150, 150, 150, clamp( a, 0, 150 ), "AIMBOT" );
	g_Draw.DrawString( g_Draw.tabfont, true, x + 210, y + 23, 150, 150, 150, clamp( a, 0, 150 ), "ANTIAIM" );
	g_Draw.DrawString( g_Draw.tabfont, true, x + 350, y + 23, 150, 150, 150, clamp( a, 0, 150 ), "VISUALS" );
	g_Draw.DrawString( g_Draw.tabfont, true, x + 490, y + 23, 150, 150, 150, clamp( a, 0, 150 ), "MISC" );

#pragma region weaponoption

	if( iTab == 1 )
	{
		g_Draw.DrawOutlineRect( x + 9, y + 355, 538, 45, 33, 33, 33, a );
		g_Draw.DrawOutlineRect( x + 10, y + 356, 536, 43, 70, 70, 70, clamp( a, 0, 80 ) );

		g_Draw.FillRGBA( x + 12, y + 358, 532, 39, 33, 33, 33, a );

		/*		g_Draw.FillRGBA( x + 13, y + 358, 98, 39, 60, 60, 60, 90 );
		g_Draw.FillRGBA( x + 121, y + 358, 98, 39, 60, 60, 60, 90 );
		g_Draw.FillRGBA( x + 229, y + 358, 98, 39, 60, 60, 60, 90 );
		g_Draw.FillRGBA( x + 337, y + 358, 98, 39, 60, 60, 60, 90 );
		g_Draw.FillRGBA( x + 445, y + 358, 98, 39, 60, 60, 60, 90 );*/

		if( LeftClick( x + 13, y + 358, 98, 38 ) && !bInComboBox )
			iWpnTab = 0;
		else
			if( LeftClick( x + 121, y + 358, 98, 38 ) && !bInComboBox )
				iWpnTab = 1;
			else
				if( LeftClick( x + 229, y + 358, 98, 38 ) && !bInComboBox )
					iWpnTab = 2;
				else
					if( LeftClick( x + 337, y + 358, 98, 38 ) && !bInComboBox )
						iWpnTab = 3;
					else
						if( LeftClick( x + 445, y + 358, 98, 38 ) && !bInComboBox )
							iWpnTab = 4;

		if( iWpnGroup == 1 )
		{
			g_Draw.DrawString( g_Draw.iconmenu, true, x + 62, y + 359, 200, 200, 200, clamp( a, 0, 120 ), "J" ); //pist
			g_Draw.DrawString( g_Draw.visualsfont, true, x + 62, y + 384, 200, 200, 200, clamp( a, 0, 120 ), "Heavy" ); //pist
		}
		else
		{
			g_Draw.DrawString( g_Draw.iconmenu, true, x + 62, y + 359, 200, 200, 200, clamp( a, 0, 120 ), "D" ); //pist
			g_Draw.DrawString( g_Draw.visualsfont, true, x + 62, y + 384, 200, 200, 200, clamp( a, 0, 120 ), "Light" ); //pist
		}

		g_Draw.DrawString( g_Draw.iconmenu, true, x + 170, y + 359, 200, 200, 200, clamp( a, 0, 120 ), "M" ); //smg
		g_Draw.DrawString( g_Draw.visualsfont, true, x + 170, y + 384, 200, 200, 200, clamp( a, 0, 120 ), "Machinegun" ); //smg

		g_Draw.DrawString( g_Draw.iconmenu, true, x + 278, y + 359, 200, 200, 200, clamp( a, 0, 120 ), "T" ); //rif
		g_Draw.DrawString( g_Draw.visualsfont, true, x + 278, y + 384, 200, 200, 200, clamp( a, 0, 120 ), "Rifle" ); //rif

		g_Draw.DrawString( g_Draw.iconmenu, true, x + 386, y + 359, 200, 200, 200, clamp( a, 0, 120 ), "b" ); //shot
		g_Draw.DrawString( g_Draw.visualsfont, true, x + 386, y + 384, 200, 200, 200, clamp( a, 0, 120 ), "Shotgun" ); //shot

		if( iWpnGroup == 7 )
		{
			g_Draw.DrawString( g_Draw.iconmenu, true, x + 494, y + 359, 200, 200, 200, clamp( a, 0, 120 ), "Y" ); //snip
			g_Draw.DrawString( g_Draw.visualsfont, true, x + 494, y + 384, 200, 200, 200, clamp( a, 0, 120 ), "Auto" ); //snip
		}
		else
			if( iWpnGroup == 6 )
			{
				g_Draw.DrawString( g_Draw.iconmenu, true, x + 494, y + 359, 200, 200, 200, clamp( a, 0, 120 ), "Z" ); //snip
				g_Draw.DrawString( g_Draw.visualsfont, true, x + 494, y + 384, 200, 200, 200, clamp( a, 0, 120 ), "Heavy" ); //snip
			}
			else
			{
				g_Draw.DrawString( g_Draw.iconmenu, true, x + 494, y + 359, 200, 200, 200, clamp( a, 0, 120 ), "a" ); //snip
				g_Draw.DrawString( g_Draw.visualsfont, true, x + 494, y + 384, 200, 200, 200, clamp( a, 0, 120 ), "Light" ); //snip
			}

		static bool bDidChanged = false;
		static bool bDidChanged1 = false;

		switch( iWpnTab )
		{
		case 0:
			bDidChanged1 = false;
			if( !bDidChanged )
			{
				iWpnGroup = 0;
				bDidChanged = true;
			}

			if( MouseOver( x + 13, y + 354, 98, 40 ) && bRightClickReleased )
				iWpnGroup += 1;

			if( iWpnGroup > 1 )
				iWpnGroup = 0;

			switch( iWpnGroup )
			{
			case 0:
				g_Draw.DrawString( g_Draw.iconmenu, true, x + 62, y + 359, r, g, b, clamp( a, 0, 120 ), "D" ); //pist
				g_Draw.DrawString( g_Draw.visualsfont, true, x + 62, y + 384, r, g, b, clamp( a, 0, 120 ), "Light" ); //pist
				break;
			case 1:
				g_Draw.DrawString( g_Draw.iconmenu, true, x + 62, y + 359, r, g, b, clamp( a, 0, 120 ), "J" ); //pist
				g_Draw.DrawString( g_Draw.visualsfont, true, x + 62, y + 384, r, g, b, clamp( a, 0, 120 ), "Heavy" ); //pist
				break;
			}
			break;
		case 1:
			bDidChanged = false;
			bDidChanged1 = false;
			iWpnGroup = 2;
			g_Draw.DrawString( g_Draw.iconmenu, true, x + 170, y + 359, r, g, b, clamp( a, 0, 120 ), "M" ); //smg
			g_Draw.DrawString( g_Draw.visualsfont, true, x + 170, y + 384, r, g, b, clamp( a, 0, 120 ), "Machinegun" ); //smg
			break;
		case 2:
			bDidChanged = false;
			bDidChanged1 = false;
			iWpnGroup = 3;
			g_Draw.DrawString( g_Draw.iconmenu, true, x + 278, y + 359, r, g, b, clamp( a, 0, 120 ), "T" ); //rif
			g_Draw.DrawString( g_Draw.visualsfont, true, x + 278, y + 384, r, g, b, clamp( a, 0, 120 ), "Rifle" ); //rif
			break;
		case 3:
			bDidChanged = false;
			bDidChanged1 = false;
			iWpnGroup = 4;
			g_Draw.DrawString( g_Draw.iconmenu, true, x + 386, y + 359, r, g, b, clamp( a, 0, 120 ), "b" ); //shot
			g_Draw.DrawString( g_Draw.visualsfont, true, x + 386, y + 384, r, g, b, clamp( a, 0, 120 ), "Shotgun" ); //shot
			break;
		case 4:

			if( !bDidChanged1 )
			{
				iWpnGroup = 5;
				bDidChanged1 = true;
			}

			if( MouseOver( x + 445, y + 354, 98, 40 ) && bRightClickReleased )
				iWpnGroup += 1;

			if( iWpnGroup > 7 )
				iWpnGroup = 5;

			switch( iWpnGroup )
			{
			case 5:
				g_Draw.DrawString( g_Draw.iconmenu, true, x + 494, y + 359, r, g, b, clamp( a, 0, 120 ), "a" ); //snip
				g_Draw.DrawString( g_Draw.visualsfont, true, x + 494, y + 384, r, g, b, clamp( a, 0, 120 ), "Light" ); //snip
				break;
			case 6:
				g_Draw.DrawString( g_Draw.iconmenu, true, x + 494, y + 359, r, g, b, clamp( a, 0, 120 ), "Z" ); //snip
				g_Draw.DrawString( g_Draw.visualsfont, true, x + 494, y + 384, r, g, b, clamp( a, 0, 120 ), "Heavy" ); //snip
				break;
			case 7:
				g_Draw.DrawString( g_Draw.iconmenu, true, x + 494, y + 359, r, g, b, clamp( a, 0, 120 ), "Y" ); //snip
				g_Draw.DrawString( g_Draw.visualsfont, true, x + 494, y + 384, r, g, b, clamp( a, 0, 120 ), "Auto" ); //snip
				break;
			}
		}
	}

#pragma endregion

	switch( iTab )
	{
	case 1:
		g_Draw.LineRGBA( x + 5, y + 21, x + 125, y + 21, r, g, b, clamp( a, 0, 200 ) );
		g_Draw.LineRGBA( x + 4, y + 21, x + 4, y + 41, r, g, b, clamp( a, 0, 200 ) );
		g_Draw.LineRGBA( x + 5, y + 41, x + 144, y + 41, r, g, b, clamp( a, 0, 200 ) );
		g_Draw.LineRGBA( x + 124, y + 21, x + 144, y + 41, r, g, b, clamp( a, 0, 200 ) );
		g_Draw.DrawString( g_Draw.tabfont, true, x + 70, y + 23, r, g, b, clamp( a, 0, 150 ), "AIMBOT" );

		g_Draw.DrawFeatureBox( x + 10, y + 48, 264, 302, 33, 33, 33, a, clamp( a, 0, 80 ), 200, 200, 200, clamp( a, 0, 120 ), "General", g_Draw.visualsfont, true );

		DrawCheckbox( x + 20, y + 57, g_Var.Ragebot[ iWpnGroup ].Enable, "Enable", a );
		DrawSlider( x + 20, y + 77, 120, 5, g_Var.Ragebot[ iWpnGroup ].Fov, 0, 180, "Field of View", a, false );
		DrawCheckbox( x + 20, y + 97, g_Var.Ragebot[ iWpnGroup ].Autoshoot, "Auto Fire", a );
		DrawCheckbox( x + 20, y + 117, g_Var.Ragebot[ iWpnGroup ].Team, "Team", a );
		//                                                                Spot
		DrawCheckbox( x + 20, y + 157, g_Var.Ragebot[ iWpnGroup ].Multibox, "Multibox", a, 160 );
		DrawCheckbox( x + 20, y + 177, g_Var.Ragebot[ iWpnGroup ].Multipoint, "Multipoints", a, 160 );


//		DrawCheckbox( x + 20, y + 177, g_Var.Ragebot[ iWpnGroup ].Multipoint, "Multipoints", a, 160 );
		DrawSlider( x + 20, y + 197, 120, 5, g_Var.Ragebot[ iWpnGroup ].HeadPointScale, 0, 1, "Head Pointscale", a, true );
		DrawSlider( x + 20, y + 217, 120, 5, g_Var.Ragebot[ iWpnGroup ].PointScale, 0, 1, "Pointscale", a, true );
		DrawCheckbox( x + 20, y + 237, g_Var.Ragebot[ iWpnGroup ].AutoPointScale, "Auto Pointscale", a );
		
		DrawCheckbox( x + 20, y + 257, g_Var.Ragebot[ iWpnGroup ].Silent, "Silent", a );
		DrawCheckbox( x + 20, y + 277, g_Var.Ragebot[ iWpnGroup ].Autowall, "Autowall", a );
		//SELECTION 
		DrawKeybox(x + 20, y + 317, g_Var.Ragebot[iWpnGroup].ForceBaim, 22, "Force Bodyaim", a, 235);
			
		DrawCombobox( x + 20, y + 297, g_Var.Ragebot[ iWpnGroup ].Selection, 0, 3, 65, 14, sTargetSelection, "Target Selection", a );
//		DrawCombobox( x + 20, y + 178, g_Var.Ragebot[ iWpnGroup ].Multipoint, 24, 2, 65, 14, sMultiPoint, "Multipoints", a, 110 );
		DrawMultiCombobox( x + 20, y + 178, g_Var.Ragebot[ iWpnGroup ].Multipointbox, 0, 4, 65, 14, sMultibox, "", a );
		DrawMultiCombobox( x + 20, y + 158, g_Var.Ragebot[ iWpnGroup ].Multiboxes, 1, 4, 65, 14, sMultibox, "", a );
		DrawCombobox( x + 20, y + 137, g_Var.Ragebot[ iWpnGroup ].Spot, 1, 3, 65, 14, sAimspot, "Preferred Hitbox", a );

		g_Draw.DrawFeatureBox( x + 281, y + 48, 264, 302, 33, 33, 33, a, clamp( a, 0, 80 ), 200, 200, 200, clamp( a, 0, 120 ), "Accuracy", g_Draw.visualsfont, true );

		if( iWpnGroup >= 5 )
		{
			DrawCheckbox( x + 291, y + 57, g_Var.Ragebot[ iWpnGroup ].Autozoom, "Auto Scope", a );
		//	DrawCheckbox( x + 291, y + 77, g_Var.Ragebot[ iWpnGroup ].Autostop, "Auto Stop", a );
			
			DrawCheckbox( x + 291, y + 97, g_Var.Ragebot[ iWpnGroup ].Recoil, "Remove Recoil", a );
			DrawCheckbox( x + 291, y + 117, g_Var.Ragebot[ iWpnGroup ].Spread, "Remove Spread", a );
			DrawSlider( x + 291, y + 137, 120, 5, g_Var.Ragebot[ iWpnGroup ].BodyAfter, 0, 8, "Target Body After", a, false );

			DrawSlider( x + 291, y + 157, 120, 5, g_Var.Ragebot[ iWpnGroup ].Hitchance, 0, 100, "Hit Chance", a, false );

			DrawSlider( x + 291, y + 297, 120, 5, g_Var.Ragebot[ iWpnGroup ].BodyAimHp, 0, 100, "Prefer Body Under", a, false );

			DrawSlider( x + 291, y + 257, 120, 5, g_Var.Ragebot[ iWpnGroup ].Mindamage, 0, 125, "Minimum Damage", g_Var.Ragebot[ iWpnGroup ].AutoMindDamage ? clamp( a, 0, 40 ) : a, false );
			DrawCheckbox( x + 291, y + 277, g_Var.Ragebot[ iWpnGroup ].AutoMindDamage, "Auto Minimum Damage", a );


			DrawMultiCombobox( x + 291, y + 237, g_Var.Ragebot[ iWpnGroup ].IgnoreHead, 2, 2, 65, 14, sIgnoreHead, "Ignore Head", a );
			DrawMultiCombobox( x + 291, y + 217, g_Var.Ragebot[ iWpnGroup ].Limbs, 3, 1, 65, 14, sIgnoreLimbs, "Ignore Limbs", a );
			DrawMultiCombobox(x + 291, y + 197, g_Var.Ragebot[iWpnGroup].PriorityRecord, 14, 3, 65, 14, sPriorityRec, "Priority Records", a);
			DrawCombobox( x + 291, y + 177, g_Var.Ragebot[ iWpnGroup ].PosAdjust, 2, 4, 65, 14, sPosAdjust, "Position Adjustment", a );
			DrawCombobox(x + 291, y + 77, g_Var.Ragebot[iWpnGroup].Autostop, 30, 3, 65, 14, sAutoStop, "Auto Stop", a);
		}
		else
		{
		//	DrawCheckbox( x + 291, y + 57, g_Var.Ragebot[ iWpnGroup ].Autostop, "Auto Stop", a );
			DrawCheckbox( x + 291, y + 77, g_Var.Ragebot[ iWpnGroup ].Recoil, "Remove Recoil", a );
			DrawCheckbox( x + 291, y + 97, g_Var.Ragebot[ iWpnGroup ].Spread, "Remove Spread", a );
			DrawSlider( x + 291, y + 117, 120, 5, g_Var.Ragebot[ iWpnGroup ].BodyAfter, 0, 8, "Target Body After", a, false );
			DrawSlider( x + 291, y + 137, 120, 5, g_Var.Ragebot[ iWpnGroup ].Hitchance, 0, 100, "Hit Chance", a, false );

			DrawSlider( x + 291, y + 277, 120, 5, g_Var.Ragebot[ iWpnGroup ].BodyAimHp, 0, 100, "Prefer Body Under", a, false );

			DrawSlider( x + 291, y + 237, 120, 5, g_Var.Ragebot[ iWpnGroup ].Mindamage, 0, 125, "Minimum Damage", g_Var.Ragebot[ iWpnGroup ].AutoMindDamage ? clamp( a, 0, 40 ) : a, false );
			DrawCheckbox( x + 291, y + 257, g_Var.Ragebot[ iWpnGroup ].AutoMindDamage, "Auto Minimum Damage", a );

			

			DrawMultiCombobox( x + 291, y + 217, g_Var.Ragebot[ iWpnGroup ].IgnoreHead, 2, 2, 65, 14, sIgnoreHead, "Ignore Head", a );
			DrawMultiCombobox( x + 291, y + 197, g_Var.Ragebot[ iWpnGroup ].Limbs, 3, 1, 65, 14, sIgnoreLimbs, "Ignore Limbs", a );
			DrawMultiCombobox(x + 291, y + 177, g_Var.Ragebot[iWpnGroup].PriorityRecord, 14, 3, 65, 14, sPriorityRec, "Priority Records", a);
			DrawCombobox( x + 291, y + 157, g_Var.Ragebot[ iWpnGroup ].PosAdjust, 2, 4, 65, 14, sPosAdjust, "Position Adjustment", a );
			DrawCombobox(x + 291, y + 57, g_Var.Ragebot[iWpnGroup].Autostop, 30, 3, 65, 14, sAutoStop, "Auto Stop", a);
		}

		break;
	case 2:
		g_Draw.LineRGBA( x + 128, y + 21, x + 267, y + 21, r, g, b, clamp( a, 0, 200 ) );
		g_Draw.LineRGBA( x + 127, y + 21, x + 147, y + 41, r, g, b, clamp( a, 0, 200 ) );
		g_Draw.LineRGBA( x + 147, y + 41, x + 287, y + 41, r, g, b, clamp( a, 0, 200 ) );
		g_Draw.LineRGBA( x + 267, y + 21, x + 287, y + 41, r, g, b, clamp( a, 0, 200 ) );
		g_Draw.DrawString( g_Draw.tabfont, true, x + 210, y + 23, r, g, b, clamp( a, 0, 150 ), "ANTIAIM" );

		g_Draw.DrawFeatureBox( x + 10, y + 48, 264, 250, 33, 33, 33, a, clamp( a, 0, 80 ), 200, 200, 200, clamp( a, 0, 120 ), "General", g_Draw.visualsfont, true );

		g_Draw.DrawFeatureBox( x + 10, y + 306, 264, 92, 33, 33, 33, a, clamp( a, 0, 80 ), 200, 200, 200, clamp( a, 0, 120 ), "Fakelag", g_Draw.visualsfont, true );

		DrawCheckbox( x + 20, y + 57, g_Var.AntiAim.Enable, "Enable", a );

		DrawSlider( x - 32, y + 95, 100, 5, g_Var.AntiAim.Tolerance, 0, 120, "", a, false );

		DrawKeybox( x + 20, y + 156, g_Var.AntiAim.BreakToggle, 4, "", a, 162 );
		DrawSlider( x + 20, y + 177, 120, 5, g_Var.AntiAim.BreakRad, 0, g_Var.AntiAim.Break == 3 ? 58 : 180, "Break Dist", g_Var.AntiAim.Break == 4 ? clamp(a, 0, 40) : a, true );
		DrawSlider( x + 20, y + 197, 120, 5, g_Var.AntiAim.Points, 4, 16, "Dynamic Points", a, false );
		DrawKeybox( x + 20, y + 217, g_Var.AntiAim.Override, 5, "Override", a, 233 );
		DrawCheckbox( x + 20, y + 237, g_Var.AntiAim.Resolve, "Resolver", a );
		DrawKeybox( x + 20, y + 257, g_Var.AntiAim.Fakewalk, 6, "Accurate Walk", a, 233 );
		DrawKeybox( x + 20, y + 277, g_Var.AntiAim.LockToggle, 7, "Lock", a, 233 );

	//	DrawCombobox( x + 20, y + 237, g_Var.AntiAim.Resolve, 2, 2, 65, 14, sResolver, "Resolver", a );
		DrawCombobox( x + 20, y + 157, g_Var.AntiAim.Break, 3, 4, 65, 14, sBreakLby, "Break", a );
		DrawCombobox( x + 20, y + 137, g_Var.AntiAim.Air, 4, 2, 65, 14, sYaw, "Air Yaw", a );
		DrawCombobox( x + 20, y + 117, g_Var.AntiAim.Move, 5, 2, 65, 14, sYaw, "Move Yaw", a );
		DrawCombobox( x + 20, y + 97, g_Var.AntiAim.Stand, 6, 2, 65, 14, sYaw, "Yaw", a );
		DrawCombobox( x + 20, y + 77, g_Var.AntiAim.Pitch, 7, 4, 65, 14, sPitch, "Pitch", a );

		DrawCheckbox( x + 20, y + 315, g_Var.Fakelag.Enable, "Enable", a );
		DrawSlider( x + 20, y + 375, 120, 5, g_Var.Fakelag.Factor, 2, 15, "Limit", a, false );

		DrawMultiCombobox( x + 20, y + 355, g_Var.Fakelag.Flags, 4, 4, 65, 14, sLagFlags, "Flags", a );
		DrawCombobox( x + 20, y + 335, g_Var.Fakelag.Style, 8, 2, 65, 14, sLagstyle, "Style", a );

		g_Draw.DrawFeatureBox( x + 281, y + 48, 264, 90, 33, 33, 33, a, clamp( a, 0, 80 ), 200, 200, 200, clamp( a, 0, 120 ), "Standing", g_Draw.visualsfont, true );

		g_Draw.DrawFeatureBox( x + 281, y + 146, 264, 90, 33, 33, 33, a, clamp( a, 0, 80 ), 200, 200, 200, clamp( a, 0, 120 ), "Move", g_Draw.visualsfont, true );

		g_Draw.DrawFeatureBox( x + 281, y + 244, 264, 90, 33, 33, 33, a, clamp( a, 0, 80 ), 200, 200, 200, clamp( a, 0, 120 ), "Air", g_Draw.visualsfont, true );

		g_Draw.DrawFeatureBox( x + 281, y + 342, 264, 56, 33, 33, 33, a, clamp( a, 0, 80 ), 200, 200, 200, clamp( a, 0, 120 ), "Binds", g_Draw.visualsfont, true );

		DrawSlider( x + 291, y + 77, 120, 5, g_Var.AntiAim.StandValue, g_Var.AntiAim.StandStyle == 0 ? -1 : 1, 180, "Add Value", a, false );
		DrawSlider( x + 291, y + 117, 120, 5, g_Var.AntiAim.StandSpeed, 1, 10, "Add Speed", a, true );

		DrawSlider( x + 291, y + 175, 120, 5, g_Var.AntiAim.MoveValue, g_Var.AntiAim.MoveStyle == 0 ? -1 : 1, 180, "Add Value", a, false );
		DrawSlider( x + 291, y + 215, 120, 5, g_Var.AntiAim.MoveSpeed, 1, 10, "Add Speed", a, true );

		DrawSlider( x + 291, y + 273, 120, 5, g_Var.AntiAim.AirValue, g_Var.AntiAim.AirStyle == 0 ? -1 : 1, 180, "Add Value", a, false );
		DrawSlider( x + 291, y + 313, 120, 5, g_Var.AntiAim.AirSpeed, 1, 10, "Add Speed", a, true );

		DrawKeybox( x + 291, y + 351, g_Var.AntiAim.LeftToggle, 16, "L | R", a, 85 );
		DrawKeybox( x + 291, y + 351, g_Var.AntiAim.RightToggle, 17, "", a, 140 );

		DrawSlider(x + 351, y + 351, 60, 5, g_Var.AntiAim.SideWaysTolerance, 0, 180, "", a, false);

		DrawKeybox( x + 291, y + 371, g_Var.AntiAim.BackToggle, 18, "B | F", a, 178 );
		DrawKeybox( x + 291, y + 371, g_Var.AntiAim.FrontToggle, 19, "", a, 233 );

		DrawCombobox( x + 291, y + 293, g_Var.AntiAim.AirEnemyBased, 9, 2, 65, 14, sEnemyBased, "Enemy Based", a );
		DrawCombobox( x + 291, y + 253, g_Var.AntiAim.AirStyle, 10, 2, 65, 14, sAddStyle, "Add Style", a );

		DrawCombobox( x + 291, y + 195, g_Var.AntiAim.MoveEnemyBased, 11, 2, 65, 14, sEnemyBased, "Enemy Based", a );
		DrawCombobox( x + 291, y + 155, g_Var.AntiAim.MoveStyle, 12, 2, 65, 14, sAddStyle, "Add Style", a );

		DrawCombobox( x + 291, y + 97, g_Var.AntiAim.StandEnemyBased, 13, 2, 65, 14, sEnemyBased, "Enemy Based", a );
		DrawCombobox( x + 291, y + 57, g_Var.AntiAim.StandStyle, 14, 2, 65, 14, sAddStyle, "Add Style", a );
		break;
	case 3:
		g_Draw.LineRGBA( x + 271, y + 21, x + 410, y + 21, r, g, b, clamp( a, 0, 200 ) );
		g_Draw.LineRGBA( x + 270, y + 21, x + 290, y + 41, r, g, b, clamp( a, 0, 200 ) );
		g_Draw.LineRGBA( x + 290, y + 41, x + 430, y + 41, r, g, b, clamp( a, 0, 200 ) );
		g_Draw.LineRGBA( x + 410, y + 21, x + 430, y + 41, r, g, b, clamp( a, 0, 200 ) );
		g_Draw.DrawString( g_Draw.tabfont, true, x + 350, y + 23, r, g, b, clamp( a, 0, 150 ), "VISUALS" );

		g_Draw.DrawFeatureBox( x + 10, y + 48, 264, 251, 33, 33, 33, a, clamp( a, 0, 80 ), 200, 200, 200, clamp( a, 0, 120 ), "Player", g_Draw.visualsfont, true );

		g_Draw.DrawFeatureBox( x + 10, y + 307, 264, 91, 33, 33, 33, a, clamp( a, 0, 80 ), 200, 200, 200, clamp( a, 0, 120 ), "Local", g_Draw.visualsfont, true );

		TabSlider( x + 17, y + 380, 234, 6, 510, iLocalSlide, g_Var.Color.Menu[ 0 ], g_Var.Color.Menu[ 1 ], g_Var.Color.Menu[ 2 ], a );

		if( iLocalSlide < 255 )
		{
			DrawCheckbox( x + 20, y + 316, g_Var.Visuals.Local.Enable, "Enable", a - iLocalSlide );

			DrawMultiCombobox( x + 20, y + 356, g_Var.Visuals.Local.Draw, 5, 4, 65, 14, sLocalFlag, "Flags", a - iLocalSlide );
			DrawCombobox( x + 20, y + 336, g_Var.Visuals.Local.Chams, 15, 4, 65, 14, sChams, "Chams", a - iLocalSlide );
			DrawSlider(x - 32, y + 334, 76, 5, g_Var.Visuals.Local.Transparency, 0, 100, "", a - iLocalSlide, false);

			DrawColorPicker( x + 20, y + 335, 20, g_Var.Color.LocalChams[ 0 ], g_Var.Color.LocalChams[ 1 ], g_Var.Color.LocalChams[ 2 ], 0, "", 152, a - iLocalSlide );
		}
		else
		{
			DrawCombobox(x + 20, y + 336, g_Var.Visuals.Local.FakeChams, 23, 1, 65, 14, sFakeChams, "Fake Chams", a - 510 + iLocalSlide);
			DrawColorPicker(x + 20, y + 335, 20, g_Var.Color.LocalFakeChams[0], g_Var.Color.LocalFakeChams[1], g_Var.Color.LocalFakeChams[2], 1, "", 152, a - 510 + iLocalSlide);

			DrawCombobox( x + 20, y + 316, g_Var.Visuals.Local.Glow, 15, 2, 65, 14, sGlow, "Glow", a - 510 + iLocalSlide );
			DrawColorPicker( x + 20, y + 315, 20, g_Var.Color.LocalGlow[ 0 ], g_Var.Color.LocalGlow[ 1 ], g_Var.Color.LocalGlow[ 2 ], 2, "", 152, a - 510 + iLocalSlide );
		}

		TabSlider( x + 17, y + 281, 234, 6, 510, iPlayerSlide, g_Var.Color.Menu[ 0 ], g_Var.Color.Menu[ 1 ], g_Var.Color.Menu[ 2 ], a );

		if( iPlayerSlide < 255 )
		{
			DrawCheckbox( x + 20, y + 57, g_Var.Visuals.Player.Enable, "Enable", a - iPlayerSlide );
			DrawCheckbox( x + 20, y + 77, g_Var.Visuals.Player.Team, "Team", a - iPlayerSlide );
			DrawCheckbox( x + 20, y + 97, g_Var.Visuals.Player.Name, "Name", a - iPlayerSlide );
			DrawCheckbox( x + 20, y + 117, g_Var.Visuals.Player.Box, "Box", a - iPlayerSlide );
			DrawCheckbox( x + 20, y + 137, g_Var.Visuals.Player.Health, "Health", a - iPlayerSlide );
			DrawSlider( x + 20, y + 217, 120, 5, g_Var.Visuals.Player.Surrounding, 0, 1000, "Surrounding", a - iPlayerSlide, false );
			DrawCheckbox( x + 20, y + 237, g_Var.Visuals.Player.Radar, "Radar", a - iPlayerSlide );
			DrawCheckbox( x + 20, y + 177, g_Var.Visuals.Player.AmmoBar, "Ammo", a - iPlayerSlide );

			DrawCombobox( x + 20, y + 257, g_Var.Visuals.Player.Bone, 16, 2, 65, 14, sBone, "Bone", a - iPlayerSlide );

			DrawColorPicker( x + 20, y + 217, 20, g_Var.Color.Surrounding[ 0 ], g_Var.Color.Surrounding[ 1 ], g_Var.Color.Surrounding[ 2 ], 3, "", 73, a - iPlayerSlide );

			DrawColorPicker( x + 20, y + 196, 20, g_Var.Color.Kevlar[ 0 ], g_Var.Color.Kevlar[ 1 ], g_Var.Color.Kevlar[ 2 ], 4, "", 152, a - iPlayerSlide );

			DrawCombobox( x + 20, y + 197, g_Var.Visuals.Player.KevHelm, 17, 2, 65, 14, sTextIcon, "Armor", a - iPlayerSlide );

			DrawColorPicker( x + 20, y + 177, 20, g_Var.Color.AmmoBar[ 0 ], g_Var.Color.AmmoBar[ 1 ], g_Var.Color.AmmoBar[ 2 ], 5, "", 203, a - iPlayerSlide );

			DrawCombobox( x + 20, y + 157, g_Var.Visuals.Player.Weapon, 18, 2, 65, 14, sTextIcon, "Weapon", a - iPlayerSlide );

			DrawColorPicker( x + 20, y + 156, 20, g_Var.Color.Weapon[ 0 ], g_Var.Color.Weapon[ 1 ], g_Var.Color.Weapon[ 2 ], 6, "", 152, a - iPlayerSlide );

			DrawColorPicker( x + 20, y + 137, 20, g_Var.Color.Health[ 0 ], g_Var.Color.Health[ 1 ], g_Var.Color.Health[ 2 ], 7, "", 203, a - iPlayerSlide );

			DrawColorPicker( x + 20, y + 117, 20, g_Var.Color.PlayerVisible[ 0 ], g_Var.Color.PlayerVisible[ 1 ], g_Var.Color.PlayerVisible[ 2 ], 8, "", 176, a - iPlayerSlide );
			DrawColorPicker( x + 20, y + 117, 20, g_Var.Color.Player[ 0 ], g_Var.Color.Player[ 1 ], g_Var.Color.Player[ 2 ], 9, "", 203, a - iPlayerSlide );

			DrawColorPicker( x + 20, y + 97, 20, g_Var.Color.Name[ 0 ], g_Var.Color.Name[ 1 ], g_Var.Color.Name[ 2 ], 10, "", 203, a - iPlayerSlide );

			DrawColorPicker( x + 20, y + 77, 20, g_Var.Color.TeamVisible[ 0 ], g_Var.Color.TeamVisible[ 1 ], g_Var.Color.TeamVisible[ 2 ], 11, "", 176, a - iPlayerSlide );
			DrawColorPicker( x + 20, y + 77, 20, g_Var.Color.Team[ 0 ], g_Var.Color.Team[ 1 ], g_Var.Color.Team[ 2 ], 12, "", 203, a - iPlayerSlide );
		}
		else
		{
			DrawCheckbox( x + 20, y + 77, g_Var.Visuals.Player.LowerBody, "LowerBody", a - 510 + iPlayerSlide );

			DrawColorPicker( x + 20, y + 77, 20, g_Var.Color.LowerBodyBar[ 0 ], g_Var.Color.LowerBodyBar[ 1 ], g_Var.Color.LowerBodyBar[ 2 ], 3, "", 203, a - 510 + iPlayerSlide );

			DrawCombobox( x + 20, y + 58, g_Var.Visuals.Glow.Player, 18, 2, 65, 14, sGlow, "Glow", a - 510 + iPlayerSlide );

			DrawColorPicker( x + 20, y + 57, 20, g_Var.Color.GlowVisible[ 0 ], g_Var.Color.GlowVisible[ 1 ], g_Var.Color.GlowVisible[ 2 ], 4, "", 125, a - 510 + iPlayerSlide );
			DrawColorPicker( x + 20, y + 57, 20, g_Var.Color.Glow[ 0 ], g_Var.Color.Glow[ 1 ], g_Var.Color.Glow[ 2 ], 5, "", 152, a - 510 + iPlayerSlide );
		}

		g_Draw.DrawFeatureBox( x + 281, y + 48, 264, 150, 33, 33, 33, a, clamp( a, 0, 80 ), 200, 200, 200, clamp( a, 0, 120 ), "Chams", g_Draw.visualsfont, true );

		g_Draw.DrawFeatureBox( x + 281, y + 206, 264, 192, 33, 33, 33, a, clamp( a, 0, 80 ), 200, 200, 200, clamp( a, 0, 120 ), "World", g_Draw.visualsfont, true );

//		DrawCheckbox( x + 291, y + 255, g_Var.Visuals.World.GrenadePred, "Grenade Prediction", a );

		TabSlider( x + 288, y + 380, 234, 6, 510, iWorldSlide, g_Var.Color.Menu[ 0 ], g_Var.Color.Menu[ 1 ], g_Var.Color.Menu[ 2 ], a );

		if( iWorldSlide < 255 )
		{
			DrawSlider( x + 291, y + 275, 120, 5, g_Var.Visuals.World.ViewModel, 0, 100, "View Model", a - iWorldSlide, false );
			DrawSlider( x + 291, y + 295, 120, 5, g_Var.Visuals.World.Fieldofview, 0, 100, "Field of View", a - iWorldSlide, false );
			DrawSlider( x + 291, y + 355, 120, 5, g_Var.Visuals.World.PropsAlpha, 0, 100, "Static Props", a - iWorldSlide, false );

			DrawMultiCombobox( x + 292, y + 335, g_Var.Visuals.World.BulletTracer, 6, 3, 65, 14, sBulletHit, "Bullet Hits", a - iWorldSlide );

			DrawColorPicker( x + 291, y + 334, 20, g_Var.Color.BulletTracer[ 0 ], g_Var.Color.BulletTracer[ 1 ], g_Var.Color.BulletTracer[ 2 ], 13, "", 152, a - iWorldSlide );

			DrawCombobox( x + 291, y + 315, g_Var.Visuals.World.Hitmarker, 19, 2, 65, 14, sHitmarker, "Hitmarker", a - iWorldSlide );
			DrawMultiCombobox( x + 291, y + 255, g_Var.Visuals.World.Grenade, 7, 2, 65, 14, sGrenades, "Grenades", a - iWorldSlide );
			DrawMultiCombobox( x + 291, y + 235, g_Var.Visuals.World.Removals, 8, 5, 65, 14, sRemovals, "Removals", a - iWorldSlide );
			DrawMultiCombobox( x + 291, y + 215, g_Var.Visuals.World.Crosshair, 9, 3, 65, 14, sCrosshair, "Crosshair", a - iWorldSlide );
		}
		else
		{
			DrawSlider( x + 291, y + 215, 120, 5, g_Var.Visuals.World.Nightmode, 0, 100, "Wall Dimness", a - 510 + iWorldSlide, false );

			DrawSlider(x + 291, y + 275, 120, 5, g_Var.Visuals.World.ViewModelOffset[0], -1, 180, "ViewModel X", a - 510 + iWorldSlide, false);
			DrawSlider(x + 291, y + 295, 120, 5, g_Var.Visuals.World.ViewModelOffset[1], -1, 180, "ViewModel Y", a - 510 + iWorldSlide, false);
			DrawSlider(x + 291, y + 315, 120, 5, g_Var.Visuals.World.ViewModelOffset[2], -1, 180, "ViewModel Z", a - 510 + iWorldSlide, false);

			DrawCombobox( x + 291, y + 255, g_Var.Visuals.World.Ragdoll, 29, 2, 65, 14, sRagdoll, "Ragdoll", a - 510 + iWorldSlide );
			DrawCheckbox( x + 291, y + 235, g_Var.Visuals.World.Fullbright, "Fullbright", a - 510 + iWorldSlide );

		//	DrawMultiCombobox( x + 291, y + 235, g_Var.Visuals.World.WorldStyle, 13, 3, 65, 14, sWorldStyle, "World Style", a - 510 + iWorldSlide );
		}

		TabSlider( x + 288, y + 180, 234, 6, 510, iChamSlide, g_Var.Color.Menu[ 0 ], g_Var.Color.Menu[ 1 ], g_Var.Color.Menu[ 2 ], a );

		if( iChamSlide < 255 )
		{
			DrawCheckbox( x + 291, y + 57, g_Var.Visuals.Chams.Enable, "Enable", a - iChamSlide );
			DrawCheckbox( x + 291, y + 97, g_Var.Visuals.Chams.Team, "Team", a - iChamSlide );
			DrawSlider(x + 239, y + 96, 100, 5, g_Var.Visuals.Chams.TeamTransparency, 0, 100, "", a - iChamSlide, false);
			DrawCheckbox( x + 291, y + 117, g_Var.Visuals.Chams.DrawOriginal, "Override", a - iChamSlide );
			DrawSlider( x + 291, y + 137, 120, 5, g_Var.Visuals.Chams.Transparency, 0, 100, "Transparency", a - iChamSlide, false );

			DrawColorPicker( x + 291, y + 156, 20, g_Var.Color.WeaponChamsVisible[ 0 ], g_Var.Color.WeaponChamsVisible[ 1 ], g_Var.Color.WeaponChamsVisible[ 2 ], 14, "", 125, a - iChamSlide );
			DrawColorPicker( x + 291, y + 156, 20, g_Var.Color.WeaponChams[ 0 ], g_Var.Color.WeaponChams[ 1 ], g_Var.Color.WeaponChams[ 2 ], 15, "", 152, a - iChamSlide );

			DrawCombobox( x + 291, y + 157, g_Var.Visuals.Chams.Weapons, 16, 2, 65, 14, sChamsWeap, "Weapons", a - iChamSlide );

			DrawColorPicker( x + 291, y + 97, 20, g_Var.Color.TeamChamsVisible[ 0 ], g_Var.Color.TeamChamsVisible[ 1 ], g_Var.Color.TeamChamsVisible[ 2 ], 17, "", 176, a - iChamSlide );
			DrawColorPicker( x + 291, y + 97, 20, g_Var.Color.TeamChams[ 0 ], g_Var.Color.TeamChams[ 1 ], g_Var.Color.TeamChams[ 2 ], 18, "", 203, a - iChamSlide );

			DrawColorPicker( x + 291, y + 76, 20, g_Var.Color.ChamsVisible[ 0 ], g_Var.Color.ChamsVisible[ 1 ], g_Var.Color.ChamsVisible[ 2 ], 19, "", 125, a - iChamSlide );
			DrawColorPicker( x + 291, y + 76, 20, g_Var.Color.Chams[ 0 ], g_Var.Color.Chams[ 1 ], g_Var.Color.Chams[ 2 ], 20, "", 152, a - iChamSlide );
			
			DrawCombobox( x + 291, y + 77, g_Var.Visuals.Chams.Players, 21, 4, 65, 14, sChams, "Player", a - iChamSlide );
		}
		else
		{
			DrawSlider(x + 291, y + 117, 120, 5, g_Var.Visuals.Chams.BacktrackAlpha, 0, 100, "Backtrack Alpha", a - 510 + iChamSlide, false);
			
			DrawCombobox(x + 291, y + 97, g_Var.Visuals.Chams.BacktrackMaterial, 30, 4, 65, 14, sChams, "Backtrack Chams", a - 510 + iChamSlide);
			DrawCombobox(x + 291, y + 77, g_Var.Visuals.Chams.View, 28, 3, 65, 14, sViewChams, "View Weapon", a - 510 + iChamSlide);
			DrawCombobox(x + 291, y + 57, g_Var.Visuals.Chams.Hands, 21, 4, 65, 14, sChamsHands, "Hands", a - 510 + iChamSlide);
			DrawSlider(x + 228, y + 56, 60, 5, g_Var.Visuals.Chams.HandsTransparency, 0, 100, "", a - 510 + iChamSlide, false);

			DrawColorPicker(x + 291, y + 96, 20, g_Var.Color.BacktrackChams[0], g_Var.Color.BacktrackChams[1], g_Var.Color.BacktrackChams[2], 14, "", 152, a - 510 + iChamSlide);

			DrawColorPicker(x + 291, y + 76, 20, g_Var.Color.ViewChams[0], g_Var.Color.ViewChams[1], g_Var.Color.ViewChams[2], 15, "", 152, a - 510 + iChamSlide);
			DrawColorPicker(x + 291, y + 56, 20, g_Var.Color.HandChamsVisible[0], g_Var.Color.HandChamsVisible[1], g_Var.Color.HandChamsVisible[2], 16, "", 125, a - 510 + iChamSlide);
			DrawColorPicker(x + 291, y + 56, 20, g_Var.Color.HandChams[0], g_Var.Color.HandChams[1], g_Var.Color.HandChams[2], 17, "", 152, a - 510 + iChamSlide);
		}

		break;
	case 4:
		g_Draw.LineRGBA( x + 414, y + 21, x + 551, y + 21, r, g, b, clamp( a, 0, 200 ) );
		g_Draw.LineRGBA( x + 413, y + 21, x + 433, y + 41, r, g, b, clamp( a, 0, 200 ) );
		g_Draw.LineRGBA( x + 433, y + 41, x + 552, y + 41, r, g, b, clamp( a, 0, 200 ) );
		g_Draw.LineRGBA( x + 551, y + 21, x + 551, y + 41, r, g, b, clamp( a, 0, 200 ) );
		g_Draw.DrawString( g_Draw.tabfont, true, x + 490, y + 23, r, g, b, clamp( a, 0, 150 ), "MISC" );

		g_Draw.DrawFeatureBox( x + 10, y + 48, 264, 350, 33, 33, 33, a, clamp( a, 0, 80 ), 200, 200, 200, clamp( a, 0, 120 ), "General", g_Draw.visualsfont, true );

		DrawCheckbox( x + 20, y + 77, g_Var.Misc.Thirdperson, "Thirdperson", a );
		DrawKeybox( x + 20, y + 77, g_Var.Misc.ThirdpersonToggleKey, 20, "", a, 215 );
		DrawSlider( x + 20, y + 97, 120, 5, g_Var.Misc.ThirdpersonDist, 0, 300, "Thirdperson Range", a, false );
		DrawCheckbox( x + 20, y + 157, g_Var.Visuals.Player.Spectators, "Spectators", a );
//		DrawCheckbox( x + 20, y + 177, g_Var.Misc.ZeusBot, "Auto Zeus", a );
//		DrawCheckbox( x + 20, y + 197, g_Var.Misc.KnifeBot, "Knife Bot", a );
		DrawCheckbox( x + 20, y + 237, g_Var.Misc.DebugResolver, "Debug Resolver", a );
		DrawCheckbox( x + 20, y + 257, g_Var.Misc.Untrusted, "Anti Untrusted", a );
		DrawCheckbox( x + 20, y + 277, g_Var.Misc.ExtendKillfeed, "Extended Killfeed", a );
		DrawCheckbox( x + 20, y + 297, g_Var.Misc.Insults, "Flame Enemy", a ); 
		DrawCheckbox(x + 20, y + 317, g_Var.Misc.Clantag, "ClanTag", a);
//		DrawCheckbox(x + 20, y + 337, g_Var.Misc.AnimFix, "Animation fix (BETA)", a);

		DrawCombobox( x + 20, y + 217, g_Var.Misc.DebugMultiPoint, 22, 2, 65, 14, sDebugAim, "Debug Multipoints", a );

		DrawMultiCombobox(x + 20, y + 197, g_Var.Misc.KnifeBot, 15, 1, 65, 14, sZeusKnife, "Knife Options", a);
		DrawMultiCombobox(x + 20, y + 177, g_Var.Misc.ZeusBot, 14, 1, 65, 14, sZeusKnife, "Zeus Options", a);
		DrawMultiCombobox( x + 20, y + 137, g_Var.Misc.Indicator, 10, 5, 65, 14, sIndicator, "Indicator", a );
		DrawMultiCombobox( x + 20, y + 117, g_Var.Misc.EventLog, 11, 2, 65, 14, sEventLogs, "Event Logs", a );
		DrawColorPicker( x + 20, y + 57, 20, g_Var.Color.Menu[ 0 ], g_Var.Color.Menu[ 1 ], g_Var.Color.Menu[ 2 ], 21, "Menu Color", 223, a );

		g_Draw.DrawFeatureBox( x + 281, y + 48, 264, 110, 33, 33, 33, a, clamp( a, 0, 80 ), 200, 200, 200, clamp( a, 0, 120 ), "Movement", g_Draw.visualsfont, true );

		DrawCheckbox( x + 291, y + 57, g_Var.Misc.AutoJump, "Auto Jump", a );
		DrawCheckbox( x + 291, y + 77, g_Var.Misc.AutoStrafe, "Auto Strafe", a );
		DrawCheckbox( x + 291, y + 97, g_Var.Misc.CrouchJump, "Crouch Jump", a );
		DrawCheckbox( x + 291, y + 117, g_Var.Misc.Slide, "Slide", a );
		DrawKeybox(x + 290, y + 137, g_Var.Misc.FakeDuck, 21, "Fake Duck", a, 235);
//		DrawCheckbox( x + 291, y + 137, g_Var.Misc.CounterStrafe, "Fast Stop", a );

		g_Draw.DrawFeatureBox( x + 281, y + 166, 264, 90, 33, 33, 33, a, clamp( a, 0, 80 ), 200, 200, 200, clamp( a, 0, 120 ), "Buy Bot", g_Draw.visualsfont, true );

		g_Draw.DrawFeatureBox( x + 281, y + 362, 264, 36, 33, 33, 33, a, clamp( a, 0, 80 ), 200, 200, 200, clamp( a, 0, 120 ), "Config", g_Draw.visualsfont, true );

		LoadButton( x + 426, y + 371, 50, 20, a );

		SaveButton( x + 486, y + 371, 50, 20, a );

		DrawCombobox( x + 291, y + 375, g_ConfigFile.m_iID, 27, 2, 125, 14, sConfig, "", a, 0 );

		g_Draw.DrawFeatureBox( x + 281, y + 264, 264, 90, 33, 33, 33, a, clamp( a, 0, 80 ), 200, 200, 200, clamp( a, 0, 120 ), "Skins", g_Draw.visualsfont, true );

		static int iActiveWeapon;

		if( pLocal && pLocal != nullptr && pLocal->GetHealth() > 0 )
		{
			CBaseCombatWeapon* pWeapon = pLocal->GetWeapon();

			if( pWeapon && pWeapon != nullptr )
			{
				if( !pWeapon->IsGrenade() && !pWeapon->IsOther() )
				{
					iActiveWeapon = pWeapon->GetWeaponID();
					g_Draw.DrawString( g_Draw.visualsfont, false, x + 291 + 50, y + 273, 200, 200, 200, clamp( a, 0, 150 ), "| %s |", pWeapon->GetWeaponName() );
				}
			}
		}
		
		UpdateButton( x + 291, y + 333, 244, 14, a );

		DrawCheckbox( x + 291, y + 273, g_Skins.Enable[ iActiveWeapon ], "PaintKit", a, 160 );
		DrawNumberBox( x + 291, y + 274, g_Skins.PaintKit[ iActiveWeapon ], 0, 65, 14, "", a );
//		DrawSlider( x + 291, y + 293, 120, 5, g_Skins.Condition[ iActiveWeapon ], 0, 100, "Condition", a, false );

		DrawNumberBox( x + 291, y + 313, g_Skins.GlovePaint, 1, 65, 14, "", a );
		DrawCombobox( x + 291, y + 313, g_Skins.Glove, 26, 7, 65, 14, sGloves, "Glove", a, 109 );
		DrawCombobox( x + 291, y + 293, g_Skins.Knife, 25, 10, 65, 14, sKnives, "Knife", a );

		DrawMultiCombobox( x + 291, y + 235, g_Var.Misc.BuyBot.Grenades, 13, 4, 65, 14, sGrenade, "Grenades", a );
		DrawMultiCombobox( x + 291, y + 215, g_Var.Misc.BuyBot.Gear, 12, 2, 65, 14, sGear, "Gear", a );
		DrawCombobox( x + 291, y + 195, g_Var.Misc.BuyBot.Secondary, 24, 3, 65, 14, sSecondary, "Secondary", a );
		DrawCombobox( x + 291, y + 175, g_Var.Misc.BuyBot.Primary, 23, 5, 65, 14, sPrimary, "Primary", a );

		break;
	}

	GetMouse();
	DrawMouse();
}