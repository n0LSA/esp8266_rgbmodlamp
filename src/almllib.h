    // #define FASTLED_INTERRUPT_RETRY_COUNT   (0)                 // default: 2; // Use this to determine how many times FastLED will attempt to re-transmit a frame if interrupted for too long by interrupts
    // #define FASTLED_ESP8266_RAW_PIN_ORDER   

#include "../include/constleds.h"
#include "../include/alml_task.h"
// #include "../include/alml_queue.h"
#include "../include/alml_webserver.h"
#include "../include/fastled/fastled.h"

#include "../include/pixel/MatrixParam.h"
// #include "../include/pixel/NeoPixelEffects.h"
#include "../include/pixel/color_palette.h"
#include "../include/pixel/effectcore.h"
#include "../include/pixel/MatrixParam.h"
#include "../include/display/lamp.h"
#include "../include/pixel/hsvloop.h"
#include "../include/pixel/set.h"

#include "../include/pixeleffect/pixeleffect.h"
#include "../include/pixeleffect/matrixeffect.h"

#include "../include/device/device.h"
#include "../include/device/output.h"

#ifdef ALML_TFT
    #include "../include/tft/tft.h" 
    #include "../include/tft/tfttxt.h"  
#endif
#ifdef ALML_IRTFT
    #include "../include/tft/irtft_device.h"    
#endif
