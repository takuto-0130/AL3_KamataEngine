#include "Player.h"
#include "./MyClass/math/Matrix4x4Func.h"
#include <imgui.h>
#include "./MyClass/math/mathFunc.h"
#include "./MyClass/math/operatorOverload.h"
#include <TextureManager.h>
#include "MyClass/Enemy/Enemy.h"

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
	for (Sprite* sprite : lockOnSprite2DReticle_) {
		delete sprite;
	}
	
}

void Player::Initialize(Model* model, uint32_t texHandle, const Vector3& pos) { 
	assert(model);

	model_ = model;
	texHandle_ = texHandle;
	worldTransform_.Initialize();
	worldTransform_.translation_ = pos;
	input_ = Input::GetInstance();
	worldTransform3DReticle_.Initialize();
	SetRadius(radius_);
	textureReticle = TextureManager::Load("./Resources/reticle.png");
	sprite2DReticle_ = Sprite::Create(textureReticle, {640,360}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f});
	
	SetCollisionAttribute(kCollisionAttributePlayer);
	SetCollisionMask(kCollisionAttributePlayer ^ 0x00000000);
}

void Player::Update(ViewProjection& viewProjection) {
	SetRadius(radius_);
	//デスフラグの立った弾を削除
	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	move = {0, 0, 0};

	Move();

	Rotate();

	worldTransform_.UpdateMatrix();

	// レティクルの設定
	SetReticlePosition(viewProjection);

	//LockOnDeadRemove();
	Attack();
	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}
	viewProjection_.matView = Inverse(worldTransform_.matWorld_);
	float inputFloat3[3] = {GetWorldPosition().x};
	
	ImGui::Begin("player");
	ImGui::InputFloat3("Player", inputFloat3);
	ImGui::End();
}

void Player::Rotate() { 
	if (input_->PushKey(DIK_Q)) {
		worldTransform_.rotation_.y -= kRotSpeed;
	} else if (input_->PushKey(DIK_E)) {
		worldTransform_.rotation_.y += kRotSpeed;
	}
}

void Player::Draw(ViewProjection& viewProjection) { 
	model_->Draw(worldTransform_, viewProjection, texHandle_);
	model_->Draw(worldTransform3DReticle_, viewProjection, texHandle_);


	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}

}

void Player::DrawUI(ViewProjection& viewProjection) {
	sprite2DReticle_->Draw();
	for (Enemy* enemy : lockOnEnemys_) {
		if (enemy->IsDead() == false) {
			Vector3 pos = enemy->GetWorldPosition();
			Matrix4x4 matViewport = MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);
			Matrix4x4 matVPV = viewProjection.matView * viewProjection.matProjection * matViewport;
			pos = Transform(pos, matVPV);
			Sprite* reticle = Sprite::Create(textureReticle, {pos.x, pos.y}, {1.0f, 0.0f, 0.0f, 1.0f}, {0.5f, 0.5f});
			reticle->Draw();
			//lockOnSprite2DReticle_.push_back(reticle);
		}
	}
}

void Player::Attack() { 
	XINPUT_STATE joyState;
	if (!Input::GetInstance()->GetJoystickState(0, joyState)) {
		return;
	}

	if (bulletInterval_ > 0) {
		bulletInterval_--;
	}
	

	if ((joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) && bulletInterval_<=0) {
		if (isLockOn_ == false) {
			const float kBulletSpeed = 1.0f;
			Vector3 velocity = GetWorldPosition3DReticle() - GetWorldPosition();
			velocity = Normalize(velocity) * kBulletSpeed;

			// 速度ベクトルを自機の向きに合わせて回転させる
			// velocity = TransformNormal(velocity, worldTransform_.matWorld_);
			//
			PlayerBullet* newBullet = new PlayerBullet();
			newBullet->Initialize(model_, GetWorldPosition(), velocity);

			bullets_.push_back(newBullet);
			bulletInterval_ = kPlayerBulletInterval;
		} else {
			//const float kBulletSpeed = 2.0f;
			//Vector3 velocity = LockOnPos_ - GetWorldPosition();
			//velocity = Normalize(velocity) * kBulletSpeed;

			//PlayerBullet* newBullet = new PlayerBullet();
			//newBullet->Initialize(model_, GetWorldPosition(), velocity);

			//bullets_.push_back(newBullet);
			//bulletInterval_ = kPlayerBulletInterval;

			for (Enemy* lockEnemy : lockOnEnemys_) {
				if (lockEnemy->IsDead() == false)
				{
					const float kBulletSpeed = 2.0f;
					Vector3 velocity = lockEnemy->GetWorldPosition() - GetWorldPosition();
					velocity = Normalize(velocity) * kBulletSpeed;

					PlayerBullet* newBullet = new PlayerBullet();
					newBullet->Initialize(model_, GetWorldPosition(), velocity);

					bullets_.push_back(newBullet);
				}
			}
			lockOnEnemys_.clear();
			bulletInterval_ = kPlayerBulletInterval;
		}
	}
}


void Player::OnCollision() {}


Vector3 Player::GetWorldPosition() const{ 
	Vector3 worldPos;
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}


const Vector3 Player::GetWorldPosition3DReticle() {
	Vector3 worldPos3DReticle;
	worldPos3DReticle.x = worldTransform3DReticle_.matWorld_.m[3][0];
	worldPos3DReticle.y = worldTransform3DReticle_.matWorld_.m[3][1];
	worldPos3DReticle.z = worldTransform3DReticle_.matWorld_.m[3][2];
	return worldPos3DReticle;
}


void Player::SetParent(const WorldTransform* parent) { 
	worldTransform_.parent_ = parent; 
}

