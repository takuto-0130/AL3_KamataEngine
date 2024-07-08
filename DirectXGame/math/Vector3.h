#pragma once

/// <summary>
/// 3次元ベクトル
/// </summary>
struct Vector3 final {
	float x;
	float y;
	float z;

	Vector3 operator+=(const Vector3& v) { 
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}
};