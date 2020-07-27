#include "movement_system.hpp"

/* predefine */
namespace function {
	movement_system c_movement;

	/* vector angle */
	static void angle_vectors( const Vector& angles, Vector& forward, Vector& right, Vector& up )
	{
		const auto sp = sin( DEG2RAD( angles.x ) ), cp = cos( DEG2RAD( angles.x ) ),
			sy = sin( DEG2RAD( angles.y ) ), cy = cos( DEG2RAD( angles.y ) ),
			sr = sin( DEG2RAD( angles.z ) ), cr = cos( DEG2RAD( angles.z ) );

		forward.x = cp * cy;
		forward.y = cp * sy;
		forward.z = -sp;

		right.x = -1 * sr * sp * cy + -1 * cr * -sy;
		right.y = -1 * sr * sp * sy + -1 * cr * cy;
		right.z = -1 * sr * cp;

		up.x = cr * sp * cy + -sr * -sy;
		up.y = cr * sp * sy + -sr * cy;
		up.z = cr * cp;
	}

	/* setup movement */
	void movement_system::setup_movement( CUserCmd* cmd, Vector& wishangle ) {
		/* fix angles */
		Vector view_fwd, view_right, view_up, cmd_fwd, cmd_right, cmd_up;
		angle_vectors( wishangle, view_fwd, view_right, view_up );
		angle_vectors( cmd->viewangles, cmd_fwd, cmd_right, cmd_up );

		/* return angles */
		const auto v8 = sqrtf( ( view_fwd.x * view_fwd.x ) + ( view_fwd.y * view_fwd.y ) );
		const auto v10 = sqrtf( ( view_right.x * view_right.x ) + ( view_right.y * view_right.y ) );
		const auto v12 = sqrtf( view_up.z * view_up.z );

		/* normalize angles */
		const Vector norm_view_fwd( ( 1.f / v8 ) * view_fwd.x, ( 1.f / v8 ) * view_fwd.y, 0.f );
		const Vector norm_view_right( ( 1.f / v10 ) * view_right.x, ( 1.f / v10 ) * view_right.y, 0.f );
		const Vector norm_view_up( 0.f, 0.f, ( 1.f / v12 ) * view_up.z );

		/* return angles */
		const auto v14 = sqrtf( ( cmd_fwd.x * cmd_fwd.x ) + ( cmd_fwd.y * cmd_fwd.y ) );
		const auto v16 = sqrtf( ( cmd_right.x * cmd_right.x ) + ( cmd_right.y * cmd_right.y ) );
		const auto v18 = sqrtf( cmd_up.z * cmd_up.z );

		/* fix angles */
		const Vector norm_cmd_fwd( ( 1.f / v14 ) * cmd_fwd.x, ( 1.f / v14 ) * cmd_fwd.y, 0.f );
		const Vector norm_cmd_right( ( 1.f / v16 ) * cmd_right.x, ( 1.f / v16 ) * cmd_right.y, 0.f );
		const Vector norm_cmd_up( 0.f, 0.f, ( 1.f / v18 ) * cmd_up.z );

		/* return angles */
		const auto v22 = norm_view_fwd.x * cmd->forwardmove;
		const auto v26 = norm_view_fwd.y * cmd->forwardmove;
		const auto v28 = norm_view_fwd.z * cmd->forwardmove;
		const auto v24 = norm_view_right.x * cmd->sidemove;
		const auto v23 = norm_view_right.y * cmd->sidemove;
		const auto v25 = norm_view_right.z * cmd->sidemove;
		const auto v30 = norm_view_up.x * cmd->upmove;
		const auto v27 = norm_view_up.z * cmd->upmove;
		const auto v29 = norm_view_up.y * cmd->upmove;

		/* fix moving */
		cmd->forwardmove = ( ( ( ( norm_cmd_fwd.x * v24 ) + ( norm_cmd_fwd.y * v23 ) ) + ( norm_cmd_fwd.z * v25 ) )
			+ ( ( ( norm_cmd_fwd.x * v22 ) + ( norm_cmd_fwd.y * v26 ) ) + ( norm_cmd_fwd.z * v28 ) ) )
			+ ( ( ( norm_cmd_fwd.y * v30 ) + ( norm_cmd_fwd.x * v29 ) ) + ( norm_cmd_fwd.z * v27 ) );
		cmd->sidemove = ( ( ( ( norm_cmd_right.x * v24 ) + ( norm_cmd_right.y * v23 ) ) + ( norm_cmd_right.z * v25 ) )
			+ ( ( ( norm_cmd_right.x * v22 ) + ( norm_cmd_right.y * v26 ) ) + ( norm_cmd_right.z * v28 ) ) )
			+ ( ( ( norm_cmd_right.x * v29 ) + ( norm_cmd_right.y * v30 ) ) + ( norm_cmd_right.z * v27 ) );
		cmd->upmove = ( ( ( ( norm_cmd_up.x * v23 ) + ( norm_cmd_up.y * v24 ) ) + ( norm_cmd_up.z * v25 ) )
			+ ( ( ( norm_cmd_up.x * v26 ) + ( norm_cmd_up.y * v22 ) ) + ( norm_cmd_up.z * v28 ) ) )
			+ ( ( ( norm_cmd_up.x * v30 ) + ( norm_cmd_up.y * v29 ) ) + ( norm_cmd_up.z * v27 ) );

		/* return movement speed */
		const auto ratio = 2.f - fmaxf( fabsf( cmd->sidemove ), fabsf( cmd->forwardmove ) ) / 450.f;
		cmd->forwardmove *= ratio;
		cmd->sidemove *= ratio;

		/* fix movement */
		wishangle = cmd->viewangles;
	}
}


