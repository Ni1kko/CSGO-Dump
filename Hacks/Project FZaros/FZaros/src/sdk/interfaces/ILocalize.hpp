#pragma once

class ILocalize {
public:
	wchar_t* Find( const char* token_name ) {
		using original_fn = wchar_t*( __thiscall* )( ILocalize*, const char* );
		return ( *( original_fn** )this )[ 11 ]( this, token_name );
	}
};