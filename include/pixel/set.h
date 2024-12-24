#ifndef SET_H
#define SET_H

  #include "../fastled/base.h"

  enum EFFECTCOLOR_MODS
  {
    EFFECTCOLOR_HUE,
    EFFECTCOLOR_PALETTE,
    EFFECTCOLOR_PALETTEHUE,
    EFFECTCOLOR_HUECC,
    EFFECTCOLOR_NULL,
  };


  enum HUE_MODS { 
    HUE_MOD_NM, 
    HUE_MOD_TIMER, 
    HUE_MOD_PASS,
    HUE_MOD_OFF, 
    HUE_MOD_INDEX, 
    HUE_MOD_CPAl, 
    HUE_MOD_CC,
    HUE_MOD_CCHUE
  };

struct RgbColor {uint8_t R; uint8_t G; uint8_t B; };
struct HsvColor {uint8_t h; uint8_t s; uint8_t v; };

class ledsTools
{
public:
  ledsTools();
  ~ledsTools();
  CRGB splitColor(String Val);
  int* splitColorToInt(String Val);
  RgbColor hsv2rgb (uint8_t h, uint8_t s, uint8_t v, int pix_nbr) ;
  HsvColor rgb2hsv(unsigned char r, unsigned char  g, unsigned char  b);
  uint32_t hsv2_32(uint8_t h, uint8_t s, uint8_t v);
  uint32_t rgb2_32(const RgbColor &c1);
  uint32_t rgb2_32(uint8_t r, uint8_t g, uint8_t b);  
  CRGB rgb32_crgb(uint32_t color);
  CRGBW rgb32_crgbw(uint32_t color);
  CRGB Wheel(byte WheelPos);
  uint8_t WheelPos(int pos);
  CRGB fade2black(uint8_t r, uint8_t  g, uint8_t  b, uint8_t ammount);
  CRGB fade2black(CRGB rgb, uint8_t ammount = 127) ;
};

class ledsSet  {
public:



  static void   fill(CRGBW * array, const struct CHSV &c, int start, int end);
  static void   fill(CRGBW * array, const struct CRGB &c, int start, int end);
  static void   fill(CRGBW * array, const struct RgbColor &c, int start, int end);
  static void   fill(CRGBW * array, const struct CRGBW &c, int start, int end);

  static void   fill(CRGB * array, const struct CHSV &c, int start, int end);
  static void   fill(CRGB * array, const struct CRGB &c, int start, int end);
  static void   fill(CRGB * array, const struct RgbColor &c, int start, int end);

  static void   fill(uint8_t op, CRGB * array, const struct CHSV &c);
  static void   fill(uint8_t op, CRGB * array, const struct CRGB &c);
  static void   fill(uint8_t op, CRGB * array, const struct RgbColor &c); 

  static void   fill(uint8_t op, CRGBW * array, const struct CHSV &c);
  static void   fill(uint8_t op, CRGBW * array, const struct CRGBW &c);
  static void   fill(uint8_t op, CRGBW * array, const struct CRGB &c);
  static void   fill(uint8_t op, CRGBW * array, const struct RgbColor &c);  

  static void   fill(uint8_t op, const struct CHSV &c);
  static void   fill(uint8_t op, const struct CRGBW &c);
  static void   fill(uint8_t op, const struct CRGB &c);
  static void   fill(uint8_t op, const struct RgbColor &c); 
};


uint32_t getPixelColor(CRGBW *leds, uint16_t index);
uint32_t getPixelColor(CRGB *leds, uint16_t index);
void led_set_hsv_(CRGB *leds, int led_nbr, int nb_leds , int pixel_offset, uint8_t hue, uint8_t sat, uint8_t val, uint8_t fade  );
void led_set_hsv(CRGBW *leds, int led_nbr, int nb_leds , int pixel_offset, uint8_t hue, uint8_t sat, uint8_t val, uint8_t fade  );
uint8_t _map8(uint8_t val, uint8_t min, uint8_t max);
uint8_t sawtooth_wave(uint8_t bpm);
int rotate (uint8_t speed, uint16_t nb_leds);
#endif
