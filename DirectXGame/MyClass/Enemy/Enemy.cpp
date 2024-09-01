#include "Enemy.h"
#ifdef _DEBUG

#include <imgui.h>

#endif // _DEBUG
#include <assert.h>
#include <MyClass/math/mathFunc.h>
#include <MyClass/math/Matrix4x4Func.h>
#include "MyClass/Player/Player.h"
#include "MyClass/math/operatorOverload.h"
#include "GameScene.h"

void Enemy::Initialize(const std::vector<Model*>& models) {
	assert(models[0]);
	BaseCharacter::Initialize(models);
	worldTransform_.translation_ = {0.0f, 2, 30.0f};
	worldTransform_.rotation_ = {0, 3.3f, 0};
	worldTransformBody_.Initialize();
	worldTransformBody_.translation_ = {0.0f, -2, 0.0f};
	worldTransformBody_.scale_ = {15, 15, 15};
	worldTransformL_arm_.Initialize();
	worldTransformL_arm_.translation_ = {-0.4f, 0.25f, 0.f};
	worldTransformR_arm_.Initialize();
	worldTransformR_arm_.translation_ = {0.4f, 0.25f, 0.f};
	worldTransformBody_.parent_ = &worldTransform_;
	worldTransformL_arm_.parent_ = &worldTransformBody_;
	worldTransformR_arm_.parent_ = &worldTransformBody_;
	InitializeArmGimmick();
	SetRadius(radius_);
	SetCollisionAttribute(kCollisionAttributeEnemy);
	SetCollisionMask((kCollisionAttributeEnemy + kCollisionAttributeEnemyBullet) ^ 0x00000000);
	HP_ = kEnemyMaxHP;
	isDead_ = false;

	textureName = TextureManager::Load("./Resources/bossName.png");
	spriteName_.reset(Sprite::Create(textureName, {350, 30}, {1.0f, 1.0f, 1.0f, 1.0f}, {0, 0}));

	textureHP = TextureManager::Load("./Resources/bossHP.png");
	spriteHP_.reset(Sprite::Create(textureHP, {355, 99}, {1.0f, 1.0f, 1.0f, 1.0f}, {0, 0}));
}

void Enemy::SetBulletModel(Model* model) { model_ = model; }

void Enemy::InitializeArmGimmick() { floatingParamater_ = 0.0f; }

void Enemy::Reset() { 
	worldTransform_.translation_ = {0.0f, 2, 30.0f};
	HP_ = kEnemyMaxHP;
	isDead_ = false;
	InitializeArmGimmick();
}

void Enemy::Update() {

	if (!isDead_) {
		UpdateArmGimmick();
		Turning();
		if (bulletInterval_ <= 0) {
			Fire();
			bulletInterval_ = kBulletInterval;
		} else {
			bulletInterval_--;
		}
		/*const float kEnemySpeed = 0.13f;
		Vector3 velocity(0, 0, kEnemySpeed);
		velocity = TransformNormal(velocity, worldTransform_.matWorld_);
		worldTransform_.rotation_.y += 0.02f;
		worldTransform_.translation_ += velocity;*/
		BaseCharacter::Update();
		worldTransformBody_.UpdateMatrix();
		worldTransformL_arm_.UpdateMatrix();
		worldTransformR_arm_.UpdateMatrix();
	}

#ifdef _DEBUG

	ImGui::Begin("Enemy");
	ImGui::DragFloat3("L", &worldTransformL_arm_.translation_.x, 0.01f);
	ImGui::DragFloat3("R", &worldTransformR_arm_.translation_.x, 0.01f);
	ImGui::DragFloat("Ro", &worldTransform_.rotation_.y, 0.01f);
	ImGui::End();

#endif // _DEBUG
}

void Enemy::Draw(const ViewProjection& viewProjection) {
	if (!isDead_) {
		models_[kEnemyBody]->Draw(worldTransformBody_, viewProjection);
		models_[kEnemyL_arm]->Draw(worldTransformL_arm_, viewProjection);
		models_[kEnemyR_arm]->Draw(worldTransformR_arm_, viewProjection);
	}
}

void Enemy::DrawUI() { 
	spriteName_->Draw();

	spriteHP_->SetTextureRect({0, 0}, {502.0f * (float(HP_) / float(kEnemyMaxHP)), 22.0f});
	spriteHP_->SetSize({502.0f * (float(HP_) / float(kEnemyMaxHP)), 22.0f
});
	spriteHP_->Draw();
}

void Enemy::UpdateArmGimmick() {
	// 浮遊移動のサイクル<frame>
	const uint16_t cycle = 150;
	// 1フレームでのパラメーター加算値
	const float step = 2.0f * float(M_PI) / cycle;
	// 1step加算
	floatingParamater_ += step;
	//
	floatingParamater_ = std::fmod(floatingParamater_, 2.0f * float(M_PI));

#ifdef _DEBUG

	ImGui::Begin("enemy");
	ImGui::DragFloat3("L", &worldTransformL_arm_.translation_.x, 0.01f);
	ImGui::DragFloat3("R", &worldTransformR_arm_.translation_.x, 0.01f);
	ImGui::End();

#endif // _DEBUG

	const float amplitudeArm = -0.1f;
	worldTransformL_arm_.translation_.y = std::sin(floatingParamater_) * amplitudeArm + kOffsetArmHeight;
	worldTransformR_arm_.translation_.y = -(std::sin(floatingParamater_) * amplitudeArm) + kOffsetArmHeight;
}

void Enemy::Turning() { 
	Vector3 lockOnPos = player_->GetWorldPosition();
	Vector3 sub = lockOnPos - GetWorldPosition();
	worldTransform_.rotation_.y = std::atan2(sub.x, sub.z);
}

Vector3 Enemy::GetOffsetPosition() const{
	const Vector3 offset = {0, 1, 0};
	Vector3 worldPos = Transform(offset, worldTransform_.matWorld_);
	return worldPos;
}

void Enemy::OnCollision() { 
	if (HP_ > 0) {
		HP_--;
	} else {
		isDead_ = true;
	}
}

void Enemy::Fire() {
	// assert(player_);
	const float kBulletSpeed = 1.0f;
	Vector3 velocity{};
	if (player_ == nullptr) {
		Vector3 enemyToPlayer = {0, 0, -1};
		Vector3 nomalize = Normalize(enemyToPlayer);
		velocity = nomalize * kBulletSpeed;
	} else {
		Vector3 playerPos = player_->GetWorldPosition();
		Vector3 enemyPos = GetWorldPosition();
		Vector3 enemyToPlayer = playerPos - enemyPos;
		Vector3 nomalize = Normalize(enemyToPlayer);
		velocity = nomalize * kBulletSpeed;
	}

	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);
	newBullet->SetPlayer(player_);

	gameScene_->AddEnemyBullet(newBullet);
}
