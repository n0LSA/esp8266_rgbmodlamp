#include "../../include/pixeleffect/pixeleffect.h"
#include "../../include/pixel/color_palette.h"
#include "../../include/display/lamp.h"


// void fade2black(RgbColor * rgb) {
//   int j = (255-global_persistence);
//   if (j==0) j=1;
//   rgb->R = (rgb->R>j) ? rgb->R-j  : 0 ;
//   rgb->G = (rgb->G>j) ? rgb->G-j  : 0 ;
//   rgb->B = (rgb->B>j) ? rgb->B-j  : 0 ;
// }
// CRGB fade2black(CRGB rgb, uint8_t ammount = 127) ;
// CRGB fade2black(CRGB rgb, uint8_t ammount) {
//   CRGB result;
//   int j = (255-ammount);
//   if (j==0) j=1;
//   result.r = (rgb.r>j) ? rgb.r-j  : 0 ;
//   result.g = (rgb.g>j) ? rgb.g-j  : 0 ;
//   result.b = (rgb.b>j) ? rgb.b-j  : 0 ;
//   return result;
// }

String EffectFx_comets::setDynCtrl(UIControl*_val) {set_dynCtrl(_val); return String();}
void EffectFx_comets::load(uint8_t strip_pos){
  _counter = 0;
}
bool EffectFx_comets::run(){
  update();

  static long     comet_next_start;
  static long     comet_birthday;
  static uint8_t  comet_length;        
  static uint8_t  comet_start_pos;
  static uint8_t  comet_hue;
  static int8_t   comet_speed;
  static int      comet_duration;
  static enum     {dead,alive} comet_status;
  // speed -> rotation pixel
  uint8_t len                 = 127;  // duration
  uint8_t rep                 = _size;   // apparition 
  uint8_t bright_max          = 255;
  uint8_t fade                = LAMPPTRGET()->get_globalBrightness(_strip_pos, brightness);
  uint8_t sat                 = 255;

  uint8_t brightness_fade_ct;
  uint8_t sat_fade_ct;
  uint8_t comet_loc_in_pix;
  uint8_t comet_brightness;
  uint8_t comet_progress;
  int     comet_displacement;

  // if (dryrun(1)) return false;
  // if (dryrun(1)) return false;     

  _pixstart = 0;
  _pixend   = _numLeds-1;
  _pixrange = (_pixend - _pixstart+1);   

  // fadeToBlackBy(_ledarray, _pixrange, 220);
  if (_isTw) {
    for (int i=_pixstart; i <= _pixend; i++) {
      CRGBW old = _ledarray_w[i];
      CRGB c = fade2black(CRGB(old.r, old.g, old.b));
      _ledarray_w[i] = CRGBW(c.r, c.g, c.b, 0);
    } 
  }else {
    for (int i=_pixstart; i <= _pixend; i++) {
      CRGB old = _ledarray[i];
      _ledarray[i] = fade2black(old);
    }    
  }


  if (comet_status==dead) { // if comet inactive
    if (millis()>(uint32_t)comet_next_start) { // and time is elapsed -> we start a new one
      comet_length       =  random8(5, _pixrange); ; // len of the path
      comet_length       *= len/256;
      comet_hue          = _hue_1V;
      comet_start_pos    = random8(0, 255);
      comet_speed        = ( random(_pixstart, _pixrange) );
      comet_duration     = random(speed, map(len,0,255,120,10000)); // random durations from 100mS to 2000mS, adj with len slider
      comet_birthday     = millis();
      comet_status       = alive;
    }
  }
  if (comet_status==alive) { // if comet alive... let it live !!
    uint8_t comet_sat = sat;
    int     comet_age = millis()-comet_birthday;
    if (comet_age<comet_duration) {
            comet_progress      = map (comet_age, 0, comet_duration, 0, 255);     // we compute the progress (0=birthday, 255=death)
            comet_brightness    = comet_progress;                                 // for now, let make brightness simple
            bright_max          = map8(bright_max,0,240);
      if (sat==255) sat--;
            brightness_fade_ct  = map8(comet_length, bright_max, 241);
            sat_fade_ct         = map8(comet_length, sat, 255);
            comet_displacement  = map(comet_progress, 0, 255, 0, comet_speed);    // we suppose the speed reflects the total displacement
      int   comet_loc           = comet_start_pos + comet_displacement;
            comet_loc_in_pix    = map8(comet_loc,_pixstart,_pixend);
      int   comet_dir           = (comet_speed>0) ? -1 : +1;                      // the comet direction on the hoop

      while (comet_brightness>5) {  

        int pixel_offset        = map( (uint8_t)_counter, 0, 255, _pixstart, _pixend);  

        if (!_isTw) led_set_hsv_  (_ledarray,   comet_loc_in_pix, _pixrange, pixel_offset, comet_hue, comet_sat, comet_brightness, fade );
        else        led_set_hsv   (_ledarray_w, comet_loc_in_pix, _pixrange, pixel_offset, comet_hue, comet_sat, comet_brightness, fade );

            comet_loc_in_pix    += comet_dir%_pixrange;
            comet_brightness    = map8(comet_brightness, 0, brightness_fade_ct);
            comet_sat           = map8(comet_sat,        0, sat_fade_ct);
      }

      delay(1);

    } else {
      comet_status=dead;                                                // if comet too old compared to duration, it's time for euthanasy
      comet_next_start = millis() + (map(rep,0, 255, 10, 255)*128);   // next comet will start in 10...1500 mS
    }
  }

  if (!dryrun(3.0)) _counter++;

  return true;
}




