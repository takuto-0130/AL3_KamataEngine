#pragma once
#include "Vector3.h"

Vector3 operator+(const Vector3& a, const Vector3& b);

Vector3 operator-(const Vector3& a, const Vector3& b);

Vector3 operator*(const Vector3& a, const Vector3& b);
Vector3 operator*(const Vector3& v, const float& s);
Vector3 operator*(const float& s, const Vector3& v);

Vector3 operator/(const Vector3& a, const Vector3& b);
Vector3 operator/(const Vector3& v, const float& s);