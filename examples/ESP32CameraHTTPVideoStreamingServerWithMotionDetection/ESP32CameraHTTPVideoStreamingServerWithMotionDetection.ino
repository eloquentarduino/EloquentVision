// not workings AS-IS, needs refactoring

#define CAMERA_MODEL_M5STACK_WIDE
#include "WiFi.h"
#include "EloquentVision.h"
#include "ESP32CameraHTTPVideoStreamingServer.h"

#define FRAME_SIZE FRAMESIZE_QVGA
#define SOURCE_WIDTH 320
#define SOURCE_HEIGHT 240
#define CHANNELS 1
#define DEST_WIDTH 32
#define DEST_HEIGHT 24
#define BLOCK_VARIATION_THRESHOLD 0.3
#define MOTION_THRESHOLD 0.2


// we're using the Eloquent::Vision namespace a lot!
using namespace Eloquent::Vision;
using namespace Eloquent::Vision::Camera;
using namespace Eloquent::Vision::ImageProcessing;
using namespace Eloquent::Vision::ImageProcessing::Downscale;
using namespace Eloquent::Vision::ImageProcessing::DownscaleStrategies;


ESP32Camera camera;
HTTPVideoStreamingServer server(81);
// the buffer to store the downscaled version of the image
uint8_t resized[DEST_HEIGHT][DEST_WIDTH];
// the downscaler algorithm
// for more details see https://eloquentarduino.github.io/2020/05/easier-faster-pure-video-esp32-cam-motion-detection
Cross<SOURCE_WIDTH, SOURCE_HEIGHT, DEST_WIDTH, DEST_HEIGHT> crossStrategy;
// the downscaler container
Downscaler<SOURCE_WIDTH, SOURCE_HEIGHT, CHANNELS, DEST_WIDTH, DEST_HEIGHT> downscaler(&crossStrategy);
// the motion detection algorithm
MotionDetection<DEST_WIDTH, DEST_HEIGHT> motion;

/**
 *
 */
void setup() {
    Serial.begin(115200);
    WiFi.softAP("ESP32", "12345678");

    camera.begin(FRAMESIZE_QVGA, PIXFORMAT_GRAYSCALE);
    motion.setBlockVariationThreshold(BLOCK_VARIATION_THRESHOLD);

    Serial.print("Camera Ready! Use 'http://");
    Serial.print(WiFi.softAPIP());
    Serial.println(":81' to stream");
}

void loop() {
    camera_fb_t *frame = camera.capture();

    // resize image and detect motion
    downscaler.downscale(frame->buf, resized);
    motion.update(resized);
    motion.detect();

    if (motion.ratio() > MOTION_THRESHOLD) {
        Serial.print("Motion detected");
        // start the streaming server when motion is detected
        // shutdown after 20 seconds if no one connects
        camera.begin(FRAMESIZE_QVGA, PIXFORMAT_JPEG);
        delay(2000);
        Serial.print("Camera Server ready! Use 'http://");
        Serial.print(WiFi.softAPIP());
        Serial.println(":81' to stream");
        server.start();
        delay(20000);
        server.stop();
        camera.begin(FRAMESIZE_QVGA, PIXFORMAT_GRAYSCALE);
        delay(2000);
    }

    // probably we don't need 30 fps, save some power
    delay(300);
}