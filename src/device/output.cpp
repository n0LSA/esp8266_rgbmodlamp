#include "../../include/device/output.h"

#include "../../include/pixel/effectcore.h"
#include "../../include/display/lamp.h"
#include "../../include/device/device.h"



// #define fsprintf(parm_a, ...) Serial.printf_P( (PGM_P)PSTR(parm_a), ##__VA_ARGS__ )
// #define fsprintf(parm_a, ...) ALT_TRACEC(parm_a, __VA_ARGS__)

extern EffectWorker  * _effects[];


void leds_segment::segment_new(uint32_t start, uint32_t end, boolean dir) {
   _array[_cnt] = new leds_segments(start, end, dir); 
   _cnt++;
}

leds_segments::leds_segments(uint32_t start, uint32_t end, boolean dir){
  _start    = start;
  _end      = end;
  _reverse  = dir;
}
boolean leds_segment::isSegment(){
  if (_cnt>0) return true;
  return false;
}
void leds_segment::get_max(uint8_t &  max){
  max = _cnt;
}
leds_segments * leds_segment::get_segment(uint8_t pos){
  if (_array) return _array[pos];
  return nullptr;
}


aramoire_segemnts * aramoire_segemntsArray[8];
int         aramoire_segemntsArray_pos  = 0;

aramoire_segemnts::aramoire_segemnts(uint32_t start, uint32_t end, boolean dir){

  aramoire_segemntsArray[aramoire_segemntsArray_pos] = this;
  _start    = start;
  _stop     = end;
  _reverse  = dir;
  aramoire_segemntsArray_pos++;
}


aramoire_segemnt * aramoire_segemntPtr = nullptr;
aramoire_segemnt * aramoire_segemntPtr_get(){return aramoire_segemntPtr;}
aramoire_segemnt::aramoire_segemnt(){
  aramoire_segemntPtr = this;
}
void aramoire_segemnt::segemnt_new(uint32_t start, uint32_t end, boolean dir, const aramoire_side &side){
  if (side == aramoire_side_left) {
    _segements_left[_segements_left_pos] = new aramoire_segemnts(start, end, dir);
    _segements_left_pos++;
  }
  if (side == aramoire_side_right) {
    _segements_right[_segements_right_pos] = new aramoire_segemnts(start, end, dir);
    _segements_right_pos++;
  } 
  
  Serial.printf_P(PSTR("%4d - %4d - %4d - %4d\n"), start, end, dir, side);
}

/*
void aramoire_segemnt::segemnt_leftRight(int numleds){
  _left   = new uint16_t[(numleds*4)];
  _right  = new uint16_t[(numleds*4)];

  uint16_t count = 0;

  for (int j = 0; j < _segements_left_pos; ++j){
    aramoire_segemnts * ptr = _segements_left[j];
    for (uint32_t i = ptr->_start; i < ptr->_stop; ++i){
      _left[count] = i;
      count++;
    }
  }

  count = 0;
  for (int j = 0; j < _segements_right_pos; ++j) {
    aramoire_segemnts * ptr = _segements_right[j];
    for (uint32_t i = ptr->_stop; i > ptr->_start; --i){
      _right[count] = i;
      count++;
    }
  }

  Serial.printf_P(PSTR("\n[CNT][%d]\n"), numleds);
  Serial.printf_P(PSTR("\n[_left]\n"));
  for (int i = 0; i < (numleds*4); ++i) { Serial.printf_P(PSTR("[%10d] %d\n"), i, _left[i] ); }
  Serial.printf_P(PSTR("\n[_right]\n"));
  for (int i = 0; i < (numleds*4); ++i) { Serial.printf_P(PSTR("[%10d] %d\n"), i, _right[i] ); } 

  boolean stripW;
  leds_managePtrGet()->get_ledsIsTw(0, stripW);

    CRGBW * ledarray_w  = leds_managePtrGet()->get_crgwbArray(0);
    CRGB  * ledarray    = leds_managePtrGet()->get_crgbArray(0);

    uint8_t segs = 0;
    aramoire_segemntPtr_get()->get_segmentCnt(segs);
 
  // for (int j = 0; j < segs / 2; ++j){
  //   aramoire_segemnts * ptr = _segements_left[j];
  //   for (uint32_t i = ptr->_start; i < ptr->_stop; ++i){
  //     if (stripW) ledarray_w[i] = ptr->_c1;
  //     else        ledarray[i]   = ptr->_c1;
  //   }
  //   ptr = _segements_right[j];
  //   for (uint32_t i = ptr->_start; i < ptr->_stop; ++i){
  //     if (stripW) ledarray_w[i] = ptr->_c1;
  //     else        ledarray[i]   = ptr->_c1;
  //   }

  // }

  // CRGBW * array = leds_managePtrGet()->get_crgwbArray(0);
  // float hueInit = 80;
  // float hue = hueInit;
  // uint8_t bri_max = 100;
  // uint8_t bri_l = bri_max;
  // uint8_t bri_r = bri_max;
  // uint8_t briDec = bri_max/(numleds*2)-1;
  // // briDec-=1;

  // for (int j = 0; j < _segements_left_pos / 2; ++j){
  //   aramoire_segemnts * ptr = _segements_left[j];
  //   for (uint32_t i = ptr->_start; i < ptr->_stop; ++i){
  //     array[i] = CHSV(hue,255,bri_l);
  //     bri_l -= briDec ;
  //     hue+=1.5;
  //   }
  // }
  // hue = hueInit;
  for (int j = 0; j < _segements_right_pos ; ++j){
    aramoire_segemnts * ptr = _segements_right[j];
    for (uint32_t i = ptr->_stop; i > ptr->_start; --i){
      // array[i] = CHSV(hue,255,bri_r);
      // bri_r -= briDec ;
      // hue+=1.5;
      if (stripW) ledarray_w[i] = ptr->_c1;
      else        ledarray[i]   = ptr->_c1;      
    }
  }
  // uint8_t hueStart = hue;
  // for (int j = _segements_left_pos / 2; j < _segements_left_pos; ++j){
  //   aramoire_segemnts * ptr = _segements_left[j];
  //   for (uint32_t i = ptr->_start; i < ptr->_stop; ++i){
  //     array[i] = CHSV(hue,255,bri_l);
  //     hue-=1.5;
  //     bri_l += briDec;
  //   }
  // } 
  // hue = hueStart;
  // for (int j = _segements_left_pos / 2; j < _segements_left_pos; ++j){
  //   aramoire_segemnts * ptr = _segements_right[j];
  //   for (uint32_t i = ptr->_stop; i > ptr->_start; --i){
  //     array[i] = CHSV(hue,255,bri_r);
  //     bri_r += briDec;
  //     hue-=1.5;
  //   }
  // }

  FastLED.show();



}
*/

