#pragma once

#include "scene.hpp"

namespace engine {

    class SceneManager {
    public:
        static void Entry();

        static void Clear() {
            pCurrentScene.reset();
        }

        static IScene* GetCurrentScene() {
            return pCurrentScene.get();
        }

        template<class Type>
        static void LoadScene() {
            pCurrentScene = std::make_unique<Type>();
            pCurrentScene->Start();
        }

        static void Update() {
            pCurrentScene->Update();
        }

    private:
        inline static std::unique_ptr<IScene> pCurrentScene;
    };

}