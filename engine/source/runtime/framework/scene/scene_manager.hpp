#pragma once

#include "scene.hpp"

namespace engine {

    class SceneManager {
    public:
        void Entry();

        template<class Type>
        void LoadScene() {
            pCurrentScene = std::make_unique<Type>();
            pCurrentScene->Start();
        }

        void Update() {
            pCurrentScene->Update();
        }

    private:
        std::unique_ptr<IScene> pCurrentScene;
    };

}