void aramoire_segemnt::get_segmentCnt(uint8_t &  max){
  max = aramoire_segemntsArray_pos;
}
aramoire_segemnts * aramoire_segemnt::get_sideSegment(aramoire_side dir, int pos){
  return (dir==aramoire_side_left) ? _segements_left[pos] : _segements_right[pos];
}
aramoire_segemnts * aramoire_segemnt::get_segment(int pos){
  return aramoire_segemntsArray[pos];
}
void aramoire_segemnt::get_sideSegmentCnt(aramoire_side dir, uint8_t & ret){
  ret = (dir==aramoire_side_left) ? _segements_left_pos : _segements_right_pos;
}
void aramoire_segemnt::isSegemnt(boolean & ret){
  ret = aramoire_segemntsArray_pos;
}
boolean aramoire_segemnt::isSegemnt(){
  return aramoire_segemntsArray_pos;
}




/**
 * @brief      convert hex to uint8_t
 *
 * @param[in]  source  maximum 1octet coded in 3charactere (%03x)
 * @param      start   postion to start extraction of 3chars
 *
 * @return     value to uint8_t
 */
uint8_t getHexS(const String & source, uint8_t & start){ 
  String s_1 = String(source[start])+String(source[start+1])+String(source[start+2]);
  start += 3;
  return (uint8_t)strtol(s_1.c_str(), NULL, 16);  
}
uint8_t getHex(const String & source, uint8_t start){ 
  String s_1 = String(source[start])+String(source[start+1])+String(source[start+2]);
  return (uint8_t)strtol(s_1.c_str(), NULL, 16);  
}

boolean ToBool(uint8_t v) {
  if (v == 0) return false;
  return true;
}

/**
 * @brief      Constructs a new instance.
 */
Output::Output(){

  _c1 = CHSV(random8(), random(200, 255), random(60, 180) );
  _coff = _c1;
  _eff_c1 = CRGB(0, 255, 0);
  _eff_c2 = CRGB(0, 0, 255);
}

/**
 * @brief      Destroys the object.
 */
Output::~Output(){
}

/**
 * @brief      Sets the value from json.
 *
 * @param[in]  doc   The new value
 */
/*
void Output::setValueFromJson(DynamicJsonDocument doc){
  String name = doc[F("name")].as<String>();

  clear();
  leds_managePtrGet()->showLeds(_pos, 0);

  _select   = doc[F("9")].as<boolean>();
  _onoff    = doc[F("10")].as<boolean>();

  _bri      = doc[F("8")].as<uint8_t>();
  _isTw     = doc[F("7")].as<boolean>();

  _hsv_h    = doc[F("0")].as<uint8_t>();
  _hsv_s    = doc[F("1")].as<uint8_t>();
  _hsv_v    = doc[F("2")].as<uint8_t>();

  _rgb_r    = doc[F("3")].as<uint8_t>();
  _rgb_g    = doc[F("4")].as<uint8_t>();
  _rgb_b    = doc[F("5")].as<uint8_t>();

  _tw       = doc[F("6")].as<uint8_t>();
  _tw_v     = doc[F("15")].as<uint8_t>(); 

  JsonObject c1 = doc["c1"];
  _c1 = CRGB(c1[F("r")].as<uint8_t>(), c1[F("g")].as<uint8_t>() , c1[F("b")].as<uint8_t>());

  JsonObject c2 = doc["coff"];
  _coff = CRGB(c2[F("r")].as<uint8_t>(), c2[F("g")].as<uint8_t>() , c2[F("b")].as<uint8_t>());

  if (doc.containsKey(F("16"))) _eff_activ = doc[F("16")].as<boolean>();  
  
  if (doc.containsKey(F("eff_c1"))){
    JsonObject effc1 = doc["eff_c1"];
    _eff_c1 = CRGB(effc1[F("r")].as<uint8_t>(), effc1[F("g")].as<uint8_t>() , effc1[F("b")].as<uint8_t>());   
  }
  if (doc.containsKey(F("eff_c2"))){
    JsonObject effc2 = doc["eff_c2"];
    _eff_c2 = CRGB(effc2[F("r")].as<uint8_t>(), effc2[F("g")].as<uint8_t>() , effc2[F("b")].as<uint8_t>());   
  }

  if (doc.containsKey(F("18"))) _eff_colorMod = doc[F("18")].as<uint8_t>(); 


  set_color();

  if ( _eff_activ && doc.containsKey(F("17")) ) {
    if (_effects[_pos]){
        char buffer[80];
        LAMPPTRGET()->getStripFolder(_pos, buffer);
        uint16_t effPos = doc[F("17")].as<uint16_t>();
        _effects[_pos]->workerset(_pos, effPos, true, true, buffer, false);       
    }
  }

}
*/
void Output::armoiureFromSpiff(){

  DynamicJsonDocument armoire(2048);

  char buffer[255];
  sprintf(buffer, "/outputs/armoire_%d.json", _pos);

  if (!AP_deserializeFile(armoire, buffer)) return;

  // serializeJsonPretty(armoire, Serial);Serial.println();
  
  // _armoireEtage = armoire[F("select")]["0"].as<uint8_t>();
  
  uint8_t segs = 0;
  _armoire.get_segmentCnt(segs);  
  for (uint8_t j = 0; j < segs; ++j){

    aramoire_segemnts * ptr = _armoire.get_segment(j);

    int rSize = 0;

    String c = armoire[F("etage")][String(j)].as<String>();
    const char** split = al_tools::explode(c, '.', rSize);
    if (split) {
      uint8_t r = atoi(split[0]);
      uint8_t g = atoi(split[1]);
      uint8_t b = atoi(split[2]);
      for(int i = 0; i < rSize; ++i) {
        delete split[i];
      }
      delete[] split;    
      ptr->_c1 = CRGB(r,g,b);      
    }

    String tw = armoire[F("etage")]["tw_" + String(j)].as<String>();
    rSize = 0;
    split = al_tools::explode(tw, '.', rSize);
    if (split) {
      ptr->_isTw  = atoi(split[0]);
      ptr->_tw    = atoi(split[1]);
      ptr->_tw_v  = atoi(split[2]);
      for(int i = 0; i < rSize; ++i) {
        delete split[i];
      }
      delete[] split;    
    }

    String hsv = armoire[F("etage")]["hsv_" + String(j)].as<String>();
    rSize = 0;
    if (split) {
      split = al_tools::explode(hsv, '.', rSize);
      ptr->_hsv_h = atoi(split[0]);
      ptr->_hsv_s = atoi(split[1]);
      ptr->_hsv_v = atoi(split[2]);
      for(int i = 0; i < rSize; ++i) {
        delete split[i];
      }
      delete[] split;  
    }
  } 
}
void Output::setValueFromSpiff(const String & path){


  armoiureFromSpiff();

  clear();
  set_color();
  // leds_managePtrGet()->showLeds(_pos, _bri);

  if (!FILESYSTEM.exists(path)) return;

  uint8_t linePos = _pos+1;

  File f;

  LH_file * _LH_file = new LH_file(f, path.c_str());

  String lineRead;
  uint32_t lrSize = _LH_file->readLine(linePos, lineRead);

  delete _LH_file;

  if (lrSize < 87) return;

  leds_managePtrGet()->showLeds(_pos, 0);
  
  _c1           = CRGB(getHex(lineRead, 0), getHex(lineRead, 3), getHex(lineRead, 6));
  _coff         = CRGB(getHex(lineRead, 10), getHex(lineRead, 13), getHex(lineRead, 16));

  _hsv_h        = getHex(lineRead, 20);
  _hsv_s        = getHex(lineRead, 23);
  _hsv_v        = getHex(lineRead, 26);

  _rgb_r        = getHex(lineRead, 30);
  _rgb_g        = getHex(lineRead, 33);
  _rgb_b        = getHex(lineRead, 36);

  _tw           = getHex(lineRead, 40);
  _isTw         = ToBool(getHex(lineRead, 43));
  _tw_v         = getHex(lineRead, 46);

  _select       = ToBool(getHex(lineRead, 50));
  _onoff        = ToBool(getHex(lineRead, 53));


  _eff_colorMod = getHex(lineRead, 60);
  _eff_activ    = ToBool(getHex(lineRead, 63));

  if ( _eff_activ )  {
    if (_effects[_pos]){
      uint16_t effPos  = getHex(lineRead, 57);
      ProgramPtrGet()->set_itemById(effPos);  
      LAMPPTRGET()->effectChangeByPos(_pos, effPos, LAMPPTRGET()->isFaderOn(), false);
    }
  }

  _eff_c1       = CRGB(getHex(lineRead, 67), getHex(lineRead, 70), getHex(lineRead, 73));
  _eff_c2       = CRGB(getHex(lineRead, 77), getHex(lineRead, 80), getHex(lineRead, 83));

  _briEff       = getHex(lineRead, 87);

  set_color();

  leds_managePtrGet()->showLeds(_pos, _bri); 
}

