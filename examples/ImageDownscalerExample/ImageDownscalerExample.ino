#define CAMERA_MODEL_M5STACK_WIDE
#include "EloquentVision.h"

#define benchmark(title, x) { time_t start = micros(); x; Serial.print("It took "); Serial.print(micros() - start); Serial.print(" micros to execute "); Serial.println(title); dump(dest, 32, 24); }


using namespace Eloquent::Vision;
using namespace Eloquent::Vision::ImageProcessing::Downscale;

ESP32Camera camera;


void setup() {
    Serial.begin(115200);
    camera.begin(FRAMESIZE_QVGA, PIXFORMAT_GRAYSCALE);
    delay(2000);
}


void loop() {
    uint8_t dest[32 * 24] = {0};

    camera_fb_t *frame = camera.capture();

    //dump("Original", frame->buf, 320, 240);
    benchmark("Nearest neighbor", downscaleImage(frame->buf, dest, nearest, 320, 240, 10));
    benchmark("Full average", downscaleImage(frame->buf, dest, fullAverage, 320, 240, 10));
    benchmark("Core average", downscaleImage(frame->buf, dest, coreAverage, 320, 240, 10));
    benchmark("Cross average", downscaleImage(frame->buf, dest, crossAverage, 320, 240, 10));
    benchmark("Diagonal average", downscaleImage(frame->buf, dest, diagonalAverage, 320, 240, 10));

    delay(10000);
}

/**
 * Dump image to serial
 * @param title
 * @param image
 * @param width
 * @param height
 */
void dump(uint8_t *image, uint16_t width, uint16_t height) {
    for (uint16_t y = 0; y < height; y++) {
        for (uint16_t x = 0; x < width; x++) {
            Serial.print(image[y * width + x]);
            Serial.print('\t');
        }
        Serial.println();
    }

    Serial.println("------------------------");
}