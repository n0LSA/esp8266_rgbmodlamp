uint8_t sine8(uint8_t a) {
    uint8_t result=127*(1+sin(a*6.28/255));
    //Serial.printf("sin8: a:%3d sin8:%3d\n", a, result);
    return result;
}


uint16_t convertSpeed(uint8_t mcl_speed) {
  //long ws2812_speed = mcl_speed * 256;
  uint16_t ws2812_speed = 61760 * (exp(0.0002336 * mcl_speed) - exp(-0.03181 * mcl_speed));
  ws2812_speed = SPEED_MAX - ws2812_speed;

  if (ws2812_speed < SPEED_MIN) {
    ws2812_speed = SPEED_MIN;
  }
  if (ws2812_speed > SPEED_MAX) {
    ws2812_speed = SPEED_MAX;
  }
  return ws2812_speed;
}

// overload setPixelColor() functions so we can use gamma correction
// (see https://learn.adafruit.com/led-tricks-gamma-correction/the-issue)
void WS2812FX::setPixelColor(uint16_t n, uint32_t c) {
    uint8_t w = (c >> 24) & 0xFF;
    uint8_t r = (c >> 16) & 0xFF;
    uint8_t g = (c >>  8) & 0xFF;
    uint8_t b =  c        & 0xFF;
    setPixelColor(n, r, g, b, w);
}

void WS2812FX::setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b) {
    setPixelColor(n, r, g, b, 0);
}

void WS2812FX::setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b, uint8_t w) {
    if(IS_GAMMA) {
        if (effectSetting->isTw) _strip_w[n] = CRGBW(r,g,b,w);
        else _strip[n] = CRGB(r,g,b);
    } else {
        if (effectSetting->isTw) _strip_w[n] = CRGBW(r,g,b,w);
        else _strip[n] = CRGB(r,g,b);
    }
}
void WS2812FX::setPixelHsv(uint16_t n, uint8_t h, uint8_t s, uint8_t v) {
    if(IS_GAMMA) {
        if (effectSetting->isTw) _strip_w[n] = CHSV(h,s,v);
        else _strip[n] = CHSV(h,s,v);
    } else {
        if (effectSetting->isTw) _strip_w[n] = CHSV(h,s,v);
        else _strip[n] = CHSV(h,s,v);
    }
}

void WS2812FX_setPixelColor(CRGBW* strip, uint16_t n, uint8_t r, uint8_t g, uint8_t b, uint8_t w) {
    strip[n] = CRGB(r,g,b);
}  
void WS2812FX_setPixelColor(CRGB* strip, uint16_t n, uint8_t r, uint8_t g, uint8_t b, uint8_t w) {
    strip[n] = CRGB(r,g,b);
}

void WS2812FX::copyPixels(uint16_t dest, uint16_t src, uint16_t count) {
  // uint8_t *pixels = getPixels();
  // uint8_t bytesPerPixel = getNumBytesPerPixel(); // 3=RGB, 4=RGBW

  // memmove(pixels + (dest * bytesPerPixel), pixels + (src * bytesPerPixel), count * bytesPerPixel);
}

// change the underlying Adafruit_NeoPixel pixels pointer (use with care)
// void WS2812FX::setPixels(uint16_t num_leds, uint8_t* ptr) {
//   free(_strip->pixels); // free existing data (if any)
//   _strip->pixels = ptr;
//   _strip->numLEDs = num_leds;
//   _strip->numBytes = num_leds * ((wOffset == rOffset) ? 3 : 4);
// }

// overload show() functions so we can use custom show()
void WS2812FX::show(void) {
  FastLED.show();   
}

void WS2812FX::start() {
  resetSegmentRuntimes();
  _running = true;
}

void WS2812FX::stop() {
  _running = false;
  strip_off();
}

void WS2812FX::pause() {
  _running = false;
}

void WS2812FX::resume() {
  _running = true;
}

void WS2812FX::trigger() {
  _triggered = true;
}

// void WS2812FX::setMode(uint8_t m) {
//   setMode(0, m);
// }

// void WS2812FX::setMode(uint8_t seg, uint8_t m) {
//   resetSegmentRuntime(seg);
//   _segments[seg].mode = constrain(m, 0, MODE_COUNT - 1);
// }

void WS2812FX::setOptions(uint8_t seg, uint8_t o) {
  _segments[seg].options = o;
}

void WS2812FX::setSpeed(uint16_t s) {
  
    for(uint8_t i=0; i < _active_segments_len; i++) {
      if(_active_segments[i] != INACTIVE_SEGMENT) {
        setSpeed(i, s);
      }
    }  
}

void WS2812FX::setSpeed(uint8_t seg, uint16_t s) {
  _segments[seg].speed = constrain(s, SPEED_MIN, SPEED_MAX);
}

void WS2812FX::increaseSpeed(uint8_t s) {
  uint16_t newSpeed = constrain(_seg->speed + s, SPEED_MIN, SPEED_MAX);
  setSpeed(newSpeed);
}

void WS2812FX::decreaseSpeed(uint8_t s) {
  uint16_t newSpeed = constrain(_seg->speed - s, SPEED_MIN, SPEED_MAX);
  setSpeed(newSpeed);
}

void WS2812FX::setColor(uint8_t r, uint8_t g, uint8_t b) {
  setColor(((uint32_t)r << 16) | ((uint32_t)g << 8) | b);
}

void WS2812FX::setColor(uint8_t r, uint8_t g, uint8_t b, uint8_t w) {
  setColor((((uint32_t)w << 24)| ((uint32_t)r << 16) | ((uint32_t)g << 8)| ((uint32_t)b)));
}

void WS2812FX::setColor(uint32_t c) {
  setColor(0, c);
}

void WS2812FX::setColor(uint8_t seg, uint32_t c) {
  _segments[seg].colors[0] = c;
}

void WS2812FX::setColors(uint8_t seg, uint32_t* c) {
  for(uint8_t i=0; i<MAX_NUM_COLORS; i++) {
    _segments[seg].colors[i] = c[i];
  }
}

// void WS2812FX::setBrightness(uint8_t b) {
//   b = constrain(b, BRIGHTNESS_MIN, BRIGHTNESS_MAX);
//   FastLED.setBrightness(b);
//   show();
// }

void WS2812FX::increaseBrightness(uint8_t s) {
  // s = constrain(getBrightness() + s, BRIGHTNESS_MIN, BRIGHTNESS_MAX);
  // setBrightness(s);
}

void WS2812FX::decreaseBrightness(uint8_t s) {
  // s = constrain(getBrightness() - s, BRIGHTNESS_MIN, BRIGHTNESS_MAX);
  // setBrightness(s);
}

void WS2812FX::setLength(uint16_t b) {
  // resetSegmentRuntimes();
  // if (b < 1) b = 1;

  // // Decrease numLEDs to maximum available memory
  // do {
  //     _strip->updateLength(b);
  //     b--;
  // } while(!_strip->numLEDs && b > 1);

  // _segments[0].start = 0;
  // _segments[0].stop = _strip->numLEDs - 1;
}

void WS2812FX::increaseLength(uint16_t s) {
  // uint16_t seglen = _segments[0].stop - _segments[0].start + 1;
  // setLength(seglen + s);
}

void WS2812FX::decreaseLength(uint16_t s) {
  // uint16_t seglen = _segments[0].stop - _segments[0].start + 1;
  // fill(FXNEO_BLACK, _segments[0].start, seglen);
  // show();

  // if (s < seglen) setLength(seglen - s);
}