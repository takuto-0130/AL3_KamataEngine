#pragma once
#include "MyClass/BaseCharacter/BaseCharacter.h"
#include <./MyClass/Enemy/EnemyBullet.h>
#include "Sprite.h"
#include <list>

const int kEnemyMaxHP = 130;

static const int kBulletInterval = 18;

class Player;
class GameScene;

class Enemy : public BaseCharacter {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="models">モデルデータ配列</param>
	void Initialize(const std::vector<Model*>& models) override;

	void SetBulletModel(Model* model);

	void InitializeArmGimmick();

	void Reset();

	void SetPlayer(Player* player) { player_ = player; }

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const ViewProjection& viewProjection) override;

	void DrawUI();

	void UpdateArmGimmick();

	void Turning();

	Vector3 GetOffsetPosition() const;

	void OnCollision() override;

	bool IsDead() const { return isDead_; }

	void Fire();

	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

	Vector3 GetLArmPosition() const { return Vector3{worldTransformL_arm_.matWorld_.m[3][0], worldTransformL_arm_.matWorld_.m[3][1], worldTransformL_arm_.matWorld_.m[3][2]}; }

	Vector3 GetRArmPosition() const { return Vector3{worldTransformR_arm_.matWorld_.m[3][0], worldTransformR_arm_.matWorld_.m[3][1], worldTransformR_arm_.matWorld_.m[3][2]}; }

private:
	GameScene* gameScene_ = nullptr;

	WorldTransform worldTransformBody_;
	WorldTransform worldTransformL_arm_;
	WorldTransform worldTransformR_arm_;

	float floatingParamater_ = 0.0f;

	float kOffsetArmHeight = 0.25f;

	const float radius_ = 3.3f;

	int HP_;
	bool isDead_ = false;
	Player* player_ = nullptr;

	int32_t bulletInterval_ = 0;
	Model* model_ = nullptr;
	std::unique_ptr<Sprite> spriteName_;
	uint32_t textureName;

	std::unique_ptr<Sprite> spriteHP_;
	uint32_t textureHP;
};

enum EnemyParts { 
	kEnemyBody, 
	kEnemyL_arm, 
	kEnemyR_arm };
