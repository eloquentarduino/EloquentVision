#pragma once

#include "Downscale.h"

using namespace Eloquent::Vision::ImageProcessing::Downscale;

namespace Eloquent {
    namespace Vision {

        template <uint16_t sourceWidth, uint16_t sourceHeight, uint8_t blockSize>
        class MotionDetection {
        public:
            MotionDetection(ImageDownscaler downscaler) :
                _downscaler(downscaler),
                _threshold(0.2) {

            }

            /**
             * Push new frame to motion detector
             * @param buffer
             */
            void update(uint8_t *buffer) {
                memcpy(_prev, _curr, sourceWidth / blockSize * sourceHeight / blockSize);
                downscaleImage(buffer, _curr, _downscaler, sourceWidth, sourceHeight, blockSize);
            }

            /**
             * Update block diff threshold value
             * @param threshold
             */
            void setDiffThreshold(float threshold) {
                _threshold = threshold;
            }

            /**
             * Get number of blocks
             * @return
             */
            uint16_t blocks() {
                return sourceWidth / blockSize * sourceHeight / blockSize;
            }

            /**
             * Detect how many blocks changed by at least a given threshold
             * @return
             */
            uint16_t detect() {
                const uint16_t destHeight = sourceHeight / blockSize;
                const uint16_t destWidth = sourceWidth / blockSize;
                uint16_t changes = 0;

                for (int y = 0; y < destHeight; y++) {
                    for (int x = 0; x < destWidth; x++) {
                        float current = _curr[y * destWidth + x];
                        float prev = _prev[y * destWidth + x];
                        float delta = abs(current - prev) / (prev > 0 ? prev : 1);

                        if (delta >= _threshold)
                            changes += 1;
                    }
                }

                return changes;
            }

            /**
             * Detect what ratio of blocks changed by at least a given threshold
             * @param threshold
             * @return
             */
            float detectRatio() {
                return (1.0 * detect()) / blocks();
            }


        protected:
            uint8_t _curr[sourceWidth / blockSize * sourceHeight / blockSize] = {0};
            uint8_t _prev[sourceWidth / blockSize * sourceHeight / blockSize] = {0};
            float _threshold;
            ImageDownscaler _downscaler;
        };
    }
}