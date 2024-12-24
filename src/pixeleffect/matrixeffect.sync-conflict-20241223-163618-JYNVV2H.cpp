#include "../../include/pixeleffect/matrixeffect.h"
#define ALS_LAMPMATRIX
#ifdef ALS_LAMPMATRIX

uint16_t getmaxDim() {return ((WIDTH>HEIGHT)?WIDTH:HEIGHT);}
uint16_t getminDim() {return ((WIDTH<HEIGHT)?WIDTH:HEIGHT);}  
CRGB *setLed(uint32_t idx, CHSV val) { leds_managePtrGet()->get_crgbArray(0)[idx] = val; return &leds_managePtrGet()->get_crgbArray(0)[idx]; }
CRGB *setLed(uint32_t idx, CRGB val) { leds_managePtrGet()->get_crgbArray(0)[idx] = val; return &leds_managePtrGet()->get_crgbArray(0)[idx]; }

void adjustColorBrightness(CRGB& color, float factor) {
  color.red = (float)color.red * factor;
  color.green = (float)color.green * factor;
  color.blue = (float)color.blue * factor;
}

void EffectLighters::load(uint8_t strip_pos){
  randomSeed(millis());
  for (uint8_t i = 0U; i < LIGHTERS_AM; i++) {
    lightersIdx=0;
    lightersPos[0U][i] = random(0, WIDTH);
    lightersPos[1U][i] = random(0, HEIGHT);
    lightersSpeed[0U][i] = (float)random(-200, 200) / 10.0f;
    lightersSpeed[1U][i] = (float)random(-200, 200) / 10.0f;
    lightersColor[i] = random(0U, 255U);
    light[i] = random(1U, 3U)*127U;
  }
}
bool EffectLighters::run(){
  return lightersRoutine(*&_ledarray);
}
String EffectLighters::setDynCtrl(UIControl*_val) {

  if(_val->getId()==3) cnt = EffectCalc::setDynCtrl(_val).toInt();
  else if(_val->getId()==4) subPix = EffectCalc::setDynCtrl(_val).toInt();
  else EffectCalc::setDynCtrl(_val).toInt(); 
  return String();
}
bool EffectLighters::lightersRoutine(CRGB *leds) {
  float speedfactor = (float)speed / 4096.0f + 0.001f;

  memset8( leds, 0, NUM_LEDS * 3);

  EVERY_N_MILLIS(333) {
    lightersIdx = (lightersIdx+1)%constrain(cnt,1,LIGHTERS_AM);
    lightersSpeed[0U][lightersIdx] += random(-10, 10);
    lightersSpeed[1U][lightersIdx] += random(-10, 10);
    lightersSpeed[0U][lightersIdx] = fmod(lightersSpeed[0U][lightersIdx], 21);
    lightersSpeed[1U][lightersIdx] = fmod(lightersSpeed[1U][lightersIdx], 21);
    light[lightersIdx] = random(255U-(cnt*8),255U);
    if(!random(cnt+3))
      light[lightersIdx] = 127;
  }

  for (uint8_t i = 0U; i < constrain(cnt,1,LIGHTERS_AM); i++) {
    lightersPos[0U][i] += lightersSpeed[0U][i]*speedfactor;
    lightersPos[1U][i] += lightersSpeed[1U][i]*speedfactor;

    if (lightersPos[0U][i] < 0) lightersPos[0U][i] = (float)(WIDTH - 1);
    if (lightersPos[0U][i] >= (float)WIDTH) lightersPos[0U][i] = 0.0f;

    if (lightersPos[1U][i] <= 0.0f) {
      lightersPos[1U][i] = 0.0f;
      lightersSpeed[1U][i] = -lightersSpeed[1U][i];
      lightersSpeed[0U][i] = -lightersSpeed[0U][i];
    }
    if (lightersPos[1U][i] >= (int32_t)(HEIGHT - 1)) {
      lightersPos[1U][i] = (HEIGHT - 1U);
      lightersSpeed[1U][i] = -lightersSpeed[1U][i];
      lightersSpeed[0U][i] = -lightersSpeed[0U][i];
    }
    if (subPix)
      EffectMath::drawPixelXYF(lightersPos[0U][i], lightersPos[1U][i], CHSV(lightersColor[i], 255U-(i*2), light[i]), 0);
    else
      EffectMath::drawPixelXY((uint8_t)lightersPos[0U][i], (uint8_t)lightersPos[1U][i], CHSV(lightersColor[i], 255U-(i*2), light[i]));
  }

  return true;
}



/*
  _effeNamIDList "constleds.cpp"
    MEffectSparcles
    ALMLPT_EFF_MEffectSparcles 
    E_FX_SPARCLES  
    HUETIMER
  workerset "eff_w.h"
    effect
      24

  eff
    1 - speed
    2 - scale
    5 - Couleur (0: aléatoire, 1: couleurs, 2: blanc)
    4 - Cyle
    8 - Couleur 1 (0: cyclique)
*/
void MEffectSparcles::load(uint8_t strip_pos) {}
bool MEffectSparcles::run() {
  if (dryrun(1.5)) return false;
  return sparklesRoutine(*&_ledarray);
}
String MEffectSparcles::setDynCtrl(UIControl*_val) {
  set_dynCtrl(_val);
  return String();
}
bool MEffectSparcles::sparklesRoutine(CRGB *leds) {

  #ifdef MIC_EFFECTS
    uint8_t mic = myLamp->getMicMapMaxPeak();
    uint8_t mic_f = map(myLamp->getMicMapFreq(), LOW_FREQ_MAP_VAL, HI_FREQ_MAP_VAL, 0, 255);
    if (isMicOn() && eff > 5)
      fadeToBlackBy(leds, NUM_LEDS, 255 - mic);
    EffectMath::fader(isMicOn() ? map(scale, 1, 255, 100, 1) : map(scale, 1, 255, 50, 1));
  #else
    EffectMath::fader(map(scale, 1, 255, 1, 50));
  #endif

  CHSV currentHSV;

  if (_varia==1) update();

  for (uint8_t i = 0; i < (uint8_t)round(2.5 * (speed / 255.0) + 1); i++) {
    uint8_t x = random8(0U, WIDTH);
    uint8_t y = random8(0U, HEIGHT);
    #ifdef MIC_EFFECTS
      switch (eff) {
        case 1 :
        case 6 :
          mic_f = myLamp->getMicMapFreq();
          break;
        case 2 :
        case 7 :
          mic_f = map(myLamp->getMicMapFreq(), LOW_FREQ_MAP_VAL, HI_FREQ_MAP_VAL, 0, 255);
          break;
        case 3 :
        case 8 :
          mic_f = sin8(myLamp->getMicMapFreq());
          break;
        case 4 :
        case 9 :
          mic_f = cos8(myLamp->getMicMapFreq());
          break;
        case 5 :
        case 10 :
          mic_f = random8(255U);
          break;
      }
    #endif
    if (EffectMath::getPixColorXY(x, y) == 0U) {
      #ifdef MIC_EFFECTS
        if (isMicOn()) {
          currentHSV = CHSV(mic_f, 255U - myLamp->getMicMapMaxPeak()/3, constrain(mic * 1.25f, 48, 255));
        } else {
          currentHSV = CHSV(random8(1U, 255U), random8(192U, 255U), random8(192U, 255U));
        }
      #else
        if (_varia==0) currentHSV = CHSV(random8(1U, 255U), random8(192U, 255U), random8(192U, 255U));
        if (_varia==1) currentHSV = CHSV(_hue_1V + random(0U, 33U), random8(192U, 255U), random8(192U, 255U));
        if (_varia==2) currentHSV = CHSV(_hue_1V, random(0U, 33U), random8(192U, 255U));
      #endif
      // EffectMath::setLed(getPixelNumber(x, y), currentHSV);
      EffectMath::drawPixelXY(x, y, currentHSV);
    }
  }
  return true;
}


/*
  _effeNamIDList "constleds.cpp"
    MEffectWhiteColorStrip
    ALMLPT_EFF_MEffectWhiteColorStripe 
    E_FX_Scal  
    NONE
  workerset "eff_w.h"
    effect
      25

  eff
    2 - scale
*/
void MEffectWhiteColorStripe::load(uint8_t strip_pos)       {}
String MEffectWhiteColorStripe::setDynCtrl(UIControl*_val)  {set_dynCtrl(_val); return String();}
bool MEffectWhiteColorStripe::run()                         {return whiteColorStripeRoutine(*&_ledarray);}
bool MEffectWhiteColorStripe::whiteColorStripeRoutine(CRGB *leds) {
  #ifdef MIC_EFFECTS
    byte _scale = isMicOn() ? myLamp->getMicMapMaxPeak() : scale;
  #else
    byte _scale = scale;
  #endif
  if(_scale <= 127){
    uint8_t centerY       = max((uint8_t)round(HEIGHT / 2.0F) - 1, 0);
    uint8_t bottomOffset  = (uint8_t)(!(HEIGHT & 1) && (HEIGHT > 1));
    for (int16_t y = centerY; y >= 0; y--) {
      int br = BRIGHTNESS-(13*(WIDTH-y)); if((br-(255-LAMPPTRGET()->get_globalBrightness(0, brightness)))<0) br=0;
      CRGB color = CHSV(
        45U,
        map(speed, 0U, 255U, 0U, 170U),
        y == centerY 
          ? BRIGHTNESS                                                                                                
          : ((_scale) / 143.0F) > ((centerY + 1.0F) - (y + 1.0F)) / (centerY + 1.0F) ? BRIGHTNESS : br); 
      for (int16_t x = 0U; x < (int16_t)WIDTH; x++) {
        EffectMath::drawPixelXY(x, y, color);
        EffectMath::drawPixelXY(x, max((uint8_t)(HEIGHT - 1U) - (y + 1U) + bottomOffset, 0U), color);
      }
    }
  } else {
    uint8_t centerX = max((uint8_t)round(WIDTH / 2.0F) - 1, 0);
    uint8_t leftOffset = (uint8_t)(!(WIDTH & 1) && (WIDTH > 1));
    for (int16_t y = 0U; y < (int16_t)HEIGHT; y++) {
      for (int16_t x = centerX; x >= 0; x--) {
        int br = BRIGHTNESS-(13*(WIDTH-x)); if((br-(255-LAMPPTRGET()->get_globalBrightness(0, brightness)))<0) br=0;
        CRGB color = CHSV(
          45U, 
          map(speed, 0U, 255U, 0U, 170U),
          x == centerX
            ? BRIGHTNESS
            : ((255-_scale) / 143.0F) > ((centerX + 1.0F) - (x + 1.0F)) / (centerX + 1.0F) ? BRIGHTNESS : br);
        EffectMath::drawPixelXY(x, y, color);
        EffectMath::drawPixelXY(max((uint8_t)(WIDTH - 1U) - (x + 1U) + leftOffset, 0U), y, color);
      }
    }
  }
  return true;
}


/*
  _effeNamIDList "constleds.cpp"
    MEffectEverythingFall
    ALMLPT_EFF_MEffectEverythingFall 
    E_4PAL  
    PAL
  workerset "eff_w.h"
    effect
      26

  eff
    1   - speed
    2   - scale
    11  - Palette (0: Cyle)
    12  - Cyle de rotation des Palettes
*/
void MEffectEverythingFall::load(uint8_t strip_pos){
  
  ledsSet::fill(0, CRGB(0,0,0)); 
}
String MEffectEverythingFall::setDynCtrl(UIControl*_val) {
  set_dynCtrl(_val);
  return String();
}
// SPARKING: What chance (out of 255) is there that a new spark will be lit?
// Higher chance = more roaring fire.  Lower chance = more flickery fire.
// Default 120, suggested range 50-200.
#define SPARKINGNEW 80U // 50 // 30 // 120 // 90 // 60
bool MEffectEverythingFall::run(){

  update();

  if (dryrun(1.5))
    return false;

  uint8_t coolingnew = map (scale, 1, 255, 93, 10);

  for (uint8_t x = 0; x < WIDTH; x++) {
    // Step 1.  Cool down every cell a little
    for (uint8_t i = 0; i < HEIGHT; i++) {
      heat[x][i] = qsub8(heat[x][i], random(0, coolingnew));
    }

    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for (unsigned int k = HEIGHT - 1; k >= 2; k--) {
      heat[x][k] = (heat[x][k - 1] + heat[x][k - 2] + heat[x][k - 2]) / 3;
    }

    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if (random8() < SPARKINGNEW) {
      int y = random(2);
      heat[x][y] = qadd8(heat[x][y], random(160, 255));
    }

    // Step 4.  Map from heat cells to LED colors
    for (uint8_t j = 0; j < HEIGHT; j++) {
      // Scale the heat value from 0-255 down to 0-240
      // for best results with color palettes.
      byte colorindex = scale8(heat[x][(uint8_t)j], 240);
      nblend(leds_managePtrGet()->get_crgbArray(0)[getPixelNumber(x, (HEIGHT - 1) - j)], ColorFromPalette(_cTargetPalette, colorindex,  LAMPPTRGET()->get_globalBrightness(0, brightness) ), 50);

    }
  }
  return true;
}

/*
  _effeNamIDList "constleds.cpp"
    MEffectPulse
    ALMLPT_EFF_MEffectPulse 
    E_FX_ScalSpeed  
    NONE
  workerset "eff_w.h"
    effect
      27

  eff
    1   - speed
    2   - scale
*/
void MEffectPulse::load(uint8_t strip_pos){
  
  ledsSet::fill(0, CRGB(0,0,0)); 
}
bool MEffectPulse::run(){
  if (dryrun(1.0))
    return false;
  return pulseRoutine(*&_ledarray);
}
String MEffectPulse::setDynCtrl(UIControl*_val) {
  set_dynCtrl(_val); 
  return String();
}
bool MEffectPulse::pulseRoutine(CRGB *leds) {

  CRGB _pulse_color;
  uint8_t _pulse_delta = 0;

  uint8_t _scale = scale;
  #ifdef MIC_EFFECTS
    #define FADE 255U - (isMicOn() ? myLamp->getMicMapMaxPeak()*2 : 248U) // (isMicOn() ? 300U - myLamp->getMicMapMaxPeak() : 5U)
    #define BLUR (isMicOn() ? myLamp->getMicMapMaxPeak()/3 : 10U) //(isMicOn() ? map(myLamp->getMicMapMaxPeak(), 1, 255, 1, 30) : 10U)
  #else
    #define FADE 7U
    #define BLUR 10U
  #endif

  //EffectMath::dimAll(248U); // если эффект устанавливается с другими эффектами от Stefan Petrick, тогда  процедура должна называться dimAll (без двоечки)
  fadeToBlackBy(leds, NUM_LEDS, FADE);
  if (pulse_step <= currentRadius) {
    for (uint8_t i = 0; i < pulse_step; i++ ) {
      uint8_t _dark = qmul8( 2U, cos8 (128U / (pulse_step + 1U) * (i + 1U))) ;
      if (_scale == 1) {            // 1 - случайные диски
        _pulse_hue = pulse_hue;
        _pulse_color = CHSV(_pulse_hue, 255U, _dark);

      } else if (_scale <= 17) {    // 2...17 - перелив цвета дисков
        _pulse_delta = (17U - _scale) ;
        _pulse_color = CHSV(_pulse_hueall, 255U, _dark);

      } else if (_scale <= 33) {    // 18...33 - выбор цвета дисков
        _pulse_hue = (_scale - 18U) * 16U ;
        _pulse_color = CHSV(_pulse_hue, 255U, _dark);

      } else if (_scale <= 50) {    // 34...50 - дискоцветы
        _pulse_hue += (_scale - 33U) * 5U ;
        _pulse_color = CHSV(_pulse_hue, 255U, _dark);

      } else if (_scale <= 67) {    // 51...67 - пузыри цветы
        uint8_t _sat =  qsub8( 255U, cos8 (128U / (pulse_step + 1U) * (i + 1U))) ;
         _pulse_hue += (68U - _scale) * 7U ;
        _pulse_color = CHSV(_pulse_hue, _sat, _dark);

      } else if (_scale < 83) {     // 68...83 - выбор цвета пузырей
        uint8_t _sat =  qsub8( 255U, cos8 (128U / (pulse_step + 1U) * (i + 1U))) ;
        _pulse_hue = (_scale - 68U) * 16U ;
        _pulse_color = CHSV(_pulse_hue, _sat, _dark);

      } else if (_scale < 100) {    // 84...99 - перелив цвета пузырей
        uint8_t _sat =  qsub8( 255U, cos8 (128U / (pulse_step + 1U) * (i + 1U))) ;
        _pulse_delta = (_scale - 85U)  ;
        _pulse_color = CHSV(_pulse_hueall, _sat, _dark);

      } else { // 100 - случайные пузыри
        uint8_t _sat =  qsub8( 255U, cos8 (128U / (pulse_step + 1U) * (i + 1U))) ;
        _pulse_hue = pulse_hue;
        _pulse_color = CHSV(_pulse_hue, _sat, _dark);
      }
      EffectMath::drawCircle(centerX, centerY, i, _pulse_color);
    }
  } else {
    centerX = random8(WIDTH - 5U) + 3U;
    centerY = random8(HEIGHT - 5U) + 3U;
    _pulse_hueall += _pulse_delta;
    pulse_hue = random8(0U, 255U);
    currentRadius = random8(3U, 9U);
    pulse_step = 0;
  }
  pulse_step++;
  EffectMath::blur2d(BLUR);
  return true;
}


/*
  _effeNamIDList "constleds.cpp"
    MEffectRainbow
    ALMLPT_EFF_MEffectRainbow 
    E_FX_ScalSpeed  
    NONE
  workerset "eff_w.h"
    effect
      28

  eff
    1 - speed
    2 - scale
*/
void MEffectRainbow::load(uint8_t strip_pos)      {}
String MEffectRainbow::setDynCtrl(UIControl*_val) {set_dynCtrl(_val); return String();}
bool MEffectRainbow::run(){
  // коэф. влияния замаплен на скорость, 4 ползунок нафиг не нужен
  hue += (6.0 * (speed / 255.0) + 0.05 ); // скорость смещения цвета зависит от кривизны наклна линии, коэф. 6.0 и 0.05
#ifdef MIC_EFFECTS
    micCoef = (getMicMapMaxPeak() > map(speed, 1, 255, 100, 10) and isMicOn() ? getMicMapMaxPeak() : 100.0)/100.0;
    twirlFactor = EffectMath::fmap((float)scale, 85, 170, 8.3, 24);      // на сколько оборотов будет закручена матрица, [0..3]
    twirlFactor *= getMicMapMaxPeak() > map(speed, 1, 255, 80, 10) and isMicOn() ? 1.5f * ((float)getMicMapFreq() / 255.0f) : 1.0f;
#else
    twirlFactor = EffectMath::fmap((float)scale, 85, 170, 8.3, 24);      // на сколько оборотов будет закручена матрица, [0..3]
    micCoef = 1.0;
#endif
  if(scale<85){
    return rainbowHorVertRoutine(false);
  } else if (scale>170){
    return rainbowHorVertRoutine(true);
  } else {
    return rainbowDiagonalRoutine();
  }
}

bool MEffectRainbow::rainbowHorVertRoutine(bool isVertical)
{
  for (uint8_t i = 0U; i < (isVertical?WIDTH:HEIGHT); i++)
  {
    for (uint8_t j = 0U; j < (isVertical?HEIGHT:WIDTH); j++)
    {
      CHSV thisColor = CHSV(((hue + i * scale) * micCoef), 255, 255);
      EffectMath::drawPixelXY((isVertical?i:j), (isVertical?j:i), thisColor);
    }
  }
  return true;
}

bool MEffectRainbow::rainbowDiagonalRoutine()
{
  for (uint8_t i = 0U; i < WIDTH; i++)
  {
    for (uint8_t j = 0U; j < HEIGHT; j++)
    {
      CRGB thisColor = CHSV((uint8_t)(hue + ((float)WIDTH / (float)HEIGHT * i + j * twirlFactor) * ((float)255 / (float)getmaxDim())), 255, LAMPPTRGET()->get_globalBrightness(0, brightness));
      EffectMath::drawPixelXY(i, j, thisColor);
    }
  }
  return true;
}


/*
  _effeNamIDList "constleds.cpp"
    MEffectRainbow
    ALMLPT_EFF_MEffectMatrix 
    E_FX_matrix  
    HUETIMER
  workerset "eff_w.h"
    effect
      29

  eff
    1 - speed
    2 - scale
*/
String MEffectMatrix::setDynCtrl(UIControl*_val) {
  set_dynCtrl(_val);

  if(_val->getId()==7)   gluk = EffectCalc::setDynCtrl(_val).toInt();


  if (_varia == 2) {
    lightersColorHue  = 3;
    white             = true;

    randColor         = false;
    colorCycle        = false;
  } else if (_varia == 1) {
    lightersColorHue  = 2;
    colorCycle        = true; 

    randColor         = false; 
    white             = false; 
  } else if (_varia == 0) {
    lightersColorHue  = 0;
    randColor         = true;

    white             = false;
    colorCycle        = false; 
  }
  return String();
}

bool MEffectMatrix::run() {
  return matrixRoutine(*&_ledarray);
}
void MEffectMatrix::load(uint8_t strip_pos){
  

  randomSeed(analogRead(millis()));
  for (uint8_t i = 0U; i < LIGHTERS_AM; i++) {
    lightersPos[0U][i] = random(0, WIDTH);
    lightersPos[1U][i] = EffectMath::randomf(HEIGHT - HEIGHT /2, HEIGHT);
    lightersSpeed[0U][i] = 1;
    lightersSpeed[1U][i] = (float)random(10, 20) / 10.0f;
    lightersColor[i] = _cMod;
    light[i] = random(196,255);
  }
}

