#pragma once

namespace Eloquent {
    namespace Vision {

        /**
         * Generate bitmap image from image array
         * @from https://stackoverflow.com/questions/2654480/writing-bmp-image-in-pure-c-c-without-other-libraries
         */
        class Bitmap {
        public:
            /**
             *
             * @param file
             */
            Bitmap(File& file) :
                _file(file) {

            }

            /**
             * Save image as RGB image
             * @param image
             * @param width
             * @param height
             */
            void convertRGB(uint8_t *image, uint16_t width, uint16_t height) {
                // @todo
            }

            /**
             * Save image as grayscale image
             * @param image
             * @param width
             * @param height
             */
            void convertGrayscale(uint8_t *image, uint16_t width, uint16_t height) {
                writeHeader(width, height);

                const uint8_t padding = 4 - ((width * 3) % 4);

                for (uint16_t y = 0; y < height; y++) {
                    const uint16_t offset = y * width;

                    for (uint16_t x = 0; x < width; x++) {
                        // grayscale is actually an RGB image with the same R, G, B
                        const uint8_t gray = image[offset + x];

                        _file.write(gray);
                        _file.write(gray);
                        _file.write(gray);
                    }

                    for (uint8_t i = padding; i > 0; i++)
                        _file.write(0);
                }
            }

        protected:
            File _file;

            /**
             * Write bitmap header
             */
            void writeHeader(uint8_t width, uint8_t height) {
                uint8_t fileHeader[14] = {'B','M', 0,0,0,0, 0,0, 0,0, 54,0,0,0};
                uint8_t infoHeader[40] = {40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0, 24,0};
                uint32_t filesize = 54 + width * height * 3;

                fileHeader[ 2] = (uint8_t)(filesize     );
                fileHeader[ 3] = (uint8_t)(filesize >> 8);
                fileHeader[ 4] = (uint8_t)(filesize >>16);
                fileHeader[ 5] = (uint8_t)(filesize >>24);

                infoHeader[ 4] = (uint8_t)(width      );
                infoHeader[ 5] = (uint8_t)(width >>  8);
                infoHeader[ 6] = (uint8_t)(width >> 16);
                infoHeader[ 7] = (uint8_t)(width >> 24);
                infoHeader[ 8] = (uint8_t)(height      );
                infoHeader[ 9] = (uint8_t)(height >>  8);
                infoHeader[10] = (uint8_t)(height >> 16);
                infoHeader[11] = (uint8_t)(height >> 24);

                for (uint8_t i = 0; i < 14; i++)
                    _file.write(fileHeader[i]);

                for (uint8_t i = 0; i < 40; i++)
                    _file.write(infoHeader[i]);
            }
        };
    }
}