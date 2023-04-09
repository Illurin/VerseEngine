#define STB_IMAGE_IMPLEMENTATION
#include "stb_loader.hpp"

namespace engine {
	namespace asset {

		StbImageLoader::StbImageLoader(std::filesystem::path path) {
			int channelInFile;
			source = stbi_load(path.string().c_str(), &width, &height, &channelInFile, 4);
		}

		StbImageLoader::~StbImageLoader() {
			stbi_image_free(source);
		}

	}
}