bool MEffectMatrix::matrixRoutine(CRGB *leds) {
  update();

  if (dryrun(2.5)) return false;

  float speedfactor = EffectMath::fmap((float)speed, 1., 255., 0.06, 0.4);
  EffectMath::dimAll(map(speed, 1, 255, 252, 240));
  
  CHSV color;

  for (uint8_t i = 0U; i < map(scale, 1, 32, 1, LIGHTERS_AM); i++)
  {
    lightersPos[1U][i] -= lightersSpeed[1U][i]*speedfactor;

    if (white) {
      color = rgb2hsv_approximate(CRGB::Gray);
      color.val = light[i];
    } else if (randColor) {
      EVERY_N_MILLIS((600 / speedfactor)*2) {
        _cMod = random(1, 250);
      }
      color = CHSV(_cMod, 255, light[i]);
    } else if (colorCycle) {
      _cMod = _hue_1V;
      color = CHSV(_hue_1V, 255, light[i]);   
    } else {
      _cMod = _hue_1V;
      color = CHSV(_hue_1V, 255, light[i]);
    }


    EffectMath::drawPixelXYF_Y(lightersPos[0U][i], lightersPos[1U][i], color);

    count += speedfactor;

    if (gluk > 1 and (uint8_t)count%2 == 0) 
      if (random8() < gluk) {
        lightersPos[0U][i] = lightersPos[0U][i] + random(-1, 2);
        light[i] = random(196,255);
      }

    if(lightersPos[1U][i] < -1) {
      lightersPos[0U][i] = random(0, WIDTH);
      lightersPos[1U][i] = EffectMath::randomf(HEIGHT - HEIGHT /2, HEIGHT);
      lightersSpeed[1U][i] = EffectMath::randomf(1.5, 2.5); 
      light[i] = random(127U, 255U);
      lightersColor[i] = _cMod;
    }
  }

  return true;
}


/*
  _effeNamIDList "constleds.cpp"
    MEffectStarFall
    ALMLPT_EFF_MEffectStarFall 
    E_FX_STARFAIL  
    HUETIMER
  workerset "eff_w.h"
    effect
      29

  eff
    1 - speed
    2 - scale
*/
bool MEffectStarFall::run(){
  return snowStormStarfallRoutine(*&_ledarray);
}
void MEffectStarFall::load(uint8_t strip_pos){
  

  randomSeed(millis());
  for (uint8_t i = 0U; i < LIGHTERS_AM; i++)
  {
    lightersPos[0U][i] = random(-(int)WIDTH, WIDTH);
    lightersPos[1U][i] = random(HEIGHT - 1, HEIGHT + 4);
    lightersSpeed[0U][i] = EffectMath::randomf(-1, 1);  // X
    lightersSpeed[1U][i] = EffectMath::randomf(1, 2);   // Y
    lightersColor[i] = random(0U, 255U);
    light[i] = 255;
  }
}
String MEffectStarFall::setDynCtrl(UIControl*_val) {
  if (_val->getId()==4) { _hueTimer = EffectCalc::setDynCtrl(_val).toInt(); LAMPPTRGET()->effectsLoop_TimerHue(_hueTimer); }
  else if (_val->getId()==8) {
    _hue_1 = EffectCalc::setDynCtrl(_val).toInt(); 
    _hue_1V = _hue_1;
  }
  else if(_val->getId()==3) effId   = EffectCalc::setDynCtrl(_val).toInt();
  else if(_val->getId()==5) isNew   = EffectCalc::setDynCtrl(_val).toInt();
  else if(_val->getId()==6) _cMod   = EffectCalc::setDynCtrl(_val).toInt();
  // else if(_val->getId()==7) _cValue = EffectCalc::setDynCtrl(_val).toInt();
  else EffectCalc::setDynCtrl(_val).toInt();
  #ifdef ALT_DEBUG_TARCE
    _val->print();  
  #endif
  return String();
}
bool MEffectStarFall::snowStormStarfallRoutine(CRGB *leds) {
  if (dryrun(2.0))
    return false;

  float speedfactor = EffectMath::fmap(speed, 1, 255, 0.25, .5);
  EffectMath::dimAll(255 - (effId == 2 ? 70 : 60) * speedfactor);
  CHSV color;

  if (_cMod==1) update(); // "Couleur (0: aléatoire, 1: couleurs, 2: blanc)"

  for (uint8_t i = 0U; i < map(scale, 1, 10, LIGHTERS_AM/8, LIGHTERS_AM); i++) // LIGHTERS_AM
  {
    //color = CHSV((effId > 1 ? lightersColor[i] : 255), (effId > 1 ? light[i] : 0), (effId > 1 ? 255 : light[i]));
    switch (effId)
    {
    case 1:
      color = CHSV(127, 0, light[i]);
      break;
    case 2:
      if (light[i] > 10) { 
        byte tmp = light[i] - 10 * speedfactor; 
        if (_cMod==0) {color = CHSV(lightersColor[i], 255 - light[i], tmp);                 light[i]=tmp; }
        else if (_cMod==1) {color = CHSV(_hue_1V, 255 - light[i], tmp);                     light[i]=tmp; } 
        else if (_cMod==2) {color = CHSV(_hue_1V, map(255 - light[i], 0, 255, 0, 33), tmp); light[i]=tmp; }
        else
          color = CHSV(lightersColor[i], 255 - light[i], tmp); light[i]=tmp; 
      }
      else color = rgb2hsv_approximate( CRGB::Black);
      break;
    case 3:
      if (_cMod==0) color = CHSV(lightersColor[i], 255, light[i]);
      else if (_cMod==1) color = CHSV(_hue_1V, 255, light[i]);
      else if (_cMod==2) color = CHSV(_hue_1V, map(60*speedfactor, 0, 255, 0, 33), light[i]);
      else    
        color = CHSV(lightersColor[i], 255, light[i]);
      break;
    default:
      break;
    }

    if (isNew) {
      lightersPos[0U][i] -= lightersSpeed[0U][effId == 1 ? 0 : i] * speedfactor;
      lightersPos[1U][i] -= 1 * speedfactor;
    } else {
      lightersPos[0U][i] += lightersSpeed[0U][i] * (speedfactor / 2);
      lightersPos[1U][i] -= lightersSpeed[1U][0] * (speedfactor / 2);
    }

   // if (isNew){
      CHSV newcolor = CHSV(color.h, color.s, map(color.v, 0, 255, color.v, LAMPPTRGET()->get_globalBrightness(0, brightness)));
      EffectMath::drawPixelXYF(lightersPos[0U][i], lightersPos[1U][i], newcolor, 0);
   /* }
    else {
      EffectMath::drawPixelXY((uint8_t)lightersPos[0U][i], (uint8_t)lightersPos[1U][i], color);
    }*/

    if(lightersPos[1U][i] < -1) {
      if (isNew) {
        lightersPos[0U][i] = random(-(int)WIDTH, WIDTH);
        lightersPos[1U][i] = effId > 1 ? random(HEIGHT / 2, HEIGHT + 4) : random(HEIGHT - 1, HEIGHT + 4);
        lightersSpeed[0U][i] = EffectMath::randomf(-1, 1);  // X
        lightersSpeed[1U][i] = EffectMath::randomf(1, 2);   // Y
      } else {
        lightersPos[0U][i] = (float)random(-(WIDTH * 10 - 2), (WIDTH * 10 - 2)) / 10.0f;
        lightersPos[1U][i] = random(HEIGHT, HEIGHT + 4);
        lightersSpeed[0U][i] = (float)random(15, 25) / 10.0f;   // X
        lightersSpeed[1U][i] = lightersSpeed[0U][i]; //(float)random(15, 25) / 10.0f;   // Y
      }
      lightersColor[i] = random(0U, 255U);
      light[i] = random(127U, 255U);
    }
  }
  return true;
}



String MEffectLighterTracers::setDynCtrl(UIControl*_val) {
/*if(_val->getId()==3)      cnt     = EffectCalc::setDynCtrl(_val).toInt();
  else if(_val->getId()==4) _cMod   = EffectCalc::setDynCtrl(_val).toInt();
  else if(_val->getId()==5) _cValue = EffectCalc::setDynCtrl(_val).toInt();
  else EffectCalc::setDynCtrl(_val).toInt(); // для всех других не перечисленных контролов просто дергаем функцию базового класса (если это контролы палитр, микрофона и т.д.)
*/
  if (_val->getId()==4) { _hueTimer = EffectCalc::setDynCtrl(_val).toInt(); LAMPPTRGET()->effectsLoop_TimerHue(_hueTimer); }
  else if (_val->getId()==8) {
    _hue_1 = EffectCalc::setDynCtrl(_val).toInt(); 
    _hue_1V = _hue_1;
  }
  else if(_val->getId()==6) {
    _cMod = EffectCalc::setDynCtrl(_val).toInt();
    if (!_cMod) {
      LAMPPTRGET()->_hsvLoop_1.set_hueRnd(2);
    } else LAMPPTRGET()->_hsvLoop_1.set_hueRnd(1);
  }
  else EffectCalc::setDynCtrl(_val).toInt();
  #ifdef ALT_DEBUG_TARCE
    _val->print();  
  #endif
  return String();
}
void MEffectLighterTracers::load(uint8_t strip_pos){
  

  for (uint8_t j = 0U; j < _AMOUNT; j++)
  {
    int8_t sign;
    // забиваем случайными данными
    coord[j][0U] = (float)WIDTH / 2.0f;
    random(0, 2) ? sign = 1 : sign = -1;
    vector[j][0U] = ((float)random(40, 150) / 10.0f) * sign;
    coord[j][1U] = (float)HEIGHT / 2;
    random(0, 2) ? sign = 1 : sign = -1;
    vector[j][1U] = ((float)random(40, 150) / 10.0f) * sign;
    light[j] = 127;
    //ballColors[j] = random(0, 9) * 28;
  }
}
bool MEffectLighterTracers::run(){
  if (dryrun(2.0))
    return false;  
  return lighterTracersRoutine(*&_ledarray);
}
bool MEffectLighterTracers::lighterTracersRoutine(CRGB *leds) {
  //float speedfactor = speed/2048.0+0.01;
  float speedfactor = EffectMath::fmap(speed, 1, 255, 0.01, .1);

  fadeToBlackBy(leds, NUM_LEDS, map(speed, 1, 255, 6, 55));

  if (_cMod<2) update(); // Couleur (0: aléatoire, 1: couleurs, 2: blanc)

  uint8_t sat = 200u;
  uint8_t maxBalls = scale;
  for (uint8_t j = 0U; j < maxBalls; j++) {
    if      (_cMod==0) ballColors[j] = _hue_1V;
    else if (_cMod==1 && !_hue_1) ballColors[j] = _hue_1V + _AMOUNT;
    else if (_cMod==1 && _hue_1) ballColors[j] = _hue_1V;
    else if (_cMod==2) sat = speedfactor;
    else
      ballColors[j] = (maxBalls-j) * _AMOUNT + j;

    for (uint8_t i = 0U; i < 2U; i++) {
      coord[j][i] += vector[j][i] * speedfactor;
      if (coord[j][i] < 0) {
        coord[j][i] = 0.0f;
        vector[j][i] = -vector[j][i];
      }
    }

    if ((uint16_t)coord[j][0U] > (WIDTH - 1)) {
      coord[j][0U] = (WIDTH - 1);
      vector[j][0U] = -vector[j][0U];
    }
    if ((uint16_t)coord[j][1U] > (HEIGHT - 1)) {
      coord[j][1U] = (HEIGHT - 1);
      vector[j][1U] = -vector[j][1U];
    }
    EVERY_N_MILLIS(random16(256, 1024)) {
      if (light[j] == 127)
        light[j] = 255;
      else light[j] = 127;
    }
    EffectMath::drawPixelXYF(coord[j][0U], coord[j][1U], CHSV(ballColors[j], sat, 255U));
  }
  blur2d(leds, WIDTH, HEIGHT, 5);
  return true;
}


/*
  _effeNamIDList "constleds.cpp"
    MEffectLightBalls 
    ALMLPT_EFF_MEffectLightBalls Paintball
    E_FX_ScalSpeed  
    NONE
  workerset "eff_w.h"
    effect
      32

  eff
    1 - speed
    2 - scale
*/
bool MEffectLightBalls::run(){
  return lightBallsRoutine(*&_ledarray);
}
String MEffectLightBalls::setDynCtrl(UIControl*_val) {set_dynCtrl(_val); return String();}
void MEffectLightBalls::load(uint8_t strip_pos){
  
}
#define BORDERTHICKNESS (1U)
bool MEffectLightBalls::lightBallsRoutine(CRGB *leds) {
  const uint8_t paintWidth = WIDTH - BORDERTHICKNESS * 2;
  const uint8_t paintHeight = HEIGHT - BORDERTHICKNESS * 2;

  // Apply some blurring to whatever's already on the matrix
  // Note that we never actually clear the matrix, we just constantly
  // blur it repeatedly.  Since the blurring is 'lossy', there's
  // an automatic trend toward black -- by design.
  uint8_t blurAmount = dim8_raw(beatsin8(3,64,100));
  blur2d(leds, WIDTH, HEIGHT, blurAmount);

  float speedScale = (((float)speed)/255.0)+0.1;

  // Use two out-of-sync sine waves
  uint16_t  i = beatsin16( 79*speedScale, 0, 255); //91
  uint16_t  j = beatsin16( 67*speedScale, 0, 255); //109
  uint16_t  k = beatsin16( 53*speedScale, 0, 255); //73
  uint16_t  m = beatsin16( 97*speedScale, 0, 255); //123

  // The color of each point shifts over time, each at a different speed.
  uint16_t ms = millis() / (scale/16 + 1);
  leds[getPixelNumber( highByte(i * paintWidth) + BORDERTHICKNESS, highByte(j * paintHeight) + BORDERTHICKNESS)] += CHSV( ms / 29, 200U, 255U);
  leds[getPixelNumber( highByte(j * paintWidth) + BORDERTHICKNESS, highByte(k * paintHeight) + BORDERTHICKNESS)] += CHSV( ms / 41, 200U, 255U);
  leds[getPixelNumber( highByte(k * paintWidth) + BORDERTHICKNESS, highByte(m * paintHeight) + BORDERTHICKNESS)] += CHSV( ms / 37, 200U, 255U);
  leds[getPixelNumber( highByte(m * paintWidth) + BORDERTHICKNESS, highByte(i * paintHeight) + BORDERTHICKNESS)] += CHSV( ms / 53, 200U, 255U);

  return true;
}



void MEffectBall::load(uint8_t strip_pos){
  

  if (scale <= 85)
    ballSize = map(scale, 1, 85, 1U, max((uint8_t)min(WIDTH,HEIGHT) / 3, 1));
  else if (scale > 85 and scale <= 170)
    ballSize = map(scale, 170, 86, 1U, max((uint8_t)min(WIDTH,HEIGHT) / 3, 1));
  else
    ballSize = map(scale, 171, 255, 1U, max((uint8_t)min(WIDTH,HEIGHT) / 3, 1));

  for (uint8_t i = 0U; i < 2U; i++) {
    coordB[i] = i? float(WIDTH - ballSize) / 2 : float(HEIGHT - ballSize) / 2;
    vectorB[i] = EffectMath::randomf(0, 24.) - 12.0;
    ballColor = CHSV(random(1, 250), random(200, 255), 255);
  }
}

bool MEffectBall::run(){
  if (dryrun(4.0))
    return false;
  return ballRoutine(*&_ledarray);
}
String MEffectBall::setDynCtrl(UIControl*_val) {set_dynCtrl(_val); return String();}

bool MEffectBall::ballRoutine(CRGB *leds) {
  if (scale <= 85)
    ballSize = map(scale, 1, 85, 1U, max((uint8_t)min(WIDTH,HEIGHT) / 3, 1));
  else if (scale > 85 and scale <= 170)
    ballSize = map(scale, 170, 86, 1U, max((uint8_t)min(WIDTH,HEIGHT) / 3, 1));
  else
    ballSize = map(scale, 171, 255, 1U, max((uint8_t)min(WIDTH,HEIGHT) / 3, 1));

  EVERY_N_SECONDS(5){
    //LOG(println,ballSize);
    for (uint8_t i = 0U; i < 2U; i++)
    {
      if(fabs(vectorB[i]) < 12)
        vectorB[i] += EffectMath::randomf(0, 8.) - 4.0;
      else if (vectorB[i] > 12)
        vectorB[i] -= EffectMath::randomf(1, 6);
      else
        vectorB[i] += EffectMath::randomf(1, 6);
      if(!(uint8_t)vectorB[i])
        vectorB[i] += vectorB[i] > 0 ? 0.25 : -0.25;

    }
  }

  for (uint8_t i = 0U; i < 2U; i++) {
    coordB[i] += vectorB[i] * EffectMath::fmap((float)speed, 1., 255., 0.02, 0.15);
    if ((int8_t)coordB[i] < 0) {
      coordB[i] = 0;
      vectorB[i] = -vectorB[i];
      if (RANDOM_COLOR) ballColor = CHSV(random(1, 250), random(200, 255), 255);
    }
  }
  if ((int8_t)coordB[0U] > (int8_t)(WIDTH - ballSize)) {
    coordB[0U] = (WIDTH - ballSize);
    vectorB[0U] = -vectorB[0U];
    if (RANDOM_COLOR) ballColor = CHSV(random(1, 250), random(200, 255), 255);
  }
  if ((int8_t)coordB[1U] > (int8_t)(HEIGHT - ballSize)) {
    coordB[1U] = (HEIGHT - ballSize);
    vectorB[1U] = -vectorB[1U];
    if (RANDOM_COLOR) ballColor = CHSV(random(1, 250), random(200, 255), 255);
  }

  if (scale <= 85)
    memset8( leds, 0, NUM_LEDS * 3);
  else if (scale > 85 and scale <= 170)
    fadeToBlackBy(leds, NUM_LEDS, 255 - map(speed, 1, 255, 245, 200));
  else
    fadeToBlackBy(leds, NUM_LEDS, 255 - map(speed, 1, 255, 253, 248));

  for (uint8_t i = 0; i < ballSize; i++) {
    for (uint8_t j = 0; j < ballSize; j++) {
      EffectMath::drawPixelXYF(coordB[0U] + (float)i, coordB[1U] + (float)j, ballColor, 0);
    }
  }
  //fpsmeter();
  return true;
}



void MEffect3DNoise::load(uint8_t strip_pos){
    #if (WIDTH > HEIGHT)
    // uint8_t noise[2*HEIGHT][WIDTH];
    noise = new uint8_t*[2*HEIGHT];
    for(int i = 0; i < 2*HEIGHT; ++i) {
        noise[i] = new uint8_t[WIDTH];
    }

    #else
      // uint8_t noise[2*WIDTH][HEIGHT];
      noise = new uint8_t*[2*WIDTH];
      for(int i = 0; i < 2*WIDTH; ++i) {
          noise[i] = new uint8_t[HEIGHT];
      }  
    #endif


  fillnoise8();

}
String MEffect3DNoise::setDynCtrl(UIControl*_val) {
  set_dynCtrl(_val);
  if(_val->getId()==3 && _val->getVal().toInt()==0 && !isRandDemo()) _cTargetPalette = ZeebraColors_p;
  else if(_val->getId()==7) blurIm = EffectCalc::setDynCtrl(_val).toInt();
  else if(_val->getId()==5) colorLoop = EffectCalc::setDynCtrl(_val).toInt();
  else EffectCalc::setDynCtrl(_val).toInt(); // для всех других не перечисленных контролов просто дергаем функцию базового класса (если это контролы палитр, микрофона и т.д.)
  return String();
}
bool MEffect3DNoise::run(){
  if (dryrun(2.0)) return false;
  update();

  #ifdef MIC_EFFECTS
    uint8_t mmf = isMicOn() ? getMicMapFreq() : 0;
    uint8_t mmp = isMicOn() ? getMicMapMaxPeak() : 0;
    _scale = (NOISE_SCALE_AMP*(float)scale/255.0+NOISE_SCALE_ADD)*(mmf>0?(1.5*mmf/255.0):1);
    _speed = NOISE_SCALE_AMP*(float)speed/512.0*(mmf<LOW_FREQ_MAP_VAL && mmp>MIN_PEAK_LEVEL?10:2.5*mmp/255.0+1);
  #else
    _scale = NOISE_SCALE_AMP*scale/255.0+NOISE_SCALE_ADD;
    _speed = NOISE_SCALE_AMP*speed/512.0;
  #endif
  fillnoise8();  
  fillNoiseLED();
  return true;
}
void MEffect3DNoise::fillNoiseLED()
{
  uint8_t dataSmoothing = 0;
  if (speed < 50)
  {
    dataSmoothing = 200 - (speed * 4);
  }
  for (uint8_t i = 0; i < EffectMath::getminDim()*2; i++)
  {
    int32_t ioffset = _scale * i;
    for (uint8_t j = 0; j < EffectMath::getmaxDim(); j++)
    {
      int32_t joffset = _scale * j;

      uint8_t data = inoise8(x + ioffset, y + joffset, z);

      data = qsub8(data, 16);
      data = qadd8(data, scale8(data, 39));

      if (dataSmoothing)
      {
        uint8_t olddata = noise[i][j];
        uint8_t newdata = scale8( olddata, dataSmoothing) + scale8( data, 256 - dataSmoothing);
        data = newdata;
      }

      noise[i][j] = data;
    }
  }
  z += _speed;

  // apply slow drift to X and Y, just for visual variation.
  x += _speed * 0.125; // 1/8
  y -= _speed * 0.0625; // 1/16

  for (uint8_t i = 0; i < WIDTH; i++)
  {
    for (uint8_t j = 0; j < HEIGHT; j++)
    {
      uint8_t index = noise[j%(EffectMath::getminDim()*2)][i];
      uint8_t bri =   noise[i%(EffectMath::getminDim()*2)][j];
      // if this palette is a 'loop', add a slowly-changing base value
      if ( colorLoop)
      {
        index += ihue;
      }
      // brighten up, as the color palette itself often contains the
      // light/dark dynamic range desired
      if ( bri > 127 && blurIm)
      {
        bri = 255;
      }
      else
      {
        bri = dim8_raw( bri * 2);
      }
      CRGB color = ColorFromPalette(_cTargetPalette, index, map(bri,0,255,bri,LAMPPTRGET()->get_globalBrightness(0, brightness)));

      EffectMath::drawPixelXY(i, j, color);
    }
  }
  ihue += 1;
}

