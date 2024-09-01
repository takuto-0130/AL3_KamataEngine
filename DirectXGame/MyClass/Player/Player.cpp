#include "Player.h"
#define _USE_MATH_DEFINES
#include <math.h>
#ifdef _DEBUG
#include <imgui.h>
#endif // _DEBUG

#include "MyClass/math/mathFunc.h"
#include "MyClass/math/Matrix4x4Func.h"
#include "MyClass/math/operatorOverload.h"
#include "MyClass/GlobalVariables/GlobalVariables.h"
#include <TextureManager.h>
#include "MyClass/Player/LockOn.h"


Player::~Player() {
	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}
}

void Player::Initialize(const std::vector<Model*>& models) { 
	assert(models[0]);
	BaseCharacter::Initialize(models);
	model_.reset(Model::Create());
	worldTransform_.translation_ = {0, 0, -30};
	worldTransformBody_.Initialize();
	worldTransformHead_.Initialize();
	worldTransformHead_.translation_ = {0.f, 1.6f, 0.f};
	worldTransformL_arm_.Initialize();
	worldTransformL_arm_.translation_ = {-0.55f, 1.22f, 0.f};
	worldTransformR_arm_.Initialize();
	worldTransformR_arm_.translation_ = {0.55f, 1.22f, 0.f};
	worldTransformBody_.parent_ = &worldTransform_;
	worldTransformHead_.parent_ = &worldTransformBody_;
	worldTransformL_arm_.parent_ = &worldTransformBody_;
	worldTransformR_arm_.parent_ = &worldTransformBody_;
	worldTransformHammer_.parent_ = &worldTransformBody_;

	worldTransformHammer_.Initialize();
	worldTransformHammer_.scale_ = {0.5f, 0.5f, 0.5f};
	worldTransformHammer_.translation_ = {0.0f, 1.4f, 0.0f};


	worldTransform3DReticle_.Initialize();
	worldTransform3DReticle_.scale_ = {0.5f, 0.5f, 0.5f};
	textureReticle = TextureManager::Load("./Resources/reticle.png");
	sprite2DReticle_.reset(Sprite::Create(textureReticle, {}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}));

	textureEdge = TextureManager::Load("./Resources/edge.png");
	spriteEdge_.reset(Sprite::Create(textureEdge, {30, 650}, {1.0f, 1.0f, 1.0f, 1.0f}, {0, 0}));

	textureGauge = TextureManager::Load("./Resources/gauge.png");
	spriteGauge_.reset(Sprite::Create(textureGauge, {30, 650}, {1.0f, 1.0f, 0.4f, 1.0f}, {0, 0}));

	textureCoolDown = TextureManager::Load("./Resources/gauge.png");
	spriteCoolDown_.reset(Sprite::Create(textureCoolDown, {30, 650}, {0.4f, 0.4f, 0.4f, 0.85f}, {0, 0}));

	textureSousa = TextureManager::Load("./Resources/sousa.png");
	spriteSousa_.reset(Sprite::Create(textureSousa, {10, 60}, {1.0f, 1.0f, 1.0f, 1.0f}, {0, 0}));


	InitializeFloatingGimmick();
	BehaviorAttackInitialize();
	input_ = Input::GetInstance();

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Player";
	GlobalVariables::GetInstance()->CreateGroup(groupName);
	GlobalVariables::GetInstance()->LoadFiles();
	globalVariables->AddItem(groupName, "Head Translation", worldTransformHead_.translation_);
	globalVariables->AddItem(groupName, "ArmL Translation", worldTransformL_arm_.translation_);
	globalVariables->AddItem(groupName, "ArmR Translation", worldTransformR_arm_.translation_);
	BaseCharacter::Collider::SetRadius(radius_);
	SetCollisionAttribute(kCollisionAttributePlayer);
	SetCollisionMask(kCollisionAttributePlayer ^ 0x00000000);
	HP_ = kPlayerMaxHP;

	textureName = TextureManager::Load("./Resources/playerName.png");
	spriteName_.reset(Sprite::Create(textureName, {350, 530}, {1.0f, 1.0f, 1.0f, 1.0f}, {0, 0}));

	textureHP = TextureManager::Load("./Resources/playerHP.png");
	spriteHP_.reset(Sprite::Create(textureHP, {355, 599}, {1.0f, 1.0f, 1.0f, 1.0f}, {0, 0}));
}

