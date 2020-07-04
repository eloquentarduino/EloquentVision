#pragma once

namespace Eloquent {
    namespace Vision {
        namespace Camera {

            template<uint16_t sourceWidth, uint16_t sourceHeight>
            class YUV422Decoder {
            public:

                /**
                 *
                 * @param buf
                 * @param dest
                 * @param blockSize
                 */
                void decode(uint8_t *buf, uint8_t *dest, uint8_t blockSize) {
                    uint16_t index = 0;
                    const uint8_t halfBlockSize = blockSize / 2;
                    const uint16_t doubleSourceWidth = sourceWidth * 2;
                    const uint8_t area = blockSize * blockSize;

                    for (uint16_t y = 0; y < sourceHeight; y += blockSize) {
                        for (uint16_t x = 0; x < sourceWidth; x += blockSize) {
                            uint16_t Y = 0;
                            uint16_t U = 0;
                            uint16_t V = 0;
                            const uint16_t xOffset = x *halfBlockSize;

                            for (uint8_t j = 0; j < blockSize; j++) {
                                uint32_t *rowPixels = (uint32_t *) &buf[(y + j) * doubleSourceWidth + xOffset];

                                for (uint8_t i = 0; i < blockSize; i += 2) {
                                    uint32_t yuyv = __builtin_bswap32(*rowPixels++);

                                    Y += (yuyv & 0xff000000) >> 24;
                                    U += (yuyv & 0xff0000) >> 16;
                                    Y += (yuyv & 0xff00) >> 8;
                                    V += (yuyv & 0xff);
                                }
                            }

                            dest[index + 0] = Y / 2 / area;
                            dest[index + 1] = U / area;
                            dest[index + 2] = V / area;
                            index += 3;
                        }
                    }
                }
            };
        }
    }
}