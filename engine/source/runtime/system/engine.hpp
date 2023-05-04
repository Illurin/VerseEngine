#pragma once

#include "runtime/system/time.hpp"
#include "platform/platform_info.hpp"

namespace engine {

    class Engine {
    public:
        explicit Engine(const char* directory);
        ~Engine() = default;

        void Start(const PlatformInfo& platformInfo);
        void Run();
        void Exit();

    private:
        void Tick();
        
        void CalculateDeltaTime();
        void CalculateFps();

    private:
        std::chrono::time_point<std::chrono::steady_clock> timePoint;
    };

}