#ifndef PIXELEFFECT_H
#define PIXELEFFECT_H

  #include "pixeleffectbase.h"

  class EffectFx_comets : public EffectFx_comm {
    boolean _direction = false;
    uint8_t _hue = 80;
    int
      _pixstart,            
      _pixend,              
      _pixrange,            
      _counter;
    public:
      EffectFx_comets(uint8_t mod = 0){_mod = mod;};
      void load(uint8_t) override;
      bool run() override;
      void setupEff(uint8_t sP) override {
        setup(sP);
      }      
      String setDynCtrl(UIControl*_val) override;
  };  

  class EffectFx_bounce : public EffectFx_comm {
    boolean _direction = true;
    uint8_t _hue = 80;
    int
      _pixstart,            
      _pixend,              
      _pixrange,            
      _pixaoe,              
      _counter;
    public:
      EffectFx_bounce(uint8_t mod = 0){_mod = mod;};
      void load(uint8_t) override;
      bool run() override;
      void setupEff(uint8_t sP) override {
        setup(sP);
      }      
      String setDynCtrl(UIControl*_val) override;
  };  

  class EffectFx_ruggle : public EffectFx_comm {
    boolean _direction = true;
    int
      _pixstart,            
      _pixend,              
      _pixrange,            
      _pixaoe,              
      _counter;
    public:
      EffectFx_ruggle(uint8_t mod = 0){_mod = mod;};
      void load(uint8_t) override;
      bool run() override;
      void setupEff(uint8_t sP) override {
        setup(sP);
      }      
      String setDynCtrl(UIControl*_val) override;
  };  

  class EffectFx_rainbowWave : public EffectFx_comm {
    boolean _direction = false;
    int
      _pixstart,            
      _pixend,              
      _pixrange,            
      _pixaoe,              
      _counter;
    public:
      EffectFx_rainbowWave(uint8_t mod = 0){_mod = mod;};
      void load(uint8_t) override;
      bool run() override;
      void setupEff(uint8_t sP) override {
        setup(sP);
      }      
      String setDynCtrl(UIControl*_val) override;
      
  };

  class EffectFx_fire2014 : public EffectFx_comm {
    boolean _direction = false;
    byte * heat = nullptr;
    int
      _pixstart,            
      _pixend,              
      _pixrange;          
    public:
      EffectFx_fire2014(uint8_t mod = 0){_mod = mod;};
      ~EffectFx_fire2014() override;
      void load(uint8_t) override;
      bool run() override;
      void setupEff(uint8_t sP) override {
        setup(sP);
      }      
      String setDynCtrl(UIControl*_val) override;
  };

  class EffectFx_Comet : public EffectFx_comm {
    CRGB _color_fg = CRGB(0,255,0);
    uint8_t _subtype = 1;          // Defines sub type to be used
    boolean _direction = false;
    int
      _pixstart,            
      _pixend,              
      _pixrange,            
      _pixaoe,              
      _counter;
    public:
      EffectFx_Comet(uint8_t mod = 0){_mod = mod;};
      void load(uint8_t) override;
      bool run() override;
      void setupEff(uint8_t sP) override {
        setup(sP);
      }      
      String setDynCtrl(UIControl*_val) override;

  };

  class EffectFx_ArmoireOutsideToInside : public EffectFx_comm {

    public:
      EffectFx_ArmoireOutsideToInside(uint8_t mod = 0){_mod = mod;};
      void load(uint8_t) override;
      bool run() override;
      void setupEff(uint8_t sP) override {
        setup(sP);
      }      
      String setDynCtrl(UIControl*_val) override;

  };

  enum EffectStatus {
    INACTIVE,
    ACTIVE,
    NUM_EFFECTSTATUS
  };
  class EffectFx_glow : public EffectCalc {
    CRGBW       * _ledarray_w = nullptr;  
    CRGB        * _ledarray   = nullptr;  
    uint16_t      _numLeds      = 0;
    uint8_t       _strip_pos;   
    boolean       _isTw;
    uint8_t       _mod = 0;
    uint8_t       _maxSeg = 1 ;


    bool _forward =true;
    bool _reverse =false;
    boolean _reverseStart = false;
    EffectStatus _status;


      CRGB _color_fg = CRGB(0,255,0);
      CRGB _color_bg;
      int
        _pixstart,            // First NeoPixel in range of effect
        _pixend,              // Last NeoPixel in range of effect
        _pixrange,            // Length of effect area
        _pixaoe,              // The length of the effect that takes place within the range
        _pixcurrent,          // Head pixel that indicates current pixel to base effect on
        _counter;
      uint8_t _subtype;          // Defines sub type to be used
      uint8_t _bri = 255;          // Defines sub type to be used
      bool
        _repeat,              // Whether or not the effect loops in area
        _direction;           // Whether or not the effect moves from start to end pixel
      unsigned long
        _lastupdate,          // Last update time, in milliseconds since sys reboot
        _delay;               // Period at which effect should update, in milliseconds
    public:
      EffectFx_glow(uint8_t mod = 0){_mod = mod;};
      void load(uint8_t) override;
      bool run() override;
   
      String setDynCtrl(UIControl*_val) override;

  };   

