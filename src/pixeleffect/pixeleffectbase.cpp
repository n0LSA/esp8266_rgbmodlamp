#include "../../include/pixeleffect/pixeleffectbase.h"
#include "../../include/pixel/color_palette.h"
#include "../../include/display/lamp.h"

extern CRGBPalette16 cTargetPalette;;

const TProgmemRGBPalette16 *paletteFire_arr[] = {
  &NormalFire_p,
  &LithiumFireColors_p,
  &NormalFire2_p,
  &WoodFireColors_p,
  &NormalFire3_p,
  &CopperFireColors_p,
  &HeatColors_p,
  &PotassiumFireColors_p,
  &MagmaColor_p,
  &RubidiumFireColors_p,
  &AlcoholFireColors_p,
  &WaterfallColors_p
};

const TProgmemRGBPalette16 *palette_arr[] = {
  &AuroraColors_p,
  &ForestColors_p,
  &NormalFire_p,
  &LavaColors_p,
  &OceanColors_p,
  &PartyColors_p,
  &RainbowColors_p,
  &ZeebraColors_p,
  &HeatColors_p,
  &CloudColors_p,
  &EveningColors_p,
  &LithiumFireColors_p,
  &WoodFireColors_p,
  &SodiumFireColors_p,
  &CopperFireColors_p,
  &AlcoholFireColors_p,
  &RubidiumFireColors_p,
  &PotassiumFireColors_p,
  &WaterfallColors_p,
  &AutumnColors_p,
  &AcidColors_p,
  &StepkosColors_p,
  &HolyLightsColors_p,
  &MagmaColor_p,//
  &NormalFire3_p,
  &NormalFire2_p,
  &HeatColors2_p,
  &WhiteBlackColors_p,
  &NeonColors_p,
  &OrangeColors_p
};


void nscale8( CRGBW* leds, uint16_t num_leds, uint8_t scale)
{
  for( uint16_t i = 0; i < num_leds; i++) {
      leds[i].nscale8( scale);
  }
}

void fadeToBlackBy( CRGBW* leds, uint16_t num_leds, uint8_t fadeBy)
{
    nscale8( leds, num_leds, 255 - fadeBy);
}

void EffectFx_neo::reset(){
  if (DevicePtrGet()->get_outputArray(_strip_pos)->_leds_segment.isSegment() ) { 
    uint8_t maxSeg = 0 ;
    DevicePtrGet()->get_outputArray(_strip_pos)->_leds_segment.get_max(maxSeg);
    for (int i = 0; i < maxSeg; ++i) {
        _neoFxxPtr->resetSegmentRuntime(i);        
    }   
  } else {
    _neoFxxPtr->resetSegmentRuntime(0);     
  }   
}



