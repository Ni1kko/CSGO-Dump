#pragma once
#include "../../utilities/includes.h"
class c_sprite {
private:
	LPDIRECT3DTEXTURE9 texture;
	bool began;
	int w, h;
public:
	LPD3DXSPRITE sprite;
	template <typename t>
	void setup( IDirect3DDevice9* device, t texture_bytes, int size, int width, int height ) {
		this->w = width;
		this->h = height;
		D3DXCreateTextureFromFileInMemoryEx( device, ( void* )texture_bytes, size, width, height, 0, D3DPOOL_DEFAULT, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, D3DCOLOR_RGBA( 0, 0, 0, 255 ), NULL, NULL, &texture );
		D3DXCreateSprite( device, &sprite );
	}
	void invalidate( );
	void draw( Vector2 pos, int alpha = 255, float rotation = 0.f );
	void draw( int x, int y, int alpha = 255, float rotation = 0.f );
};
