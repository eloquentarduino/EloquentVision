#pragma once


namespace Eloquent {
    namespace Vision {
        namespace IO {

            /**
             * Write bitmap image to disk
             * @from https://stackoverflow.com/questions/2654480/writing-bmp-image-in-pure-c-c-without-other-libraries
             */
            template<uint16_t width, uint16_t height>
            class BitmapWriter {
            public:

                /**
                 *
                 * @param file
                 * @param image
                 */
                void writeGrayscale(File& file, uint8_t *image) {
                    writeHeader(file);

                    for (uint16_t i = height - 1; ; i--) {
                        const uint16_t y = i * width;

                        for (int x = 0; x < width; x++) {
                            file.write(image + y + x, 1);
                            file.write(image + y + x, 1);
                            file.write(image + y + x, 1);
                        }

                        // post-condition to prevent i overflow
                        if (i == 0)
                            break;
                    }
                }

                /**
                 *
                 * @param file
                 * @param image
                 */
                void writeRGB888(File& file, uint8_t *image) {
                    writeHeader(file);

                    for (int i = 0; i < height; i++) {
                        const uint32_t y = i * width * 3;

                        file.write(image + y, width * 3);
                    }
                }

                /**
                 *
                 * @param file
                 * @param image
                 */
                void writeRGB565(File& file, uint8_t *image) {
                    uint8_t hb, lb;

                    writeHeader(file);

                    // @from https://github.com/espressif/esp32-camera/blob/master/conversions/to_bmp.c#L279
                    for(uint16_t i = 0; i < width * height; i++) {
                        hb = *image++;
                        lb = *image++;

                        file.write((lb & 0x1F) << 3);
                        file.write((hb & 0x07) << 5 | (lb & 0xE0) >> 3);
                        file.write(hb & 0xF8);
                    }
                }

            protected:
                /**
                 * Get size of padding per row
                 * @return
                 */
                uint8_t paddingSize() {
                    return (4 - ((width * 3) % 4)) % 4;
                }

                /**
                 * Write Bitmap header
                 */
                void writeHeader(File& file) {
                    unsigned char fileHeader[14] = {'B', 'M', 0, 0, 0, 0, 0, 0, 0, 0, 54, 0, 0, 0};
                    unsigned char imageHeader[40] = {40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 24, 0};
                    uint32_t filesize = 54 + width * height * 3;

                    fileHeader[2] = (unsigned char) (filesize);
                    fileHeader[3] = (unsigned char) (filesize >> 8);
                    fileHeader[4] = (unsigned char) (filesize >> 16);
                    fileHeader[5] = (unsigned char) (filesize >> 24);

                    imageHeader[4] = (unsigned char) (width);
                    imageHeader[5] = (unsigned char) (width >> 8);
                    imageHeader[6] = (unsigned char) (width >> 16);
                    imageHeader[7] = (unsigned char) (width >> 24);
                    imageHeader[8] = (unsigned char) (height);
                    imageHeader[9] = (unsigned char) (height >> 8);
                    imageHeader[10] = (unsigned char) (height >> 16);
                    imageHeader[11] = (unsigned char) (height >> 24);

                    file.write(fileHeader, 14);
                    file.write(imageHeader, 40);
                }
            };
        }
    }
}