String EffectFx_bounce::setDynCtrl(UIControl*_val) {set_dynCtrl(_val); return String();}
void EffectFx_bounce::load(uint8_t strip_pos){
  _counter = 0;
}
bool EffectFx_bounce::run(){
  update();

  #define MAX_DOT 16

  static int speed        [MAX_DOT];
  static int pos          [MAX_DOT];
  static int mass         [MAX_DOT];
  static uint8_t          last_rep;

  if (dryrun(1)) return false;

  _pixstart = 0;
  _pixend   = _numLeds-1;
  _pixrange = (_pixend - _pixstart+1);   

  if (_isTw) fadeToBlackBy(_ledarray_w, _pixrange, 127);
  else fadeToBlackBy(_ledarray, _pixrange, 127);

  uint8_t rep     = 2;
  uint8_t hv      = _varia;
          _hue    = _hue_1V;

  if (last_rep != rep) {
    for (int i=0; i<MAX_DOT; i++) {
      mass[i]     = 4;
      speed[i]    = ((i&1)==0) ? 2000*(i+1)/rep : -2000*(i+1)/rep;
      pos[i]      = i * (_pixrange/rep);
      //Serial.printf("i:%3d mass:%3d speed:%4d\n",i, mass[i], speed[i]);
    }
    last_rep = rep;
  } 

  for (int dot_num=0; dot_num<rep; dot_num++) {
    // 1/ we calculate the new position from the older one and the current speed
    pos[dot_num] = (pos[dot_num] + speed[dot_num]/100)%(_pixrange*20); // new position = old position + signed speed
    if (pos[dot_num]<0) pos[dot_num]+=(_pixrange*20);
    // 2/ we detect collisions
    for (int j=0; j<rep; j++) { // now we search collisions for every dot
      if ( (j!=dot_num) && (abs(pos[j]-pos[dot_num])<10) ){ //now we found one
        int v1i = speed[dot_num];
        int v2i = speed[j];
        int m1  = mass[dot_num];
        int m2  = mass[j];        
        // we apply cinetic & constant motion formula (see http://www.grasp.ulg.ac.be/nvdw/NVdw/Documents_files/08.collisions.pdf)
        int v1f = (   v1i*(m1-m2)  +  v2i*(m2+m2)   )   /   (m1+m2);
        int v2f = (   v2i*(m1-m2)  +  v1i*(m1+m1)   )   /   (m1+m2);        
        speed[dot_num]  = v1f;
        speed[j]        = v2f;
      }   
    }
    // 03/ and at last we draw the dots on the ledstrip
    for (int dot_num=0; dot_num<rep; dot_num++) {
      int varicolor = (dot_num<<8)/rep;           // varies from 0 to 256
      uint8_t var = 0xff & ((varicolor*hv>>8)-(hv/2));
      uint8_t hue = _hue + var;
      int pixel_offset = map( (uint8_t)_counter, 0, 255, _pixstart, _pixend);
      if (_isTw)  led_set_hsv(_ledarray_w,pos[dot_num]/20,_pixrange,pixel_offset,hue, 255, LAMPPTRGET()->get_globalBrightness(_strip_pos, brightness), 127);
      else led_set_hsv_(_ledarray,pos[dot_num]/20,_pixrange,pixel_offset,hue, 255, LAMPPTRGET()->get_globalBrightness(_strip_pos, brightness), 127);
    }        
  }
  _counter = (_direction) ? _counter + 1 : _counter - 1;
  
  return true;
}

String EffectFx_ruggle::setDynCtrl(UIControl*_val) {set_dynCtrl(_val); return String();}
void EffectFx_ruggle::load(uint8_t strip_pos){
  _counter = 0;
}
bool EffectFx_ruggle::run(){
  update();

  if (dryrun(1)) return false;

  _pixstart = 0;
  _pixend   = _numLeds-1;
  _pixrange = (_pixend - _pixstart+1);   

  if (_isTw) fadeToBlackBy(_ledarray_w, _pixrange, 127);
  else fadeToBlackBy(_ledarray, _pixrange, 127);

  uint8_t rep     = scale;
  uint8_t hv      = _size;
  uint8_t len     = map8(60, 1, _pixrange/rep);

  for (int dot_num=0; dot_num<rep; dot_num++) {
    int varicolor = (dot_num<<8)/rep; // varies from 0 to 256
    uint8_t var = 0xff & ((varicolor*hv>>8)-(hv/2));
    uint8_t hue = _hue_1V + var;
    int x=256-sawtooth_wave(10+dot_num*2+rep-1); // select the dot position according to its number. Notice: the frequency depends on the dot selected so they are not in sync
    x=map8(x, _pixstart, _pixend);
    int pixel_offset = map( (uint8_t)_counter, 0, 255, _pixstart, _pixend);
    if (_isTw) 
      for (int j=0; j<len; j++)   led_set_hsv(_ledarray_w, x+j, _pixrange, pixel_offset, hue, 255, LAMPPTRGET()->get_globalBrightness(_strip_pos, brightness), 127);
    else 
      for (int j=0; j<len; j++)   led_set_hsv_(_ledarray, x+j, _pixrange, pixel_offset, hue, 255, LAMPPTRGET()->get_globalBrightness(_strip_pos, brightness), 127);
  }
  _counter = (_direction) ? _counter + 1 : _counter - 1;
  return true;
}

