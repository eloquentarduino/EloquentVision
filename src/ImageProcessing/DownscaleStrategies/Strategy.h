#pragma once

namespace Eloquent {
    namespace Vision {
        namespace ImageProcessing {
            namespace DownscaleStrategies {

                /**
                 * Abstract class for a block reducer
                 * @tparam sourceWidth
                 * @tparam sourceHeight
                 * @tparam destWidth
                 * @tparam destHeight
                 */
                template<uint16_t sourceWidth, uint16_t sourceHeight, uint8_t destWidth, uint8_t destHeight>
                class Strategy {
                public:
                    /**
                     * Computing function
                     * @param block
                     * @return
                     */
                    virtual const uint8_t apply(uint8_t block[sourceHeight / destHeight][sourceWidth / destWidth]) = 0;
                };
            }
        }
    }
}