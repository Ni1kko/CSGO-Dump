#include "group.hpp"

Vector2 c_group::get_cursor_pos( ) {
	return this->cursor_pos;
}

void c_group::set_cursor_pos( const Vector2 &pos ) {
	this->cursor_pos = pos;
}

c_group::c_group( const std::string &label, const Vector2 &size ) {
	this->label = label;
	this->size = size;
}

void c_group::think( ) {
	Vector2 draw_position = this->get_parent( )->get_child_draw_position( ) + this->position;
	bool only_update_blocking = menu->is_blocking( );

	float content_height = this->cursor_pos.y - 16;

	if ( menu->get_mouse_wheel( ) != 0 && menu->is_hovered( draw_position, draw_position + size ) ) {
		if ( content_height > this->size.y - 36 ) {
			if ( !menu->is_blocking( ) )
				this->scroll += menu->get_mouse_wheel( ) * 12;

			menu->set_mouse_wheel( 0 );

			if ( this->scroll > 0 )
				this->scroll = 0;
			else if ( this->scroll < ( size.y - 36 ) - content_height )
				this->scroll = ( size.y - 36 ) - content_height;
		}
	}

	for ( c_element *child : this->children ) {
		if ( ( !only_update_blocking && menu->is_hovered( draw_position, draw_position + size ) ) ||
			( only_update_blocking && menu->is_blocking( child ) ) )
		{
			child->think( );
		}
	}
}

void c_group::draw( ) {
	Vector2 draw_position = this->get_parent( )->get_child_draw_position( ) + this->position;

	g_renderer_d3d->rect_fill( draw_position.x, draw_position.y, size.x, size.y, Color( 24, 24, 24 ) );
	g_renderer_d3d->rect( draw_position.x - 1, draw_position.y - 1, size.x + 2, size.y + 2, Color( 44, 44, 44 ) );

	Vector2 text_size = g_renderer_d3d->menu_bold->measure_size( this->label );

	D3DVIEWPORT9 old_viewport = g_renderer_d3d->get_viewport( );
	g_renderer_d3d->set_viewport( { ( DWORD )draw_position.x, ( DWORD )draw_position.y, ( DWORD )size.x, ( DWORD )size.y, 0.f, 1.f } );

	for ( c_element *child : this->children ) {
		if ( !menu->is_blocking( ) || ( menu->is_blocking( ) && !menu->is_blocking( child ) ) )
			child->draw( );
	}

	g_renderer_d3d->rect_fill( draw_position, { size.x, 5 }, { 24, 24, 24, 255 } );
	g_renderer_d3d->gradient_v( draw_position + Vector2( 0, 5 ), { size.x, 12 }, { 24, 24, 24, 255 }, { 24, 24, 24, 0 } );

	if ( this->cursor_pos.y - 16 > this->size.y - 36 )
		g_renderer_d3d->gradient_v( draw_position + Vector2( 0, size.y - 12 ), { size.x, 12 }, { 24, 24, 24, 0 }, { 24, 24, 24, 255 } );

	g_renderer_d3d->set_viewport( old_viewport );

	g_renderer_d3d->line( draw_position.x + 6, draw_position.y - 1, draw_position.x + text_size.x + 9, draw_position.y - 1, Color( 17, 17, 17 ) );
	g_renderer_d3d->line( draw_position.x + 6, draw_position.y - 2, draw_position.x + text_size.x + 9, draw_position.y - 2, Color( 17, 17, 17 ) );

	g_renderer_d3d->menu_bold->draw_text( this->label, { draw_position.x + 8, draw_position.y - 1 - ( text_size.y / 2 ) }, Color( 255, 255, 255 ), font_flags_d3d::font_dropshadow );
}

void c_group::add_child( c_element *child ) {
	c_element::add_child( child );

	this->cursor_pos.y += child->get_size( ).y + 16;
	child->set_position( this->cursor_pos - Vector2( 0, child->get_size( ).y + 16 ) );
}

Vector2 c_group::get_child_draw_position( ) {
	return this->get_parent( )->get_child_draw_position( ) + this->position + Vector2( 16, 20 + this->scroll );
}