String EffectFx_Comet::setDynCtrl(UIControl*_val) {
  set_dynCtrl(_val);
  if(_val->getId()==5) {
    _hue_palette = EffectCalc::setDynCtrl(_val).toInt();
    LAMPPTRGET()->set_palLoop(LAMPPTRGET()->effectsLoop_pal()); 
    if (_hue_palette && _effectType == OET_HUETIMER && _hue_1 == 0) 
      LAMPPTRGET()->effectsLoop_TimerPal(static_cast<uint32_t>(_hueTimer*_timerPalFactor));    
  }
  else EffectCalc::setDynCtrl(_val).toInt(); // для всех других не перечисленных контролов просто дергаем функцию базового класса (если это контролы палитр, микрофона и т.д.)
  return String();
}
void EffectFx_Comet::load(uint8_t strip_pos){
  _counter = 0;
  if (_hue_palette && _effectType == OET_HUETIMER && _hue_1 == 0) 
    LAMPPTRGET()->effectsLoop_TimerPal(static_cast<uint32_t>(_hueTimer*_timerPalFactor));
  Serial.printf_P(PSTR("\t[effect child load]\n\t\teff_es: %s\n\t\t_hue_palette: %d\n\t\t_hue_1: %d\n\t\t_hueTimer: %d\n"), 
    _effeNamIDList[(uint8_t)effect]._nameId, 
    isHuePalEffect(), 
    getHue_1(), 
    _hueTimer*_timerPalFactor);
}
bool EffectFx_Comet::run(){
  update();

  CRGB wavecolor;
  _color_fg = CHSV(_hue_1V, 255,  LAMPPTRGET()->get_globalBrightness(_strip_pos, brightness));

  if (dryrun(1)) return false;
      
  boolean reverse = _reverseStart;
  for (int A = 0; A < _maxSeg; ++A) {
    if ( _isSegment && _isSegmentEnabled) { 
      _pixstart = DevicePtrGet()->get_outputArray(_strip_pos)->_leds_segment.get_segment(A)->_start;
      _pixend   = DevicePtrGet()->get_outputArray(_strip_pos)->_leds_segment.get_segment(A)->_end;  
    }else{
      _pixstart = 0;
      _pixend   = _numLeds-1;
    }
    _pixrange = (_pixend - _pixstart+1);  

    for (int i = _pixstart; i <= _pixend; i++) {

      uint32_t pixelPos = (reverse)? (_pixrange-1) - i : i;
      if (_maxSeg > 1 && !_reverseStart) pixelPos = (_pixrange-1) - pixelPos;

      float ratio;
      if (!scale) {
        ratio = cubicwave8((255 * (pixelPos - _pixstart) / _pixrange) + _counter) / 255.0;
      } else {
        ratio = triwave8((255 * (pixelPos - _pixstart) / _pixrange) + _counter) / 255.0;
      }

      if (_hue_palette) {
      wavecolor = ColorFromPalette(_cTargetPalette, (pixelPos * 4) + _counter, LAMPPTRGET()->get_globalBrightness(_strip_pos, brightness), LINEARBLEND); // NOTE: paletteBlend should be global
      wavecolor = CRGB(wavecolor.r * ratio, wavecolor.g * ratio, wavecolor.b * ratio);        
      } else {
      wavecolor = CRGB(_color_fg.r * ratio, _color_fg.g * ratio, _color_fg.b * ratio);
      }

      // CRGB wavecolor = CRGB(_color_fg.r * ratio, _color_fg.g * ratio, _color_fg.b * ratio);
      // _pixset[i] = wavecolor;
      if (_isTw)  _ledarray_w[pixelPos] = wavecolor;
      else        _ledarray[pixelPos]   = wavecolor;    
    }

    _counter = (_direction) ? _counter + 2 : _counter - 2;

    reverse = !reverse;
  }
  return true;
}




