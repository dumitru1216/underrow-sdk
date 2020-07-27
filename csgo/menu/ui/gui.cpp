#include "gui.h"
#include <cstddef>

IDirect3DStateBlock9* state_block;
ImFont* Main;
ImFont* MainCaps;
bool save_config = false;
bool load_config = false;

namespace function {
	menu c_menu;

	void legit_tab( ) {
		ImGui::Columns( 2, NULL, false );

		ImGui::Dummy( ImVec2( 0, -2 ) ); ImGui::SameLine( );
		ImGui::Dummy( ImVec2( 0, 0 ) ); ImGui::SameLine( );

		ImGui::PushStyleVar( ImGuiStyleVar_WindowPadding, ImVec2( 16, 16 ) );

		ImGui::BeginChild( "main 1", ImVec2( 269, 201 ), true );
		{




		}
		ImGui::EndChild( true );

		ImGui::PopStyleVar( );
		ImGui::Dummy( ImVec2( 0, -2 ) ); ImGui::SameLine( );
		ImGui::Dummy( ImVec2( 0, 0 ) ); ImGui::SameLine( );
		ImGui::PushStyleVar( ImGuiStyleVar_WindowPadding, ImVec2( 16, 16 ) );

		ImGui::BeginChild( "main 2", ImVec2( 269, 152 ), true );
		{
		}
		ImGui::EndChild( true );

		ImGui::NextColumn( );

		ImGui::SetCursorPosX( ImGui::GetCursorPosX( ) - 9 );

		ImGui::BeginChild( "main 3", ImVec2( 289, 152 ), true );
		{
		}
		ImGui::EndChild( true );

		ImGui::Spacing( );

		ImGui::SetCursorPosX( ImGui::GetCursorPosX( ) - 9 );
		ImGui::SetCursorPosY( ImGui::GetCursorPosY( ) - 7 );

		ImGui::BeginChild( "main 4", ImVec2( 289, 201 ), true );
		{
		}
		ImGui::EndChild( true );

		ImGui::PopStyleVar( );
		ImGui::Columns( );
	}

	void rage_tab( ) {
		ImGui::Columns( 2, NULL, false );

		ImGui::Dummy( ImVec2( 0, -2 ) ); ImGui::SameLine( );
		ImGui::Dummy( ImVec2( 0, 0 ) ); ImGui::SameLine( );

		ImGui::PushStyleVar( ImGuiStyleVar_WindowPadding, ImVec2( 16, 16 ) );

		ImGui::BeginChild( "ragebot [ main ]", ImVec2( 269, 201 ), true );
		{
			ImGui::Checkbox( "enable aimbot", &variable.Config.enable_aimbot );
			ImGui::Checkbox( "automatic stop", &variable.Config.auto_stop );
		}
		ImGui::EndChild( true );

		ImGui::PopStyleVar( );
		ImGui::Dummy( ImVec2( 0, -2 ) ); ImGui::SameLine( );
		ImGui::Dummy( ImVec2( 0, 0 ) ); ImGui::SameLine( );
		ImGui::PushStyleVar( ImGuiStyleVar_WindowPadding, ImVec2( 16, 16 ) );
		ImGui::BeginChild( "ragebot [ accuracy ]", ImVec2( 269, 152 ), true );
		{
			ImGui::SliderInt( "hitchance", &variable.Config.hitchance_value, 0, 100, "%.f%%" );
			ImGui::SliderInt( "minimum damage", &variable.Config.min_damage, 0, 100, "%.f%%" );

		}
		ImGui::EndChild( true );

		ImGui::NextColumn( );

		ImGui::SetCursorPosX( ImGui::GetCursorPosX( ) - 9 );

		ImGui::BeginChild( "ragebot [ multipoint ]", ImVec2( 289, 152 ), true ); {
			const char* hitscan_type[ ] = { "head only", "body only", "full hitscan" };
			ImGui::Combo( "hitscan mode", &variable.Config.hitscan_type, hitscan_type, IM_ARRAYSIZE( hitscan_type ) );
			if ( variable.Config.hitscan_type == 2 ) {
				ImGui::Checkbox( "multipoint", &variable.Config.multi_point );
				if ( variable.Config.multi_point ) {
					ImGui::Checkbox( "head", &variable.Config.head );
					ImGui::SameLine( );
					ImGui::Checkbox( "body", &variable.Config.body );
					ImGui::SameLine( );
					ImGui::Checkbox( "neck", &variable.Config.neck );
					ImGui::Checkbox( "legs", &variable.Config.legs );
					ImGui::SameLine( );
					ImGui::Checkbox( "arms", &variable.Config.arms );
				}
				ImGui::TextColored( ImColor( 255, 0, 0, 255 ), "multipoint scale" );
				ImGui::SliderInt( "head scale", &variable.Config.head_scale, 0, 100, "%.f%%" );
				ImGui::SliderInt( "body scale", &variable.Config.body_scale, 0, 100, "%.f%%" );
			}
		}
		ImGui::EndChild( true );

		ImGui::Spacing( );

		ImGui::SetCursorPosX( ImGui::GetCursorPosX( ) - 9 );
		ImGui::SetCursorPosY( ImGui::GetCursorPosY( ) - 7 );

		ImGui::BeginChild( "ragebot [ anti-aim ]", ImVec2( 289, 201 ), true ); {
			
		}
		ImGui::EndChild( true );

		ImGui::PopStyleVar( );
		ImGui::Columns( );
	}

