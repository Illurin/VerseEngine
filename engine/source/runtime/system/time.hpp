#pragma once

namespace engine {

	class Time final {
	public:
		static double GetDeltaTime() { return deltaTime; }
		static double GetFps() { return fps; }

		static void SetDeltaTime(double deltaTime) { Time::fps = deltaTime; }
		static void SetFps(double fps) { Time::fps = fps; }

	private:
		static double deltaTime;
		static double fps;
	};

}