#include <cctype>
#include <iomanip>
#include <sstream>
#include <windows.h>

#include "Colorpicker.hpp"

struct c_rgb { double r, g, b; }; // a fraction between 0 and 1
struct c_hsv { double h, s, v; }; // a fraction between 0 and 1, but hue is in degrees (0 to 360)

c_hsv rgb_to_hsv( const c_rgb &in ) {
	c_hsv result;
	double min, max, delta;

	min = in.r < in.g ? in.r : in.g;
	min = min < in.b ? min : in.b;

	max = in.r > in.g ? in.r : in.g;
	max = max > in.b ? max : in.b;

	result.v = max;
	delta = max - min;

	if ( delta < 0.0001 ) {
		result.s = 0;
		result.h = 0;

		return result;
	}

	if ( max > 0 ) {
		result.s = ( delta / max );
	}
	else {
		result.s = 0;
		result.h = NAN;

		return result;
	}

	if ( in.r >= max ) {
		result.h = ( in.g - in.b ) / delta;
	}
	else {
		if ( in.g >= max ) {
			result.h = 2 + ( in.b - in.r ) / delta;
		}
		else {
			result.h = 4 + ( in.r - in.g ) / delta;
		}
	}

	result.h *= 60;

	if ( result.h < 0 )
		result.h += 360;

	return result;
}
c_rgb hsv_to_rgb( const c_hsv &in ) {
	c_rgb result;

	double hh, p, q, t, ff;
	long i;

	if ( in.s <= 0 ) {
		result.r = in.v;
		result.g = in.v;
		result.b = in.v;

		return result;
	}

	hh = ( in.h >= 360 ? 0 : in.h ) / 60;
	i = ( long )hh;

	ff = hh - i;

	p = in.v * ( 1 - in.s );
	q = in.v * ( 1 - ( in.s * ff ) );
	t = in.v * ( 1 - ( in.s * ( 1 - ff ) ) );

	switch ( i ) {
	case 0:
		result.r = in.v;
		result.g = t;
		result.b = p;

		break;

	case 1:
		result.r = q;
		result.g = in.v;
		result.b = p;

		break;

	case 2:
		result.r = p;
		result.g = in.v;
		result.b = t;

		break;

	case 3:
		result.r = p;
		result.g = q;
		result.b = in.v;

		break;

	case 4:
		result.r = t;
		result.g = p;
		result.b = in.v;

		break;

	case 5:
	default:
		result.r = in.v;
		result.g = p;
		result.b = q;

		break;

	}

	return result;
}

static Color hue_bar_Colors[ 7 ] = {
	Color( 255, 0, 0, 255 ),
	Color( 255, 255, 0, 255 ),
	Color( 0, 255, 0, 255 ),
	Color( 0, 255, 255, 255 ),
	Color( 0, 0, 255, 255 ),
	Color( 255, 0, 255, 255 ),
	Color( 255, 0, 0, 255 )
};

void c_colorpicker::context_menu_copy( c_colorpicker *self ) {
	Color col = config->get_color( self->get_var_name( ) );
	std::stringstream Color_hex;

	Color_hex << "#";
	Color_hex << std::hex << std::setw( 2 ) << std::setfill( '0' ) << std::uppercase << col.r();
	Color_hex << std::hex << std::setw( 2 ) << std::setfill( '0' ) << std::uppercase << col.g();
	Color_hex << std::hex << std::setw( 2 ) << std::setfill( '0' ) << std::uppercase << col.b();

	if ( self->alpha_bar )
		Color_hex << std::hex << std::setw( 2 ) << std::setfill( '0' ) << std::uppercase << col.a();

	if ( OpenClipboard( nullptr ) ) {
		EmptyClipboard( );

		HGLOBAL clipboard_buffer = GlobalAlloc( GMEM_DDESHARE, Color_hex.str( ).size( ) + 1 );
		char *buffer = ( char* )GlobalLock( clipboard_buffer );
		strcpy( buffer, Color_hex.str( ).c_str( ) );

		GlobalUnlock( clipboard_buffer );
		SetClipboardData( CF_TEXT, clipboard_buffer );
		CloseClipboard( );
	}
}

