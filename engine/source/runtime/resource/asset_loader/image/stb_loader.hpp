#pragma once

#include "stb/stb_image.h"

#include "runtime/system/file_system/path.hpp"

namespace engine {
    namespace asset {

        class StbImageLoader {
        public:
            StbImageLoader(std::filesystem::path path);
            ~StbImageLoader();

            stbi_uc* GetSource() const { return source; }
            int GetWidth() const { return width; }
            int GetHeight() const { return height; }
            
        private:
            int width{ 0 }, height{ 0 };
            stbi_uc* source{ nullptr };
        };

    }
}