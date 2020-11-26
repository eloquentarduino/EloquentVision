#pragma once

#include "Downscale.h"

using namespace Eloquent::Vision::ImageProcessing::Downscale;

namespace Eloquent {
    namespace Vision {

        /**
         * Eloquent class to detection motion in a stream of images
         *
         * @tparam sourceWidth width of the source image
         * @tparam sourceHeight height of the source image
         * @tparam blockSize size of a (squared) block
         */
        template <uint16_t sourceWidth, uint16_t sourceHeight, uint8_t blockSize>
        class MotionDetection {
        public:
            /**
             *
             * @param downscaler
             */
            MotionDetection(ImageDownscaler downscaler) :
                _downscaler(downscaler),
                _smooth(0.5),
                _threshold(0.2) {

            }

            /**
             * Push new frame to motion detector
             * @param image
             */
            void update(uint8_t *image) {
                for (int i = 0, m = blocks(); i < m; i++)
                    // smoothed transition
                    _prev[i] = (1 - _smooth) * _curr[i] + _smooth * _prev[i];

                downscaleImage(image, _curr, _downscaler, sourceWidth, sourceHeight, blockSize);
            }

            /**
             * Update block diff threshold value
             * @param threshold
             * @deprecated
             */
            void setDiffThreshold(float threshold) {
                _threshold = threshold;
            }

            /**
             * Set frame transition smoothing factor
             * (set to 0 for no smoothing)
             * @param smooth in the range [0, 1[
             */
            void setSmoothingFactor(float smooth) {
                _smooth = smooth >= 1 || smooth <= 0 ? 0 : smooth;
            }

            /**
             * Update block diff threshold value
             * @param threshold
             */
            void setBlockVariationThreshold(float threshold) {
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

                for (uint16_t i = 0, m = blocks(); i < m; i++) {
                    float current = _curr[i];
                    float prev = _prev[i];

                    // relative delta
                    if (_threshold < 1) {
                        float delta = abs(current - prev) / (prev > 0 ? prev : 1);

                        if (delta >= _threshold)
                            changes += 1;
                    }
                    else {
                        // absolute delta
                        float delta = abs(current - prev);

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
                return (1.0f * detect()) / blocks();
            }


        protected:
            /**
             * Current frame (in blocks)
             */
            uint8_t _curr[sourceWidth / blockSize * sourceHeight / blockSize] = {0};
            /**
             * Previous frame (in blocks)
             */
            uint8_t _prev[sourceWidth / blockSize * sourceHeight / blockSize] = {0};
            /**
             * Smoothing factor for transition from a frame to the next
             * 0 means to completely discard the previous frame
             * 1 means no update at all (useless)
             */
            float _smooth;
            /**
             * Threshold to mark a block as "different" from the previous
             *  - if threshold < 1, it is interpreted as relative
             *  - if threshold >= 1, it is interpreted as absolute
             *  - set to 0 to detect every single change (not recommended)
             */
            float _threshold;
            /**
             * Downscaler strategy
             */
            ImageDownscaler _downscaler;
        };
    }
}