void Output::set_pos(uint8_t p) {
  _pos = p;
} 
void Output::set_name       (String v)    {_name  = v;}
void Output::set_type       (uint8_t v)   {_type  = v;}
void Output::set_pin        (uint8_t v)   {_pin   = v;}
void Output::set_size       (uint16_t v)  {_size  = v;}
void Output::set_bri        (uint8_t v)   {_bri   = v;};
void Output::set_effActiv   (boolean v)   {_eff_activ   = v;};

String    Output::get_name        () {return _name;}
uint8_t   Output::get_type        () {return _type;}
uint8_t   Output::get_pin         () {return _pin;}
uint16_t  Output::get_size        () {return _size;}

boolean   Output::get_select      (){return _select;};
boolean   Output::get_onoff       (){return _onoff;};
uint8_t   Output::get_bri         (){return _bri;};
uint8_t   Output::get_briEff      (){return _briEff;};
uint8_t   Output::get_briOld      (){return _briOld;};
boolean   Output::get_isTw        (){return _isTw;};
uint8_t   Output::get_tw          (){return _tw;};
uint8_t   Output::get_tw_v        (){return _tw_v;};
uint8_t   Output::get_hsv_h       (){return _hsv_h;};
uint8_t   Output::get_hsv_s       (){return _hsv_s;};
uint8_t   Output::get_hsv_v       (){return _hsv_v;};
uint8_t   Output::get_rgb_r       (){return _rgb_r;};
uint8_t   Output::get_rgb_g       (){return _rgb_g;};
uint8_t   Output::get_rgb_b       (){return _rgb_b;};
CRGB      Output::get_c1          (){return _c1;};
CRGB      Output::get_coff        (){return _coff;};

boolean   Output::get_eff_activ   (){return _eff_activ;};

boolean Output::set_eff_autoplay(const String & cmd, const JsonObject & value){
  if (cmd != FPSTR(req_eff_autoplay)) return false;
  if (!ProgramPtrGet())               return false;
  boolean v = value[F("v")].as<boolean>(); 
  if (v)  ProgramPtrGet()->remote_action(apSetter_t::APSET_PLAY_START);
  else    ProgramPtrGet()->remote_action(apSetter_t::APSET_PLAY_STOP);
  return true;
}

/*
void Output::set_eff_f(const String & cmd, const JsonObject & value){
  if (cmd != FPSTR(req_eff_f))  return;
  if (!RemoteControlPtrGet())   return;
  if (!ProgramPtrGet())         return;

  String action = value[F("c")].as<String>();
  if ((action == FPSTR(req_eff_next)))       {_eff_activ = true;RemoteControlPtrGet()->eff_next();}
  else if ((action == FPSTR(req_eff_prev)))  {_eff_activ = true;RemoteControlPtrGet()->eff_prev();} 
  else if ((action == FPSTR(req_eff_load))) {
    _eff_activ = true;
    uint8_t p = value[F("v")].as<uint8_t>();
    LAMPPTRGET()->effectChangeByPos(_pos, p, LAMPPTRGET()->isFaderOn(), true);
    ProgramPtrGet()->set_itemById(p);
  }  
}
*/

boolean Output::set_etage(const String & cmd, const JsonObject & value){ // eff_avtiv
  uint8_t v = value[F("v")].as<uint8_t>();
  if (cmd == FPSTR(req_etage)) {
    _armoireEtage = v-1;
    set_colorArmoireToOutput();   
    return true;
  }
  if (cmd == FPSTR(req_etages)) {
    _armoireEtage = v;
    set_colorArmoireToOutput();  
    return true;  
  }
  return false;
}

boolean Output::set_eff_sav(const String & cmd, const JsonObject & value){
  if (cmd != FPSTR(req_eff_sav)) return false;
  LAMPPTRGET()->effectSav(_pos);
  return true;
}
boolean Output::set_eff_activ(const String & cmd, const JsonObject & value){ // eff_avtiv
  if (cmd != FPSTR(req_eff_activ)) return false;
  uint8_t v = value[F("v")].as<uint8_t>();
  clear();
  if (!_eff_activ && (v == 0) ) {
    _eff_activ = true;
  } 
  if (_eff_activ && (v == 1) ) _eff_activ = false;
  return true;
}
boolean Output::set_eff_speedIncr(const String & cmd, const JsonObject & value){
  if (cmd != FPSTR(req_eff_speedIncr)) return false;
  int8_t v = value[F("v")].as<int8_t>();
  LAMPPTRGET()->effect_setspd_incr(_pos, v);
  return true;
}
boolean Output::set_eff_speed(const String & cmd, const JsonObject & value){
  if (cmd != FPSTR(req_eff_speed)) return false;
  uint8_t v = value[F("v")].as<uint8_t>();
  LAMPPTRGET()->effect_setspd(_pos, v);
  return true;
}
boolean Output::set_eff_bri(const String & cmd, const JsonObject & value){
  if (cmd != FPSTR(req_eff_bri)) return false;
  uint8_t v = value[F("v")].as<uint8_t>();
  LAMPPTRGET()->effect_setbri(_pos, v);
  return true;
}
boolean Output::set_eff_scale(const String & cmd, const JsonObject & value){
  if (cmd != FPSTR(req_eff_scale)) return false;
  uint8_t v = value[F("v")].as<uint8_t>();
  LAMPPTRGET()->effect_setscale(_pos, v);
  return true;
}
boolean Output::set_eff_id(const String & cmd, const JsonObject & value){
  if (cmd != FPSTR(req_eff_id)) return false;
  JsonArray array = value[F("A")];
  if (array && array.size() > 0) {
    for(uint8_t i = 0; i < array.size(); i++) {
        uint8_t v   = array[i][F("v")].as<uint8_t>();
        uint8_t id  = array[i][F("id")].as<uint8_t>(); 
        LAMPPTRGET()->effect_id(_pos, id, v);           
    }
  } else {
    uint8_t v   = value[F("v")].as<uint8_t>();
    uint8_t id  = value[F("id")].as<uint8_t>();
    LAMPPTRGET()->effect_id(_pos, id, v);    
  }
  return true;
}  
boolean Output::set_eff_colorMod(const String & cmd, const JsonObject & value){
  if (cmd != FPSTR(req_eff_effColorMod)) return false;
  uint8_t v = value[F("v")].as<uint8_t>();
  _eff_colorMod = v;
  return true;
}

