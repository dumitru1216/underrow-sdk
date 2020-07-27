#include "player_esp.hpp"
#include "../../../sdk/interfaces/c_global_vars_base.hpp"
#include "../../../sdk/interfaces/iv_model_info.hpp"

/* predefine */
namespace function {
	player_esp c_player_esp;

	/* set to string */
	std::string string_upper( std::string strToConvert ) {
		std::transform( strToConvert.begin( ), strToConvert.end( ), strToConvert.begin( ), ::toupper );
		return strToConvert;
	}

	/* fix networked name */
	std::string fix_networked_name( std::string name ) {
		std::string cname = name;

		if ( cname[ 0 ] == 'C' )
			cname.erase( cname.begin( ) );

		auto start_of_weap = cname.find( "Weapon" );
		if ( start_of_weap != std::string::npos )
			cname.erase( cname.begin( ) + start_of_weap, cname.begin( ) + start_of_weap + 6 );

		return cname;
	}

	void player_esp::setup_visuals( C_BaseEntity* entity, float alpha, matrix3x4_t* bone_matrix ) {
		/* variables */
		Vector min, max;
		entity->GetRenderBounds( min, max );
		Vector pos, pos_3d, top, top_3d;
		pos_3d = entity->get_abs_origin( ) - Vector( 0, 0, 10 );
		top_3d = pos_3d + Vector( 0, 0, max.z + 15 );

		/* screen size */
		int screen_x, screen_y;
		csgo_engine->get_screen_size( screen_x, screen_y );

		/* world to screen */
		if ( Utils::world_to_screen( pos_3d, pos ) && Utils::world_to_screen( top_3d, top ) ) {
			/* variables */
			int height = ( pos.y - top.y );
			int width = height / 2;

			/* name esp */
			if ( variable.Config.name_esp ) {
				PlayerInfo_t ent_info;
				if ( !csgo_engine->GetPlayerInfo( entity->EntIndex( ), &ent_info ) ) return;

				std::string name = ent_info.szName;
				csgo_surface->text( pos.x, top.y - 14, color_t( 255, 255, 255, alpha ), csgo::esp_font, true, name.c_str( ) );
			}

			/* box esp */
			if ( variable.Config.box_esp ) {
				csgo_surface->rect( pos.x - width / 2, top.y, width, height, color_t( 255, 255, 255, alpha ) );
				csgo_surface->rect( pos.x - width / 2 + 1, top.y + 1, width - 2, height - 2, color_t( 20, 20, 20, alpha ) );
				csgo_surface->rect( pos.x - width / 2 - 1, top.y - 1, width + 2, height + 2, color_t( 20, 20, 20, alpha ) );
			}

			/* healthbar */
			if ( variable.Config.health_bar ) {
				/* color system */
				int enemy_hp = entity->get_health( );
				int hp_red = 255 - ( enemy_hp * 2.55 );
				int hp_green = enemy_hp * 2.55;
				color_t health_color = color_t( hp_red, hp_green, 1, alpha );

				/* offset */
				float offset = ( height / 4.f ) + 5;
				int hp = height - ( ( height * enemy_hp ) / 100 );

				/* intense math & black bar */
				csgo_surface->line( ( pos.x - width / 2 ) - 6, top.y + hp, ( pos.x - width / 2 ) - 6, top.y + height - 1, health_color );
				csgo_surface->line( ( pos.x - width / 2 ) - 5, top.y + hp, ( pos.x - width / 2 ) - 5, top.y + height - 1, health_color );

				/* text when hp < 100 */
				if ( enemy_hp < 100 )
					csgo_surface->text( ( pos.x - width / 2 ) - 5, top.y + hp - 2, color_t( 255, 255, 255, alpha ), csgo::health_bar_font, true, string_upper( std::to_string( enemy_hp ) ).c_str( ) );

			}

			/* weapon esp */
			if ( variable.Config.weapon_name ) {
				/* get weapon */
				auto weapon = entity->get_active_weapon( );
				if ( !weapon ) return;

				/* weapon name */
				std::string weap_name = weapon->get_name( );

				/* yoy */
				csgo_surface->text( pos.x, pos.y + 1, color_t( 255, 255, 255, alpha ), csgo::health_bar_font, true, string_upper( weap_name ).c_str( ) );
			}

			/* skeleton */
			if ( variable.Config.skeleton_esp ) {
				/* hitboxes */
				static Vector hitbox_w2s[ 19 ];

				/* color */
				color_t color = color_t( 255, 255, 255, alpha );

				/* matrix & setup bones */
				matrix3x4_t matrix[ 128 ];
				entity->SetupBones( matrix, 128, 256, globalvars->curtime );

				/* finish skeleton */
				studiohdr_t* studio_hdr = model_info->GetStudiomodel( entity->GetModel( ) );
				if ( studio_hdr ) {
					for ( int i = 0; i < studio_hdr->numbones; i++ ) {
						mstudiobone_t* bone = studio_hdr->GetBone( i );
						if ( !bone || !( bone->flags & 0x00000100 ) || bone->parent == -1 )
							continue;

						Vector bone_position1; 
						if ( !Utils::world_to_screen( Vector( matrix[ i ][ 0 ][ 3 ], matrix[ i ][ 1 ][ 3 ], matrix[ i ][ 2 ][ 3 ] ), bone_position1 ) )
							continue;

						Vector bone_position2;
						if ( !Utils::world_to_screen( Vector( matrix[ bone->parent ][ 0 ][ 3 ], matrix[ bone->parent ][ 1 ][ 3 ], matrix[ bone->parent ][ 2 ][ 3 ] ), bone_position2 ) )
							continue;

						csgo_surface->line( ( int )bone_position1.x, ( int )bone_position1.y, ( int )bone_position2.x, ( int )bone_position2.y, color_t( 255, 255, 255, alpha ) );
					}
				}
			}

			/* flags */
			if ( variable.Config.flags_esp ) {
				std::vector<std::pair<std::string, color_t>> side_info;

				/* flags info */
				if ( entity->armor_value( ) > 0 )
					side_info.push_back( std::pair<std::string, color_t>( string_upper( entity->get_armor_name( ) ), color_t( 255, 255, 255, alpha ) ) );
			
				if ( entity->is_scoped( ) )
					side_info.push_back( std::pair<std::string, color_t>( "ZOOM", color_t( 52, 165, 207, alpha ) ) );
			
				/* draw flags */
				int i = 0;
				for ( auto Text : side_info ) {
					csgo_surface->text( ( pos.x + width / 2 ) + 3, top.y + i, Text.second, csgo::health_bar_font, false, Text.first.c_str( ) );
					i += 9;
				}
			}		
		}
	}

