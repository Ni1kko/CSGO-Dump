#include "sprite.hpp"

void c_sprite::invalidate( ) {
	this->texture->Release( );
	delete this->texture;
	this->sprite->Release( );
	delete this->sprite;
}

void c_sprite::draw( Vector2 pos, int alpha, float rotation ) {
	D3DXVECTOR3 position;

	position.x = pos.x;
	position.y = pos.y;
	position.z = 0.01f;

	D3DXVECTOR3 scaling( 1.f, 1.f, 1.f );
	D3DXVECTOR3 centre( 100, 100, 0 );
	D3DXMATRIX matrix;

	D3DXMatrixTransformation( &matrix, NULL, 0, &scaling, &centre, NULL, &position );

	sprite->SetTransform( &matrix );

	sprite->Draw( texture, NULL, NULL, NULL, D3DCOLOR_RGBA( 255, 255, 255, alpha ) );
}

void c_sprite::draw( int x, int y, int alpha, float rotation ) {
	draw( Vector2( x, y ), alpha, rotation );
}
