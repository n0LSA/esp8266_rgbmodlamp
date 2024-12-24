#include "../../include/fastled/base.h"
#include "../../include/pixel/set.h"
// extern MatrixParam _MatrixParamBase;

// void fill_solid( struct CRGBW * leds, int numToFill, const struct CRGB& color)
// {
//     for( int i = 0; i < numToFill; i++) {
//         leds[i] = color;
//     }
// }
// void fill_solid( struct CRGBW * leds, int numToFill, const struct CHSV& color)
// {
//     for( int i = 0; i < numToFill; i++) {
//         leds[i] = color;
//     }
// }

// void fill_solid(struct CRGBW * leds, int numToFill, const struct CRGBW& color){
//  for(int i = 0; i < numToFill; i++){
//    leds[i] = color;
//  }
// }

// void fill_solid( struct CRGBW * leds, const struct CRGB& color, int start, int end)
// {
//     for( int i = start; i < end; i++) {
//         leds[i] = color;
//     }
// }

#ifndef DEVICE_OUTPUTS_MAX
  #ifdef ADS_OC
    #define DEVICE_OUTPUTS_MAX ADS_OC
  #else
    #define DEVICE_OUTPUTS_MAX 6
  #endif
#endif



CRGB * ledsBase::get_leds()   {return _leds;}
CRGBW * ledsBase::get_ledsW() {return _ledsW;}
void ledsBase::get_numLedsW( uint16_t & result) {result = _num_ledsW;}
void ledsBase::get_numLeds( uint16_t & result)  {result = _num_leds;}
void ledsBase::get_ledsIsTw(boolean & result)   {result = _hackTw;}
void ledsBase::get_ledsPin(uint8_t & result)    {result = _pin;}
void ledsBase::get_stripPos(uint8_t & result)   {result = _stripPos;}

void ledsBase::set_numLedsW( uint16_t  result)  {_num_ledsW   = result;}
void ledsBase::set_numLeds( uint16_t  result)   {_num_leds  = result;}
void ledsBase::set_ledsIsTw(boolean  result)    {_hackTw    = result;}
void ledsBase::set_ledsPin(uint8_t  result)     {_pin       = result;}
void ledsBase::set_stripPos(uint8_t  result)    {_stripPos  = result;}


leds_setting * leds_settingArray[DEVICE_OUTPUTS_MAX];
leds_setting::leds_setting(ledsBase * myLed){
  _myLed    = myLed;
  uint8_t pos;
  myLed->get_stripPos(pos);
  // _MatrixParam = new MatrixParam(pos); 
};
ledsBase * leds_setting::myLed() {return _myLed;}
MatrixParam * leds_setting::matrix() {return _MatrixParam;}



leds_manage * leds_managePtr = nullptr;
leds_manage * leds_managePtrGet(){return leds_managePtr;}
leds_manage::leds_manage(){
  leds_managePtr = this;
}
void leds_manage::strip_add(ledsBase * myLed){
  leds_settingArray[_cnt] = new leds_setting(myLed) ;
  _cnt++;
}
leds_setting * leds_manage::setting(uint8_t p){
  return leds_settingArray[p] ;
}
CRGB * leds_manage::get_crgbArray(uint8_t pos){
  return setting(pos)->myLed()->get_leds();
}
CRGBW * leds_manage::get_crgwbArray(uint8_t pos){
  return setting(pos)->myLed()->get_ledsW();
}
void leds_manage::get_numLeds(uint8_t pos, uint16_t & result){
  setting(pos)->myLed()->get_numLeds(result);  
}
void leds_manage::get_ledsIsTw(uint8_t pos, boolean & result){
  setting(pos)->myLed()->get_ledsIsTw(result);   
}
void leds_manage::get_ledsPin(uint8_t pos, uint8_t & result){
  setting(pos)->myLed()->get_ledsPin(result); 
}
MatrixParam * leds_manage::get_matrix(uint8_t pos){
  return setting(pos)->matrix();
}

void leds_manage::showLeds(uint8_t nbr, uint8_t val){
  val = map(val, 0, 255, 0, 128);
  FastLED[nbr].showLeds(  val );
}


uint32_t leds_manage::getPixelNumber(uint8_t pos, uint16_t x, uint16_t y) 
{

  uint16_t nbLeds;
  uint16_t width_v;
  // uint16_t width;
  // uint16_t height;
  uint8_t segements;
  bool type;
  get_matrix(pos)->get_width_v(width_v);
  // get_matrix(pos)->get_width(width);
  // get_matrix(pos)->get_height(height);
  get_matrix(pos)->get_segements(segements);
  get_matrix(pos)->get_type(type);
  get_numLeds(pos, nbLeds);

  #ifndef XY_EXTERN
    uint32_t this_y = 0;
    uint32_t this_x = 0;
    get_matrix(pos)->thisXY(1, 1, x, y, this_x, this_y);

    if ((this_y % 2 == 0) || type) {
      return ((uint32_t)this_y * segements * width_v + this_x) % nbLeds;
    }
    else {
      return ((uint32_t)this_y * segements * width_v + width_v - this_x - 1) % nbLeds;
    }
  #else
  // // any out of bounds address maps to the first hidden pixel
  // if ( (x >= _MatrixParam._widthw) || (y >= _MatrixParam._height) ) {
  //  return (LAST_VISIBLE_LED + 1);
  // }
  // uint16_t i = (y * _MatrixParam._widthw) + x;
  // uint16_t j = pgm_read_dword(&XYTable[i]);
  return 0;
#endif
}  

void leds_manage::print(){
  for(int i = 0; i < _cnt; i++) {
      uint16_t  nbLedsW   = leds_settingArray[i]->myLed()->_num_ledsW;
      uint16_t  nbLeds    = leds_settingArray[i]->myLed()->_num_leds;
      uint8_t   stripPos  = leds_settingArray[i]->myLed()->_stripPos;
      boolean   tw        = leds_settingArray[i]->myLed()->_hackTw;
      uint8_t   pin       = leds_settingArray[i]->myLed()->_pin;
      Serial.printf_P(PSTR("[%d] nbleds: %4d %4d - stripPos: %d - tw: %d - pin: %d\n"),
        i, nbLeds, nbLedsW, stripPos, tw, pin);
  }
}