	/* alpha */
	float render_alpha[ 64 ];
	
	/* render */
	void player_esp::setup( ) {
		/* checks */
		if ( !csgo::m_local || !csgo_engine->is_in_game( ) || !variable.Config.enable_esp )  return;
		
		/* setup */
		for ( int i = 1; i <= globalvars->maxClients; i++ ) {
			/* checks */
			auto entity = entity_list->GetClientEntity( i );
			if ( !entity ) continue;
			if ( !csgo::m_local )  return;
			if ( entity == csgo::m_local )  continue;
			if ( entity->get_team( ) == csgo::m_local->get_team( ) ) continue;

			if ( variable.Config.dormancy_esp ) {
				if ( ( entity->IsDormant( ) || entity->get_health( ) <= 0 ) && render_alpha[ entity->EntIndex( ) ] > 0 )
					render_alpha[ entity->EntIndex( ) ] -= 5;
				else if ( render_alpha[ entity->EntIndex( ) ] < 210 && !( entity->IsDormant( ) || entity->get_health( ) <= 0 ) )
					render_alpha[ entity->EntIndex( ) ] += 5;

				if ( !entity->is_alive( ) || entity->IsDormant( ) && render_alpha[ entity->EntIndex( ) ] <= 0 )
					continue;
			}

			setup_visuals( entity, render_alpha[ entity->EntIndex( ) ] );
		}
	}
}
