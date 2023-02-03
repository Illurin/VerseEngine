#define STB_IMAGE_IMPLEMENTATION
#include "stb_loader.hpp"

namespace engine {
	namespace asset {

		StbImageLoader::StbImageLoader(const char* path) {
			int channelInFile;
			source = stbi_load(path, &width, &height, &channelInFile, 4);
		}

		StbImageLoader::~StbImageLoader() {
			stbi_image_free(source);
		}

	}
}