String EffectFx_rainbowWave::setDynCtrl(UIControl*_val) {set_dynCtrl(_val); return String();}
void EffectFx_rainbowWave::load(uint8_t strip_pos){
  _counter = 0;
  // Serial.printf("_isSegment: %d,  _isSegmentEnabled: %d\n",_isSegment , _isSegmentEnabled);
}
bool EffectFx_rainbowWave::run(){
  if (dryrun(1)) return false;
      
  boolean reverse = false;
  for (int A = 0; A < _maxSeg; ++A) {
    if ( _isSegment && _isSegmentEnabled ) { 
      _pixstart = DevicePtrGet()->get_outputArray(_strip_pos)->_leds_segment.get_segment(A)->_start;
      _pixend   = DevicePtrGet()->get_outputArray(_strip_pos)->_leds_segment.get_segment(A)->_end;  
    }else{
      _pixstart = 0;
      _pixend   = _numLeds-1;
    }
    _pixrange = (_pixend - _pixstart+1);   

    float ratio = 255.0  / (_pixrange);
    for (uint16_t i = _pixstart; i < _pixend+1; i++) {

      // uint32_t pixelPos = i;
      uint16_t pixelPos = (reverse)? (_pixstart+_pixend) - i : i;
      // if (_maxSeg > 1 && !_reverseStart) pixelPos = (_pixrange-1) - pixelPos;
      // Serial.printf("reverse: %d | range: %-4d | start: %-4d | end: %-4d | pix: %-4d\n", reverse, _pixrange, _pixstart, _pixend+1, pixelPos);
      // uint16_t pPos = ( _pixstart + (_counter/scale) + i );
      uint16_t pPos = ( _pixstart + _counter ) * map(scale, 0, 255, 1, 6) + i;

      // uint8_t angle = (_varia == 255) ? 255 : (_varia > 127) ? 30 : 36 ;
// 30 36
      CRGB color = CHSV((uint8_t)( pPos   * ratio  )   , 255, LAMPPTRGET()->get_globalBrightness(_strip_pos, brightness));

      // CRGB color = CHSV((uint8_t)((60) * ratio), 255, 255);
      if (_isTw)  _ledarray_w[pixelPos] = color;
      else        _ledarray[pixelPos] = color;

      _counter = (_direction) ? _counter + 1 : _counter - 1;
    }
    
    
    reverse = (reverse)? false : true;
  }
  return true;
}



class heatMap {
public:
  uint8_t * pixels;
  byte * heat;
  heatMap(uint8_t size) {
    pixels  = new uint8_t[size+1];
    heat    = new byte[size+1];
  }
  ~heatMap() {
    delete[] pixels;
    delete[] heat;
  }
  void set_pixel(uint8_t pos, uint16_t value) {pixels[pos]=value;}
  void set_heat(uint8_t pos, uint16_t value) {heat[pos]=value;}
  uint16_t get_pixel(uint8_t pos) {return pixels[pos];}
  byte get_heat(uint8_t pos) {return heat[pos];}
};
LList<heatMap *>  heatMap_list;

EffectFx_fire2014:: ~EffectFx_fire2014() {
  while (heatMap_list.size()) {
    heatMap * item = heatMap_list.shift();
    if (item) delete item;
  }
    heatMap_list.clear();  
}    
String EffectFx_fire2014::setDynCtrl(UIControl*_val) {
  set_dynCtrl(_val);
  return String();
}
void EffectFx_fire2014::load(uint8_t strip_pos){
  boolean reverse = false;
  for (int A = 0; A < _maxSeg; A++) { 
    if ( _isSegment && _isSegmentEnabled ) { 
      _pixstart = DevicePtrGet()->get_outputArray(_strip_pos)->_leds_segment.get_segment(A)->_start;
      _pixend   = DevicePtrGet()->get_outputArray(_strip_pos)->_leds_segment.get_segment(A)->_end;  
    }else{
      _pixstart = 0;
      _pixend   = _numLeds-1;
    }
    _pixrange = (_pixend - _pixstart);   
    uint8_t cnt = 0;
    heatMap_list.add( new heatMap(_pixrange) );
    if (!reverse) {
      for (int i = _pixstart; i < _pixend; ++i) {
        heatMap_list.get(A)->set_pixel(cnt, i);
        cnt++;
      }
    } else {
      for(int i = _pixend ; i >= _pixstart; i--) { 
        heatMap_list.get(A)->set_pixel(cnt, i);
        cnt++;
      }  
    }
    reverse = !reverse;
  }
  FastLED.clear();
  FastLED.show();

}
#ifndef MIN
  #define MIN(X,Y) (((X)<(Y)) ? (X):(Y))
