#pragma once

#include <esp_http_server.h>
#include "ESP32Camera.h"


namespace Eloquent {
    namespace Vision {

        class ESP32CameraHTTPVideoStreaming {
        public:
            ESP32CameraHTTPVideoStreaming(ESP32Camera& camera) :
                _camera(camera),
                _isPlaying(false) {

            }

            bool play() {
                bool error = false;

                if (_isPlaying) {

                }
                else {
                    error = httpd_start(&_httpd, &config);
                }
            }

        protected:
            bool _isPlaying;
            httpd_handle_t _httpd;
            ESP32Camera _camera;
        };
    }
}