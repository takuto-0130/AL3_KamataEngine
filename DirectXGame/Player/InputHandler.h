#pragma once
#include "Command.h"
class InputHandler {
public:
	ICommand* HandleInput();

	void AssinMoveRightCommand2PressKeyD();
	void AssinMoveLeftCommand2PressKeyA();

private:
	ICommand* pressKeyD_;
	ICommand* pressKeyA_;
};
