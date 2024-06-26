#include "TimedCall.h"

TimedCall::TimedCall(std::function<void(void)> timerFinish, uint32_t time) { 
	timerFinish_ = timerFinish;
	time_ = time;
}

void TimedCall::Update() { 
	if (isFinished_) {
		return;
	}
	time_--;
	if (time_ <= 0) {
		isFinished_ = true;
		timerFinish_();
	}
}