void EffectFx_neo::update(){
    if (oldBpm!= speed){
      oldBpm = speed;
      _neoFxxPtr->setSpeed(convertSpeed(speed));
    }   
    neoSetting.intensity  = LAMPPTRGET()->get_globalBrightness(_strip_pos, brightness) ;
    neoSetting.scale = scale;

    if (oldBri!= LAMPPTRGET()->get_globalBrightness(_strip_pos, brightness)){
      oldBri = LAMPPTRGET()->get_globalBrightness(_strip_pos, brightness);
      // ALT_TRACEC("main", ">>>>>>>>>%d\n", LAMPPTRGET()->get_globalBrightness(_strip_pos, brightness));
      if (_hue_1 != 0) neoSetting.c1 = hsv2rgb (_hue_1, 255, LAMPPTRGET()->get_globalBrightness(_strip_pos, brightness), 0);  
      if (_hue_2 != 0) neoSetting.c2 = hsv2rgb (_hue_2, 255, LAMPPTRGET()->get_globalBrightness(_strip_pos, brightness), 0);   
      statu = true;     
    }      


    if ( _effectType == OET_HUETIMER || _effectType == OET_HTPG ) {
      if (statu || _effectType == OET_HTPG) {
          if (_hue_1==0) {
                            neoSetting.c1 = hsv2rgb (LAMPPTRGET()->_hsvLoop_1.hue(),        255, LAMPPTRGET()->get_globalBrightness(_strip_pos, brightness), 0); 
            if (_hue_2==0)  neoSetting.c2 = hsv2rgb (LAMPPTRGET()->_hsvLoop_1.hue()+_varia, 255, LAMPPTRGET()->get_globalBrightness(_strip_pos, brightness), 0);    
            if (_effectType == OET_HUETIMER) LAMPPTRGET()->_hsvLoop_1.hueChangeToggle();         
          }
          if (_hue_2==0 && _hue_1!=0) {
            neoSetting.c2 = hsv2rgb (LAMPPTRGET()->_hsvLoop_2.hue(), 255, LAMPPTRGET()->get_globalBrightness(_strip_pos, brightness), 0);    
            if (_effectType == OET_HUETIMER) LAMPPTRGET()->_hsvLoop_2.hueChangeToggle();         
          }            
      } else if (_colorMod == 1) {

      }

/*
      if (_hue_1==0 && _hue_2==0) {
        if (statu || _effectType == OET_HTPG) {
          neoSetting.c1 = hsv2rgb (LAMPPTRGET()->_hsvLoop_1.hue(), 255, LAMPPTRGET()->get_globalBrightness(_strip_pos, brightness), 0); 
          neoSetting.c2 = hsv2rgb (LAMPPTRGET()->_hsvLoop_2.hue(), 255, LAMPPTRGET()->get_globalBrightness(_strip_pos, brightness), 0);    
          if (_effectType == OET_HUETIMER) {LAMPPTRGET()->_hsvLoop_1.hueChangeToggle(); LAMPPTRGET()->_hsvLoop_2.hueChangeToggle();}         
        } 
      }
      if (_hue_1==0 && _hue_2!=0) {
        if (statu || _effectType == OET_HTPG) {
          neoSetting.c1 = hsv2rgb (LAMPPTRGET()->_hsvLoop_1.hue(), 255, LAMPPTRGET()->get_globalBrightness(_strip_pos, brightness), 0); 
          neoSetting.c2 = hsv2rgb (LAMPPTRGET()->_hsvLoop_2.hue(), 255, LAMPPTRGET()->get_globalBrightness(_strip_pos, brightness), 0);    
          if (_effectType == OET_HUETIMER) {LAMPPTRGET()->_hsvLoop_1.hueChangeToggle(); LAMPPTRGET()->_hsvLoop_2.hueChangeToggle();}         
        } 
      }

      if (_hue_1==0) {
        if (statu || _effectType == OET_HTPG) {
                          neoSetting.c1 = hsv2rgb (LAMPPTRGET()->_hsvLoop_1.hue(),        255, LAMPPTRGET()->get_globalBrightness(_strip_pos, brightness), 0); 
          if (_hue_2==0)  neoSetting.c2 = hsv2rgb (LAMPPTRGET()->_hsvLoop_1.hue()+_varia, 255, LAMPPTRGET()->get_globalBrightness(_strip_pos, brightness), 0);    
          if (_effectType == OET_HUETIMER) LAMPPTRGET()->_hsvLoop_1.hueChangeToggle();         
        } 
      }
      if (_hue_2==0 && _hue_1!=0) {
        if (statu || _effectType == OET_HTPG) {
                          neoSetting.c2 = hsv2rgb (LAMPPTRGET()->_hsvLoop_2.hue(),        255, LAMPPTRGET()->get_globalBrightness(_strip_pos, brightness), 0); 
          if (_hue_2!=0)  neoSetting.c2 = hsv2rgb (LAMPPTRGET()->_hsvLoop_2.hue()+_varia, 255, LAMPPTRGET()->get_globalBrightness(_strip_pos, brightness), 0);    
          if (_effectType == OET_HUETIMER) LAMPPTRGET()->_hsvLoop_2.hueChangeToggle();         
        } 
      }  
*/
    } 

    if ((_effectType == OET_PALFIRE) && _pTimer ){
      if (LAMPPTRGET()->_palPos != _pPos) {
        if (LAMPPTRGET()->_palPos>FIRE_PALETTS_COUNT-1){
          _pPos=0;
          LAMPPTRGET()->_palPos=0;
        } 
        _pPos = LAMPPTRGET()->_palPos;
      }
      neoSetting.pPalette =  * paletteFire_arr[_pPos] ;      
    }   
    if ((_effectType == OET_PAL) && _pTimer ){
      if (LAMPPTRGET()->_palPos != _pPos) {
        if (LAMPPTRGET()->_palPos>FASTLED_PALETTS_COUNT-1){
          _pPos=0;
          LAMPPTRGET()->_palPos=0;
        } 
        _pPos = LAMPPTRGET()->_palPos;
      }
      neoSetting.pPalette =  * palette_arr[_pPos] ;      
    }   
    if (_effectType == OET_PALGRAD && _pTimer) {
      if ((millis()-_timer_toward)>40){
          nblendPaletteTowardPalette(  neoSetting.pPalette, _gTargetPalette, 48);
          _timer_toward = millis();
      }
      if (LAMPPTRGET()->_palPos != _pPos) {
        if (LAMPPTRGET()->_palPos>GRADIENT_PALETTS_COUNT-1){
          _pPos=0;
          LAMPPTRGET()->_palPos=0;
        } 
        _pPos = LAMPPTRGET()->_palPos;
      }
      _gTargetPalette = gGradientPalettes[_pPos] ;        
    } 

    if (_effectType == OET_PALDUO && _pTimer ){
      if (_defaultPal == OET_PAL) {
        if (LAMPPTRGET()->_palPos != _pPos) {
          if (LAMPPTRGET()->_palPos>FASTLED_PALETTS_COUNT-1){
            _pPos=0;
            LAMPPTRGET()->_palPos=0;
          } 
          _pPos = LAMPPTRGET()->_palPos;
        }
        neoSetting.pPalette =  *palette_arr[_pPos] ;        
      }  
      if (_defaultPal == OET_PALFIRE) {
        if (LAMPPTRGET()->_palPos != _pPos) {
          if (LAMPPTRGET()->_palPos>FIRE_PALETTS_COUNT-1){
            _pPos=0;
            LAMPPTRGET()->_palPos=0;
          } 
          _pPos = LAMPPTRGET()->_palPos;
        }
        neoSetting.pPalette =  *paletteFire_arr[_pPos] ;        
      }  
      if (_defaultPal == OET_PALGRAD) {
        if ((millis()-_timer_toward)>40){
            nblendPaletteTowardPalette(  neoSetting.pPalette, _gTargetPalette, 48);
            _timer_toward = millis();
        }
        if (LAMPPTRGET()->_palPos != _pPos) {
          if (LAMPPTRGET()->_palPos>GRADIENT_PALETTS_COUNT-1){
            _pPos=0;
            LAMPPTRGET()->_palPos=0;
          } 
          _pPos = LAMPPTRGET()->_palPos;
        }
        _gTargetPalette = gGradientPalettes[_pPos] ;        
      }        
    } 

    
    _neoFxxPtr->service(statu, show);
} 