void c_colorpicker::context_menu_paste( c_colorpicker *self ) {
	if ( OpenClipboard( nullptr ) ) {
		std::string input( ( char* )GetClipboardData( CF_TEXT ) );

		input.erase( input.begin( ), std::find_if( input.begin( ), input.end( ), [ ] ( int ch ) {
			return !std::isspace( ch );
		} ) );
		input.erase( std::find_if( input.rbegin( ), input.rend( ), [ ] ( int ch ) {
			return !std::isspace( ch );
		} ).base( ), input.end( ) );

		if ( input.at( 0 ) != '#' )
			return;

		int component_r = std::stoi( input.substr( 1, 2 ), 0, 16 );
		int component_g = std::stoi( input.substr( 3, 2 ), 0, 16 );
		int component_b = std::stoi( input.substr( 5, 2 ), 0, 16 );
		int component_a = input.size( ) > 7 ? std::stoi( input.substr( 7, 2 ), 0, 16 ) : 255;

		config->set( self->get_var_name( ), Color( component_r, component_g, component_b, component_a ) );

		c_hsv Color_hsv = rgb_to_hsv( {
			( double )( ( float )component_r / 255.f ),
			( double )( ( float )component_g / 255.f ),
			( double )( ( float )component_b / 255.f )
			} );

		self->hue = ( float )Color_hsv.h / 360.f;
		self->cursor_pos.x = ( float )Color_hsv.s * 150.f;
		self->cursor_pos.y = 150.f - ( ( float )Color_hsv.v * 150.f );
		self->alpha = ( float )component_a / 255.f;

		c_colorpicker::context_menu_copy( self );
	}
}

c_colorpicker::c_colorpicker( const std::string &label, const std::string &var_name, bool is_inline, bool alpha_bar ) {
	this->label = std::move( label );
	this->var_name = std::move( var_name );
	this->is_inline = is_inline;
	this->alpha_bar = alpha_bar;

	c_hsv Color_hsv = rgb_to_hsv( {
		( double )( ( float )config->get_color( var_name ).r() / 255.f ),
		( double )( ( float )config->get_color( var_name ).g() / 255.f ),
		( double )( ( float )config->get_color( var_name ).b() / 255.f )
		} );

	this->hue = ( float )Color_hsv.h / 360.f;
	this->cursor_pos.x = ( float )Color_hsv.s * 150.f;
	this->cursor_pos.y = 150.f - ( ( float )Color_hsv.v * 150.f );

	if ( is_inline )
		this->size = Vector2( 0, -16 );
	else
		this->size = Vector2( 150, 27 );

	if ( alpha_bar )
		this->alpha = ( float )config->get_color( var_name ).a() / 255.f;
	else
		this->alpha = 1.0f;

	this->context_menu_actions.push_back( { "copy", &c_colorpicker::context_menu_copy } );
	this->context_menu_actions.push_back( { "paste", &c_colorpicker::context_menu_paste } );
}

