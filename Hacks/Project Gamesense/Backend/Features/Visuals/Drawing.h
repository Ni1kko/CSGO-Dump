#pragma once
#include "../../Include/GlobalIncludes.h"
#include "../../SDK/color.h"
#include "../../Interfaces/Interfaces.h"

namespace Graphics
{
	class CDrawing
	{
		public:
			class Fonts
			{
				public:
					DWORD esp_main;
					DWORD esp_name;
					DWORD esp_indicator;
					DWORD esp_side;
					DWORD esp_icons;
					DWORD eventlogs;
					DWORD weapon_icons;
			} fonts;

			void Initialize();

			void Rect( int x, int y, int w, int h, CColor color );
			void rect_filled(int x, int y, int w, int h, CColor color);
			void RectP( int x, int y, int x2, int y2, CColor color );
			void RectOutlined( int x, int y, int w, int h, CColor color, CColor outline );
			void Outline( int x, int y, int w, int h, CColor color );
			void Line( int x, int y, int x2, int y2, CColor color );
			void PolyLine( int* x, int* y, int count, CColor color );

			void Polygon( int count, Vertex_t* vertexs, CColor color );
			void PolygonOutlined( int count, Vertex_t* vertexs, CColor color, CColor outline );

			void GradientVertical( int x, int y, int w, int h, CColor c1, CColor c2 );
			void GradientHorizontal( int x, int y, int w, int h, CColor c1, CColor c2 );

			void Text(int x, int y, const char* text, int font, CColor color);
			RECT TextSize( const char* text, int font );

			void Text(int x, int y, const wchar_t* text, int font, CColor color);
			RECT TextSize(const wchar_t* text, int font);

			void Draw3DCube( float scalar, Vector angles, Vector middle_origin, CColor outline );

			void DrawCircle( int x, int y, int radius, CColor c );

		private:
			static bool m_init;
	};

	extern CDrawing* Drawing;
}