boolean Output::set_select(const String & cmd, const JsonObject & value){
  if (cmd != FPSTR(req_s_output)) return false;
  uint8_t v = value[F("v")].as<uint8_t>();
  (v == 1) ? _select = true : _select = false;
  return true;
}

boolean Output::set_toggle(const String & cmd, const JsonObject & value){
  if (cmd != FPSTR(req_toglle)) return false;
  onoff();
  return true;
}
boolean Output::set_onoff(const String & cmd, const JsonObject & value){
  if (cmd != FPSTR(req_lampOnOff)) return false;
  onoff(value);
  return true;
}

boolean Output::set_bri(const String & cmd, const JsonObject & value){
  if (cmd != FPSTR(req_ledBri)) return false;
  uint8_t v = value[F("v")].as<uint8_t>(); 
  set_briGen(v);
  return true;
}
boolean Output::set_briIncr(const String & cmd, const JsonObject & value){
  if (cmd != FPSTR(req_ledBriIncr)) return false;
  int8_t v = value[F("v")].as<int8_t>(); 
  set_briIncr(v);
  return true;
}

boolean Output::set_tw(const String & cmd, const JsonObject & value){
  if (cmd != FPSTR(req_lampWhite)) return false;
  if (!get_onoff()) return false;
  set_color_tw(value); 
  return true;
}
boolean Output::set_tw_f(const String & cmd, const JsonObject & value){
  if (cmd != FPSTR(req_lampWhite_f)) return false;
  if (!get_onoff()) return false;
  _eff_activ = false;
  clear();
  set_color_tw(_tw); 
  return true;
}  
boolean Output::set_tw_v(const String & cmd, const JsonObject & value){
  if (cmd != FPSTR(req_lampWhite_v)) return false;
  if (!get_onoff()) return false;
  _eff_activ = false;
  clear();  
  set_color_tw_v(value);
  return true;
}
boolean Output::set_hsv_h(const String & cmd, const JsonObject & value){
  if (cmd != FPSTR(req_lampHue)) return false;
  if (!get_onoff()) return false;   
  set_color_h(value);
  return true;
}
boolean Output::set_hsv_hIncr(const String & cmd, const JsonObject & value){
  if (cmd != FPSTR(req_lampHueIncr)) return false;
  if (!get_onoff()) return false;   
  set_color_hIncr(value);
  return true;
}

boolean Output::set_hsv_s(const String & cmd, const JsonObject & value){
  if (cmd != FPSTR(req_lampSat)) return false;
  if (!get_onoff()) return false;
  set_color_s(value);
  return true;
}
boolean Output::set_hsv_v(const String & cmd, const JsonObject & value){
  if (cmd != FPSTR(req_lampBri)) return false;
  if (!get_onoff()) return false;
  set_color_v(value);
  return true;
}

boolean Output::set_rgb_f(const String & cmd, const JsonObject & value){
  if (cmd != FPSTR(req_lampSetColor_f)) return false;
  if (!get_onoff()) return false;
  _eff_activ = false;
  clear();
  set_color_rgb(value,false);
  return true;
}  
boolean Output::set_rgb(const String & cmd, const JsonObject & value){
  if (cmd != FPSTR(req_lampSetColor)) return false;
  if (!get_onoff()) return false;
  // Serial.println("set_rgb");
  set_color_rgb(value,false);
  return true;
}

boolean Output::set_eff_rgb1(const String & cmd, const JsonObject & value){
  if (cmd != FPSTR(req_lampSetColor)) return false;
  if (!get_onoff()) return false;
  set_color_eff_rgb1(value);
  return true;
}



void Output::clear() {
  ledsSet::fill(_pos, CRGB(0,0,0));
}
void Output::onoff() {
  _onoff = !_onoff;
  if (_onoff) {
    _c1 = _coff;
    // set_colorArmoire();
    if (!_eff_activ) {set_color();leds_managePtrGet()->showLeds(_pos, _bri);}
    LAMPPTRGET()->changePower(_pos, true);
  } else {
    _coff = _c1;
    _c1 = CRGB(0,0,0);
    // set_colorArmoire();
    ledsSet::fill(_pos, CRGB(0,0,0));
    leds_managePtrGet()->showLeds(_pos, 0);
    LAMPPTRGET()->changePower(_pos, false);
  }
}
void Output::onoff(const JsonObject & value) {
  uint8_t v = value[F("v")].as<uint8_t>();
  if (!_onoff && (v == 0) ) {
    _onoff = true;
    _c1 = _coff;
    // set_colorArmoire();
    if (!_eff_activ) {set_color();leds_managePtrGet()->showLeds(_pos, _bri);}
    LAMPPTRGET()->changePower(_pos, true);
  } 
  else if (_onoff && (v == 1) ) {
    _onoff = false;
    _coff = _c1;
    _c1 = CRGB(0,0,0);
    // set_colorArmoire();
    ledsSet::fill(_pos, CRGB(0,0,0));
    leds_managePtrGet()->showLeds(_pos, 0);
    LAMPPTRGET()->changePower(_pos, false);
  }
}

void Output::set_color(CRGB c, int tw) {
  boolean stripW;
  leds_managePtrGet()->get_ledsIsTw(_pos, stripW);

  if (_isArmoire) {

    CRGBW * ledarray_w  = leds_managePtrGet()->get_crgwbArray(_pos);
    CRGB  * ledarray    = leds_managePtrGet()->get_crgbArray(_pos);

    uint8_t segs = 0;
    _armoire.get_segmentCnt(segs);

    for (int j = 0; j < segs/2; ++j){
      aramoire_segemnts * ptr = _armoire.get_sideSegment(aramoire_side_left, j);
      for (uint32_t i = ptr->_start; i < ptr->_stop; ++i){
        if (ptr->_isTw && stripW) {
          if (tw>=0) {
            ledarray_w[i] = CRGBW(0, 0, 0, tw);  
          } else ledarray_w[i] = c; 
        } else {
          if (stripW) ledarray_w[i] = c;
          else        ledarray[i]   = c;
        } 
      }
    }

    for (int j = 0; j < segs/2 ; ++j){
      aramoire_segemnts * ptr = _armoire.get_sideSegment(aramoire_side_right, j);
      for (uint32_t i = ptr->_stop; i > ptr->_start; --i){
        if (ptr->_isTw && stripW) {
          if (tw>=0) {
            ledarray_w[i] = CRGBW(0, 0, 0, tw);  
          } else ledarray_w[i] = c;
        } else {
          if (stripW) ledarray_w[i] = c;
          else        ledarray[i]   = c;
        }          
      }
    }
    return;
  }

  if (_isTw && stripW) {
    if (tw>=0) {
      ledsSet::fill(_pos, CRGBW(0, 0, 0, tw));
    } else ledsSet::fill(_pos, c);
  } else ledsSet::fill(_pos, c);
}