void Player::Reset() {
	worldTransform_.translation_ = {0, 0, -30};
	worldTransform_.rotation_ = {0, 0, 0};
	for (PlayerBullet* bullet : bullets_) {
		bullet->OnCollision();
	}
	SetCollisionMask(kCollisionAttributePlayer ^ 0x00000000);
	dashGauge = 0;
	HP_ = kPlayerMaxHP;
	isDead_ = false;
	rotateY = 0;
}

void Player::InitializeFloatingGimmick() { floatingParamater_ = 0.0f; }

void Player::BehaviorAttackInitialize() { 
	attackParamater_ = float(M_PI) / 2.0f;
	furiageWaite_ = 0;
	attackWait_ = 0;
	attackCurrent_ = 0;
	attackAfterWait_ = 0;
}

void Player::BehaviorDashInitialize() { 
	worldTransform_.rotation_.y = rotateY;
	workDash_.dashParamater_ = 0;
	workDash_.dashCurrentTime_ = 0;
	Vector3 vec{0, 0, 1};
	Matrix4x4 rYMatrix = MakeRotateYMatrix(worldTransform_.rotation_.y);
	vec = TransformNormal(vec, rYMatrix);
	workDash_.goalVector_ = vec * workDash_.dashLength_;
}

void Player::Update() { 
	if (!isDead_) {
		if (dashGauge < kDashGaugeMax) {
			dashGauge += kDashGaugeUp;
		} else {
			dashGauge = kDashGaugeMax;
		}
	}

	// デスフラグの立った弾を削除
	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	ApplyGlobalVariables();
	if(!isDead_){
		if (behaviorRequest_) {
			behavior_ = behaviorRequest_.value();
			switch (behavior_) {
			case Behavior::kRoot:
			default:
				InitializeFloatingGimmick();

				SetCollisionMask(kCollisionAttributePlayer ^ 0x00000000);
				break;
			case Behavior::kAttack:
				BehaviorAttackInitialize();
				break;
			case Behavior::kDash:
				BehaviorDashInitialize();
				SetCollisionMask((kCollisionAttributePlayer + kCollisionAttributeEnemyBullet) ^ 0x00000000);
				break;
			}
			behaviorRequest_ = std::nullopt;
		}

		switch (behavior_) {
		case Behavior::kRoot:
		default:
			BehaviorRootUpdate();
			break;
		case Behavior::kAttack:
			BehaviorAttackUpdate();
			break;
		case Behavior::kDash:
			BehaviorDashUpdate();
			break;
		}
	}
#ifdef _DEBUG

	int a = attackWaitTime_;
	int b = attackTime_;
	int c = attackAfterWaitTime_;
	int d = furiageTime_;
	Vector3 v = viewProjection_->rotation_;
	ImGui::Begin("Attack");
	ImGui::SliderInt("before", &d, 0, 100);
	ImGui::SliderInt("wait", &a, 0, 100);
	ImGui::SliderInt("attack", &b, 0, 100);
	ImGui::SliderInt("after", &c, 0, 100);
	ImGui::InputInt("frame", &workDash_.dashCurrentTime_);
	ImGui::DragFloat3("rotate", &worldTransform_.rotation_.x, 0.01f);
	ImGui::InputFloat3("Crotate", &v.x);
	ImGui::DragFloat3("pos", &worldTransform_.translation_.x, 0.01f);
	ImGui::End();
	attackWaitTime_ = a;
	attackTime_ = b;
	attackAfterWaitTime_ = c;
	furiageTime_ = d;
#endif // _DEBUG

	
	// 自機のワールド座標から3Dレティクルのワールド座標を計算
	if(!isDead_){
		{
			if (lockOn_ && lockOn_->ExsitTarget()) {
				worldTransform3DReticle_.translation_ = lockOn_->GetTargetPosition();
			} else {
				const float kDistancePlayerTo3DReticle = 50.0f;
				Vector3 offset{0, 0, 1.0f};
				offset = TransformNormal(offset, worldTransform_.matWorld_);
				offset = Normalize(offset) * kDistancePlayerTo3DReticle;
				worldTransform3DReticle_.translation_ = GetHeadWorldPosition() + offset;
			}
			worldTransform3DReticle_.UpdateMatrix();
		}
		//
		{
			Vector3 positionReticle = GetWorldPosition3DReticle();
			Matrix4x4 matViewport = MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);
			Matrix4x4 matViewProjectionViewport = viewProjection_->matView * viewProjection_->matProjection * matViewport;
			positionReticle = Transform(positionReticle, matViewProjectionViewport);
			sprite2DReticle_->SetPosition({positionReticle.x, positionReticle.y});
		}

		Attack();
		for (PlayerBullet* bullet : bullets_) {
			bullet->Update();
		}
	}
}

