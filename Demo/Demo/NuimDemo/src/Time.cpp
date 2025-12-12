#include "NuimDemoPCH.h"
#include "Time.hpp"

namespace NuimDemo {
	Time::clock::time_point Time::s_prevTime;

	Time::clock::time_point Time::s_startTime;

	float Time::s_deltaTime = 0.0f;

	float Time::s_totalTime = 0.0f;

	void Time::Init()
	{
		s_startTime = clock::now();
		s_prevTime = s_startTime;

		s_deltaTime = 0.0f;
		s_totalTime = 0.0f;
	}

	void Time::Tick()
	{
		clock::time_point now = clock::now();

		std::chrono::duration<float> delta = now - s_prevTime;
		s_deltaTime = delta.count();

		s_prevTime = now;

		std::chrono::duration<float> total = now - s_startTime;
		s_totalTime = total.count();
	}


}


