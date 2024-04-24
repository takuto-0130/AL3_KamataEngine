#include "WorldTransform.h"
#include "MyClass/math/Matrix4x4Func.h"

void WorldTransform::UpdateMatrix() { 
	
	matWorld_ = MakeAffineMatrix(scale_, rotation_, translation_); 
	TransferMatrix();
}