void MEffect3DNoise::fillnoise8()
{
  for (uint8_t i = 0; i < EffectMath::getminDim()*2; i++)
  {
    int32_t ioffset = _scale * i;
    for (uint8_t j = 0; j < EffectMath::getmaxDim(); j++)
    {
      int32_t joffset = _scale * j;
      noise[i][j] = inoise8(x + ioffset, y + joffset, z);
    }
  }
  z += _speed;
}


/*
void MEffect3DNoise::load(uint8_t strip_pos){
    noisex = random16();
    noisey = random16();
    noisez = random16();
}
String MEffect3DNoise::setDynCtrl(UIControl*_val) {
  set_dynCtrl(_val);
  if(_val->getId()==3 && _val->getVal().toInt()==0 && !isRandDemo()) _cTargetPalette = ZeebraColors_p;
  // else if(_val->getId()==7) blurIm = EffectCalc::setDynCtrl(_val).toInt();
  // else if(_val->getId()==5) colorLoop = EffectCalc::setDynCtrl(_val).toInt();
  else EffectCalc::setDynCtrl(_val).toInt(); // для всех других не перечисленных контролов просто дергаем функцию базового класса (если это контролы палитр, микрофона и т.д.)
  return String();
}
void MEffect3DNoise::fillnoise8(uint32_t noisespeedx, uint16_t noisescale) {


  // If we're runing at a low "speed", some 8-bit artifacts become visible
  // from frame-to-frame.  In order to reduce this, we can do some fast data-smoothing.
  // The amount of data smoothing we're doing depends on "speed".
  uint8_t dataSmoothing = 0;
  uint16_t lowestNoise =    map(noisespeedx, 0, 255, 0, 60) < (unsigned int)noisespeedy ? map(noisespeedx, 0, 255, 0, 60) : (unsigned int)noisespeedy;
  lowestNoise = lowestNoise < noisespeedz ? lowestNoise : noisespeedz;
  if( lowestNoise < 8) {
    dataSmoothing = 200 - (lowestNoise * 4);
  }

  for(int i = 0; i < MAX_DIMENSION; i++) {
    int ioffset = noisescale * i;
    for(int j = 0; j < MAX_DIMENSION; j++) {
      int joffset = noisescale * j;

      uint8_t data = inoise8(noisex + ioffset, noisey + joffset, noisez);

      // The range of the inoise8 function is roughly 16-238.
      // These two operations expand those values out to roughly 0..255
      // You can comment them out if you want the raw noise data.
      data = qsub8(data,16);
      data = qadd8(data,scale8(data,39));

      if( dataSmoothing ) {
        uint8_t olddata = noise[i][j];
        uint8_t newdata = scale8( olddata, dataSmoothing) + scale8( data, 256 - dataSmoothing);
        data = newdata;
      }

      noise[i][j] = data;
    }
  }

  noisex += map(noisespeedx, 0, 255, 0, 60);
  noisey += noisespeedy;
  noisez += noisespeedz;
}
void MEffect3DNoise::mapNoiseToLEDsUsingPalette(CRGBPalette16 palette, uint8_t hueReduce)
{
  static uint8_t ihue=0;


  for(int i = 0; i < WIDTH; i++) {
    for(int j = 0; j < HEIGHT; j++) {
      // We use the value at the (i,j) coordinate in the noise
      // array for our brightness, and the flipped value from (j,i)
      // for our pixel's index into the color palette.

      uint8_t index = noise[j][i];
      uint8_t bri =   noise[i][j];

      // if this palette is a 'loop', add a slowly-changing base value
      if( colorLoop) {
        index += ihue;
      }

      // brighten up, as the color palette itself often contains the
      // light/dark dynamic range desired
      if( bri > 127 ) {
        bri = 180;
      } else {
        bri = dim8_raw( bri * 2);
      }

      if(hueReduce > 0) {
        if(index < hueReduce) index = 0;
        else index -= hueReduce;
      }
      bri = map(bri, 0, 255, bri, LAMPPTRGET()->get_globalBrightness(0, brightness));
      CRGB color = ColorFromPalette( palette, index, bri);
      uint16_t n = EffectMath_PRIVATE::getPixelNumber(i, j);

      _ledarray[n] = color;
    }
  }

  ihue+=1;
}
bool MEffect3DNoise::run() {
  // generate noise data

  update();

  fillnoise8(speed, scale);

  // convert the noise data to colors in the LED array
  // using the current palette
  mapNoiseToLEDsUsingPalette(_cTargetPalette, _fade);

  return 10;
}
*/

bool MEffectBBalls::run(){
  return bBallsRoutine(_ledarray);
}

void MEffectBBalls::regen(){
  FastLED.clear();
  randomSeed(millis());
  if (_scale <= 16) {
    bballsNUM_BALLS =  map(_scale, 1, 16, 1, bballsMaxNUM_BALLS);
  } else {
    bballsNUM_BALLS =  map(_scale, 32, 17, 1, bballsMaxNUM_BALLS);
  }
  for (int i = 0 ; i < bballsNUM_BALLS ; i++) {          // Initialize variables
    bballsCOLOR[i] = random(0, 255);
    bballsBri[i] = 156;
    bballsX[i] = random(0, WIDTH-1);
    bballsBri[i] = halo ? 200 : (bballsX[i - 1] == bballsX[i] ? bballsBri[i-1] + 32 : 156);
    bballsTLast[i] = millis();
    bballsPos[i] = 0.0f;                                 // Balls start on the ground
    bballsVImpact[i] = bballsVImpact0 + EffectMath::randomf( - 2., 2.);                   // And "pop" up at vImpact0
    bballsCOR[i] = 0.90f - float(i) / pow(bballsNUM_BALLS, 2);
    bballsShift[i] = false;
  }
}


// !++ (разобраться отдельно)
String MEffectBBalls::setDynCtrl(UIControl*_val){
  if(_val->getId()==3) { _scale = EffectCalc::setDynCtrl(_val).toInt(); regen(); }
  else if(_val->getId()==4) halo = EffectCalc::setDynCtrl(_val).toInt();
  else EffectCalc::setDynCtrl(_val).toInt(); 
  return String();
}

void MEffectBBalls::load(uint8_t strip_pos){
  
  regen();
}

bool MEffectBBalls::bBallsRoutine(CRGB *leds)
{
  fadeToBlackBy(leds, NUM_LEDS, _scale <= 16 ? 255 : 50);

  for (int i = 0 ; i < bballsNUM_BALLS ; i++) {
    bballsTCycle =  millis() - bballsTLast[i] ;     // Calculate the time since the last time the ball was on the ground

    // A little kinematics equation calculates positon as a function of time, acceleration (gravity) and intial velocity
    bballsHi = 0.55 * bballsGRAVITY * pow( bballsTCycle / (float)(1550 - speed * 3) , 2.0 ) + bballsVImpact[i] * bballsTCycle / (float)(1550 - speed * 3);

    if ( bballsHi < 0 ) {
      bballsTLast[i] = millis();
      bballsHi = 0.0f;                            // If the ball crossed the threshold of the "ground," put it back on the ground
      bballsVImpact[i] = bballsCOR[i] * bballsVImpact[i] ;   // and recalculate its new upward velocity as it's old velocity * COR


      //if ( bballsVImpact[i] < 0.01 ) bballsVImpact[i] = bballsVImpact0;  // If the ball is barely moving, "pop" it back up at vImpact0
      if ( bballsVImpact[i] < 0.1 ) // сделал, чтобы мячики меняли свою прыгучесть и положение каждый цикл
      {
        bballsCOR[i] = 0.90 - (EffectMath::randomf(0., 9.)) / pow(EffectMath::randomf(4., 9.), 2.); // сделал, чтобы мячики меняли свою прыгучесть каждый цикл
        bballsShift[i] = bballsCOR[i] >= 0.85;                             // если мячик максимальной прыгучести, то разрешаем ему сдвинуться
        bballsVImpact[i] = bballsVImpact0;
      }
    }
    bballsPos[i] = bballsHi * (float)(HEIGHT - 1) / bballsH0;       // Map "h" to a "pos" integer index position on the LED strip
    if (bballsShift[i] > 0.0f && bballsPos[i] >= (float)HEIGHT - 1.5) {                  // если мячик получил право, то пускай сдвинется на максимальной высоте 1 раз
      bballsShift[i] = 0.0f;
      if (bballsCOLOR[i] % 2 == 0) {                                       // чётные налево, нечётные направо
        if (bballsX[i] <= 0) bballsX[i] = (WIDTH - 1U);
        else bballsX[i] -= 1;
      } else {
        if (bballsX[i] >= (int8_t)(WIDTH - 1U)) bballsX[i] = 0;
        else bballsX[i] += 1;
      }
    }

    //if (bballsPos[i] < HEIGHT - 1)
      if (halo){ // если ореол включен
        EffectMath::drawCircleF(bballsX[i], bballsPos[i] + 2.75, 3., CHSV(bballsCOLOR[i], 225, bballsBri[i]));
      } else {
        // попытка создать объем с помощью яркости. Идея в том, что шарик на переднем фоне должен быть ярче, чем другой,
        // который движится в том же Х. И каждый следующий ярче предыдущего.
        bballsBri[i] = (bballsX[i - 1] == bballsX[i] ? bballsBri[i-1] + 32 : 156);
        EffectMath::drawPixelXYF_Y(bballsX[i], bballsPos[i], CHSV(bballsCOLOR[i], 255, bballsBri[i]), 5);
      }
  }
  return true;
}



void MEffectSinusoid3::load(uint8_t strip_pos){
  
}
String MEffectSinusoid3::setDynCtrl(UIControl*_val) {
  set_dynCtrl(_val);
  return String();
}
bool MEffectSinusoid3::run(){
  return sinusoid3Routine(*&_ledarray);
}
bool MEffectSinusoid3::sinusoid3Routine(CRGB *leds) {
  const uint8_t semiHeightMajor =  HEIGHT / 2 + (HEIGHT % 2);
  const uint8_t semiWidthMajor =  WIDTH / 2  + (WIDTH % 2) ;
  float e_s3_speed = 0.004 * speed + 0.015; // speed of the movement along the Lissajous curves
  float e_s3_size = EffectMath::fmap(scale,1,255,3,9); // amplitude of the curves

  float time_shift = millis()&0xFFFFF; // на больших значениях будет страннео поведение, поэтому уменьшаем точность, хоть и будет иногда срыв картинки, но в 18 минут, так что - хрен с ним

  for (uint8_t y = 0; y < HEIGHT; y++) {
    for (uint8_t x = 0; x < WIDTH; x++) {
      CRGB color;
      uint8_t _scale = map8(255-scale,50,150);

      float cx = (y - semiHeightMajor) + float(e_s3_size * (sin16 (e_s3_speed * 98.301 * time_shift)))/32767.0;  // the 8 centers the middle on a 16x16
      float cy = (x - semiWidthMajor) + float(e_s3_size * (cos16 (e_s3_speed * 72.0874 * time_shift)))/32767.0;
      int8_t v = 127 * (1 + sin16 ( 127*_scale*EffectMath::sqrt ( (((float)cx*cx) + ((float)cy*cy)) ) )/32767.0);
      color.r = ~v;

      cx = (y - semiHeightMajor) + float(e_s3_size * (sin16 (e_s3_speed * 68.8107 * time_shift)))/32767.0;
      cy = (x - semiWidthMajor) + float(e_s3_size * (cos16 (e_s3_speed * 65.534 * time_shift)))/32767.0;
      v = 127 * (1 + sin16 ( 127*_scale*EffectMath::sqrt ( (((float)cx*cx) + ((float)cy*cy)) ) )/32767.0);
      color.g = ~v;

      cx = (y - semiHeightMajor) + float(e_s3_size * (sin16 (e_s3_speed * 134.3447 * time_shift)))/32767.0;
      cy = (x - semiWidthMajor) + float(e_s3_size * (cos16 (e_s3_speed * 170.3884 * time_shift)))/32767.0;
      v = 127 * (1 + sin16 ( 127*_scale*EffectMath::sqrt ( (((float)cx*cx) + ((float)cy*cy)) ) )/32767.0);
      color.b = ~v;

      // adjustColorBrightness(color, LAMPPTRGET()->get_globalBrightness(0, brightness));
      EffectMath::drawPixelXY(x, y, color);
    }
  }

  EffectMath::dimAll(LAMPPTRGET()->get_globalBrightness(0, brightness));
  
  return true;
}

void MEffectMetaBalls::load(uint8_t strip_pos){
  
}
String MEffectMetaBalls::setDynCtrl(UIControl*_val) {
  set_dynCtrl(_val);
  return String();
}
bool MEffectMetaBalls::run(){
  return metaBallsRoutine(*&_ledarray);
}
bool MEffectMetaBalls::metaBallsRoutine(CRGB *leds)
{
  float _speed = speed/127.0;

  // get some 2 random moving points
  uint8_t x2 = inoise8(millis() * _speed, 25355, 685 ) / WIDTH;
  uint8_t y2 = inoise8(millis() * _speed, 355, 11685 ) / HEIGHT;

  uint8_t x3 = inoise8(millis() * _speed, 55355, 6685 ) / WIDTH;
  uint8_t y3 = inoise8(millis() * _speed, 25355, 22685 ) / HEIGHT;

  // and one Lissajou function
  uint8_t x1 = beatsin8(23 * _speed, 0, 15);
  uint8_t y1 = beatsin8(28 * _speed, 0, 15);

  for (uint8_t y = 0; y < HEIGHT; y++) {
    for (uint8_t x = 0; x < WIDTH; x++) {

      // calculate distances of the 3 points from actual pixel
      // and add them together with weightening
      uint8_t  dx =  fabs(x - x1);
      uint8_t  dy =  fabs(y - y1);
      uint8_t dist = 2 * EffectMath::sqrt((dx * dx) + (dy * dy));

      dx =  fabs(x - x2);
      dy =  fabs(y - y2);
      dist += EffectMath::sqrt((dx * dx) + (dy * dy));

      dx =  fabs(x - x3);
      dy =  fabs(y - y3);
      dist += EffectMath::sqrt((dx * dx) + (dy * dy));

      // inverse result
      byte color = scale*4 / (dist==0?1:dist);

      // map color between thresholds
      if (color > 0 and color < 60) {
        EffectMath::drawPixelXY(x, y, CHSV(color * 9, 255, 255));
      } else {
        EffectMath::drawPixelXY(x, y, CHSV(0, 255, 255));
      }
      // show the 3 points, too
      EffectMath::drawPixelXY(x1, y1, CRGB(255, 255, 255));
      EffectMath::drawPixelXY(x2, y2, CRGB(255, 255, 255));
      EffectMath::drawPixelXY(x3, y3, CRGB(255, 255, 255));
    }
  }
  return true;
}

// ----------- Эффект "Осцилятор"
// (c) Сотнег (SottNick)
bool MEffectOscilator::run() {
  if (millis() - timer < (unsigned)map(speed, 1U, 255U, 70, 15)) return true;
  else timer = millis(); // не могу сообразить, как по другому скоростью управлять
  CRGB currColors[3];
  for (uint8_t c = 0; c < 3; c++)
    currColors[c] = ColorFromPalette(_cTargetPalette, c * 85U + hue);

  // расчёт химической реакции и отрисовка мира
  uint16_t colorCount[3] = {0U, 0U, 0U};
  hue++;
  FastLED.clear();
  for (uint8_t x = 0; x < WIDTH; x++) {
      for (uint8_t y = 0; y < HEIGHT; y++) {
          if (oscillatingWorld[x][y].red){
             colorCount[0]++;
             if (greenNeighbours(x, y) > 2)
                oscillatingWorld[x][y].color = 1U;
          }
          else if (oscillatingWorld[x][y].green){
             colorCount[1]++;
             if (blueNeighbours(x, y) > 2)
                oscillatingWorld[x][y].color = 2U;
          }
          else {//if (oscillatingWorld[x][y].blue){
             colorCount[2]++;
             if (redNeighbours(x, y) > 2)
                oscillatingWorld[x][y].color = 0U;
          }
          drawPixelXYFseamless((float)x + 0.5, (float)y + 0.5, currColors[oscillatingWorld[x][y].color]);
      }
  }


  // проверка зацикливания
  if (colorCount[0] == deltaHue && colorCount[1] == deltaHue2 && colorCount[2] == deltaValue) {
    step++;
    if (step > 10U){
      if (colorCount[0] < colorCount[1])
        step = 0;
      else
        step = 1;
      if (colorCount[2] < colorCount[step])
        step = 2;
      colorCount[step] = 0U;
      step = 0U;
    }
  }
  else
    step = 0U;
  
  // вброс хаоса
  if (hue == hue2){// чтобы не каждый ход
    hue2 += random8(220U) + 36U;
    uint8_t tx = random8(WIDTH);
    deltaHue = oscillatingWorld[tx][0U].color + 1U;
    if (deltaHue > 2U) deltaHue = 0U;
    oscillatingWorld[tx][0U].color = deltaHue;
    oscillatingWorld[(tx + 1U) % WIDTH][0U].color = deltaHue;
    oscillatingWorld[(tx + 2U) % WIDTH][0U].color = deltaHue;
  }

  deltaHue = colorCount[0];
  deltaHue2 = colorCount[1];
  deltaValue = colorCount[2];

  // вброс исчезнувшего цвета
  for (uint8_t c = 0; c < 3; c++)
  {
    if (colorCount[c] < 6U){
      uint8_t tx = random8(WIDTH);
      uint8_t ty = random8(HEIGHT);
      if (random8(2U)){
        oscillatingWorld[tx][ty].color = c;
        oscillatingWorld[(tx + 1U) % WIDTH][ty].color = c;
        oscillatingWorld[(tx + 2U) % WIDTH][ty].color = c;
      }
      else {
        oscillatingWorld[tx][ty].color = c;
        oscillatingWorld[tx][(ty + 1U) % HEIGHT].color = c;
        oscillatingWorld[tx][(ty + 2U) % HEIGHT].color = c;
      }
    }
  }

  // перенос на следующий цикл
  for (uint8_t x = 0; x < WIDTH; x++) {
      for (uint8_t y = 0; y < HEIGHT; y++) {
          setCellColors(x, y);
      }
  }

  //fpsmeter();
  return true;
}
String MEffectOscilator::setDynCtrl(UIControl*_val) {
  set_dynCtrl(_val);
  if(_val->getId()==7) {
    if (!EffectCalc::setDynCtrl(_val).toInt()) {
      _defaultPal= OET_PAL;
    }else {
      _defaultPal= OET_PALGRAD;
    }
  }    
  return String();
}
void MEffectOscilator::load(uint8_t s) {
  // palettesload();
  step = 0U;
 //случайное заполнение
  for (uint8_t i = 0; i < WIDTH; i++) {
    for (uint8_t j = 0; j < HEIGHT; j++) {
      oscillatingWorld[i][j].color = random8(3);
      setCellColors(i, j);
    }
  }
  timer = millis();
}

void MEffectOscilator::drawPixelXYFseamless(float x, float y, CRGB color)
{
  uint8_t xx = (x - (int)x) * 255, yy = (y - (int)y) * 255, ix = 255 - xx, iy = 255 - yy;
  // calculate the intensities for each affected pixel
  #define WU_WEIGHT(a,b) ((uint8_t) (((a)*(b)+(a)+(b))>>8))
  uint8_t wu[4] = {WU_WEIGHT(ix, iy), WU_WEIGHT(xx, iy),
                   WU_WEIGHT(ix, yy), WU_WEIGHT(xx, yy)};
  // multiply the intensities by the colour, and saturating-add them to the pixels
  for (uint8_t i = 0; i < 4; i++) {
    uint8_t xn = (int8_t)(x + (i & 1)) % WIDTH;
    uint8_t yn = (int8_t)(y + ((i >> 1) & 1)) % HEIGHT;
    CRGB clr = EffectMath::getPixColorXY(xn, yn);
    clr.r = qadd8(clr.r, (color.r * wu[i]) >> 8);
    clr.g = qadd8(clr.g, (color.g * wu[i]) >> 8);
    clr.b = qadd8(clr.b, (color.b * wu[i]) >> 8);
    EffectMath::drawPixelXY(xn, yn, clr);
  }
}

int MEffectOscilator::redNeighbours(uint8_t x, uint8_t y) {
  return (oscillatingWorld[(x + 1) % WIDTH][y].red) +
         (oscillatingWorld[x][(y + 1) % HEIGHT].red) +
         (oscillatingWorld[(x + EffectMath::getmaxWidthIndex()) % WIDTH][y].red) +
         (oscillatingWorld[x][(y + EffectMath::getmaxHeightIndex()) % HEIGHT].red) +
         (oscillatingWorld[(x + 1) % WIDTH][(y + 1) % HEIGHT].red) +
         (oscillatingWorld[(x + EffectMath::getmaxWidthIndex()) % WIDTH][(y + 1) % HEIGHT].red) +
         (oscillatingWorld[(x + EffectMath::getmaxWidthIndex()) % WIDTH][(y + EffectMath::getmaxHeightIndex()) % HEIGHT].red) +
         (oscillatingWorld[(x + 1) % WIDTH][(y + EffectMath::getmaxHeightIndex()) % HEIGHT].red);
    }

int MEffectOscilator::blueNeighbours(uint8_t x, uint8_t y) {
  return (oscillatingWorld[(x + 1) % WIDTH][y].blue) +
         (oscillatingWorld[x][(y + 1) % HEIGHT].blue) +
         (oscillatingWorld[(x + EffectMath::getmaxWidthIndex()) % WIDTH][y].blue) +
         (oscillatingWorld[x][(y + EffectMath::getmaxHeightIndex()) % HEIGHT].blue) +
         (oscillatingWorld[(x + 1) % WIDTH][(y + 1) % HEIGHT].blue) +
         (oscillatingWorld[(x + EffectMath::getmaxWidthIndex()) % WIDTH][(y + 1) % HEIGHT].blue) +
         (oscillatingWorld[(x + EffectMath::getmaxWidthIndex()) % WIDTH][(y + EffectMath::getmaxHeightIndex()) % HEIGHT].blue) +
         (oscillatingWorld[(x + 1) % WIDTH][(y + EffectMath::getmaxHeightIndex()) % HEIGHT].blue);
}
  
