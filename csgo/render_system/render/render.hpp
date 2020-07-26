#pragma once
#include "../../sdk/other/vector.hpp"
#include <vector>
#include <cstdint>
#include <string_view>
#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

namespace render {
	/* fonts */
	namespace fonts {
		extern ID3DXFont* main_font;
	}

	/* vertex */
	struct vtx_t {
		float x, y, z, rhw;
		std::uint32_t color;
	};

	/* custom vertex */
	struct custom_vtx_t {
		float x, y, z, rhw;
		std::uint32_t color;
		float tu, tv;
	};

	/* some vars */
	struct pos {
		int x, y;
	};

	struct dim {
		int w, h;
	};

	struct rect {
		int x, y, w, h;
	};

	/* font { render } */
	void create_font( void** font, const std::string& family, int size, bool bold );
	void create_font( void** font, const std::string& family, int size, int width );
	void text_size( void* font, const std::string& text, dim& dimentions );
	void text( int x, int y, std::uint32_t color, void* font, const std::string& text );
	void create_fonts( );
	void destroy_fonts( );

	/* draw list */
	void filled_rect( int x, int y, int width, int height, std::uint32_t color );
	void gradient_rect( int x, int y, int width, int height, std::uint32_t color1, std::uint32_t color2, bool is_horizontal );
	void rect( int x, int y, int width, int height, std::uint32_t color );
	void line( int x, int y, int x2, int y2, std::uint32_t color );
	void circle( int x, int y, int radius, int segments, std::uint32_t color );
	
	/* texture scalling */
	void texture( unsigned char* data, int x, int y, int width, int height, float scale );
	void clip_rect( int x, int y, int width, int height );

	/* key pressed */
	bool key_pressed( const std::uint32_t key );

	/* screen size */
	void screen_size( int& width, int& height );

	/* mouse pos */
	void mouse_pos( pos& position );
}