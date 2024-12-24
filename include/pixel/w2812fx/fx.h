#ifndef _FX_H_
#define _FX_H_

// #include "effect_config.h"
// #ifdef LAMP_FX_NEO

/*
  WS2812FX.h - Library for WS2812 LED effects.

  Harm Aldick - 2016
  www.aldick.org
  FEATURES
    * A lot of blinken modes and counting
    * WS2812FX can be used as drop-in replacement for Adafruit NeoPixel Library
  NOTES
    * Uses the Adafruit NeoPixel library. Get it here:
      https://github.com/adafruit/Adafruit_NeoPixel
  LICENSE
  The MIT License (MIT)
  Copyright (c) 2016  Harm Aldick
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:
  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
  CHANGELOG
  2016-05-28   Initial beta release
  2016-06-03   Code cleanup, minor improvements, new modes
  2016-06-04   2 new fx, fixed setColor (now also resets _mode_color)
  2017-02-02   added external trigger functionality (e.g. for sound-to-light)
*/

#include <Arduino.h>

#include "../../fastled/fastled.h"
#include "../set.h"
#include "../effectcore.h"


#define MAX_NUM_COLORS    3 /* number of colors per segment */

#ifdef ESP8266
#define MAX_SEGMENT_DATA  2048
#else
#define MAX_SEGMENT_DATA  8192
#endif
#define IBN 5100

// #define MODE_COUNT (70)


typedef struct WS2812FX_setting { // 16 bytes
    uint16_t      pos = 0;
    uint16_t      numLeds = 0;
    uint8_t       speed = 100;
    uint8_t       fade = 60;
    uint8_t       intensity = 127;
    uint8_t       scale = 127;
    uint8_t       size = 3;
    uint8_t       variation = 127;
    uint8_t       palette = 1;
    RgbColor      c1;
    RgbColor      c2;
    boolean       reverse;
    boolean       reverseStart;
    boolean       segment;
    CRGBPalette16 pPalette;
    boolean       isTw;

} ws2812fx_setting;
class WS2812FX : public ledsTools {

  public:
    bool              effectLoad  = true;
    bool              _reverseStart  = true;
    uint8_t           _stripPos   = 0;
    CRGBW             * _strip_w;
    CRGB              * _strip;
    ws2812fx_setting  * effectSetting;

    typedef uint16_t (WS2812FX::*mode_ptr)(void);

    mode_ptr _effectMod[1];
    // segment parameters
    typedef struct Segment { // 20 bytes
      bool        reverse;
      uint16_t    start;
      uint16_t    stop;
      uint16_t    speed;
      uint8_t     mode;
      uint8_t     options;
      uint8_t     size;
      uint8_t     variation;
      uint8_t     intensity;
      uint8_t     fade;
      uint8_t     palette = 0;
      uint32_t    colors[MAX_NUM_COLORS];
    } segment;

    typedef struct Segment_runtime { // 16 bytes
      unsigned long   next_time;
      uint32_t        step;
      uint32_t        counter_mode_step;
      uint32_t        counter_mode_call;
      uint8_t         aux_param;   // auxilary param (usually stores a color_wheel index)
      uint8_t         aux_param2;  // auxilary param (usually stores bitwise options)
      uint16_t        aux_param3; // auxilary param (usually stores a segment index)

      byte* data = nullptr;
      bool allocateData(uint16_t len){
        if (data && _dataLen == len) return true; //already allocated
        deallocateData();
        if (WS2812FX::_usedSegmentData + len > MAX_SEGMENT_DATA) return false; //not enough memory
        data = new (std::nothrow) byte[len];
        if (!data) return false; //allocation failed
        WS2812FX::_usedSegmentData += len;
        _dataLen = len;
        memset(data, 0, len);
        return true;
      }
      void deallocateData(){
        delete[] data;
        data = nullptr;
        WS2812FX::_usedSegmentData -= _dataLen;
        _dataLen = 0;
      }
      void reset(){next_time = 0; counter_mode_step = 0; counter_mode_call = 0; aux_param = 0; aux_param2 = 0; deallocateData();}
      private:
        uint16_t _dataLen = 0;
    } segment_runtime;

    WS2812FX(uint16_t n, CRGBW * targetStrip, WS2812FX_setting * setting);
    WS2812FX(uint16_t n, CRGB * targetStrip, WS2812FX_setting * setting);
    ~WS2812FX();

    void
      resetSegments(void),
      resetSegmentRuntimes(void),
      resetSegmentRuntime(uint8_t);

  private:


