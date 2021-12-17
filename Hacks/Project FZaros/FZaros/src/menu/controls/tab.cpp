#include "tab.hpp"

Vector2 c_tab::get_cursor_pos( ) {
	return this->cursor_pos;
}

void c_tab::set_cursor_pos( const Vector2 &pos ) {
	this->cursor_pos = pos;
}

c_tab::c_tab( const std::string &label ) {
	this->label = label;
}

void c_tab::think( ) {
	for ( c_element *child : this->children )
		child->think( );
}

void c_tab::draw( ) {
	for ( c_element *child : this->children )
		child->draw( );
}

void c_tab::add_child( c_element *child ) {
	c_element::add_child( child );

	this->cursor_pos.y += child->get_size( ).y + 16;
	
	if ( this->cursor_pos.y - 16 >= ( this->parent->get_size( ).y - 32 ) ) {
		this->cursor_pos.x += child->get_size( ).x + 16;
		this->cursor_pos.y = child->get_size( ).y + 16;
	}

	child->set_position( this->cursor_pos - Vector2( 0, child->get_size( ).y + 16 ) );
}

Vector2 c_tab::get_child_draw_position( ) {
	return c_element::get_child_draw_position( );
}
