#ifndef HSVLOOP_H
#define HSVLOOP_H

#include "../fastled/base.h"  
#include "set.h"  

class hsvloop : public ledsTools
{
public:
  hsvloop();
  ~hsvloop();

  void  mod(int hue_rnd, int & hue_regressif, uint8_t & hue_max,uint8_t & hue_min, uint8_t & hue);
  int   getMod(HUE_MODS mod);
  void  nm(uint8_t & value);
  void  select(CRGB c, uint8_t & value);
  void  select(CRGB c, CRGB c2, uint8_t & value, uint8_t & value_2);
  void  select(RgbColor c, uint8_t & value);
  void  select(RgbColor c, RgbColor c2, uint8_t & value, uint8_t & value_2);
};

class hsvloopManage : public hsvloop
{
  hsvloop           _hsvLoop;
  EFFECTCOLOR_MODS  _colorMod       = EFFECTCOLOR_HUE;
  HUE_MODS          _hueMod         = HUE_MOD_TIMER;
  adri_timer        * _hueTimer     = nullptr;
  int               _hueRnd         = 1;
  int               _hueRegressif   = 0; 
  CRGB              _cRgb           = CRGB(0,   0, 255);
  uint8_t           _hue            = 0;
  uint32_t          _hueDelay       = 100;
  uint8_t           _hueMin         = 0;
  uint8_t           _hueMax         = 255;   
  uint8_t           _cMod           = 0;  
   boolean          _hue_change     = true;
public:
  hsvloopManage();
  ~hsvloopManage();
  uint8_t hue();
  void hueLoop(uint32_t delay) ;
  void hueChangeToggle() ;
  void hueReset() ;
  void hueChangeSet(boolean v) ;
  boolean hueChangeGet() ;
  
  void set_hueRnd(uint8_t value)  { _hueRnd = value; }
  void set_hue(uint8_t value)     {_hue = value;};
};
#endif // HSVLOOP_H