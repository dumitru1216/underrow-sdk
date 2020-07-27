#include "gui.h"
#include <cstddef>

IDirect3DStateBlock9* state_block;
ImFont* Main;
ImFont* MainCaps;
bool show_popup = false;
bool save_config = false;
bool load_config = false;
bool reverse = false;
int offset = 0;

namespace function {
	menu c_menu;

	void menu::think( ) {
		ImGui::GetIO( ).MouseDrawCursor = opened;

		static int page = 0;

		if ( opened ) {

			/* style colors */
			ImGui::GetStyle( ).Colors[ ImGuiCol_CheckMark ] = ImVec4( variable.Config.clr_menu[ 0 ], variable.Config.clr_menu[ 1 ], variable.Config.clr_menu[ 2 ], 1.f );
			ImGui::GetStyle( ).Colors[ ImGuiCol_SliderGrab ] = ImVec4( variable.Config.clr_menu[ 0 ], variable.Config.clr_menu[ 1 ], variable.Config.clr_menu[ 2 ], 1.f );
			ImGui::GetStyle( ).Colors[ ImGuiCol_SliderGrabActive ] = ImVec4( variable.Config.clr_menu[ 0 ], variable.Config.clr_menu[ 1 ], variable.Config.clr_menu[ 2 ], 1.f );

			ImGui::SetNextWindowSize( ImVec2( 600, 390 ), ImGuiSetCond_FirstUseEver );
			ImGui::Begin( "angeldust", &opened, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_ShowBorders | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar ); {
				ImColor c = ImColor( variable.Config.clr_menu[ 0 ], variable.Config.clr_menu[ 1 ], variable.Config.clr_menu[ 2 ] );
				
				/* push font */
				ImGui::PushFont( Main );

				ImGui::BeginChild( "###tabs", ImVec2( 462, 30 ), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse ); {
					if ( ImGui::ButtonT( "legit", ImVec2( 0, 0 ), page, 0, c, false ) ) page = 0; ImGui::SameLine( );
					if ( ImGui::ButtonT( "rage", ImVec2( 0, 0 ), page, 1, c, false ) ) page = 1; ImGui::SameLine( );
					if ( ImGui::ButtonT( "visuals", ImVec2( 0, 0 ), page, 2, c, false ) ) page = 2; ImGui::SameLine( );
					if ( ImGui::ButtonT( "misc", ImVec2( 0, 0 ), page, 3, c, false ) ) page = 3; ImGui::SameLine( );
					if ( ImGui::ButtonT( "skins", ImVec2( 0, 0 ), page, 4, c, false ) ) page = 4; ImGui::SameLine( );
					if ( ImGui::ButtonT( "config", ImVec2( 0, 0 ), page, 5, c, false ) ) page = 5;
				}
				ImGui::EndChild( );
				ImGui::SameLine( );
				ImGui::BeginChild( "###name", ImVec2( 0, 30 ), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse ); {
					ImGui::Text( "underrow | beta 0.11" );
				}
				ImGui::EndChild( );

				switch ( page ) {
					case 0: {
						/* doing the 2 childs where will be features */
						ImGui::BeginChild( "###1", ImVec2( 290, 380 / 2 ), true ); {

						}
						ImGui::EndChild( );
						ImGui::SameLine( );
						ImGui::BeginChild( "###2", ImVec2( 0, 380 / 2 ), true ); {

						}
						ImGui::EndChild( );
						ImGui::SetCursorPosY( 380 / 2 + 50 );
						ImGui::BeginChild( "###3", ImVec2( 290, 0 ), true ); {

						}
						ImGui::EndChild( );
						ImGui::SameLine( );
						ImGui::BeginChild( "###4", ImVec2( 0, 0 ), true ); {

						}
						ImGui::EndChild( );

					} break;
					case 1: {
						/* doing the 2 childs where will be features */
						ImGui::BeginChild( "###5", ImVec2( 290, 380 / 2 ), true ); {
							ImGui::Checkbox( "enable aimbot", &variable.Config.enable_aimbot );
							ImGui::Checkbox( "automatic stop", &variable.Config.auto_stop );

							
						}
						ImGui::EndChild( );
						ImGui::SameLine( );
						ImGui::BeginChild( "###6", ImVec2( 0, 380 / 2 ), true ); {
							ImGui::SliderInt( "hitchance", &variable.Config.hitchance_value, 0, 100, "%.f%%" );
							ImGui::SliderInt( "minimum damage", &variable.Config.min_damage, 0, 100, "%.f%%" );
						}
						ImGui::EndChild( );
						ImGui::SetCursorPosY( 380 / 2 + 50 );
						ImGui::BeginChild( "###7", ImVec2( 290, 0 ), true ); {
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
						ImGui::EndChild( );
						ImGui::SameLine( );
						ImGui::BeginChild( "###8", ImVec2( 0, 0 ), true ); {

						}
						ImGui::EndChild( );

					} break;
					case 2: {
						/* doing the 2 childs where will be features */
						ImGui::BeginChild( "###9", ImVec2( 290, 380 / 2 ), true ); {
							ImGui::Checkbox( "enable esp", &variable.Config.enable_esp );
							ImGui::Checkbox( "dormancy esp", &variable.Config.dormancy_esp );
							ImGui::Checkbox( "draw box", &variable.Config.box_esp );
							ImGui::Checkbox( "draw health", &variable.Config.health_bar );
							ImGui::Checkbox( "draw name", &variable.Config.name_esp );
							ImGui::Checkbox( "draw weapon", &variable.Config.weapon_name );
							ImGui::Checkbox( "draw flags", &variable.Config.flags_esp );
							ImGui::Checkbox( "draw skeleton", &variable.Config.skeleton_esp );
						}
						ImGui::EndChild( );
						ImGui::SameLine( );
						ImGui::BeginChild( "###10", ImVec2( 0, 380 / 2 ), true ); {

						}
						ImGui::EndChild( );
						ImGui::SetCursorPosY( 380 / 2 + 50 );
						ImGui::BeginChild( "###11", ImVec2( 290, 0 ), true ); {

						}
						ImGui::EndChild( );
						ImGui::SameLine( );
						ImGui::BeginChild( "###12", ImVec2( 0, 0 ), true ); {

						}
						ImGui::EndChild( );

					} break;
					case 3: {
						/* doing the 2 childs where will be features */
						ImGui::BeginChild( "###13", ImVec2( 290, 380 / 2 ), true ); {

						}
						ImGui::EndChild( );
						ImGui::SameLine( );
						ImGui::BeginChild( "###14", ImVec2( 0, 380 / 2 ), true ); {

						}
						ImGui::EndChild( );
						ImGui::SetCursorPosY( 380 / 2 + 50 );
						ImGui::BeginChild( "###15", ImVec2( 290, 0 ), true ); {

						}
						ImGui::EndChild( );
						ImGui::SameLine( );
						ImGui::BeginChild( "###16", ImVec2( 0, 0 ), true ); {

						}
						ImGui::EndChild( );

					} break;
					case 4: {
						/* doing the 2 childs where will be features */
						ImGui::BeginChild( "###17", ImVec2( 290, 380 / 2 ), true ); {

						}
						ImGui::EndChild( );
						ImGui::SameLine( );
						ImGui::BeginChild( "###18", ImVec2( 0, 380 / 2 ), true ); {

						}
						ImGui::EndChild( );
						ImGui::SetCursorPosY( 380 / 2 + 50 );
						ImGui::BeginChild( "###19", ImVec2( 290, 0 ), true ); {

						}
						ImGui::EndChild( );
						ImGui::SameLine( );
						ImGui::BeginChild( "###20", ImVec2( 0, 0 ), true ); {

						}
						ImGui::EndChild( );					
					} break;
					case 5: {
						/* doing the 2 childs where will be features */
						ImGui::BeginChild( "###21", ImVec2( 285, 0 ), true ); {
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
								strcpy( buffer, config_items[ current_config ].c_str( ) );

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
						ImGui::EndChild( );
					} break;
				}


				/* pop font */
				ImGui::PopFont( );
			}
			ImGui::End( );
		}
	}

}