/*
#if defined(ESP8266) || defined(ESP32)
  #define SPEED_MIN (uint16_t)2
#else
  #define SPEED_MIN (uint16_t)10
#endif
#define SPEED_MAX (uint16_t)65535
uint16_t convertSpeed(uint8_t mcl_speed) {
  //long ws2812_speed = mcl_speed * 256;
  uint16_t ws2812_speed = 61760 * (exp(0.0002336 * mcl_speed) - exp(-0.03181 * mcl_speed));
  ws2812_speed = SPEED_MAX - ws2812_speed;

  if (ws2812_speed < SPEED_MIN) {
    ws2812_speed = SPEED_MIN;
  }
  if (ws2812_speed > SPEED_MAX) {
    ws2812_speed = SPEED_MAX;
  }
  return ws2812_speed;
}

void WS2812FX::setSpeed(uint8_t seg, uint16_t s) {
  _segments[seg].speed = constrain(s, SPEED_MIN, SPEED_MAX);
}
  
*/
/*
  class EffectFx_colorwipe : public EffectFx_comm {
      uint32_t _counter_mode_step;
      uint16_t _speed;
      uint16_t _seg_len;    
      uint16_t _stop;    
      uint16_t _start;    

    public:
      EffectFx_Comet(uint8_t mod = 0){_mod = mod;};
      void load(uint8_t) override;
      bool run() override;
      void setupEff(uint8_t sP) override {
        setup(sP);
      }      
      String setDynCtrl(UIControl*_val) override;

  };

*/

  class EffectFx_colorwipe : public EffectFx_comm {
  private:
      uint32_t _counter_mode_step;
      uint16_t _seg_len;    
      uint16_t _stop;    
      uint16_t _start;         
      unsigned long  _next_time;
      unsigned long _now; 
      uint16_t colorwipe(uint8_t color1, uint8_t color2, bool rev);
  public:
      EffectFx_colorwipe(uint8_t mod = 0){_mod = mod;};
      void load(uint8_t) override;
      bool run() override;
      void setupEff(uint8_t sP) override {
        setup(sP);
      }      
      String setDynCtrl(UIControl*_val) override;
  };

  class EffectFx_aurora : public EffectFx_comm {
  private:
      uint8_t _nb_Wave;
      uint8_t _scale;

      CRGB colors[3];


  public:
      EffectFx_aurora(uint8_t mod = 0){_mod = mod;};
      ~EffectFx_aurora() override;
      void load(uint8_t) override;
      bool run() override;
      void setupEff(uint8_t sP) override {
        setup(sP);
      }      
      String setDynCtrl(UIControl*_val) override;
  };
#endif // PIXELEFFECT_H