#include "main/setup.hpp"
#include "menu.hpp"
#include "config/config.hpp"

#include "menu.hpp"
#include "controls/tab.hpp"
#include "controls/group.hpp"
#include "controls/checkbox.hpp"
#include "controls/slider.hpp"
#include "controls/combo.hpp"
#include "controls/multicombo.hpp"
#include "controls/hotkey.hpp"
#include "controls/colorpicker.hpp"
#include "controls/button.hpp"

#include "menu.hpp"
#include "controls/tab.hpp"
#include "sprites/texture.hpp"

#define DopamineTabSize 462

void __stdcall d3dx_color_fill( D3DXVECTOR4 *out, const D3DXVECTOR2 *texture_coordinates, const D3DXVECTOR2 *texel_size, LPVOID data ) {
	D3DCOLORVALUE *color = ( D3DCOLORVALUE* )data;
	*out = D3DXVECTOR4( color->r, color->g, color->b, color->a );
}

void c_menu::poll_input( ) {
	this->previous_mouse_pos = this->mouse_pos;

	for ( int i = 0; i < 256; i++ ) {
		this->previous_key_state[ i ] = this->key_state[ i ];
		this->key_state[ i ] = GetAsyncKeyState( i );
	}

	POINT mouse_point;

	GetCursorPos( &mouse_point );
	ScreenToClient( FindWindow( "Valve001", "Counter-Strike: Global Offensive" ), &mouse_point );

	this->mouse_pos = Vector2( mouse_point.x, mouse_point.y );
}

bool c_menu::is_key_down( int key ) {
	return this->key_state[ key ] == true;
}

bool c_menu::is_key_pressed( int key ) {
	return this->previous_key_state[ key ] == false && this->key_state[ key ] == true;
}

bool c_menu::is_key_released( int key ) {
	return this->previous_key_state[ key ] == true && this->key_state[ key ] == false;
}

void c_menu::set_mouse_wheel( int mouse_wheel ) {
	this->mouse_wheel = mouse_wheel;
}

int c_menu::get_mouse_wheel( ) {
	return this->mouse_wheel;
}

Vector2 c_menu::get_mouse_pos( ) {
	return this->mouse_pos;
}

bool c_menu::in_bounds( Vector2 pos, Vector2 size ) {
	// grab cursor
	auto cursor = get_mouse_pos( );

	// math for inbounds
	return ( cursor.x > pos.x &&
		cursor.y > pos.y &&
		cursor.x < pos.x + size.x &&
		cursor.y < pos.y + size.y );
}


bool c_menu::is_hovered( Vector2 min, Vector2 max ) {
	return this->mouse_pos.x >= min.x && this->mouse_pos.y >= min.y &&
		this->mouse_pos.x <= max.x && this->mouse_pos.y <= max.y;
}

void c_menu::add_tab( c_tab *tab ) {
	tab->set_parent( this );

	this->tabs.push_back( tab );
	this->active_tab = tab;
}

bool c_menu::is_blocking( c_element *elem ) {
	if ( elem == nullptr )
		return this->blocking != nullptr;

	return this->blocking == elem;
}

void c_menu::block( c_element *elem ) {
	this->blocking = elem;
}

c_element *c_menu::get_blocking( ) {
	return this->blocking;
}

c_tab *c_menu::get_active_tab( ) {
	return this->active_tab;
}

void c_menu::set_active_tab( c_tab *tab ) {
	this->active_tab = tab;
}

bool c_menu::get_hotkey( const std::string &value ) {
	int key = config->get_int( value );
	int activation_type = config->get_int( value + "_type", 1 );

	switch ( activation_type ) {
	case 0:
		return true;
		break;
	case 1:
		return menu->is_key_down( key );
		break;
	case 2:
		if ( hotkey_states.find( value ) == hotkey_states.end( ) ) hotkey_states[ value ] = menu->is_key_down( key );
		else if ( menu->is_key_pressed( key ) ) hotkey_states[ value ] = !hotkey_states[ value ];
		return hotkey_states.at( value );
		break;
	case 3:
		return !menu->is_key_down( key );
		break;
	}
}