void EffectFx_neo::set_dynCtrl(UIControl*_val){
  if      (_val->getId()==3) { neoSetting.fade = EffectCalc::setDynCtrl(_val).toInt(); }
  else if (_val->getId()==4) { 
    LAMPPTRGET()->effectsLoop_TimerHue(EffectCalc::setDynCtrl(_val).toInt()); 
  }
  else if (_val->getId()==5) { neoSetting.variation = _varia = EffectCalc::setDynCtrl(_val).toInt(); }
  else if (_val->getId()==6) { neoSetting.size = EffectCalc::setDynCtrl(_val).toInt(); }
  else if(_val->getId()==7) {
    if (!EffectCalc::setDynCtrl(_val).toInt()) {
      _defaultPal= OET_PAL;
    }else {
      _defaultPal= OET_PALGRAD;
    }
  }     
  else if (_val->getId()==8) {
    boolean state = LAMPPTRGET()->_hsvLoop_1.hueChangeGet();
    LAMPPTRGET()->_hsvLoop_1.hueChangeSet(true);
    _hue_1 = EffectCalc::setDynCtrl(_val).toInt(); 
    if (_hue_1 != 0) {
      neoSetting.c1 = hsv2rgb (_hue_1-1, 255, LAMPPTRGET()->get_globalBrightness(_strip_pos, brightness), 0);
    } 
    LAMPPTRGET()->_hsvLoop_1.hueChangeSet(state);
  }
  else if (_val->getId()==9) {
    boolean state = LAMPPTRGET()->_hsvLoop_2.hueChangeGet();
    LAMPPTRGET()->_hsvLoop_2.hueChangeSet(true);
    _hue_2 = EffectCalc::setDynCtrl(_val).toInt(); 
    if (_hue_2 != 0) {
      neoSetting.c2 = hsv2rgb (_hue_2-1, 255, LAMPPTRGET()->get_globalBrightness(_strip_pos, brightness), 0);
    }     
    LAMPPTRGET()->_hsvLoop_2.hueChangeSet(state);
  }
  else if (_val->getId()==11) { 
    uint8_t value = EffectCalc::setDynCtrl(_val).toInt();
    if (value == 0) {
      _pTimer = true; 
    }
    else {
      _pTimer = false;
      if (_effectType == OET_PAL) {
        _pPos = value - 1;
        neoSetting.pPalette =  *palette_arr[_pPos];  
      } 
      if (_effectType == OET_PALFIRE) {
        _pPos = value - 1;
        neoSetting.pPalette =  *paletteFire_arr[_pPos];  
      }       
      if (_effectType == OET_PALGRAD) {
        _pPos = value - 1;
        neoSetting.pPalette = gGradientPalettes[_pPos] ;  
      }           
      if (_effectType == OET_PALDUO ){
        if (_defaultPal == OET_PAL) {
          _pPos = value - 1;
          _pPos = map(_pPos, 0, GRADIENT_PALETTS_COUNT, 0, FASTLED_PALETTS_COUNT-1);
          neoSetting.pPalette =  *palette_arr[_pPos];
        }
        if (_defaultPal == OET_PALFIRE) {
          _pPos = value - 1;
          _pPos = map(_pPos, 0, GRADIENT_PALETTS_COUNT, 0, FIRE_PALETTS_COUNT-1);
          neoSetting.pPalette =  *paletteFire_arr[_pPos];
        }        
        if (_defaultPal == OET_PALGRAD) {
          _pPos = value - 1;
          neoSetting.pPalette = gGradientPalettes[_pPos] ;  
        }        
      }  
    }
    LAMPPTRGET()->set_hueLoop     (LAMPPTRGET()->effectsLoop_hue());
    LAMPPTRGET()->set_palLoop     (LAMPPTRGET()->effectsLoop_pal());      
  }
  else if (_val->getId()==12) {
    if (isPalEffect(_effectType) )
      LAMPPTRGET()->effectsLoop_TimerPal(static_cast<uint32_t>(EffectCalc::setDynCtrl(_val).toInt()*_timerPalFactor));
  }  
  else EffectCalc::setDynCtrl(_val).toInt();
  #ifdef ALT_DEBUG_TARCE
    _val->print();  
  #endif
}
EffectFx_neo::~EffectFx_neo(){
  delete _neoFxxPtr;
  _neoFxxPtr= nullptr;
}   
void EffectFx_neo::instance(){
  boolean isTw;
  leds_managePtrGet()->get_ledsIsTw(_strip_pos, isTw);    
  if (isTw) _neoFxxPtr = new WS2812FX(_strip_pos, ledarray_w, &neoSetting);
  else      _neoFxxPtr = new WS2812FX(_strip_pos, ledarray, &neoSetting);  

  LAMPPTRGET()->effectsLoop_pal_reset(_pPos);

  _gTargetPalette       =  gGradientPalettes[_pPos] ;
  neoSetting.pPalette   =  gGradientPalettes[_pPos] ;   
}
void EffectFx_neo::setup(uint8_t strip_pos, OUTPUT_EFFECTTYPE mod){ 
  _strip_pos = strip_pos;

  boolean isTw;
  leds_managePtrGet()->get_ledsIsTw(strip_pos, isTw);
  if (isTw) ledarray_w  = leds_managePtrGet()->get_crgwbArray(strip_pos);
  else      ledarray    = leds_managePtrGet()->get_crgbArray(strip_pos);

  leds_managePtrGet()->get_numLeds(strip_pos, numLeds);

  _effectType = mod;

  // String s;
  // effectType_toString(mod, s);
  // ALT_TRACEC("main", "test: %s\n", s.c_str());

  neoSetting.pos            = 0;
  neoSetting.numLeds        = numLeds;
  neoSetting.segment        = true;
  neoSetting.reverseStart   = false;
  neoSetting.c1             = {0,80,0};
  neoSetting.c2             = {0,0,80};    
  neoSetting.isTw           = isTw;                
  neoSetting.fade           = getCtrlVal(3).toInt();                
  neoSetting.scale          = scale;                
  neoSetting.size           = 3;                
  neoSetting.intensity      = getCtrlVal(0).toInt();;                
  neoSetting.palette        = 0;                
  oldBpm                    = 0;
  oldBri                    = 0;



} 
void EffectFx_mode::load(uint8_t strip_pos){}
void EffectFx_mode::setupEff(uint8_t strip_pos){
  setup(strip_pos,  effectType_toMod(_effeNamIDList[_mod]._effType) );
  neoSetting.segment = atoi(_effeNamIDList[_mod]._segment);
  
  ALT_TRACEC(ALML_DEBUGREGION_PIXELEFFECT, "effeNamIDList\n");
  ALT_TRACEC(ALML_DEBUGREGION_PIXELEFFECT, "&c:1&s:\t_mod(position list): %d\n", _mod);
  ALT_TRACEC(ALML_DEBUGREGION_PIXELEFFECT, "&c:1&s:\t_effType: %s\n", _effeNamIDList[_mod]._effType);
  ALT_TRACEC(ALML_DEBUGREGION_PIXELEFFECT, "&c:1&s:\t_effType: %d\n", effectType_toMod(_effeNamIDList[_mod]._effType));
  ALT_TRACEC(ALML_DEBUGREGION_PIXELEFFECT, "&c:1&s:\t_nameId: %s\n", _effeNamIDList[_mod]._nameId);
  ALT_TRACEC(ALML_DEBUGREGION_PIXELEFFECT, "&c:1&s:\t_cat: %s\n", _effeNamIDList[_mod]._cat);
  ALT_TRACEC(ALML_DEBUGREGION_PIXELEFFECT, "&c:1&s:\t_segment: %s\n", _effeNamIDList[_mod]._segment);
  ALT_TRACEC(ALML_DEBUGREGION_PIXELEFFECT, "neoSetting\n");
  ALT_TRACEC(ALML_DEBUGREGION_PIXELEFFECT, "&c:1&s:\tsegment: %d\n", neoSetting.segment);
  ALT_TRACEC(ALML_DEBUGREGION_PIXELEFFECT, "&c:1&s:\tisTw: %d\n", neoSetting.isTw);
  ALT_TRACEC(ALML_DEBUGREGION_PIXELEFFECT, "&c:1&s:\tnumLeds: %d\n", neoSetting.numLeds);

  ALT_TRACEC(ALML_DEBUGREGION_PIXELEFFECT, "_strip_pos: %d\n", _strip_pos);
  ALT_TRACEC(ALML_DEBUGREGION_PIXELEFFECT, "numLeds: %d\n", numLeds);

  instance();

  switch (_mod) {
  case 0    : _neoFxxPtr->_effectMod[0] = &WS2812FX::mode_pride                 ; break;
  case 1    : _neoFxxPtr->_effectMod[0] = &WS2812FX::mode_pride                 ; break;
  case 2    : _neoFxxPtr->_effectMod[0] = &WS2812FX::mode_colorwaves            ; break;
  case 3    : _neoFxxPtr->_effectMod[0] = &WS2812FX::mode_colorwaves            ; break;
  case 4    : _neoFxxPtr->_effectMod[0] = &WS2812FX::mode_rainbow_cycle         ; break;
  case 5    : _neoFxxPtr->_effectMod[0] = &WS2812FX::mode_rainbow_cycle         ; break;
  case 6    : _neoFxxPtr->_effectMod[0] = &WS2812FX::mode_rainbow               ; break;
  case 7    : _neoFxxPtr->_effectMod[0] = &WS2812FX::mode_color_wipe            ; break;
  case 8    : _neoFxxPtr->_effectMod[0] = &WS2812FX::mode_color_wipe            ; break;
  case 9    : _neoFxxPtr->_effectMod[0] = &WS2812FX::mode_color_wipe_rev_inv    ; break;
  case 10   : _neoFxxPtr->_effectMod[0] = &WS2812FX::mode_color_wipe_rev_inv    ; break;
  case 11   : _neoFxxPtr->_effectMod[0] = &WS2812FX::mode_color_wipe_random     ; break;
  case 12   : _neoFxxPtr->_effectMod[0] = &WS2812FX::mode_color_wipe_random     ; break;
  case 13   : _neoFxxPtr->_effectMod[0] = &WS2812FX::mode_tricolor_chase        ; break;
  case 14   : _neoFxxPtr->_effectMod[0] = &WS2812FX::mode_tricolor_chase        ; break;
  case 15   : _neoFxxPtr->_effectMod[0] = &WS2812FX::mode_breath                ; break;
  case 16   : _neoFxxPtr->_effectMod[0] = &WS2812FX::mode_colortwinkle          ; break;
  case 17   : _neoFxxPtr->_effectMod[0] = &WS2812FX::mode_fade                  ; break;
  case 18   : _neoFxxPtr->_effectMod[0] = &WS2812FX::mode_scan                  ; break;
  case 19   : _neoFxxPtr->_effectMod[0] = &WS2812FX::mode_fireworks             ; break;
  case 20   : _neoFxxPtr->_effectMod[0] = &WS2812FX::mode_fireworks_random      ; break;
  case 21   : _neoFxxPtr->_effectMod[0] = &WS2812FX::mode_tricolor_wipe         ; break;
  case 22   : _neoFxxPtr->_effectMod[0] = &WS2812FX::mode_theater_chase_rainbow ; break;
  case 23   : _neoFxxPtr->_effectMod[0] = &WS2812FX::mode_blends                ; break;
  case 24   : _neoFxxPtr->_effectMod[0] = &WS2812FX::mode_wavesins              ; break;
  case 25   : _neoFxxPtr->_effectMod[0] = &WS2812FX::mode_chase_blackout        ; break;
  default:break;
  }   
}
bool EffectFx_mode::run()                                     {update(); return true;}
String EffectFx_mode::setDynCtrl(UIControl*_val)              {set_dynCtrl(_val); return String();}



