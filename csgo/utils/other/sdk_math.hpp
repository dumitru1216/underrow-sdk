#pragma once
#include "../../sdk\other/vmatrix.hpp"

#define PI 3.14159265358979323846f

class Math {
public:

	void vector_angles(const Vector &vecForward, Vector &vecAngles);
	void vector_angles(const Vector& forward, Vector& up, Vector& angles);
	void angle_vectors( const Vector& angles, Vector* forward );
	void angle_vectors( const Vector& angles, Vector* forward, Vector* right, Vector* up );
	float random_float( float min, float max );
	void normalize_angles( Vector& angles );
	Vector calc_angle( const Vector& vecSource, const Vector& vecDestination );
	float grd_to_bog( float GRD );

	void VectorTransform(const Vector in1, const matrix3x4_t in2, Vector &out);


	vec_t VectorNormalize(Vector& v);





	float NormalizeYaw(float yaw);


	void RandomSeed(int seed);

	

	bool Clamp(Vector &angles);

	void ClampAngles(Vector &angles);

	

	float Distance(Vector2D point1, Vector2D point2);
}; 
extern Math math;