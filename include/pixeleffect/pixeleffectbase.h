#ifndef PIXELEFFECTBASE_H
#define PIXELEFFECTBASE_H

  #include "../fastled/base.h"  
  // #include "../pixel/NeoPixelEffects.h"
  #include "../pixel/w2812fx/fx.h"
  #include "../pixel/effectcore.h"
  #include "../device/device.h"

  class EffectFx_neo : public EffectCalc {
  protected:
    OUTPUT_EFFECTTYPE _effectType = OET_HUETIMER;
    OUTPUT_EFFECTTYPE _defaultPal = OET_PAL;

    CRGBPalette16       _gTargetPalette;    
    CRGBPalette16       _cTargetPalette;    

    int                 _pPos         = 0;
    unsigned long       _timer_toward =  0; 
    
    WS2812FX            * _neoFxxPtr  = nullptr;
    CRGBW               * ledarray_w  = nullptr;  
    CRGB                * ledarray    = nullptr;  
    uint16_t            numLeds       = 0;      
    WS2812FX_setting    neoSetting;
    uint8_t             oldBpm        = 0;  
    uint8_t             oldBri        = 0;  
    boolean             statu         = false;  
    boolean             show          = true;
    uint8_t             _strip_pos;   
    uint8_t             _varia        = 60;
    uint8_t             _hue_1        = 0;
    uint8_t             _hue_2        = 0;
    uint8_t             _mod          = 0;
    uint32_t            _timerPalFactor = 1000;
    // 0 c1 = hue_1, c2 = hue_1+varia
    // 1 c1 = heu_1, c2 = hue_2
    uint8_t             _colorMod = 0;

  public:
    ~EffectFx_neo();
    void reset() override;
    virtual void setup( uint8_t strip_pos , OUTPUT_EFFECTTYPE mod = OET_NONE);
    virtual void update();
    virtual void instance();
    virtual void set_dynCtrl(UIControl*_val);
    bool isPalEffect(OUTPUT_EFFECTTYPE value) {
      if (value == OUTPUT_EFFECTTYPE::OET_PAL) return true;
      if (value == OUTPUT_EFFECTTYPE::OET_PALGRAD) return true;
      if (value == OUTPUT_EFFECTTYPE::OET_PALFIRE) return true;
      if (value == OUTPUT_EFFECTTYPE::OET_PALDUO) return true;
      return false;
    }
    bool isPalEffect() override {
      if (_effectType == OUTPUT_EFFECTTYPE::OET_PAL) return true;
      if (_effectType == OUTPUT_EFFECTTYPE::OET_PALGRAD) return true;
      if (_effectType == OUTPUT_EFFECTTYPE::OET_PALFIRE) return true;
      if (_effectType == OUTPUT_EFFECTTYPE::OET_PALDUO) return true;
      return false;
    }  
    OUTPUT_EFFECTTYPE get_effectType() override {return _effectType;}   
  };
  class EffectFx_mode :  public EffectFx_neo {
  public:
    EffectFx_mode(uint8_t mod = 0){_mod = mod;};
    void load(uint8_t) override;
    bool run() override;
    String setDynCtrl(UIControl*_val) override;
    void setupEff(uint8_t sP) override;
  };


  class EffectFx_comm : public EffectCalc {
  protected:
    OUTPUT_EFFECTTYPE _effectType = OET_HUETIMER;
    OUTPUT_EFFECTTYPE _defaultPal = OET_PAL;

    CRGBPalette16       _gTargetPalette;    
    CRGBPalette16       _cTargetPalette;    
    
    uint8_t             _pPos           = 0;
    unsigned long       _timer_toward   =  0; 
    unsigned long       _timer_palette  =  0; 

    CRGBW     * _ledarray_w     = nullptr;  
    CRGB      * _ledarray       = nullptr;  
    uint16_t  _numLeds          = 0;
    uint8_t   _strip_pos        = 0;    
    boolean   _isTw             = false;
    boolean   _isSegment        = false;
    boolean   _isSegmentEnabled = true;
    uint8_t   _mod              = 0;
    uint8_t   _maxSeg           = 1 ;
    boolean   _reverseStart     = true;
    boolean   _active           = false;
    uint32_t  _lastupdate       = 0;
    // uint32_t _speed            = 60;
    uint8_t   _varia            = 60;
    uint8_t   _hue_1            = 0;
    uint8_t   _hue_1V           = 0;
    uint32_t  _hueTimer         = 0;
    // uint8_t    _hue_2            = 0;
    // uint8_t    _hue_2V           = 0;
    uint8_t   _fade             = 0;
    uint8_t   _size             = 0;
    uint8_t   _rep              = 0;
    uint32_t  _timerPalFactor   = 1000;

  public:
    // ~EffectFx_comm(){Serial.println("delete effectFx COM");};
    virtual ~EffectFx_comm() = default; 
    virtual void setup(uint8_t strip_pos);
    boolean dryrun(float n, uint8_t delay = 16) ; 
    boolean dryrun(uint8_t s, float n, uint8_t delay = 16) ;  
    virtual void set_dynCtrl(UIControl*_val);
    virtual void update();
    bool isPalEffect(OUTPUT_EFFECTTYPE value) {
      if (value == OUTPUT_EFFECTTYPE::OET_PAL) return true;
      if (value == OUTPUT_EFFECTTYPE::OET_PALGRAD) return true;
      if (value == OUTPUT_EFFECTTYPE::OET_PALFIRE) return true;
      if (value == OUTPUT_EFFECTTYPE::OET_PALDUO) return true;
      return false;
    }
    bool isPalEffect() override {
      if (_effectType == OUTPUT_EFFECTTYPE::OET_PAL) return true;
      if (_effectType == OUTPUT_EFFECTTYPE::OET_PALGRAD) return true;
      if (_effectType == OUTPUT_EFFECTTYPE::OET_PALFIRE) return true;
      if (_effectType == OUTPUT_EFFECTTYPE::OET_PALDUO) return true;
      return false;
    }  
    OUTPUT_EFFECTTYPE get_effectType() override {return _effectType;}  
    uint8_t getHue_1() override {return _hue_1;}
  };

  void nscale8( CRGBW* leds, uint16_t num_leds, uint8_t scale);
  void fadeToBlackBy( CRGBW* leds, uint16_t num_leds, uint8_t fadeBy);
#endif // PIXELEFFECT_H