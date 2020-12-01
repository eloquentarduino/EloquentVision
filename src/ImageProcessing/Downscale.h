#pragma once

#include "DownscaleStrategies/Center.h"
#include "DownscaleStrategies/Core.h"
#include "DownscaleStrategies/Cross.h"
#include "DownscaleStrategies/Diagonal.h"

namespace Eloquent {
    namespace Vision {
        namespace ImageProcessing {
            namespace Downscale {

                /**
                 *
                 * @tparam sourceWidth
                 * @tparam sourceHeight
                 * @tparam channels
                 */
                template<uint16_t sourceWidth, uint16_t sourceHeight, uint8_t channels, uint8_t destWidth, uint8_t destHeight>
                class Downscaler {
                public:
                    /**
                     *
                     * @param strategy
                     */
                    Downscaler(DownscaleStrategies::Strategy<sourceWidth, sourceHeight, destWidth, destHeight> *strategy) :
                            _strategy(strategy) {

                    }

                    /**
                     * Downscale image according to the given strategy
                     * @param source
                     * @param dest
                     */
                    void downscale(uint8_t *source, uint8_t dest[destHeight][destWidth]) {
                        const uint8_t blockSizeX = sourceWidth / destWidth;
                        const uint8_t blockSizeY = sourceHeight / destHeight;
                        const uint32_t blockRowsOffset = blockSizeY * sourceWidth;
                        uint32_t offsetY = 0;
                        uint8_t block[blockSizeY][blockSizeX];

                        for (uint16_t destY = 0; destY < destHeight; destY++) {
                            for (uint16_t destX = 0; destX < destWidth; destX++) {
                                uint32_t offset = offsetY + destX * blockSizeX;

                                // fill the block
                                // @todo select channel
                                for (uint8_t y = 0; y < blockSizeY; y++) {
                                    memcpy(block[y], source + offset, blockSizeX);
                                    offset += sourceWidth;
                                }

                                dest[destY][destX] = _strategy->apply(block);
                            }

                            offsetY += blockRowsOffset;
                        }
                    }

                protected:
                    /**
                     * Strategy for downscale
                     */
                    DownscaleStrategies::Strategy<sourceWidth, sourceHeight, destWidth, destHeight> *_strategy;
                };
            }
        }
    }
}