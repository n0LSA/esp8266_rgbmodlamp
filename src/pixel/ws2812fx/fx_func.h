uint32_t WS2812FX::color_from_palette(uint16_t i, bool mapping, bool wrap, uint8_t mcol, uint8_t pbri)
{
  
  //   uint32_t color = SEGCOLOR(mcol);
  //   if (pbri != 255) {
  //     CRGB crgb_color = col_to_crgb(color);
  //     crgb_color.nscale8_video(pbri);
  //     return crgb_to_col(crgb_color);
  //   } else {
  //     return color;
  //   }


  // uint8_t paletteIndex = i;
  // if (mapping) paletteIndex = (i*255)/(_seg_len -1);
  // if (!wrap) paletteIndex = scale8(paletteIndex, 240); //cut off blend at palette "end"
  // CRGB fastled_col;
  // fastled_col = ColorFromPalette( currentPalette, paletteIndex, pbri, (paletteBlend == 3)? NOBLEND:LINEARBLEND);

  // return crgb_to_col(fastled_col);


  if ((_seg->palette == 0 && mcol < 3) ) {
    uint32_t color = crgb_to_col(CRGB(0,0,255));
    if (pbri == 255) return color;
    return RGBW32(scale8_video(R(color),pbri), scale8_video(G(color),pbri), scale8_video(B(color),pbri), scale8_video(W(color),pbri));
  }

  uint8_t paletteIndex = i;
  if (mapping && _seg_len > 1) paletteIndex = (i*255)/(_seg_len -1);
  if (!wrap) paletteIndex = scale8(paletteIndex, 240); //cut off blend at palette "end"
  CRGB fastled_col;
  fastled_col = ColorFromPalette(currentPalette, paletteIndex, pbri, (paletteBlend == 3)? NOBLEND:LINEARBLEND);

  return crgb_to_col(fastled_col);  
}

/*
 * Turns everything off. Doh.
 */
void WS2812FX::strip_off() {
  FastLED.clear();
  show();
}

/*
 * Put a value 0 to 255 in to get a color value.
 * The colours are a transition r -> g -> b -> back to r
 * Inspired by the Adafruit examples.
 */
uint32_t WS2812FX::color_wheel(uint8_t pos) {
  pos = 255 - pos;
  if(pos < 85) {
    return ((uint32_t)(255 - pos * 3) << 16) | ((uint32_t)(0) << 8) | (pos * 3);
  } else if(pos < 170) {
    pos -= 85;
    return ((uint32_t)(0) << 16) | ((uint32_t)(pos * 3) << 8) | (255 - pos * 3);
  } else {
    pos -= 170;
    return ((uint32_t)(pos * 3) << 16) | ((uint32_t)(255 - pos * 3) << 8) | (0);
  }
}

/*
 * Returns a new, random wheel index with a minimum distance of 42 from pos.
 */
uint8_t WS2812FX::get_random_wheel_index(uint8_t pos) {
  uint8_t r = 0;
  uint8_t x = 0;
  uint8_t y = 0;
  uint8_t d = 0;

  while(d < 42) {
    r = random8();
    x = abs(pos - r);
    y = 255 - x;
    d = min(x, y);
  }

  return r;
}

// fast 8-bit random number generator shamelessly borrowed from FastLED
uint8_t WS2812FX::random8() {
    _rand16seed = (_rand16seed * 2053) + 13849;
    return (uint8_t)((_rand16seed + (_rand16seed >> 8)) & 0xFF);
}

// note random8(lim) generates numbers in the range 0 to (lim -1)
uint8_t WS2812FX::random8(uint8_t lim) {
    uint8_t r = random8();
    r = ((uint16_t)r * lim) >> 8;
    return r;
}

uint16_t WS2812FX::random16() {
    return (uint16_t)random8() * 256 + random8();
}

// note random16(lim) generates numbers in the range 0 to (lim - 1)
uint16_t WS2812FX::random16(uint16_t lim) {
    uint16_t r = random16();
    r = ((uint32_t)r * lim) >> 16;
    return r;
}
uint8_t WS2812FX::random8(uint8_t min, uint8_t max) {
  return (random(min, max));
}

// Return the sum of all LED intensities (can be used for
// rudimentary power calculations)
uint32_t WS2812FX::intensitySum() {
  // uint8_t *pixels = getPixels();
  // uint32_t sum = 0;
  // for(uint16_t i=0; i <numBytes; i++) {
  //   sum+= pixels[i];
  // }
  return 0;
}

