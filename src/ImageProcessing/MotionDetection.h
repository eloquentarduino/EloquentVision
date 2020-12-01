#pragma once

#include "Downscale.h"

using namespace Eloquent::Vision::ImageProcessing::Downscale;

namespace Eloquent {
    namespace Vision {

        /**
         * Eloquent class to detect motion in a stream of images
         *
         * @tparam sourceWidth width of the source image
         * @tparam sourceHeight height of the source image
         */
        template <uint16_t sourceWidth, uint16_t sourceHeight>
        class MotionDetection {
        public:
            /**
             *
             * @param smooth transition smoothing factor
             */
            MotionDetection(float smooth = 0.25) :
                _smooth(smooth),
                _threshold(0.2) {

                const uint8_t zeros[sourceWidth] = {0};

                for (uint16_t y = 0; y < sourceHeight; y++) {
                    memcpy(_curr[y], zeros, sourceWidth);
                    memcpy(_prev[y], zeros, sourceWidth);
                }
            }

            /**
             * Push new frame to motion detector
             * @param image
             */
            void update(uint8_t image[sourceHeight][sourceWidth]) {
                for (uint16_t y = 0; y < sourceHeight; y++) {
                    for (uint16_t x = 0; x < sourceWidth; x++) {
                        // smoothed transition
                        _prev[y][x] = (1 - _smooth) * _curr[y][x] + _smooth * _prev[y][x];
                        _curr[y][x] = image[y][x];
                    }
                }
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
             * Get number of pixels
             * @return
             */
            inline uint16_t pixels() {
                return sourceWidth * sourceHeight;
            }

            /**
             * Detect how many pixels changed by at least a given threshold
             * @return
             */
            uint16_t detect() {
                uint16_t changes = 0;

                // relative threshold
                if (_threshold < 1) {
                    for (uint16_t y = 0; y < sourceHeight; y++) {
                        for (uint16_t x = 0; x < sourceWidth; x++) {
                            float current = _curr[y][x];
                            float prev = _prev[y][x];
                            float delta = abs(current - prev) / (prev > 0 ? prev : 1);

                            if (delta >= _threshold)
                                changes += 1;
                        }
                    }
                }
                else {
                    // absolute threshold
                    for (uint16_t y = 0; y < sourceHeight; y++) {
                        for (uint16_t x = 0; x < sourceWidth; x++) {
                            float current = _curr[y][x];
                            float prev = _prev[y][x];
                            float delta = abs(current - prev);

                            if (delta >= _threshold)
                                changes += 1;
                        }
                    }
                }

                return (_changes = changes);
            }

            /**
             * Get the ratio of pixels that changed in the last frame
             * @return
             */
            float ratio() {
                return (1.0f * _changes) / pixels();
            }


        protected:
            /**
             * Current frame
             */
            uint8_t _curr[sourceHeight][sourceWidth];

            /**
             * Previous frame
             */
            uint8_t _prev[sourceHeight][sourceWidth];

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
             * Cache number of changes
             */
            uint8_t _changes;
        };
    }
}