// #include "effect_config.h"

#include "../../../include/pixel/w2812fx/fx.h"
#include "../../../include/device/device.h"

#ifndef MIN
#define MIN(a,b) ((a)<(b)?(a):(b))
#endif
#ifndef MAX
#define MAX(a,b) ((a)>(b)?(a):(b))
#endif


#define WLED_FPS         42
#define FRAMETIME        (1000/WLED_FPS)

#define PALETTE_SOLID_WRAP (paletteBlend == 1 || paletteBlend == 3)

#define DEFAULT_SPEED      (uint16_t)1000
#define DEFAULT_COLOR      (uint32_t)0xFF0000

#if defined(ESP8266) || defined(ESP32)
  #define SPEED_MIN (uint16_t)2
#else
  #define SPEED_MIN (uint16_t)10
#endif
#define SPEED_MAX (uint16_t)65535

/* each segment uses 36 bytes of SRAM memory, so if you're compile fails
  because of insufficient flash memory, decreasing MAX_NUM_SEGMENTS may help */
#define MAX_NUM_SEGMENTS         16
#define MAX_NUM_ACTIVE_SEGMENTS  16
#define INACTIVE_SEGMENT        255 /* max uint_8 */
#define MAX_CUSTOM_MODES          2

// segment options
// bit    7: reverse animation
// bits 4-6: fade rate (0-7)
// bit    3: gamma correction
// bits 1-2: size
// bits   0: TBD
#define NO_OPTIONS   (uint8_t)B00000000
#define REVERSE      (uint8_t)B10000000
#define IS_REVERSE   ((_seg->options & REVERSE) == REVERSE)
#define FADE_XFAST   (uint8_t)B00010000
#define FADE_FAST    (uint8_t)B00100000
#define FADE_MEDIUM  (uint8_t)B00110000
#define FADE_SLOW    (uint8_t)B01000000
#define FADE_XSLOW   (uint8_t)B01010000
#define FADE_XXSLOW  (uint8_t)B01100000
#define FADE_GLACIAL (uint8_t)B01110000
#define FADE_RATE    ((_seg->options >> 4) & 7)
#define GAMMA        (uint8_t)B00001000
#define IS_GAMMA     ((_seg->options & GAMMA) == GAMMA)
#define SIZE_SMALL   (uint8_t)B00000000
#define SIZE_MEDIUM  (uint8_t)B00000010
#define SIZE_LARGE   (uint8_t)B00000100
#define SIZE_XLARGE  (uint8_t)B00000110
#define SIZE_OPTION  ((_seg->options >> 1) & 3)
#define SEGCOLOR(x)   gamma32(_seg->colors[x])

// segment runtime options (aux_param2)
#define FRAME           (uint8_t)B10000000
#define SET_FRAME       (_seg_rt->aux_param2 |=  FRAME)
#define CLR_FRAME       (_seg_rt->aux_param2 &= ~FRAME)
#define CYCLE           (uint8_t)B01000000
#define SET_CYCLE       (_seg_rt->aux_param2 |=  CYCLE)
#define CLR_CYCLE       (_seg_rt->aux_param2 &= ~CYCLE)
#define CLR_FRAME_CYCLE (_seg_rt->aux_param2 &= ~(FRAME | CYCLE))

// #include "../tools/tools.h"
// #include "set.h"
uint16_t WS2812FX::_usedSegmentData = 0;

#include "fx_set.h"
#include "fx_segment.h"
#include "fx_func.h"



const byte gammaT[] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
    2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
    5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
   10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
   17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
   25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
   37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
   51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
   69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
   90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
  115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
  144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
  177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
  215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };

uint8_t WS2812FX::gamma8(uint8_t b)
{
  return gammaT[b];
}

uint32_t WS2812FX::gamma32(uint32_t color)
{
  if (!gammaCorrectCol) return color;
  uint8_t w = (color >> 24);
  uint8_t r = (color >> 16);
  uint8_t g = (color >>  8);
  uint8_t b =  color;
  w = gammaT[w];
  r = gammaT[r];
  g = gammaT[g];
  b = gammaT[b];
  return ((w << 24) | (r << 16) | (g << 8) | (b));
}


void WS2812FX::print() { 
  Serial.print("numLeds: ");Serial.println(effectSetting->numLeds);
  Serial.print("reverse: ");Serial.println(effectSetting->reverse);
  Serial.print("isTw: ");Serial.println(effectSetting->isTw);
  Serial.print("scale: ");Serial.println(effectSetting->scale);
  Serial.print("size: ");Serial.println(effectSetting->size);
  Serial.print("fade: ");Serial.println(effectSetting->fade);
  Serial.print("speed: ");Serial.println(effectSetting->speed);
}
void WS2812FX::setup(uint16_t n, WS2812FX_setting * setting) { 
  uint8_t maxSeg = 0 ;
  DevicePtrGet()->get_outputArray(n)->_leds_segment.get_max(maxSeg);
  if (maxSeg == 0) maxSeg = 1;

  _effectMod[0] = &WS2812FX::mode_color_wipe;

  effectSetting = setting;

  _reverseStart = effectSetting->reverseStart;

  // Serial.print("isTw: ");Serial.println(effectSetting->isTw);
  // Serial.print("scale: ");Serial.println(effectSetting->scale);
  // Serial.print("size: ");Serial.println(effectSetting->size);
  // Serial.print("fade: ");Serial.println(effectSetting->fade);
  // Serial.print("speed: ");Serial.println(effectSetting->speed);
  
  _stripPos = n;

  // brightness = DEFAULT_BRIGHTNESS + 1; // Adafruit_NeoPixel internally offsets brightness by 1
  // _running = true;

  _segments_len = maxSeg;
  _active_segments_len = maxSeg;

  // create all the segment arrays and init to zeros
  _segments           = new segment[maxSeg]();
  _active_segments    = new uint8_t[maxSeg]();
  _segment_runtimes   = new segment_runtime[maxSeg]();

  // init segment pointers
  _seg     = _segments;
  _seg_rt  = _segment_runtimes;

  resetSegments();
  
  if (DevicePtrGet()->get_outputArray(n)->_leds_segment.isSegment() && effectSetting->segment) { 

    bool reverse = _reverseStart;

    Serial.printf_P(PSTR("\nsegment initialize - reverse: %d\n"), reverse);    
    for (int i = 0; i < maxSeg; ++i) {
        uint8_t start = DevicePtrGet()->get_outputArray(n)->_leds_segment.get_segment(i)->_start;
        uint8_t end   = DevicePtrGet()->get_outputArray(n)->_leds_segment.get_segment(i)->_end;

        setSegment(i, start, end,  effectSetting->pos, DEFAULT_COLOR, DEFAULT_SPEED, reverse);

        Serial.printf_P(PSTR("\t[%2d] s: %3d e: %3d d: %3d p: %3d\n"), i, start, end, reverse, effectSetting->pos);

        reverse = !reverse;

        resetSegmentRuntime(i);        
    }   
    Serial.println();

  } else {
    effectSetting->segment = false;
    setSegment(0, 0, effectSetting->numLeds - 1, effectSetting->pos, DEFAULT_COLOR, DEFAULT_SPEED, _reverseStart);
    resetSegmentRuntime(0);     
  } 
  start();
  // Serial.print("segement: ");Serial.println(effectSetting->segment);
  // Serial.print("numLeds: ");Serial.println(effectSetting->numLeds);
  // Serial.print("reverse: ");Serial.println(effectSetting->reverse);

}

WS2812FX::WS2812FX(uint16_t n, CRGBW * targetStrip, WS2812FX_setting * setting) {
  _strip_w =  targetStrip;
  setup(n, setting);
}

WS2812FX::WS2812FX(uint16_t n, CRGB * targetStrip, WS2812FX_setting * setting) {
  _strip =  targetStrip;
  setup(n, setting);

}

WS2812FX::~WS2812FX(){
    // fsprintf("[DELETE WS2812FX]\n");
    delete[] _segments;
    delete[] _active_segments;
    delete[] _segment_runtimes;
}

void WS2812FX::init() {
  resetSegmentRuntimes();
}

void WS2812FX::service(boolean & setCycle, boolean & _show) {

    currentPalette = effectSetting->pPalette;

    now = millis(); // Be aware, millis() rolls over every 49 days
    nowUp = now;
    
    bool doShow = false;
    _show = doShow;

    if (DevicePtrGet()->get_outputArray(_stripPos)->_leds_segment.isSegment() && effectSetting->segment ) { 
        uint8_t maxSeg = 0 ;
        DevicePtrGet()->get_outputArray(_stripPos)->_leds_segment.get_max(maxSeg);

        for (int i = 0; i < maxSeg; ++i) {
                uint8_t start   = DevicePtrGet()->get_outputArray(_stripPos)->_leds_segment.get_segment(i)->_start;
                uint8_t end     = DevicePtrGet()->get_outputArray(_stripPos)->_leds_segment.get_segment(i)->_end;
                boolean reverse = DevicePtrGet()->get_outputArray(_stripPos)->_leds_segment.get_segment(i)->_reverse;
                _seg            = &_segments[_active_segments[i]];

                _seg->reverse   = reverse;
                _seg->palette   = effectSetting->palette;
                _seg->size      = effectSetting->size;
                _seg->variation = effectSetting->variation;
                _seg->intensity = effectSetting->intensity;
                _seg->fade      = effectSetting->fade;
                _seg->colors[0] = rgb2_32(effectSetting->c1);
                _seg->colors[1] = rgb2_32(effectSetting->c2);

                _seg_len        = (uint16_t)(end - start+1);                
                _seg_rt         = &_segment_runtimes[i];

                CLR_FRAME_CYCLE;

                setCycle=false;

                if(now > _seg_rt->next_time ) {
                    SET_FRAME;
                    doShow = true;
                     _show = doShow;
                    uint16_t delay = (this->*_effectMod[0])();
                    if (_seg_rt->counter_mode_step == 0) setCycle=true;
                    _seg_rt->next_time = now + max(delay, SPEED_MIN);
                    _seg_rt->counter_mode_call++;
                }              
        } 

    } else {

        int i = 0;

        _seg     = &_segments[_active_segments[i]];

        _seg->reverse   = false;
        _seg->palette   = effectSetting->palette;
        _seg->size      = effectSetting->size;
        _seg->variation = effectSetting->variation;
        _seg->intensity = effectSetting->intensity;
        _seg->fade      = effectSetting->fade;
        _seg->colors[0] = rgb2_32(effectSetting->c1);
        _seg->colors[1] = rgb2_32(effectSetting->c2);

        _seg_len        = (uint16_t)(_seg->stop - _seg->start + 1);
        _seg_rt         = &_segment_runtimes[i];

        if (setCycle) setCycle = false;

        CLR_FRAME_CYCLE;

        if(now > _seg_rt->next_time ) {

            SET_FRAME;

            doShow = true;

            uint16_t delay = (this->*_effectMod[0])();

            if (_seg_rt->counter_mode_step == 0) setCycle=true;

            _seg_rt->next_time = now + max(delay, SPEED_MIN);

            _seg_rt->counter_mode_call++;
        }
    }


    if(doShow) {
        // show();
    }

    // _triggered = false;

  // }
}








/* #####################################################
#
#  Mode Functions
#
##################################################### */

/*
 * No blinking. Just plain old static light.
 */
uint16_t WS2812FX::mode_static(void) {
  if (effectSetting->isTw)  ledsSet::fill(_strip_w, _seg->colors[0], _seg->start, _seg_len);
  else                      ledsSet::fill(_strip,   _seg->colors[0], _seg->start, _seg_len);
  SET_CYCLE;
  return _seg->speed;
}


/*
 * Blink/strobe function
 * Alternate between color1 and color2
 * if(strobe == true) then create a strobe effect
 */
uint16_t WS2812FX::blink(uint32_t color1, uint32_t color2, bool strobe) {
  if(_seg_rt->counter_mode_call & 1) {
    uint32_t color = (IS_REVERSE) ? color1 : color2; // off
    if (effectSetting->isTw)  ledsSet::fill(_strip_w, color, _seg->start, _seg_len);
    else                      ledsSet::fill(_strip,   color, _seg->start, _seg_len);
    SET_CYCLE;
    return strobe ? _seg->speed - 20 : (_seg->speed / 2);
  } else {
    uint32_t color = (IS_REVERSE) ? color2 : color1; // on
    if (effectSetting->isTw)  ledsSet::fill(_strip_w, color, _seg->start, _seg_len);
    else                      ledsSet::fill(_strip,   color, _seg->start, _seg_len);
    return strobe ? 20 : (_seg->speed / 2);
  }
}


/*
 * Normal blinking. 50% on/off time.
 */
uint16_t WS2812FX::mode_blink(void) {
  return blink(_seg->colors[0], _seg->colors[1], false);
}


/*
 * Classic Blink effect. Cycling through the rainbow.
 */
uint16_t WS2812FX::mode_blink_rainbow(void) {
  return blink(color_wheel(_seg_rt->counter_mode_call & 0xFF), _seg->colors[1], false);
}


/*
 * Classic Strobe effect.
 */