	void visuals_tab( ) {
		ImGui::Columns( 2, NULL, false );

		ImGui::Dummy( ImVec2( 0, -2 ) ); ImGui::SameLine( );
		ImGui::Dummy( ImVec2( 0, 0 ) ); ImGui::SameLine( );

		ImGui::PushStyleVar( ImGuiStyleVar_WindowPadding, ImVec2( 16, 16 ) );

		ImGui::BeginChild( "visuals [ players ]", ImVec2( 269, 201 ), true ); {
			ImGui::Checkbox( "enable esp", &variable.Config.enable_esp );
			ImGui::Checkbox( "dormancy esp", &variable.Config.dormancy_esp );
			ImGui::Checkbox( "draw box", &variable.Config.box_esp );
			ImGui::Checkbox( "draw health", &variable.Config.health_bar );
			ImGui::Checkbox( "draw name", &variable.Config.name_esp );
			ImGui::Checkbox( "draw weapon", &variable.Config.weapon_name );
			ImGui::Checkbox( "draw flags", &variable.Config.flags_esp );
			ImGui::Checkbox( "draw skeleton", &variable.Config.skeleton_esp );		
		}
		ImGui::EndChild( true );

		ImGui::PopStyleVar( );
		ImGui::Dummy( ImVec2( 0, -2 ) ); ImGui::SameLine( );
		ImGui::Dummy( ImVec2( 0, 0 ) ); ImGui::SameLine( );
		ImGui::PushStyleVar( ImGuiStyleVar_WindowPadding, ImVec2( 16, 16 ) );

		ImGui::BeginChild( "visuals [ chams ]", ImVec2( 269, 152 ), true );
		{


		}
		ImGui::EndChild( true );

		ImGui::NextColumn( );

		ImGui::SetCursorPosX( ImGui::GetCursorPosX( ) - 9 );

		ImGui::BeginChild( "visuals [ other ]", ImVec2( 289, 152 ), true );
		{

		}
		ImGui::EndChild( true );

		ImGui::Spacing( );

		ImGui::SetCursorPosX( ImGui::GetCursorPosX( ) - 9 );
		ImGui::SetCursorPosY( ImGui::GetCursorPosY( ) - 7 );

		ImGui::BeginChild( "visuals [ world ]", ImVec2( 289, 201 ), true );
		{
			ImGui::Checkbox( "wrold modulate", &variable.Config.world_modulate );
			if ( variable.Config.world_modulate ) {
				ImGui::SliderInt( "night value", &variable.Config.night_value, 0, 100, "%.f%%" );
			}
			ImGui::Checkbox( "skybox modulate", &variable.Config.prop );
			ImGui::SameLine( );
			ImGui::ColorEdit4( "###or", variable.Config.world_color, ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_NoInputs );
		}
		ImGui::EndChild( true );

		ImGui::PopStyleVar( );
		ImGui::Columns( );
	}