int MEffectOscilator::greenNeighbours(uint8_t x, uint8_t y) {
  return (oscillatingWorld[(x + 1) % WIDTH][y].green) +
         (oscillatingWorld[x][(y + 1) % HEIGHT].green) +
         (oscillatingWorld[(x + EffectMath::getmaxWidthIndex()) % WIDTH][y].green) +
         (oscillatingWorld[x][(y + EffectMath::getmaxHeightIndex()) % HEIGHT].green) +
         (oscillatingWorld[(x + 1) % WIDTH][(y + 1) % HEIGHT].green) +
         (oscillatingWorld[(x + EffectMath::getmaxWidthIndex()) % WIDTH][(y + 1) % HEIGHT].green) +
         (oscillatingWorld[(x + EffectMath::getmaxWidthIndex()) % WIDTH][(y + EffectMath::getmaxHeightIndex()) % HEIGHT].green) +
         (oscillatingWorld[(x + 1) % WIDTH][(y + EffectMath::getmaxHeightIndex()) % HEIGHT].green);
}

void MEffectOscilator::setCellColors(uint8_t x, uint8_t y) {
  oscillatingWorld[x][y].red = (oscillatingWorld[x][y].color == 0U);
  oscillatingWorld[x][y].green = (oscillatingWorld[x][y].color == 1U);
  oscillatingWorld[x][y].blue = (oscillatingWorld[x][y].color == 2U);
}

//------------ Эффект "Шторм" 
// (с) kostyamat 1.12.2020
// !++
String MEffectWrain::setDynCtrl(UIControl*_val)
{
  set_dynCtrl(_val);
  if(_val->getId()==1) {
    speed = EffectCalc::setDynCtrl(_val).toInt();
    speedFactor = EffectMath::fmap(speed, 1, 255, 0.125, .75) * EffectCalc::speedfactor;
  }
  else if(_val->getId()==2) _scale = EffectCalc::setDynCtrl(_val).toInt();
  else if(_val->getId()==3) { 
    uint8_t val = EffectCalc::setDynCtrl(_val).toInt(); 
    white = (val == 2); 
    randColor = (val == 1); 
    if (val == 0) white = randColor = 0;
  }
  else if(_val->getId()==5) clouds = EffectCalc::setDynCtrl(_val).toInt();
  else if(_val->getId()==6) storm = EffectCalc::setDynCtrl(_val).toInt();
  else if(_val->getId()==10) type = EffectCalc::setDynCtrl(_val).toInt();
  // else if(_val->getId()==7) {
  //   if (!EffectCalc::setDynCtrl(_val).toInt()) {
  //     _defaultPal= OET_PAL;
  //   }else {
  //     _defaultPal= OET_PALGRAD;
  //   }
  // }   
  // else EffectCalc::setDynCtrl(_val).toInt(); // для всех других не перечисленных контролов просто дергаем функцию базового класса (если это контролы палитр, микрофона и т.д.)
  return String();
}

void MEffectWrain::reload() {
  randomSeed(millis());
  for (byte i = 0; i < counts; i++) {
    dotPosX[i] = EffectMath::randomf(0, WIDTH); // Разбрасываем капли по ширине
    dotPosY[i] = EffectMath::randomf(0, HEIGHT);  // и по высоте
    dotColor[i] = random(0, 9) * 31;              // цвет капли
    dotAccel[i] = (float)random(5, 10) / 100;     // делаем частицам немного разное ускорение 
    dotBri[i] = random(170, 255);
  }
}

void MEffectWrain::load(uint8_t s) {
  speedFactor = EffectMath::fmap(speed, 1, 255, 0.125, .75) * EffectCalc::speedfactor;
  reload();
}

bool MEffectWrain::run() {
  update();
  if (_flash and (millis() - timer < 500)) 
    _flash = true;
  else _flash = false;

  if (_flash == false) {
    switch (type)
    {
    case 1:
    case 5:
      FastLED.clear();
      break;
    case 2:
    case 6:
      fadeToBlackBy(_ledarray, NUM_LEDS, 200. * speedFactor);
      break;
    case 3:
    case 7:
      fadeToBlackBy(_ledarray, NUM_LEDS, 100. * speedFactor);
      break;
    case 4:
    case 8:
      fadeToBlackBy(_ledarray, NUM_LEDS, 50. * speedFactor);
      break;
    default:
      break;
    } 
  }

  //
  for (byte i = 0; i < map(_scale, 1, 45, 2, counts); i++) {
    dotColor[i]++;
    dotPosX[i] += (speedFactor * dotChaos + dotAccel[i]) * dotDirect; // смещение по горизонтали
    dotPosY[i] -= (speedFactor + dotAccel[i]);

    // Обеспечиваем бесшовность по Y.
    if (dotPosY[i] < 0)
    {                                                             // достигли низа, обновляем каплю
      dotPosY[i] = ((float)HEIGHT - (clouds ? cloudHeight : 1.)); // переносим каплю в начало трека
      dotPosX[i] += EffectMath::randomf(-1, 1);                   // сдвигаем каплю туда-сюда по горизонтали
      dotBri[i] = random(170, 200);                               // задаем капле новое значение яркости
    }
    if (dotPosY[i] > (EffectMath::getmaxHeightIndex()))
      dotPosY[i] = 0;

    // Обеспечиваем бесшовность по X.
    if (dotPosX[i] < 0)
      dotPosX[i] = EffectMath::getmaxWidthIndex();
    if (dotPosX[i] > EffectMath::getmaxWidthIndex())
      dotPosX[i] = 0;

    if (randColor) {
      if (dotDirect) EffectMath::drawPixelXYF(dotPosX[i], dotPosY[i], CHSV(dotColor[i], 256U - beatsin88(2 * speed, 1, 196), beatsin88(1 * speed, 64, 255)));
      else EffectMath::drawPixelXYF_Y(dotPosX[i], dotPosY[i], CHSV(dotColor[i], 256U - beatsin88(2 * speed, 1, 196), beatsin88(1 * speed, 64, 255)));
    } else if (white) {
      CHSV color = rgb2hsv_approximate(CRGB::Gray);
      color.value = dotBri[i] - 48;
      if (dotDirect) EffectMath::drawPixelXYF(dotPosX[i], dotPosY[i], color);
      else EffectMath::drawPixelXYF_Y(dotPosX[i], dotPosY[i], color);
    }
    else {
      CHSV color = rgb2hsv_approximate(ColorFromPalette(_cTargetPalette, dotColor[i], dotBri[i]));
      color.sat = 128;
      if (dotDirect) EffectMath::drawPixelXYF(dotPosX[i], dotPosY[i], color);
      else EffectMath::drawPixelXYF_Y(dotPosX[i], dotPosY[i], color);
    }
  }

  // Раздуваем\угасаем ветер
  if (type <= 4) {
    uint8_t val = triwave8(windProgress += speedFactor);
    dotChaos = (float)val / 254;
    if (val == 0) {
      dotDirect = random(-1, 2); //выбираем направление ветра лево-право, рандом 2 не возвращает (как не странно).
    }
  } else dotDirect = 0;

    // Рисуем тучку и молнию
  if (clouds) {
    if (randColor) curPalette = palettes.at(0);  // устанавливаем палитру RainbowColors_p
    if (white) curPalette = palettes.at(FASTLED_PALETTS_COUNT-1);     // WaterfallColors_p
    if (storm) _flash = Lightning(200);
    Clouds((storm ? _flash : false));
  } else if (storm) {
    _flash = Lightning(200);
  }
  return true;
}

bool MEffectWrain::Lightning(uint16_t chanse)
{
  CRGB lightningColor = CHSV(30,90,255);
  //uint8_t lightning[WIDTH][HEIGHT];
  // ESP32 does not like static arrays  https://github.com/espressif/arduino-esp32/issues/2567
if (random16() < chanse)
  {            
    timer = millis();
    //uint8_t *lightning = (uint8_t *)malloc(WIDTH * HEIGHT);                                                           // Odds of a lightning bolt
    lightning[scale8(random8(), EffectMath::getmaxWidthIndex()) + EffectMath::getmaxHeightIndex() * WIDTH] = 255; // Random starting location
    for (uint8_t ly = EffectMath::getmaxHeightIndex(); ly > 1; ly--)
    {
      for (uint8_t lx = 1; lx < EffectMath::getmaxWidthIndex(); lx++)
      {
        if (lightning[lx + ly * WIDTH] == 255)
        {
          lightning[lx + ly * WIDTH] = 0;
          uint8_t dir = random8(4);
          switch (dir)
          {
          case 0:
            EffectMath::drawPixelXY(lx + 1, ly - 1, lightningColor);
            lightning[(lx + 1) + (ly - 1) * WIDTH] = 255; // move down and right
            break;
          case 1:
            EffectMath::drawPixelXY(lx, ly - 1, lightningColor); 
            lightning[lx + (ly - 1) * WIDTH] = 255;                                 // move down
            break;
          case 2:
            EffectMath::drawPixelXY(lx - 1, ly - 1, lightningColor);
            lightning[(lx - 1) + (ly - 1) * WIDTH] = 255; // move down and left
            break;
          case 3:
            EffectMath::drawPixelXY(lx - 1, ly - 1, lightningColor);
            lightning[(lx - 1) + (ly - 1) * WIDTH] = 255; // fork down and left
            EffectMath::drawPixelXY(lx - 1, ly - 1, lightningColor);
            lightning[(lx + 1) + (ly - 1) * WIDTH] = 255; // fork down and right
            break;
          }
        }
      }
    }

    //free(lightning);
    return true;
  }
  return false;
}

// Функция рисует тучу в верхней части матрицы 
void MEffectWrain::Clouds(bool flash)
{
  uint8_t dataSmoothing = 50; //196
  uint16_t noiseX = beatsin16(1, 10, 4000, 0, 150);
  uint16_t noiseY = beatsin16(1, 1000, 10000, 0, 50);
  uint16_t noiseZ = beatsin16(1, 10, 4000, 0, 100);
  uint16_t noiseScale = 50; // A value of 1 will be so zoomed in, you'll mostly see solid colors. A value of 4011 will be very zoomed out and shimmery
  //uint8_t *_noise = (uint8_t *)malloc(WIDTH * cloudHeight);

  // This is the array that we keep our computed noise values in
  //static uint8_t _noise[WIDTH][cloudHeight];
  for (uint8_t x = 0; x < WIDTH; x++)
  {
    int xoffset = noiseScale * x;

    for (uint8_t z = 0; z < cloudHeight; z++) {
      int yoffset = noiseScale * z ;
      uint8_t noiseData = qsub8(inoise8(noiseX + xoffset, noiseY + yoffset, noiseZ), 16);
      noiseData = qadd8(noiseData, scale8(noiseData, 39));
      _noise[x * cloudHeight + z] = scale8(_noise[x * cloudHeight + z], dataSmoothing) + scale8(noiseData, 256 - dataSmoothing);
      if (flash)
        EffectMath::drawPixelXY(x, HEIGHT - z - 1, CHSV(random8(20,30), 250, random8(64, 100)));
      else 
        nblend(EffectMath::getPixel(x, EffectMath::getmaxHeightIndex() - z), ColorFromPalette(_cTargetPalette, _noise[x * cloudHeight + z], _noise[x * cloudHeight + z]), (500 / cloudHeight));
    }
    noiseZ++;
  }

  if (millis() - timer < 300) {
    for (uint8_t i = 0; i < WIDTH; i++)
    {
      //for (byte z = 0; z < 10; z++)
        EffectMath::drawPixelXYF(i, EffectMath::randomf((float)HEIGHT - 4.5, (float)HEIGHT - 2.5), CHSV(0, 250, random8(120, 200)), 0);
    }
  }
}

//-------- по мотивам Эффектов Particle System -------------------------
// https://github.com/fuse314/arduino-particle-sys
// https://github.com/giladaya/arduino-particle-sys
// https://www.youtube.com/watch?v=S6novCRlHV8&t=51s

//при попытке вытащить из этой библиотеки только минимально необходимое выяснилось, что там очередной (третий) вариант реализации субпиксельной графики.
//ну его нафиг. лучше будет повторить визуал имеющимися в прошивке средствами.

void MEffectFairy::particlesUpdate(uint8_t i) {
  trackingObjectState[i] -= 1 * speedFactor; 

  //apply velocity
  trackingObjectPosX[i] += trackingObjectSpeedX[i] * speedFactor;
  trackingObjectPosY[i] += trackingObjectSpeedY[i] * speedFactor;
  if(trackingObjectState[i] == 0 || trackingObjectPosX[i] <= -1 || trackingObjectPosX[i] >= WIDTH || trackingObjectPosY[i] <= -1 || trackingObjectPosY[i] >= HEIGHT) 
    trackingObjectIsShift[i] = false;
}

// ============= ЭФФЕКТ ИСТОЧНИК ===============
// (c) SottNick
// выглядит как https://github.com/fuse314/arduino-particle-sys/blob/master/examples/StarfieldFastLED/StarfieldFastLED.ino

void MEffectFairy::fountEmit(uint8_t i) {
  if (hue++ & 0x01)
    hue2++;

  trackingObjectPosX[i] = WIDTH * 0.5;
  trackingObjectPosY[i] = HEIGHT * 0.5;


  trackingObjectSpeedX[i] = (((float)random8()-127.)/512.); 
  trackingObjectSpeedY[i] = EffectMath::sqrt(0.0626-trackingObjectSpeedX[i] * trackingObjectSpeedX[i]); 
  
  if(random8(2U)) trackingObjectSpeedY[i]=-trackingObjectSpeedY[i];

  trackingObjectState[i] = EffectMath::randomf(50, 250); 
#ifdef MIC_EFFECTS
  if (type)
    trackingObjectHue[i] = isMicOn() ? getMicMapFreq() : hue2;
  else 
    trackingObjectHue[i] = random8(getMicMapFreq(), 255);
#else
  if (type)
    trackingObjectHue[i] = hue2;
  else 
    trackingObjectHue[i] = random8(255);
#endif
  trackingObjectIsShift[i] = true; 
}

void MEffectFairy::fount(CRGB *leds){
  step = deltaValue; //счётчик количества частиц в очереди на зарождение в этом цикле

  EffectMath::dimAll(EffectMath::fmap(speed, 1, 255, 180, 127)); //ахах-ха. очередной эффект, к которому нужно будет "подобрать коэффициенты"

  //go over particles and update matrix cells on the way
  for (int i = 0; i < enlargedObjectNUM; i++) {
    if (!trackingObjectIsShift[i] && step) {
      fountEmit(i);
      step--;
    }
    if (trackingObjectIsShift[i]) { 
      particlesUpdate(i);

      //generate RGB values for particle
      CRGB baseRGB;
        baseRGB = CHSV(trackingObjectHue[i], 255, _video); 

      baseRGB.nscale8(trackingObjectState[i]);
      EffectMath::drawPixelXYF(trackingObjectPosX[i], trackingObjectPosY[i], baseRGB, 0);
    }
  }
  if (blur) EffectMath::blur2d(blur * 10); // Размытие 
}

// ============= ЭФФЕКТ ФЕЯ ===============
// (c) SottNick
#define FAIRY_BEHAVIOR //типа сложное поведение

void MEffectFairy::fairyEmit(uint8_t i) {
    if (deltaHue++ & 0x01)
      if (hue++ & 0x01)
        hue2++;//counter++;
    trackingObjectPosX[i] = boids[0].location.x;
    trackingObjectPosY[i] = boids[0].location.y;

    //хотите навставлять speedFactor? - тут не забудьте
    //trackingObjectSpeedX[i] = ((float)random8()-127.)/512./0.25*speedFactor; 
    trackingObjectSpeedX[i] = ((float)random8()-127.)/512.; 
    trackingObjectSpeedY[i] = EffectMath::sqrt(0.0626-trackingObjectSpeedX[i]*trackingObjectSpeedX[i]); 
    if(random8(2U)) { trackingObjectSpeedY[i]=-trackingObjectSpeedY[i]; }

    trackingObjectState[i] = random8(20, 80); 
    trackingObjectHue[i] = hue2;
    trackingObjectIsShift[i] = true; 
}

bool MEffectFairy::fairy(CRGB * leds) {
  step = deltaValue; //счётчик количества частиц в очереди на зарождение в этом цикле
  
#ifdef FAIRY_BEHAVIOR
  if (!deltaHue && deltaHue2 && fabs(boids[0].velocity.x) + fabs(boids[0].velocity.y) < 0.15){ 
    deltaHue2 = 0U;
    
    boids[1].velocity.x = (((float)random8()+255.) / 4080.);
    boids[1].velocity.y = (((float)random8()+255.) / 2040.);
    if (boids[0].location.x > WIDTH * 0.5) boids[1].velocity.x = -boids[1].velocity.x;
    if (boids[0].location.y > HEIGHT * 0.5) boids[1].velocity.y = -boids[1].velocity.y;
  }
  if (!deltaHue2){
    step = 1U;
    
    boids[0].location.x += boids[1].velocity.x * speedFactor;
    boids[0].location.y += boids[1].velocity.y * speedFactor;
    deltaHue2 = (boids[0].location.x <= 0 || boids[0].location.x >= EffectMath::getmaxWidthIndex() || boids[0].location.y <= 0 || boids[0].location.y >= EffectMath::getmaxHeightIndex());
  }
  else
#endif // FAIRY_BEHAVIOR
  {  
    PVector attractLocation = PVector(WIDTH * 0.5, HEIGHT * 0.5);
    //float attractMass = 10;
    //float attractG = .5;
    // перемножаем и получаем 5.
    Boid boid = boids[0];
    PVector force = attractLocation - boid.location;      // Calculate direction of force
    float d = force.mag();                                // Distance between objects
    d = constrain(d, 5.0f, HEIGHT);//видео снято на 5.0f  // Limiting the distance to eliminate "extreme" results for very close or very far objects
//d = constrain(d, modes[currentMode].Scale / 10.0, HEIGHT);

    force.normalize();                                    // Normalize vector (distance doesn't matter here, we just want this vector for direction)
    float strength = (5. * boid.mass) / (d * d);          // Calculate gravitional force magnitude 5.=attractG*attractMass
//float attractMass = (modes[currentMode].Scale) / 10.0 * .5;
    force *= strength * speedFactor;                                    // Get force vector --> magnitude * direction
    boid.applyForce(force);
    boid.update();
    
    if (boid.location.x <= -1) boid.location.x = -boid.location.x;
    else if (boid.location.x >= WIDTH) boid.location.x = -boid.location.x+WIDTH+WIDTH;
    if (boid.location.y <= -1) boid.location.y = -boid.location.y;
    else if (boid.location.y >= HEIGHT) boid.location.y = -boid.location.y+HEIGHT+HEIGHT;
    boids[0] = boid;

    if (!deltaHue) {
      if (random8(3U)){
        d = ((random8(2U)) ? boids[0].velocity.x : boids[0].velocity.y) * ((random8(2U)) ? .2 : -.2);
        boids[0].velocity.x += d * speedFactor;
        boids[0].velocity.y -= d * speedFactor;
      }
      else {
        if (fabs(boids[0].velocity.x) < 0.02)
          boids[0].velocity.x = -boids[0].velocity.x;
        else if (fabs(boids[0].velocity.y) < 0.02)
          boids[0].velocity.y = -boids[0].velocity.y;
      }
    }
  }

  EffectMath::dimAll(EffectMath::fmap(speed, 1, 255, 180, 127));

  //go over particles and update matrix cells on the way
  for(int i = 0; i<enlargedObjectNUM; i++) {
    if (!trackingObjectIsShift[i] && step) {
      fairyEmit(i);
      step--;
    }
    if (trackingObjectIsShift[i]){ 
      // вернуться и поглядеть, что это
      if (type && trackingObjectSpeedY[i] > -1) trackingObjectSpeedY[i] -= 0.05; //apply acceleration
      particlesUpdate(i);

      //generate RGB values for particle
      CRGB baseRGB = CHSV(trackingObjectHue[i], 255,255); 

      baseRGB.nscale8(trackingObjectState[i]);//эквивалент
      EffectMath::drawPixelXYF(trackingObjectPosX[i], trackingObjectPosY[i], baseRGB, 0);
    }
  }

  EffectMath::drawPixelXYF(boids[0].location.x, boids[0].location.y, CHSV(hue, 160U, 255U) /*temp*/, 0);  
  return true;
}

bool MEffectFairy::run() {
#ifdef MIC_EFFECTS
   _video = isMicOn() ? constrain(getMicMaxPeak() * EffectMath::fmap(gain, 1.0f, 255.0f, 1.25f, 5.0f), 48U, 255U) : 255;
#endif

  switch (effect)
  {
  case EFF_FAIRY:
    return fairy(_ledarray);
    break;
  case EFF_FOUNT:
    fount(_ledarray);
    break; 
  default:
    break;
  }
  //fpsmeter();
  return true;
}


void MEffectFairy::load(uint8_t s){
  //---- Общее для двух эффектов
  if (effect == EFF_FAIRY) speedFactor = EffectMath::fmap(speed, 1, 255, 0.05, .25) * EffectCalc::speedfactor;
  else speedFactor = EffectMath::fmap(speed, 1, 255, 0.2, 1.) * EffectCalc::speedfactor;
  enlargedObjectNUM = map(scale, 1, 255, 4, trackingOBJECT_MAX_COUNT);


  if(effect==EFF_FAIRY)
    deltaValue = 10; // количество зарождающихся частиц за 1 цикл //perCycle = 1;
  else
    deltaValue = enlargedObjectNUM / (EffectMath::sqrt(CENTER_X_MAJOR * CENTER_X_MAJOR + CENTER_Y_MAJOR * CENTER_Y_MAJOR) * 4U) + 1U; // 4 - это потому что за 1 цикл частица пролетает ровно четверть расстояния между 2мя соседними пикселями

  if (enlargedObjectNUM > trackingOBJECT_MAX_COUNT)
    enlargedObjectNUM = trackingOBJECT_MAX_COUNT;
  for (uint16_t i = 0; i < enlargedOBJECT_MAX_COUNT; i++)
    trackingObjectIsShift[i] = false; 

  //---- Только для эффекта Фея
  // лень было придумывать алгоритм для таектории феи, поэтому это будет нулевой "бойд" из эффекта Притяжение
  boids[0] = Boid(random8(WIDTH), random8(HEIGHT));
  boids[0].mass = 0.5;//((float)random8(33U, 134U)) / 100.; // random(0.1, 2); // сюда можно поставить регулятор разлёта. чем меньше число, тем дальше от центра будет вылет
  boids[0].velocity.x = ((float) random8(46U, 100U)) / 500.0;
  if (random8(2U)) boids[0].velocity.x = -boids[0].velocity.x;
  boids[0].velocity.y = 0;
  hue = random8();
  #ifdef FAIRY_BEHAVIOR
    deltaHue2 = 1U;
  #endif
}

