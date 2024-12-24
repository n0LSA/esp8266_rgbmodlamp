#ifndef LAMP_H
#define LAMP_H

  #include "../fastled/base.h"  
  #include "../pixel/set.h"  
  #include "../pixel/hsvloop.h"  
  #include "../pixel/effectcore.h"
  #include "effectslist.h"
  #include "remote.h"


  typedef enum _LAMPMODE {
    MODE_NORMAL = 0,
    MODE_DEMO,
    MODE_WHITELAMP,
    MODE_ALARMCLOCK,
    MODE_OTA
  } LAMPMODE;

  typedef enum _SCHEDULER { 
    T_DISABLE = 0,    // 
    T_ENABLE,         // 
    T_FRAME_ENABLE,   // Вкл
    T_RESET,          // 
  } SCHEDULER;

  typedef union {
  struct {
    bool ONflag:1;              // definit si lampe is power on/off ( off = effecticker desactivate) 
    bool isGlobalBrightness:1;  // definit si le slider de reglage effect midfie la BRI GLOBAL ou la BRI PROPRE A LEFFET ( AUCUN REGLAGE POUR LA BRI PAR EFFECT ) 
    bool isEffClearing:1;       // 
    bool isFader:1;           // fadeByLight entre le chgement d"effet
    bool isDebug:1;             //
    bool isRandDemo:1;           // mode demo 
    bool isHueLoop:1;           //  
    bool isPalLoop:1;           //  
    // bool isPalgradLoop:1;       //  
  };
  // uint32_t lampflags; //
  } LAMPFLAGS;

  class LAMP 
  {
    // Output * _output;
    // Effectslist * _Effectslist;
    RemoteControl * _RemoteControl = nullptr;



    boolean isLampShow = false;

    uint8_t   _outPutCount  = 0;

    uint8_t   _steps;

    LAMPMODE    _mode = MODE_NORMAL; 
    LAMPSTATE   _lampState; // 
    LAMPFLAGS   _flags;

    // EffectList  * _effectList = nullptr;

    void frameShow(const uint32_t ticktime);

    void effectsTick();

    uint32_t _effectsTimer_now;
    uint32_t _effectsLoop_timerMax = 750;     
    uint32_t _effectsLoop_timerPalMax = 60000;     
    uint32_t _effectsLoop_timerPalGradMax = 30000;     
    unsigned long _timer_effectsLoopPal;
    unsigned long _timer_effectsLoopPalGrad;  
    boolean effectsLoop_TimerHue(const String & cmd, const JsonObject & value);
    boolean effectsLoop_TimerPal(const String & cmd, const JsonObject & value);
    // boolean effectsLoop_TimerPalGrad(const String & cmd, const JsonObject & value);

    void fader(uint8_t sPos, const uint8_t _tgtbrt, std::function<void(void)> callback);
  public:
      LAMP(uint8_t p);
      
      Ticker _fadeTicker;             //
      Ticker _effectsTicker;          // 

      void effectsTimer_now(uint32_t & v) { v = _effectsTimer_now;}
      void effectsTimer(SCHEDULER action) ;
      void effectsTimer(uint8_t sP, SCHEDULER action, uint32_t _begin = 0);

      void changePower(uint8_t sP); 
      void changePower(uint8_t sP, bool);

      hsvloopManage     _hsvLoop_1;
      hsvloopManage     _hsvLoop_2;
      uint8_t           _palPos = 0;
      uint8_t           _effTimerMod = 0;
      void effectsLoop_pal_reset(uint8_t p =0);
      boolean effectsLoop_Timer(const String & cmd, const JsonObject & value);
      void effectsLoop_TimerPal(const uint32_t & v);
      void effectsLoop();
      void effectsLoop_TimerHue(uint32_t);
      uint8_t effectsLoop_hue();
      uint8_t effectsLoop_pal();
      uint8_t effectsLoop_activate();
      void effectsLoop_pal(uint32_t & v) { v = _effectsLoop_timerPalMax;}

      void getStripFolder(uint8_t pos, char buffer[]) ;
      void getStripPlayListFolder(uint8_t pos, char buffer[]) ;

      void setup();   
      void loop();   

      // bool isColorPicker()        {return _isColorPicker;}
      bool isRandDemoOn()         {return _flags.isRandDemo;}
      bool isDebugOn()            {return _flags.isDebug;}
      bool isLampOn()             {return _flags.ONflag;}
      bool isGlobalBrightnessOn() {return _flags.isGlobalBrightness;}
      bool isFaderOn()            {return _flags.isFader;}

      bool isHueLoopOn()          {return _flags.isHueLoop;}
      bool isPalLoopOn()          {return _flags.isPalLoop;}
      // bool isPalgradLoopOn()      {return _flags.isPalgradLoop;}

      LAMPMODE get_Mode()         {return _mode;}


      // void set_colorPicker(bool flag)       {_isColorPicker = flag;}
      void set_debug(bool flag)             {_flags.isDebug     = flag; _lampState.isDebug    = flag;}
      void set_randDemo(bool flag)          {_flags.isRandDemo  = flag; _lampState.isRandDemo = flag; }
      void set_Mode(LAMPMODE v1)            {_mode=v1;}
      void set_lampOn(bool flag)            {_flags.ONflag = flag;}
      void set_fader(bool flag)             {_flags.isFader = flag;}
      void set_globalBrightness(bool val)   {_flags.isGlobalBrightness = val;}

      void set_hueLoop(bool val)            {_flags.isHueLoop       = val;}
      void set_palLoop(bool val)            {_flags.isPalLoop       = val;}

      uint8_t get_globalBrightness(uint8_t sP, uint8_t v);

      void effect_setspd_incr(uint8_t,int8_t);
      void effect_setspd(uint8_t,uint8_t);
      void effect_setbri(uint8_t,uint8_t);
      void effect_setbri_incr(uint8_t,int8_t);
      void effect_setscale(uint8_t,uint8_t);

      void effectChangeByPos(uint8_t stripPos, uint16_t pos, boolean fade, boolean effSav, boolean skip = false);
      void effectSav(uint8_t stripPos);
      void effectSetDefault(uint8_t stripPos);

      EffectWorker * get_effect(uint8_t sp);
      void effect_id(uint8_t stripPos, uint8_t id, uint8_t value) ;

      void fadelight(uint8_t stripPos, const uint8_t _targetbrightness, const uint32_t _duration, std::function<void(void)> callback);
  
      void json_generalSetting(JsonObject & root);
  };
  LAMP * LAMPPTRGET();
#endif // LAMP_H