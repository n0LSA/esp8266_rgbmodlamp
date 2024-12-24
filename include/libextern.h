#ifndef LIBEXTERN_H
  #define LIBEXTERN_H

    #include <Arduino.h>
    #include <ArduinoJson.h>
    #include <Ticker.h>

    #ifdef FILESYSTEM 
      #if defined USE_LITTLEFS
        #if defined(ESP8266)
          #include <LittleFS.h> 
        #elif defined(ESP32)
          #include <FS.h>
          #include <LITTLEFS.h>
        #endif
      #elif defined USE_SPIFFS
        #include <FS.h>
      #endif
    #endif  

  #include "def.h"
  #include "defleds.h"
  #include "constleds.h"
  #include "constlp.h"
  #include "config.h"
  
  #include <arduinoPatternList.h>

  #include <wificonnectevo.h>

  #include <adri_timer.h>

  #include <littlefs_helper.h>


  #include <resizeBuff.h>
  #include <LinkedList.h>

  #if defined(ESP32)
    #include <ESPmDNS.h>
    #include <WiFi.h>
  #elif defined(ESP8266)
    #include <ESP8266WiFi.h>
    #include <ESP8266HTTPClient.h>
  #endif


  #include <WiFiUdp.h>


#endif // _LIBEXTERN_H
