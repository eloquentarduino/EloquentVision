#pragma once

#ifdef ESP32
#include "camera/ESP32Camera.h"
#endif

#include "ImageProcessing/Downscale.h"
#include "ImageProcessing/MotionDetection.h"
#include "ImageProcessing/YUV422Decoder.h"
#include "ImageProcessing/RGB565Decoder.h"
//#include "Bitmap.h"