void c_menu::think( ) {
	if ( !initialized )
		return;

	this->poll_input( );

	if ( this->is_hovered( fixed_position - Vector2( 6, 6 ), fixed_position + Vector2( size.x + 6, 6 ) ) ) {
		if ( this->is_key_pressed( VK_LBUTTON ) && !this->dragging )
			this->dragging = true;
	}
	else if ( !this->is_blocking( ) ) {
		for ( int i = 0; i < ( int )tabs.size( ); i++ ) {
			c_tab *tab = tabs[ i ];

			int tab_text_width, tab_text_height;

			Vector2 text_size = g_renderer_d3d->menu->measure_size( tab->get_label( ) );

			const int tab_width = (DopamineTabSize / ( int )tabs.size( ) );

			Vector2 tab_hover_min( fixed_position.x + 19 + ( tab_width * i ), fixed_position.y + ( this->size.y - 40 ) );
			Vector2 tab_hover_max = tab_hover_min + Vector2( tab_width, 25 );

			if ( this->is_hovered( tab_hover_min, tab_hover_max ) && this->is_key_pressed( VK_LBUTTON ) )
				this->active_tab = tab;
		}
	}

	if ( this->is_key_down( VK_LBUTTON ) && this->dragging ) {
		Vector2 mouse_delta = this->previous_mouse_pos - this->mouse_pos;
		this->position -= mouse_delta;
	}

	if ( !this->is_key_down( VK_LBUTTON ) && this->dragging )
		this->dragging = false;

	this->active_tab->think( );
}