#endif
#define COOLING  55
#define SPARKING 120
bool EffectFx_fire2014::run(){
  if (dryrun(1)) return false;

  if (_isTw) fadeToBlackBy(_ledarray_w, _numLeds, _numLeds/2);
  else fadeToBlackBy(_ledarray, _numLeds, _numLeds/2);

  if ( _isSegment && _isSegmentEnabled ) { 
    _pixstart = DevicePtrGet()->get_outputArray(_strip_pos)->_leds_segment.get_segment(0)->_start;
    _pixend   = DevicePtrGet()->get_outputArray(_strip_pos)->_leds_segment.get_segment(0)->_end;  
  }else{
    _pixstart = 0;
    _pixend   = _numLeds-1;
  }
  _pixrange = (_pixend - _pixstart+1);   

  uint8_t cellsR    = random8(160,255);
  uint8_t coolR     = random8((((20 + speed/3) * 16) / _pixrange)+2);
  uint8_t spark     = random8();
  uint8_t ignition  = max(3,_pixrange/10); 
  uint8_t cellsY    = random8(ignition);

  bool sReverse = false;
  for (int A = 0; A < _maxSeg; A++) {
  // uint8_t A = 1;
    if ( _isSegment && _isSegmentEnabled ) { 
      _pixstart = DevicePtrGet()->get_outputArray(_strip_pos)->_leds_segment.get_segment(A)->_start;
      _pixend   = DevicePtrGet()->get_outputArray(_strip_pos)->_leds_segment.get_segment(A)->_end;  
    }else{
      _pixstart = 0;
      _pixend   = _numLeds-1;
    }
    _pixrange = (_pixend - _pixstart+1);   

    if (!coolR) coolR = random8((((20 + speed/3) * 16) / _pixrange)+2);
    // if (!coolR) coolR = random8(0, ((COOLING * 10) / _pixrange) + 2);

  // Step 1.  Cool down every cell a little
    for( int i = 0; i < _pixrange; i++) {
      byte Lheat = heatMap_list.get(A)->get_heat(i);
      uint8_t minTemp = 0;
      if (i<ignition) {
        //cool /= (ignition-i)/3 + 1;     // ignition area cools slower
        minTemp = (ignition-i)/4 + 16;  // and should not become black
      }
      uint8_t temp = qsub8(Lheat, coolR);
      heatMap_list.get(A)->set_heat(i, temp<minTemp ? minTemp : temp);   
    }
  
    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for( int k = _pixrange - 1; k > 1; k--) {
      // heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
      byte result = (heatMap_list.get(A)->get_heat(k - 1) + heatMap_list.get(A)->get_heat(k - 2) << 1) / 3;    
      heatMap_list.get(A)->set_heat(k, result);
    }
    
    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if( spark < LAMPPTRGET()->get_globalBrightness(0, brightness) ) {
      heatMap_list.get(A)->set_heat(cellsY, qadd8( heatMap_list.get(A)->get_heat(cellsY), cellsR ) );

    }

    // Step 4.  Map from heat cells to LED colors
    for( int j = 0; j < _pixrange; j++) {
      byte colorindex = MIN( heatMap_list.get(A)->get_heat(j), 240);
      CRGB color = ColorFromPalette(MagmaColor_p, colorindex, LAMPPTRGET()->get_globalBrightness(0, brightness));

      int pixelnumber = heatMap_list.get(A)->get_pixel(j);
      if (_isTw)  _ledarray_w[pixelnumber] = color;
      else        _ledarray[pixelnumber]   = color;

    }
  }
  return true;
}
/*
bool EffectFx_fire2014::run(){
  if (dryrun(1)) return false;

  if (_isTw) fadeToBlackBy(_ledarray_w, _numLeds, _numLeds/2);
  else fadeToBlackBy(_ledarray, _numLeds, _numLeds/2);


  int     cellsY  = random8(2);
  uint8_t cellsR  = random8(126,255);
  uint8_t coolR   = 0;
  uint8_t spark   = random8();

  bool sReverse = false;
  for (int A = 0; A < _maxSeg; A++) {
  // uint8_t A = 1;
    if ( _isSegment && _isSegmentEnabled ) { 
      _pixstart = DevicePtrGet()->get_outputArray(_strip_pos)->_leds_segment.get_segment(A)->_start;
      _pixend   = DevicePtrGet()->get_outputArray(_strip_pos)->_leds_segment.get_segment(A)->_end;  
    }else{
      _pixstart = 0;
      _pixend   = _numLeds-1;
    }
    _pixrange = (_pixend - _pixstart+1);   
    uint8_t ignition = max(3,_pixrange/10);
    
    if (!coolR) cool = random8((((20 + speed/3) * 16) / _pixrange)+2);
    // if (!coolR) coolR = random8(0, ((COOLING * 10) / _pixrange) + 2);

  // Step 1.  Cool down every cell a little
    for( int i = 0; i < _pixrange; i++) {
      // heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / _pixrange) + 2));
      byte Lheat = heatMap_list.get(A)->get_heat(i);
      heatMap_list.get(A)->set_heat(i, qsub8( Lheat,  coolR ));
    }
  
    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for( int k = _pixrange - 1; k >= 2; k--) {
      // heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
      byte result = (heatMap_list.get(A)->get_heat(k - 1) + heatMap_list.get(A)->get_heat(k - 2) + heatMap_list.get(A)->get_heat(k - 2)) / 3;    
      heatMap_list.get(A)->set_heat(k, result);
    }
    
    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if( spark < SPARKING ) {
      heatMap_list.get(A)->set_heat(cellsY, qadd8( heatMap_list.get(A)->get_heat(cellsY), cellsR ) );
    }

    // Step 4.  Map from heat cells to LED colors
    for( int j = 0; j < _pixrange; j++) {
      byte colorindex = scale8( heatMap_list.get(A)->get_heat(j), 240);
      CRGB color = ColorFromPalette(MagmaColor_p, colorindex, LAMPPTRGET()->get_globalBrightness(0, brightness));

      int pixelnumber = heatMap_list.get(A)->get_pixel(j);
      if (_isTw)  _ledarray_w[pixelnumber] = color;
      else        _ledarray[pixelnumber]   = color;

    }
  }
  return true;
}
*/