void Player::BehaviorRootUpdate() {
	UpdateFlotingGimmick();
	Move();

	PlayerUpdateMatrix();
	
	XINPUT_STATE joyState;
	Vector3 move{0, 0, 0};
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		/*if (joyState.Gamepad.wButtons == XINPUT_GAMEPAD_X) {
			behaviorRequest_ = Behavior::kAttack;
		}*/
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A && dashGauge >= kDashDecrease) {
			behaviorRequest_ = Behavior::kDash;
			dashGauge -= kDashDecrease;
		}
	}
}

void Player::BehaviorAttackUpdate() {
	//// 1フレームでのパラメーター加算値
	//
	//const float waitStep = 2.0f * float(M_PI) / (furiageTime_ * 4.0f);

	//const float step = 2.0f * float(M_PI) / (attackTime_ * 4.0f);

	//const float amplitude = 1.9f;

	//if (furiageWaite_ <= furiageTime_) {
	//	furiageWaite_++;
	//	attackParamater_ -= waitStep;
	//}
	//else if (attackWait_ < attackWaitTime_) {
	//	attackWait_++;
	//}
	//else if (attackCurrent_ <= attackTime_) {
	//	attackCurrent_++;
	//	attackParamater_ += step;
	//} 
	//else if (attackAfterWait_ < attackAfterWaitTime_) {
	//	attackAfterWait_++;
	//}
	//else {
	//	behaviorRequest_ = Behavior::kRoot;
	//}
	//
	//float offset = -0.2f;

	//worldTransformHammer_.rotation_.x = std::sin(attackParamater_) * amplitude + offset;

	//offset = -3.4f;
	//worldTransformL_arm_.rotation_.x = std::sin(attackParamater_) * amplitude + offset;
	//worldTransformR_arm_.rotation_.x = std::sin(attackParamater_) * amplitude + offset;
	//
	//BaseCharacter::Update();
	//worldTransformBody_.UpdateMatrix();
	//worldTransformHead_.UpdateMatrix();
	//worldTransformL_arm_.UpdateMatrix();
	//worldTransformR_arm_.UpdateMatrix();
	//worldTransformHammer_.UpdateMatrix();
}

