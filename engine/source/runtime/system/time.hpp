#pragma once

namespace engine {

	class Time final {
	public:
		static double GetDeltaTime() { return deltaTime; }
		static double GetFps() { return fps; }

		static void SetDeltaTime(double deltaTime) { Time::deltaTime = deltaTime; }
		static void SetFps(double fps) { Time::fps = fps; }

	private:
		static inline double deltaTime{ 0.0 };
		static inline double fps{ 0.0 };
	};

}