String EffectFx_ArmoireOutsideToInside::setDynCtrl(UIControl*_val) {set_dynCtrl(_val); return String();}
void EffectFx_ArmoireOutsideToInside::load(uint8_t strip_pos){}
bool EffectFx_ArmoireOutsideToInside::run(){
  
  LAMPPTRGET()->effectsLoop_TimerHue(_hueTimer*2.5);
  update();
      
  float hueInit = _hue_1V;
  float hue = hueInit;
  uint8_t segs = 0;
  uint8_t bri = LAMPPTRGET()->get_globalBrightness(_strip_pos, brightness);
  uint8_t bri_max = bri;
  uint8_t bri_l = bri_max;
  uint8_t bri_r = bri_max;


  aramoire_segemntPtr_get()->get_sideSegmentCnt(aramoire_side_left, segs);
  for (int j = 0; j < segs / 2; ++j){
    aramoire_segemnts * ptr = aramoire_segemntPtr_get()->get_sideSegment(aramoire_side_left, j);
    for (uint32_t i = ptr->_start; i < ptr->_stop; ++i){
      if (_isTw)  _ledarray_w[i]  = CHSV(hue,255,bri_l);
      else        _ledarray[i]    = CHSV(hue,255,bri_l);      
      hue+=map(_varia, 0, 255, 0, 15);
      bri_l -= bri_max/((ptr->_stop-ptr->_start)*2)-1 ;
    }
  }
  hue = hueInit;
  for (int j = 0; j < segs / 2; ++j){
    aramoire_segemnts * ptr = aramoire_segemntPtr_get()->get_sideSegment(aramoire_side_right, j);
    for (uint32_t i = ptr->_stop; i > ptr->_start; --i){
      if (_isTw)  _ledarray_w[i]  = CHSV(hue,255,bri_r);
      else        _ledarray[i]    = CHSV(hue,255,bri_r);   
      hue+=map(_varia, 0, 255, 0, 15);
      bri_r -= bri_max/((ptr->_stop-ptr->_start)*2)-1 ;
    }
  }
  uint8_t hueStart = hue;
  for (int j = segs / 2; j < segs; ++j){
    aramoire_segemnts * ptr = aramoire_segemntPtr_get()->get_sideSegment(aramoire_side_left, j);
    for (uint32_t i = ptr->_start; i < ptr->_stop; ++i){
      if (_isTw)  _ledarray_w[i]  = CHSV(hue,255,bri_l);
      else        _ledarray[i]    = CHSV(hue,255,bri_l);   
      hue-=map(_varia, 0, 255, 0, 15);
      bri_l += bri_max/((ptr->_stop-ptr->_start)*2)-1 ;
    }
    // hue+=3.5;
  } 
  hue = hueStart;
  for (int j = segs / 2; j < segs; ++j){
    aramoire_segemnts * ptr = aramoire_segemntPtr_get()->get_sideSegment(aramoire_side_right, j);
    for (uint32_t i = ptr->_stop; i > ptr->_start; --i){
      if (_isTw)  _ledarray_w[i]  = CHSV(hue,255,bri_r);
      else        _ledarray[i]    = CHSV(hue,255,bri_r);   
      hue-=map(_varia, 0, 255, 0, 15);
      bri_r += bri_max/((ptr->_stop-ptr->_start)*2)-1 ;
    }
  }
  return true;
}

/*
bool EffectFx_glow::run(){
  unsigned long now = millis();
  if (now - _lastupdate > _delay) {
    _lastupdate = now;

    if (DevicePtrGet()->get_outputArray(_strip_pos)->_leds_segment.isSegment()  ) { 

      uint8_t maxSeg = 0 ;
      DevicePtrGet()->get_outputArray(_strip_pos)->_leds_segment.get_max(maxSeg);

      for (int A = 0; A < maxSeg; ++A) {

        uint8_t start   = DevicePtrGet()->get_outputArray(_strip_pos)->_leds_segment.get_segment(A)->_start;
        uint8_t end     = DevicePtrGet()->get_outputArray(_strip_pos)->_leds_segment.get_segment(A)->_end;  

        _pixstart = start;
        _pixend = end;

        _pixrange = (end - start+1);   
        _pixaoe = 1;



          if (_pixrange % 2 == 0 && _pixaoe % 2 != 0) {
            _pixaoe++;
          } else if (_pixrange % 2 != 0 && _pixaoe % 2 == 0) {
            _pixaoe--;
          }

          float ratio = _counter / 100.0;
          CRGB glowcolor;
          glowcolor.r = _color_fg.r * ratio;
          glowcolor.g = _color_fg.g * ratio;
          glowcolor.b = _color_fg.b * ratio;

          if (_direction == _forward) {
            _counter++;
            if (_counter >= 100) _direction = _reverse;
          } else {
            _counter--;
            if (_counter <= 0) {
              Serial.println("RESET");
              _direction = _forward;
              if (!_repeat) {
                _lastupdate = 0;
              }
            }
          }          

          int glow_area_half = (_pixrange - _pixaoe) / 2;
          for (int i = 0; i < glow_area_half ; i++) {
            uint8_t denom = glow_area_half + 1 - i;
            CRGB tempcolor = CRGB(glowcolor.r / denom, glowcolor.g / denom, glowcolor.b / denom);

            if (_isTw) {
              _ledarray_w[_pixstart + i] = tempcolor;
              _ledarray_w[_pixend - i] = tempcolor;      
            }
            else {
              _ledarray[_pixstart + i] = tempcolor;
              _ledarray[_pixend - i] = tempcolor;       
            }
            for (int i = 0; i < _pixaoe; i++) {
              if (_isTw) {
                _ledarray_w[_pixstart + glow_area_half + i] = glowcolor;
              }
              else {
                _ledarray[_pixstart + glow_area_half + i] = glowcolor;
              }     
            }
          }
      }
    }
  }
  return true;
}
*/

