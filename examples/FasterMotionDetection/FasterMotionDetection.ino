#define CAMERA_MODEL_M5STACK_WIDE
#include "EloquentVision.h"

#define FRAME_SIZE FRAMESIZE_QVGA
#define SOURCE_WIDTH 320
#define SOURCE_HEIGHT 240
#define BLOCK_SIZE 10
#define DEST_WIDTH (SOURCE_WIDTH / BLOCK_SIZE)
#define DEST_HEIGHT (SOURCE_HEIGHT / BLOCK_SIZE)
#define BLOCK_DIFF_THRESHOLD 0.2
#define IMAGE_DIFF_THRESHOLD 0.1
#define DEBUG 0

using namespace Eloquent::Vision;
using namespace Eloquent::Vision::ImageProcessing::Downscale;

ESP32Camera camera;
uint8_t prevFrame[DEST_WIDTH * DEST_HEIGHT] = { 0 };
uint8_t currentFrame[DEST_WIDTH * DEST_HEIGHT] = { 0 };

// function prototypes
bool motionDetect();
void updateFrame();


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
    /**
     * Algorithm:
     *  1. grab frame
     *  2. compare with previous to detect motion
     *  3. update previous frame
     */

    time_t start = millis();
    camera_fb_t *frame = camera.capture();

    downscaleImage(frame->buf, currentFrame, nearest, SOURCE_WIDTH, SOURCE_HEIGHT, BLOCK_SIZE);

    if (motionDetect()) {
        Serial.print("Motion detected @ ");
        Serial.print(floor(1000.0f / (millis() - start)));
        Serial.println(" FPS");
    }

    updateFrame();
}


/**
 * Compute the number of different blocks
 * If there are enough, then motion happened
 */
bool motionDetect() {
    uint16_t changes = 0;
    const uint16_t blocks = DEST_WIDTH * DEST_HEIGHT;

    for (int y = 0; y < DEST_HEIGHT; y++) {
        for (int x = 0; x < DEST_WIDTH; x++) {
            float current = currentFrame[y * DEST_WIDTH + x];
            float prev = prevFrame[y * DEST_WIDTH + x];
            float delta = abs(current - prev) / prev;

            if (delta >= BLOCK_DIFF_THRESHOLD)
                changes += 1;
        }
    }

    return (1.0 * changes / blocks) > IMAGE_DIFF_THRESHOLD;
}


/**
 * Copy current frame to previous
 */
void updateFrame() {
    memcpy(prevFrame, currentFrame, DEST_WIDTH * DEST_HEIGHT);
}