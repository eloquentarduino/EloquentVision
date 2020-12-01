#pragma once

#include "Strategy.h"

namespace Eloquent {
    namespace Vision {
        namespace ImageProcessing {
            namespace DownscaleStrategies {

                /**
                 * Return the average of diagonal pixels
                 * @tparam sourceWidth
                 * @tparam sourceHeight
                 * @tparam destWidth
                 * @tparam destHeight
                 */
                template <uint16_t sourceWidth, uint16_t sourceHeight, uint8_t destWidth, uint8_t destHeight>
                class Diagonal : public Strategy<sourceWidth, sourceHeight, destWidth, destHeight> {
                public:

                    /**
                     * Computing function
                     * @param block
                     * @return
                     */
                    const uint8_t apply(uint8_t block[sourceHeight / destHeight][sourceWidth / destWidth]) {
                        const uint8_t blockSizeX = sourceWidth / destWidth;
                        const uint8_t blockSizeY = sourceHeight / destHeight;
                        const uint8_t minSize = min(blockSizeX, blockSizeY);
                        const float stepX = blockSizeX / minSize;
                        const float stepY = blockSizeY / minSize;
                        uint32_t accumulator = 0;
                        uint8_t count = 0;

                        // left diagonal
                        for (float x = 0, y = 0; x < blockSizeX && y < blockSizeY; ) {
                            accumulator += block[(uint8_t) y][(uint8_t) x];
                            count += 1;
                            x += stepX;
                            y += stepY;
                        }

                        // right diagonal
                        for (float x = blockSizeX - 1, y = blockSizeY - 1; x >= 0 && y >= 0; ) {
                            accumulator += block[(uint8_t) y][(uint8_t) x];
                            count += 1;
                            x -= stepX;
                            y -= stepY;
                        }

                        return accumulator / count;
                    }
                };
            }
        }
    }
}