#ifndef LEDS_BASE_H
#define LEDS_BASE_H

  #include "../libextern.h"
  #include "../pixel/MatrixParam.h"

/// scale four one byte values by a fith one, which is treated as
///         the numerator of a fraction whose demominator is 256
///         In other words, it computes r,g,b,w * (scale / 256)
///
///         THIS FUNCTION ALWAYS MODIFIES ITS ARGUMENTS IN PLACE

LIB8STATIC void nscale8x4( uint8_t& r, uint8_t& g, uint8_t& b, uint8_t& w, fract8 scale)
{
#if SCALE8_C == 1
#if (FASTLED_SCALE8_FIXED == 1)
  uint16_t scale_fixed = scale + 1;
  r = (((uint16_t)r) * scale_fixed) >> 8;
  g = (((uint16_t)g) * scale_fixed) >> 8;
  b = (((uint16_t)b) * scale_fixed) >> 8;
  w = (((uint16_t)w) * scale_fixed) >> 8;
#else
  r = ((int)r * (int)(scale) ) >> 8;
  g = ((int)g * (int)(scale) ) >> 8;
  b = ((int)b * (int)(scale) ) >> 8;
  w = ((int)w * (int)(scale) ) >> 8;
#endif
#elif SCALE8_AVRASM == 1
  r = scale8_LEAVING_R1_DIRTY(r, scale);
  g = scale8_LEAVING_R1_DIRTY(g, scale);
  b = scale8_LEAVING_R1_DIRTY(b, scale);
  w = scale8_LEAVING_R1_DIRTY(w, scale);
  cleanup_R1();
#else
#error "No implementation for nscale8x3 available."
#endif
}

  struct CRGBW  {
    union {
      struct {
        union {
          uint8_t g;
          uint8_t green;
        };
        union {
          uint8_t r;
          uint8_t red;
        };
        union {
          uint8_t b;
          uint8_t blue;
        };
        union {
          uint8_t w;
          uint8_t white;
        };
      };
      uint8_t raw[4];
    };

    CRGBW(){}

    CRGBW(uint8_t rd, uint8_t grn, uint8_t blu, uint8_t wht){
      r = rd;
      g = grn;
      b = blu;
      w = wht;
    }

    inline void operator = (const CRGB c) __attribute__((always_inline)){ 
      this->r = c.r;
      this->g = c.g;
      this->b = c.b;
      this->white = 0;
    }

    /// scale down a RGBW to N 256ths of it's current brightness, using
    /// 'plain math' dimming rules, which means that if the low light levels
    /// may dim all the way to 100% black.
    inline CRGBW& nscale8 (uint8_t scaledown )
    {
      nscale8x4( r, g, b, w, scaledown);
      return *this;
    }

    /// scale down a RGBW to N 256ths of it's current brightness, using
    /// 'plain math' dimming rules, which means that if the low light levels
    /// may dim all the way to 100% black.
    inline CRGBW& nscale8 (const CRGBW & scaledown )
    {
      r = ::scale8(r, scaledown.r);
      g = ::scale8(g, scaledown.g);
      b = ::scale8(b, scaledown.b);
      w = 0;
      return *this;
    }    
  };

  inline uint16_t getRGBWsize(uint16_t nleds){
    uint16_t nbytes = nleds * 4;
    if(nbytes % 3 > 0) return nbytes / 3 + 1;
    else return nbytes / 3;
  }



  class ledsBase
  { 
  public:

    
    uint16_t      _num_ledsW    = 0;
    uint8_t       _pin          = 0;
    uint8_t       _stripPos     = 0;
    uint16_t      _num_leds     = 0;
    uint8_t       _contructor   = 0;
    boolean       _hackTw       = false;
    CRGB          * _leds       = nullptr;
    CRGB          * _ledsRGB    = nullptr;
    CRGBW         * _ledsW      = nullptr;

    void get_numLedsW( uint16_t & result);
    void set_numLedsW( uint16_t  result);
    void set_numLeds( uint16_t  result);
    void set_ledsIsTw(boolean  result);
    void set_ledsPin(uint8_t  result);
    void set_stripPos(uint8_t  result);  

    CRGB * get_leds();
    CRGBW * get_ledsW();

    void get_ledsPin(uint8_t & result);
    void get_numLeds( uint16_t & result);
    void get_ledsIsTw(boolean & re);
    void get_stripPos(uint8_t & result);

  };



  class leds_setting
  {
    ledsBase      * _myLed;
    MatrixParam   * _MatrixParam;
  public:
    ledsBase * myLed();
    MatrixParam * matrix();
    leds_setting(ledsBase * myLed);
  };  

  

  class leds_manage
  {
    uint8_t _cnt = 0;

    leds_setting * setting(uint8_t pos);

  public:

    leds_manage();
    ~leds_manage();
    void strip_add(ledsBase * myLed);

    MatrixParam * get_matrix(uint8_t pos);

    CRGB * get_crgbArray(uint8_t pos);
    CRGBW * get_crgwbArray(uint8_t pos);

    void get_numLeds(uint8_t pos, uint16_t & result);
    void get_ledsPin(uint8_t pos, uint8_t & val);
    void get_ledsIsTw(uint8_t pos, boolean & val);

    void showLeds(uint8_t pos, uint8_t val);

    uint32_t getPixelNumber(uint8_t pos, uint16_t x, uint16_t y);

    void print();

  };
  leds_manage * leds_managePtrGet();




#endif // LEDS_BASE_H