// !++
String MEffectFairy::setDynCtrl(UIControl*_val){
  set_dynCtrl(_val);
  if(_val->getId()==1) {
    if (effect == EFF_FAIRY) speedFactor = EffectMath::fmap(EffectCalc::setDynCtrl(_val).toInt(), 1, 255, 0.05, .25) * EffectCalc::speedfactor;
    else speedFactor = EffectMath::fmap(EffectCalc::setDynCtrl(_val).toInt(), 1, 255, 0.2, 1.) * EffectCalc::speedfactor;
  } else if(_val->getId()==2) {
    enlargedObjectNUM = map(EffectCalc::setDynCtrl(_val).toInt(), 1, 255, 4, trackingOBJECT_MAX_COUNT);
    if (enlargedObjectNUM > trackingOBJECT_MAX_COUNT)
      enlargedObjectNUM = trackingOBJECT_MAX_COUNT;
  } else if(_val->getId()==3) type = EffectCalc::setDynCtrl(_val).toInt();
  else if(_val->getId()==4) blur = EffectCalc::setDynCtrl(_val).toInt();
  else if(_val->getId()==5) gain = EffectCalc::setDynCtrl(_val).toInt();
  else EffectCalc::setDynCtrl(_val).toInt(); // для всех других не перечисленных контролов просто дергаем функцию базового класса (если это контролы палитр, микрофона и т.д.)
  return String();
}


// ---------- Эффект "Бульбулятор"
// "Circles" (C) Elliott Kember https://editor.soulmatelights.com/gallery/11
// адаптация и переделка - kostyamat

//!++
String MEffectCircles::setDynCtrl(UIControl*_val){
  set_dynCtrl(_val);
  if(_val->getId()==1) speedFactor = EffectMath::fmap(EffectCalc::setDynCtrl(_val).toInt(), 1, 255, 0.5, 5) * EffectCalc::speedfactor;
  else if(_val->getId()==2) count = map(EffectCalc::setDynCtrl(_val).toInt(), 1, 255, 3, NUMBER_OF_CIRCLES);
  else if(_val->getId()==4) gain = EffectCalc::setDynCtrl(_val).toInt();
  else if(_val->getId()==7) {
    if (!EffectCalc::setDynCtrl(_val).toInt()) {
      _defaultPal= OET_PAL;
    }else {
      _defaultPal= OET_PALGRAD;
    }
  }    
  else EffectCalc::setDynCtrl(_val).toInt(); // для всех других не перечисленных контролов просто дергаем функцию базового класса (если это контролы палитр, микрофона и т.д.)
  return String();
}

void MEffectCircles::load(uint8_t s) {
  count = map(scale, 1, 255, 3, NUMBER_OF_CIRCLES);
  speedFactor = EffectMath::fmap(speed, 1, 255, 0.5, 5) * EffectCalc::speedfactor;
  for (uint8_t i = 0; i < NUMBER_OF_CIRCLES; i++) circles.add( new Circle());
  for (uint8_t i = 0; i < NUMBER_OF_CIRCLES; i++) {
    circles.get(i)->reset();
  }
}

void MEffectCircles::drawCircle(CRGB *leds, Circle circle) {
  int16_t centerX = circle.centerX;
  int16_t centerY = circle.centerY;
  uint8_t hue = circle.hue;
  float radius = circle.radius();

  int16_t startX = centerX - ceil(radius);
  int16_t endX = centerX + ceil(radius);
  int16_t startY = centerY - ceil(radius);
  int16_t endY = centerY + ceil(radius);

  for (int16_t x = startX; x < endX; x++) {
    for (int16_t y = startY; y < endY; y++) {
      float distance = EffectMath::sqrt(sq(x - centerX) + sq(y - centerY));
      if (distance > radius)
        continue;

      uint16_t brightness;
      if (radius < 1) { // last pixel
        brightness = 255.0 * radius;
      } else {
        float percentage = distance / radius;
        float fraction = 1.0 - percentage;
        brightness = (float)_video * fraction;
      }
      EffectMath::getPixel(x, y) += ColorFromPalette(_cTargetPalette, hue, brightness);
    }
  }
}

bool MEffectCircles::run() {
#ifdef MIC_EFFECTS
  _video = isMicOn() ? constrain(getMicMaxPeak() * EffectMath::fmap(gain, 1.0f, 255.0f, 1.25f, 5.0f), 48U, 255U) : 255;
#endif
  
  randomSeed(millis());
  FastLED.clear();
  update();

  for (uint8_t i = 0; i < count; i++) {
    circles.get(i)->bpm += speedFactor;
    if (circles.get(i)->radius() < 0.001) {
#ifdef MIC_EFFECTS
      circles.get(i)->hue = isMicOn() ? getMicMapFreq() : random(0, NUMBER_OF_CIRCLES) * 255 / count;
#else
      circles.get(i)->hue = random(0, NUMBER_OF_CIRCLES) * 255 / count;
#endif
      circles.get(i)->move();
    }
    drawCircle(_ledarray, *circles.get(i));
  }
  return true;
}

// ----------- Эффект "Бенгальские Огни"
// (c) stepko https://wokwi.com/arduino/projects/289797125785520649
// 06.02.2021
void MEffectBengalL::load(uint8_t s) {
  speedFactor = EffectMath::fmap(speed, 1, 255, 0.1, 1) * EffectCalc::speedfactor;
  for (byte i = 0; i < sparksNum; i++) {
    regen(i);
  }
  //speedFactor = EffectMath::fmap(speed, 1, 255, 0.1, 1)*EffectCalc::speedfactor;
}

//!++
String MEffectBengalL::setDynCtrl(UIControl*_val){
  if(_val->getId()==1) speedFactor = EffectMath::fmap(EffectCalc::setDynCtrl(_val).toInt(), 1, 255, 0.1, 1) * EffectCalc::speedfactor;
  else if(_val->getId()==3) centerRun = EffectCalc::setDynCtrl(_val).toInt();
  else EffectCalc::setDynCtrl(_val).toInt(); // для всех других не перечисленных контролов просто дергаем функцию базового класса (если это контролы палитр, микрофона и т.д.)
  return String();
}

void MEffectBengalL::phisics(byte id) {
  sparksPos[0][id] += sparksSpeed[0][id] * speedFactor;
  sparksPos[1][id] += sparksSpeed[1][id] * speedFactor;
  sparksSpeed[1][id] -= .98 * speedFactor;
  sparksSat[id] += (255. / (float)WIDTH) * speedFactor;            // остывание искор
  sparksFade[id] -= (255. / (float)(HEIGHT*1.5)) * speedFactor;    // угасание искор
  if (sparksSpeed[0][id] > 0)
    sparksSpeed[0][id] -= 0.1 * speedFactor;
  else
    sparksSpeed[0][id] += 0.1 * speedFactor;
  if (sparksPos[0][id] <= 0 || sparksPos[0][id] >= WIDTH * 10 || sparksPos[1][id] < 0) {
    regen(id);
  }
}

void MEffectBengalL::regen(byte id) {
  sparksPos[0][id] = gPos[0];
  sparksPos[1][id] = gPos[1];
  sparksSpeed[0][id] = random(-10, 10);
  sparksSpeed[1][id] = random(-5, 20);
  sparksColor[id] = random8();
  sparksSat[id] = 10;
  sparksFade[id] = 255;
}


bool MEffectBengalL::run() {
  fadeToBlackBy(_ledarray, NUM_LEDS, beatsin8(5, 20, 100));
  if (centerRun) {
    gPos[0] = beatsin16(_x, 0, EffectMath::getmaxWidthIndex() * 10);
    gPos[1] = beatsin16(_y, 0, EffectMath::getmaxHeightIndex() * 10);
  } else {
    gPos[0] = WIDTH / 2 * 10;
    gPos[1] = HEIGHT / 2 * 10;
  }
  for (byte i = 0; i < map(scale, 1, 255, 8, sparksNum); i++) {
    phisics(i);
    if (sparksPos[1][i] < (EffectMath::getmaxHeightIndex() * 10) and sparksPos[1][i] >= 0)
      if (sparksPos[0][i] < (EffectMath::getmaxWidthIndex() * 10) and sparksPos[0][i] >= 0)
        EffectMath::drawPixelXYF(sparksPos[0][i] / 10,  sparksPos[1][i] / 10, CHSV(sparksColor[i], constrain(sparksSat[i], 5, 255), constrain(sparksFade[i], 32, 255)));
  }

  EVERY_N_SECONDS(period) {
    for (byte i = 0; i < map(scale, 1, 255, 1, sparksNum); i++) regen(i);
    period = random(10, 60);
    _x = random(1, 10);
    _y = random(1, 10);
  }
  return true;
}

// ----------- Эффект "Шары"
// (c) stepko and kostyamat https://wokwi.com/arduino/projects/289839434049782281
// 07.02.2021
void MEffectBalls::load(uint8_t s) {
  randomSeed(millis());

  speedFactor = EffectMath::fmap(speed, 1, 255, 0.15, 0.5) * EffectCalc::speedfactor;

  for (byte i = 0; i < ballsAmount; i++) {
    radius[i] = EffectMath::randomf(0.5, radiusMax);
    ball[i][2] = EffectMath::randomf(0.5, 1.1) * speedFactor;
    ball[i][3] = EffectMath::randomf(0.5, 1.1) * speedFactor;
    ball[i][0] = random(0, WIDTH);
    ball[i][1] = random(0, HEIGHT);
    color[i] = random(0, 255);
  }
}


// !++
String MEffectBalls::setDynCtrl(UIControl*_val){
  set_dynCtrl(_val);
  if(_val->getId()==1) speedFactor = EffectMath::fmap(EffectCalc::setDynCtrl(_val).toInt(), 1, 255, 0.15, 0.5) * EffectCalc::speedfactor;
  else if(_val->getId()==7) {
    if (!EffectCalc::setDynCtrl(_val).toInt()) {
      _defaultPal= OET_PAL;
    }else {
      _defaultPal= OET_PALGRAD;
    }
  }   
  else EffectCalc::setDynCtrl(_val).toInt(); // для всех других не перечисленных контролов просто дергаем функцию базового класса (если это контролы палитр, микрофона и т.д.)
  return String();
}

bool MEffectBalls::run() {
  update();

  fadeToBlackBy(_ledarray, NUM_LEDS, map(speed, 1, 255, 5, 20));

  for (byte i = 0; i < map(scale, 1, 255, 2, ballsAmount); i++) {
    if (rrad[i]) {  // тут у нас шарики надуваются\сдуваются по ходу движения
      radius[i] += (fabs(ball[i][2]) > fabs(ball[i][3])? fabs(ball[i][2]) : fabs(ball[i][3])) * 0.1 * speedFactor;
      if (radius[i] >= radiusMax) {
        rrad[i] = false;
      }
    } else {
      radius[i] -= (fabs(ball[i][2]) > fabs(ball[i][3])? fabs(ball[i][2]) : fabs(ball[i][3])) * 0.1 * speedFactor;
      if (radius[i] < 1.) {
        rrad[i] = true;
        color[i] = random(0, 255);
      }
    }


    //EffectMath::drawCircleF(ball[i][1], ball[i][0], radius[i], ColorFromPalette(*curPalette, color[i]), 0.5);
    if (radius[i] > 1) 
      EffectMath::fill_circleF(ball[i][1], ball[i][0], radius[i], ColorFromPalette(_cTargetPalette, color[i]));
    else 
      EffectMath::drawPixelXYF(ball[i][1], ball[i][0], ColorFromPalette(_cTargetPalette, color[i]));


    if (ball[i][0] + radius[i] >= EffectMath::getmaxHeightIndex())
      ball[i][0] += (ball[i][2] * ((EffectMath::getmaxHeightIndex() - ball[i][0]) / radius[i] + 0.005));
    else if (ball[i][0] - radius[i] <= 0)
      ball[i][0] += (ball[i][2] * (ball[i][0] / radius[i] + 0.005));
    else
      ball[i][0] += ball[i][2];
    //-----------------------
    if (ball[i][1] + radius[i] >= EffectMath::getmaxWidthIndex())
      ball[i][1] += (ball[i][3] * ((EffectMath::getmaxWidthIndex() - ball[i][1]) / radius[i] + 0.005));
    else if (ball[i][1] - radius[i] <= 0)
      ball[i][1] += (ball[i][3] * (ball[i][1] / radius[i] + 0.005));
    else
      ball[i][1] += ball[i][3];
    //------------------------
    if (ball[i][0] < 0.01) {
      ball[i][2] = EffectMath::randomf(0.5, 1.1) * speedFactor;
      ball[i][0] = 0.01;
    }
    else if (ball[i][0] > (float)EffectMath::getmaxHeightIndex()) {
      ball[i][2] = EffectMath::randomf(0.5, 1.1) * speedFactor;
      ball[i][2] = -ball[i][2];
      ball[i][0] = (float)EffectMath::getmaxHeightIndex();
    }
    //----------------------
    if (ball[i][1] < 0.01) {
      ball[i][3] = EffectMath::randomf(0.5, 1.1) * speedFactor;
      ball[i][1] = 0.01;
    }
    else if (ball[i][1] > EffectMath::getmaxWidthIndex()) {
      ball[i][3] = EffectMath::randomf(0.5, 1.1) * speedFactor;
      ball[i][3] = -ball[i][3];
      ball[i][1] = EffectMath::getmaxWidthIndex();
    }
  }
  EffectMath::blur2d(_ledarray, WIDTH, HEIGHT, 48);
  return true;
}

// ---------- Ефект лабіринт
//reworked by stepko
void MEffectMaze::newGameMaze() {
  playerPos[0] = !MAZE_SHIFT;
  playerPos[1] = !MAZE_SHIFT;

  gameOverFlag = false;
  buttons = 4;

  GenerateMaze(maze, MAZE_WIDTH, MAZE_HEIGHT);    // генерировать лабиринт обычным способом
  SolveMaze(maze, MAZE_WIDTH, MAZE_HEIGHT);       // найти путь

  if (!(GAMEMODE || mazeMode)) {
    for (byte y = 0; y < MAZE_HEIGHT; y++) {
      for (byte x = 0; x < MAZE_WIDTH; x++) {
        switch (maze[(y + MAZE_SHIFT) * MAZE_WIDTH + (x + MAZE_SHIFT)]) {
          case 1:  EffectMath::drawPixelXY(x, y, color); break;
          case 2:
            EffectMath::drawPixelXY(x, y, 0x000000);
            break;
          default: EffectMath::drawPixelXY(x, y, 0x000000); break;
        }
      }
      // Отображаем сгенерированный лабиринт строка за строкой
      FastLED.show();  // стоит убрать, и все начинает мерцать
      delay(50);
    }

  } else {
    for (byte y = 0; y < FOV; y++) {
      for (byte x = 0; x < FOV; x++) {
        switch (maze[(y + MAZE_SHIFT) * MAZE_WIDTH + (x + MAZE_SHIFT)]) {
          case 1:  EffectMath::drawPixelXY(x, y, color);  break;
          case 2:
            EffectMath::drawPixelXY(x, y, 0x000000);
            break;
          default: EffectMath::drawPixelXY(x, y, 0x000000);  break;
        }
      }
    }
  }

  // Отрисовка - с видимыми границами по периметру (настройки MAZE_SHIFT выше)
  // Слева от начальной позиции делаем дыру - это вход
  if (playerPos[0]>0) {
    playerPos[0] = playerPos[0] - 1;
    EffectMath::drawPixelXY(playerPos[0], playerPos[1], 0x000000);
  }
  
  EffectMath::drawPixelXY(playerPos[0], playerPos[1],  playerColor);

  mazeStarted = false;  
}
void MEffectMaze::load(uint8_t s) {
    FastLED.clear();

}
bool MEffectMaze::run() {
  if (loadingFlag || gameOverFlag) {  
    if (loadingFlag) FastLED.clear();
    gameTimer.setInterval(map(speed, 1, 255, 500, 50));   // установить начальную скорость
    loadingFlag = false;
    newGameMaze();
    // modeCode = MC_MAZE;
  }

  if (gameDemo && !gamePaused) demoMaze();
  buttonsTickMaze();
  return true;
}

void MEffectMaze::buttonsTickMaze() {
  if (gameDemo && !gameTimer.isReady()) { // тут крутим скорость в демо-режиме
    return;
  }

  if (checkButtons()) {
    bool btnPressed = false;
    if (buttons == 3) {   // кнопка нажата
      btnPressed = true;
      int8_t newPos = playerPos[0] - 1;
      if (newPos >= 0 && newPos <= (int)WIDTH - 1)
        if (EffectMath::getPixColorXY(newPos, playerPos[1]) == 0) {
          movePlayer(newPos, playerPos[1], playerPos[0], playerPos[1]);
          playerPos[0] = newPos;
        }
    }
    if (buttons == 1) {   // кнопка нажата
      btnPressed = true;
      int8_t newPos = playerPos[0] + 1;
      if (newPos >= 0 && newPos <= (int)WIDTH - 1)
        if (EffectMath::getPixColorXY(newPos, playerPos[1]) == 0) {
          movePlayer(newPos, playerPos[1], playerPos[0], playerPos[1]);
          playerPos[0] = newPos;
        }
    }
    if (buttons == 0) {   // кнопка нажата
      btnPressed = true;
      int8_t newPos = playerPos[1] + 1;
      if (newPos >= 0 && newPos <= (int)HEIGHT - 1)
        if (EffectMath::getPixColorXY(playerPos[0], newPos) == 0) {
          movePlayer(playerPos[0], newPos, playerPos[0], playerPos[1]);
          playerPos[1] = newPos;
        }
    }
    if (buttons == 2) {   // кнопка нажата
      btnPressed = true;
      int8_t newPos = playerPos[1] - 1;
      if (newPos >= 0 && newPos <= (int)HEIGHT - 1)
        if (EffectMath::getPixColorXY(playerPos[0], newPos) == 0) {
          movePlayer(playerPos[0], newPos, playerPos[0], playerPos[1]);
          playerPos[1] = newPos;
        }
    }
    if (btnPressed && !mazeStarted) {
      mazeStarted = true;
      labTimer = millis();
    }
    buttons = 4;
  }
}

void MEffectMaze::movePlayer(int8_t nowX, int8_t nowY, int8_t prevX, int8_t prevY) {
  if (!track) EffectMath::drawPixelXY(prevX, prevY, 0x000000);
  EffectMath::drawPixelXY(nowX, nowY,  playerColor);

  if ((nowX == (MAZE_WIDTH - 2) - MAZE_SHIFT) && (nowY == (MAZE_HEIGHT - 1) - MAZE_SHIFT)) {
    gameOverFlag = true;

  /*  FastLED.show();
    delay(250);
    FastLED.clear();
    //if (!gameDemo) {
      displayScore((millis() - labTimer) / 1000);
      FastLED.show();
    //}
    delay(1500);*/
    return;
  }

  if (GAMEMODE || mazeMode) {
    for (int8_t y = nowY - FOV; y < nowY + FOV; y++) {
      for (int8_t x = nowX - FOV; x < nowX + FOV; x++) {
        if (x < 0 || x > (int)WIDTH - 1 || y < 0 || y > (int)HEIGHT - 1) continue;
        if (maze[(y + MAZE_SHIFT) * MAZE_WIDTH + (x + MAZE_SHIFT)] == 1) {
          EffectMath::drawPixelXY(x, y, GLOBAL_COLOR_1);
        }
      }
      FastLED.show();
    }
  }
}

void MEffectMaze::demoMaze() {
  if (checkPath(0, 1)) buttons = 0;
  if (checkPath(1, 0)) buttons = 1;
  if (checkPath(0, -1)) buttons = 2;
  if (checkPath(-1, 0)) buttons = 3;
}

bool MEffectMaze::checkPath(int8_t x, int8_t y) {
  // если проверяемая клетка является путью к выходу
  if ( (maze[(playerPos[1] + y + MAZE_SHIFT) * MAZE_WIDTH + (playerPos[0] + x + MAZE_SHIFT)]) == 2) {
    maze[(playerPos[1] + MAZE_SHIFT) * MAZE_WIDTH + (playerPos[0] + MAZE_SHIFT)] = 4;   // убираем текущую клетку из пути (2 - метка пути, ставим любое число, например 4)
    return true;
  }
  else return false;
}

// копаем лабиринт
void MEffectMaze::CarveMaze(char *maze, int width, int height, int x, int y) {
  int x1, y1;
  int x2, y2;
  int dx, dy;
  int dir, count;

  dir = random(10) % 4;
  count = 0;
  while (count < 4) {
    dx = 0; dy = 0;
    switch (dir) {
      case 0:  dx = 1;  break;
      case 1:  dy = 1;  break;
      case 2:  dx = -1; break;
      default: dy = -1; break;
    }
    x1 = x + dx;
    y1 = y + dy;
    x2 = x1 + dx;
    y2 = y1 + dy;
    if (   x2 > 0 && x2 < width && y2 > 0 && y2 < height
           && maze[y1 * width + x1] == 1 && maze[y2 * width + x2] == 1) {
      maze[y1 * width + x1] = 0;
      maze[y2 * width + x2] = 0;
      x = x2; y = y2;
      dir = random(10) % 4;
      count = 0;
    } else {
      dir = (dir + 1) % 4;
      count += 1;
    }
  }
}