void c_colorpicker::think( ) {
	Vector2 draw_position = this->get_parent( )->get_child_draw_position( ) + this->position;
	float group_width = this->get_parent( )->get_size( ).x - 46.f + 8;

	Vector2 picker_pos( draw_position + Vector2( group_width - 21.f, 12.f ) );
	Vector2 picker_size( 174.f, this->alpha_bar ? 173.f : 158.f );

	if ( menu->is_hovered( draw_position + Vector2( group_width - 20, 0 ), draw_position + Vector2( group_width, 8 ) ) && !menu->is_blocking( ) &&
		!menu->is_blocking( this ) && !this->context_menu_open && menu->is_key_pressed( VK_LBUTTON ) )
	{
		menu->block( this );
	}
	else if ( this->context_menu_open ) {
		if ( !menu->is_hovered( context_menu_pos, context_menu_pos + Vector2( 100.f, ( float )context_menu_actions.size( ) * 18.f ) ) &&
			menu->is_blocking( this ) && menu->is_key_pressed( VK_LBUTTON ) )
		{
			this->context_menu_open = false;
			menu->block( nullptr );
		}
	}
	else if ( !menu->is_hovered( picker_pos, picker_pos + picker_size ) && menu->is_blocking( this ) && menu->is_key_pressed( VK_LBUTTON ) ) {
		this->context_menu_open = false;
		menu->block( nullptr );
	}

	if ( menu->is_blocking( this ) && !context_menu_open && !changing_alpha && !changing_color && !changing_hue ) {
		if ( menu->is_key_down( VK_CONTROL ) && menu->is_key_pressed( 'C' ) )
			c_colorpicker::context_menu_copy( this );
		else if ( menu->is_key_down( VK_CONTROL ) && menu->is_key_pressed( 'V' ) )
			c_colorpicker::context_menu_paste( this );
	}

	if ( menu->is_hovered( picker_pos, picker_pos + picker_size ) && menu->is_blocking( this ) && menu->is_key_pressed( VK_LBUTTON ) && !this->context_menu_open ) {
		if ( !this->changing_alpha && !this->changing_hue && !this->changing_color ) {
			if ( menu->is_hovered( picker_pos + Vector2( 2, 2 ), picker_pos + Vector2( 152, 152 ) ) ) {
				this->changing_color = true;
			}
			else if ( menu->is_hovered( picker_pos + Vector2( 161.f, 6.f ), picker_pos + Vector2( 172.f, 156.f ) ) ) {
				this->changing_hue = true;
			}
			else if ( menu->is_hovered( picker_pos + Vector2( 6.f, 160.f ), picker_pos + Vector2( 156.f, 171.f ) ) && this->alpha_bar ) {
				this->changing_alpha = true;
			}
		}
	}
	else if ( this->context_menu_open ) {
		if ( menu->is_hovered( context_menu_pos, context_menu_pos + Vector2( 100.f, context_menu_actions.size( ) * 18.f ) ) &&
			menu->is_blocking( this ) && menu->is_key_pressed( VK_LBUTTON ) )
		{
			for ( int i = 0; i < ( int )context_menu_actions.size( ); i++ ) {
				Vector2 context_menu_action_pos_min( context_menu_pos + Vector2( 1.f, 1.f + ( 18.f * ( float )i ) ) );
				Vector2 context_menu_action_pos_max( context_menu_action_pos_min + Vector2( 98.f, 17.f ) );

				if ( menu->is_hovered( context_menu_action_pos_min, context_menu_action_pos_max ) ) {
					context_menu_actions[ i ].second( this );

					this->context_menu_open = false;
					menu->block( nullptr );
				}
			}
		}
	}

	if ( menu->is_hovered( draw_position + Vector2( group_width - 20, 0 ), draw_position + Vector2( group_width, 8.f ) ) && !menu->is_blocking( ) &&
		!menu->is_blocking( this ) && !this->context_menu_open && menu->is_key_pressed( VK_RBUTTON ) )
	{
		this->context_menu_open = true;
		this->context_menu_pos = picker_pos + Vector2( -100 + 21, 2 );

		menu->block( this );
	}

	if ( menu->is_blocking( this ) && !this->context_menu_open && ( this->changing_alpha || this->changing_color || this->changing_hue ) ) {
		if ( !menu->is_key_down( VK_LBUTTON ) ) {
			this->changing_alpha = false;
			this->changing_color = false;
			this->changing_hue = false;
		}
		else {
			if ( this->alpha_bar && this->changing_alpha ) {
				this->alpha = ( menu->get_mouse_pos( ).x - ( picker_pos.x + 6.f ) ) / 150.f;

				if ( this->alpha < 0 ) this->alpha = 0;
				if ( this->alpha > 1 ) this->alpha = 1;
			}
			else if ( this->changing_color ) {
				this->cursor_pos.x = menu->get_mouse_pos( ).x - ( picker_pos.x + 6.f );
				this->cursor_pos.y = menu->get_mouse_pos( ).y - ( picker_pos.y + 6.f );

				if ( this->cursor_pos.x < 0.f ) this->cursor_pos.x = 0.f;
				if ( this->cursor_pos.x > 150.f ) this->cursor_pos.x = 150.f;

				if ( this->cursor_pos.y < 0.f ) this->cursor_pos.y = 0.f;
				if ( this->cursor_pos.y > 150.f ) this->cursor_pos.y = 150.f;
			}
			else if ( this->changing_hue ) {
				this->hue = ( menu->get_mouse_pos( ).y - ( picker_pos.y + 6.f ) ) / 150.f;

				if ( this->hue < 0.f ) this->hue = 0.f;
				if ( this->hue > 1.f ) this->hue = 1.f;
			}

			float saturation = this->cursor_pos.x / 150.f;
			float brightness = 1.f - ( this->cursor_pos.y / 150.f );

			c_rgb new_Color = hsv_to_rgb( { this->hue * 360.f, saturation, brightness } );

			config->set( this->var_name, Color(
				( int )( new_Color.r * 255.f ),
				( int )( new_Color.g * 255.f ),
				( int )( new_Color.b * 255.f ),
				( int )( this->alpha * 255.f )
			) );
		}
	}
}

