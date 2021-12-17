#pragma once
#include "../../Backend/Include/GlobalIncludes.h"

#define get_a(col) (((col)&0xff000000)>>24)
#define get_r(col) (((col)&0x00ff0000)>>16)
#define get_g(col) (((col)&0x0000ff00)>>8)
#define get_b(col) ((col)&0x000000ff)

enum CircleType
{
	FULL, HALF, QUARTER
};

enum TextAlignment
{
	LEFT, CENTER, RIGHT
};

struct Vertex
{
	float x, y, z, rhw;
	unsigned long color;
};

namespace Graphics
{
	namespace Fonts
	{
		extern LPD3DXFONT Font_INF;
		extern LPD3DXFONT Font_ESP;
		extern LPD3DXFONT Font_IND;
	}

	class CRender
	{
		public:
			void Initialize( IDirect3DDevice9* pDevice );
			void Reset();

			inline bool IsInitialized()
			{
				return m_initialized;
			};

			void CreateObjects();
			void ReleaseObjects();

			void Begin();
			void End();

			int StringWidth( ID3DXFont* pFont, std::string szStr );
			int StringHeight( ID3DXFont* pFont, std::string szStr );

			void Line( int x, int y, int x2, int y2, D3DCOLOR color );
			void Box( int x, int y, int w, int h, D3DCOLOR color );

			void FilledRect( int x, int y, int w, int h, D3DCOLOR color );
			void FilledRectOutlined( int x, int y, int w, int h, D3DCOLOR color, D3DCOLOR outline, int thickness = 1 );

			void BorderedRect( int x, int y, int w, int h, D3DCOLOR color, int thickness = 1 );
			void BorderedRectOutlined( int x, int y, int w, int h, D3DCOLOR color, D3DCOLOR outline, int thickness = 1 );

			void Gradient( int x, int y, int w, int h, D3DCOLOR color, D3DCOLOR other_color, bool vertical = false );
			void GradientOutlined( int x, int y, int w, int h, D3DCOLOR color, D3DCOLOR other_color, D3DCOLOR outline_color, bool vertical = false );

			void BorderedCircle( int x, int y, int r, int p, D3DCOLOR color );
			void FilledCircle( int x, int y, int r, int p, D3DCOLOR color );
			void GradientCircle( float x, float y, float rad, float rotate, int type, int resolution, D3DCOLOR color, D3DCOLOR color2 );
			void RainbowCircle( float x, float y, float rad, float rotate, int type, int resolution );

			void Text( std::string text, int x, int y, int o, LPD3DXFONT font, bool bordered, D3DCOLOR color );

		private:
			struct screen_t
			{
				float w;
				float h;
				float xc;
				float yc;
			} m_screen;

			LPDIRECT3DDEVICE9			m_device;
			LPDIRECT3DVERTEXBUFFER9		m_vb;
			LPDIRECT3DINDEXBUFFER9		m_ib;
			IDirect3DStateBlock9*		m_stateblock;

			LPD3DXLINE		m_line;
			LPD3DXSPRITE	m_sprite;

			bool m_candraw = false;
			static bool m_initialized;
	};

	extern CRender* Render;
}
