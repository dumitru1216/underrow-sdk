#pragma once
#include <filesystem>

class c_config final
{
public:
	void run( const char* name ) noexcept;
	void load( size_t ) noexcept;
	void save( size_t ) const noexcept;
	void add( const char* ) noexcept;
	void remove( size_t ) noexcept;
	void rename( size_t, const char* ) noexcept;
	void reset( ) noexcept;

	constexpr auto& get_configs( ) noexcept {
		return configs;
	}

	struct
	{
		bool enable_aimbot;
		int Hitchance;
		int hitchance_value;
		int min_damage;
		bool Resolver;
		int body_scale;
		int head_scale;
		bool multi_point;
		bool delay_shot;
		int hitscan_type;
		bool IgnoreLimbs;
		bool auto_stop;
		bool FixShotPitch;
		bool PosBacktrack;
		bool ShotBacktrack;

		bool head;
		bool body;
		bool arms;
		bool neck;
		bool legs;


		bool Antiaim;
		bool DesyncAngle;
		bool RandJitterInRange;
		int	JitterRange;
		int	Fakelag;
		bool FakeLagOnPeek;
		bool ChokeShotOnPeek;

		/* esp */
		bool enable_esp;	
		bool dormancy_esp;
		bool name_esp;
		bool weapon_name;
		bool box_esp;	
		bool health_bar;
		bool skeleton_esp;
		bool flags_esp;

		/* world */
		bool world_modulate;
		bool modulate;
		int night_value;
		bool prop;
		bool enabled;
		float prop_color[ 4 ] = { 1.0f, 1.0f, 1.0f, 1.0f };
		float world_color[ 4 ] = { 1.0f, 1.0f, 1.0f, 1.0f };

		bool NoZoom;
		bool NoScope;
		bool NoRecoil;
		int Fov;
		bool Crosshair;

		float clr_menu[ 4 ] = { 1.0f, 0.0f, 0.0f, 1.0f };

		bool Bhop;
		bool AutoStrafe;
		bool LegitBacktrack;
		bool Ak47meme;
		int	Test;
	}Config;



private:
	std::filesystem::path path;
	std::vector<std::string> configs;
};
extern c_config variable;