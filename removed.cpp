ImGui::PushFont( MainCaps );
ImGui::Dummy( ImVec2( 250, 0 ) ); ImGui::SameLine( );
ImGui::SetCursorPosY( ImGui::GetCursorPosY( ) + 7 );
ImGui::TextColored( ImVec4( variable.Config.clr_menu[ 0 ], variable.Config.clr_menu[ 1 ], variable.Config.clr_menu[ 2 ], 1.f ), "underrow beta" );
ImGui::PopFont( );

ImGui::PushFont( Main );
ImGui::Dummy( ImVec2( 0, -2 ) ); ImGui::SameLine( );
ImGui::Dummy( ImVec2( 0, 0 ) ); ImGui::SameLine( );

ImGui::BeginChild( "tabs", ImVec2( 568, 30 ), true, ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar );
{
	if ( ImGui::ButtonT( "legit", ImVec2( 568 / 5, -1 ), page, 0, c, false ) ) page = 0; ImGui::SameLine( 0, 0 );
	if ( ImGui::ButtonT( "rage", ImVec2( 568 / 5, -1 ), page, 1, c, false ) ) page = 1; ImGui::SameLine( 0, 0 );
	if ( ImGui::ButtonT( "visual", ImVec2( 568 / 5, -1 ), page, 2, c, false ) ) page = 2; ImGui::SameLine( 0, 0 );
	if ( ImGui::ButtonT( "other", ImVec2( 568 / 5, -1 ), page, 3, c, false ) ) page = 3; ImGui::SameLine( 0, 0 );
	if ( ImGui::ButtonT( "config", ImVec2( 568 / 5, -1 ), page, 4, c, false ) ) page = 4; ImGui::SameLine( 0, 0 );
}
ImGui::EndChild( );
ImGui::PopFont( );

ImGui::PushFont( Menu );

switch ( page ) {

}

ImGui::PopFont( );