void Player::BehaviorDashUpdate() { 
	Vector3 step = workDash_.goalVector_ / static_cast<float>(workDash_.dashFrame);
	if (workDash_.dashCurrentTime_ <= workDash_.dashFrame) {
		workDash_.dashCurrentTime_++;
		worldTransform_.translation_ += step;
	} else {
		behaviorRequest_ = Behavior::kRoot;
	}

#ifdef _DEBUG
	ImGui::Begin("Attack");
	ImGui::DragFloat3("rotate", &step.x, 0.01f);
	ImGui::End();
#endif // _DEBUG

	BaseCharacter::Update();
	worldTransformBody_.UpdateMatrix();
	worldTransformHead_.UpdateMatrix();
	worldTransformL_arm_.UpdateMatrix();
	worldTransformR_arm_.UpdateMatrix();
}

void Player::BehaviorJumpUpdate() {}

void Player::Move() {
	XINPUT_STATE joyState;
	Vector3 move{0, 0, 0};
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		const float threshold = 0.7f;
		bool isMoving = false;
		move = {float(joyState.Gamepad.sThumbLX) / SHRT_MAX, 0, float(joyState.Gamepad.sThumbLY) / SHRT_MAX};
		if (Length(move) > threshold) {
			isMoving = true;
		}
		if (isMoving) {
			move.x += float(joyState.Gamepad.sThumbLX) / SHRT_MAX;
			move.z += float(joyState.Gamepad.sThumbLY) / SHRT_MAX;
			move = TransformNormal(Normalize(move), MakeRotateYMatrix(viewProjection_->rotation_.y));
			worldTransform_.translation_ += move * kCharacterSpeed_;
			// Y軸周り角度(θy)
			rotateY = std::atan2(move.x, move.z);
		}
	}
	MoveLimit();
	worldTransform_.rotation_.y = LerpShortAngle(worldTransform_.rotation_.y, rotateY, 0.1f);
}

void Player::Draw(const ViewProjection& viewProjection) { 
	if (!isDead_) {
		models_[kBody]->Draw(worldTransformBody_, viewProjection);
		models_[kHead]->Draw(worldTransformHead_, viewProjection);
		models_[kL_arm]->Draw(worldTransformL_arm_, viewProjection);
		models_[kR_arm]->Draw(worldTransformR_arm_, viewProjection);
		for (PlayerBullet* bullet : bullets_) {
			bullet->Draw(viewProjection);
		}
	}

	/*if (behavior_ == Behavior::kAttack) {
		models_[kHammer]->Draw(worldTransformHammer_, viewProjection);
	}*/

}

void Player::DrawUI() {
	float a = fmod(worldTransform_.rotation_.y - viewProjection_->rotation_.y, 6.283f);
	if (!isDead_) {
		if ((worldTransform_.rotation_.y - viewProjection_->rotation_.y) > 0) {
			if (a < 1.0f || a > 5.0f) {
				sprite2DReticle_->Draw();
			}
		} else {
			if (a > -1.0f || a < -5.0f) {
				sprite2DReticle_->Draw();
			}
		}
		spriteGauge_->SetTextureRect({0, 0}, {dashGauge, 30.0f});
		spriteGauge_->SetSize({dashGauge, 30.0f});
		spriteGauge_->Draw();
		/*spriteCoolDown_->SetTextureRect({0, 0}, {240.0f, 30.0f});
		spriteCoolDown_->SetSize({240.0f, 30.0f});
		spriteCoolDown_->Draw();*/
		spriteEdge_->Draw();
		spriteSousa_->Draw();


		spriteName_->Draw();

		spriteHP_->SetTextureRect({0, 0}, {502.0f * (float(HP_) / float(kPlayerMaxHP)), 22.0f});
		spriteHP_->SetSize({502.0f * (float(HP_) / float(kPlayerMaxHP)), 22.0f});
		spriteHP_->Draw();
	}
}