void Player::SingleLockOn(ViewProjection& viewProjection) {
	isLockOn_ = false;
	for (Enemy* enemy : enemys_) {
		Vector3 pos = enemy->GetWorldPosition();
		Vector2 spritePosition = sprite2DReticle_->GetPosition();
		Matrix4x4 matViewport = MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);
		Matrix4x4 matVPV = viewProjection.matView * viewProjection.matProjection * matViewport;
		LockOnPos_ = pos;
		pos = Transform(pos, matVPV);
		if (Length(Vector2{pos.x, pos.y} - spritePosition) <= 20) {
			sprite2DReticle_->SetPosition(Vector2{pos.x, pos.y});
			isLockOn_ = true;
			break;
		}
	}
}

void Player::MultiLockOn(ViewProjection& viewProjection) {
	sprite2DReticle_->SetColor({1, 1, 1, 1});
	if (lockOnEnemys_.size() == 0) {
		isLockOn_ = false;
	}
	for (Enemy* enemy : enemys_) {
		Vector3 pos = enemy->GetWorldPosition();
		Vector2 spritePosition = sprite2DReticle_->GetPosition();
		Matrix4x4 matViewport = MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);
		Matrix4x4 matVPV = viewProjection.matView * viewProjection.matProjection * matViewport;
		pos = Transform(pos, matVPV);
		if (Length(Vector2{pos.x, pos.y} - spritePosition) <= reticleRadius_) {
			sprite2DReticle_->SetColor({1, 0, 0, 1});
			bool isAdd = true;
			for (Enemy* lockEnemy : lockOnEnemys_) {
				if (lockEnemy == enemy) {
					isAdd = false;
					break;
				}
			}
			if (isAdd) {
				isLockOn_ = true;
				lockOnEnemys_.push_back(enemy);
			}
			break;
		}
	}
}

void Player::LockOnDeadRemove() {
	lockOnEnemys_.remove_if([](Enemy* enemy) {
		if (enemy == nullptr) {
			delete enemy;
			return true;
		}
		return false;
	});
}


void Player::SetReticlePosition(ViewProjection& viewProjection) {
	// SingleLockOn(viewProjection);
	MultiLockOn(viewProjection);
	// 2DReticle
	Vector2 spritePosition = sprite2DReticle_->GetPosition();
	XINPUT_STATE joyState;
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		spritePosition.x += float(joyState.Gamepad.sThumbRX) / SHRT_MAX * 5.0f;
		spritePosition.y -= float(joyState.Gamepad.sThumbRY) / SHRT_MAX * 5.0f;
		sprite2DReticle_->SetPosition(spritePosition);
	}

	// 3DReticle
	Matrix4x4 matViewport = MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);
	Matrix4x4 matVPV = viewProjection.matView * viewProjection.matProjection * matViewport;
	Matrix4x4 matInverseVPV = Inverse(matVPV);
	//スクリーン座標
	Vector3 posNear = {float(spritePosition.x), float(spritePosition.y), 0};
	Vector3 posFar = {float(spritePosition.x), float(spritePosition.y), 1};
	//ワールド座標へ
	posNear = Transform(posNear, matInverseVPV);
	posFar = Transform(posFar, matInverseVPV);
	//レティクルレイの方向
	Vector3 reticleDirection = posFar - posNear;
	reticleDirection = Normalize(reticleDirection);
	//カメラから照準オブジェクトの距離
	const float kDistanceTestObject = 200.0f;
	worldTransform3DReticle_.translation_ = posNear + (reticleDirection * kDistanceTestObject);
	worldTransform3DReticle_.UpdateMatrix();

	/*if (isLockOn_ == false) {
		sprite2DReticle_->SetColor({1, 1, 1, 1});
	} else {
		sprite2DReticle_->SetColor({1, 0, 0, 1});
	}*/
	ReticleLimit();
	ImGui::Begin("player");
	ImGui::Text("2DReticle:(%f,%f)", sprite2DReticle_->GetPosition().x, sprite2DReticle_->GetPosition().y);
	ImGui::Text("Near:(%+.2f,%+.2f,%+.2f)", posNear.x, posNear.y, posNear.z);
	ImGui::Text("Far:(%+.2f,%+.2f,%+.2f)", posFar.x, posFar.y, posFar.z);
	ImGui::Text("3DReticle:(%+.2f,%+.2f,%+.2f)", worldTransform3DReticle_.translation_.x,
		worldTransform3DReticle_.translation_.y, worldTransform3DReticle_.translation_.z);
	ImGui::End();
}

void Player::Move() { 
	XINPUT_STATE joyState;
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		move.x += float(joyState.Gamepad.sThumbLX) / SHRT_MAX * kCharacterSpeed;
		move.y += float(joyState.Gamepad.sThumbLY) / SHRT_MAX * kCharacterSpeed;
	} else {
		Vector3 dir{};
		if (input_->PushKey(DIK_W)) {
			dir.y += 1.0f;
		}
		if (input_->PushKey(DIK_A)) {
			dir.x -= 1.0f;
		}
		if (input_->PushKey(DIK_S)) {
			dir.y -= 1.0f;
		}
		if (input_->PushKey(DIK_D)) {
			dir.x += 1.0f;
		}
		move = dir * kCharacterSpeed;
	}

	// 座標移動（ベクトルの加算）
	worldTransform_.translation_ = worldTransform_.translation_ + move;

	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, kMoveLimitY);
}

void Player::ReticleLimit() {
	Vector2 spritePosition = sprite2DReticle_->GetPosition();
	spritePosition.x = max(spritePosition.x, 0);
	spritePosition.x = min(spritePosition.x, WinApp::kWindowWidth);
	spritePosition.y = max(spritePosition.y, 0);
	spritePosition.y = min(spritePosition.y, WinApp::kWindowHeight);
	sprite2DReticle_->SetPosition(spritePosition);
}
