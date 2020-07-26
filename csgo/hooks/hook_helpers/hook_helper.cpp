#include "hook_helper.hpp"
#include "../../utils/globals/globals.hpp"

namespace function {
	helpers c_helpers;

	void helpers::create_fonts( ) {
		/* create fonts */
		csgo::courier_new = csgo_surface->FontCreate( );
		csgo::tahoma = csgo_surface->FontCreate( );

		/* setup font */
		csgo_surface->SetFontGlyphSet( csgo::courier_new, "Courier New", 14, 300, 0, 0, font_flag_outline );
		csgo_surface->SetFontGlyphSet( csgo::tahoma, "Tahoma", 12, 700, 0, 0, font_flag_antialias );

	}
}