void Player::UpdateFlotingGimmick() { 
	// 浮遊移動のサイクル<frame>
	const uint16_t cycle = 120;
	// 1フレームでのパラメーター加算値
	const float step = 2.0f * float(M_PI) / cycle;

	const float amplitude = 0.2f;
	// 1step加算
	floatingParamater_ += step;
	// 
	floatingParamater_ = std::fmod(floatingParamater_, 2.0f * float(M_PI));


	worldTransformBody_.translation_.y = std::sin(floatingParamater_) * amplitude;

	const float amplitudeArm = -0.4f;
	worldTransformL_arm_.rotation_.x = std::sin(floatingParamater_) * amplitudeArm;
	worldTransformR_arm_.rotation_.x = std::sin(floatingParamater_) * amplitudeArm;
}

void Player::ApplyGlobalVariables() { 
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Player";
	worldTransformHead_.translation_ = globalVariables->GetVector3Value(groupName, "Head Translation");
	worldTransformL_arm_.translation_ = globalVariables->GetVector3Value(groupName, "ArmL Translation");
	worldTransformR_arm_.translation_ = globalVariables->GetVector3Value(groupName, "ArmR Translation");
}

void Player::Attack() {
	XINPUT_STATE joyState;
	if (!Input::GetInstance()->GetJoystickState(0, joyState)) {
		return;
	}

	if (bulletInterval_ > 0) {
		bulletInterval_--;
	}

	if ((joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) && bulletInterval_ <= 0) {
		if (isLockOn_ == false) {
			const float kBulletSpeed = 1.0f;
			Vector3 velocity = GetWorldPosition3DReticle() - GetHeadWorldPosition();
			velocity = Normalize(velocity) * kBulletSpeed;

			// 速度ベクトルを自機の向きに合わせて回転させる
			// velocity = TransformNormal(velocity, worldTransform_.matWorld_);
			//
			PlayerBullet* newBullet = new PlayerBullet();
			newBullet->Initialize(model_.get(), GetHeadWorldPosition(), velocity);

			bullets_.push_back(newBullet);
			bulletInterval_ = kPlayerBulletInterval;
		} 
	}
	if ((joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)) {
		worldTransformL_arm_.rotation_.x = -1.5f;
		worldTransformR_arm_.rotation_.x = -1.5f;
	}
}

void Player::PlayerUpdateMatrix() {
	BaseCharacter::Update();
	worldTransformBody_.UpdateMatrix();
	worldTransformHead_.UpdateMatrix();
	worldTransformL_arm_.UpdateMatrix();
	worldTransformR_arm_.UpdateMatrix();
	worldTransformHammer_.UpdateMatrix();
}

Vector3 Player::GetWorldPosition() const {
	Vector3 worldPos;
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}

Vector3 Player::GetHeadWorldPosition() const {
	Vector3 worldPos;
	worldPos.x = worldTransformHead_.matWorld_.m[3][0];
	worldPos.y = worldTransformHead_.matWorld_.m[3][1];
	worldPos.z = worldTransformHead_.matWorld_.m[3][2];
	return worldPos;
}

const Vector3 Player::GetWorldPosition3DReticle() {
	Vector3 worldPos3DReticle;
	worldPos3DReticle.x = worldTransform3DReticle_.matWorld_.m[3][0];
	worldPos3DReticle.y = worldTransform3DReticle_.matWorld_.m[3][1];
	worldPos3DReticle.z = worldTransform3DReticle_.matWorld_.m[3][2];
	return worldPos3DReticle;
}

void Player::MoveLimit() { 
	if (worldTransform_.translation_.x <= -kLimitRange) {
		worldTransform_.translation_.x = -kLimitRange;
	}
	if (worldTransform_.translation_.x >= kLimitRange) {
		worldTransform_.translation_.x = kLimitRange;
	}
	if (worldTransform_.translation_.z <= -kLimitRange) {
		worldTransform_.translation_.z = -kLimitRange;
	}
	if (worldTransform_.translation_.z >= kLimitRange) {
		worldTransform_.translation_.z = kLimitRange;
	}
}

void Player::OnCollision() {
	if (HP_ > 0) {
		HP_--;
	} else {
		isDead_ = true;
	}
}