void EffectFx_comm::setup(uint8_t strip_pos){ 

  _strip_pos = strip_pos;
  _effectType = effectType_toMod(_effeNamIDList[_mod]._effType) ;
  leds_managePtrGet()->get_ledsIsTw(_strip_pos, _isTw);

  if (_isTw) _ledarray_w  = leds_managePtrGet()->get_crgwbArray(_strip_pos);
  else       _ledarray    = leds_managePtrGet()->get_crgbArray(_strip_pos);

  leds_managePtrGet()->get_numLeds(_strip_pos, _numLeds);
  _isSegment = DevicePtrGet()->get_outputArray(_strip_pos)->_leds_segment.isSegment() && atoi(_effeNamIDList[_mod]._segment);

  if (DevicePtrGet()->get_outputArray(_strip_pos)->_leds_segment.isSegment()  ) {
    DevicePtrGet()->get_outputArray(_strip_pos)->_leds_segment.get_max(_maxSeg);
  }


  ALT_TRACEC(ALML_DEBUGREGION_PIXELEFFECT, "effeNamIDList\n");
  ALT_TRACEC(ALML_DEBUGREGION_PIXELEFFECT, "&c:1&s:\t_mod(position list): %d\n", _mod);
  ALT_TRACEC(ALML_DEBUGREGION_PIXELEFFECT, "&c:1&s:\t_effType: %s\n", _effeNamIDList[_mod]._effType);
  ALT_TRACEC(ALML_DEBUGREGION_PIXELEFFECT, "&c:1&s:\t_effType: %d\n", _effectType);
  ALT_TRACEC(ALML_DEBUGREGION_PIXELEFFECT, "&c:1&s:\t_nameId: %s\n", _effeNamIDList[_mod]._nameId);
  ALT_TRACEC(ALML_DEBUGREGION_PIXELEFFECT, "&c:1&s:\t_cat: %s\n", _effeNamIDList[_mod]._cat);
  ALT_TRACEC(ALML_DEBUGREGION_PIXELEFFECT, "&c:1&s:\t_segment: %s\n", _effeNamIDList[_mod]._segment);
  ALT_TRACEC(ALML_DEBUGREGION_PIXELEFFECT, "_strip_pos: %d\n", _strip_pos);
  ALT_TRACEC(ALML_DEBUGREGION_PIXELEFFECT, "_isTw: %d\n", _isTw);
  ALT_TRACEC(ALML_DEBUGREGION_PIXELEFFECT, "_numLeds: %d\n", _numLeds);
  ALT_TRACEC(ALML_DEBUGREGION_PIXELEFFECT, "_isSegment: %d\n", _isSegment);
  ALT_TRACEC(ALML_DEBUGREGION_PIXELEFFECT, "_maxSeg: %d\n", _maxSeg);

}
boolean EffectFx_comm::dryrun(float n, uint8_t delay){
  if((millis() - _lastupdate) < (unsigned)(float(255 - speed) / n)) {
    _active=false;
  } else {
    _lastupdate = millis();
    _active=true;
  }
  return !_active;
}
boolean EffectFx_comm::dryrun(uint8_t s, float n, uint8_t delay){
  if((millis() - _lastupdate) < (unsigned)(float(255 - s) / n)) {
    _active=false;
  } else {
    _lastupdate = millis();
    _active=true;
  }
  return !_active;
}
void EffectFx_comm::set_dynCtrl(UIControl*_val){
  if      (_val->getId()==3) { _fade = EffectCalc::setDynCtrl(_val).toInt(); }
  else if (_val->getId()==4) { 
    if (_hue_palette && _effectType == OET_HUETIMER && _hue_1 == 0) {
      LAMPPTRGET()->effectsLoop_TimerPal(static_cast<uint32_t>(EffectCalc::setDynCtrl(_val).toInt()*_timerPalFactor));
    }
    else {
      _hueTimer = EffectCalc::setDynCtrl(_val).toInt(); 
      LAMPPTRGET()->effectsLoop_TimerHue(_hueTimer); 
    }
  }
  else if (_val->getId()==8) {
    _hue_1 = EffectCalc::setDynCtrl(_val).toInt(); 
    _hue_1V = _hue_1;   
    Serial.printf("com set_dynCtrl _hue_1: %d\n", getHue_1());
    LAMPPTRGET()->set_palLoop(LAMPPTRGET()->effectsLoop_pal());   
    LAMPPTRGET()->set_hueLoop(LAMPPTRGET()->effectsLoop_hue());     
    if (_hue_palette && _effectType == OET_HUETIMER) {

      if (_defaultPal == OET_PALGRAD)  {
        _pPos = map(_hue_1, 0, 255, 0, GRADIENT_PALETTS_COUNT-1);
        _cTargetPalette = gGradientPalettes[_pPos] ;    
        Serial.printf("com set_dynCtrl gGradientPalettes _pPos: %d\n", _pPos);     
      }
      if (_defaultPal == OET_PAL) {
        _pPos = map(_hue_1, 0, 255, 0, FASTLED_PALETTS_COUNT-1);
        _cTargetPalette =  *palette_arr[_pPos];
        Serial.printf("com set_dynCtrl palette_arr _pPos: %d\n", _pPos);  
      }
      if (_defaultPal == OET_PALFIRE) {
        _pPos = map(_hue_1, 0, 255, 0, FIRE_PALETTS_COUNT-1);
        _cTargetPalette =  *paletteFire_arr[_pPos];
        Serial.printf("com set_dynCtrl paletteFire_arr _pPos: %d\n", _pPos);  
      } 
    }
  }
  else if (_val->getId()==9) {
    // _hue_2 = EffectCalc::setDynCtrl(_val).toInt(); 
    // if (_hue_2 != 0) {neoSetting.c2 = hsv2rgb (_hue_2, 255, 255, 0);}     
  }
  else if (_val->getId()==5)  { _varia = EffectCalc::setDynCtrl(_val).toInt(); }
  else if (_val->getId()==6)  { _size = EffectCalc::setDynCtrl(_val).toInt(); }
  else if (_val->getId()==10) { _rep = EffectCalc::setDynCtrl(_val).toInt(); }
  else if (_val->getId()==11) { 
    uint8_t value = EffectCalc::setDynCtrl(_val).toInt();
    if (value == 0) {
      _pTimer = true; 
    }
    else {
      _pTimer = false;
      if (_effectType == OET_PAL) {
        _pPos = value - 1;
        _cTargetPalette =  *palette_arr[_pPos];  
      } 
      if (_effectType == OET_PALFIRE) {
        _pPos = value - 1;
        _cTargetPalette =  *paletteFire_arr[_pPos];  
      }       
      if (_effectType == OET_PALGRAD) {
        _pPos = value - 1;
        _cTargetPalette = gGradientPalettes[_pPos] ;  
      }           
      if (_effectType == OET_PALDUO ){
        if (_defaultPal == OET_PAL) {
          _pPos = value - 1;
          _pPos = map(_pPos, 0, GRADIENT_PALETTS_COUNT, 0, FASTLED_PALETTS_COUNT-1);
          _cTargetPalette =  *palette_arr[_pPos];
        }
        if (_defaultPal == OET_PALFIRE) {
          _pPos = value - 1;
          _pPos = map(_pPos, 0, GRADIENT_PALETTS_COUNT, 0, FIRE_PALETTS_COUNT-1);
          _cTargetPalette =  *paletteFire_arr[_pPos];
        }        
        if (_defaultPal == OET_PALGRAD) {
          _pPos = value - 1;
          _cTargetPalette = gGradientPalettes[_pPos] ;  
        }        
      }
    }
    LAMPPTRGET()->set_hueLoop     (LAMPPTRGET()->effectsLoop_hue());
    LAMPPTRGET()->set_palLoop     (LAMPPTRGET()->effectsLoop_pal());
  }
  else if (_val->getId()==12) {
    if (isPalEffect(_effectType)) 
      LAMPPTRGET()->effectsLoop_TimerPal(static_cast<uint32_t>(EffectCalc::setDynCtrl(_val).toInt()*_timerPalFactor));
  }
  else EffectCalc::setDynCtrl(_val).toInt();
  #ifdef ALT_DEBUG_TARCE
    _val->print();  
  #endif
}

