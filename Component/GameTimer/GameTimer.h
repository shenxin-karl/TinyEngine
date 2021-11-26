#pragma once
#include <chrono>

namespace com {

namespace chrono = std::chrono;
class GameTimer {
public:
	GameTimer();
	void reset();
	void start();
	void stop();
	void tick();
	float totalTime() const;
	float deltaTime() const;
	std::uint32_t FPS() const;
	float mspf() const;
	bool oneSecondTrigger() const;
private:
	chrono::steady_clock::time_point baseTime_;
	chrono::steady_clock::time_point prevTime_;
	chrono::steady_clock::time_point stoppedTime_;
	float	      deltaTime_;
	float	      pausedTime_;
	std::uint32_t prevFrameTimes_;
	std::uint32_t currFameTimes_;
	std::time_t	  nextTime_;
	bool		  stopped_;
	bool		  newSeconds_;
};

}