void c_colorpicker::draw( ) {
	Vector2 draw_position = this->get_parent( )->get_child_draw_position( ) + this->position;
	float group_width = this->get_parent( )->get_size( ).x - 46.f + 8;

	Vector2 picker_pos( draw_position + Vector2( group_width - 21.f, 12.f ) );
	Vector2 picker_size( 174.f, this->alpha_bar ? 173.f : 158.f );
	Color preview = Color(
		config->get_color( this->var_name ).r(),
		config->get_color( this->var_name ).g(),
		config->get_color( this->var_name ).b(),
		255
	);

	g_renderer_d3d->rect_fill( ( int )draw_position.x + ( int )group_width - 20, ( int )draw_position.y, 20, 10, Color( 17, 17, 17 ) );
	g_renderer_d3d->gradient_v( int( draw_position.x + group_width ) - 19, ( int )draw_position.y + 1, 18, 8, preview,
		Color( int( preview.r() * .65f ), int( preview.g() * .65f ), int( preview.b() * .65f ), preview.a() ) );

	if ( !this->is_inline )
		g_renderer_d3d->menu->draw_text( this->label, { ( float )draw_position.x + 15, ( float )draw_position.y - 3 }, Color( 220, 220, 220 ),
			font_flags_d3d::font_dropshadow );

	if ( menu->is_blocking( this ) && !this->context_menu_open ) {
		Color picked_Color = config->get_color( this->var_name );
		c_rgb hue_Color = hsv_to_rgb( { ( double )this->hue * 360.f, 1.f, 1.f } );

		g_renderer_d3d->rect( ( int )picker_pos.x, ( int )picker_pos.y, ( int )picker_size.x + 4, ( int )picker_size.y + 4, Color( 17, 17, 17 ) );
		g_renderer_d3d->rect( ( int )picker_pos.x + 1, ( int )picker_pos.y + 1, ( int )picker_size.x + 2, ( int )picker_size.y + 2, Color( 60, 60, 60 ) );
		g_renderer_d3d->rect_fill( ( int )picker_pos.x + 2, ( int )picker_pos.y + 2, ( int )picker_size.x, ( int )picker_size.y, Color( 44, 44, 44 ) );

		g_renderer_d3d->rect( ( int )picker_pos.x + 5, ( int )picker_pos.y + 5, 152, 152, Color( 0, 0, 0 ) );
		g_renderer_d3d->gradient_h( ( int )picker_pos.x + 6, ( int )picker_pos.y + 6, 150, 150, Color( 255, 255, 255 ), Color(
			( int )( hue_Color.r * 255.0f ),
			( int )( hue_Color.g * 255.0f ),
			( int )( hue_Color.b * 255.0f ) ) );

		g_renderer_d3d->gradient_v( ( int )picker_pos.x + 6, ( int )picker_pos.y + 6, 150, 150, Color( 0, 0, 0, 0 ), Color( 0, 0, 0 ) );
		g_renderer_d3d->rect( ( int )picker_pos.x + 160, ( int )picker_pos.y + 5, 13, 152, Color( 0, 0, 0 ) );

		g_renderer_d3d->gradient_v( ( int )picker_pos.x + 161, ( int )picker_pos.y + 6 + ( 25 * 0 ), 11, 25, hue_bar_Colors[ 0 ], hue_bar_Colors[ 1 ] );
		g_renderer_d3d->gradient_v( ( int )picker_pos.x + 161, ( int )picker_pos.y + 6 + ( 25 * 1 ), 11, 25, hue_bar_Colors[ 1 ], hue_bar_Colors[ 2 ] );
		g_renderer_d3d->gradient_v( ( int )picker_pos.x + 161, ( int )picker_pos.y + 6 + ( 25 * 2 ), 11, 25, hue_bar_Colors[ 2 ], hue_bar_Colors[ 3 ] );
		g_renderer_d3d->gradient_v( ( int )picker_pos.x + 161, ( int )picker_pos.y + 6 + ( 25 * 3 ), 11, 25, hue_bar_Colors[ 3 ], hue_bar_Colors[ 4 ] );
		g_renderer_d3d->gradient_v( ( int )picker_pos.x + 161, ( int )picker_pos.y + 6 + ( 25 * 4 ), 11, 25, hue_bar_Colors[ 4 ], hue_bar_Colors[ 5 ] );
		g_renderer_d3d->gradient_v( ( int )picker_pos.x + 161, ( int )picker_pos.y + 6 + ( 25 * 5 ), 11, 25, hue_bar_Colors[ 5 ], hue_bar_Colors[ 6 ] );

		g_renderer_d3d->rect( ( int )picker_pos.x + 5 + int( ( 148.f * ( this->cursor_pos.x / 150.0f ) ) ),
			( int )picker_pos.y + 5 + int( ( 148.f * ( this->cursor_pos.y / 150.f ) ) ), 4, 4, Color( 0, 0, 0 ) );

		if ( this->alpha_bar ) {
			g_renderer_d3d->rect( ( int )picker_pos.x + 5, ( int )picker_pos.y + 159, 152, 13, Color( 0, 0, 0 ) );
			g_renderer_d3d->rect_fill( ( int )picker_pos.x + 6, ( int )picker_pos.y + 160, 150, 11, Color( picked_Color.r(), picked_Color.g(), picked_Color.b(), picked_Color.a() ) );

			g_renderer_d3d->rect( ( int )picker_pos.x + 6 + int( ( 146.f * this->alpha ) ), ( int )picker_pos.y + 160, 4, 11, Color( 0, 0, 0 ) );
		}

		g_renderer_d3d->rect( ( int )picker_pos.x + 161, int( picker_pos.y + 6 + ( 146.f * this->hue ) ), 11, 4, Color( 0, 0, 0 ) );
	}
	else if ( menu->is_blocking( this ) && this->context_menu_open ) {
		g_renderer_d3d->rect( ( int )context_menu_pos.x, ( int )context_menu_pos.y, 100, 20 * context_menu_actions.size( ), Color( 17, 17, 17 ) );
		g_renderer_d3d->rect_fill( int( context_menu_pos.x ) + 1, int( context_menu_pos.y ) + 1, 98, ( 18 * context_menu_actions.size( ) ) - 2, Color( 40, 40, 40 ) );

		for ( int i = 0; i < ( int )context_menu_actions.size( ); i++ ) {
			Vector2 context_menu_action_pos_min( context_menu_pos + Vector2( 1.f, 1.f + ( 18.f * ( float )i ) ) );
			Vector2 context_menu_action_pos_max( context_menu_action_pos_min + Vector2( 98, 17 ) );

			bool hovered = menu->is_hovered( context_menu_action_pos_min, context_menu_action_pos_max );

			g_renderer_d3d->rect_fill( ( int )context_menu_action_pos_min.x, ( int )context_menu_action_pos_min.y, 98, 20, hovered ? Color( 32, 32, 32 ) : Color( 40, 40, 40 ) );

			if ( hovered ) {
				g_renderer_d3d->menu_bold->draw_text( context_menu_actions[ i ].first, { context_menu_action_pos_min.x + 5, context_menu_action_pos_min.y + 3 },
					Color( 220, 220, 220 ), font_flags_d3d::font_dropshadow );
			}
			else {
				g_renderer_d3d->menu->draw_text( context_menu_actions[ i ].first, { context_menu_action_pos_min.x + 5, context_menu_action_pos_min.y + 3 },
					{ 220, 220, 220 }, font_flags_d3d::font_dropshadow );

			}
		}
	}
}
