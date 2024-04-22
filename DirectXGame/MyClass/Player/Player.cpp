#include "Player.h"
#include "./MyClass/math/Matrix4x4Func.h"
#include <imgui.h>

const float kMoveLimitX = 50;
const float kMoveLimitY = 50;

void Player::Initialize(Model* model, uint32_t texHandle) { 
	assert(model);

	model_ = model;
	texHandle_ = texHandle;
	worldTransform_.Initialize();
	input_ = Input::GetInstance();
}

void Player::Update() {
	worldTransform_.TransferMatrix();

	//キャラの移動ベクトル
	Vector3 move = {0, 0, 0};

	//キャラの移動の速さ
	const float kCharacterSpeed = 0.2f;

	//押した方向で移動ベクトルを変更(左右)
	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kCharacterSpeed;
	}
	else if(input_->PushKey(DIK_RIGHT)){
		move.x += kCharacterSpeed;
	}

	// 押した方向で移動ベクトルを変更(上下)
	if (input_->PushKey(DIK_UP)) {
		move.y += kCharacterSpeed;
	} else if (input_->PushKey(DIK_DOWN)) {
		move.y -= kCharacterSpeed;
	}

	//座標移動（ベクトルの加算）
	worldTransform_.translation_ = {
	    worldTransform_.translation_.x + move.x,
	    worldTransform_.translation_.y + move.y,
	    worldTransform_.translation_.z + move.z,
	};

	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, kMoveLimitY);


	worldTransform_.matWorld_ = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	float inputFloat3[3] = {worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z};
	
	ImGui::Begin("player");
	ImGui::SliderFloat3("Player", inputFloat3, -100.0f,100.0f);
	ImGui::End();
	worldTransform_.translation_.x = inputFloat3[0];
	worldTransform_.translation_.y = inputFloat3[1];
	worldTransform_.translation_.z = inputFloat3[2];
}

void Player::Draw(ViewProjection& viewProjection) { 
	model_->Draw(worldTransform_, viewProjection, texHandle_);


}