void Output::set_color() {
  
  boolean stripW;
  leds_managePtrGet()->get_ledsIsTw(_pos, stripW);

  if (_isArmoire) {

    CRGBW * ledarray_w  = leds_managePtrGet()->get_crgwbArray(_pos);
    CRGB  * ledarray    = leds_managePtrGet()->get_crgbArray(_pos);

    uint8_t segs = 0;
    _armoire.get_segmentCnt(segs);

    for (int j = 0; j < segs/2; ++j){
      aramoire_segemnts * ptr = _armoire.get_sideSegment(aramoire_side_left, j);
      for (uint32_t i = ptr->_start; i < ptr->_stop; ++i){
        if (ptr->_isTw && stripW) {
          if (_onoff) {
            ledarray_w[i] = CRGBW(0, 0, 0, ptr->_tw);  
          } else ledarray_w[i] = (_onoff) ? ptr->_c1 : CRGB(0,0,0); 
        } else {
          if (stripW) ledarray_w[i] = (_onoff) ? ptr->_c1 : CRGB(0,0,0);
          else        ledarray[i]   = (_onoff) ? ptr->_c1 : CRGB(0,0,0);
        } 
      }
    }

    for (int j = 0; j < segs/2 ; ++j){
      aramoire_segemnts * ptr = _armoire.get_sideSegment(aramoire_side_right, j);
      for (uint32_t i = ptr->_stop; i > ptr->_start; --i){
        if (ptr->_isTw && stripW) {
          if (_onoff) {
            ledarray_w[i] = CRGBW(0, 0, 0, ptr->_tw);  
          } else ledarray_w[i] = (_onoff) ? ptr->_c1 : CRGB(0,0,0);
        } else {
          if (stripW) ledarray_w[i] = (_onoff) ? ptr->_c1 : CRGB(0,0,0);
          else        ledarray[i]   = (_onoff) ? ptr->_c1 : CRGB(0,0,0);
        }          
      }
    }
    return;
  }

  if (_isTw && stripW) {
    if (_onoff) {
      // if (_isArmoire) for(uint8_t i = 0; i < 4; ++i) {set_color(i, CRGBW(0, 0, 0, _tw));}
      // else ledsSet::fill(_pos, CRGBW(0, 0, 0, _tw));
      ledsSet::fill(_pos, CRGBW(0, 0, 0, _tw));
    }
    else ledsSet::fill(_pos, _c1);
  } else ledsSet::fill(_pos, _c1);
}

void Output::set_briArmoire(int8_t amount, boolean add){
  if (!_isArmoire) return;

  boolean stripW;
  leds_managePtrGet()->get_ledsIsTw(_pos, stripW);

  if (_armoireEtage == 0) {
    uint8_t segs = 0;
    _armoire.get_segmentCnt(segs);
    for (uint8_t j = 0; j < segs; ++j){
      aramoire_segemnts * ptr = _armoire.get_segment(j);

      if (ptr->_isTw && stripW) {
        if (_onoff) {
          ptr->_tw = ((add) ? ptr->_tw : 0) + amount;
          ptr->_hsv_v = ptr->_tw;
          CRGB hsv = CHSV(ptr->_hsv_h, ptr->_hsv_s, ptr->_hsv_v);
          ptr->_rgb_r = hsv.r;
          ptr->_rgb_g = hsv.g;
          ptr->_rgb_b = hsv.b;        
        }
      } else {
        if (ptr->_isTw) {
          ptr->_tw = ((add)? ptr->_tw : 0) + amount;
          ptr->_hsv_v = ptr->_tw;
          CRGB hsv = CHSV(ptr->_hsv_h, ptr->_hsv_s, ptr->_hsv_v);
          ptr->_rgb_r = hsv.r;
          ptr->_rgb_g = hsv.g;
          ptr->_rgb_b = hsv.b;         
          ptr->_c1 = CHSV(
            45U,                                                                              
            map(ptr->_tw_v, 0U, 255U, 0U, 170U),                           
            ptr->_tw
          ); 
          ptr->_coff = ptr->_c1;
        } else {
          ptr->_hsv_v = ((add)? ptr->_hsv_v : 0) + amount;
          ptr->_tw = ptr->_hsv_v;
          CRGB hsv = CHSV(ptr->_hsv_h, ptr->_hsv_s, ptr->_hsv_v);
          ptr->_rgb_r = hsv.r;
          ptr->_rgb_g = hsv.g;
          ptr->_rgb_b = hsv.b;   
          ptr->_c1 = CRGB(ptr->_rgb_r, ptr->_rgb_g, ptr->_rgb_b);     
          ptr->_coff = ptr->_c1;
        }
      }
    }   
  } else {
    set_colorArmoire();
  }
}

void Output::set_colorArmoireToOutput(){
  if (!_isArmoire) return; 
  if (_armoireEtage == 0) return;

  aramoire_segemnts * ptr = _armoire.get_sideSegment(aramoire_side_right, _armoireEtage-1);


  _isTw       = ptr->_isTw; 
  _tw         = ptr->_tw;
  _tw_v       = ptr->_tw_v;
  _hsv_h      = ptr->_hsv_h;
  _hsv_s      = ptr->_hsv_s;
  _hsv_v      = ptr->_hsv_v;
  _rgb_r      = ptr->_rgb_r;
  _rgb_g      = ptr->_rgb_g;
  _rgb_b      = ptr->_rgb_b;
  _c1         = ptr->_c1;
  _coff       = ptr->_coff;   

}

void Output::set_colorArmoire(){
  if (!_isArmoire) return;
  if (_armoireEtage == 0) {
    uint8_t segs = 0;
    _armoire.get_segmentCnt(segs);
    for (uint8_t j = 0; j < segs; ++j){
      aramoire_segemnts * ptr = _armoire.get_segment(j);
      ptr->_isTw  = _isTw;
      ptr->_tw    = _tw;
      ptr->_tw_v  = _tw_v;
      ptr->_hsv_h = _hsv_h;
      ptr->_hsv_s = _hsv_s;
      ptr->_hsv_v = _hsv_v;
      ptr->_rgb_r = _rgb_r;
      ptr->_rgb_g = _rgb_g;
      ptr->_rgb_b = _rgb_b;
      ptr->_c1    = _c1;
      ptr->_coff  = _coff;
    }   
  } else {
    aramoire_segemnts * ptr = _armoire.get_sideSegment(aramoire_side_left, _armoireEtage-1);
      ptr->_isTw  = _isTw;
      ptr->_tw    = _tw;
      ptr->_tw_v  = _tw_v;
      ptr->_hsv_h = _hsv_h;
      ptr->_hsv_s = _hsv_s;
      ptr->_hsv_v = _hsv_v;
      ptr->_rgb_r = _rgb_r;
      ptr->_rgb_g = _rgb_g;
      ptr->_rgb_b = _rgb_b;
      ptr->_c1    = _c1;
      ptr->_coff  = _coff;        
    ptr = _armoire.get_sideSegment(aramoire_side_right, _armoireEtage-1);
      ptr->_isTw  = _isTw;
      ptr->_tw    = _tw;
      ptr->_tw_v  = _tw_v;
      ptr->_hsv_h = _hsv_h;
      ptr->_hsv_s = _hsv_s;
      ptr->_hsv_v = _hsv_v;
      ptr->_rgb_r = _rgb_r;
      ptr->_rgb_g = _rgb_g;
      ptr->_rgb_b = _rgb_b;        
      ptr->_c1    = _c1;        
      ptr->_coff  = _coff;        
  }
}