uint16_t WS2812FX::mode_strobe(void) {
  return blink(_seg->colors[0], _seg->colors[1], true);
}


/*
 * Classic Strobe effect. Cycling through the rainbow.
 */
uint16_t WS2812FX::mode_strobe_rainbow(void) {
  return blink(color_wheel(_seg_rt->counter_mode_call & 0xFF), _seg->colors[1], true);
}

uint16_t WS2812FX::mode_tricolor_wipe(void)
{
  uint32_t cycleTime = _seg->speed;
  uint32_t perc = now % cycleTime;
  uint16_t prog = (perc * 65535) / cycleTime;
  uint16_t ledIndex = (prog * _seg_len * 3) >> 16;
  uint16_t ledOffset = ledIndex;

  for (int i = 0; i < _seg_len; i++)
  {
    setPixelColor(i, color_from_palette(i, true, PALETTE_SOLID_WRAP, 2));
  }
  
  if(ledIndex < _seg_len) { //wipe from 0 to 1
    for (int i = 0; i < _seg_len; i++)
    {
      setPixelColor(i, (i > ledOffset)? SEGCOLOR(0) : SEGCOLOR(1));
    }
  } else if (ledIndex < _seg_len*2) { //wipe from 1 to 2
    ledOffset = ledIndex - _seg_len;
    for (int i = ledOffset +1; i < _seg_len; i++)
    {
      setPixelColor(i, SEGCOLOR(1));
    }
  } else //wipe from 2 to 0
  {
    ledOffset = ledIndex - _seg_len*2;
    for (int i = 0; i <= ledOffset; i++)
    {
      setPixelColor(i, SEGCOLOR(0));
    }
  }

  return FRAMETIME;
}

/*
 * Color wipe function
 * LEDs are turned on (color1) in sequence, then turned off (color2) in sequence.
 * if (bool rev == true) then LEDs are turned off in reverse order
 */
uint16_t WS2812FX::color_wipe(uint32_t color1, uint32_t color2, bool rev) {


  if(_seg_rt->counter_mode_step < _seg_len) {
    uint32_t led_offset = _seg_rt->counter_mode_step;
    if(IS_REVERSE) {
      setPixelColor(_seg->stop - led_offset, color1);
    } else {
      setPixelColor(_seg->start + led_offset, color1);
    }
  } else {
    uint32_t led_offset = _seg_rt->counter_mode_step - _seg_len;
    if((IS_REVERSE && !rev) || (!IS_REVERSE && rev)) {
      setPixelColor(_seg->stop - led_offset, color2);
    } else {
      setPixelColor(_seg->start + led_offset, color2);
    }
  }

  _seg_rt->counter_mode_step = (_seg_rt->counter_mode_step + 1) % (_seg_len * 2);

  if(_seg_rt->counter_mode_step == 0) SET_CYCLE;

  return ((_seg->speed / (_seg_len * 2))*6);
}

uint16_t WS2812FX::color_wipe_inv(uint32_t color1, uint32_t color2, bool rev) { // rgb2_32(setting.c1)
    uint32_t  len     = (_seg_len/2);
    uint32_t  modulo  = _seg_len % 2;
    // if (!effectSetting->segment) len = len - 1;

    if( modulo  && (_seg_rt->counter_mode_step == len)  ) { 
      if(IS_REVERSE)  setPixelColor(len, 0);
      else            setPixelColor(_seg->start+len, 0);
      _seg_rt->counter_mode_step = (_seg_rt->counter_mode_step + 1) % (_seg_len  );
      return 0;
    }
    else if(_seg_rt->counter_mode_step < (uint32_t)((len))  ) {
      uint32_t led_offset = _seg_rt->counter_mode_step;
      // if ((effectSetting->segment)) led_offset = ((len-1) - led_offset);
      if(IS_REVERSE) { 
        setPixelColor(_seg->start    + led_offset, color1);
        setPixelColor(_seg->stop     - led_offset, color1); 
      } else {
        setPixelColor(_seg->start    + led_offset, color1);
        setPixelColor(_seg->stop     - led_offset, color1);             
      }
    } else {
      uint32_t led_offset = (_seg_rt->counter_mode_step) - (len);
      // led_offset = ((len) - led_offset);
      if(IS_REVERSE) { 
        if (_reverseStart)
          setPixelColor(( (_seg->stop/2) - led_offset ),  color2);
        else
          setPixelColor(( len - led_offset ),  color2);

        if (!effectSetting->segment)
          setPixelColor(( (_seg->stop/2) + led_offset ) +modulo,  color2); 
        else {
          if (_reverseStart)
            setPixelColor(( (_seg->stop/2) + led_offset ),  color2); 
          else
            setPixelColor(( len + led_offset ),  color2);
        } 
      } else {
        if (!effectSetting->segment)
          setPixelColor(( (_seg->start+len)   - led_offset) -modulo,  color2);
        else {
          if (_reverseStart)
            setPixelColor(( (_seg->start+len)   - led_offset),  color2);
          else  
            setPixelColor(( (_seg->stop+len)   - led_offset) +modulo,  color2);
        }
        if (!effectSetting->segment)
          setPixelColor(( (_seg->start+len)   + led_offset),  color2);
        else {
          if (_reverseStart)
            setPixelColor(( (_seg->start+len)   + led_offset),  color2);
          else
            setPixelColor(( (_seg->stop+len)   + led_offset) +modulo,  color2);
        }

      }
    }
    _seg_rt->counter_mode_step = (_seg_rt->counter_mode_step + 1) % (_seg_len  );
    if (_seg_rt->counter_mode_step == 0) {SET_CYCLE;}
    return (_seg->speed / (_seg_len * 2))*6;
}

/*
 * Lights all LEDs one after another.
 */
uint16_t WS2812FX::mode_color_wipe(void) {
  return color_wipe(_seg->colors[0], _seg->colors[1], false);
}

uint16_t WS2812FX::mode_color_wipe_inv(void) {
  return color_wipe_inv(_seg->colors[1], _seg->colors[0], false);
}

uint16_t WS2812FX::mode_color_wipe_rev(void) {
  return color_wipe(_seg->colors[0], _seg->colors[1], true);
}

uint16_t WS2812FX::mode_color_wipe_rev_inv(void) {
  return color_wipe(_seg->colors[1], _seg->colors[0], true);
}


/*
 * Turns all LEDs after each other to a random color.
 * Then starts over with another color.
 */
uint16_t WS2812FX::mode_color_wipe_random(void) {
  if(_seg_rt->counter_mode_step % _seg_len == 0) { // aux_param will store our random color wheel index
    _seg_rt->aux_param = get_random_wheel_index(_seg_rt->aux_param);
  }
  // uint32_t color = color_wheel(_seg_rt->aux_param);
  ledsTools _tools;
  CRGB c = _tools.Wheel(_seg_rt->aux_param);
  HsvColor hsv = _tools.rgb2hsv(c.r,c.g,c.b);
  RgbColor rgb = _tools.hsv2rgb(hsv.h,hsv.s,_seg->intensity,0);
  uint32_t color = rgb2_32(rgb);
  return color_wipe(color, color, false) * 2;
}


/*
 * Random color introduced alternating from start and end of strip.
 */
uint16_t WS2812FX::mode_color_sweep_random(void) {
  if(_seg_rt->counter_mode_step % _seg_len == 0) { // aux_param will store our random color wheel index
    _seg_rt->aux_param = get_random_wheel_index(_seg_rt->aux_param);
  }
  uint32_t color = color_wheel(_seg_rt->aux_param);
  return color_wipe(color, color, true) * 2;
}


/*
 * Lights all LEDs in one random color up. Then switches them
 * to the next random color.
 */
uint16_t WS2812FX::mode_random_color(void) {
  _seg_rt->aux_param = get_random_wheel_index(_seg_rt->aux_param); // aux_param will store our random color wheel index
  uint32_t color = color_wheel(_seg_rt->aux_param);
  if (effectSetting->isTw)  ledsSet::fill(_strip_w, color, _seg->start, _seg_len);
  else                      ledsSet::fill(_strip,   color, _seg->start, _seg_len);
  SET_CYCLE;
  return _seg->speed;
}


/*
 * Lights every LED in a random color. Changes one random LED after the other
 * to another random color.
 */
uint16_t WS2812FX::mode_single_dynamic(void) {
  if(_seg_rt->counter_mode_call == 0) {
    for(uint16_t i=_seg->start; i <= _seg->stop; i++) {
      setPixelColor(i, color_wheel(random8()));
    }
  }

  setPixelColor(_seg->start + random16(_seg_len), color_wheel(random8()));
  SET_CYCLE;
  return _seg->speed;
}


/*
 * Lights every LED in a random color. Changes all LED at the same time
 * to new random colors.
 */
uint16_t WS2812FX::mode_multi_dynamic(void) {
  for(uint16_t i=_seg->start; i <= _seg->stop; i++) {
    setPixelColor(i, color_wheel(random8()));
  }
  SET_CYCLE;
  return _seg->speed;
}




/*
 * Does the "standby-breathing" of well known i-Devices. Fixed Speed.
 * Use mode "fade" if you like to have something similar with a different speed.
 */

uint16_t WS2812FX::mode_breath(void) {
  int lum = _seg_rt->counter_mode_step;
  if(lum > 255) lum = 511 - lum; // lum = 15 -> 255 -> 15

  uint16_t delay;
  if(lum == 15) delay = 970; // 970 pause before each breath
  else if(lum <=  25) delay = 38; // 19
  else if(lum <=  50) delay = 36; // 18
  else if(lum <=  75) delay = 28; // 14
  else if(lum <= 100) delay = 20; // 10
  else if(lum <= 125) delay = 14; // 7
  else if(lum <= 150) delay = 11; // 5
  else delay = 10; // 4

  uint32_t color =  color_blend(_seg->colors[1], _seg->colors[0], map(lum, 0, 255, lum, _seg->intensity));

  if (effectSetting->isTw)  ledsSet::fill(_strip_w, color, _seg->start, _seg->stop+1);
  else                      ledsSet::fill(_strip,   color, _seg->start, _seg->stop+1);

  _seg_rt->counter_mode_step += 2;
  if(_seg_rt->counter_mode_step > (512-15)) {
    _seg_rt->counter_mode_step = 15;
    SET_CYCLE;
  }
  return delay;
}
/*
uint16_t WS2812FX::mode_breath(void) {
  uint16_t var = 0;
  uint16_t counter = (now * ((_seg->speed >> 3) +10));
  counter = (counter >> 2) + (counter >> 4); //0-16384 + 0-2048
  if (counter < 16384) {
    if (counter > 8192) counter = 8192 - (counter - 8192);
    var = sin16(counter) / 103; //close to parabolic in range 0-8192, max val. 23170
  }
  
  uint8_t lum = 30 + var;
  for(uint16_t i=_seg->start; i <= _seg->stop; i++) {
    setPixelColor(i, color_blend(_seg->colors[1], color_from_palette(i, true, PALETTE_SOLID_WRAP, 0), lum));
  }

  return _seg->speed / _seg_len;
}
*/
// #define PALETTE_SOLID_WRAP (paletteBlend == 1 || paletteBlend == 3)

// uint16_t WS2812FX::mode_breath(void) {
//   uint16_t var = 0;
//   uint16_t counter = (nowUp * ((_seg->speed >> 3) +10));
//   counter = (counter >> 2) + (counter >> 4); //0-16384 + 0-2048
//   if (counter < 16384) {
//     if (counter > 8192) counter = 8192 - (counter - 8192);
//     var = sin16(counter) / 103; //close to parabolic in range 0-8192, max val. 23170
//   }
  
//   uint8_t lum = 30 + var;
//   for(uint16_t i=_seg->start; i <= _seg->stop; i++) {
//     setPixelColor(i, color_blend(SEGCOLOR(1), color_from_palette(i, true, PALETTE_SOLID_WRAP, 0), lum));
//   }

//   return _seg->speed / 256;
// }


/*
 * Fades the LEDs between two colors
 */
uint16_t WS2812FX::mode_fade(void) {
  int lum = _seg_rt->counter_mode_step;
  if(lum > 255) lum = 511 - lum; // lum = 0 -> 255 -> 0

  uint32_t color = color_blend(0, _seg->colors[0], lum);
  if (effectSetting->isTw)  ledsSet::fill(_strip_w, color, _seg->start, _seg_len);
  else                      ledsSet::fill(_strip,   color, _seg->start, _seg_len);

  _seg_rt->counter_mode_step += 4;
  if(_seg_rt->counter_mode_step > 511) {
    _seg_rt->counter_mode_step = 0;
    SET_CYCLE;
  }
  return (_seg->speed / 128);
}


/*
 * scan function - runs a block of pixels back and forth.
 */