String EffectFx_colorwipe::setDynCtrl(UIControl*_val) {
  set_dynCtrl(_val); 
  return String();
}
void EffectFx_colorwipe::load(uint8_t strip_pos){
  _stop     = _numLeds;
  _start    = 0;
  _seg_len  = (uint16_t)(_stop - _start );
  // Serial.printf("stop: %d, start: %d, len: %d\n", _stop, _start, _seg_len);

}

template<class X>
void colorswitchbounce(X * targetStrip, uint16_t start, uint16_t end, uint16_t numLeds, uint16_t delay, boolean reverse, boolean * hueChange, uint8_t h, uint8_t s, uint8_t v, uint8_t cv, uint8_t fade){

    static  int         pos     = 0;
    static  int         state   = 0;
    static  int         pass    = 0;
    static  boolean     dir     = true;
    static  uint8_t     hue     = 0;
    static  uint8_t     oldHue;
    static  long        lastChange;

    if (pass > 0 ) { 
        oldHue = h; 
        pass = 0; 
        switch (state){
            case 0: oldHue+=cv; break;
            case 1: oldHue+=cv; break;
        } 

    }
    hue =  oldHue;

    if ((millis() - lastChange) > delay )  { lastChange = millis();      }
    else                                   { *hueChange = false; return; }
    *hueChange = true;

    if(state > 1) { 
        state = 0;
    }

    fadeToBlackBy( targetStrip, numLeds, fade);
    switch (state){
        case 0: targetStrip[pos] = CHSV(hue+(cv), s, v); pos++; break;
        case 1: targetStrip[pos] = CHSV(hue+(cv*2), s, v); pos--;  break;
    } 

    if(pos < start)    { pos = start;    state++;pass++;}
    if(pos >= numLeds)  { pos = numLeds;  state++;pass++;}  

}

bool EffectFx_colorwipe::run() {
    update();
    boolean hueChange = true;
    colorswitchbounce<CRGBW>(_ledarray_w, 
        _start, _stop, _seg_len,
        speed, 
        true, 
        &hueChange, 
        _hue_1V, 
        255, 
        LAMPPTRGET()->get_globalBrightness(0, brightness),
        _varia,
        _fade
    );
    LAMPPTRGET()->_hsvLoop_1.hueChangeSet(hueChange);
  return true;
  
}




/*
  Aurora effect
*/

//CONFIG
#ifdef ESP8266
  #define W_MAX_COUNT  9          //Number of simultaneous waves
#else
  #define W_MAX_COUNT 20          //Number of simultaneous waves
#endif
#define W_MAX_SPEED 6             //Higher number, higher speed
#define W_WIDTH_FACTOR 6          //Higher number, smaller waves

//24 bytes
class AuroraWave {
  private:
    uint16_t ttl;
    CRGB basecolor;
    float basealpha;
    uint16_t age;
    uint16_t width;
    float center;
    bool goingleft;
    float speed_factor;
    bool alive = true;

  public:
    void init(uint32_t segment_length, CRGB color) {
      ttl = random(500, 1501);
      basecolor = color;
      basealpha = random(60, 101) / (float)100;
      age = 0;
      width = random(segment_length / 20, segment_length / W_WIDTH_FACTOR); //half of width to make math easier
      if (!width) width = 1;
      center = random(101) / (float)100 * segment_length;
      goingleft = random(0, 2) == 0;
      speed_factor = (random(10, 31) / (float)100 * W_MAX_SPEED / 255);
      alive = true;
    }

    CRGB getColorForLED(int ledIndex) {      
      if(ledIndex < center - width || ledIndex > center + width) return 0; //Position out of range of this wave

      CRGB rgb;

      //Offset of this led from center of wave
      //The further away from the center, the dimmer the LED
      float offset = ledIndex - center;
      if (offset < 0) offset = -offset;
      float offsetFactor = offset / width;

      //The age of the wave determines it brightness.
      //At half its maximum age it will be the brightest.
      float ageFactor = 0.1;        
      if((float)age / ttl < 0.5) {
        ageFactor = (float)age / (ttl / 2);
      } else {
        ageFactor = (float)(ttl - age) / ((float)ttl * 0.5);
      }

      //Calculate color based on above factors and basealpha value
      float factor = (1 - offsetFactor) * ageFactor * basealpha;
      rgb.r = basecolor.r * factor;
      rgb.g = basecolor.g * factor;
      rgb.b = basecolor.b * factor;
    
      return rgb;
    };

    //Change position and age of wave
    //Determine if its sill "alive"
    void update(uint32_t segment_length, uint32_t speed) {
      if(goingleft) {
        center -= speed_factor * speed;
      } else {
        center += speed_factor * speed;
      }

      age++;

      if(age > ttl) {
        alive = false;
      } else {
        if(goingleft) {
          if(center + width < 0) {
            alive = false;
          }
        } else {
          if(center - width > segment_length) {
            alive = false;
          }
        }
      }
    };

