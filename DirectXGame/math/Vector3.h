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
};