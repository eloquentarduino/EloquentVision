#define CAMERA_MODEL_M5STACK_WIDE
#include <WiFi.h>
#include "EloquentVision.h"
#include "ESP32CameraHTTPVideoStreamingServer.h"


using namespace Eloquent::Vision;
using namespace Eloquent::Vision::Camera;


ESP32Camera camera;
HTTPVideoStreamingServer server(81);


/**
 *
 */
void setup() {
    Serial.begin(115200);
    WiFi.softAP("ESP32", "12345678");

    camera.begin(FRAMESIZE_QVGA, PIXFORMAT_JPEG);
    server.start();

    Serial.print("Camera Ready! Use 'http://");
    Serial.print(WiFi.softAPIP());
    Serial.println(":81' to stream");
}

void loop() {
}