uint16_t WS2812FX::scan(bool dual)
{
  uint32_t cycleTime = 750 + (255 - _seg->speed)*150;
  uint32_t perc = now % cycleTime;
  uint16_t prog = (perc * 65535) / cycleTime;
  uint16_t size = 1 + ((_seg->intensity * _seg_len) >> 9);
  uint16_t ledIndex = (prog * ((_seg_len *2) - size *2)) >> 16;

  // fill(SEGCOLOR(1));
  if (effectSetting->isTw)  ledsSet::fill(_strip_w, SEGCOLOR(1), _seg->start, _seg->stop+1);
  else                      ledsSet::fill(_strip,   SEGCOLOR(1), _seg->start, _seg->stop+1);
  int start = 0;
  int led_offset;

  if (IS_REVERSE && effectSetting->segment) {
    start = _seg->start;
  }

  led_offset = ledIndex - (_seg_len - size);
  led_offset = abs(led_offset);
  if (IS_REVERSE && effectSetting->segment) {
    led_offset = (_seg_len-size)-led_offset;
  }

    if (dual) {
      for (uint16_t j = led_offset; j < led_offset + size; j++) {
        uint16_t i2 = (_seg_len ) -1 -j;
        setPixelColor(i2, color_from_palette(i2, true, PALETTE_SOLID_WRAP, (SEGCOLOR(2))? 2:0));
      }
    }

    for (uint16_t j = start + led_offset; j < start + led_offset + size; j++) {
      setPixelColor(j, color_from_palette(j, true, PALETTE_SOLID_WRAP, 0));
    }      



  return FRAMETIME;
}

uint16_t WS2812FX::scan(uint32_t color1, uint32_t color2, bool dual) {
  int8_t dir = _seg_rt->aux_param ? -1 : 1;
  uint8_t size = 1 << SIZE_OPTION;
  uint32_t pixelPos ;

  if (effectSetting->isTw)  ledsSet::fill(_strip_w, color2, _seg->start, _seg_len);
  else                      ledsSet::fill(_strip,   color2, _seg->start, _seg_len);

  for(uint8_t i = 0; i < size; i++) {

    // uint32_t pixelPos = _seg->start + _seg_rt->counter_mode_step + i;
    // pixelPos = (IS_REVERSE) ? _seg->stop - pixelPos : pixelPos;

    if (IS_REVERSE) {
      if (dual) setPixelColor(_seg->stop - _seg_rt->counter_mode_step - i, color1);

      setPixelColor(_seg->start + _seg_rt->counter_mode_step + i, color1);

    } else {
      if (dual) {
        pixelPos = _seg->stop + _seg_rt->counter_mode_step - i;
        // pixelPos = pixelPos + _seg->stop ;
        setPixelColor(pixelPos, color1);
      }
      pixelPos = _seg->start - _seg_rt->counter_mode_step - i;
      // pixelPos = _seg->stop - pixelPos ;      
      setPixelColor(pixelPos, color1);
    }

    // if(IS_REVERSE || dual) {
    //   setPixelColor(_seg->stop - _seg_rt->counter_mode_step - i, color1);
    // }
    // if(!IS_REVERSE || dual) {
    //   setPixelColor(_seg->start + _seg_rt->counter_mode_step + i, color1);
    // }

    // setPixelColor(pixelPos, color1);
  }

  if (IS_REVERSE) _seg_rt->counter_mode_step += dir;
  else _seg_rt->counter_mode_step -= dir;
  if(_seg_rt->counter_mode_step == 0) {
    _seg_rt->aux_param = 0;
    SET_CYCLE;
  }
  if(_seg_rt->counter_mode_step >= (uint16_t)(_seg_len - size)) _seg_rt->aux_param = 1;    


  return (_seg->speed / (_seg_len * 2));
}


/*
 * Runs a block of pixels back and forth.
 */
uint16_t WS2812FX::mode_scan(void) {
  return scan(false);
}


/*
 * Runs two blocks of pixels back and forth in opposite directions.
 */
uint16_t WS2812FX::mode_dual_scan(void) {
  return scan(true);
}


/*
 * Cycles all LEDs at once through a rainbow.
 */
  // fastled_col.red =   (color >> 16 & 0xFF);
  // fastled_col.green = (color >> 8  & 0xFF);
  // fastled_col.blue =  (color       & 0xFF);
uint16_t WS2812FX::mode_rainbow(void) {
  uint32_t color = color_wheel(_seg_rt->counter_mode_step);
  ledsTools _tools;
  CRGB c1 = _tools.rgb32_crgb(color);
  HsvColor hsv = rgb2hsv(c1.r, c1.g, c1.b);
  c1 = CHSV(hsv.h, hsv.s, _seg->intensity) ;

  // c1 = _tools.fade2black(c1.r, c1.g, c1.b, _seg->intensity) ;
  if (effectSetting->isTw)  ledsSet::fill(_strip_w, c1, _seg->start, _seg->stop+1);
  else                      ledsSet::fill(_strip,   c1, _seg->start, _seg->stop+1);

  _seg_rt->counter_mode_step = (_seg_rt->counter_mode_step + 1) & 0xFF;

  if(_seg_rt->counter_mode_step == 0)  SET_CYCLE;

  return (_seg->speed / 256);
}


/*
 * Cycles a rainbow over the entire string of LEDs.
 */
uint16_t WS2812FX::mode_rainbow_cycle(void) {
  // float ratio = 256  / map(effectSetting->scale, 0, 255, 0, _seg_len);
  uint8_t val, bc = _seg->intensity, bright_var = _seg->fade;
  for(uint16_t i=0; i < _seg_len; i++) {
    // uint32_t color = color_wheel(((i * 256 / _seg_len) + _seg_rt->counter_mode_step) & 0xFF);
    ledsTools _tools;
    CRGB c = _tools.Wheel(((i* effectSetting->scale / _seg_len) + _seg_rt->counter_mode_step) & 0xFF);
    HsvColor hsv = _tools.rgb2hsv(c.r,c.g,c.b);

    val = map( (i * (uint8_t)(effectSetting->scale/2) / _seg_len ) + map(_seg_rt->counter_mode_step, 0, UINT32_MAX, 0, _seg_len) * 2 , 0 , _seg_len , 0, 255);
    val = (val<128) ? 2 * val : 2 * (255-val) ;     
    val = map8(val, map(bright_var, 0, 255, 5, bc), bc);
    // val = (bright_var<bc) ? map8(val, bright_var, bc) : bc;

    RgbColor rgb = _tools.hsv2rgb(hsv.h,hsv.s,val,0);  
    uint32_t color = rgb2_32(rgb);  

    uint32_t pixelPos = (IS_REVERSE)? _seg->stop - i : _seg->start + i;

    setPixelColor(pixelPos, color);
  }
 
  if (_reverseStart) _seg_rt->counter_mode_step = (_seg_rt->counter_mode_step + 1) & 0xFF;
  else               _seg_rt->counter_mode_step = (_seg_rt->counter_mode_step - 1) & 0xFF;

  if(_seg_rt->counter_mode_step == 0) SET_CYCLE;

  return (_seg->speed / 256);
}

/*
 * Tricolor chase function
 */
uint16_t WS2812FX::tricolor_chase(uint32_t color1, uint32_t color2) {
  uint32_t cycleTime = 50 + ((255 - _seg->speed)<<1);
  uint32_t it = now / cycleTime;  // iterator
  uint8_t width = (1 + (_seg->size>>4)); // value of 1-16 for each colour
  uint8_t index = it % (width*3);
  
  for (uint16_t i = 0; i < _seg_len; i++, index++) {
    if (index > (width*3)-1) index = 0;

    uint32_t color = color1;
    if (index > (width<<1)-1) color = color_from_palette(i, true, PALETTE_SOLID_WRAP, 1);
    else if (index > width-1) color = color2;

    uint16_t pixelPos = _seg_len - i -1;
    pixelPos = (IS_REVERSE)? (_seg_len-1) - pixelPos:pixelPos;

    setPixelColor(pixelPos, color);
  }
  return FRAMETIME;
}
uint16_t WS2812FX::tricolor_chase(uint32_t color1, uint32_t color2, uint32_t color3) {
  uint8_t   sizeCnt   = _seg->size;
  if        (sizeCnt < 1) sizeCnt = 1;
  else if   (sizeCnt > 3) sizeCnt = 3;

  uint8_t   sizeCnt2  = sizeCnt   + sizeCnt;
  uint8_t   sizeCnt3  = sizeCnt2  + sizeCnt;
  uint16_t  index     = _seg_rt->counter_mode_step % sizeCnt3;

  for(uint16_t i=0; i < _seg_len; i++, index++) {
    index = index % sizeCnt3;

    uint32_t color = color1;
    if(index < sizeCnt) color = color_from_palette(i, true, PALETTE_SOLID_WRAP, 1);
    else if(index < sizeCnt2) color = color2;

    if(IS_REVERSE) {
      setPixelColor(_seg->start + i, color);
    } else {
      setPixelColor(_seg->stop - i, color);
    }
  }

  _seg_rt->counter_mode_step++;
  if(_seg_rt->counter_mode_step % _seg_len == 0) SET_CYCLE;

  return (_seg->speed / _seg_len);
}


/*
 * Tricolor chase mode
 */
uint16_t WS2812FX::mode_tricolor_chase(void) {
  return tricolor_chase(_seg->colors[0], _seg->colors[1], _seg->colors[2]);
}


/*
 * Alternating white/red/black pixels running.
 */
uint16_t WS2812FX::mode_circus_combustus(void) {
  return tricolor_chase(COLOR_RED, COLOR_WHITE, COLOR_BLACK);
}


/*
 * Theatre-style crawling lights.
 * Inspired by the Adafruit examples.
 */
uint16_t WS2812FX::mode_theater_chase(void) {
  return tricolor_chase(_seg->colors[0], _seg->colors[1], _seg->colors[1]);
}


/*
 * Theatre-style crawling lights with rainbow effect.
 * Inspired by the Adafruit examples.
 */
uint16_t WS2812FX::mode_theater_chase_rainbow(void) {
  _seg_rt->counter_mode_step = (_seg_rt->counter_mode_step + 1) & 0xFF;
  uint32_t color = color_wheel(_seg_rt->counter_mode_step);
  return tricolor_chase(color, _seg->colors[1], _seg->colors[1]);
}


/*
 * Running lights effect with smooth sine transition.
 */
uint16_t WS2812FX::mode_running_lights(void) {
  uint8_t size = 1 << SIZE_OPTION;
  uint8_t sineIncr = max(1, (256 / _seg_len) * size);
  for(uint16_t i=0; i < _seg_len; i++) {
    int lum = (int)sine8(((i + _seg_rt->counter_mode_step) * sineIncr));
    uint32_t color = color_blend(_seg->colors[0], _seg->colors[1], lum);
    if(IS_REVERSE) {
      setPixelColor(_seg->start + i, color);
    } else {
      setPixelColor(_seg->stop - i,  color);
    }
  }
  _seg_rt->counter_mode_step = (_seg_rt->counter_mode_step + 1) % 256;
  if(_seg_rt->counter_mode_step == 0) SET_CYCLE;
  return (_seg->speed / _seg_len);
}


/*
 * twinkle function
 */
uint16_t WS2812FX::twinkle(uint32_t color1, uint32_t color2) {
  if(_seg_rt->counter_mode_step == 0) {
    if (effectSetting->isTw)  ledsSet::fill(_strip_w, color2, _seg->start, _seg_len);
    else                      ledsSet::fill(_strip,   color2, _seg->start, _seg_len);
    uint16_t min_leds = (_seg_len / 4) + 1; // make sure, at least one LED is on
    _seg_rt->counter_mode_step = random(min_leds, min_leds * 2);
    SET_CYCLE;
  }

  setPixelColor(_seg->start + random16(_seg_len), color1);

  _seg_rt->counter_mode_step--;
  return (_seg->speed / _seg_len);
}

/*
 * Blink several LEDs on, reset, repeat.
 * Inspired by www.tweaking4all.com/hardware/arduino/arduino-led-strip-effects/
 */
uint16_t WS2812FX::mode_twinkle(void) {
  return twinkle(_seg->colors[0], _seg->colors[1]);
}

/*
 * Blink several LEDs in random colors on, reset, repeat.
 * Inspired by www.tweaking4all.com/hardware/arduino/arduino-led-strip-effects/
 */
uint16_t WS2812FX::mode_twinkle_random(void) {
  return twinkle(color_wheel(random8()), _seg->colors[1]);
}


/*
 * fade out functions
 */
void WS2812FX::fade_out() {
  return fade_out(_seg->colors[1]);
}