    void
      print(),
      setup(uint16_t n, WS2812FX_setting * setting),
      init(void),
      start(void),
      stop(void),
      pause(void),
      resume(void),
      strip_off(void),
      fade_out(void),
      fade_out(uint32_t),
      // setMode(uint8_t m),
      // setMode(uint8_t seg, uint8_t m),
      setOptions(uint8_t seg, uint8_t o),
      setSpeed(uint8_t seg, uint16_t s),
      increaseSpeed(uint8_t s),
      decreaseSpeed(uint8_t s),
      setColor(uint8_t r, uint8_t g, uint8_t b),
      setColor(uint8_t r, uint8_t g, uint8_t b, uint8_t w),
      setColor(uint32_t c),
      setColor(uint8_t seg, uint32_t c),
      setColors(uint8_t seg, uint32_t* c),
      // setBrightness(uint8_t b),
      increaseBrightness(uint8_t s),
      decreaseBrightness(uint8_t s),
      setLength(uint16_t b),
      increaseLength(uint16_t s),
      decreaseLength(uint16_t s),
      trigger(void),
      setCycle(void),
      setNumSegments(uint8_t n),
      setSegment(uint8_t n, uint16_t start, uint16_t stop, uint8_t mode, uint32_t color,          uint16_t speed, bool reverse),
      setSegment(uint8_t n, uint16_t start, uint16_t stop, uint8_t mode, uint32_t color,          uint16_t speed, uint8_t options),
      setSegment(uint8_t n, uint16_t start, uint16_t stop, uint8_t mode, const uint32_t colors[], uint16_t speed, bool reverse),
      setSegment(uint8_t n, uint16_t start, uint16_t stop, uint8_t mode, const uint32_t colors[], uint16_t speed, uint8_t options),
      setIdleSegment(uint8_t n, uint16_t start, uint16_t stop, uint8_t mode, uint32_t color,          uint16_t speed, uint8_t options),
      setIdleSegment(uint8_t n, uint16_t start, uint16_t stop, uint8_t mode, const uint32_t colors[], uint16_t speed, uint8_t options),
      addActiveSegment(uint8_t seg),
      removeActiveSegment(uint8_t seg),
      swapActiveSegment(uint8_t oldSeg, uint8_t newSeg),
      setPixelColor(uint16_t n, uint32_t c),
      setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b),
      setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b, uint8_t w),
      setPixelHsv(uint16_t n, uint8_t h, uint8_t s, uint8_t v),
      copyPixels(uint16_t d, uint16_t s, uint16_t c),
      blur(uint8_t blur_amount),
      show(void),
      fill(uint32_t c, uint16_t first, uint16_t count);

    boolean
      isRunning(void),
      isTriggered(void),
      isFrame(void),
      isFrame(uint8_t),
      isCycle(void),
      isCycle(uint8_t),
      isActiveSegment(uint8_t seg);

    bool gammaCorrectCol = true;

    uint8_t
      random8(void),
      random8(uint8_t),
      random8(uint8_t, uint8_t),
      getMode(void),
      getMode(uint8_t),
      // getModeCount(void),
      // setCustomMode(const __FlashStringHelper* name, uint16_t (*p)()),
      // setCustomMode(uint8_t i, const __FlashStringHelper* name, uint16_t (*p)()),
      getNumSegments(void),
      get_random_wheel_index(uint8_t),
      getOptions(uint8_t),
      gamma8(uint8_t),
      getNumBytesPerPixel(void);

    uint16_t
      random16(void),
      random16(uint16_t),
      getSpeed(void),
      getSpeed(uint8_t),
      getLength(void),
      getNumBytes(void);

    uint32_t
      color_wheel(uint8_t),
      getColor(void),
      getColor(uint8_t),
      intensitySum(void),
      gamma32(uint32_t),
      crgb_to_col(CRGB);  

    unsigned long now;   
    uint32_t nowUp;
    uint32_t* getColors(uint8_t);
    uint32_t* intensitySums(void);
    uint8_t*  getActiveSegments(void);

    // const __FlashStringHelper* getModeName(uint8_t m);

    WS2812FX::Segment* getSegment(void);

    WS2812FX::Segment* getSegment(uint8_t);

    WS2812FX::Segment* getSegments(void);

    WS2812FX::Segment_runtime* getSegmentRuntime(void);

    WS2812FX::Segment_runtime* getSegmentRuntime(uint8_t);

    WS2812FX::Segment_runtime* getSegmentRuntimes(void);

    // mode helper functions
    uint16_t
      blink(uint32_t, uint32_t, bool strobe),
      color_wipe(uint32_t, uint32_t, bool),
      color_wipe_inv(uint32_t, uint32_t, bool),
      twinkle(uint32_t, uint32_t),
      twinkle_fade(uint32_t),
      sparkle(uint32_t, uint32_t),
      chase(uint32_t, uint32_t, uint32_t),
      chase_flash(uint32_t, uint32_t),
      running(uint32_t, uint32_t),
      fireworks(uint32_t),
      fire_flicker(int),
      tricolor_chase(uint32_t, uint32_t),
      tricolor_chase(uint32_t, uint32_t, uint32_t),
      candle(bool),
      scan(bool),
      ripple_base(bool),
      scan(uint32_t, uint32_t, bool);

    uint32_t
      color_blend(uint32_t, uint32_t, uint8_t, bool b16 = false);

    CRGB col_to_crgb(uint32_t color);

  public:
    void
      service(boolean & setCycle, boolean & _show),
      setSpeed(uint16_t s);

    // builtin modes
    uint16_t
      mode_static(void),
      mode_blink(void),
      mode_blink_rainbow(void),
      mode_strobe(void),
      mode_strobe_rainbow(void),
      mode_color_wipe(void),
      mode_color_wipe_inv(void),
      mode_color_wipe_rev(void),
      mode_color_wipe_rev_inv(void),
      mode_color_wipe_random(void),
      mode_color_sweep_random(void),
      mode_random_color(void),
      mode_single_dynamic(void),
      mode_multi_dynamic(void),
      mode_breath(void),
      mode_fade(void),
      mode_scan(void),
      mode_dual_scan(void),
      mode_theater_chase(void),
      mode_theater_chase_rainbow(void),
      mode_rainbow(void),
      mode_rainbow_cycle(void),
      mode_running_lights(void),
      mode_twinkle(void),
      mode_twinkle_random(void),
      mode_twinkle_fade(void),
      mode_twinkle_fade_random(void),
      mode_sparkle(void),
      mode_flash_sparkle(void),
      mode_hyper_sparkle(void),
      mode_multi_strobe(void),
      mode_chase_white(void),
      mode_chase_color(void),
      mode_chase_random(void),
      mode_chase_rainbow(void),
      mode_chase_flash(void),
      mode_chase_flash_random(void),
      mode_chase_rainbow_white(void),
      mode_chase_blackout(void),
      mode_chase_blackout_rainbow(void),
      mode_running_color(void),
      mode_running_red_blue(void),
      mode_running_random(void),
      mode_larson_scanner(void),
      mode_comet(void),
      mode_fireworks(void),
      mode_fireworks_random(void),
      mode_merry_christmas(void),
      mode_halloween(void),
      mode_fire_flicker(void),
      mode_fire_flicker_soft(void),
      mode_fire_flicker_intense(void),
      mode_circus_combustus(void),
      mode_bicolor_chase(void),
      mode_tricolor_chase(void),
      mode_colortwinkle(void),
      mode_twinklefox(void),
      mode_twinklecat(void),
      mode_colorwaves(void),
      mode_pride(void),
      mode_fire_2012(void),
      mode_candle(void),
      mode_candle_multi(void),
      mode_ripple(void),
      mode_noise16_1(void),
      mode_noise16_2(void),
      mode_noise16_3(void),
      mode_noise16_4(void),
      mode_tricolor_wipe(void),
      mode_bouncing_balls(void),
      mode_aurora(void),
      mode_blends(void),
      mode_wavesins(void),
      mode_pacifica(void);


  
  private:
    CRGB pacifica_one_layer(uint16_t i, CRGBPalette16& p, uint16_t cistart, uint16_t wavescale, uint8_t bri, uint16_t ioff);
    CRGB twinklefox_one_twinkle(uint32_t ms, uint8_t salt, bool cat);
    uint16_t twinklefox_base(bool cat);
    uint32_t color_from_palette(uint16_t i, bool mapping, bool wrap, uint8_t mcol, uint8_t pbri = 255);

    CRGBPalette16 currentPalette;

    uint16_t _rand16seed;
    static uint16_t _usedSegmentData;

    boolean
      _running,
      _triggered;

    segment* _segments;                 // array of segments (20 bytes per element)
    segment_runtime* _segment_runtimes; // array of segment runtimes (16 bytes per element)
    uint8_t* _active_segments;          // array of active segments (1 bytes per element)

    uint8_t _segments_len = 0;          // size of _segments array
    uint8_t _active_segments_len = 0;   // size of _segments_runtime and _active_segments arrays
    uint8_t _num_segments = 0;          // number of configured segments in the _segments array
    uint8_t paletteBlend = 0;

    segment* _seg;                      // currently active segment (20 bytes)
    segment_runtime* _seg_rt;           // currently active segment runtime (16 bytes)

    uint16_t _seg_len;                  // num LEDs in the currently active segment
};




uint16_t convertSpeed(uint8_t mcl_speed);
// #endif
#endif