	void misc_tab( ) {
		ImGui::Columns( 2, NULL, false );

		ImGui::Dummy( ImVec2( 0, -2 ) ); ImGui::SameLine( );
		ImGui::Dummy( ImVec2( 0, 0 ) ); ImGui::SameLine( );

		ImGui::PushStyleVar( ImGuiStyleVar_WindowPadding, ImVec2( 16, 16 ) );

		ImGui::BeginChild( "misc [ main ]", ImVec2( 269, 361 ), true );
		{

		}
		ImGui::EndChild( true );
		ImGui::NextColumn( );
		ImGui::SetCursorPosX( ImGui::GetCursorPosX( ) - 9 );

		ImGui::BeginChild( "misc [ other ]", ImVec2( 289, 152 ), true );
		{
			ImGui::Text( "menu color" );
			ImGui::SameLine( );
			ImGui::ColorEdit4( "###mmen0", variable.Config.clr_menu, ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_NoInputs );
			
		}
		ImGui::EndChild( true );

		ImGui::Spacing( );

		ImGui::SetCursorPosX( ImGui::GetCursorPosX( ) - 9 );
		ImGui::SetCursorPosY( ImGui::GetCursorPosY( ) - 7 );

		ImGui::BeginChild( "misc [ premium ]", ImVec2( 289, 201 ), true );
		{
			
		}
		ImGui::EndChild( true );

		ImGui::PopStyleVar( );
		ImGui::Columns( );
	}

	void config_tab( ) {
		ImGui::Columns( 2, NULL, false );

		ImGui::Dummy( ImVec2( 0, -2 ) ); ImGui::SameLine( );
		ImGui::Dummy( ImVec2( 0, 0 ) ); ImGui::SameLine( );

		ImGui::PushStyleVar( ImGuiStyleVar_WindowPadding, ImVec2( 16, 16 ) );

		ImGui::BeginChild( "configs [ configs ]", ImVec2( 269, 361 ), true );
		{
			constexpr auto& config_items = variable.get_configs( );
			static int current_config = -1;

			if ( static_cast< size_t >( current_config ) >= config_items.size( ) )
				current_config = -1;

			static char buffer[ 16 ];

			if ( ImGui::ListBox( "", &current_config, [ ]( void* data, int idx, const char** out_text ) {
				auto& vector = *static_cast< std::vector<std::string>* >( data );
				*out_text = vector[ idx ].c_str( );
				return true;
				}, &config_items, config_items.size( ), 5 ) && current_config != -1 )
				strcpy_s( buffer, config_items[ current_config ].c_str( ) );

				ImGui::SetCursorPosX( ImGui::GetCursorPosX( ) + 18 );
				ImGui::PushID( 0 );
				ImGui::PushItemWidth( 178 );
				if ( ImGui::InputText( "", buffer, IM_ARRAYSIZE( buffer ), ImGuiInputTextFlags_EnterReturnsTrue ) ) {
					if ( current_config != -1 )
						variable.rename( current_config, buffer );
				}
				ImGui::PopID( );
				ImGui::NextColumn( );

				ImGui::SetCursorPosX( ImGui::GetCursorPosX( ) + 18 );
				if ( ImGui::Button( ( "create" ), ImVec2( 85, 20 ) ) ) {
					variable.add( buffer );
				}

				ImGui::SameLine( );

				if ( ImGui::Button( ( "reset" ), ImVec2( 85, 20 ) ) ) {
					variable.reset( );
				}

				ImGui::SetCursorPosX( ImGui::GetCursorPosX( ) + 18 );
				if ( current_config != -1 ) {
					if ( ImGui::Button( ( "load" ), ImVec2( 85, 20 ) ) ) {
						variable.load( current_config );

						load_config = true;

					}

					ImGui::SameLine( );

					if ( ImGui::Button( ( "save" ), ImVec2( 85, 20 ) ) ) {
						variable.save( current_config );

						save_config = true;

					}

					ImGui::SetCursorPosX( ImGui::GetCursorPosX( ) + 18 );
					if ( ImGui::Button( ( "remove" ), ImVec2( 85, 20 ) ) ) {
						variable.remove( current_config );
					}
				}
		}
		ImGui::EndChild( true );
		ImGui::NextColumn( );
		ImGui::SetCursorPosX( ImGui::GetCursorPosX( ) - 9 );

		ImGui::BeginChild( "config [ lua -> presets ]", ImVec2( 289, 152 ), true );
		{

		}
		ImGui::EndChild( true );

		ImGui::Spacing( );

		ImGui::SetCursorPosX( ImGui::GetCursorPosX( ) - 9 );
		ImGui::SetCursorPosY( ImGui::GetCursorPosY( ) - 7 );

		ImGui::BeginChild( "config [ lua -> custom ]", ImVec2( 289, 201 ), true );
		{

		}
		ImGui::EndChild( true );

		ImGui::PopStyleVar( );
		ImGui::Columns( );
	}


