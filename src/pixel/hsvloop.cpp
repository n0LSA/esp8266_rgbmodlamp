#include "../../include/pixel/hsvloop.h"


hsvloop::hsvloop(){}
hsvloop::~hsvloop(){}



void hsvloop::mod(int hue_rnd, int & hue_regressif, uint8_t & hue_max,uint8_t & hue_min, uint8_t & hue){
    static uint8_t count;  
    uint8_t min;
    uint8_t max;
    uint8_t hue_incr = 1;


        
    switch (hue_rnd) {
      case 0:
        if (hue_max < hue_min)  hue_max = hue_min;
        hue += hue_incr;
      if (hue >= hue_max) hue = hue_min;
      break;
      case 1:
        if(hue_regressif < 1)   hue_regressif = 1;
        if(hue_regressif > 2)   hue_regressif = 1;        
        // Serial.printf("hue_regressif: %d\n", hue_regressif);
        // Serial.printf("hue_incr: %d\n", hue_incr);
        if (hue_regressif == 2){
          hue -= hue_incr;
          if ((hue - hue_incr) < hue_min) {
            hue = hue_min; 
            hue_regressif = 1;
          }
        }
        else if (hue_regressif == 1){
          hue += hue_incr;  
          if ((hue + hue_incr) >= hue_max ) {
            hue_regressif = 2;
          }         
        }
      break;  
      case 2:
        hue = WheelPos(count);
        min = map(hue, 0, 255, hue / 2, hue);
        max = map(hue, 0, 255, hue,   hue * 2);
        if (count == random8() )      count = random(min, max);
        if (count == random8() / 2 )    count = random(min, max);
        if (count == random8() * 2 )    count = random(min, max);
        count++;
      break; 
      case 3:
        hue = WheelPos(random8());
      break;                      
   }
}

int hsvloop::getMod(HUE_MODS mod){
  if (mod == HUE_MOD_TIMER)   {return 0;}
  if (mod == HUE_MOD_CPAl)    {return 1;}
  if (mod == HUE_MOD_NM)      {return 2;}
  if (mod == HUE_MOD_CC)      {return 3;}
  return -1;
}

void hsvloop::nm(uint8_t & value){value++;}

void hsvloop::select(CRGB c, uint8_t & value){
   HsvColor sHue = rgb2hsv(c.r, c.g, c.b);
   value = sHue.h;
}
void hsvloop::select(CRGB c, CRGB c2, uint8_t & value, uint8_t & value_2){
  HsvColor sHue = rgb2hsv(c.r, c.g, c.b);
  value = sHue.h;

  sHue = rgb2hsv(c2.r, c2.g, c2.b);
  value_2 = sHue.h;   
}


void hsvloop::select(RgbColor c, uint8_t & value){
   HsvColor sHue = rgb2hsv(c.R, c.G, c.B);
   value = sHue.h;
}
void hsvloop::select(RgbColor c, RgbColor c2, uint8_t & value, uint8_t & value_2){
  HsvColor sHue = rgb2hsv(c.R, c.G, c.B);
  value = sHue.h;

  sHue = rgb2hsv(c2.R, c2.G, c2.B);
  value_2 = sHue.h;   
}

hsvloopManage::hsvloopManage(){
  _hueTimer = new adri_timer(_hueDelay, true);  

}
void hsvloopManage::hueLoop(uint32_t delay){
    if      (_hsvLoop.getMod(_hueMod) == 0) {if (_hue_change && _hueTimer->loop(delay)) {/*fsprintf("[hueLoop]\n");*/ _hsvLoop.mod(_hueRnd, _hueRegressif, _hueMax, _hueMin, _hue);}  }  
    else if (_hsvLoop.getMod(_hueMod) == 1) {if (_hue_change && _hueTimer->loop(delay)) _hsvLoop.mod(_hueRnd, _hueRegressif, _hueMax, _hueMin, _hue);  }          
    else if (_hsvLoop.getMod(_hueMod) == 2) {_hsvLoop.nm(_hue);      }       
    else if (_hsvLoop.getMod(_hueMod) == 3) {_hsvLoop.select(_cRgb, _hue);   }   
}
uint8_t hsvloopManage::hue() {return _hue;}
void hsvloopManage::hueChangeToggle() {_hue_change=!_hue_change;}
void hsvloopManage::hueChangeSet(boolean v) {_hue_change=v;}
boolean hsvloopManage::hueChangeGet() {return _hue_change;}
void hsvloopManage::hueReset(){
  _hue = 60;
  _hueTimer->set_duration(millis());
}
hsvloopManage::~hsvloopManage(){
  delete _hueTimer;
}