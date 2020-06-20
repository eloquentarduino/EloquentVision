#pragma once

namespace Eloquent {
    namespace Vision {
        namespace ImageProcessing {
            namespace Downscale {

                /**
                 * Downscaler function prototype
                 */
                typedef uint8_t (*ImageDownscaler)(uint8_t *source, uint16_t sourceWidth, uint16_t sourceHeight,
                                                   uint16_t sourceX, uint16_t sourceY, uint8_t blockSize);


                /**
                 * Nearest neighbor (central pixel only)
                 * @param source
                 * @param sourceWidth
                 * @param sourceHeight
                 * @param sourceX
                 * @param sourceY
                 * @param blockSize
                 * @return
                 */
                uint8_t nearest(uint8_t *source, uint16_t sourceWidth, uint16_t sourceHeight, uint16_t sourceX,
                                uint16_t sourceY, uint8_t blockSize) {
                    const uint16_t x = sourceX + blockSize / 2;
                    const uint16_t y = sourceY + blockSize / 2;

                    return source[y * sourceWidth + x];
                }

                /**
                 * Average all the pixels in the block
                 * @param source
                 * @param sourceWidth
                 * @param sourceHeight
                 * @param sourceX
                 * @param sourceY
                 * @param blockSize
                 * @return
                 */
                uint8_t fullAverage(uint8_t *source, uint16_t sourceWidth, uint16_t sourceHeight, uint16_t sourceX,
                                    uint16_t sourceY, uint8_t blockSize) {
                    uint32_t pixel = 0;

                    for (uint8_t blockY = 0; blockY < blockSize; blockY++) {
                        uint16_t y = sourceY + blockY;

                        for (uint16_t blockX = 0; blockX < blockSize; blockX++) {
                            uint16_t x = sourceX + blockX;

                            pixel += source[y * sourceWidth + x];
                        }
                    }

                    return pixel / blockSize / blockSize;
                }

                /**
                 * Average the innermost pixels
                 * @param source
                 * @param sourceWidth
                 * @param sourceHeight
                 * @param sourceX
                 * @param sourceY
                 * @param blockSize
                 * @return
                 */
                uint8_t coreAverage(uint8_t *source, uint16_t sourceWidth, uint16_t sourceHeight, uint16_t sourceX,
                                    uint16_t sourceY, uint8_t blockSize) {
                    uint32_t pixel = 0;
                    uint16_t count = 0;

                    for (uint8_t blockY = blockSize / 3; blockY < blockSize * 2 / 3; blockY++) {
                        uint16_t y = sourceY + blockY;

                        for (uint16_t blockX = blockSize / 3; blockX < blockSize * 2 / 3; blockX++) {
                            uint16_t x = sourceX + blockX;

                            pixel += source[y * sourceWidth + x];
                            count += 1;
                        }
                    }

                    return pixel / count;
                }


                /**
                 * Average the pixels along the two diagonals
                 * @param source
                 * @param sourceWidth
                 * @param sourceHeight
                 * @param sourceX
                 * @param sourceY
                 * @param blockSize
                 * @return
                 */
                uint8_t diagonalAverage(uint8_t *source, uint16_t sourceWidth, uint16_t sourceHeight, uint16_t sourceX,
                                        uint16_t sourceY, uint8_t blockSize) {
                    uint32_t pixel = 0;
                    uint16_t count = 0;

                    for (uint8_t i = 0; i < blockSize; i++) {
                        uint16_t y = sourceY + i;
                        uint16_t x1 = sourceX + i;
                        uint16_t x2 = sourceX + blockSize - i - 1;

                        pixel += source[y * sourceWidth + x1];
                        pixel += source[y * sourceWidth + x2];
                        count += 1;
                    }

                    return pixel / count;
                }

                /**
                 * Average the pixels in the center column and center row
                 * @param source
                 * @param sourceWidth
                 * @param sourceHeight
                 * @param sourceX
                 * @param sourceY
                 * @param blockSize
                 * @return
                 */
                uint8_t crossAverage(uint8_t *source, uint16_t sourceWidth, uint16_t sourceHeight, uint16_t sourceX,
                                     uint16_t sourceY, uint8_t blockSize) {
                    uint32_t pixel = 0;
                    uint16_t start = sourceY * sourceWidth + sourceX;
                    uint16_t offsetX = start + ((blockSize + 1) / 2);
                    uint16_t offsetY = start + (blockSize / 2) * sourceWidth;

                    for (uint8_t i = 0; i < blockSize; i++) {
                        pixel += source[offsetX + i * sourceWidth];
                        pixel += source[offsetY + i];
                    }

                    return pixel / (2 * blockSize);
                }

                /**
                 *
                 * @param source
                 * @param dest
                 * @param f
                 * @param sourceWidth
                 * @param sourceHeight
                 * @param blockSize
                 */
                void downscaleImage(uint8_t *source, uint8_t *dest, ImageDownscaler f, uint16_t sourceWidth,
                                    uint16_t sourceHeight, uint8_t blockSize) {
                    uint16_t destWidth = sourceWidth / blockSize;
                    uint16_t destHeight = sourceHeight / blockSize;

                    for (uint16_t destY = 0; destY < destHeight; destY++) {
                        uint16_t sourceY = destY * blockSize;

                        for (int destX = 0; destX < destWidth; destX++) {
                            uint16_t sourceX = destX * blockSize;
                            uint32_t pixel = f(source, sourceWidth, sourceHeight, sourceX, sourceY, blockSize);

                            dest[destY * destWidth + destX] = pixel;
                        }
                    }
                }

                void _downscaleForPersonDetection(uint8_t *source, uint8_t *dest, uint16_t sourceWidth, uint16_t sourceHeight) {
                    uint16_t destWidth = 96;
                    uint16_t destHeight = 96;
                    uint8_t blockSizeX = floor(sourceWidth / destWidth);
                    uint8_t blockSizeY = floor(sourceHeight / destHeight);

                    for (uint16_t destY = 0; destY < destHeight; destY++) {
                        uint16_t sourceY = (destY + 0.5) * blockSizeY;

                        for (int destX = 0; destX < destWidth; destX++) {
                            uint16_t sourceX = (destX + 0.5) * blockSizeX;

                            dest[destY * destWidth + destX] = source[sourceY * sourceWidth + sourceX];
                        }
                    }
                }

                void downscaleForPersonDetection(uint8_t *source, uint8_t *dest) {
                    // assumes source is 160 x 120
                    for (int y = 0; y < 96; y++)
                        for (int x = 0; x < 96; x++)
                            dest[y * 96 + x] = source[(y + 12) * 160 + (x + 32)];
                }

            }
        }
    }
}