void Output::set_color_tw_v(const JsonObject & value){
  uint8_t scale = value[F("v")].as<uint8_t>();
  _c1 = CHSV(
    45U,                                                                              
    map(scale, 0U, 255U, 0U, 170U),                           
    _tw
  ); 
  _coff = _c1;
  _tw_v = scale;
  _isTw = true;
  set_colorArmoire();
  set_color();
  leds_managePtrGet()->showLeds(_pos, _bri); 
}
void Output::set_color_tw(uint8_t value){
  _c1 = CHSV(
    45U,                                                                              
    map(_tw_v, 0U, 255U, 0U, 170U),                           
    value
  ); 
  _coff = _c1;
  _tw = value;
  _isTw = true;
  set_colorArmoire();
  set_color();
  leds_managePtrGet()->showLeds(_pos, _bri); 
}
void Output::set_color_tw(const JsonObject & value){
  uint8_t scale = value[F("v")].as<uint8_t>();
  _c1 = CHSV(
    45U,                                                                              
    map(_tw_v, 0U, 255U, 0U, 170U),                           
    scale
  ); 
  _coff = _c1;
  _tw = scale;
  _isTw = true;
  set_colorArmoire();
  set_color();
  leds_managePtrGet()->showLeds(_pos, _bri); 
}

void Output::set_color_rgb(const JsonObject & value, boolean setBri) {
  uint8_t r = value[F("r")].as<uint8_t>();
  uint8_t g = value[F("g")].as<uint8_t>();
  uint8_t b = value[F("b")].as<uint8_t>();

  _rgb_r = r;
  _rgb_g = g;
  _rgb_b = b;   

  if ( value.containsKey(F("h")) && value.containsKey(F("s")) && value.containsKey(F("v")) ) {  

    uint8_t h = value[F("h")].as<uint8_t>();
    uint8_t s = value[F("s")].as<uint8_t>();
    uint8_t v = value[F("v")].as<uint8_t>();

    _hsv_h = h;   
    _hsv_s = s;   
    _hsv_v = v;  

  } else {
    ledsTools tools;
    HsvColor hsv = tools.rgb2hsv (_rgb_r, _rgb_g, _rgb_b);   

    _hsv_h = hsv.h;   
    _hsv_s = hsv.s;   

    if (setBri) _hsv_v = hsv.v;

    CRGB c = CHSV(_hsv_h, _hsv_s, _hsv_v);

    _rgb_r = c.r;
    _rgb_g = c.g;
    _rgb_b = c.b;  

  }

  _c1 = CRGB(_rgb_r, _rgb_g, _rgb_b);
  _coff = _c1;
  _isTw = false; 

  set_colorArmoire();
  set_color();
  leds_managePtrGet()->showLeds(_pos, _bri); 
}

void Output::set_color_h(const JsonObject & value){
  uint8_t v = value[F("v")].as<uint8_t>();
  _hsv_h = v; 
  _c1 = CHSV(_hsv_h, _hsv_s, _hsv_v);
  _rgb_r = _c1.r;
  _rgb_g = _c1.g;
  _rgb_b = _c1.b;    
  _coff = _c1;
  _isTw = false;
  set_colorArmoire();
  set_color();
  leds_managePtrGet()->showLeds(_pos, _bri); 
}
void Output::set_color_hIncr(const JsonObject & value){
  uint8_t amount = value[F("v")].as<int8_t>();

  // int fb = (_hsv_h+amount);

  if (_eff_activ) clear();
  _eff_activ = false;

  uint8_t newVal = (uint8_t)(_hsv_h+amount);

  // if ((amount>0) && (fb < 255) )  newVal = (uint8_t)(_hsv_h+amount);
  // if ((amount<0) && (fb > 0) )    newVal = (uint8_t)(_hsv_h+amount);  

  _hsv_h = newVal; 
  _c1 = CHSV(_hsv_h, _hsv_s, _hsv_v);
  _rgb_r = _c1.r;
  _rgb_g = _c1.g;
  _rgb_b = _c1.b;    
  _coff = _c1;
  _isTw = false;
  set_colorArmoire();
  set_color();
  leds_managePtrGet()->showLeds(_pos, _bri); 
}
void Output::set_color_s(const JsonObject & value){
  uint8_t v = value[F("v")].as<uint8_t>();  
  _hsv_s = v; 
  _c1 = CHSV(_hsv_h, _hsv_s, _hsv_v);
  _rgb_r = _c1.r;
  _rgb_g = _c1.g;
  _rgb_b = _c1.b;  
  _coff = _c1;
  _isTw = false;
  set_colorArmoire();
  set_color();
  leds_managePtrGet()->showLeds(_pos, _bri); 
}
void Output::set_color_v(const JsonObject & value){
  uint8_t v = value[F("v")].as<uint8_t>();
  _hsv_v = v; 
  _c1 = CHSV(_hsv_h, _hsv_s, _hsv_v);
  _rgb_r = _c1.r;
  _rgb_g = _c1.g;
  _rgb_b = _c1.b;     
  _coff = _c1;
  _isTw = false;
  set_colorArmoire();
  set_color();
  leds_managePtrGet()->showLeds(_pos, _bri); 
}

void Output::set_briIncr(int8_t amount) {
  if (_eff_activ) {
    LAMPPTRGET()->effect_setbri_incr(_pos, amount);
  } else {
    set_colorBriIncr(_pos, amount); 
  }
}
void Output::set_briGen(int8_t amount) {
  if (_eff_activ) {
    LAMPPTRGET()->effect_setbri(_pos, amount);
  } else {
    set_colorBriIncr(_pos, amount, false); 
  }
}
void Output::set_briGlobal(uint8_t v) {
  _briEff = v;
  // _hsv_v  = v;
  // _tw     = v;
  // CRGB hsv = CHSV(_hsv_h, _hsv_s, _hsv_v);
  // _rgb_r = hsv.r;
  // _rgb_g = hsv.g;
  // _rgb_b = hsv.b;  

  // boolean stripW;
  // leds_managePtrGet()->get_ledsIsTw(_pos, stripW);  
  // if (_isTw && stripW) {

  // }else{
  //   if (_isTw) {
  //     _c1 = CHSV(
  //       45U,                                                                              
  //       map(_tw_v, 0U, 255U, 0U, 170U),                           
  //       _tw
  //     ); 
  //     _coff = _c1;
  //   }else{
  //     _c1 = CRGB(_rgb_r, _rgb_g, _rgb_b);     
  //     _coff = _c1;
  //   }
  // }
};

