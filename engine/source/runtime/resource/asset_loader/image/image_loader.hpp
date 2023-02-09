#pragma once

#include "stb_loader.hpp"
#include "runtime/render/interface/rhi.hpp"

namespace engine {
	namespace asset {

		struct RawImage {
			uint32_t width, height, bpp;
			uint64_t rowPitch;
			uint64_t size;
			std::vector<uint8_t> source;
		};

		class ImageLoader {
		public:
			static RawImage Load(const char* path, uint32_t bpp) {
				RawImage rawImage;

				StbImageLoader loader(path);
				rawImage.width = static_cast<uint32_t>(loader.GetWidth());
				rawImage.height = static_cast<uint32_t>(loader.GetHeight());
				rawImage.bpp = bpp;

				rawImage.rowPitch = (static_cast<uint64_t>(rawImage.width) * static_cast<uint64_t>(bpp) + 7) / 8;
				rawImage.size = rawImage.rowPitch * static_cast<uint64_t>(rawImage.height);

				rawImage.source.resize(rawImage.size);
				auto source = loader.GetSource();
				std::copy(source, source + rawImage.size, rawImage.source.data());

				return rawImage;
			}
		};

	}
}