// генерацтор лабиринта
void MEffectMaze::GenerateMaze(char *maze, int width, int height) {
  int x, y;
  for (x = 0; x < width * height; x++) {
    maze[x] = 1;
  }
  maze[1 * width + 1] = 0;
  for (y = 1; y < height; y += 2) {
    for (x = 1; x < width; x += 2) {
      CarveMaze(maze, width, height, x, y);
    }
  }
  // вход и выход
  maze[0 * width + 1] = 0;
  maze[(height - 1) * width + (width - 2)] = 0;

  track = random8(0,2);
  color = CHSV(hue += 8, random8(192, 255), 192);
  
 /* CHSV tmp = rgb2hsv_approximate(color);
  tmp.s = 255;
  tmp.hue += 100;
  tmp.val = 255; */

  playerColor = CHSV(hue + random(63, 127), random8(127, 200), 255);;
  
}

// решатель (ищет путь)
void MEffectMaze::SolveMaze(char *maze, int width, int height) {
  int dir, count;
  int x, y;
  int dx, dy;
  int forward;
  // Remove the entry and exit. 
  maze[0 * width + 1] = 1;
  maze[(height - 1) * width + (width - 2)] = 1;

  forward = 1;
  dir = 0;
  count = 0;
  x = 1;
  y = 1;
  unsigned int attempts = 0;
  while (x != width - 2 || y != height - 2) {
    if (attempts++ > maxSolves) {   // если решатель не может найти решение (maxSolves в 5 раз больше числа клеток лабиринта)
      gameOverFlag = true;          // перегенерировать лабиринт
      break;                        // прервать решение
    }
    dx = 0; dy = 0;
    switch (dir) {
      case 0:  dx = 1;  break;
      case 1:  dy = 1;  break;
      case 2:  dx = -1; break;
      default: dy = -1; break;
    }
    if (   (forward  && maze[(y + dy) * width + (x + dx)] == 0)
           || (!forward && maze[(y + dy) * width + (x + dx)] == 2)) {
      maze[y * width + x] = forward ? 2 : 3;
      x += dx;
      y += dy;
      forward = 1;
      count = 0;
      dir = 0;
    } else {
      dir = (dir + 1) % 4;
      count += 1;
      if (count > 3) {
        forward = 0;
        count = 0;
      }
    }
  }
  
  // Replace the entry and exit.
  maze[(height - 2) * width + (width - 2)] = 2;
  maze[(height - 1) * width + (width - 2)] = 2;
}

void MEffectMaze::setspd(const byte _spd)
{
  EffectCalc::setspd(_spd);
  gameTimer.setInterval(map(speed, 1, 255, 500, 50));   // установить скорость
}

// --------- Эффект "Вьющийся Цвет"
// (c) Stepko https://wokwi.com/arduino/projects/283705656027906572
// доработка - kostyamat
bool MEffect_Frizzles::run() {
  _speed = EffectMath::fmap(speed, 1, 255, 0.25, 3);
  if (scale <= 127) _scale = EffectMath::fmap(scale, 1, 255, 1, 8);
  else _scale = EffectMath::fmap(scale, 1, 255, 8, 1);

  for(float i= (float)8 * _scale; i> 0; i--)
    EffectMath::drawPixelXY(
      beatsin8(12. * _speed + i * _speed, 0, EffectMath::getmaxWidthIndex()), 
      beatsin8(15. * _speed + i * _speed, 0, EffectMath::getmaxHeightIndex()), 
      CHSV(beatsin8(12. * _speed, 0, 255), scale > 127 ? 255 - i*8 : 255, scale > 127 ? 127 + i*8 : 255)
      );
  EffectMath::blur2d(_ledarray, WIDTH, HEIGHT, 16);
  return true;
}

// --------- Эффект "Вьющийся Цвет"
// (c) Stepko https://wokwi.com/arduino/projects/283705656027906572
// доработка - kostyamat
void MEffectRacer::load(uint8_t s ) {
  speedFactor = _speed * EffectMath::fmap(speed, 1, 255, 0.33, 2)*3;
  addRadius = _addRadius * EffectMath::fmap(speed, 1, 255, 0.33, 2)*3;
  aimChange();
}
String MEffectRacer::setDynCtrl(UIControl*_val){
  set_dynCtrl(_val);
  if(_val->getId()==1) {
    uint8_t sp = EffectCalc::setDynCtrl(_val).toInt();
    speedFactor = _speed * EffectMath::fmap(sp, 1, 255, 0.33, 2)*3;
    addRadius = _addRadius * EffectMath::fmap(sp, 1, 255, 0.33, 2)*3;
  } 
  else if(_val->getId()==7) {
    if (!EffectCalc::setDynCtrl(_val).toInt()) {
      _defaultPal= OET_PAL;
    }else {
      _defaultPal= OET_PALGRAD;
    }
  }   
  else EffectCalc::setDynCtrl(_val).toInt(); // для всех других не перечисленных контролов просто дергаем функцию базового класса (если это контролы палитр, микрофона и т.д.)
  return String();
}

bool MEffectRacer::run() {
  if (dryrun(2)) return false;
  update();

  fadeToBlackBy(_ledarray, NUM_LEDS, 16 * speedFactor);

  if (round(posX / 4) > aimX) {
    posX -= speedFactor;
  }
  if (round(posY / 4) > aimY) {
    posY -= speedFactor;
  }
  if (round(posX / 4) < aimX) {
    posX += speedFactor;
  }
  if (round(posY / 4) < aimY) {
    posY += speedFactor;
  }
  if (round(posX / 4) == aimX && round(posY / 4) == aimY) {
    aimChange();
  }
  radius += addRadius;
  angle += radius;
  switch (hue%3)
  {
  case 0:
    EffectMath::drawCircleF(aimX, aimY, radius, color); // рисуем круг
    break;
  
  case 1:
    drawStarF(aimX, aimY, 1.3 * radius, radius, 4, angle, color); // рисуем квадрат
    break;
  case 2:
    drawStarF(aimX, aimY, 2 * radius, radius, starPoints, angle, color); // рисуем звезду
    break;
  }
  
  EffectMath::drawPixelXYF(posX / 4, posY / 4, CHSV(0, 0, 255)); // отрисовываем бегуна

  return true;
}

void MEffectRacer::aimChange() {
  aimX = random(0, WIDTH-1);  // позиция цели 
  aimY = random(0, HEIGHT-1);
  radius = 1; // начальный размер цели = 1 пиксель
  hue = millis()>>1; //random(0, 255);
  color = ColorFromPalette(_cTargetPalette, hue, 180);
  starPoints = random(3, 7); // количество лучей у звезды
}

void MEffectRacer::drawStarF(float x, float y, float biggy, float little, int16_t points, float dangle, CRGB color) {
  float radius2 = 255.0 / points;
  for (int i = 0; i < points; i++) {
    EffectMath::drawLineF(x + ((little * (sin8(i * radius2 + radius2 / 2 - dangle) - 128.0)) / 128), y + ((little * (cos8(i * radius2 + radius2 / 2 - dangle) - 128.0)) / 128), x + ((biggy * (sin8(i * radius2 - dangle) - 128.0)) / 128), y + ((biggy * (cos8(i * radius2 - dangle) - 128.0)) / 128), color);
    EffectMath::drawLineF(x + ((little * (sin8(i * radius2 - radius2 / 2 - dangle) - 128.0)) / 128), y + ((little * (cos8(i * radius2 - radius2 / 2 - dangle) - 128.0)) / 128), x + ((biggy * (sin8(i * radius2 - dangle) - 128.0)) / 128), y + ((biggy * (cos8(i * radius2 - dangle) - 128.0)) / 128), color);

  }
}


// --------- Эффект "Северное Сияние"
// (c) kostyamat 05.02.2021
// идеи подсмотрены тут https://www.reddit.com/r/FastLED/comments/jyly1e/challenge_fastled_sketch_that_fits_entirely_in_a/
void MEffectPolarL::load(uint8_t s) {
  adjastHeight = EffectMath::fmap((float)HEIGHT, 8, 32, 28, 12);
  adjScale = map((int)WIDTH, 8, 64, 310, 63);
  _scale = map(scale, 1, 255, 30, adjScale);
  _speed = map(speed, 1, 255, 128, 16);
  palettesload();
}

void MEffectPolarL::palettesload(){
  // собираем свой набор палитр для эффекта
  palettes.reserve(numpalettes);
  palettes.push_back(&AuroraColors_p); 
  palettes.push_back(&RainbowColors_p);
  palettes.push_back(&PartyColors_p);
  palettes.push_back(&HeatColors_p);
  palettes.push_back(&RainbowColors_p);
  palettes.push_back(&HeatColors_p);
  palettes.push_back(&LithiumFireColors_p);
  palettes.push_back(&GreenAuroraColors_p); 
  palettes.push_back(&BlueAuroraColors_p);
  palettes.push_back(&NeonAuroraColors_p);
  palettes.push_back(&PotassiumFireColors_p);
  palettes.push_back(&WoodFireColors_p);
  palettes.push_back(&SodiumFireColors_p);
  palettes.push_back(&WaterfallColors_p); 
   
  usepalettes = true; // включаем флаг палитр
  scale2pallete();    // выставляем текущую палитру
}

// нахрена кастомный мапинг тут???
// А потому, что мне нужно переменную flag получить! Есть варианты? Вариант ничем не хуже setDynCtrl

void MEffectPolarL::palettemap(std::vector<PGMPalette*> &_pals, const uint8_t _val, const uint8_t _min, const uint8_t _max){
  std::size_t idx = (_val-1); // т.к. сюда передается точное значение контрола, то приводим его к 0
  if (!_pals.size() || idx>=_pals.size()) {
    Serial.println(F("No palettes loaded or wrong value!"));
    return;
  }
  flag = idx;
  curPalette = _pals.at(idx);
}

// !++
String MEffectPolarL::setDynCtrl(UIControl*_val){
  set_dynCtrl(_val);
  if(_val->getId()==3) setPalette(_val);
  _scale = map(scale, 1, 255, 30, adjScale);
  _speed = map(speed, 1, 255, 128, 16);  
  return String();
}

bool MEffectPolarL::run() {
  
  for (byte x = 0; x < WIDTH; x++) {
    for (byte y = 0; y < HEIGHT; y++) {
      timer++;
      //uint16_t i = x*y;
      EffectMath::drawPixelXY(x, y, 
          ColorFromPalette(*curPalette,
            qsub8(
              inoise8(/*i*/timer % 2 + x * _scale,
                y * 16 + timer % 16,
                timer / _speed
              ),
              fabs((float)HEIGHT/2 - (float)y) * adjastHeight
            )
          ));
      if (flag == 1) { // Тут я модифицирую стандартные палитры 
        CRGB tmpColor = EffectMath::getPixColorXY(x, y);
        CRGB led = tmpColor;
        led.g = tmpColor.r;
        led.r = tmpColor.g;
        led.g /= 6;
        led.r += led.r < 206 ? 48 : 0;
        EffectMath::drawPixelXY(x, y, led);
      } else if (flag == 3) {
        CRGB led = EffectMath::getPixColorXY(x, y);
        led.b += 48;
        led.g += led.g < 206 ? 48 : 0;
        EffectMath::drawPixelXY(x, y, led);
      }
    }
  }

  return true;
}

// ----------------- Эффект "Магма"
// (c) Сотнег (SottNick) 2021
// адаптация и доводка до ума - kostyamat
void MEffect_Magma::load(uint8_t s) {
  regen();
  speedFactor = EffectMath::fmap(speed, 1, 255, 0.075, .5) * EffectCalc::speedfactor;
  palettesload();
}

// !++
String MEffect_Magma::setDynCtrl(UIControl*_val){
  set_dynCtrl(_val);
  if (_val->getId()==1) speedFactor = EffectMath::fmap(EffectCalc::setDynCtrl(_val).toInt(), 1, 255, 0.075, .5) * EffectCalc::speedfactor;
  else if(_val->getId()==2) ObjectNUM = map(EffectCalc::setDynCtrl(_val).toInt(), 1, 100, WIDTH/2, enlargedOBJECT_MAX_COUNT);
  else if(_val->getId()==5) {setPalette(_val);}
  else EffectCalc::setDynCtrl(_val).toInt(); // для всех других не перечисленных контролов просто дергаем функцию базового класса (если это контролы палитр, микрофона и т.д.)
  regen();
  return String();
}

void MEffect_Magma::palettesload(){
  // собираем свой набор палитр для эффекта
  palettes.reserve(12);
  palettes.push_back(&MagmaColor_p);
  palettes.push_back(&CopperFireColors_p);
  palettes.push_back(&NormalFire_p);
  palettes.push_back(&SodiumFireColors_p);
  palettes.push_back(&HeatColors2_p);
  palettes.push_back(&PotassiumFireColors_p);
  palettes.push_back(&NormalFire3_p);
  palettes.push_back(&AlcoholFireColors_p);
  palettes.push_back(&NormalFire2_p);
  palettes.push_back(&LithiumFireColors_p);
  palettes.push_back(&WoodFireColors_p);
  palettes.push_back(&WaterfallColors_p);
  
  usepalettes = true; // включаем флаг палитр
  scale2pallete();    // выставляем текущую палитру
}

void MEffect_Magma::regen() {
  for (uint8_t j = 0; j < HEIGHT; j++) {
    shiftHue[j] = map(j, 0, HEIGHT+HEIGHT/4, 255, 0);// init colorfade table
  }


  for (uint8_t i = 0 ; i < enlargedOBJECT_MAX_COUNT ; i++) {
    LeapersRestart_leaper(i);  
    trackingObjectHue[i] = 50U;
  }
}

bool MEffect_Magma::run() {

  fadeToBlackBy(_ledarray, NUM_LEDS, 50);

  for (uint8_t i = 0; i < ObjectNUM; i++) {
    LeapersMove_leaper(i);
    EffectMath::drawPixelXYF(trackingObjectPosX[i], trackingObjectPosY[i], ColorFromPalette(*curPalette, trackingObjectHue[i]), 0);
  }

  for (uint8_t i = 0; i < WIDTH; i++) {
    for (uint8_t j = 0; j < HEIGHT; j++) {
     EffectMath::getPixel(i, EffectMath::getmaxHeightIndex() - j) += ColorFromPalette(*curPalette, qsub8(inoise8(i * deltaValue, (j + ff_y + random8(2)) * deltaHue, ff_z), shiftHue[j]), 127U);
    }
  }

  ff_y += speedFactor * 2;
  ff_z += speedFactor;
  //EffectMath::blur2d(leds, WIDTH, HEIGHT, 32);
  return true;
}

void MEffect_Magma::LeapersMove_leaper(uint8_t l) {

  trackingObjectPosX[l] += trackingObjectSpeedX[l] * speedFactor;
  trackingObjectPosY[l] += trackingObjectShift[l] * speedFactor;

  // bounce off the ceiling?
  if (trackingObjectPosY[l] > HEIGHT + HEIGHT/4) {
    trackingObjectShift[l] = -trackingObjectShift[l];
  }
  
  // settled on the floor?
  if (trackingObjectPosY[l] <= (HEIGHT/8-1)) {
    LeapersRestart_leaper(l);
  }

  // bounce off the sides of the screen?
  if (trackingObjectPosX[l] < 0 || trackingObjectPosX[l] > EffectMath::getmaxWidthIndex()) {
    LeapersRestart_leaper(l);
  }
  
  trackingObjectShift[l] -= Gravity * speedFactor;
}

void MEffect_Magma::LeapersRestart_leaper(uint8_t l) {
  randomSeed(millis());
  // leap up and to the side with some random component
  trackingObjectSpeedX[l] = EffectMath::randomf(-0.75, 0.75);
  trackingObjectShift[l] = EffectMath::randomf(0.50, 0.85);
  trackingObjectPosX[l] = EffectMath::randomf(0, WIDTH);
  trackingObjectPosY[l] = EffectMath::randomf(0, (float)HEIGHT/4-1);

  // for variety, sometimes go 100% faster
  if (random8() < 12) {
    trackingObjectShift[l] += trackingObjectShift[l] * EffectMath::randomf(1.5, 2.5);
  }
}


// --------------------- Эффект "Звездный Десант"
// Starship Troopers https://editor.soulmatelights.com/gallery/839-starship-troopers
// Based on (c) stepko`s codes https://editor.soulmatelights.com/gallery/639-space-ships
// reworked (c) kostyamat (subpixel, shift speed control, etc)  08.04.2021

// !++
String MEffectStarShips::setDynCtrl(UIControl*_val){
  set_dynCtrl(_val);
  if (_val->getId()==1) {
    _SSfade = map(speed, 1, 255, 35, 25);
    speedFactor = EffectMath::fmap(speed, 1, 255, 0.20, 1);
    chance = 4096. / speedFactor;
  }
  else if (_val->getId()==5) {
    _dir = EffectCalc::setDynCtrl(_val).toInt();
  }
  else EffectCalc::setDynCtrl(_val).toInt(); // для всех других не перечисленных контролов просто дергаем функцию базового класса (если это контролы палитр, микрофона и т.д.)
  return String();
}

void MEffectStarShips::load(uint8_t s) {
  FastLED.clear();
  _SSfade = map(speed, 1, 255, 35, 25);
  speedFactor = EffectMath::fmap(speed, 1, 255, 0.20, 1);
  chance = 4096. / speedFactor;  
}

void MEffectStarShips::MoveX(uint8_t am = 128, int8_t amplitude = 1, float shift = 0) {
  CRGB ledsbuff[WIDTH];
  for (uint8_t y = 0; y < HEIGHT; y++) {
    int16_t amount = ((int16_t)am - 128) * 2 * amplitude + shift * 256  ;
    int8_t delta = abs(amount) >> 8 ;
    int8_t fraction = abs(amount) & 255;
    for (uint8_t x = 0 ; x < WIDTH; x++) {
      if (amount < 0) {
        zD = x - delta; zF = zD - 1;
      } else {
        zD = x + delta; zF = zD + 1;
      }
      CRGB PixelA = CRGB::Black  ;
      if ((zD >= 0) && (zD < WIDTH)) PixelA = EffectMath::getPixel(zD, y);
      CRGB PixelB = CRGB::Black ;
      if ((zF >= 0) && (zF < WIDTH)) PixelB = EffectMath::getPixel(zF, y);
      ledsbuff[x] = (PixelA.nscale8(ease8InOutApprox(255 - fraction))) + (PixelB.nscale8(ease8InOutApprox(fraction)));   // lerp8by8(PixelA, PixelB, fraction );
    }
    for(uint8_t x = 0; x < WIDTH; x++){
    EffectMath::getPixel(x, y) = ledsbuff[x];}
  }
}

void MEffectStarShips::MoveY(uint8_t am = 128, int8_t amplitude = 1, float shift = 0) {
  CRGB ledsbuff[HEIGHT];
  for (uint8_t x = 0; x < WIDTH; x++) {
    int16_t amount = ((int16_t)am - 128) * 2 * amplitude + shift * 256 ;
    int8_t delta = abs(amount) >> 8 ;
    int8_t fraction = abs(amount) & 255;
    for (uint8_t y = 0 ; y < HEIGHT; y++) {
      if (amount < 0) {
        zD = y - delta; zF = zD - 1;
      } else {
        zD = y + delta; zF = zD + 1;
      }
      CRGB PixelA = CRGB::Black ;
      if ((zD >= 0) && (zD < HEIGHT)) PixelA = EffectMath::getPixel(x, zD);
      CRGB PixelB = CRGB::Black ;
      if ((zF >= 0) && (zF < HEIGHT)) PixelB = EffectMath::getPixel(x, zF);
      ledsbuff[y] = (PixelA.nscale8(ease8InOutApprox(255 - fraction))) + (PixelB.nscale8(ease8InOutApprox(fraction)));
    }
    for(uint8_t y = 0; y < HEIGHT; y++){
    EffectMath::getPixel(x, y) = ledsbuff[y];}
  }
}

bool MEffectStarShips::run() {
  update();
  fadeToBlackBy(_ledarray, NUM_LEDS, _SSfade);
  switch (_dir) {
    case 1: // Up
        MoveY(255);
      break;
    case 2: // Up - Right 
        MoveY(255);
        MoveX(255);
      break;
    case 3: // Right
        MoveX(255);
      break;
    case 4: // Down - Right 
        MoveY(0);
        MoveX(255);
      break;
    case 5: // Down
        MoveY(0);
      break;
    case 6: // Down - Left
       MoveY(0);
      MoveX(0);
      break;
    case 7: // Left
        MoveX(0);
      break;
    case 8: // Up - Left 
        MoveY(255);
        MoveX(0);
      break;
   default:
    MoveY(beatsin88(1460*speedFactor, 0, 255, 0, 32768));
    MoveX(beatsin88(1502*speedFactor, 0, 255, 0, 0));
    break;  }

  for (byte i = 0; i < scale; i++) {
    float x = (float)beatsin88(3840*speedFactor + i*256, 0, EffectMath::getmaxWidthIndex() *4, 0, scale*i*256) /4;
    float y = (float)beatsin88(3072*speedFactor + i*256, 0, EffectMath::getmaxWidthIndex() *4, 0, 0) /4;
    if ((x >= 0 and x <= EffectMath::getmaxWidthIndex()) and (y >= 0 and y <= EffectMath::getmaxHeightIndex())) draw(x, y, ColorFromPalette(_cTargetPalette, beatsin88(256*12.*speedFactor + i*256, 0, 255), 255));
  }
  EffectMath::blur2d(16);
  return true;
}

void MEffectStarShips::draw(float x, float y, CRGB color) {
  EffectMath::drawPixelXYF(x, y, color);
  if (WIDTH > 24 || HEIGHT > 24) {
    if (x < WIDTH - 2) EffectMath::drawPixelXYF(x + 1, y, color);
    if (x > 1) EffectMath::drawPixelXYF(x - 1, y, color);
    if (y < HEIGHT - 2) EffectMath::drawPixelXYF(x, y + 1, color);
    if (y > 1) EffectMath::drawPixelXYF(x, y - 1, color);
  }
}



// ------------- Эффект "Флаги"
// (c) Stepko + kostyamat
// 17.03.21
// https://editor.soulmatelights.com/gallery/739-flags
String MEffectFlags::setDynCtrl(UIControl*_val){
  set_dynCtrl(_val);
  if (_val->getId()==1)
    {_speed = map(EffectCalc::setDynCtrl(_val).toInt(), 1, 255, 1, 16);}
  else if (_val->getId()==3) _flag = EffectCalc::setDynCtrl(_val).toInt();
  else EffectCalc::setDynCtrl(_val).toInt(); // для всех других не перечисленных контролов просто дергаем функцию базового класса (если это контролы палитр, микрофона и т.д.)
  return String();
}

