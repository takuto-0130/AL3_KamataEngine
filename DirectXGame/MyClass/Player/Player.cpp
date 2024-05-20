#include "Player.h"
#include "./MyClass/math/Matrix4x4Func.h"
#include <imgui.h>
#include "./MyClass/math/mathFunc.h"
#include "./MyClass/math/operatorOverload.h"
#include <TextureManager.h>

const float kMoveLimitX = 25;
const float kMoveLimitY = 12;

// キャラの移動ベクトル
Vector3 move = {0, 0, 0};
// キャラの移動の速さ
const float kCharacterSpeed = 0.2f;

//回転の速さ[ラジアン/frame]
const float kRotSpeed = 0.02f;

Player::Player() {}

Player::~Player() { 
	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}
	delete sprite2DReticle_;
}

void Player::Initialize(Model* model, uint32_t texHandle, const Vector3& pos) { 
	assert(model);

	model_ = model;
	texHandle_ = texHandle;
	worldTransform_.Initialize();
	worldTransform_.translation_ = pos;
	input_ = Input::GetInstance();
	worldTransform3DReticle_.Initialize();

	uint32_t textureReticle = TextureManager::Load("./Resources/reticle.png");
	sprite2DReticle_ = Sprite::Create(textureReticle, {}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f});
}

void Player::Update(ViewProjection& viewProjection) {
	//デスフラグの立った弾を削除
	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	move = {0, 0, 0};

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
	worldTransform_.translation_ = worldTransform_.translation_ + move;

	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, kMoveLimitY);

	Rotate();

	worldTransform_.UpdateMatrix();

	// 自機のワールド座標から3Dレティクルのワールド座標を計算
	{ 
		const float kDistancePlayerTo3DReticle = 50.0f;
		Vector3 offset{0, 0, 1.0f};
		offset = TransformNormal(offset, worldTransform_.matWorld_);
		offset = Normalize(offset) * kDistancePlayerTo3DReticle;
		worldTransform3DReticle_.translation_ = GetWorldPosition() + offset;
		worldTransform3DReticle_.UpdateMatrix();
	}
	//
	{ 
		Vector3 positionReticle = GetWorldPosition3DReticle();
		Matrix4x4 matViewport = MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);
		Matrix4x4 matViewProjectionViewport = viewProjection.matView * viewProjection.matProjection * matViewport;
		positionReticle = Transform(positionReticle, matViewProjectionViewport);
		sprite2DReticle_->SetPosition({positionReticle.x, positionReticle.y});
	}

	float inputFloat3[3] = {worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z};
	
	ImGui::Begin("player");
	ImGui::SliderFloat3("Player", inputFloat3, -100.0f,100.0f);
	ImGui::End();
	worldTransform_.translation_.x = inputFloat3[0];
	worldTransform_.translation_.y = inputFloat3[1];
	worldTransform_.translation_.z = inputFloat3[2];

	Attack();
	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}
}

void Player::Rotate() { 
	if (input_->PushKey(DIK_A)) {
		worldTransform_.rotation_.y -= kRotSpeed;
	} else if (input_->PushKey(DIK_D)) {
		worldTransform_.rotation_.y += kRotSpeed;
	}
}

void Player::Draw(ViewProjection& viewProjection) { 
	model_->Draw(worldTransform_, viewProjection, texHandle_);

	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}

}

void Player::DrawUI() { 
	sprite2DReticle_->Draw();
}

void Player::Attack() { 

	if (input_->TriggerKey(DIK_SPACE)) {
		const float kBulletSpeed = 1.0f;
		Vector3 velocity = GetWorldPosition3DReticle() - GetWorldPosition();
		velocity = Normalize(velocity) * kBulletSpeed;

		//速度ベクトルを自機の向きに合わせて回転させる
		//velocity = TransformNormal(velocity, worldTransform_.matWorld_);
		//
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, GetWorldPosition(), velocity);

		bullets_.push_back(newBullet);
	}

}

void Player::OnCollision() {}

Vector3 Player::GetWorldPosition() { 
	Vector3 worldPos;
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}

Vector3 Player::GetWorldPosition3DReticle() {
	Vector3 worldPos3DReticle;
	worldPos3DReticle.x = worldTransform3DReticle_.matWorld_.m[3][0];
	worldPos3DReticle.y = worldTransform3DReticle_.matWorld_.m[3][1];
	worldPos3DReticle.z = worldTransform3DReticle_.matWorld_.m[3][2];
	return worldPos3DReticle;
}

void Player::SetParent(const WorldTransform* parent) { 
	worldTransform_.parent_ = parent;
}
