#pragma once
#include <chrono>

namespace NuimDemo {
	class Time
	{
	public:
		static void Init();

		static void Tick();

		static float GetDeltaTime() { return s_deltaTime; }

		static float GetTotalTime() { return s_totalTime; }

	private:
		using clock = std::chrono::steady_clock;
		static clock::time_point s_prevTime;
		static clock::time_point s_startTime;
		
		static float s_deltaTime;
		static float s_totalTime;
	};
}



