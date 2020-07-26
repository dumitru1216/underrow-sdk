#include "gui.h"
#include <cstddef>

IDirect3DStateBlock9* state_block;
ImFont* Main;
ImFont* MainCaps;

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

			ImGui::SetNextWindowSize( ImVec2( 600, 455 ), ImGuiSetCond_FirstUseEver );
			ImGui::Begin( "angeldust", &opened, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_ShowBorders | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar ); {
				ImColor c = ImColor( variable.Config.clr_menu[ 0 ], variable.Config.clr_menu[ 1 ], variable.Config.clr_menu[ 2 ] );
				
				/* push font */
				ImGui::PushFont( Main );

				ImGui::BeginChild( "###tabs", ImVec2( 480, 30 ), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse ); {

				}
				ImGui::EndChild( );
				ImGui::SameLine( );
				ImGui::BeginChild( "###name", ImVec2( 0, 30 ), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse ); {
					ImGui::TextColored( c, "under" );
				}
				ImGui::EndChild( );

				/* pop font */
				ImGui::PopFont( );
			}
			ImGui::End( );
		}
	}

}