void Output::set_colorBriIncr(uint8_t sPos, int8_t amount, boolean add) {

  boolean stripW;
  leds_managePtrGet()->get_ledsIsTw(sPos, stripW);
  if (_isTw && stripW) {
    if (_onoff) {
      int fb = (_tw+amount);
      if (add && (amount>0) && (fb > 255) ) return;
      if (add && (amount<0) && (fb < 0) ) return;        
      _tw = ((add)? _tw : 0) + amount;
      _hsv_v = _tw;
      CRGB hsv = CHSV(_hsv_h, _hsv_s, _hsv_v);
      _rgb_r = hsv.r;
      _rgb_g = hsv.g;
      _rgb_b = hsv.b;        
      if (add && _tw == 255) {
        for(unsigned i = 0; i < 3; ++i) {
          // ledsSet::fill(sPos, CRGB(0,0,0));
          set_color(CRGB(0,0,0), -1);
          leds_managePtrGet()->showLeds(sPos, _bri);  
          delay(250);      
          // ledsSet::fill(sPos, CRGBW(0, 0, 0, _tw));
          set_color(CRGB(0,0,0), _tw);
          leds_managePtrGet()->showLeds(sPos, _bri);    
          delay(250);            
        }  
        // if (dir) index = 0;      
      } 
      set_briArmoire(amount, add);
      if (!_isArmoire) ledsSet::fill(sPos, CRGBW(0, 0, 0, _tw));
      else set_color();
      leds_managePtrGet()->showLeds(sPos, _bri);           
    }
  } else {
    if (_isTw) {
      int fb = (_tw+amount);
      if (add && (amount>0) && (fb > 255) ) return;
      if (add && (amount<0) && (fb < 0) ) return;      
      _tw = ((add)? _tw : 0) + amount;
      _hsv_v = _tw;
      CRGB hsv = CHSV(_hsv_h, _hsv_s, _hsv_v);
      _rgb_r = hsv.r;
      _rgb_g = hsv.g;
      _rgb_b = hsv.b;         
      _c1 = CHSV(
        45U,                                                                              
        map(_tw_v, 0U, 255U, 0U, 170U),                           
        _tw
      ); 
      _coff = _c1;
      if (add && _tw == 255) {
        for(unsigned i = 0; i < 3; ++i) {
          // ledsSet::fill(sPos, CRGB(0,0,0));
          set_color(CRGB(0,0,0), -1);
          leds_managePtrGet()->showLeds(sPos, _bri);  
          delay(250);      
          // ledsSet::fill(sPos, _c1);
          set_color(_c1, -1);
          leds_managePtrGet()->showLeds(sPos, _bri);    
          delay(250);         
        } 
        // if (dir) index = 0;          
      }
      set_briArmoire(amount, add);
      if (!_isArmoire) ledsSet::fill(sPos, _c1);
      else set_color();
      leds_managePtrGet()->showLeds(sPos, _bri);      
    } else {
      int fb = (_hsv_v+amount);
      if (add && (amount>0) && (fb > 255) ) return;
      if (add && (amount<0) && (fb < 0) ) return;
      _hsv_v = ((add)? _hsv_v : 0) + amount;
      _tw = _hsv_v;
      // ledsTools tools;
      // RgbColor hsv = tools.hsv2rgb (_hsv_h, _hsv_s, _hsv_v, 0);
      CRGB hsv = CHSV(_hsv_h, _hsv_s, _hsv_v);
      _rgb_r = hsv.r;
      _rgb_g = hsv.g;
      _rgb_b = hsv.b;   
      _c1 = CRGB(_rgb_r, _rgb_g, _rgb_b);     
      _coff = _c1;
      if (add && _hsv_v == 255) {
        for(unsigned i = 0; i < 3; ++i) {
          // ledsSet::fill(sPos, CRGB(0,0,0));
          set_color(CRGB(0,0,0), -1);
          leds_managePtrGet()->showLeds(sPos, _bri);  
          delay(250);      
          // ledsSet::fill(sPos, _c1);
          set_color(_c1, -1);
          leds_managePtrGet()->showLeds(sPos, _bri);    
          delay(250);           
        } 
        // if (dir) index = 0;        
      }
      set_briArmoire(amount, add);
      if (!_isArmoire) ledsSet::fill(sPos, _c1);
      else set_color();
      leds_managePtrGet()->showLeds(sPos, _bri);            
    }
  }
}

void Output::set_lastColor() { 
  if (!_isArmoire) ledsSet::fill(_pos, _c1);
  else set_color();
  leds_managePtrGet()->showLeds(_pos, _bri);   
}

void Output::set_briLoop() {
  if (!_select)   return;
  if (!_onoff)    return;
  if (_eff_activ) return;

  boolean stripW;
  leds_managePtrGet()->get_ledsIsTw(_pos, stripW);
  if (_isTw && stripW) {
    if (_onoff) {
      _tw++;
      if (_tw == 255) {
        for(unsigned i = 0; i < 3; ++i) {
          ledsSet::fill(_pos, CRGB(0,0,0));
          leds_managePtrGet()->showLeds(_pos, _bri);  
          delay(250);      
          ledsSet::fill(_pos, CRGBW(0, 0, 0, _tw));
          leds_managePtrGet()->showLeds(_pos, _bri);    
          delay(250);            
        }        
      } 
      ledsSet::fill(_pos, CRGBW(0, 0, 0, _tw));
      leds_managePtrGet()->showLeds(_pos, _bri);           
    }
  } else {
    if (_isTw) {
      _tw_v++;
      _c1 = CHSV(
        45U,                                                                              
        map(_tw_v, 0U, 255U, 0U, 170U),                           
        _tw
      ); 
      _coff = _c1;
      if (_tw_v == 255) {
        for(unsigned i = 0; i < 3; ++i) {
          ledsSet::fill(_pos, CRGB(0,0,0));
          leds_managePtrGet()->showLeds(_pos, _bri);  
          delay(250);      
          ledsSet::fill(_pos, _c1);
          leds_managePtrGet()->showLeds(_pos, _bri);    
          delay(250);            
        }        
      }
      ledsSet::fill(_pos, _c1);
      leds_managePtrGet()->showLeds(_pos, _bri);      
    } else {
      _hsv_v++;
      // ledsTools tools;
      // RgbColor hsv = tools.hsv2rgb (_hsv_h, _hsv_s, _hsv_v, 0);
      // _rgb_r = hsv.R;
      // _rgb_g = hsv.G;
      // _rgb_b = hsv.B;   
      // _c1 = CRGB(_rgb_r, _rgb_g, _rgb_b);  
      _c1 = CHSV(_hsv_h, _hsv_s, _hsv_v);
      _rgb_r = _c1.r;
      _rgb_g = _c1.g;
      _rgb_b = _c1.b;           
      _coff = _c1;
      if (_hsv_v == 255) {
        for(unsigned i = 0; i < 3; ++i) {
          ledsSet::fill(_pos, CRGB(0,0,0));
          leds_managePtrGet()->showLeds(_pos, _bri);  
          delay(250);      
          ledsSet::fill(_pos, _c1);
          leds_managePtrGet()->showLeds(_pos, _bri);    
          delay(250);            
        }        
      }
      ledsSet::fill(_pos, _c1);
      leds_managePtrGet()->showLeds(_pos, _bri);            
    }
  }
}




