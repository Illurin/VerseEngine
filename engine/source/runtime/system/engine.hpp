#pragma once

#include "runtime/system/time.hpp"
#include "runtime/system/platform/platform_info.hpp"
#include "runtime/system/platform/input_system.hpp"
#include "runtime/framework/scene/scene.hpp"
#include "runtime/render/render_device.hpp"
#include "runtime/math/math.hpp"
#include "runtime/resource/asset_loader/image/image_loader.hpp"

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

    private:
        std::unique_ptr<IScene> pScene;
    };

}