void c_menu::draw( ) {
	if ( this->menu_background == nullptr ) {
		this->menu_background = new c_sprite( );
		this->menu_background->setup( g_renderer_d3d->device, background_texture_bytes, sizeof( background_texture_bytes ), 1000, 1000 );

		D3DXCreateSprite( g_renderer_d3d->device, &menu_texture_sprite );

		int screen_w, screen_h;
		g_Interfaces->gameEngine->getScreenSize( screen_w, screen_h );

		g_renderer_d3d->device->CreateTexture( screen_w, screen_h, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &menu_texture, nullptr );
		initialized = true;
	}

	constexpr float fade_factor = 6.666667f;
	float fade_increment = ( fade_factor * g_Interfaces->globalVars->frametime ) * 255.0f;

	if ( menu->opened )
		menu_alpha += ( int )fade_increment;
	else
		menu_alpha -= ( int )fade_increment;

	menu_alpha = max( 0, min( 255, menu_alpha ) );

	if ( menu_alpha == 0 )
		return;

	IDirect3DSurface9 *back_buffer_surface;
	IDirect3DSurface9 *menu_render_surface;

	menu_texture->GetSurfaceLevel( 0, &menu_render_surface );

	g_renderer_d3d->device->GetRenderTarget( 0, &back_buffer_surface );
	g_renderer_d3d->device->SetRenderTarget( 0, menu_render_surface );

	D3DVIEWPORT9 old_viewport = g_renderer_d3d->get_viewport( );

	fixed_position.x = max( 6, min( position.x, old_viewport.Width - size.x - 6 ) );
	fixed_position.y = max( 6, min( position.y, old_viewport.Height - size.y - 6 ) );

	g_renderer_d3d->set_viewport( { ( DWORD )fixed_position.x, ( DWORD )fixed_position.y, ( DWORD )size.x, ( DWORD )size.y, 0.f, 1.f } );

	this->menu_background->sprite->Begin( D3DXSPRITE_ALPHABLEND );
	this->menu_background->draw( fixed_position );
	this->menu_background->sprite->End( );

	g_renderer_d3d->set_viewport( old_viewport );

	g_renderer_d3d->rect( fixed_position.x - 1, fixed_position.y - 1, size.x + 2, size.y + 2, Color( 60, 60, 60 ) );
	g_renderer_d3d->rect( fixed_position.x - 2, fixed_position.y - 2, size.x + 4, size.y + 4, Color( 40, 40, 40 ) );
	g_renderer_d3d->rect( fixed_position.x - 3, fixed_position.y - 3, size.x + 6, size.y + 6, Color( 40, 40, 40 ) );
	g_renderer_d3d->rect( fixed_position.x - 4, fixed_position.y - 4, size.x + 8, size.y + 8, Color( 40, 40, 40 ) );
	g_renderer_d3d->rect( fixed_position.x - 5, fixed_position.y - 5, size.x + 10, size.y + 10, Color( 60, 60, 60 ) );
	g_renderer_d3d->rect( fixed_position.x - 6, fixed_position.y - 6, size.x + 12, size.y + 12, Color( 31, 31, 31 ) );

	g_renderer_d3d->rect_fill( fixed_position.x + 1, fixed_position.y + 1, size.x - 2, 2, config->get_color( "clr_menu" ) );
	g_renderer_d3d->line( fixed_position.x + 1, fixed_position.y + 2, fixed_position.x + size.x - 1, fixed_position.y + 2,
		Color( 0, 0, 0, ( int )( 80.0f * ( ( float )config->get_color( "clr_menu" ).a( ) / 255.0f ) ) ) );

	g_renderer_d3d->rect_fill( fixed_position.x + 19, fixed_position.y + this->size.y - 40, DopamineTabSize, 25, Color( 24, 24, 24 ) );
	g_renderer_d3d->rect( fixed_position.x + 18, fixed_position.y + this->size.y - 41, DopamineTabSize + 2, 27, Color( 44, 44, 44 ) );

	for ( int i = 0; i < ( int )tabs.size( ); i++ ) {
		c_tab *tab = tabs[ i ];

		auto tab_text_color = tab == active_tab ? config->get_color( "clr_menu" ) : Color( 210, 210, 210 );
		auto tab_text_font = tab == active_tab ? g_renderer_d3d->menu_bold : g_renderer_d3d->menu;

		const int tab_width = (DopamineTabSize / ( int )tabs.size( ) );

		int tab_center_x = fixed_position.x + 19 + ( tab_width * i ) + ( tab_width / 2 );
		int tab_center_y = fixed_position.y + ( this->size.y - 40 ) + ( 25 / 2 );

		Vector2 tab_text_size = tab_text_font->measure_size( tab->get_label( ) );

		tab_text_font->draw_text( tab->get_label( ), { tab_center_x - ( tab_text_size.x / 2 ), tab_center_y - ( tab_text_size.y / 2 ) },
			tab_text_color, font_flags_d3d::font_dropshadow );

		if ( i != tabs.size( ) - 1 ) {
			g_renderer_d3d->line( { float( tab_center_x + ( tab_width / 2 ) ), float( tab_center_y - 4 ) },
				{ float( tab_center_x + ( tab_width / 2 ) ), float( tab_center_y + 5 ) }, { 44, 44, 44 } );
		}
	}

	this->active_tab->draw( );

	if ( this->is_blocking( ) )
		this->get_blocking( )->draw( );

	g_renderer_d3d->device->SetRenderTarget( 0, back_buffer_surface );

	menu_texture_sprite->Begin( D3DXSPRITE_ALPHABLEND );

	D3DXVECTOR3 position = { 0.0f, 0.0f, 0.01f };
	D3DXVECTOR3 scaling( 1.f, 1.f, 1.f );
	D3DXVECTOR3 spriteCentre( 100, 100, 0 );

	D3DXMATRIX matrix;
	D3DXMatrixTransformation( &matrix, NULL, 0, &scaling, &spriteCentre, NULL, &position );

	menu_texture_sprite->SetTransform( &matrix );
	menu_texture_sprite->Draw( menu_texture, NULL, NULL, NULL, D3DCOLOR_RGBA( 255, 255, 255, menu_alpha ) );
	menu_texture_sprite->End( );

	g_renderer_d3d->device->SetRenderTarget( 0, menu_render_surface );
	g_renderer_d3d->device->Clear( 0, nullptr, D3DCLEAR_TARGET, D3DCOLOR_RGBA( 255, 255, 255, 0 ), 0.01f, 0 );
	g_renderer_d3d->device->SetRenderTarget( 0, back_buffer_surface );
}

Vector2 c_menu::get_child_draw_position( ) {
	return fixed_position + Vector2( 19, 22 );
}

c_menu *menu = new c_menu( );
