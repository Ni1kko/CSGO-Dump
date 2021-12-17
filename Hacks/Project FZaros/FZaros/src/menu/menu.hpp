#pragma once
#include "../utilities/includes.h"
#include "sprites/sprite.hpp"
#include "../utilities/renderer_d3d.hpp"

class c_element {
protected:
	Vector2 position;
	Vector2 size;

	std::string label;
	std::string var_name;

	std::vector< c_element* > children;
	c_element *parent;

public:
	virtual void think( ) = 0;
	virtual void draw( ) = 0;

	virtual c_element *get_parent( ) {
		return this->parent;
	}
	virtual void set_parent( c_element *parent ) {
		this->parent = parent;
	}

	virtual bool is_child( c_element *child ) {
		for ( c_element *elem : this->children )
			if ( elem == child )
				return true;

		return false;
	}
	virtual void add_child( c_element *child ) {
		this->children.push_back( child );
		child->set_parent( this );
	}

	virtual void set_label( const std::string &label ) {
		this->label = std::move( label );
	}
	virtual std::string get_label( ) {
		return this->label;
	}

	virtual void set_var_name( const std::string &var_name ) {
		this->var_name = std::move( var_name );
	}
	virtual std::string get_var_name( ) {
		return this->var_name;
	}

	virtual void set_size( const Vector2 &size ) {
		this->size = size;
	}
	virtual Vector2 get_size( ) {
		return this->size;
	}

	virtual void set_position( const Vector2 &position ) {
		this->position = position;
	}
	virtual Vector2 get_position( ) {
		return this->position;
	}

	virtual Vector2 get_child_draw_position( ) {
		if ( this->parent == nullptr )
			return this->position;

		return this->parent->get_child_draw_position( ) + this->position;
	}

};

class c_tab;

class c_menu : public c_element {
private:
	bool initialized = false;
	bool dragging = false;
	int menu_alpha = 0;

	Vector2 previous_mouse_pos;
	Vector2 mouse_pos;

	std::unordered_map<std::string, bool> hotkey_states;
	bool previous_key_state[ 256 ];
	bool key_state[ 256 ];
	int mouse_wheel;

	std::vector< c_tab* > tabs;
	c_tab *active_tab;

	c_element *blocking = nullptr;
	c_sprite *menu_background;

	IDirect3DTexture9 *menu_texture;
	LPD3DXSPRITE menu_texture_sprite;

public:
	void think( ) override;
	void draw( ) override;

	Vector2 get_child_draw_position( ) override;

	void poll_input( );
	bool is_key_down( int key );
	bool is_key_pressed( int key );
	bool is_key_released( int key );

	void set_mouse_wheel( int mouse_wheel );
	int get_mouse_wheel( );

	Vector2 get_mouse_pos( );
	bool in_bounds( Vector2 pos, Vector2 size );
	bool is_hovered( Vector2 min, Vector2 max );

	void add_tab( c_tab *tab );

	bool is_blocking( c_element *elem = nullptr );
	void block( c_element *elem );
	c_element *get_blocking( );

	c_tab *get_active_tab( );
	void set_active_tab( c_tab *tab );

	bool get_hotkey( const std::string &value );

	bool opened = false;
	Vector2 fixed_position;

};
extern c_menu *menu;