void WS2812FX::fade_out(uint32_t targetColor) {
  static const uint8_t rateMapH[] = {0, 1, 1, 1, 2, 3, 4, 6};
  static const uint8_t rateMapL[] = {0, 2, 3, 8, 8, 8, 8, 8};

  uint8_t rate  = FADE_RATE;
  uint8_t rateH = rateMapH[rate];
  uint8_t rateL = rateMapL[rate];

  uint32_t color = targetColor;
  int w2 = (color >> 24) & 0xff;
  int r2 = (color >> 16) & 0xff;
  int g2 = (color >>  8) & 0xff;
  int b2 =  color        & 0xff;

  for(uint16_t i=_seg->start; i <= _seg->stop; i++) {
    if (effectSetting->isTw)  color = getPixelColor(_strip_w, i); // current color
    else                      color = getPixelColor(_strip,   i); // current color

    if(rate == 0) { // old fade-to-black algorithm
      setPixelColor(i, (color >> 1) & 0x7F7F7F7F);
    } else { // new fade-to-color algorithm
      int w1 = (color >> 24) & 0xff;
      int r1 = (color >> 16) & 0xff;
      int g1 = (color >>  8) & 0xff;
      int b1 =  color        & 0xff;

      // calculate the color differences between the current and target colors
      int wdelta = w2 - w1;
      int rdelta = r2 - r1;
      int gdelta = g2 - g1;
      int bdelta = b2 - b1;

      // if the current and target colors are almost the same, jump right to the target
      // color, otherwise calculate an intermediate color. (fixes rounding issues)
      wdelta = abs(wdelta) < 3 ? wdelta : (wdelta >> rateH) + (wdelta >> rateL);
      rdelta = abs(rdelta) < 3 ? rdelta : (rdelta >> rateH) + (rdelta >> rateL);
      gdelta = abs(gdelta) < 3 ? gdelta : (gdelta >> rateH) + (gdelta >> rateL);
      bdelta = abs(bdelta) < 3 ? bdelta : (bdelta >> rateH) + (bdelta >> rateL);

      setPixelColor(i, r1 + rdelta, g1 + gdelta, b1 + bdelta, w1 + wdelta);
    }
  }
}

/*
 * twinkle_fade function
 */
uint16_t WS2812FX::twinkle_fade(uint32_t color) {
  fade_out();

  if(random8(3) == 0) {
    uint8_t size = 1 << SIZE_OPTION;
    uint16_t index = _seg->start + random16(_seg_len - size);
    if (effectSetting->isTw)  ledsSet::fill(_strip_w, color, index, size);
    else                      ledsSet::fill(_strip,   color, index, size);
    SET_CYCLE;
  }
  return (_seg->speed / 8);
}


/*
 * Blink several LEDs on, fading out.
 */
uint16_t WS2812FX::mode_twinkle_fade(void) {
  return twinkle_fade(_seg->colors[0]);
}


/*
 * Blink several LEDs in random colors on, fading out.
 */
uint16_t WS2812FX::mode_twinkle_fade_random(void) {
  return twinkle_fade(color_wheel(random8()));
}

/*
 * Sparkle function
 * color1 = background color
 * color2 = sparkle color
 */
uint16_t WS2812FX::sparkle(uint32_t color1, uint32_t color2) {
  if(_seg_rt->counter_mode_step == 0) {
    if (effectSetting->isTw)  ledsSet::fill(_strip_w, color1, _seg->start, _seg_len);
    else                      ledsSet::fill(_strip,   color1, _seg->start, _seg_len);
  }

  uint8_t size = 1 << SIZE_OPTION;
  if (effectSetting->isTw)  ledsSet::fill(_strip_w, color1, _seg->start + _seg_rt->aux_param3, size);
  else                      ledsSet::fill(_strip,   color1, _seg->start + _seg_rt->aux_param3, size);

  _seg_rt->aux_param3 = random16(_seg_len - size); // aux_param3 stores the random led index
  if (effectSetting->isTw)  ledsSet::fill(_strip_w, color2, _seg->start + _seg_rt->aux_param3, size);
  else                      ledsSet::fill(_strip,   color2, _seg->start + _seg_rt->aux_param3, size);

  SET_CYCLE;
  return (_seg->speed / 32);
}


/*
 * Blinks one LED at a time.
 * Inspired by www.tweaking4all.com/hardware/arduino/arduino-led-strip-effects/
 */
uint16_t WS2812FX::mode_sparkle(void) {
  return sparkle(_seg->colors[1], _seg->colors[0]);
}


/*
 * Lights all LEDs in the color. Flashes white pixels randomly.
 * Inspired by www.tweaking4all.com/hardware/arduino/arduino-led-strip-effects/
 */
uint16_t WS2812FX::mode_flash_sparkle(void) {
  return sparkle(_seg->colors[0], COLOR_WHITE);
}


/*
 * Like flash sparkle. With more flash.
 * Inspired by www.tweaking4all.com/hardware/arduino/arduino-led-strip-effects/
 */
uint16_t WS2812FX::mode_hyper_sparkle(void) {
  if (effectSetting->isTw)  ledsSet::fill(_strip_w, _seg->colors[0], _seg->start, _seg_len);
  else                      ledsSet::fill(_strip,   _seg->colors[0], _seg->start, _seg_len);

  uint8_t size = 1 << SIZE_OPTION;
  for(uint8_t i=0; i<8; i++) {
    if (effectSetting->isTw)  ledsSet::fill(_strip_w, COLOR_WHITE, _seg->start + random16(_seg_len - size), size);
    else                      ledsSet::fill(_strip,   COLOR_WHITE, _seg->start + random16(_seg_len - size), size);
  }

  SET_CYCLE;
  return (_seg->speed / 32);
}


/*
 * Strobe effect with different strobe count and pause, controlled by speed.
 */
uint16_t WS2812FX::mode_multi_strobe(void) {
  if (effectSetting->isTw)  ledsSet::fill(_strip_w, _seg->colors[1], _seg->start, _seg_len);
  else                      ledsSet::fill(_strip,   _seg->colors[1], _seg->start, _seg_len);

  uint16_t delay = 200 + ((9 - (_seg->speed % 10)) * 100);
  uint16_t count = 2 * ((_seg->speed / 100) + 1);
  if(_seg_rt->counter_mode_step < count) {
    if((_seg_rt->counter_mode_step & 1) == 0) {
      if (effectSetting->isTw)  ledsSet::fill(_strip_w, _seg->colors[0], _seg->start, _seg_len);
      else                      ledsSet::fill(_strip,   _seg->colors[0], _seg->start, _seg_len);
      delay = 20;
    } else {
      delay = 50;
    }
  }

  _seg_rt->counter_mode_step = (_seg_rt->counter_mode_step + 1) % (count + 1);
  if(_seg_rt->counter_mode_step == 0) SET_CYCLE;
  return delay;
}


/*
 * color chase function.
 * color1 = background color
 * color2 and color3 = colors of two adjacent leds
 */
uint16_t WS2812FX::chase(uint32_t color1, uint32_t color2, uint32_t color3) {
  uint8_t size = 1 << SIZE_OPTION;
  for(uint8_t i=0; i<size; i++) {
    uint16_t a = (_seg_rt->counter_mode_step + i) % _seg_len;
    uint16_t b = (a + size) % _seg_len;
    uint16_t c = (b + size) % _seg_len;
    if(IS_REVERSE) {
      setPixelColor(_seg->stop - a, color1);
      setPixelColor(_seg->stop - b, color2);
      setPixelColor(_seg->stop - c, color3);
    } else {
      setPixelColor(_seg->start + a, color1);
      setPixelColor(_seg->start + b, color2);
      setPixelColor(_seg->start + c, color3);
    }
  }

  if(_seg_rt->counter_mode_step + (size * 3) == _seg_len) SET_CYCLE;

  _seg_rt->counter_mode_step = (_seg_rt->counter_mode_step + 1) % _seg_len;
  return (_seg->speed / _seg_len);
}


/*
 * Bicolor chase mode
 */
uint16_t WS2812FX::mode_bicolor_chase(void) {
  return chase(_seg->colors[0], _seg->colors[1], _seg->colors[2]);
}


/*
 * White running on _color.
 */
uint16_t WS2812FX::mode_chase_color(void) {
  return chase(_seg->colors[0], COLOR_WHITE, COLOR_WHITE);
}


/*
 * Black running on _color.
 */
uint16_t WS2812FX::mode_chase_blackout(void) {
  return chase(_seg->colors[0], COLOR_BLACK, COLOR_BLACK);
}


/*
 * _color running on white.
 */
uint16_t WS2812FX::mode_chase_white(void) {
  return chase(COLOR_WHITE, _seg->colors[0], _seg->colors[0]);
}


/*
 * White running followed by random color.
 */
uint16_t WS2812FX::mode_chase_random(void) {
  if(_seg_rt->counter_mode_step == 0) {
    _seg_rt->aux_param = get_random_wheel_index(_seg_rt->aux_param);
  }
  return chase(color_wheel(_seg_rt->aux_param), COLOR_WHITE, COLOR_WHITE);
}


/*
 * Rainbow running on white.
 */
uint16_t WS2812FX::mode_chase_rainbow_white(void) {
  uint16_t n = _seg_rt->counter_mode_step;
  uint16_t m = (_seg_rt->counter_mode_step + 1) % _seg_len;
  uint32_t color2 = color_wheel(((n * 256 / _seg_len) + (_seg_rt->counter_mode_call & 0xFF)) & 0xFF);
  uint32_t color3 = color_wheel(((m * 256 / _seg_len) + (_seg_rt->counter_mode_call & 0xFF)) & 0xFF);

  return chase(COLOR_WHITE, color2, color3);
}


/*
 * White running on rainbow.
 */
uint16_t WS2812FX::mode_chase_rainbow(void) {
  uint8_t color_sep = 256 / _seg_len;
  uint8_t color_index = _seg_rt->counter_mode_call & 0xFF;
  uint32_t color = color_wheel(((_seg_rt->counter_mode_step * color_sep) + color_index) & 0xFF);

  return chase(color, COLOR_WHITE, COLOR_WHITE);
}


/*
 * Black running on rainbow.
 */
uint16_t WS2812FX::mode_chase_blackout_rainbow(void) {
  uint8_t color_sep = 256 / _seg_len;
  uint8_t color_index = _seg_rt->counter_mode_call & 0xFF;
  // uint32_t color = color_wheel(((_seg_rt->counter_mode_step * color_sep) + color_index) & 0xFF);
  ledsTools _tools;
  CRGB c = _tools.Wheel(((_seg_rt->counter_mode_step * color_sep) + color_index) & 0xFF);  
  HsvColor hsv = _tools.rgb2hsv(c.r,c.g,c.b);
  RgbColor rgb = _tools.hsv2rgb(hsv.h,hsv.s,_seg->intensity,0);
  uint32_t color = rgb2_32(rgb);
  return chase(color, COLOR_BLACK, COLOR_BLACK);
}

/*
 * running white flashes function.
 * color1 = background color
 * color2 = flash color
 */
uint16_t WS2812FX::chase_flash(uint32_t color1, uint32_t color2) {
  const static uint8_t flash_count = 4;
  uint8_t flash_step = _seg_rt->counter_mode_call % ((flash_count * 2) + 1);

  if(flash_step < (flash_count * 2)) {
    uint32_t color = (flash_step % 2 == 0) ? color2 : color1;
    uint16_t n = _seg_rt->counter_mode_step;
    uint16_t m = (_seg_rt->counter_mode_step + 1) % _seg_len;
    if(IS_REVERSE) {
      setPixelColor(_seg->stop - n, color);
      setPixelColor(_seg->stop - m, color);
    } else {
      setPixelColor(_seg->start + n, color);
      setPixelColor(_seg->start + m, color);
    }
    return 30;
  } else {
    _seg_rt->counter_mode_step = (_seg_rt->counter_mode_step + 1) % _seg_len;
    if(_seg_rt->counter_mode_step == 0) {
      // update aux_param so mode_chase_flash_random() will select the next color
      _seg_rt->aux_param = get_random_wheel_index(_seg_rt->aux_param);
      SET_CYCLE;
    }
  }
  return (_seg->speed / _seg_len);
}

/*
 * White flashes running on _color.
 */
uint16_t WS2812FX::mode_chase_flash(void) {
  return chase_flash(_seg->colors[0], COLOR_WHITE);
}


/*
 * White flashes running, followed by random color.
 */
uint16_t WS2812FX::mode_chase_flash_random(void) {
  return chase_flash(color_wheel(_seg_rt->aux_param), COLOR_WHITE);
}


/*
 * Alternating pixels running function.
 */


uint16_t WS2812FX::running(uint32_t color1, uint32_t color2) {
  uint8_t size = 2 << SIZE_OPTION;
  uint32_t color = (_seg_rt->counter_mode_step & size) ? color1 : color2;

  if(IS_REVERSE) {
    copyPixels(_seg->start, _seg->start + 1, _seg_len - 1);
    setPixelColor(_seg->stop, color);
  } else {
    copyPixels(_seg->start + 1, _seg->start, _seg_len - 1);
    setPixelColor(_seg->start, color);
  }

  _seg_rt->counter_mode_step = (_seg_rt->counter_mode_step + 1) % _seg_len;
  if(_seg_rt->counter_mode_step == 0) SET_CYCLE;
  return (_seg->speed / _seg_len);
}


/*
 * Alternating color/white pixels running.
 */
uint16_t WS2812FX::mode_running_color(void) {
  return running(_seg->colors[0], _seg->colors[1]);
}


/*
 * Alternating red/blue pixels running.
 */
uint16_t WS2812FX::mode_running_red_blue(void) {
  return running(COLOR_RED, COLOR_BLUE);
}


/*
 * Alternating red/green pixels running.
 */
uint16_t WS2812FX::mode_merry_christmas(void) {
  return running(COLOR_RED, COLOR_GREEN);
}

