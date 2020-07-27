#include "hook_helper.hpp"
#include "../../utils/globals/globals.hpp"

namespace function {
	helpers c_helpers;

	void helpers::create_fonts( ) {
		/* create fonts */
		csgo::esp_font = csgo_surface->FontCreate( );
		csgo::health_bar_font = csgo_surface->FontCreate( );

		/* setup font */
		csgo_surface->SetFontGlyphSet( csgo::esp_font, "Verdana", 12, 550, 0, 0, font_flag_dropshadow | font_flag_antialias );
		csgo_surface->SetFontGlyphSet( csgo::health_bar_font, "Small Fonts", 8, 400, 0, 0, font_flag_outline );

	}
}