#pragma once
#include "Matrix4x4.h"
#include "Vector3.h"
#include "Vector2.h"
#include <vector>
#define _USE_MATH_DEFINES
#include <math.h>

Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);

float Length(const Vector3& v);

float Length(const Vector2& v);

Vector3 Normalize(const Vector3& v);

float Dot(const Vector3& v1, const Vector3& v2);

Vector3 Subtruct(const Vector3& v1, const Vector3& v2);

Vector3 Add(const Vector3& v1, const Vector3& v2);

const Vector3 Multiply(const Vector3& v1, const Vector3& v2);

const Vector3 Multiply(const Vector3& v, const float& s);

const Vector3 Multiply(const float& s, const Vector3& v);

float Lerp(const float& a, const float& b, float t);

Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t);

Vector3 Slerp(const Vector3& v1, const Vector3& v2, float t);

Vector3 CatmullRomInterpolation(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3, float t);

Vector3 CatmullRomPosition(const std::vector<Vector3>& points, float t);

float LerpShortAngle(float a, float b, float t);
