#include "world_esp.hpp"
#include "../../../sdk/interfaces/i_cvar.hpp"
#include "../../../sdk/interfaces/i_material.hpp"

namespace function {
	world_esp c_world_esp;

	void world_esp::setup( ) {
		static bool old_nightmode;
		int old_night_mode_value = variable.Config.night_value;
		float night_amount = variable.Config.night_value / 100.f;

		if ( old_nightmode != variable.Config.world_modulate || old_night_mode_value != variable.Config.night_value ) {
			ConVar* r_DrawSpecificStaticProp = cvar->FindVar( "r_DrawSpecificStaticProp" );
			*( int* )( ( DWORD )&r_DrawSpecificStaticProp->fnChangeCallback + 0xC ) = 0; // ew
			r_DrawSpecificStaticProp->SetValue( 0 );

			for ( auto i = material_system->FirstMaterial( ); i != material_system->InvalidMaterial( ); i = material_system->NextMaterial( i ) )
			{
				IMaterial* pMaterial = material_system->GetMaterial( i );
				if ( !pMaterial )
					continue;

				if ( strstr( pMaterial->GetTextureGroupName( ), "World" ) || strstr( pMaterial->GetTextureGroupName( ), "StaticProp" ) )
				{
					if ( variable.Config.world_modulate )
					{
						if ( strstr( pMaterial->GetTextureGroupName( ), "StaticProp" ) )
							pMaterial->ColorModulate( night_amount, night_amount, night_amount );
						else
							pMaterial->ColorModulate( night_amount, night_amount, night_amount );
					}
					else
						pMaterial->ColorModulate( 1.0f, 1.0f, 1.0f );
				}

				if ( strstr( pMaterial->GetTextureGroupName( ), "SkyBox" ) ) {
					if ( variable.Config.prop ) {
						auto sky_color = variable.Config.world_color;
						pMaterial->ColorModulate( sky_color[ 0 ], sky_color[ 1 ], sky_color[ 2 ] );
					}
				}
			}
			old_nightmode = variable.Config.world_modulate;
		}
	}
}