bool MEffectFlags::run() {
  changeFlags();
  fadeToBlackBy(_ledarray, NUM_LEDS, 32);
  for (uint8_t i = 0; i < WIDTH; i++) {
    thisVal = mix(inoise8(((float) i * DEVIATOR)-counter,counter/2,(float)i*SPEED_ADJ),128,i*(255/WIDTH));
    thisMax = map(thisVal, 0, 255, 0, HEIGHT - 1);
  for (byte j = 0; j < HEIGHT; j++) {
    if(j>((flag == 2 || flag == 7)? (HEIGHT-1-thisMax+HEIGHT/2) :(thisMax+HEIGHT/2)) || ((int8_t)j < (int8_t)((flag == 2 || flag == 7)?HEIGHT/2-1-thisMax:thisMax-HEIGHT/2))) EffectMath::getPixel(i, j)=0; else
    switch (flag)
    {
    case 1:
      germany(i,j);
      break;
    case 2:
      usa(i,j);
      break;
    case 3:
    case 0:
      ukraine(i,j);
      break;
    case 4:
      belarus(i,j);
      break;
    case 5:
      italy(i,j);
      break;
    case 6:
      spain(i,j);
      break;
    case 7:
      uk(i,j);
      break;
    case 8:
      france(i,j);
      break;
    case 9:
      poland(i,j);
      break;
    
    default:
      break;
    }

  }}
  EffectMath::blur2d(_ledarray, WIDTH, HEIGHT, 32);
  counter += (float)_speed * SPEED_ADJ;
  return true;
}

void MEffectFlags::changeFlags() {
  if (!_flag) {
    EVERY_N_SECONDS(CHANGE_FLAG) {
      count++;
      flag = count % 10;
    }
  }
  else
    flag = _flag - 1;
}





/*
  Source 
    https://github.com/DmytroKorniienko/FireLamp_EmbUI/blob/master/src/effects.cpp 

  _effeNamIDList "constleds.cpp"
    MEffectFire2021
    ALMLPT_EFF_MEffectFire2021
    E_FX_NFIRE2021
  workerset "eff_w.h"
    effect
      48

  eff
    1   - speed
    2   - scale   
    5   -         DFTINTF_088
    11  -         DFTINTF_120 
    12  -         DFTINTF_121 
*/
// ----------- Эффект "Огненная Лампа"
// https://editor.soulmatelights.com/gallery/546-fire
// (c) Stepko 17.06.21
// sparks (c) kostyamat 10.01.2022 https://editor.soulmatelights.com/gallery/1619-fire-with-sparks
void MEffectFire2021::load(uint8_t sp) {
  speedFactor = map(speed, 1, 255, 20, 100) * speedfactor;
  _scale = map(scale, 1, 100, 32, 132);  



  for (uint8_t i = 0; i < sparksCount; i++) sparks.add( new Spark());
  for (uint8_t i = 0; i < sparks.size(); i++) 
    sparks.get(i)->reset(*&_ledarray);
}

// !++
String MEffectFire2021::setDynCtrl(UIControl*_val) {
  set_dynCtrl(_val);
  if(_val->getId()==1) speedFactor = map(EffectCalc::setDynCtrl(_val).toInt(), 1, 255, 20, 100) * speedfactor;
  else if(_val->getId()==2) _scale = map(EffectCalc::setDynCtrl(_val).toInt(), 1, 100, 32, 132);
  else if(_val->getId()==5) withSparks = EffectCalc::setDynCtrl(_val).toInt();
  else EffectCalc::setDynCtrl(_val).toInt(); // для всех других не перечисленных контролов просто дергаем функцию базового класса (если это контролы палитр, микрофона и т.д.)
  return String();
}

bool MEffectFire2021::run() {
  if (dryrun(2.)) return false;
  update();

  t += speedFactor;

  if (withSparks) {
    for (uint8_t i = 0; i < sparks.size(); i++) {
      sparks.get(i)->addXY((float)random(-1, 2) / 2, 0.5 * speedfactor);
      if (sparks.get(i)->getY() > HEIGHT && !random(0, 50))
        sparks.get(i)->reset(*&_ledarray);
      else
        sparks.get(i)->draw();
    }    
  }

  for (byte x = 0; x < WIDTH; x++) {
    for (byte y = 0; y < HEIGHT; y++) {
     
      int16_t bri= inoise8(x * _scale, (y * _scale) - t) - ((withSparks ? y + spacer : y) * (256 / WIDTH));
      byte col = bri;
      if(bri<0){bri= 0;} if(bri!=0) {bri= 256 - (bri* 0.2);}
      uint8_t globalBri = LAMPPTRGET()->get_globalBrightness(0, brightness);
      if (bri > globalBri) bri = globalBri;     
      nblend(EffectMath::getPixel(x, y), ColorFromPalette(_cTargetPalette, col, bri), speedFactor);}
  }
  return true;
}




/*
  Source 
    https://github.com/DmytroKorniienko/FireLamp_EmbUI/blob/master/src/effects.cpp 

  _effeNamIDList "constleds.cpp"
    MEffectPuzzles 
    ALMLPT_EFF_MEffectPuzzles
    E_FX_PUZZLE
  workerset "eff_w.h"
    effect
      48

  eff
    1   - speed
    2   - scale   
    3   -         DFTINTF_10D
    7   -         DFTINTF_131 
    11  -         DFTINTF_120 
    12  -         DFTINTF_121 
*/
// ----------- Эффект "Пятнашки"
// https://editor.soulmatelights.com/gallery/1471-puzzles-subpixel
// (c) Stepko 10.12.21
String MEffectPuzzles::setDynCtrl(UIControl*_val) {
  set_dynCtrl(_val);
  if(_val->getId()==1) speedFactor = EffectMath::fmap(EffectCalc::setDynCtrl(_val).toInt(), 1, 255, 0.05, 0.5);
  else if(_val->getId()==3) {
    PSizeX = EffectCalc::setDynCtrl(_val).toInt();
    PSizeY = PSizeX;
    regen();
  }
  else if(_val->getId()==7) {
    if (!EffectCalc::setDynCtrl(_val).toInt()) {
      _defaultPal= OET_PAL;
    }else {
      _defaultPal= OET_PALGRAD;
    }
  }  
  else EffectCalc::setDynCtrl(_val).toInt(); // для всех других не перечисленных контролов просто дергаем функцию базового класса (если это контролы палитр, микрофона и т.д.)
  return String();
}

void MEffectPuzzles::load(uint8_t strip_pos) {
  
  speedFactor = EffectMath::fmap(speed, 1, 255, 0.05, 0.5);
  // regen();

}

void MEffectPuzzles::regen() {
  Ecols = (WIDTH / PSizeX);
  Erows = (HEIGHT / PSizeY);
  Ca = (WIDTH % PSizeX)? 1 : 0;
  Ra = (HEIGHT % PSizeY)? 1 : 0;
  PCols = round(Ecols) + Ca;
  PRows = round(Erows) + Ra;
  step = 0;
  puzzle.resize(PCols, std::vector<uint8_t>(PRows, 0));

  byte n = 0;
  for (byte x = 0; x < PCols; x++) {
    for (byte y = 0; y < PRows; y++) { 
      n++;
      puzzle[x][y] = (255/ (PCols*PRows)) * n; 
    }
  }
  z_dot[0] = random(0, PCols);
  z_dot[1] = random(0, PRows);

}

void MEffectPuzzles::draw_square(byte x1, byte y1, byte x2, byte y2, byte col) {
  for (byte x = x1; x < x2; x++) {
    for (byte y = y1; y < y2; y++) {
      if (col == 0) { EffectMath::getPixel(x, y) = CRGB(0, 0, 0); } 
      else if ((x == x1 || x == x2 - 1) || (y == y1 || y == y2 - 1))
        EffectMath::getPixel(x, y) = ColorFromPalette(_cTargetPalette, col, LAMPPTRGET()->get_globalBrightness(0, brightness));
      else EffectMath::getPixel(x, y) = CHSV(0, 0, 96);
    }
  }
}

void MEffectPuzzles::draw_squareF(float x1, float y1, float x2, float y2, byte col) {
  for (float x = x1; x < x2; x++) {
    for (float y = y1; y < y2; y++) {
      if ((x == x1 || x == x2 - 1) || (y == y1 || y == y2 - 1))
        EffectMath::drawPixelXYF(x, y, ColorFromPalette(_cTargetPalette, col, LAMPPTRGET()->get_globalBrightness(0, brightness)));
      else EffectMath::drawPixelXYF(x, y, CHSV(0, 0, 96));
    }
  }
}

bool MEffectPuzzles::run() { 
  for (byte x = 0; x < PCols; x++) {
    for (byte y = 0; y < PRows; y++) {
      draw_square(x * PSizeX, y * PSizeY, (x + 1) * PSizeX, (y + 1) * PSizeY, puzzle[x][y]);
    }
  }
  switch (step) {
    case 0:
      XorY = !XorY;
      if (XorY) {
        if (z_dot[0] == PCols - 1)
          move[0] = -1;
        else if (z_dot[0] == 0) move[0] = 1;
        else move[0] = (move[0] == 0) ? (random8() % 2) * 2 - 1 : move[0];
      } else {
        if (z_dot[1] == PRows - 1)
          move[1] = -1;
        else if (z_dot[1] == 0) move[1] = 1;
        else move[1] = (move[1] == 0) ? (random8() % 2) * 2 - 1 : move[1];
      }
      move[(XorY) ? 1 : 0] = 0;
      step = 1;
      break;
    case 1:
     color = puzzle[z_dot[0] + move[0]][z_dot[1] + move[1]];
      puzzle[z_dot[0] + move[0]][z_dot[1] + move[1]] = 0;
      step = 2;
      break;
    case 2:
      draw_squareF(((z_dot[0] + move[0]) * PSizeX) + shift[0], ((z_dot[1] + move[1]) * PSizeY) + shift[1], ((z_dot[0] + move[0] + 1) * PSizeX) + shift[0], (z_dot[1] + move[1] + 1) * PSizeY + shift[1], color);
      shift[0] -= (move[0] * speedFactor);
      shift[1] -= (move[1] * speedFactor);
      if ((fabs(shift[0]) >= WIDTH / PCols) || (fabs(shift[1]) >= HEIGHT / PRows)) {
        shift[0] = 0;
        shift[1] = 0;
        puzzle[z_dot[0]][z_dot[1]] = color;
        step = 3;
      }
      break;
    case 3:
      z_dot[0] += move[0];
      z_dot[1] += move[1];
      step = 0;
      break;
  }
  return true;
}


/*
  Source 
    https://github.com/DmytroKorniienko/FireLamp_EmbUI/blob/master/src/effects.cpp 

  _effeNamIDList "constleds.cpp"
    MEffectPile 
    ALMLPT_EFF_MEffectPile
    
  workerset "eff_w.h"
    effect
      47

  eff
    1   - speed
    2   - scale   
    7   -         DFTINTF_131 
    11  -         DFTINTF_120 
    12  -         DFTINTF_121 
*/
// ============= Ефект Кольорові драже ===============
// (c)stepko    
void MEffectPile::load(uint8_t strip_pos) {
  
  _scale = scale;
}                                                             
String MEffectPile::setDynCtrl(UIControl*_val) {
  set_dynCtrl(_val);
  if(_val->getId()==1) speed = map(EffectCalc::setDynCtrl(_val).toInt(), 1, 255, 20, 200);
  else if(_val->getId()==2) _scale = EffectCalc::setDynCtrl(_val).toInt();
  else if(_val->getId()==7) {
    if (!EffectCalc::setDynCtrl(_val).toInt()) {
      _defaultPal= OET_PAL;
    }else {
      _defaultPal= OET_PALGRAD;
    }
  }  
  else EffectCalc::setDynCtrl(_val).toInt(); 
  return String();
}

void MEffectPile::changeFrame() {
  memcpy(F[0], F[1], WIDTH * HEIGHT);
         
                                                          
    uint8_t temp = map8(random(256), _scale, 255U);
  if (pcnt >= map8(temp, 2U, HEIGHT - 3U)) {
               
                 
    temp = HEIGHT + 1U - pcnt;
    if (!random8(4U)) {
      if (random8(2U))
        temp = 2U;
      else
        temp = 3U;      
    }
    for (byte y = 0; y < pcnt; y++) {
    for (byte x = 0; x < WIDTH; x++) {
      if(random(255)%3) F[1][x][y] = 0;
    }
  }
}
pcnt = 0;
  for (byte y = 0; y < HEIGHT; y++) {
    for (byte x = 0; x < WIDTH; x++) {
      if (!F[1][x][y]) continue;
      if (F[1][x][y] && !F[1][x][y - 1] && y) {F[1][x][y - 1] = F[1][x][y]; F[1][x][y] = 0; }
      else if (F[1][x][y] && F[1][x][y - 1] && !F[1][x + 1][y - 1]  && !(x==WIDTH-1) && y) {F[1][x + 1][y - 1] = F[1][x][y]; F[1][x][y] = 0; }
       else if (F[1][x][y] && F[1][x][y - 1] && !F[1][x - 1][y - 1] && x && y) {F[1][x - 1][y - 1] = F[1][x][y]; F[1][x][y] = 0; }
        else if (pcnt < y) pcnt = y;
    }
  }
  if (!(random(255) % 6)) F[1][WIDTH/2 - (random(-2, WIDTH%2 ? 3 : 4))][HEIGHT - 1] = random(16, 255);
}

bool MEffectPile::run() {
  update();

  shift += speed;
  if (shift >= 255) {
    changeFrame();
    shift = 0;
  }
  for (byte x = 0; x < WIDTH; x++) {
    for (byte y = 0; y < HEIGHT; y++) {
      CRGB col1 = ColorFromPalette(_cTargetPalette, F[0][x][y], (F[0][x][y]) ? LAMPPTRGET()->get_globalBrightness(0, brightness) : 0);
      EffectMath::drawPixelXY(x, y, nblend(col1, ColorFromPalette(_cTargetPalette, F[1][x][y],(F[1][x][y]) ? LAMPPTRGET()->get_globalBrightness(0, brightness) : 0), shift));
    }
  }
  return true;
}




/*
  Source 
    https://github.com/DmytroKorniienko/FireLamp_EmbUI/blob/master/src/effects.cpp 

  _effeNamIDList "constleds.cpp"
    MEffectDNA 
    ALMLPT_EFF_MEffectDNA
    E_FX_DNA
  workerset "eff_w.h"
    effect
      46

  eff
    1   - speed
    2   - scale   DFTINTF_0D2 Effet (0: cyclique)
    3   -         DFTINTF_0DC Balayage
    5   -         DFTINTF_0F9 comportement 1/2
*/
// ============= Эффект ДНК ===============
// (c) Stepko
// https://editor.soulmatelights.com/gallery/1520-dna
//по мотивам визуала эффекта by Yaroslaw Turbin
//https://vk.com/ldirko программный код которого он запретил брать
// !++
void MEffectDNA::load(uint8_t strip_pos) {
  
  speedFactor = EffectMath::fmap(speed, 1, 255, 0.5, 5) * speedfactor; 
  _scale = scale;
}
String MEffectDNA::setDynCtrl(UIControl*_val) {
  set_dynCtrl(_val);
  if(_val->getId()==1) speedFactor  = EffectMath::fmap(EffectCalc::setDynCtrl(_val).toInt(), 1, 255, 0.5, 5) * speedfactor;
  else if(_val->getId()==3)  type = EffectCalc::setDynCtrl(_val).toInt();
  else if(_val->getId()==2)  _scale = EffectCalc::setDynCtrl(_val).toInt();
  else if(_val->getId()==5) bals = EffectCalc::setDynCtrl(_val).toInt();
  else EffectCalc::setDynCtrl(_val).toInt(); // для всех других не перечисленных контролов просто дергаем функцию базового класса (если это контролы палитр, микрофона и т.д.)
  return String();
}

bool MEffectDNA::run() {

  fadeToBlackBy(_ledarray, NUM_LEDS, 32);
  t += speedFactor; // (float)millis()/10;
  if (type == 0) {
    EVERY_N_SECONDS(30) {
      _type ++;
      if (_type > 3)
        _type = 1;
    }
  } else _type = type;

  if (_type == 1 or _type == 4) a = (256.0 / (float)WIDTH);
  else a = (256.0 / (float)HEIGHT);

  for (byte i = 0; i < ((_type == 1) ? HEIGHT : WIDTH); i++) {
    uint16_t shift = (i * _scale);
    float sin1 = (1.0 + sin(radians(t + shift))) * 128.0;
    byte brightFront =  constrain(112 * (1 + sin(radians(t + shift + 90))) + 30, 96, LAMPPTRGET()->get_globalBrightness(0, brightness)); 
    byte brightBack =  constrain(112 * (1 + sin(radians(t + shift + 270))) + 30, 96, LAMPPTRGET()->get_globalBrightness(0, brightness));

    float x = 0, y = 0, x1 = 0, y1 = 0;
    uint8_t width_height;

    switch (_type)
    {
    case 1: // Вертикальная ДНК
      width_height = WIDTH;
      x = sin1 /a;
      y = i;
      x1 = (float)(width_height - 1) - (sin1 / a);
      y1 = i;
      break;
    case 2: // Вертикально-горизонтальная
      width_height = HEIGHT;
      x = sin1 /a;
      y = i;
      y1 = (float)(width_height - 1) - (sin1 / a);
      x1 = i;
      break;
    case 3: // Горизонтальная ДНК
      width_height = HEIGHT;
      y = sin1 /a;
      x = i;
      y1 = (float)(width_height - 1) - (sin1 / a);
      x1 = i;
      break;
    
    default:
      break;
    }

    if (flag or !bals) {
      EffectMath::drawPixelXYF(x, y, CHSV(sin1, 255, brightFront));
    }
    if (!flag or !bals)
      EffectMath::drawPixelXYF(x1, y1, CHSV(~(byte)sin1, 255, brightBack));
    flag = !flag; 
  }
  blur2d(_ledarray, WIDTH, HEIGHT, 64);

  return true;
}



/*
  Source 
    https://github.com/DmytroKorniienko/FireLamp_EmbUI/blob/master/src/effects.cpp 

  _effeNamIDList "constleds.cpp"
    MEffectSmoker 
    ALMLPT_EFF_MEffectSmoker
    E_FX_SMOKER
  workerset "eff_w.h"
    effect
      45

  eff
    1   - speed
    2   - scale   DFTINTF_11A Couleur (1: cyclique)  
*/
// ----------- Эффект "Дым"
// based on code by @Stepko (c) 23/12/2021
// !++
void MEffectSmoker::load(uint8_t strip_pos) {
  
  speedFactor = EffectMath::fmap(speed, 1, 255, 3, 20) * speedfactor; 
  saturation = _fade;
}
String MEffectSmoker::setDynCtrl(UIControl*_val) {
  set_dynCtrl(_val);
  if(_val->getId()==1) speedFactor = EffectMath::fmap(EffectCalc::setDynCtrl(_val).toInt(), 1, 255, 3, 20) * speedfactor;
  else if(_val->getId()==3) {saturation = EffectCalc::setDynCtrl(_val).toInt();}
  else if (_val->getId()==4) { _hueTimer = EffectCalc::setDynCtrl(_val).toInt(); LAMPPTRGET()->effectsLoop_TimerHue(_hueTimer); }
  else if (_val->getId()==8) {
    _hue_1 = EffectCalc::setDynCtrl(_val).toInt(); 
    _hue_1V = _hue_1;
  }
  else if(_val->getId()==6) {
    _cMod = EffectCalc::setDynCtrl(_val).toInt();
    if (!_cMod) {
      LAMPPTRGET()->_hsvLoop_1.set_hueRnd(2);
    } else LAMPPTRGET()->_hsvLoop_1.set_hueRnd(1);
  }
  else EffectCalc::setDynCtrl(_val).toInt();

  return String();
}

bool MEffectSmoker::run() {
  update();
  t += speedFactor;
  for (byte x = 0; x < WIDTH; x++) {
    for (byte y = 0; y < HEIGHT; y++) { 
      uint8_t bri= inoise8(x * beatsin8(glitch, 20, 40), (y * scale) - t);
      bri = map(bri, 0, 255, bri, LAMPPTRGET()->get_globalBrightness(0, brightness));
      nblend(EffectMath::getPixel(x, y), CHSV(_hue_1V, saturation, bri), speedFactor);}
  }
  
  EVERY_N_SECONDS(random8(10, 31)) {
    glitch = random(1, 3);
  }
  return true;
}


/*
  Source 
    https://github.com/DmytroKorniienko/FireLamp_EmbUI/blob/master/src/effects.cpp 

  _effeNamIDList "constleds.cpp"
    MEffectMirage 
    ALMLPT_EFF_MEffectMirage
    E_FX_MIRAGE
  workerset "eff_w.h"
    effect
      44

  eff
    1   - speed
    2   - scale   DFTINTF_11A Couleur (1: cyclique)  
*/
// ----------- Эффект "Мираж"
// based on code by @Stepko (c) 23/12/2021

// !++
void MEffectMirage::load(uint8_t strip_pos) {
  
  _speed = map(speed, 1, 255, 64, 512) * speedfactor;
  color = scale;
  if (color == 0) colorShift = true;
  else colorShift = false;  
}
String MEffectMirage::setDynCtrl(UIControl*_val) {
  set_dynCtrl(_val);
  if(_val->getId()==1) _speed = map(EffectCalc::setDynCtrl(_val).toInt(), 1, 255, 64, 512) * speedfactor;
  else if(_val->getId()==2) {
    color = EffectCalc::setDynCtrl(_val).toInt();
    if (color == 0) colorShift = true;
    else colorShift = false;
  }
  else EffectCalc::setDynCtrl(_val).toInt(); // для всех других не перечисленных контролов просто дергаем функцию базового класса (если это контролы палитр, микрофона и т.д.)
  return String();
}