void Output::set_color_eff_rgb1(const JsonObject & value) {
  uint8_t r = value[F("r")].as<uint8_t>();
  uint8_t g = value[F("g")].as<uint8_t>();
  uint8_t b = value[F("b")].as<uint8_t>();
  _eff_c1 = CRGB(r, g, b);
}

/**
 * @brief     rotation des couleur par valeur HSV
 * @see       https://github.com/AdriLighting/
 * 
 * @author    Adrilighting
 * @date      mar. 28 déc. 2021 16:25:30 
 * 
 * @return     renvoi true si _eff_colorMod == 0
 */
boolean Output::eff_colormod_1c(){
  if (_eff_colorMod==1)return true;
  return false;
}
boolean Output::eff_colormod_2c(){
  if (_eff_colorMod==2)return true;
  return false;
}
/**
 * @brief     rotation des couleur par valeur HSV
 * @see       https://github.com/AdriLighting/
 * 
 * @author    Adrilighting
 * @date      mar. 28 déc. 2021 16:25:30 
 * 
 * @return     renvoi true si _eff_colorMod == 0
 */
boolean Output::eff_isHueTimer(){
  if (_eff_colorMod==0)return true;
  return false;
}

void Output::eff_get_c1(RgbColor & c1){
  RgbColor c;
  c.R = _eff_c1.r;
  c.G = _eff_c1.g;
  c.B = _eff_c1.b;
  c1= c;
}
void Output::eff_get_c2(RgbColor & c1){
  RgbColor c;
  c.R = _eff_c2.r;
  c.G = _eff_c2.g;
  c.B = _eff_c2.b;
  c1= c;
}
uint8_t Output::eff_get_hue(){
  if (_eff_colorMod == 0) {
    return LAMPPTRGET()->_hsvLoop_1.hue();
  }
  if (_eff_colorMod == 1) {
    ledsTools tools;
    HsvColor hsv = tools.rgb2hsv (_eff_c1.r, _eff_c1.g, _eff_c1.b);
    return hsv.h;
  }
  return 30;
}
uint8_t Output::get_eff_colorMod(){
  return _eff_colorMod;
}

/*
OUTPUT_EFFECTTYPE Output::get_defaultPal(){
  return _output_defaultPal;
}
void Output::set_defaultPal(OUTPUT_EFFECTTYPE v1){
  _output_defaultPal = v1;
}

OUTPUT_EFFECTTYPE Output::get_effectType(){
  return _output_effecttype;
}
void Output::set_effectType(OUTPUT_EFFECTTYPE v1){
  _output_effecttype = v1;
}
bool Output::isPalEffect(OUTPUT_EFFECTTYPE value) {
  if (value == OUTPUT_EFFECTTYPE::OET_PAL) return true;
  if (value == OUTPUT_EFFECTTYPE::OET_PALGRAD) return true;
  if (value == OUTPUT_EFFECTTYPE::OET_PALFIRE) return true;
  if (value == OUTPUT_EFFECTTYPE::OET_PALDUO) return true;
  return false;
}   
*/

CRGB Output::fade2black(CRGBW rgb) {
  CRGB result;
  int j = (255-60);
  if (j==0) j=1;
  result.r = (rgb.r>j) ? rgb.r-j  : 0 ;
  result.g = (rgb.g>j) ? rgb.g-j  : 0 ;
  result.b = (rgb.b>j) ? rgb.b-j  : 0 ;
  return result;
}
CRGB Output::fade2black(CRGB rgb) {
  CRGB result;
  int j = (255-60);
  if (j==0) j=1;
  result.r = (rgb.r>j) ? rgb.r-j  : 0 ;
  result.g = (rgb.g>j) ? rgb.g-j  : 0 ;
  result.b = (rgb.b>j) ? rgb.b-j  : 0 ;
  return result;
}

void Output::stripFadeToBlack(boolean isSegment){
  CRGBW     * _ledarray_w = nullptr;  
  CRGB      * _ledarray   = nullptr;  
  boolean   _isTw;
  boolean   _isSegment;
  boolean   _isSegmentEnabled = isSegment;
  uint8_t   _maxSeg       = 1 ;
  uint16_t  _numLeds      = 0;

  uint16_t
    _pixstart,          
    _pixend;            
    // _pixrange;           

  leds_managePtrGet()->get_ledsIsTw(_pos, _isTw);
  leds_managePtrGet()->get_numLeds(_pos, _numLeds);

  _isSegment = DevicePtrGet()->get_outputArray(_pos)->_leds_segment.isSegment();

  if (DevicePtrGet()->get_outputArray(_pos)->_leds_segment.isSegment()  ) {
    DevicePtrGet()->get_outputArray(_pos)->_leds_segment.get_max(_maxSeg);
  }

  if (_isTw) _ledarray_w  = leds_managePtrGet()->get_crgwbArray(_pos);
  else       _ledarray    = leds_managePtrGet()->get_crgbArray(_pos);

  for (int A = 0; A < _maxSeg; ++A) {
    if ( _isSegment && _isSegmentEnabled ) { 
      _pixstart = DevicePtrGet()->get_outputArray(_pos)->_leds_segment.get_segment(A)->_start;
      _pixend   = DevicePtrGet()->get_outputArray(_pos)->_leds_segment.get_segment(A)->_end;  
    }else{
      _pixstart = 0;
      _pixend   = _numLeds-1;
    }
    // _pixrange = (_pixend - _pixstart+1);     

    for (int i=_pixstart; i <= _pixend; i++) {
      if (!_isTw)  {
        CRGB old = _ledarray[i]; 
        _ledarray[i] = fade2black(old);
      }
      else        {
        CRGBW old = _ledarray_w[i]; 
        _ledarray_w[i] = fade2black(old);
      }
    }
  }
}








/*
extern uint32_t _audio_avg ;
extern uint32_t _audio_avgMax ;
void Output::audio_classique(){
  CRGBW       * ledarray_w  = nullptr;  
  CRGB        * ledarray    = nullptr;  
  uint16_t      numLeds     = 0;  
  boolean isTw;
  leds_managePtrGet()->get_ledsIsTw(_pos, isTw);
  if (isTw) ledarray_w  = leds_managePtrGet()->get_crgwbArray(_pos);
  else      ledarray    = leds_managePtrGet()->get_crgbArray(_pos);
  leds_managePtrGet()->get_numLeds(_pos, numLeds);
  int maxVal = map(_audio_avg, 0, _audio_avgMax, 0, numLeds);
  for (int i = 0; i < numLeds; ++i)
  {
    if (isTw) (i>=maxVal) ? ledarray_w[i] = CRGB(0,0,0) : ledarray_w[i] = CRGB(0,0,255);
    else      (i>=maxVal) ? ledarray[i]   = CRGB(0,0,0) : ledarray[i]   = CRGB(0,0,255);
  }
  
  // leds_managePtrGet()->showLeds(_pos, _bri);  
}
*/


// void Output::set_effectEnabled (const char cmd){if (cmd == ) || (cmd == ) {fspritf("[cmd: %s]", cmd) } }
// void Output::set_slectMod    (const char cmd){if ((cmd == req_toglle) || (cmd == req_ss_toglle))     {fspritf("[cmd: %s]", cmd) } }

