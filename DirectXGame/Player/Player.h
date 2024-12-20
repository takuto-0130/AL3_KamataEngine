#pragma once
#include <Vector2.h>
#include <Sprite.h>
#include <memory>
class Player {
public:
	void Init();

	void Update();

	void Draw();

	// メンバ関数
	void MoveRight();
	void MoveLeft();

private:
	Vector2 pos_;
	float speed_;
	std::unique_ptr<Sprite> playerSpr_;
};