void MEffectMirage::drawDot(float x, float y, byte a){
uint8_t xx = (x - (int) x) * 255, yy = (y - (int) y) * 255, ix = 255 - xx, iy = 255 - yy;
#define WU(a, b)((uint8_t)(((a) * (b) + (a) + (b)) >> 8))
  uint8_t wu[4] = {
    WU(ix, iy),
    WU(xx, iy),
    WU(ix, yy),
    WU(xx, yy)
  };
  // multiply the intensities by the colour, and saturating-add them to the pixels
  for (uint8_t i = 0; i < 4; i++) {
    int16_t xn = x + (i & 1), yn = y + ((i >> 1) & 1);
    byte clr = buff[xn][yn];
    clr = constrain(qadd8(clr, (a * wu[i]) >> 8), 0, 240);
    buff[xn][yn] = clr;
  }
}

void MEffectMirage::blur() {
  uint16_t sum;
  for (byte x = 1; x < WIDTH + 1; x++) {
    for (byte y = 1; y < HEIGHT + 1; y++) {
      sum = buff[x][y];
      sum += buff[x + 1][y];
      sum += buff[x][y - 1];
      sum += buff[x][y + 1];
      sum += buff[x - 1][y];
      sum /= 5;
      buff[x][y] = sum;
    }
  }
}

bool MEffectMirage::run() {
  blur();
  float x1 = (float)beatsin88(15UL * _speed, div, width) / div;
  float y1 = (float)beatsin88(20UL * _speed, div, height) / div;
  float x2 = (float)beatsin88(16UL * _speed, div, width) / div;
  float y2 = (float)beatsin88(14UL * _speed, div, height) / div;
  float x3 = (float)beatsin88(12UL * _speed, div, width) / div;
  float y3 = (float)beatsin88(16UL * _speed, div, height) / div;
  drawDot(x1, y1, 200);
  drawDot(x1 + 1, y1, 200);
  drawDot(x2, y2, 200);
  drawDot(x2 + 1, y2, 200);
  drawDot(x3, y3, 200);
  drawDot(x3 + 1, y3, 200);
  for (byte x = 1; x < WIDTH + 1; x++) {
    for (byte y = 1; y < HEIGHT + 1; y++) {
      EffectMath::getPixel(x - 1, y - 1) = CHSV(colorShift ? color++ : color, buff[x][y], LAMPPTRGET()->get_globalBrightness(0, brightness));
    }
  }
  return true;
}


/*
  Source 
    https://github.com/DmytroKorniienko/FireLamp_EmbUI/blob/master/src/effects.cpp

  _effeNamIDList "constleds.cpp"
    MEffectWcolor 
    ALMLPT_EFF_MEffectWcolor 
    E_FX_WCOLOR
  workerset "eff_w.h"
    effect
      43

  eff
    1   - speed
    2   - scale   DFTINTF_0EF Densité
    5   -         DFTINTF_0F9 Comportement: 1/2
*/
// -------------------- Эффект "Акварель"
// (c) kostyamat 26.12.2021
// https://editor.soulmatelights.com/gallery/1587-oil
// !++
String MEffectWcolor::setDynCtrl(UIControl*_val){
  set_dynCtrl(_val);
  if(_val->getId()==1) {
    speedFactor = EffectMath::fmap(EffectCalc::setDynCtrl(_val).toInt(), 1, 255, 0.1, 0.5);
    blur = 64.f * speedFactor;
    speedFactor *= EffectCalc::speedfactor;
  }  else if(_val->getId()==2) {
    bCounts = map(EffectCalc::setDynCtrl(_val).toInt(), 1, 8, HEIGHT/4, HEIGHT);
    blots.resize(bCounts);
    for (byte i = 0; i < bCounts; i++) {
      blots[i].reset(i, bCounts);
    }
  }
  else if(_val->getId()==5) mode = EffectCalc::setDynCtrl(_val).toInt();
  else EffectCalc::setDynCtrl(_val).toInt(); // для всех других не перечисленных контролов просто дергаем функцию базового класса (если это контролы палитр, микрофона и т.д.)
  return String();
}

void MEffectWcolor::load(uint8_t strip_pos) {
  

  bCounts = map(scale, 1, 8, HEIGHT/4, HEIGHT);
  blots.resize(bCounts);

  speedFactor = EffectMath::fmap(speed, 1, 255, 0.1, 0.5);
  blur = 64.f * speedFactor;
  speedFactor *= EffectCalc::speedfactor;

  for (byte i = 0; i < bCounts; i++) {
    blots[i].reset(i, bCounts);
  }
}

bool MEffectWcolor::run() {
  if (dryrun(3)) return false;

  fadeToBlackBy(_ledarray, NUM_LEDS, blur);
  for (byte i = 0; i < bCounts; i++) {
    blots[i].drawing();
    blots[i].appendXY( mode ? ((float)inoise8(t+= speedFactor, 0, i * 100) / 256) - 0.5f : 0, -speedFactor);
    if(blots[i].getY() < -0.1) {
      blots[i].reset(i, bCounts);
      random16_set_seed(millis());
    }
  }
  EffectMath::blur2d(_ledarray, WIDTH, HEIGHT, 32); 
  return true;
}



/*
  Source 
    https://github.com/DmytroKorniienko/FireLamp_EmbUI/blob/master/src/effects.cpp
   * Aurora: https://github.com/pixelmatix/aurora
   * https://github.com/pixelmatix/aurora/blob/sm3.0-64x64/PatternSpiro.h
   * Copyright (c) 2014 Jason Coon
   * Неполная адаптация SottNick

  _effeNamIDList "constleds.cpp"
    MEffectSpiro 
    ALMLPT_EFF_MEffectSpiro Spiral
    E_FX_SPIRO
  workerset "eff_w.h"
    effect
      38

  eff
    1   - speed
    2   - scale   
    11  -         DFTINTF_120 Palette (0: Cyle
    12  -         DFTINTF_121 Cyle de rotation des Palettes
*/
void MEffectSpiro::load(uint8_t strip_pos){
  
  speedFactor = EffectMath::fmap(speed, 1., 255., 0.75, 3);
  EffectMath::dimAll(255);
}

//!++
String MEffectSpiro::setDynCtrl(UIControl*_val) {
  set_dynCtrl(_val);
  if(_val->getId()==1) speedFactor = EffectMath::fmap(EffectCalc::setDynCtrl(_val).toInt(), 1., 255., 0.75, 3);
  else if(_val->getId()==7) {
    if (!EffectCalc::setDynCtrl(_val).toInt()) {
      _defaultPal= OET_PAL;
    }else {
      _defaultPal= OET_PALGRAD;
    }
  }
  else EffectCalc::setDynCtrl(_val).toInt(); // для всех других не перечисленных контролов просто дергаем функцию базового класса (если это контролы палитр, микрофона и т.д.)
  return String();
}

bool MEffectSpiro::run() {
  // страхуемся от креша

  if (dryrun(3)) return false;
  update();

  uint8_t dim = beatsin8(16. / speedFactor, 5, 10);
    fadeToBlackBy(_ledarray, NUM_LEDS, dim);
  static float t;t +=  speedFactor * 0.05f;
  float CalcRad = (sin(t / 2) + 1);
  if (CalcRad <= 0.001) {
    if(!incenter){
    if(AM<=1 || AM >= ((WIDTH + HEIGHT) / 2)) change = !change;
          if (change) {
            if(AM >= 4)
              AM *= 2;
            else
              AM += 1;
          }
          else {
            if(AM > 4)
              AM /= 2;
            else
              AM -= 1;
          }
      Angle = 6.28318531 / AM;
  }
    incenter = 1;
  } else incenter = 0;
  float radX = CalcRad * spirocenterX / 2;
  float radY = CalcRad * spirocenterY / 2;
  for (byte i = 0; i < AM; i++) {
    EffectMath::drawPixelXYF((spirocenterX + sin(t + (Angle * i)) * radX), (spirocenterY + cos(t + (Angle * i)) * radY), ColorFromPalette(_cTargetPalette, t*10 + ((256 / AM) * i), LAMPPTRGET()->get_globalBrightness(0, brightness)));
  }
  EffectMath::blur2d(32);
  return true;
}
/*
void MEffectSpiro::load(uint8_t strip_pos){
  
}
String MEffectSpiro::setDynCtrl(UIControl*_val) {
  set_dynCtrl(_val);
  return String();
}
bool MEffectSpiro::run(){
  update();
  return spiroRoutine(*&_ledarray);
}
bool MEffectSpiro::spiroRoutine(CRGB *leds)
{

  // страхуемся от креша
  // if (curPalette == nullptr) {
  //   return false;
  // }

  bool change = false;
  float speed_factor = (float)speed/127.0 + 1;
  uint8_t spirooffset = 256 / spirocount;

  //EffectMath::dimAll(254U - palettescale);
  //EffectMath::dimAll(250-speed_factor*7);
  uint8_t dim = beatsin8(16 / speed_factor, 5, 10);
  EffectMath::dimAll(254 - dim);

  for (int i = 0; i < spirocount; i++) {
    uint8_t  x = EffectMath::mapsincos8(MAP_SIN, spirotheta1 + i * spirooffset, spirominx, spiromaxx);
    uint8_t  y = EffectMath::mapsincos8(MAP_COS, spirotheta1 + i * spirooffset, spirominy, spiromaxy);
    uint8_t x2 = EffectMath::mapsincos8(MAP_SIN, spirotheta2 + i * spirooffset, x - spiroradiusx, x + spiroradiusx);
    uint8_t y2 = EffectMath::mapsincos8(MAP_COS, spirotheta2 + i * spirooffset, y - spiroradiusy, y + spiroradiusy);
    CRGB color = ColorFromPalette(_cTargetPalette, (spirohueoffset + i * spirooffset), 128U);
    uint32_t pixelPos = getPixelNumber(x2, y2);
    if (pixelPos < NUM_LEDS){
      CRGB tmpColor = EffectMath::getPixColorXY(x2, y2);
      tmpColor += color;

      setLed(pixelPos, tmpColor);
    }

    if(x2 == spirocenterX && y2 == spirocenterY) change = true; // в центре могут находится некоторое время
  }

  spirotheta2 += 2.*speed_factor;
  spirotheta1 += 1.*speed_factor;
  spirohueoffset += 1.*speed_factor;

  if (change && !spirohandledChange) { // меняем кол-во спиралей
    spirohandledChange = true;

    if (spirocount >= WIDTH || spirocount == 1)
      spiroincrement = !spiroincrement;

    if (spiroincrement) {
      if(spirocount >= 4)
        spirocount *= 2;
      else
        spirocount += 1;
    } else {
      if(spirocount > 4)
        spirocount /= 2;
      else
          spirocount -= 1;
    }

    spirooffset = 256 / spirocount;
  }

  // сброс спустя время, чтобы счетчик спиралей не менялся скачками
  if(spirohandledChange){
    if(internalCnt==25) { // спустя 25 кадров
      spirohandledChange = false;
      internalCnt=0;
    } else {
      internalCnt++;
    }
  }

  EffectMath::blur2d(25);
  return true;
}
*/

/*
  Source 
    https://github.com/DmytroKorniienko/FireLamp_EmbUI/blob/master/src/effects.cpp
    RadialFire
    (c) Stepko and Sutaburosu https://editor.soulmatelights.com/gallery/1570-radialfire
    23/12/21
    !++

  _effeNamIDList "constleds.cpp"
    MEffectRadialFire 
    ALMLPT_EFF_MEffectRadialFire Matrix Fire
    E_FX_NFIRE
  workerset "eff_w.h"
    effect
      41

  eff
    1   - speed
    2   - scale   DFTINTF_088 Echelle 
    5   -         DFTINTF_0F9 Comportement: 1/2 
    11  -         DFTINTF_120 Palette (0: Cyle
    12  -         DFTINTF_121 Cyle de rotation des Palettes
*/
String MEffectRadialFire::setDynCtrl(UIControl*_val){
  set_dynCtrl(_val);
  if(_val->getId()==1) {
    speed = EffectCalc::setDynCtrl(_val).toInt();
    speedFactor = EffectMath::fmap(speed, 1, 255, 2., 20.);
  } else if (_val->getId()==5) mode = EffectCalc::setDynCtrl(_val).toInt();
  return String();
}

void MEffectRadialFire::load(uint8_t strip_pos) {
  
  speedFactor = EffectMath::fmap(speed, 1, 255, 2., 20.);
  XY_angle.resize(MIN_MAX, std::vector<int>(MIN_MAX, 0));
  XY_radius.resize(MIN_MAX, std::vector<byte>(MIN_MAX, 0));
  for (int8_t x = -C_X; x < C_X + (int8_t)(WIDTH % 2); x++) {
    for (int8_t y = -C_Y; y < C_Y + (int8_t)(HEIGHT % 2); y++) {
      XY_angle[x + C_X][y + C_Y] = atan2(y, x) * (180. / 2. / PI) * MIN_MAX;
      XY_radius[x + C_X][y + C_Y] = hypotf(x, y); // thanks Sutaburosu
    }
  }
  // palettesload();

}

// void MEffectRadialFire::palettesload(){
//   // собираем свой набор палитр для эффекта
//   palettes.reserve(NUMPALETTES);
//   palettes.push_back(&NormalFire_p);
//   palettes.push_back(&LithiumFireColors_p);
//   palettes.push_back(&NormalFire2_p);
//   palettes.push_back(&WoodFireColors_p);
//   palettes.push_back(&NormalFire3_p);
//   palettes.push_back(&CopperFireColors_p);
//   palettes.push_back(&HeatColors_p);
//   palettes.push_back(&PotassiumFireColors_p);
//   palettes.push_back(&MagmaColor_p);
//   palettes.push_back(&RubidiumFireColors_p);
//   palettes.push_back(&AlcoholFireColors_p); 
//   palettes.push_back(&WaterfallColors_p);

//   usepalettes = true; // включаем флаг палитр
//   scale2pallete();    // выставляем текущую палитру
// }

bool MEffectRadialFire::run() {
  update();

  t += speedFactor;
  for (uint8_t x = 0; x < WIDTH; x++) {
    for (uint8_t y = 0; y < HEIGHT; y++) {
      int angle = XY_angle[x][y];
      byte radius = mode ? MIN_MAX - 3 - XY_radius[x][y] : XY_radius[x][y];
      int16_t Bri = inoise8(angle, radius * scale - t, x * scale) - radius * (256 /MIN_MAX);
      byte Col = Bri;
      if (Bri < 0) Bri = 0; 
      if(Bri != 0) Bri = 256 - (Bri * 0.2);
      uint8_t globalBri = LAMPPTRGET()->get_globalBrightness(0, brightness);
      if (Bri > globalBri) Bri = globalBri;      
      nblend(EffectMath::getPixel(x, y), ColorFromPalette(_cTargetPalette, Col, Bri), speed);
    }
  }
  return true;
}


/*
  Source 
    https://github.com/DmytroKorniienko/FireLamp_EmbUI/blob/master/src/effects.cpp

  _effeNamIDList "constleds.cpp"
    MEffectSplashBals 
    ALMLPT_EFF_MEffectMetaBalls Matrix Splash Bals
    E_FX_SPLASHBALS
  workerset "eff_w.h"
    effect
      40

  eff
    1   - speed
    2   - scale
    6   - _size   DFTINTF_0D5 Quantité
    11  -         DFTINTF_120 Palette (0: Cyle
    12  -         DFTINTF_121 Cyle de rotation des Palettes
*/
String MEffectSplashBals::setDynCtrl(UIControl*_val){
  set_dynCtrl(_val);
  if (_val->getId()==1) speedFactor = EffectMath::fmap(speed, 1, 255, 1, 3) * speedfactor;
  return String();
}

void MEffectSplashBals::loadDefault() {_timerPalFactor = 1000;}
void MEffectSplashBals::load(uint8_t strip_pos) {
  
  speedFactor = EffectMath::fmap(speed, 1, 255, 1, 3) * speedfactor;
  for (uint8_t i = 0; i < 6; i++) {
    iniX[i] = random(0, 8);
    iniY[i] = random(1, 9);
  }
}

bool MEffectSplashBals::run() {
  if (dryrun(1.7)) return false;
  update();

  fadeToBlackBy(_ledarray, NUM_LEDS, 100);
  hue++;

  for (byte i = 0; i < _size; i++) {
    x[i] = (float)beatsin88(((10UL + iniX[i]) * 256) * speedFactor, 0, (WIDTH - 1) * DEV) / DEV;
    y[i] = (float)beatsin88(((10UL + iniY[i]) * 256) * speedFactor, 0, (HEIGHT - 1) * DEV) / DEV;
    for (byte j = i; j < _size; j++) {
      byte a = dist(x[i], y[i], x[j], y[j]);
      if ((i != j) & (a <= float(min(WIDTH, HEIGHT) / 2))) {
        EffectMath::drawLineF(x[i], y[i], x[j], y[j], CHSV(0, 0, EffectMath::fmap(a, min(WIDTH, HEIGHT), 0, 48, LAMPPTRGET()->get_globalBrightness(0, brightness) )));
      }
    }
    EffectMath::fill_circleF(x[i], y[i], EffectMath::fmap(fabs(float(WIDTH / 2) - x[i]), 0, WIDTH / 2, R, 0.2), ColorFromPalette(_cTargetPalette, 256 - 256/HEIGHT * fabs(float(HEIGHT/2) - y[i]), LAMPPTRGET()->get_globalBrightness(0, brightness)));
  }
  EffectMath::blur2d(_ledarray, WIDTH, HEIGHT, 48);
  return true;
}


/*
  Source 
    https://github.com/DmytroKorniienko/FireLamp_EmbUI/blob/master/src/effects.cpp

  _effeNamIDList "constleds.cpp"
    MEffectFlower 
    ALMLPT_EFF_MEffectFlower Matrix Flowering
    E_FX_Wave
  workerset "eff_w.h"
    effect
      39

  eff
    1   - speed
*/
void MEffectFlower::load(uint8_t strip_pos){
  
}
String MEffectFlower::setDynCtrl(UIControl*_val) {
  set_dynCtrl(_val);
  return String();
}
int16_t MEffectFlower::ZVcalcRadius(int16_t x, int16_t y) {
  x *= x;
  y *= y;
  int16_t radi = sin8(x + y);
  return radi;
}

int16_t MEffectFlower::ZVcalcDist(uint8_t x, uint8_t y, float center_x, float center_y) {
  int16_t a = (center_y - y - .5);
  int16_t b = (center_x - x - .5);
  int16_t dista = ZVcalcRadius(a, b);
  return dista;
}

bool MEffectFlower::run() {

  effTimer = (1+sin(radians((float)millis()/6000)))*12.5;
  ZVoffset += EffectMath::fmap((float)speed, 1, 255, 0.2, 6.0);;
  
  for (uint8_t x = 0; x < WIDTH; x++) {
    for (uint8_t y = 0; y < HEIGHT; y++) {
      int dista = ZVcalcDist(x, y, COLS_HALF, ROWS_HALF);
      
      // exclude outside of circle
      int bri = 1;
      if (dista += max(COLS_HALF,ROWS_HALF)) {
        bri = map(dista, -effTimer,max(COLS_HALF,ROWS_HALF), 255, 110);
        bri += ZVoffset;
        bri = sin8(bri);
      }

      int hue = map(dista, max(COLS_HALF,ROWS_HALF),-3,  125, 255);
      uint8_t globalBri = LAMPPTRGET()->get_globalBrightness(0, brightness);
      if (bri > globalBri) bri = globalBri;
      EffectMath::drawPixelXY(x, y, CHSV(hue+ZVoffset/4, 255, bri )  );
      EffectMath::drawPixelXY(x, y, CHSV(hue+ZVoffset/4, 255, bri )  );
    }
  } 
  return true;
}


/*
  _effeNamIDList "constleds.cpp"
    MEffectRadialNuclearNoise 
    ALMLPT_EFF_MEffectRadialNuclearNoise Matrix RadialNuclearNoise
    E_FX_ScalSpeed  
    E_FX_BSS
  workerset "eff_w.h"
    effect
      42

  eff
    1 - speed
    2 - scale
*/
//RadialNuclearNoise
//Stepko and Sutaburosu
//15/03/23
struct{
  uint8_t angle;
  uint8_t radius;
} rMap[NUM_LEDS];
void MEffectRadialNuclearNoise::load(uint8_t strip_pos){
  
  speedFactor = EffectMath::fmap(speed, 1, 255, 1, 6) * speedfactor;  
  for (int8_t x = -C_X; x < C_X + (LED_COLS % 2); x++) {
    for (int8_t y = -C_Y; y < C_Y + (LED_ROWS % 2); y++) {
      uint16_t coord = EffectMath_PRIVATE::getPixelNumber(x + C_X, y + C_Y);
      rMap[coord].angle = 128 * (atan2(y, x) / PI);
      rMap[coord].radius = hypot(x, y) * mapp; //thanks Sutaburosu
    }
  }
}
String MEffectRadialNuclearNoise::setDynCtrl(UIControl*_val) {
  set_dynCtrl(_val);
  if (_val->getId()==1) speedFactor = EffectMath::fmap(speed, 1, 255, 1, 6) * speedfactor;  
  return String();
}

bool MEffectRadialNuclearNoise::run() {
  if (dryrun(3)) return false;

  FastLED.clear();

  
  static uint16_t t;
  t += speedFactor;
  uint16_t t1 = t / scale;
  for (uint8_t x = 0; x < LED_COLS; x++) {
    for (uint8_t y = 0; y < LED_ROWS; y++) {
      uint16_t coord = EffectMath_PRIVATE::getPixelNumber(x,y);
      uint8_t angle = sin8(t1 / 2 + rMap[coord].angle * 3);
      uint8_t radius = (rMap[coord].radius) * 2 - t;
      uint8_t noise[3] = { inoise8(angle, radius, t1), inoise8(angle, 12032 + t1, radius), inoise8(radius, 120021 + t1, angle) };
      for(uint8_t i = 0; i < 3; i++){
        noise[i] = (noise[i] < 128) ? 0 : constrain((noise[i] - 128) * 3,0,255);
      }
      _ledarray[coord] = CRGB(noise[0], noise[1], noise[2]);
    }
  }
  return true;
}
#endif