void EffectFx_comm::update(){
  if ( _effectType == OET_HUETIMER || _effectType == OET_HTPG ) {
    if (_hue_1==0) {
      _hue_1V = LAMPPTRGET()->_hsvLoop_1.hue(); 

      if (_hue_palette) {
        // uint8_t vPos = map(_pPos, 0, 255, 0, GRADIENT_PALETTS_COUNT);
        // _gTargetPalette = gGradientPalettes[vPos] ;   
        // Serial.printf("EffectFx_comm - pal change: %d\n", _pPos);
        if (_defaultPal == OET_PALGRAD)  {
          if ((millis()-_timer_toward)>40){
              nblendPaletteTowardPalette(  _cTargetPalette, _gTargetPalette, 48);
              _timer_toward = millis();
          }
          if (LAMPPTRGET()->_palPos != _pPos) {
            if (LAMPPTRGET()->_palPos>GRADIENT_PALETTS_COUNT-1){
              _pPos=0;
              LAMPPTRGET()->_palPos=0;
            } 
            _pPos = LAMPPTRGET()->_palPos;
          }
          _gTargetPalette = gGradientPalettes[_pPos] ; 
        }
        if (_defaultPal == OET_PAL) {
          if (LAMPPTRGET()->_palPos != _pPos) {
            if (LAMPPTRGET()->_palPos>FASTLED_PALETTS_COUNT-1){
              _pPos=0;
              LAMPPTRGET()->_palPos=0;
            } 
            _pPos = LAMPPTRGET()->_palPos;
          }
          _cTargetPalette =  * palette_arr[_pPos] ;  
        }
        if (_defaultPal == OET_PALFIRE) {
          if (LAMPPTRGET()->_palPos != _pPos) {
            if (LAMPPTRGET()->_palPos>FIRE_PALETTS_COUNT-1){
              _pPos=0;
              LAMPPTRGET()->_palPos=0;
            } 
            _pPos = LAMPPTRGET()->_palPos;
          }
          _cTargetPalette =  *paletteFire_arr[_pPos] ;    
        } 
      }
    }
  } 
  if ((_effectType == OET_PALFIRE) && _pTimer){
    if (LAMPPTRGET()->_palPos != _pPos) {
      if (LAMPPTRGET()->_palPos>FIRE_PALETTS_COUNT-1){
        _pPos=0;
        LAMPPTRGET()->_palPos=0;
      } 
      _pPos = LAMPPTRGET()->_palPos;
    }
    _cTargetPalette =  * paletteFire_arr[_pPos] ;      
  }   
  if ((_effectType == OET_PAL) && _pTimer) {
    if (LAMPPTRGET()->_palPos != _pPos) {
      if (LAMPPTRGET()->_palPos>FASTLED_PALETTS_COUNT-1){
        _pPos=0;
        LAMPPTRGET()->_palPos=0;
      } 
      _pPos = LAMPPTRGET()->_palPos;
    }
    _cTargetPalette =  * palette_arr[_pPos] ;      
  }   
  if ((_effectType == OET_PALGRAD) && _pTimer) {
    if ((millis()-_timer_toward)>40){
        nblendPaletteTowardPalette(  _cTargetPalette, _gTargetPalette, 48);
        _timer_toward = millis();
    }
    if (LAMPPTRGET()->_palPos != _pPos) {
      if (LAMPPTRGET()->_palPos>GRADIENT_PALETTS_COUNT-1){
        _pPos=0;
        LAMPPTRGET()->_palPos=0;
      } 
      _pPos = LAMPPTRGET()->_palPos;
    }
    _gTargetPalette = gGradientPalettes[_pPos] ;        
  } 


  if (_effectType == OET_PALDUO && _pTimer ){
    if (_defaultPal == OET_PAL) {
      if (LAMPPTRGET()->_palPos != _pPos) {
        if (LAMPPTRGET()->_palPos>FASTLED_PALETTS_COUNT-1){
          _pPos=0;
          LAMPPTRGET()->_palPos=0;
        } 
        _pPos = LAMPPTRGET()->_palPos;
      }
      _cTargetPalette =  *palette_arr[_pPos] ;        
    }  
    if (_defaultPal == OET_PALFIRE) {
      if (LAMPPTRGET()->_palPos != _pPos) {
        if (LAMPPTRGET()->_palPos>FIRE_PALETTS_COUNT-1){
          _pPos=0;
          LAMPPTRGET()->_palPos=0;
        } 
        _pPos = LAMPPTRGET()->_palPos;
      }
      _cTargetPalette =  *paletteFire_arr[_pPos] ;        
    }  
    if (_defaultPal == OET_PALGRAD) {
      if ((millis()-_timer_toward)>40){
          nblendPaletteTowardPalette(  _cTargetPalette, _gTargetPalette, 48);
          _timer_toward = millis();
      }
      if (LAMPPTRGET()->_palPos != _pPos) {
        if (LAMPPTRGET()->_palPos>GRADIENT_PALETTS_COUNT-1){
          _pPos=0;
          LAMPPTRGET()->_palPos=0;
        } 
        _pPos = LAMPPTRGET()->_palPos;
      }
      _gTargetPalette = gGradientPalettes[_pPos] ;        
    }        
  }   
} 
