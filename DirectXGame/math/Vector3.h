#pragma once

/// <summary>
/// 3次元ベクトル
/// </summary>
struct Vector3 final {
	float x;
	float y;
	float z;

	Vector3& operator+=(const Vector3& a) {
		x = a.x + x;
		y = a.y + y;
		z = a.z + z;
		return *this;
	}

	Vector3& operator-=(const Vector3& a) {
		x = x - a.x;
		y = y - a.y;
		z = z - a.z;
		return *this;
	}

	Vector3& operator*=(const float& a) {
		x = x * a;
		y = y * a;
		z = z * a;
		return *this;
	}

	Vector3& operator/=(const float& a) {
		x = x / a;
		y = y / a;
		z = z / a;
		return *this;
	}
};