	void menu::think( )
	{
		ImGui::GetIO( ).MouseDrawCursor = opened;

		static int page = 0;

		if ( opened ) {



			ImGui::GetStyle( ).Colors[ ImGuiCol_CheckMark ] = ImVec4( variable.Config.clr_menu[ 0 ], variable.Config.clr_menu[ 1 ], variable.Config.clr_menu[ 2 ], 1.f );
			ImGui::GetStyle( ).Colors[ ImGuiCol_SliderGrab ] = ImVec4( variable.Config.clr_menu[ 0 ], variable.Config.clr_menu[ 1 ], variable.Config.clr_menu[ 2 ], 1.f );
			ImGui::GetStyle( ).Colors[ ImGuiCol_SliderGrabActive ] = ImVec4( variable.Config.clr_menu[ 0 ], variable.Config.clr_menu[ 1 ], variable.Config.clr_menu[ 2 ], 1.f );

			ImGui::SetNextWindowSize( ImVec2( 600, 455 ), ImGuiSetCond_FirstUseEver );

			ImGui::Begin( "angeldust", &opened, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_ShowBorders | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar ); {
				ImVec2 p = ImGui::GetCursorScreenPos( );
				ImColor c = ImColor( variable.Config.clr_menu[ 0 ], variable.Config.clr_menu[ 1 ], variable.Config.clr_menu[ 2 ] );

				ImGui::GetWindowDrawList( )->AddRectFilled( ImVec2( p.x, p.y + 3 ), ImVec2( p.x + ImGui::GetWindowWidth( ), p.y + -3 ), ImColor( variable.Config.clr_menu[ 0 ], variable.Config.clr_menu[ 1 ], variable.Config.clr_menu[ 2 ] ) );



				ImGui::PushFont( MainCaps );
				ImGui::Dummy( ImVec2( 250, 0 ) ); ImGui::SameLine( );
				ImGui::SetCursorPosY( ImGui::GetCursorPosY( ) + 7 );
				ImGui::TextColored( ImVec4( variable.Config.clr_menu[ 0 ], variable.Config.clr_menu[ 1 ], variable.Config.clr_menu[ 2 ], 1.f ), "ca$hbot.xyz" );
				ImGui::PopFont( );

				ImGui::PushFont( Main );
				ImGui::Dummy( ImVec2( 0, -2 ) ); ImGui::SameLine( );
				ImGui::Dummy( ImVec2( 0, 0 ) ); ImGui::SameLine( );

				ImGui::BeginChild( "tabs", ImVec2( 568, 30 ), true, ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar );
				{
					if ( ImGui::ButtonT( "ragebot", ImVec2( 95, -1 ), page, 0, c, false ) ) page = 0; ImGui::SameLine( 0, 0 );
					if ( ImGui::ButtonT( "legitbot", ImVec2( 95, -1 ), page, 1, c, false ) ) page = 1; ImGui::SameLine( 0, 0 );
					if ( ImGui::ButtonT( "visuals", ImVec2( 95, -1 ), page, 2, c, false ) ) page = 2; ImGui::SameLine( 0, 0 );
					if ( ImGui::ButtonT( "misc", ImVec2( 95, -1 ), page, 3, c, false ) ) page = 3; ImGui::SameLine( 0, 0 );
					if ( ImGui::ButtonT( "config & lua", ImVec2( 95, -1 ), page, 4, c, false ) ) page = 4; ImGui::SameLine( 0, 0 );
					if ( ImGui::ButtonT( "skins", ImVec2( 95, -1 ), page, 5, c, false ) ) page = 5; ImGui::SameLine( 0, 0 );
				}
				ImGui::EndChild( );
				ImGui::PopFont( );

				ImGui::PushFont( Main );

				switch ( page ) {
				case 0: rage_tab( );
					break;
				case 1: legit_tab( );
					break;
				case 2: visuals_tab( );
					break;
				case 3: misc_tab( );
					break;
				case 4: config_tab( );
					break;
				}

				ImGui::PopFont( );
			}
			ImGui::End( );

		}

	}

}