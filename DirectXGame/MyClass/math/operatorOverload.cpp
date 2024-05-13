#include "operatorOverload.h"

Vector3 operator+(const Vector3& a, const Vector3& b) {
	Vector3 result;
	result.x = a.x + b.x;
	result.y = a.y + b.y;
	result.z = a.z + b.z;
	return result;
}
Vector3 operator-(const Vector3& a, const Vector3& b) {
	Vector3 result;
	result.x = a.x - b.x;
	result.y = a.y - b.y;
	result.z = a.z - b.z;
	return result;
}
Vector3 operator*(const Vector3& a, const Vector3& b) {
	Vector3 result;
	result.x = a.x * b.x;
	result.y = a.y * b.y;
	result.z = a.z * b.z;
	return result;
}
Vector3 operator/(const Vector3& a, const Vector3& b) {
	Vector3 result;
	result.x = a.x / b.x;
	result.y = a.y / b.y;
	result.z = a.z / b.z;
	return result;
}