/*
 * Alternating orange/purple pixels running.
 */
uint16_t WS2812FX::mode_halloween(void) {
  return running(COLOR_PURPLE, COLOR_ORANGE);
}


/*
 * Random colored pixels running.
 */
uint16_t WS2812FX::mode_running_random(void) {
  uint8_t size = 2 << SIZE_OPTION;
  if((_seg_rt->counter_mode_step) % size == 0) {
    _seg_rt->aux_param = get_random_wheel_index(_seg_rt->aux_param);
  }

  uint32_t color = color_wheel(_seg_rt->aux_param);

  return running(color, color);
}


/*
 * K.I.T.T.
 */
uint16_t WS2812FX::mode_larson_scanner(void) {
  fade_out();

  if(_seg_rt->counter_mode_step < _seg_len) {
    if(IS_REVERSE) {
      setPixelColor(_seg->stop - _seg_rt->counter_mode_step, _seg->colors[0]);
    } else {
      setPixelColor(_seg->start + _seg_rt->counter_mode_step, _seg->colors[0]);
    }
  } else {
    uint16_t index = (_seg_len * 2) - _seg_rt->counter_mode_step - 2;
    if(IS_REVERSE) {
      setPixelColor(_seg->stop - index, _seg->colors[0]);
    } else {
      setPixelColor(_seg->start + index, _seg->colors[0]);
    }
  }

  _seg_rt->counter_mode_step++;
  if(_seg_rt->counter_mode_step >= (uint16_t)((_seg_len * 2) - 2)) {
    _seg_rt->counter_mode_step = 0;
    SET_CYCLE;
  }

  return (_seg->speed / (_seg_len * 2));
}


/*
 * Firing comets from one end.
 */
uint16_t WS2812FX::mode_comet(void) {
  fade_out();

  if(IS_REVERSE) {
    setPixelColor(_seg->stop - _seg_rt->counter_mode_step, _seg->colors[0]);
  } else {
    setPixelColor(_seg->start + _seg_rt->counter_mode_step, _seg->colors[0]);
  }

  _seg_rt->counter_mode_step = (_seg_rt->counter_mode_step + 1) % _seg_len;
  if(_seg_rt->counter_mode_step == 0) SET_CYCLE;

  return (_seg->speed / _seg_len);
}



/*
 * Fireworks function.
 */
/*
uint16_t WS2812FX::fireworks(uint32_t color) {
  fade_out(0);
  if (_seg_rt->counter_mode_call == 0) {
    _seg_rt->aux_param = UINT8_MAX;
    _seg_rt->aux_param2 = UINT8_MAX;
  }
  bool valid1 = (_seg_rt->aux_param < (_seg->start+_seg_len));
  bool valid2 = (_seg_rt->aux_param2 < (_seg->start+_seg_len));
  uint32_t sv1 = 0, sv2 = 0;
  if (valid1) {
    if (effectSetting->isTw)  sv1 = getPixelColor(_strip_w, _seg_rt->aux_param); // current color
    else                      sv1 = getPixelColor(_strip,   _seg_rt->aux_param); // current color    
    // sv1 = getPixelColor(_seg_rt->aux_param);
  }
  if (valid2) {
    if (effectSetting->isTw)  sv2 = getPixelColor(_strip_w, _seg_rt->aux_param2); // current color
    else                      sv2 = getPixelColor(_strip,   _seg_rt->aux_param2); // current color        
    // sv2 = getPixelColor(_seg_rt->aux_param2);
  }
  blur(255-_seg->speed);
  if (valid1) setPixelColor(_seg_rt->aux_param , sv1);
  if (valid2) setPixelColor(_seg_rt->aux_param2, sv2);

  for(uint16_t i=0; i<MAX(1, (_seg->start+_seg_len)/20); i++) {
    if(random8(129 - (_seg->intensity >> 1)) == 0) {
      uint16_t index = random((_seg->start+_seg_len));
      setPixelColor(index, color_from_palette(color, false, false, 0));
      _seg_rt->aux_param2 = _seg_rt->aux_param;
      _seg_rt->aux_param = index;
    }
  }

  return 0;
}*/
uint16_t WS2812FX::fireworks(uint32_t color) {
  fade_out(0);

// for better performance, manipulate the Adafruit_NeoPixels pixels[] array directly
  /*
  uint8_t bytesPerPixel = 3; // 3=RGB, 4=RGBW
  uint16_t startPixel = _seg->start * bytesPerPixel + bytesPerPixel;
  uint16_t stopPixel = _seg->stop * bytesPerPixel;
  
  for(uint16_t i=startPixel; i <stopPixel; i++) {
    uint16_t tmpPixel = 
      (_strip[i - bytesPerPixel] >> 2) +
      _strip[i] +
      (_strip_w[i + bytesPerPixel] >> 2);
    _strip[i] =  tmpPixel > 255 ? 255 : tmpPixel;
  }
  */

  uint8_t size = 1 << SIZE_OPTION;
  if(!_triggered) {
    for(uint16_t i=0; i<max(1, _seg_len/20); i++) {
      if(random8(10) == 0) {
        uint16_t index = _seg->start + random16(_seg_len - size + 1);
        CRGB fastled_col = col_to_crgb(color);
        HsvColor hsv = rgb2hsv(fastled_col.red, fastled_col.green, fastled_col.blue);
        uint8_t bright = (hsv.v > effectSetting->intensity)?effectSetting->intensity:hsv.v;
        setPixelHsv(index, hsv.h, hsv.s, bright);
        SET_CYCLE;
      }
    }
  } else {
    for(uint16_t i=0; i<max(1, _seg_len/10); i++) {
      uint16_t index = _seg->start + random16(_seg_len - size + 1);
      CRGB fastled_col = col_to_crgb(color);
      HsvColor hsv = rgb2hsv(fastled_col.red, fastled_col.green, fastled_col.blue);
      uint8_t bright = (hsv.v > effectSetting->intensity)?effectSetting->intensity:hsv.v;
      setPixelHsv(index, hsv.h, hsv.s, bright);
      SET_CYCLE;
    }
  }

  return (_seg->speed / 16);
}

/*
 * Firework sparks.
 */
uint16_t WS2812FX::mode_fireworks(void) {
  uint32_t color = COLOR_BLACK;
  do { // randomly choose a non-BLACK color from the colors array
    color = _seg->colors[random8(MAX_NUM_COLORS)];
  } while (color == COLOR_BLACK);
  return fireworks(color);
}

/*
 * Random colored firework sparks.
 */
uint16_t WS2812FX::mode_fireworks_random(void) {
  return fireworks(color_wheel(random8()));
}



uint16_t WS2812FX::candle(bool multi)
{
  if (multi)
  {
    //allocate segment data
    uint16_t dataSize = (effectSetting->numLeds -1) *3;
    // uint16_t dataSize;
    // dataSize = (effectSetting->segment)?(effectSetting->numLeds -1) *3:(_seg_len -1) *3; //max. 1365 pixels (ESP8266)

    if (!_seg_rt->allocateData(dataSize)) return candle(false); //allocation failed
  }

  //max. flicker range controlled by intensity
  uint8_t valrange = _seg->intensity;
  uint8_t rndval = valrange >> 1; //max 127

  //step (how much to move closer to target per frame) coarsely set by speed
  uint8_t speedFactor = 4;
  if (_seg->speed > 252) { //epilepsy
    speedFactor = 1;
  } else if (_seg->speed > 99) { //regular candle (mode called every ~25 ms, so 4 frames to have a new target every 100ms)
    speedFactor = 2;
  } else if (_seg->speed > 49) { //slower fade
    speedFactor = 3;
  } //else 4 (slowest)

  uint16_t numCandles = (multi) ? _seg_len : 1;

  for (uint16_t i = 0; i < numCandles; i++)
  {
    uint16_t d = 0; //data location

    uint8_t s = _seg_rt->aux_param, s_target = _seg_rt->aux_param2, fadeStep = _seg_rt->counter_mode_step;
    if (i > 0) {
      d = (i-1) *3;
      s = _seg_rt->data[d]; 
      s_target = _seg_rt->data[d+1]; 
      fadeStep = _seg_rt->data[d+2];
    }
    if (fadeStep == 0) { //init vals
      s = 128; 
      s_target = 130 + random8(4); 
      fadeStep = 1;
    }

    bool newTarget = false;
    if (s_target > s) { //fade up
      s = qadd8(s, fadeStep);
      if (s >= s_target) newTarget = true;
    } else {
      s = qsub8(s, fadeStep);
      if (s <= s_target) newTarget = true;
    }

    if (newTarget) {
      s_target = random8(rndval) + random8(rndval); //between 0 and rndval*2 -2 = 252
      if (s_target < (rndval >> 1)) s_target = (rndval >> 1) + random8(rndval);
      uint8_t offset = (255 - valrange);
      s_target += offset;

      uint8_t dif = (s_target > s) ? s_target - s : s - s_target;
    
      fadeStep = dif >> speedFactor;
      if (fadeStep == 0) fadeStep = 1;
    }

     if (i > 0) {
      setPixelColor(_seg->start+i, color_blend(SEGCOLOR(1), color_from_palette(_seg->start+i, true, PALETTE_SOLID_WRAP, 0), s));

      _seg_rt->data[d] = s; _seg_rt->data[d+1] = s_target; _seg_rt->data[d+2] = fadeStep;
    } else {
      for (uint16_t j = _seg->start; j < _seg->stop+1; j++) {
        uint32_t pixelPos = (IS_REVERSE)? (_seg_len-1) - j:j;
        setPixelColor(pixelPos, color_blend(SEGCOLOR(1), color_from_palette(pixelPos, true, PALETTE_SOLID_WRAP, 0), s));
      }

      _seg_rt->aux_param = s; _seg_rt->aux_param2 = s_target; _seg_rt->counter_mode_step = fadeStep;
    }
  }

  return FRAMETIME;
}

uint16_t WS2812FX::mode_candle()
{
  return candle(false);
}


uint16_t WS2812FX::mode_candle_multi()
{
  return candle(true);
}


/*
 * Fire flicker function
 */
uint16_t WS2812FX::fire_flicker(int rev_intensity) {
  byte w = (_seg->colors[0] >> 24) & 0xFF;
  byte r = (_seg->colors[0] >> 16) & 0xFF;
  byte g = (_seg->colors[0] >>  8) & 0xFF;
  byte b = (_seg->colors[0]        & 0xFF);
  byte lum = max(w, max(r, max(g, b))) / rev_intensity;
  for(uint16_t i=_seg->start; i <= _seg->stop; i++) {
    int flicker = random8(lum);
    setPixelColor(i, max(r - flicker, 0), max(g - flicker, 0), max(b - flicker, 0), max(w - flicker, 0));
  }

  SET_CYCLE;
  return (_seg->speed / _seg_len);
}

/*
 * Random flickering.
 */
uint16_t WS2812FX::mode_fire_flicker(void) {
  return fire_flicker(3);
}

/*
* Random flickering, less intensity.
*/
uint16_t WS2812FX::mode_fire_flicker_soft(void) {
  return fire_flicker(6);
}

/*
* Random flickering, more intensity.
*/
uint16_t WS2812FX::mode_fire_flicker_intense(void) {
  return fire_flicker(1);
}

//based on https://gist.github.com/kriegsman/5408ecd397744ba0393e
uint16_t WS2812FX::mode_colortwinkle()
{
  uint16_t dataSize = (_seg_len+7) >> 3; //1 bit per LED
  if (!_seg_rt->allocateData(dataSize)) return mode_pride(); //allocation failed
  // return mode_pride();
  CRGB fastled_col, prev;
  fract8 fadeUpAmount = 8 + (_seg->fade/4), fadeDownAmount = 5 + (_seg->fade/7);
  for (uint16_t i = _seg->start; i < _seg->stop+1; i++) {

    uint8_t pixelPos = i;

    if (effectSetting->isTw)  fastled_col = col_to_crgb(getPixelColor(_strip_w, pixelPos));
    else                      fastled_col = col_to_crgb(getPixelColor(_strip, pixelPos));

    prev = fastled_col;
    uint16_t index = pixelPos >> 3;
    uint8_t  bitNum = pixelPos & 0x07;
    bool fadeUp = bitRead(_seg_rt->data[index], bitNum);
    
    if (fadeUp) {
      CRGB incrementalColor = fastled_col;
      incrementalColor.nscale8_video( fadeUpAmount);
      fastled_col += incrementalColor;

      if (fastled_col.red == 255 || fastled_col.green == 255 || fastled_col.blue == 255) {
        bitWrite(_seg_rt->data[index], bitNum, false);
      }
      setPixelColor(pixelPos, fastled_col.red, fastled_col.green, fastled_col.blue);

      uint32_t getPixel;
      if (effectSetting->isTw)  getPixel = getPixelColor(_strip_w,pixelPos);
      else                      getPixel = getPixelColor(_strip,  pixelPos);
      if (col_to_crgb(getPixel) == prev) //fix "stuck" pixels
      {
        fastled_col += fastled_col;
        setPixelColor(pixelPos, fastled_col.red, fastled_col.green, fastled_col.blue);
      }
    } else {
      fastled_col.nscale8( 255 - fadeDownAmount);
      setPixelColor(pixelPos, fastled_col.red, fastled_col.green, fastled_col.blue);
    }
  }

  for (uint16_t j = 0; j <= _seg_len / 50; j++)
  {
    if (random8() <= _seg->intensity) {
      for (uint8_t times = 0; times < 5; times++) {
        int i = random(_seg->start, _seg->stop+1);
        uint32_t pixelPos = i;

        uint32_t getPixel;
        if (effectSetting->isTw)  getPixel = getPixelColor(_strip_w,  pixelPos);
        else                      getPixel = getPixelColor(_strip,    pixelPos);
        if(getPixel == 0) {
          fastled_col = ColorFromPalette(currentPalette, random8(), 64, NOBLEND);
          uint16_t index = pixelPos >> 3;
          uint8_t  bitNum = pixelPos & 0x07;
          bitWrite(_seg_rt->data[index], bitNum, true);
          setPixelColor(pixelPos, fastled_col.red, fastled_col.green, fastled_col.blue);
          break; //only spawn 1 new pixel per frame per 50 LEDs
        }
      }
    }
  }
  return (_seg->speed / _seg_len);
}




