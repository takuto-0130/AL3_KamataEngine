#pragma once
#include "Matrix4x4.h"
#include "Vector3.h"

Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);

float Length(const Vector3& v);

Vector3 Normalize(const Vector3& v);
