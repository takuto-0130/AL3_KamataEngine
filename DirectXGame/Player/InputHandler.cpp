#include "InputHandler.h"
#include "Input.h"

ICommand* InputHandler::HandleInput() {
	if (Input::GetInstance()->PushKey(DIK_D)) {
		return pressKeyD_;
	}
	if (Input::GetInstance()->PushKey(DIK_A)) {
		return pressKeyA_;
	}
	return nullptr;
}

void InputHandler::AssinMoveRightCommand2PressKeyD() {
	ICommand* command = new MoveRightCommand();
	pressKeyD_ = command;
}

void InputHandler::AssinMoveLeftCommand2PressKeyA() {
	ICommand* command = new MoveLeftCommand();
	pressKeyA_ = command;
}