//Water ripple
//propagation velocity from speed
//drop rate from intensity

//4 bytes
typedef struct Ripple {
  uint8_t state;
  uint8_t color;
  uint16_t pos;
} ripple;

#ifdef ESP8266
  #define MAX_RIPPLES   56
#else
  #define MAX_RIPPLES  100
#endif
uint16_t WS2812FX::ripple_base(bool rainbow)
{
  uint16_t maxRipples = min(1 + (_seg_len >> 2), MAX_RIPPLES);  // 56 max for 16 segment ESP8266
  uint16_t dataSize = sizeof(ripple) * maxRipples;

  if (!_seg_rt->allocateData(dataSize)) return mode_static(); //allocation failed
 
  Ripple* ripples = reinterpret_cast<Ripple*>(_seg_rt->data);

  // ranbow background or chosen background, all very dim.
  if (rainbow) {
    if (_seg_rt->counter_mode_call ==0) {
      _seg_rt->aux_param = random8();
      _seg_rt->aux_param2 = random8();
    }
    if (_seg_rt->aux_param == _seg_rt->aux_param2) {
      _seg_rt->aux_param2 = random8();
    }
    else if (_seg_rt->aux_param2 > _seg_rt->aux_param) {
      _seg_rt->aux_param++;
    } else {
      _seg_rt->aux_param--;
    }
    // fill();
    CRGB c = col_to_crgb(color_blend(color_wheel(_seg_rt->aux_param),COLOR_BLACK,235));
    if (effectSetting->isTw)  ledsSet::fill(_strip_w, c, _seg->start, _seg->stop+1);
    else                      ledsSet::fill(_strip,   c, _seg->start, _seg->stop+1);
  } else {
    // fill(SEGCOLOR(1));
    if (effectSetting->isTw)  ledsSet::fill(_strip_w, _seg->colors[1], _seg->start, _seg->stop+1);
    else                      ledsSet::fill(_strip,   _seg->colors[1], _seg->start, _seg->stop+1);    
  }
  
  //draw wave
  for (uint16_t i = 0; i < maxRipples; i++)
  {
    uint16_t ripplestate = ripples[i].state;
    if (ripplestate)
    {
      uint8_t rippledecay = (_seg->speed >> 4) +1; //faster decay if faster propagation
      uint16_t rippleorigin = ripples[i].pos;
      uint32_t col = color_from_palette(ripples[i].color, false, false, 255);
      uint16_t propagation = ((ripplestate/rippledecay -1) * _seg->speed);
      int16_t propI = propagation >> 8;
      uint8_t propF = propagation & 0xFF;
      int16_t left = rippleorigin - propI -1;
      uint8_t amp = (ripplestate < 17) ? triwave8((ripplestate-1)*8) : map(ripplestate,17,255,255,2);

      for (int16_t v = left; v < left +4; v++)
      {
        uint8_t mag = scale8(cubicwave8((propF>>2)+(v-left)*64), amp);
        if (v < _seg_len && v >= 0)
        {
          uint32_t color ;
          if (effectSetting->isTw)  color = getPixelColor(_strip_w, v); // current color
          else                      color = getPixelColor(_strip,   v); // current color          
          setPixelColor(v, color_blend(color, col, mag));
        }
        int16_t w = left + propI*2 + 3 -(v-left);
        if (w < _seg_len && w >= 0)
        {
          uint32_t color;
          if (effectSetting->isTw)  color = getPixelColor(_strip_w, w); // current color
          else                      color = getPixelColor(_strip,   w); // current color
          setPixelColor(w, color_blend(color, col, mag));
        }
      }  
      ripplestate += rippledecay;
      ripples[i].state = (ripplestate > 254) ? 0 : ripplestate;
    } else //randomly create new wave
    {
      if (random16(IBN + 10000) <= _seg->intensity)
      {
        ripples[i].state = 1;
        ripples[i].pos = random16(_seg_len);
        ripples[i].color = random8(); //color
      }
    }
  }
  return (_seg->speed / _seg_len);
}
#undef MAX_RIPPLES

uint16_t WS2812FX::mode_ripple(void) {
  return ripple_base(true);
}


uint16_t WS2812FX::mode_noise16_1()
{
  uint16_t scale = 320;                                      // the "zoom factor" for the noise
  CRGB fastled_col;
  _seg_rt->step += (1 + _seg->speed/16);

  for (uint16_t i = _seg->start; i < _seg->stop + 1; i++) {
    uint16_t  pixelPos = i;

    uint16_t shift_x = beatsin8(11);                           // the x position of the noise field swings @ 17 bpm
    uint16_t shift_y = _seg_rt->step/42;             // the y position becomes slowly incremented


    uint16_t real_x = (pixelPos + shift_x) * scale;                  // the x position of the noise field swings @ 17 bpm
    uint16_t real_y = (pixelPos + shift_y) * scale;                  // the y position becomes slowly incremented
    uint32_t real_z = _seg_rt->step;                          // the z position becomes quickly incremented

    uint8_t noise = inoise16(real_x, real_y, real_z) >> 8;   // get the noise data and scale it down

    uint8_t index = sin8(noise * 3);                         // map LED color based on noise data

    fastled_col = ColorFromPalette(currentPalette, index, 255, LINEARBLEND);   // With that value, look up the 8 bit colour palette value and assign it to the current LED.
    setPixelColor(pixelPos, fastled_col.red, fastled_col.green, fastled_col.blue);
  }

  return (_seg->speed / _seg_len);
}
uint16_t WS2812FX::mode_noise16_2()
{
  uint16_t scale = 1000;                                       // the "zoom factor" for the noise
  CRGB fastled_col;
  _seg_rt->step += (1 + (_seg->speed >> 1));

  for (uint16_t i = _seg->start; i < _seg->stop + 1; i++) {
    uint16_t  pixelPos = i;

    uint16_t shift_x = _seg_rt->step >> 6;                         // x as a function of time

    uint32_t real_x = (pixelPos + shift_x) * scale;                  // calculate the coordinates within the noise field

    uint8_t noise = inoise16(real_x, 0, 4223) >> 8;    // get the noise data and scale it down

    uint8_t index = sin8(noise * 3);                          // map led color based on noise data

    fastled_col = ColorFromPalette(currentPalette, index, noise, LINEARBLEND);   // With that value, look up the 8 bit colour palette value and assign it to the current LED.
    setPixelColor(pixelPos, fastled_col.red, fastled_col.green, fastled_col.blue);
  }

   return (_seg->speed / _seg_len);
}
uint16_t WS2812FX::mode_noise16_3()
{
  uint16_t scale = 800;                                       // the "zoom factor" for the noise
  CRGB fastled_col;
  _seg_rt->step += (1 + _seg->speed);

  for (uint16_t i = _seg->start; i < _seg->stop + 1; i++) {
    uint16_t  pixelPos = i;

    uint16_t shift_x = 4223;                                  // no movement along x and y
    uint16_t shift_y = 1234;

    uint32_t real_x = (pixelPos + shift_x) * scale;                  // calculate the coordinates within the noise field
    uint32_t real_y = (pixelPos + shift_y) * scale;                  // based on the precalculated positions
    uint32_t real_z = _seg_rt->step*8;  

    uint8_t noise = inoise16(real_x, real_y, real_z) >> 8;    // get the noise data and scale it down

    uint8_t index = sin8(noise * 3);                          // map led color based on noise data

    fastled_col = ColorFromPalette(currentPalette, index, noise, LINEARBLEND);   // With that value, look up the 8 bit colour palette value and assign it to the current LED.
    setPixelColor(pixelPos, fastled_col.red, fastled_col.green, fastled_col.blue);
  }

  return (_seg->speed / _seg_len);
}
//https://github.com/aykevl/ledstrip-spark/blob/master/ledstrip.ino
uint16_t WS2812FX::mode_noise16_4()
{
  CRGB fastled_col;
  uint32_t stp = (now * _seg->speed) >> 7;
  for (uint16_t i = _seg->start; i < _seg->stop + 1; i++) {
    uint16_t  pixelPos = i;

    int16_t index = inoise16(uint32_t(pixelPos) << 12, stp);
    fastled_col = ColorFromPalette(currentPalette, index);
    setPixelColor(pixelPos, fastled_col.red, fastled_col.green, fastled_col.blue);
  }
  return (_seg->speed / _seg_len);
}


//  TwinkleFOX by Mark Kriegsman: https://gist.github.com/kriegsman/756ea6dcae8e30845b5a
//
//  TwinkleFOX: Twinkling 'holiday' lights that fade in and out.
//  Colors are chosen from a palette. Read more about this effect using the link above!

// If COOL_LIKE_INCANDESCENT is set to 1, colors will
// fade out slighted 'reddened', similar to how
// incandescent bulbs change color as they get dim down.
#define COOL_LIKE_INCANDESCENT 1


CRGB WS2812FX::twinklefox_one_twinkle(uint32_t ms, uint8_t salt, bool cat)
{
  // Overall twinkle speed (changed)
  uint16_t ticks = ms / _seg_rt->aux_param;
  uint8_t fastcycle8 = ticks;
  uint16_t slowcycle16 = (ticks >> 8) + salt;
  slowcycle16 += sin8(slowcycle16);
  slowcycle16 = (slowcycle16 * 2053) + 1384;
  uint8_t slowcycle8 = (slowcycle16 & 0xFF) + (slowcycle16 >> 8);
  
  // Overall twinkle density.
  // 0 (NONE lit) to 8 (ALL lit at once).
  // Default is 5.
  uint8_t twinkleDensity = (_seg->intensity >> 5) +1;

  uint8_t bright = 0;
  if (((slowcycle8 & 0x0E)/2) < twinkleDensity) {
    uint8_t ph = fastcycle8;
    // This is like 'triwave8', which produces a
    // symmetrical up-and-down triangle sawtooth waveform, except that this
    // function produces a triangle wave with a faster attack and a slower decay
    if (cat) //twinklecat, variant where the leds instantly turn on
    {
      bright = 255 - ph;
    } else { //vanilla twinklefox
      if (ph < 86) {
      bright = ph * 3;
      } else {
        ph -= 86;
        bright = 255 - (ph + (ph/2));
      }
    }
  }

  uint8_t hue = slowcycle8 - salt;
  CRGB c;
  if (bright > 0) {
    c = ColorFromPalette(currentPalette, hue, bright, NOBLEND);
    if(COOL_LIKE_INCANDESCENT == 1) {
      // This code takes a pixel, and if its in the 'fading down'
      // part of the cycle, it adjusts the color a little bit like the
      // way that incandescent bulbs fade toward 'red' as they dim.
      if (fastcycle8 >= 128) 
      {
        uint8_t cooling = (fastcycle8 - 128) >> 4;
        c.g = qsub8(c.g, cooling);
        c.b = qsub8(c.b, cooling * 2);
      }
    }
  } else {
    c = CRGB::Black;
  }
  return c;
}

