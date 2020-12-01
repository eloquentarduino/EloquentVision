#define CAMERA_MODEL_M5STACK_WIDE

#include <FS.h>
#include <SPIFFS.h>
#include "EloquentVision.h"

#define FRAME_SIZE FRAMESIZE_QVGA
#define SOURCE_WIDTH 320
#define SOURCE_HEIGHT 240
#define CHANNELS 1
#define DEST_WIDTH 32
#define DEST_HEIGHT 24
#define BLOCK_VARIATION_THRESHOLD 0.3
#define MOTION_THRESHOLD 0.2

using namespace Eloquent::Vision;
using namespace Eloquent::Vision::IO;
using namespace Eloquent::Vision::ImageProcessing;
using namespace Eloquent::Vision::ImageProcessing::Downscale;
using namespace Eloquent::Vision::ImageProcessing::DownscaleStrategies;

ESP32Camera camera;
uint8_t resized[DEST_HEIGHT][DEST_WIDTH];
Cross<SOURCE_WIDTH, SOURCE_HEIGHT, DEST_WIDTH, DEST_HEIGHT> crossStrategy;
Downscaler<SOURCE_WIDTH, SOURCE_HEIGHT, CHANNELS, DEST_WIDTH, DEST_HEIGHT> downscaler(&crossStrategy);
MotionDetection<DEST_WIDTH, DEST_HEIGHT> motion;
BitmapWriter<SOURCE_WIDTH, SOURCE_HEIGHT> bitmapWriter;


// debounce prototype (see later)
bool debounceMotion(bool touch = false);


/**
 *
 */
void setup() {
    Serial.begin(115200);
    SPIFFS.begin(true);
    camera.begin(FRAME_SIZE, PIXFORMAT_GRAYSCALE);
    motion.setBlockVariationThreshold(BLOCK_VARIATION_THRESHOLD);
}

/**
 *
 */
void loop() {
    uint32_t start = millis();
    camera_fb_t *frame = camera.capture();

    // resize image and detect motion
    downscaler.downscale(frame->buf, resized);
    motion.update(resized);
    motion.detect();

    // compute FPS
    Serial.print(1000.0f / (millis() - start));
    Serial.println(" fps");

    // on motion detected, save image to filesystem
    if (motion.ratio() > MOTION_THRESHOLD) {
        Serial.println("Motion detected");

        // save image
        if (debounceMotion()) {
            Serial.println("The image will be saved as /capture.bmp");
            File imageFile = SPIFFS.open("/capture.bmp", "wb");

            // take a new pic in the hope it is less affected by the motion noise
            // (you may comment this out if you want)
            delay(500);
            frame = camera.capture();

            // write as bitmap
            // *this will take ~20 seconds for a 320x240 image
            // and take 225 Kb of space*
            bitmapWriter.writeGrayscale(imageFile, frame->buf);
            imageFile.close();
            debounceMotion(true);
        }
    }
}

/**
 * Debounce repeated motion detections
 * @return
 */
bool debounceMotion(bool touch) {
    static uint32_t lastMotion = 0;

    // update last tick
    if (lastMotion == 0 || touch)
        lastMotion = millis();

    // debounce
    if (millis() - lastMotion > 5000) {
        lastMotion = millis();

        return true;
    }

    return false;
}