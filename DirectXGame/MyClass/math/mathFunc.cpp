#include "mathFunc.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include <MyClass/math/operatorOverload.h>
#include <algorithm>

Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m) { 
	Vector3 result{
		v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0], 
		v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1], 
		v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2]};
	return result;
}

float Length(const Vector3& v) { 
	return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

Vector3 Normalize(const Vector3& v) {
	float len = Length(v);
	Vector3 result{v.x / len, v.y / len, v.z / len};
	return result;
}

float Dot(const Vector3& v1, const Vector3& v2) { 
	return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
}

Vector3 Subtruct(const Vector3& v1, const Vector3& v2) { 
	return {v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};
}

Vector3 Add(const Vector3& v1, const Vector3& v2) { 
	return {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z}; 
}

const Vector3 Multiply(const Vector3& v1, const Vector3& v2) {
	return {v1.x * v2.x, v1.y * v2.y, v1.z * v2.z}; 
}

const Vector3 Multiply(const Vector3& v, const float& s) { 
	return {v.x * s, v.y * s, v.z * s}; 
}

const Vector3 Multiply(const float& s, const Vector3& v) { 
	return {v.x * s, v.y * s, v.z * s}; 
}

float Lerp(const float& a, const float& b, float t) { 
	return a + t * (b - a);
}

Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t) { 
	return v1 + t * (v2 - v1);
}

Vector3 Slerp(const Vector3& v1, const Vector3& v2, float t) { 
	Vector3 nV1 = Normalize(v1);
	Vector3 nV2 = Normalize(v2);
	float dot = Dot(nV1, nV2);

	dot = (std::min)(dot, 1.0f);
	float theta = std::acos(dot);
	float sinTheta = std::sin(theta);
	float sinThetaFrom = std::sin((1 - t) * theta);
	float sinThetaTo = std::sin(t * theta);
	Vector3 nSlerp{};
	if (sinTheta < 1.0e-5) {
		nSlerp = nV1;
	} else {
		nSlerp = (sinThetaFrom * nV1 + sinThetaTo * nV2) / sinTheta;
	}
	float length = Lerp(Length(v1), Length(v2), t);

	return length * nSlerp;
}