//  This function loops over each pixel, calculates the
//  adjusted 'clock' that this pixel should use, and calls
//  "CalculateOneTwinkle" on each pixel.  It then displays
//  either the twinkle color of the background color,
//  whichever is brighter.
uint16_t WS2812FX::twinklefox_base(bool cat)
{
  // "PRNG16" is the pseudorandom number generator
  // It MUST be reset to the same starting value each time
  // this function is called, so that the sequence of 'random'
  // numbers that it generates is (paradoxically) stable.
  uint16_t PRNG16 = 11337;

  // Calculate speed
  if (_seg->fade > 100) _seg_rt->aux_param = 3 + ((255 - _seg->fade) >> 3);
  else _seg_rt->aux_param = 22 + ((100 - _seg->fade) >> 1);

  // Set up the background color, "bg".
  CRGB bg;
  bg = col_to_crgb(SEGCOLOR(1));
  uint8_t bglight = bg.getAverageLight();
  if (bglight > 64) {
    bg.nscale8_video(16); // very bright, so scale to 1/16th
  } else if (bglight > 16) {
    bg.nscale8_video(64); // not that bright, so scale to 1/4th
  } else {
    bg.nscale8_video(86); // dim, scale to 1/3rd.
  }

  uint8_t backgroundBrightness = bg.getAverageLight();

  for (uint16_t i = _seg->start; i < _seg->stop+1; i++) {
  
    PRNG16 = (uint16_t)(PRNG16 * 2053) + 1384; // next 'random' number
    uint16_t myclockoffset16= PRNG16; // use that number as clock offset
    PRNG16 = (uint16_t)(PRNG16 * 2053) + 1384; // next 'random' number
    // use that number as clock speed adjustment factor (in 8ths, from 8/8ths to 23/8ths)
    uint8_t myspeedmultiplierQ5_3 =  ((((PRNG16 & 0xFF)>>4) + (PRNG16 & 0x0F)) & 0x0F) + 0x08;
    uint32_t myclock30 = (uint32_t)((millis() * myspeedmultiplierQ5_3) >> 3) + myclockoffset16;
    uint8_t  myunique8 = PRNG16 >> 8; // get 'salt' value for this pixel

    // We now have the adjusted 'clock' for this pixel, now we call
    // the function that computes what color the pixel should be based
    // on the "brightness = f( time )" idea.
    CRGB c = twinklefox_one_twinkle(myclock30, myunique8, cat);

    uint8_t cbright = c.getAverageLight();
    int16_t deltabright = cbright - backgroundBrightness;

    uint32_t pixelPos = (IS_REVERSE)? (_seg_len-1) - i:i;

    if (deltabright >= 32 || (!bg)) {
      // If the new pixel is significantly brighter than the background color,
      // use the new color.
      setPixelColor(pixelPos, c.red, c.green, c.blue);
    } else if (deltabright > 0) {
      // If the new pixel is just slightly brighter than the background color,
      // mix a blend of the new color and the background color
      setPixelColor(pixelPos, color_blend(crgb_to_col(bg), crgb_to_col(c), deltabright * 8));
    } else {
      // if the new pixel is not at all brighter than the background color,
      // just use the background color.
      setPixelColor(pixelPos, bg.r, bg.g, bg.b);
    }
  }
  return (_seg->speed / _seg_len);
}

uint16_t WS2812FX::mode_twinklefox()
{
  return twinklefox_base(false);
}

uint16_t WS2812FX::mode_twinklecat()
{
  return twinklefox_base(true);
}

template<class X>
void _colorwavesEx(    X * sLedarray, uint16_t numleds, CRGBPalette16& sPalette, uint16_t from, uint16_t to, uint8_t bpm, uint8_t intensity, boolean reverse, bool effectload, bool sRverseStart, uint32_t now) {
  static uint16_t sPseudotime = 0;
  static uint16_t sLastMillis = 0;
  static uint16_t sHue16 = 0;
  if (effectload){
    sPseudotime = 0;
    sLastMillis = 0;
    sHue16 = 0;      
  }
  // uint16_t    duration = 10 + bpm;
  uint8_t     brightdepth           = beatsin88( 341, 96, 224);
  uint16_t    brightnessthetainc16  = beatsin88( 203, (25 * 256), (40 * 256));
  uint8_t     msmultiplier          = beatsin88(147, 23, 60);

  uint16_t    hue16                 = sHue16;//gHue * 256;
  uint16_t    hueinc16              = beatsin88(113, 300, 1500);
  // uint16_t    hueinc16              = beatsin88(113, 60, 300)*intensity*10/255; 

  uint16_t    ms                    = now;
  uint16_t    deltams               = ms - sLastMillis ;
              sLastMillis           = ms;
              // sPseudotime           += duration * msmultiplier;
              sPseudotime           += deltams * msmultiplier;
              sHue16                += deltams * beatsin88( 400, 5, 9);
  uint16_t    brightnesstheta16     = sPseudotime;
  CRGB        fastled_col;

  for(  uint16_t i = from; i < to+1; i++) {

              hue16                 += hueinc16;
  uint8_t     hue8                  = hue16 / 256;
  uint16_t    h16_128               = hue16 >> 7;

  if ( h16_128 & 0x100) {
              hue8                  = 255 - (h16_128 >> 1);
  } else {
              hue8                  = h16_128 >> 1;
  }

            brightnesstheta16       += brightnessthetainc16;
  uint16_t  b16                     = sin16( brightnesstheta16  ) + 32768;

  uint16_t  bri16                   = (uint32_t)((uint32_t)b16 * (uint32_t)b16) / 65536;
  uint8_t   bri8                    = (uint32_t)(((uint32_t)bri16) * brightdepth) / 65536;
            bri8                    += (255 - brightdepth);

  uint8_t   index                   = hue8;
  //        index                   = triwave8( index);
  //        index                   = scale8( index, 240);
  uint32_t  pixelPos                = (reverse)? (from+to) - i:i;
  if (!sRverseStart) pixelPos = (from+to) - pixelPos;

  CRGB      newcolor                = ColorFromPalette( sPalette, index, map(bri8,0,255,0, intensity));
            fastled_col             = WS2812FX_col_to_crgb(getPixelColor(sLedarray, pixelPos));
  nblend(fastled_col, newcolor, 128);
  WS2812FX_setPixelColor(sLedarray, pixelPos, fastled_col.red, fastled_col.green, fastled_col.blue, 0);
  }         
} 

// ColorWavesWithPalettes by Mark Kriegsman: https://gist.github.com/kriegsman/8281905786e8b2632aeb
// This function draws color waves with an ever-changing,
// widely-varying set of parameters, using a color palette.
uint16_t WS2812FX::mode_colorwaves()
{
    if (effectSetting->isTw)  _colorwavesEx<CRGBW>(_strip_w,  _seg_len, currentPalette, _seg->start, _seg->stop, 0, _seg->intensity, IS_REVERSE, effectLoad, _reverseStart, now);
    else                      _colorwavesEx<CRGB>(_strip,     _seg_len, currentPalette, _seg->start, _seg->stop, 0, _seg->intensity, IS_REVERSE, effectLoad, _reverseStart, now);
    if (effectLoad)effectLoad=false;    
    return (_seg->speed / _seg_len);
}

template<class X>
void _pride( X * sLedarray, uint16_t numleds, uint16_t from, uint16_t to, uint8_t intensity, boolean sReverse, uint8_t speed, bool effectload, bool sRverseStart, uint32_t now) {
    static uint16_t sPseudotime = 0;
    static uint16_t sLastMillis = 0;
    static uint16_t sHue16 = 0;
    // static uint8_t gHue = 0;
    // if (effectload){
    //   sPseudotime = 0;
    //   sLastMillis = 0;
    //   sHue16 = 0;      
    // }
    uint8_t   sat8                    = beatsin88( 87, 220, 250);
    uint8_t   brightdepth             = beatsin88( 341, 96, 224);
    uint16_t  brightnessthetainc16    = beatsin88( 203, (25 * 256), (40 * 256));
    uint8_t   msmultiplier            = beatsin88(147, 23, 60);

    uint16_t  hue16                   = sHue16;//gHue * 256;
    uint16_t  hueinc16                = beatsin88(113, 1, 3000);

    uint16_t  ms                      = now;
    uint16_t  deltams                 = ms - sLastMillis ;

    sLastMillis  = ms;
    sPseudotime += deltams * msmultiplier;
    sHue16      += deltams * beatsin88( 400, 5, 9);

    uint16_t  brightnesstheta16      = sPseudotime;

    CRGB      fastled_col;

    for(  uint16_t i = from; i < to+1; i++) {
              hue16             += hueinc16;
    uint8_t   hue8              = hue16 / 256;

              brightnesstheta16 += brightnessthetainc16;
    uint16_t  b16               = sin16(brightnesstheta16) + 32768;

    uint16_t  bri16             = (uint32_t) ((uint32_t) b16 * (uint32_t) b16) / 65536;
    uint8_t   bri8              = (uint32_t) (((uint32_t) bri16) * brightdepth) / 65536;
              bri8              += (255 - brightdepth);

    // uint32_t  pixelPos        = i;
    // uint32_t  pixelPos                = (!reverse)? (from+to) - i:i;
    uint16_t pixelPos = (sReverse)? (from+to) - i:i;
    if (!sRverseStart) pixelPos = (from+to) - pixelPos;

    CHSV      newcolor        = CHSV(hue8, sat8, map(bri8, 0, 255, 0, intensity));
              fastled_col     = WS2812FX_col_to_crgb(getPixelColor(sLedarray, pixelPos));

    nblend(fastled_col, newcolor, 128);
    WS2812FX_setPixelColor(sLedarray, pixelPos, fastled_col.red, fastled_col.green, fastled_col.blue, 0);            
    } 
}
/*
uint16_t WS2812FX::mode_pride(void)
{
  uint16_t duration = 10 + _seg->speed;
  uint16_t sPseudotime = _seg_rt->step;
  uint16_t sHue16 = _seg_rt->aux_param;

  uint8_t sat8 = beatsin88( 87, 220, 250);
  uint8_t brightdepth = beatsin88( 341, 96, 224);
  uint16_t brightnessthetainc16 = beatsin88( 203, (25 * 256), (40 * 256));
  uint8_t msmultiplier = beatsin88(147, 23, 60);

  uint16_t hue16 = sHue16;//gHue * 256;
  uint16_t hueinc16 = beatsin88(113, 1, 3000);

  sPseudotime += duration * msmultiplier;
  sHue16 += duration * beatsin88( 400, 5,9);
  uint16_t brightnesstheta16 = sPseudotime;
  CRGB fastled_col;

  for (uint16_t i = _seg->start; i < _seg->stop+1; i++) {
    uint32_t pixelPos = (IS_REVERSE)? (_seg->start+_seg->stop) - i:i;
    if (!_reverseStart) pixelPos = (_seg->start+_seg->stop) - pixelPos;

    hue16 += hueinc16;
    uint8_t hue8 = hue16 >> 8;

    brightnesstheta16  += brightnessthetainc16;
    uint16_t b16 = sin16( brightnesstheta16  ) + 32768;

    uint16_t bri16 = (uint32_t)((uint32_t)b16 * (uint32_t)b16) / 65536;
    uint8_t bri8 = (uint32_t)(((uint32_t)bri16) * brightdepth) / 65536;
    bri8 += (255 - brightdepth);

    CRGB newcolor = CHSV( hue8, sat8, bri8);
    uint32_t color;
    if (effectSetting->isTw)  color = getPixelColor(_strip_w, pixelPos); // current color
    else                      color = getPixelColor(_strip,   pixelPos); // current color    
    fastled_col = col_to_crgb(color);

    nblend(fastled_col, newcolor, 64);
    setPixelColor(pixelPos, fastled_col.red, fastled_col.green, fastled_col.blue);
  }
  _seg_rt->step = sPseudotime;
  _seg_rt->aux_param = sHue16;
  return (_seg->speed / _seg_len);
}
*/
uint16_t WS2812FX::mode_pride()
{
    if (effectSetting->isTw)  _pride<CRGBW> (_strip_w,  _seg_len, _seg->start, _seg->stop, _seg->intensity, IS_REVERSE, 0, effectLoad, _reverseStart, now);
    else                      _pride<CRGB>  (_strip,    _seg_len, _seg->start, _seg->stop, _seg->intensity, IS_REVERSE, 0, effectLoad, _reverseStart, now);

    if (effectLoad)effectLoad=false;

    return (_seg->speed / _seg_len);
}

uint16_t WS2812FX::mode_fire_2012()
{
  uint32_t it = nowUp >> 5; //div 32

  if (!_seg_rt->allocateData(_seg_len)) return mode_static(); //allocation failed
  
  byte* heat = _seg_rt->data;

  if (it != _seg_rt->step)
  {
    // Step 1.  Cool down every cell a little
    for (uint16_t i = 0; i < _seg_len; i++) {
      _seg_rt->data[i] = qsub8(heat[i],  random8(0, (((20 + _seg->speed /3) * 10) / _seg_len) + 2) );
    }
  
    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for (uint16_t k= _seg_len -1; k > 1; k--) {
      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
    }
    
    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if (random8() <= _seg->intensity) {
      uint8_t y = random8(7);
      if (y < _seg_len) heat[y] = qadd8(heat[y], random8(160,255));
    }
    _seg_rt->step = it;
  }

  // Step 4.  Map from heat cells to LED colors
  for (uint16_t j = 0; j < _seg_len; j++) {
    CRGB color = ColorFromPalette(currentPalette, MIN(heat[j],240), 255, LINEARBLEND);
    setPixelColor(j, color.red, color.green, color.blue);
  }
  return FRAMETIME;
}



