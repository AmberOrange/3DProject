#ifndef TIMER_HPP
#define TIMER_HPP

#include <d3d11.h>

class Timer
{
public:
	Timer();
	Timer(const Timer &other);
	virtual ~Timer();

	void StartTimer();
	double GetTime();
	double GetFrameTime();

private:
	double countsPerSecond;
	__int64 CounterStart;

	int frameCount;
	int fps;

	__int64 frameTimeOld;
	double frameTime;
};

#endif