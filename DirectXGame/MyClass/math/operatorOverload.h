#pragma once
#include "Vector3.h"
#include "MyClass/math/Matrix4x4Func.h"

Vector3 operator+(const Vector3& a, const Vector3& b);

Vector3 operator-(const Vector3& a, const Vector3& b);

Vector3 operator*(const Vector3& a, const Vector3& b);
Vector3 operator*(const Vector3& v, const float& s);
Vector3 operator*(const float& s, const Vector3& v);

Vector3 operator/(const Vector3& a, const Vector3& b);
Vector3 operator/(const Vector3& v, const float& s);

Matrix4x4 operator*(const Matrix4x4& m1, const Matrix4x4& m2);