//  "Pacifica"
//  Gentle, blue-green ocean waves.
//  December 2019, Mark Kriegsman and Mary Corey March.
//  For Dan.
//
//
// In this animation, there are four "layers" of waves of light.  
//
// Each layer moves independently, and each is scaled separately.
//
// All four wave layers are added together on top of each other, and then 
// another filter is applied that adds "whitecaps" of brightness where the 
// waves line up with each other more.  Finally, another pass is taken
// over the led array to 'deepen' (dim) the blues and greens.
//
// The speed and scale and motion each layer varies slowly within independent 
// hand-chosen ranges, which is why the code has a lot of low-speed 'beatsin8' functions
// with a lot of oddly specific numeric ranges.
//
// These three custom blue-green color palettes were inspired by the colors found in
// the waters off the southern coast of California, https://goo.gl/maps/QQgd97jjHesHZVxQ7
//
// Modified for WLED, based on https://github.com/FastLED/FastLED/blob/master/examples/Pacifica/Pacifica.ino
//
uint16_t WS2812FX::mode_pacifica()
{
  uint32_t nowOld = nowUp;

  CRGBPalette16 pacifica_palette_1 = 
    { 0x000507, 0x000409, 0x00030B, 0x00030D, 0x000210, 0x000212, 0x000114, 0x000117, 
      0x000019, 0x00001C, 0x000026, 0x000031, 0x00003B, 0x000046, 0x14554B, 0x28AA50 };
  CRGBPalette16 pacifica_palette_2 = 
    { 0x000507, 0x000409, 0x00030B, 0x00030D, 0x000210, 0x000212, 0x000114, 0x000117, 
      0x000019, 0x00001C, 0x000026, 0x000031, 0x00003B, 0x000046, 0x0C5F52, 0x19BE5F };
  CRGBPalette16 pacifica_palette_3 = 
    { 0x000208, 0x00030E, 0x000514, 0x00061A, 0x000820, 0x000927, 0x000B2D, 0x000C33, 
      0x000E39, 0x001040, 0x001450, 0x001860, 0x001C70, 0x002080, 0x1040BF, 0x2060FF };

  // if (SEGMENT.palette) {
  //   pacifica_palette_1 = currentPalette;
  //   pacifica_palette_2 = currentPalette;
  //   pacifica_palette_3 = currentPalette;
  // }

  // Increment the four "color index start" counters, one for each wave layer.
  // Each is incremented at a different speed, and the speeds vary over time.
  uint16_t sCIStart1 = _seg_rt->aux_param, sCIStart2 = _seg_rt->aux_param3, sCIStart3 = _seg_rt->step, sCIStart4 = _seg_rt->step >> 16;
  //static uint16_t sCIStart1, sCIStart2, sCIStart3, sCIStart4;
  //uint32_t deltams = 26 + (_seg->speed >> 3);
  uint32_t deltams = (FRAMETIME >> 2) + ((FRAMETIME * _seg->speed) >> 7);
  uint64_t deltat = (nowUp >> 2) + ((nowUp * _seg->speed) >> 7);
  nowUp = deltat;

  uint16_t speedfactor1 = beatsin16(3, 179, 269);
  uint16_t speedfactor2 = beatsin16(4, 179, 269);
  uint32_t deltams1 = (deltams * speedfactor1) / 256;
  uint32_t deltams2 = (deltams * speedfactor2) / 256;
  uint32_t deltams21 = (deltams1 + deltams2) / 2;
  sCIStart1 += (deltams1 * beatsin88(1011,10,13));
  sCIStart2 -= (deltams21 * beatsin88(777,8,11));
  sCIStart3 -= (deltams1 * beatsin88(501,5,7));
  sCIStart4 -= (deltams2 * beatsin88(257,4,6));
  _seg_rt->aux_param = sCIStart1; _seg_rt->aux_param3 = sCIStart2;
  _seg_rt->step = sCIStart4; _seg_rt->step = (_seg_rt->step << 16) + sCIStart3;

  // Clear out the LED array to a dim background blue-green
  //fill(132618);

  uint8_t basethreshold = beatsin8( 9, 55, 65);
  uint8_t wave = beat8( 7 );
  
  for (uint16_t i = _seg->start; i < _seg->stop+1; i++) {
    CRGB c = CRGB(2, 6, 10);
    uint32_t pixelPos = (IS_REVERSE)? (_seg->stop-_seg->start) - i:i;
    // Render each of four layers, with different scales and speeds, that vary over time
    c += pacifica_one_layer(pixelPos, pacifica_palette_1, sCIStart1, beatsin16(3, 11 * 256, 14 * 256), beatsin8(10, 70, 130), 0-beat16(301));
    c += pacifica_one_layer(pixelPos, pacifica_palette_2, sCIStart2, beatsin16(4,  6 * 256,  9 * 256), beatsin8(17, 40,  80),   beat16(401));
    c += pacifica_one_layer(pixelPos, pacifica_palette_3, sCIStart3,                         6 * 256 , beatsin8(9, 10,38)   , 0-beat16(503));
    c += pacifica_one_layer(pixelPos, pacifica_palette_3, sCIStart4,                         5 * 256 , beatsin8(8, 10,28)   ,   beat16(601));
    
    // Add extra 'white' to areas where the four layers of light have lined up brightly
    uint8_t threshold = scale8( sin8( wave), 20) + basethreshold;
    wave += 7;
    uint8_t l = c.getAverageLight();
    if (l > threshold) {
      uint8_t overage = l - threshold;
      uint8_t overage2 = qadd8(overage, overage);
      c += CRGB(overage, overage2, qadd8(overage2, overage2));
    }

    //deepen the blues and greens
    c.blue  = scale8(c.blue,  145); 
    c.green = scale8(c.green, 200); 
    c |= CRGB( 2, 5, 7);

    setPixelColor(pixelPos, c.red, c.green, c.blue);
  }

  nowUp = nowOld;
  return FRAMETIME;
}

// Add one layer of waves into the led array
CRGB WS2812FX::pacifica_one_layer(uint16_t i, CRGBPalette16& p, uint16_t cistart, uint16_t wavescale, uint8_t bri, uint16_t ioff)
{
  uint16_t ci = cistart;
  uint16_t waveangle = ioff;
  uint16_t wavescale_half = (wavescale >> 1) + 20;
  
  waveangle += ((120 + _seg->intensity) * i); //original 250 * i
  uint16_t s16 = sin16(waveangle) + 32768;
  uint16_t cs = scale16(s16, wavescale_half) + wavescale_half;
  ci += (cs * i);
  uint16_t sindex16 = sin16(ci) + 32768;
  uint8_t sindex8 = scale16(sindex16, 240);
  return ColorFromPalette(p, sindex8, bri, LINEARBLEND);
}




//each needs 12 bytes
typedef struct Ball {
  unsigned long lastBounceTime;
  float impactVelocity;
  float height;
} ball;

/*
*  Bouncing Balls Effect
*/
uint16_t WS2812FX::mode_bouncing_balls(void) {
  //allocate segment data
  uint16_t maxNumBalls = 16; 
  uint16_t dataSize = sizeof(ball) * maxNumBalls;
  // if (!SEGENV.allocateData(dataSize)) return mode_static(); //allocation failed
  if (!_seg_rt->allocateData(dataSize)) return mode_static(); //allocation failed
  
  Ball* balls = reinterpret_cast<Ball*>(_seg_rt->data);
  
  // number of balls based on intensity setting to max of 7 (cycles colors)
  // non-chosen color is a random color
  uint8_t numBalls = int(((_seg->intensity * (maxNumBalls - 0.8f)) / 255) + 1);
  
  float gravity                           = -9.81; // standard value of gravity
  float impactVelocityStart               = sqrt( -2 * gravity);

  unsigned long time = millis();

  if (_seg_rt->counter_mode_call == 0) {
    for (uint8_t i = 0; i < maxNumBalls; i++) balls[i].lastBounceTime = time;
  }
  
  bool hasCol2 = SEGCOLOR(2);
  // fill(hasCol2 ? BLACK : SEGCOLOR(1));
  if (effectSetting->isTw)  ledsSet::fill(_strip_w, hasCol2 ? COLOR_BLACK : SEGCOLOR(1), _seg->start, _seg->stop);
  else                      ledsSet::fill(_strip,   hasCol2 ? COLOR_BLACK : SEGCOLOR(1), _seg->start, _seg->stop);  

  for (uint8_t i = 0; i < numBalls; i++) {
    float timeSinceLastBounce = (time - balls[i].lastBounceTime)/((255-_seg->speed)*8/256 +1);
    balls[i].height = 0.5 * gravity * pow(timeSinceLastBounce/1000 , 2.0) + balls[i].impactVelocity * timeSinceLastBounce/1000;

    if (balls[i].height < 0) { //start bounce
      balls[i].height = 0;
      //damping for better effect using multiple balls
      float dampening = 0.90 - float(i)/pow(numBalls,2);
      balls[i].impactVelocity = dampening * balls[i].impactVelocity;
      balls[i].lastBounceTime = time;

      if (balls[i].impactVelocity < 0.015) {
        balls[i].impactVelocity = impactVelocityStart;
      }
    }
    
    uint32_t color = SEGCOLOR(0);
    if (_seg->palette) {
      color = color_wheel(i*(256/MAX(numBalls, 8)));
    } else if (hasCol2) {
      color = SEGCOLOR(i % MAX_NUM_COLORS);
    }

    uint16_t pos = round(balls[i].height * (_seg_len - 1));
    setPixelColor(pos, color);
  }

  return FRAMETIME;
}




/*
 * ICU mode
 */
// uint16_t WS2812FX::mode_icu(void) {
//   uint16_t dest = _seg_rt->counter_mode_step & 0xFFFF;
 
//   setPixelColor(_seg->start + dest, _seg->colors[0]);
//   setPixelColor(_seg->start + dest + _seg_len/2, _seg->colors[0]);

//   if(_seg_rt->aux_param3 == dest) { // pause between eye movements
//     if(random8(6) == 0) { // blink once in a while
//       setPixelColor(_seg->start + dest, COLOR_BLACK);
//       setPixelColor(_seg->start + dest + _seg_len/2, COLOR_BLACK);
//       return 200;
//     }
//     _seg_rt->aux_param3 = random16(_seg_len/2);
//     SET_CYCLE;
//     return 1000 + random16(2000);
//   }

//   setPixelColor(_seg->start + dest, COLOR_BLACK);
//   setPixelColor(_seg->start + dest + _seg_len/2, COLOR_BLACK);

//   if(_seg_rt->aux_param3 > _seg_rt->counter_mode_step) {
//     _seg_rt->counter_mode_step++;
//     dest++;
//   } else if (_seg_rt->aux_param3 < _seg_rt->counter_mode_step) {
//     _seg_rt->counter_mode_step--;
//     dest--;
//   }

//   setPixelColor(_seg->start + dest, _seg->colors[0]);
//   setPixelColor(_seg->start + dest + _seg_len/2, _seg->colors[0]);

//   return (_seg->speed / _seg_len);
// }

/*
 * Custom modes
 */


/*
  Blends random colors across palette
  Modified, originally by Mark Kriegsman https://gist.github.com/kriegsman/1f7ccbbfa492a73c015e
*/
uint16_t WS2812FX::mode_blends(void) {

  uint16_t pixelLen = _seg_len > UINT8_MAX ? UINT8_MAX : _seg_len;
  uint16_t dataSize = sizeof(uint32_t) * (pixelLen + 1);  // max segment length of 56 pixels on 16 segment ESP8266
  if (!_seg_rt->allocateData(dataSize)) return mode_static(); //allocation failed
  uint32_t* pixels = reinterpret_cast<uint32_t*>(_seg_rt->data);
  // uint8_t blendSpeed = map(effectSetting->scale, 0, UINT8_MAX, 10, 128);
  uint8_t shift = (now * ((_seg->speed >> 3) +1)) >> 8;

  for (int i = 0; i < pixelLen; i++) {
    CRGB fastled_col = ColorFromPalette(currentPalette, shift + quadwave8((i + 1) * 16), effectSetting->intensity, LINEARBLEND);
    pixels[i] = color_blend(pixels[i], crgb_to_col(fastled_col), effectSetting->scale);
    shift += 3;
  }

  uint16_t offset = 0;
  for (int i = 0; i < _seg_len; i++) {
    setPixelColor(i, pixels[offset++]);
    if (offset > pixelLen) offset = 0;
  }

  return FRAMETIME;
}

/////////////////////////
//     Waveins         //
/////////////////////////
// Uses beatsin8() + phase shifting. By: Andrew Tuline
uint16_t WS2812FX::mode_wavesins(void) {

  for (int i = _seg->start; i < _seg->stop+1; i++) {
    uint8_t bri = sin8(millis()/4 + i * _seg->fade);
    if (bri>_seg->intensity)bri=_seg->intensity;
    uint8_t index = beatsin8(_seg->speed, effectSetting->scale, effectSetting->scale+effectSetting->size, 0, i * (map(effectSetting->variation,0,255,0,32)<<3)); // custom3 is reduced resolution slider
    //SEGMENT.setPixelColor(i, ColorFromPalette(SEGPALETTE, index, bri, LINEARBLEND));
    CRGB fastled_col = ColorFromPalette(currentPalette, index, bri, LINEARBLEND);
    setPixelColor(i, fastled_col.r, fastled_col.g, fastled_col.b);
  }

  return (1000 / 33);
} // mode_waveins()