                    // FASTLED_ESP8266_RAW_PIN_ORDER, FASTLED_ESP8266_D1_PIN_ORDER or FASTLED_ESP8266_NODEMCU_PIN_ORDER

#ifndef DEF_H
#define DEF_H

  static const char ALML_DEBUGREGION_REMOTE     [] PROGMEM = "alml remote";
  static const char ALML_DEBUGREGION_TASK       [] PROGMEM = "alml task";
  static const char ALML_DEBUGREGION_WEBSERVER  [] PROGMEM = "alml webserver";
  static const char ALML_DEBUGREGION_EFFECT     [] PROGMEM = "alml effect";
  static const char ALML_DEBUGREGION_PIXELEFFECT  [] PROGMEM = "alml pixeleffect";

  #define ALML_TFT_CS    D2     // TFT CS  pin is connected to NodeMCU pin D2
  #define ALML_TFT_RST   D3     // TFT RST pin is connected to NodeMCU pin D3
  #define ALML_TFT_DC    D4     // TFT DC  pin is connected to NodeMCU pin D4
#endif // DEF_H