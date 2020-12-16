#pragma once


namespace Eloquent {
    namespace Vision {
        namespace IO {

            /**
             * Write jpeg image to disk
             */
            template<uint16_t width, uint16_t height>
            class JpegWriter {
            public:

                /**
                 *
                 * @param file
                 * @param image
                 */
                bool writeGrayscale(File& file, uint8_t *image, uint8_t quality = 30) {
                    uint8_t *jpeg;
                    size_t len;

                    if (fmt2jpg(image, width * height, width, height, PIXFORMAT_GRAYSCALE, quality, &jpeg, &len)) {
                        file.write(jpeg, len);

                        return true;
                    }

                    return false;
                }
            };
        }
    }
}