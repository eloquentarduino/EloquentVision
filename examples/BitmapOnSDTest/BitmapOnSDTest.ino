#define CAMERA_MODEL_M5STACK_WIDE
#include "FS.h"
#include "SPIFFS.h"
#include "EloquentVision.h"

#define FRAME_SIZE FRAMESIZE_QVGA
#define SOURCE_WIDTH 320
#define SOURCE_HEIGHT 240


using namespace Eloquent::Vision;

ESP32Camera camera;


/**
 *
 */
void setup() {
    Serial.begin(115200);
    SPIFFS.begin(true);
    camera.begin(FRAME_SIZE, PIXFORMAT_GRAYSCALE);
    delay(3000);
}

/**
 *
 */
void loop() {
    uint8_t *rawImage = camera.capture()->buf;
    File file = SPIFFS.open("/bitmap.bmp", "wb");

    if (!file) Serial.println("Not open");

    Bitmap bitmap(file);

    bitmap.convertGrayscale(rawImage, SOURCE_WIDTH, SOURCE_HEIGHT);
    file.close();

    file = SPIFFS.open("/bitmap.bmp", "r");
    Serial.print("Size: ");
    Serial.println(file.size());
    file.close();

    delay(30000);
}