// Return the sum of each color's intensity. Note, the order of
// intensities in the returned array depends on the type of WS2812
// LEDs you have. NEO_GRB LEDs will return an array with entries
// in a different order then NEO_RGB LEDs.
uint32_t* WS2812FX::intensitySums() {
  // static uint32_t intensities[] = { 0, 0, 0, 0 };
  // memset(intensities, 0, sizeof(intensities));

  // uint8_t *pixels = getPixels();
  // uint8_t bytesPerPixel = getNumBytesPerPixel(); // 3=RGB, 4=RGBW
  // for(uint16_t i=0; i <numBytes; i += bytesPerPixel) {
  //   intensities[0] += pixels[i];
  //   intensities[1] += pixels[i + 1];
  //   intensities[2] += pixels[i + 2];
  //   if(bytesPerPixel == 4) intensities[3] += pixels[i + 3]; // for RGBW LEDs
  // }
  return 0;
}
CRGB WS2812FX_col_to_crgb(uint32_t color)
{
  CRGB fastled_col;
  fastled_col.red =   (color >> 16 & 0xFF);
  fastled_col.green = (color >> 8  & 0xFF);
  fastled_col.blue =  (color       & 0xFF);
  return fastled_col;
}
CRGB WS2812FX::col_to_crgb(uint32_t color)
{
  CRGB fastled_col;
  fastled_col.red =   (color >> 16 & 0xFF);
  fastled_col.green = (color >> 8  & 0xFF);
  fastled_col.blue =  (color       & 0xFF);
  return fastled_col;
}

uint32_t WS2812FX::crgb_to_col(CRGB fastled)
{
  return (((uint32_t)fastled.red << 16) | ((uint32_t)fastled.green << 8) | fastled.blue);
}


/*
 * color blend function
 */
uint32_t WS2812FX::color_blend(uint32_t color1, uint32_t color2, uint8_t blend, bool b16) {
  if(blend == 0)   return color1;
  uint16_t blendmax = b16 ? 0xFFFF : 0xFF;
  if(blend == blendmax) return color2;
  uint8_t shift = b16 ? 16 : 8;

  uint32_t w1 = W(color1);
  uint32_t r1 = R(color1);
  uint32_t g1 = G(color1);
  uint32_t b1 = B(color1);

  uint32_t w2 = W(color2);
  uint32_t r2 = R(color2);
  uint32_t g2 = G(color2);
  uint32_t b2 = B(color2);

  uint32_t w3 = ((w2 * blend) + (w1 * (blendmax - blend))) >> shift;
  uint32_t r3 = ((r2 * blend) + (r1 * (blendmax - blend))) >> shift;
  uint32_t g3 = ((g2 * blend) + (g1 * (blendmax - blend))) >> shift;
  uint32_t b3 = ((b2 * blend) + (b1 * (blendmax - blend))) >> shift;

  return RGBW32(r3, g3, b3, w3);

}

/*
 * blurs segment content, source: FastLED colorutils.cpp
 */
void WS2812FX::blur(uint8_t blur_amount)
{
  uint8_t keep = 255 - blur_amount;
  uint8_t seep = blur_amount >> 1;
  CRGB carryover = CRGB::Black;
  for(uint16_t i = 0; i < _seg_len; i++)
  {


    // CRGB cur = col_to_crgb(getPixelColor(i));
    CRGB cur;
    if (effectSetting->isTw) cur = getPixelColor(_strip_w, i); 
    else                    cur = getPixelColor(_strip,   i); 

    CRGB part = cur;
    part.nscale8(seep);
    cur.nscale8(keep);
    cur += carryover;
    if(i > 0) {
      // uint32_t c = getPixelColor(i-1);
      uint32_t c;
      if (effectSetting->isTw) c = getPixelColor(_strip_w, i-1); 
      else                    c = getPixelColor(_strip,   i-1);       
      uint8_t r = R(c);
      uint8_t g = G(c);
      uint8_t b = B(c);
      setPixelColor(i-1, qadd8(r, part.red), qadd8(g, part.green), qadd8(b, part.blue));
    }
    setPixelColor(i,cur.red, cur.green, cur.blue);
    carryover = part;
  }
}

void WS2812FX::fill(uint32_t c, uint16_t first, uint16_t count) {
  uint16_t i, end;

  // If first LED is past end of strip or outside segment boundaries, nothing to do
  if (first >= effectSetting->numLeds || first < _seg->start || first > _seg->stop) {
    return;
  }

  // Calculate the index ONE AFTER the last pixel to fill
  if (count == 0) {
    end = _seg->stop + 1; // Fill to end of segment
  } else {
    end = first + count;
    if(end > (_seg->stop + 1)) end = _seg->stop + 1;
  }

  if (end > effectSetting->numLeds) end = effectSetting->numLeds;

  for (i = first; i < end; i++) {
    setPixelColor(i, c);
  }
}