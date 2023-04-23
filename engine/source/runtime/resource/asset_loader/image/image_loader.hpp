#pragma once

#include "stb_loader.hpp"
#include "runtime/render/interface/rhi.hpp"

namespace engine {
    namespace asset {

        struct RawImage {
            uint32_t width, height, bpp;
            uint64_t rowPitch, size;
            std::vector<uint8_t> source;
        };

        struct RawCubemap {
            uint32_t width, height, bpp;
            uint64_t rowPitch, size;
            uint32_t faceWidth, faceHeight;
            uint64_t faceRowPitch, faceSize;
            std::vector<uint8_t> right;
            std::vector<uint8_t> left;
            std::vector<uint8_t> top;
            std::vector<uint8_t> bottom;
            std::vector<uint8_t> front;
            std::vector<uint8_t> back;
        };

        class ImageLoader {
        public:
            static RawImage Load(std::filesystem::path path, uint32_t bpp) {
                RawImage rawImage;
                
                StbImageLoader loader(path);
                rawImage.width = static_cast<uint32_t>(loader.GetWidth());
                rawImage.height = static_cast<uint32_t>(loader.GetHeight());
                rawImage.bpp = bpp;

                if (rawImage.width <= 0 || rawImage.height <= 0)
                    throw std::runtime_error("Cannot load in the image");

                rawImage.rowPitch = (static_cast<uint64_t>(rawImage.width) * static_cast<uint64_t>(bpp) + 7) / 8;
                rawImage.size = rawImage.rowPitch * static_cast<uint64_t>(rawImage.height);

                rawImage.source.resize(rawImage.size);
                auto source = loader.GetSource();
                std::copy(source, source + rawImage.size, rawImage.source.data());

                return rawImage;
            }

            // Cubemap:
            // 
            //              top(+Y)
            //             
            // left(-X)     front(-Z)    right(+X)    back(+Z)
            //             
            //              bottom(-Y)

            static RawCubemap LoadCubemap(std::filesystem::path path, uint32_t bpp) {
                RawCubemap rawCubemap;

                StbImageLoader loader(path);
                rawCubemap.width = static_cast<uint32_t>(loader.GetWidth());
                rawCubemap.height = static_cast<uint32_t>(loader.GetHeight());
                rawCubemap.bpp = bpp;

                if (rawCubemap.width <= 0 || rawCubemap.height <= 0)
                    throw std::runtime_error("Cannot load in the image");

                rawCubemap.rowPitch = (static_cast<uint64_t>(rawCubemap.width) * static_cast<uint64_t>(bpp) + 7) / 8;
                rawCubemap.size = rawCubemap.rowPitch * static_cast<uint64_t>(rawCubemap.height);

                rawCubemap.faceWidth = rawCubemap.width / 4;
                rawCubemap.faceHeight = rawCubemap.height / 3;
                rawCubemap.faceRowPitch = (static_cast<uint64_t>(rawCubemap.faceWidth) * static_cast<uint64_t>(bpp) + 7) / 8;
                rawCubemap.faceSize = rawCubemap.faceRowPitch * static_cast<uint64_t>(rawCubemap.faceHeight);

                auto source = loader.GetSource();

                rawCubemap.top.resize(rawCubemap.faceSize);
                for (uint32_t i = 0; i < rawCubemap.faceHeight; i++) {
                    uint64_t srcOffset = rawCubemap.rowPitch * static_cast<uint64_t>(i) + rawCubemap.faceRowPitch;
                    uint64_t dstOffset = rawCubemap.faceRowPitch * static_cast<uint64_t>(i);
                    std::copy(source + srcOffset, source + srcOffset + rawCubemap.faceRowPitch, rawCubemap.top.begin() + dstOffset);
                }

                rawCubemap.left.resize(rawCubemap.faceSize);
                for (uint32_t i = 0; i < rawCubemap.faceHeight; i++) {
                    uint64_t srcOffset = rawCubemap.rowPitch * static_cast<uint64_t>(i + rawCubemap.faceHeight);
                    uint64_t dstOffset = rawCubemap.faceRowPitch * static_cast<uint64_t>(i);
                    std::copy(source + srcOffset, source + srcOffset + rawCubemap.faceRowPitch, rawCubemap.left.begin() + dstOffset);
                }

                rawCubemap.front.resize(rawCubemap.faceSize);
                for (uint32_t i = 0; i < rawCubemap.faceHeight; i++) {
                    uint64_t srcOffset = rawCubemap.rowPitch * static_cast<uint64_t>(i + rawCubemap.faceHeight) + rawCubemap.faceRowPitch;
                    uint64_t dstOffset = rawCubemap.faceRowPitch * static_cast<uint64_t>(i);
                    std::copy(source + srcOffset, source + srcOffset + rawCubemap.faceRowPitch, rawCubemap.front.begin() + dstOffset);
                }

                rawCubemap.right.resize(rawCubemap.faceSize);
                for (uint32_t i = 0; i < rawCubemap.faceHeight; i++) {
                    uint64_t srcOffset = rawCubemap.rowPitch * static_cast<uint64_t>(i + rawCubemap.faceHeight) + 2 * rawCubemap.faceRowPitch;
                    uint64_t dstOffset = rawCubemap.faceRowPitch * static_cast<uint64_t>(i);
                    std::copy(source + srcOffset, source + srcOffset + rawCubemap.faceRowPitch, rawCubemap.right.begin() + dstOffset);
                }

                rawCubemap.back.resize(rawCubemap.faceSize);
                for (uint32_t i = 0; i < rawCubemap.faceHeight; i++) {
                    uint64_t srcOffset = rawCubemap.rowPitch * static_cast<uint64_t>(i + rawCubemap.faceHeight) + 3 * rawCubemap.faceRowPitch;
                    uint64_t dstOffset = rawCubemap.faceRowPitch * static_cast<uint64_t>(i);
                    std::copy(source + srcOffset, source + srcOffset + rawCubemap.faceRowPitch, rawCubemap.back.begin() + dstOffset);
                }

                rawCubemap.bottom.resize(rawCubemap.faceSize);
                for (uint32_t i = 0; i < rawCubemap.faceHeight; i++) {
                    uint64_t srcOffset = rawCubemap.rowPitch * static_cast<uint64_t>(i + 2 * rawCubemap.faceHeight) + rawCubemap.faceRowPitch;
                    uint64_t dstOffset = rawCubemap.faceRowPitch * static_cast<uint64_t>(i);
                    std::copy(source + srcOffset, source + srcOffset + rawCubemap.faceRowPitch, rawCubemap.bottom.begin() + dstOffset);
                }

                return rawCubemap;
            }
        };

    }
}