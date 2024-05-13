#include "mathFunc.h"
#define _USE_MATH_DEFINES
#include <cmath>

Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m) { 
	Vector3 result{
		v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0], 
		v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1], 
		v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2]};
	return result;
}

float Length(const Vector3& v) { return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z); }

Vector3 Normalize(const Vector3& v) {
	float len = Length(v);
	Vector3 result{v.x / len, v.y / len, v.z / len};
	return result;
}