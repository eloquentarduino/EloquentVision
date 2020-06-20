#define CAMERA_MODEL_M5STACK_WIDE
#include "EloquentVision.h"

#define FRAME_SIZE FRAMESIZE_QVGA
#define SOURCE_WIDTH 320
#define SOURCE_HEIGHT 240
#define BLOCK_SIZE 10
#define BLOCK_DIFF_THRESHOLD 0.2
#define IMAGE_DIFF_THRESHOLD 0.1

using namespace Eloquent::Vision;
using namespace Eloquent::Vision::ImageProcessing;
using namespace Eloquent::Vision::ImageProcessing::Downscale;

ESP32Camera camera;
MotionDetection<SOURCE_WIDTH, SOURCE_HEIGHT, BLOCK_SIZE> motion(nearest);


/**
 *
 */
void setup() {
    Serial.begin(115200);
    camera.begin(FRAME_SIZE, PIXFORMAT_GRAYSCALE);
}

/**
 *
 */
void loop() {
    motion.update(camera.capture()->buf);

    if (motion.detectRatio() > IMAGE_DIFF_THRESHOLD) {
        Serial.print("Motion detected");
    }
}