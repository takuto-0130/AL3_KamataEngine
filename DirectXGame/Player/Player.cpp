#include "Player.h"
#include "TextureManager.h"

void Player::Init() { 
	pos_ = {60.0f, 60.0f};
	speed_ = 3.0f;

	uint32_t tex = TextureManager::GetInstance()->Load("white1x1.png");
	//playerSpr_ = std::make_unique<Sprite>();
	playerSpr_.reset(Sprite::Create(tex, pos_));
	playerSpr_->SetSize({30.0f, 30.0f});
}

void Player::Update() { playerSpr_->SetPosition(pos_); }

void Player::Draw() { playerSpr_->Draw(); }

void Player::MoveRight() { pos_.x += speed_; }

void Player::MoveLeft() { pos_.x -= speed_; }
