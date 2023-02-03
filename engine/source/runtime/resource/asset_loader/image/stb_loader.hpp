#pragma once

#include "runtime/system/global_config.hpp"

#include "stb/stb_image.h"

namespace engine {
	namespace asset {

		class StbImageLoader {
		public:
			StbImageLoader(const char* path);
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