    bool stillAlive() {
      return alive;
    };
};
LList<AuroraWave *>  aurora_wave;

EffectFx_aurora:: ~EffectFx_aurora() {
  while (aurora_wave.size()) {
    AuroraWave * item = aurora_wave.shift();
    if (item) delete item;
  }
  aurora_wave.clear();  
}  
String EffectFx_aurora::setDynCtrl(UIControl*_val) {
  set_dynCtrl(_val); 
  if(_val->getId()==5) {
    _hue_palette = EffectCalc::setDynCtrl(_val).toInt();
    LAMPPTRGET()->set_palLoop(LAMPPTRGET()->effectsLoop_pal()); 
    if (_hue_palette && _effectType == OET_HUETIMER && _hue_1 == 0) 
      LAMPPTRGET()->effectsLoop_TimerPal(static_cast<uint32_t>(_hueTimer*_timerPalFactor));    
  }
  else if  (_val->getId()==2) {
    while (aurora_wave.size()) {
      AuroraWave * item = aurora_wave.shift();
      if (item) delete item;
    }
    aurora_wave.clear(); 
    _nb_Wave = map(scale, 0, 255, 2, W_MAX_COUNT);
    for (int i = 0; i < _nb_Wave; i++) {
      aurora_wave.add( new AuroraWave());
      if (isHuePalEffect()) {
          colors[0] = ColorFromPalette(_cTargetPalette, random8(), 255, LINEARBLEND); // NOTE: paletteBlend should be global
      } else {
          colors[0] = CHSV(_hue_1V, 255,255);
      } 
      aurora_wave.get(i)->init(_numLeds, colors[random(0,3)]);
    }    
  } 
  else EffectCalc::setDynCtrl(_val).toInt();  
  return String();
}
void EffectFx_aurora::load(uint8_t strip_pos){
  if (_hue_palette && _effectType == OET_HUETIMER && _hue_1 == 0) 
    LAMPPTRGET()->effectsLoop_TimerPal(static_cast<uint32_t>(_hueTimer*_timerPalFactor));
  Serial.printf_P(PSTR("\t[effect child load]\n\t\teff_es: %s\n\t\t_hue_palette: %d\n\t\t_hue_1: %d\n\t\t_hueTimer: %d\n"), 
    _effeNamIDList[(uint8_t)effect]._nameId, 
    isHuePalEffect(), 
    getHue_1(), 
    _hueTimer*_timerPalFactor);

  colors[0] = CRGB(0,0,0);
  colors[1] = CRGB(0,0,0);
  colors[2] = CRGB(0,0,0);

  _nb_Wave = map(scale, 0, 255, 2, W_MAX_COUNT);
  for (int i = 0; i < _nb_Wave; i++) {
    aurora_wave.add( new AuroraWave());
    if (isHuePalEffect()) {
        colors[0] = ColorFromPalette(_cTargetPalette, random8(), 255, LINEARBLEND); // NOTE: paletteBlend should be global
    } else {
        colors[0] = CHSV(_hue_1V, 255,255);
    }
    aurora_wave.get(i)->init(_numLeds, colors[random(0,3)]);
  }

}

bool EffectFx_aurora::run(void) {

  //aux1 = Wavecount
  //aux2 = Intensity in last loop

  //TODO: I am not sure this is a correct way of handling memory allocation since if it fails on 1st run
  // it will display static effect but on second run it may crash ESP since data will be nullptr
  update();
  if (dryrun(0.8)) return false;

  for (int i = 0; i < _nb_Wave; i++) {
    //Update values of wave
    aurora_wave.get(i)->update(_numLeds, speed);

    if(!(aurora_wave.get(i)->stillAlive())) {
      //If a wave dies, reinitialize it starts over.
      if (isHuePalEffect()) {
        colors[0] = ColorFromPalette(_cTargetPalette, random8(), 255, LINEARBLEND); // 
      } else {
        colors[0] = CHSV(_hue_1V, 255,255);
      }      
      aurora_wave.get(i)->init(_numLeds, colors[random(0,3)]);
    }
  }

  uint8_t backlight = 1; //dimmer backlight if less active colors
  if (colors[0]) backlight++;
  if (colors[1]) backlight++;
  if (colors[2]) backlight++;
  //Loop through LEDs to determine color
  for (int i = 0; i < _numLeds; i++) {    
    CRGB mixedRgb = CRGB(backlight, backlight, backlight);

    //For each LED we must check each wave if it is "active" at this position.
    //If there are multiple waves active on a LED we multiply their values.
    for (int  j = 0; j < _nb_Wave; j++) {
      CRGB rgb = aurora_wave.get(j)->getColorForLED(i);
      if(rgb != CRGB(0)) {       
        mixedRgb += rgb;
      }
    }

    // setPixelColor(i, mixedRgb[0], mixedRgb[1], mixedRgb[2]);
      if (_isTw)  _ledarray_w[i] = CRGB(mixedRgb.r,mixedRgb.g,mixedRgb.b);
      else        _ledarray[i]   = CRGB(mixedRgb.r,mixedRgb.g,mixedRgb.b);    
  }
  
  return true;
}