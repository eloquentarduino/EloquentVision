#pragma once

#include "Strategy.h"

namespace Eloquent {
    namespace Vision {
        namespace ImageProcessing {
            namespace DownscaleStrategies {

                /**
                 * Return the innermost pixels
                 * @tparam sourceWidth
                 * @tparam sourceHeight
                 * @tparam destWidth
                 * @tparam destHeight
                 */
                template <uint16_t sourceWidth, uint16_t sourceHeight, uint8_t destWidth, uint8_t destHeight>
                class Core : public Strategy<sourceWidth, sourceHeight, destWidth, destHeight> {
                public:
                    /**
                     *
                     * @param radius how far for the center to consider pixels
                     */
                    Core(uint8_t radius = 1) :
                            _radius(radius) {

                    }

                    /**
                     * Computing function
                     * @param block
                     * @return
                     */
                    const uint8_t apply(uint8_t block[sourceHeight / destHeight][sourceWidth / destWidth]) {
                        const uint8_t centerX = sourceWidth / destWidth * 0.5;
                        const uint8_t centerY = sourceHeight / destHeight * 0.5;
                        uint16_t accumulator = 0;
                        uint8_t count = 0;

                        for (uint8_t y = max(0, centerY - _radius); y < centerY + _radius; y++) {
                            for (uint8_t x = max(0, centerX - _radius); x < centerX + _radius; x++) {
                                accumulator += block[y][x];
                                count += 1;
                            }
                        }

                        return accumulator / count;
                    }

                protected:
                    uint8_t _radius;
                };
            }
        }
    }
}