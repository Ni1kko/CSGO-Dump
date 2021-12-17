#include "slider.hpp"

float map_number( float value, float min_old, float max_old, float min_new, float max_new ) {
	return ( value - min_old ) / ( max_old - min_old ) * ( max_new - min_new ) + min_new;
}

c_slider::c_slider( const std::string &label, const std::string &var_name, float min_value, float max_value, bool round_number, const std::wstring &display_format ) {
	this->label = std::move( label );
	this->var_name = std::move( var_name );
	this->min_value = min_value;
	this->max_value = max_value;
	this->round = round_number;
	this->display_format = std::move( display_format );
	this->size = Vector2( 180, 14 );
}

c_slider::c_slider( const std::string &label, const std::string &var_name, float min_value, float max_value, bool round_number, const std::string &display_format ) :
	c_slider( std::move( label ), std::move( var_name ), min_value, max_value, round_number, std::wstring( display_format.begin( ), display_format.end( ) ) ) { }

void c_slider::think( ) {
	Vector2 draw_position = this->get_parent( )->get_child_draw_position( ) + this->position + Vector2( 16, 15 );
	Vector2 slider_size( ( this->get_parent( )->get_size( ) - Vector2( 32, 0 ) ).x - 31.0f + 8 - 30, 5.0f );

	if ( menu->is_hovered( draw_position, draw_position + slider_size ) && menu->is_key_pressed( VK_LBUTTON ) )
		menu->block( this );

	if ( menu->is_blocking( this ) && menu->is_key_down( VK_LBUTTON ) ) {
		float step = slider_size.x / ( this->max_value - this->min_value );
		float offset = max( this->min_value, min( ( menu->get_mouse_pos( ).x - draw_position.x ), slider_size.x ) );

		float new_value = map_number( offset, 0, slider_size.x, this->min_value, this->max_value );

		if ( this->round )
			new_value = std::roundf( new_value );

		config->set( var_name, new_value );
	}

	if ( menu->is_blocking( this ) && !menu->is_key_down( VK_LBUTTON ) )
		menu->block( nullptr );

	if ( config->get_float( this->var_name ) < this->min_value )
		config->set( var_name, this->min_value );
}

void c_slider::draw( ) {
	Vector2 draw_position = this->get_parent( )->get_child_draw_position( ) + this->position;

	float max_width = ( this->get_parent( )->get_size( ) - Vector2( 32, 0 ) ).x - 31.0f + 8 - 30;
	float slider_width = map_number( config->get_float( this->var_name, this->min_value ), this->min_value, this->max_value, 0, max_width );

	g_renderer_d3d->rect( draw_position.x + 15, draw_position.y + 14, max_width + 2, 7, Color( 17, 17, 17 ) );

	Color colour = Color(
		config->get_color( "clr_menu" ).r(),
		config->get_color( "clr_menu" ).g(),
		config->get_color( "clr_menu" ).b(),
		config->get_color( "clr_menu" ).a()
	);

	g_renderer_d3d->gradient_v( draw_position.x + 16, draw_position.y + 15, max_width, 5, Color( 44, 44, 44 ),
		Color( 29, 29, 29 ) );
	g_renderer_d3d->gradient_v( draw_position.x + 16, draw_position.y + 15, slider_width, 5, colour,
		Color( colour.r() * 0.44, colour.g() * 0.44, colour.b() * 0.44, colour.a() ) );

	wchar_t buffer[ 64 ];
	swprintf( buffer, 64, this->display_format.c_str( ), config->get_float( this->var_name ) );

	// useless for now
	//if ( config->get_float( this->var_name ) != this->max_value )
	//	render::get( ).draw_text( draw_position.x + this->get_size( ).x - 35, draw_position.y + this->get_size( ).y - 1, render::get( ).menu_font_small, "+", false, color( 200, 200, 200 ) );

	//if ( config->get_float( this->var_name ) != this->min_value )
	//	render::get( ).draw_text( draw_position.x + 10, draw_position.y + this->get_size( ).y - 1, render::get( ).menu_font_small, "-", false, color( 200, 200, 200 ) );
	

	std::wstring to_convert( buffer );
	std::string str( to_convert.begin( ), to_convert.end( ) );

	g_renderer_d3d->menu->draw_text( this->label, { draw_position.x + 15, draw_position.y - 3 }, Color( 220, 220, 220 ), font_flags_d3d::font_dropshadow );
	g_renderer_d3d->menu->draw_text( str, { draw_position.x + 15 + slider_width - 2, draw_position.y + 14 }, Color( 220, 220, 220 ), font_flags_d3d::font_dropshadow );


}
