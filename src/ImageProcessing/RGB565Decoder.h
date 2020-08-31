#pragma once

namespace Eloquent {
    namespace Vision {
        namespace Camera {

            template<uint16_t sourceWidth, uint16_t sourceHeight>
            class RGB565Decoder {
            public:

                /**
                 * Decode RGB565 image into [R, G, B, R, G, B] array
                 * Source format is RRRRRGGGGGGBBBBB
                 * @param buf
                 * @param dest
                 * @param blockSize
                 */
                void decode(uint8_t *buf, uint8_t *dest, uint8_t blockSize) {
                    uint16_t index = 0;
                    const uint8_t halfBlockSize = blockSize / 2;
                    const uint16_t doubleSourceWidth = sourceWidth * 2;
                    const uint8_t area = blockSize * blockSize;
                    const float multiplier5 = 255.0f / 32 / area;
                    const float multiplier6 = 255.0f / 64 / area;

                    for (uint16_t y = 0; y < sourceHeight; y += blockSize) {
                        for (uint16_t x = 0; x < sourceWidth; x += blockSize) {
                            uint16_t R = 0;
                            uint16_t G = 0;
                            uint16_t B = 0;
                            const uint16_t xOffset = x * halfBlockSize;

                            for (uint8_t j = 0; j < blockSize; j++) {
                                uint32_t *rowPixels = (uint32_t *) &buf[(y + j) * doubleSourceWidth + xOffset];

                                for (uint8_t i = 0; i < blockSize; i += 2) {
                                    uint32_t rgb = __builtin_bswap32(*rowPixels++);

                                    R += (rgb & 0b1111100000000000) >> 11;
                                    G += (rgb & 0b0000011111100000) >> 5;
                                    B += (rgb & 0b0000000000011111);
                                }
                            }

                            dest[index++] = multiplier5 * R;
                            dest[index++] = multiplier6 * G;
                            dest[index++] = multiplier5 * B;
                        }
                    }
                }
            };
        }
    }
}