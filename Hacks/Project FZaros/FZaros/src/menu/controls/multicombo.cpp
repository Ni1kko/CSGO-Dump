#include "multicombo.hpp"

std::string construct_label( const std::vector<std::string> &items, int value ) {
	if ( value == 0 )
		return "-";

	std::string result;

	for ( int i = 0; i < items.size( ); i++ ) {
		bool length_exceeded = result.length( ) >= 16;
		auto first_item = result.empty( );

		if ( value & ( 1 << ( i + 1 ) ) && !length_exceeded ) {
			if ( !first_item )
				result.append( ", " );

			result.append( items[ i ] );
		}
		else if ( length_exceeded ) {
			result.append( "..." );
			break;
		}
	}

	return result;
}

c_multicombo::c_multicombo( const std::string &label, const std::string &var_name, const std::vector<std::string> &items ) {
	this->label = std::move( label );
	this->var_name = std::move( var_name );
	this->items = std::move( items );
	this->size = Vector2( 150, 27 );
}

void c_multicombo::think( ) {
	float group_width = this->get_parent( )->get_size( ).x - 61.0f + 8 - 30;

	Vector2 draw_position = this->get_parent( )->get_child_draw_position( ) + this->position + Vector2( 16, 15 );
	Vector2 combo_size( group_width - 2, 18 );

	Vector2 first_item_min( draw_position + Vector2( 16, 23 ) );
	Vector2 last_item_max( first_item_min + Vector2( combo_size.x, combo_size.y * this->items.size( ) ) - Vector2( 0, 1 ) );

	if ( menu->is_hovered( draw_position, draw_position + combo_size ) && menu->is_key_pressed( VK_LBUTTON ) && !menu->is_blocking( this ) ) {
		menu->block( this );
	}
	else if ( !menu->is_hovered( first_item_min, last_item_max ) && menu->is_key_pressed( VK_LBUTTON ) && menu->is_blocking( this ) ) {
		menu->block( nullptr );
	}

	if ( menu->is_blocking( this ) && menu->is_key_pressed( VK_LBUTTON ) ) {
		int value = config->get_int( this->var_name );

		for ( int i = 0; i < this->items.size( ); i++ ) {
			Vector2 item_min( draw_position + Vector2( 16, 23 + ( combo_size.y * i ) ) );
			Vector2 item_max( item_min + combo_size - Vector2( 0, 1 ) );

			if ( menu->is_hovered( item_min, item_max ) ) {
				if ( value & ( 1 << ( i + 1 ) ) )
					value &= ~( 1 << ( i + 1 ) );
				else
					value |= ( 1 << ( i + 1 ) );
			}
		}

		config->set( this->var_name, value );
	}
}

void c_multicombo::draw( ) {
	float group_width = this->get_parent( )->get_size( ).x - 61.0f + 8 - 30;

	Vector2 draw_position = this->get_parent( )->get_child_draw_position( ) + this->position;
	Vector2 combo_size( group_width - 2, 18 );

	g_renderer_d3d->rect( draw_position.x + 15, draw_position.y + 14, combo_size.x + 2, combo_size.y + 2, Color( 17, 17, 17 ) );

	if ( menu->is_hovered( draw_position + Vector2( 16, 15 ), draw_position + Vector2( 16, 15 ) + combo_size ) && !menu->is_blocking( this ) )
		g_renderer_d3d->gradient_v( draw_position.x + 16, draw_position.y + 15, combo_size.x, combo_size.y, Color( 54, 54, 54 ),
			Color( 40, 40, 40 ) );
	else
		g_renderer_d3d->gradient_v( draw_position.x + 16, draw_position.y + 15, combo_size.x, combo_size.y, Color( 50, 50, 50 ),
			Color( 35, 35, 35 ) );

	auto arrow = [ ] ( int x, int y, Color clr ) {
		for ( auto i = 5; i >= 2; --i ) {
			auto offset = 5 - i;

			g_renderer_d3d->line( x + offset, y + offset, x + offset + std::clamp( i - offset, 0, 5 ), y + offset, clr );
		}
	};

	arrow( draw_position.x + combo_size.x + 4, draw_position.y + ( combo_size.y + 4 ), Color( 0, 0, 0 ) );
	arrow( draw_position.x + combo_size.x + 4, draw_position.y + ( combo_size.y + 5 ), Color( 151, 151, 151 ) );

	g_renderer_d3d->menu->draw_text( this->label, { draw_position.x + 15, draw_position.y - 3 }, Color( 220, 220, 220 ), font_flags_d3d::font_dropshadow );
	g_renderer_d3d->menu->draw_text( construct_label( this->items, config->get_int( this->var_name )), { draw_position.x + 21, draw_position.y + 17 }, Color( 220, 220, 220 ),
		font_flags_d3d::font_dropshadow );

	if ( menu->is_blocking( this ) ) {
		int value = config->get_int( this->var_name );
		g_renderer_d3d->rect( draw_position.x + 15, draw_position.y + 19 + combo_size.y, combo_size.x + 2, ( combo_size.y * this->items.size( ) ) + 2, Color( 17, 17, 17 ) );

		for ( int i = 0; i < this->items.size( ); i++ ) {
			Vector2 item_min( draw_position + Vector2( 16, 20 + ( combo_size.y * ( i + 1 ) ) ) );
			Vector2 item_max( item_min + combo_size - Vector2( 0, 1 ) );

			bool is_hovered = menu->is_hovered( item_min, item_max );
			bool is_active = value & ( 1 << ( i + 1 ) );

			g_renderer_d3d->rect_fill( item_min.x, item_min.y, combo_size.x, combo_size.y, is_hovered ? Color( 60, 60, 60 ) : Color( 44, 44, 44 ) );

			if ( is_hovered || is_active ) {
				g_renderer_d3d->menu_bold->draw_text( this->items[ i ], { item_min.x + 5, item_min.y + 2 },
					is_active ? config->get_color( "clr_menu" ) : Color( 220, 220, 220 ), font_flags_d3d::font_dropshadow );
			}
			else {
				g_renderer_d3d->menu->draw_text( this->items[ i ], { item_min.x + 5, item_min.y + 2 },
					{ 220, 220, 220 }, font_flags_d3d::font_dropshadow );

			}
		}
	}
}
