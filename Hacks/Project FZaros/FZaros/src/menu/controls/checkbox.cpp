#include "checkbox.hpp"

c_checkbox::c_checkbox( const std::string &label, const std::string &var_name ) {
	this->label = std::move( label );
	this->var_name = std::move( var_name );
	this->size = Vector2( 7, 3 );
}

void c_checkbox::think( ) {
	Vector2 draw_position = this->get_parent( )->get_child_draw_position( ) + this->position;

	Vector2 text_size = g_renderer_d3d->menu->measure_size( this->label );

	if ( menu->is_hovered( draw_position, draw_position + Vector2( 7, 7 ) ) && menu->is_key_pressed( VK_LBUTTON ) )
		config->set( var_name, !config->get_bool( var_name ) );
	else if ( menu->is_hovered( draw_position + Vector2( 15, -2 ), draw_position + Vector2( 15, -2 ) + Vector2( text_size.x, text_size.y ) ) && menu->is_key_pressed( VK_LBUTTON ) )
		config->set( var_name, !config->get_bool( var_name ) );
}

void c_checkbox::draw( ) {
	Vector2 draw_position = this->get_parent( )->get_child_draw_position( ) + this->position;

	g_renderer_d3d->rect( draw_position.x, draw_position.y, 9, 9, Color( 17, 17, 17 ) );

	Color colour = Color(
		config->get_color( "clr_menu" ).r(),
		config->get_color( "clr_menu" ).g(),
		config->get_color( "clr_menu" ).b(),
		config->get_color( "clr_menu" ).a()
	);

	if ( config->get_bool( var_name ) )
		g_renderer_d3d->gradient_v( draw_position.x + 1, draw_position.y + 1, 7, 7, colour,
			Color( colour.r() * 0.44, colour.g() * 0.44, colour.b() * 0.44, colour.a() ) );
	else
		g_renderer_d3d->gradient_v( draw_position.x + 1, draw_position.y + 1, 7, 7, Color( 44, 44, 44 ),
			Color( 29, 29, 29 ) );

	g_renderer_d3d->menu->draw_text( this->label, { draw_position.x + 15, draw_position.y - 2 }, Color( 220, 220, 220 ), font_flags_d3d::font_dropshadow );
}
