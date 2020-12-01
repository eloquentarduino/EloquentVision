#pragma once

#include "Strategy.h"

namespace Eloquent {
    namespace Vision {
        namespace ImageProcessing {
            namespace DownscaleStrategies {

                /**
                 * Return the center pixel
                 * @tparam sourceWidth
                 * @tparam sourceHeight
                 * @tparam destWidth
                 * @tparam destHeight
                 */
                template <uint16_t sourceWidth, uint16_t sourceHeight, uint8_t destWidth, uint8_t destHeight>
                class Center : public Strategy<sourceWidth, sourceHeight, destWidth, destHeight> {
                public:
                    /**
                     * Computing function
                     * @param block
                     * @return
                     */
                    const uint8_t apply(uint8_t block[sourceHeight / destHeight][sourceWidth / destWidth]) {
                        return block[sourceHeight / destHeight / 2][sourceWidth / destWidth / 2];
                    }
                };
            }
        }
    }
}