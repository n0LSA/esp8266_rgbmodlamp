#include "../../include/pixel/set.h"

namespace {
  void fill_solid( struct CRGBW * leds, uint16_t numToFill, const struct CRGB& color) {for( int i = 0; i < numToFill; i++) {leds[i] = color;}}
  void fill_solid( struct CRGBW * leds, uint16_t numToFill, const struct CHSV& color) {for( int i = 0; i < numToFill; i++) {leds[i] = color;}}
  void fill_solidW(struct CRGBW * leds, uint16_t numToFill, const struct CRGBW& color){for(int i = 0; i < numToFill; i++){leds[i] = color;}}
  void fill_solid( struct CRGBW * leds, uint16_t numToFill, const struct RgbColor& c) {for( int i = 0; i < numToFill; i++) {leds[i] = CRGB(c.R, c.G, c.B);}}

  void fill_solid( struct CRGB * leds, const struct CHSV& color, int start, int end)  {for( int i = start; i < end; i++) {leds[i] = color;}}
  void fill_solid( struct CRGB * leds, const struct CRGB& color, int start, int end)  {for( int i = start; i < end; i++) {leds[i] = color;}}  
  void fill_solid( struct CRGB * leds, const struct RgbColor& c, int start, int end)  {for( int i = start; i < end; i++) {leds[i] = CRGB(c.R, c.G, c.B);}}

  void fill_solid( struct CRGBW * leds, const struct CHSV& color, int start, int end) {for( int i = start; i < end; i++) {leds[i] = color;}}
  void fill_solid( struct CRGBW * leds, const struct CRGB& color, int start, int end) {for( int i = start; i < end; i++) {leds[i] = color;}}  
  void fill_solidW(struct CRGBW * leds, const struct CRGBW& color, int start, int end){for( int i = start; i < end; i++) {leds[i] = color;}}
  // void fill_solid( struct CRGBW * leds, const struct RgbColor& c, int start, int end)  {for( int i = start; i < end; i++) {leds[i] = CRGB(c.R, c.G, c.B);}}
    
    int _num_leds(uint8_t op) {uint16_t nb;leds_managePtrGet()->get_numLeds(op, nb);return nb;} 
    CRGB * _CRGBGET(uint8_t op) {return leds_managePtrGet()->get_crgbArray(op);} 
    CRGBW * _CRGBWGET(uint8_t op) {return leds_managePtrGet()->get_crgwbArray(op);} 
}

void ledsSet::fill(CRGBW * array, const struct CHSV &c, int start, int end)   {fill_solid(array, c, start, end);} 
void ledsSet::fill(CRGBW * array, const struct CRGB &c, int start, int end)   {fill_solid(array, c, start, end);}
void ledsSet::fill(CRGBW * array, const struct RgbColor &c, int start, int end) {fill_solid(array, CRGB(c.R, c.G, c.B), start, end);}
void ledsSet::fill(CRGBW * array, const struct CRGBW &c, int start, int end)  {fill_solidW(array, c, start, end);}

void ledsSet::fill(CRGB * array, const struct CHSV &c, int start, int end)      {fill_solid(array, c, start, end);} 
void ledsSet::fill(CRGB * array, const struct CRGB &c, int start, int end)      {fill_solid(array, c, start, end);}
void ledsSet::fill(CRGB * array, const struct RgbColor &c, int start, int end)  {fill_solid(array, CRGB(c.R, c.G, c.B), start, end);}

void ledsSet::fill(uint8_t op, CRGB * array, const struct CHSV &c)      {fill_solid(array, c, 0, _num_leds(op));} 
void ledsSet::fill(uint8_t op, CRGB * array, const struct CRGB &c)      {fill_solid(array, c, 0, _num_leds(op));}
void ledsSet::fill(uint8_t op, CRGB * array, const struct RgbColor &c)    {fill_solid(array, CRGB(c.R, c.G, c.B), 0, _num_leds(op));}

void ledsSet::fill(uint8_t op, CRGBW * array, const struct CHSV &c)     {fill_solid(array,  c, 0, _num_leds(op));}  
void ledsSet::fill(uint8_t op, CRGBW * array, const struct CRGBW &c)    {fill_solidW(array, c, 0, _num_leds(op));}
void ledsSet::fill(uint8_t op, CRGBW * array, const struct RgbColor &c) {fill_solid(array,  CRGB(c.R, c.G, c.B), 0, _num_leds(op));}
void ledsSet::fill(uint8_t op, CRGBW * array, const struct CRGB &c)     {fill_solid(array,  c, 0, _num_leds(op));}

void ledsSet::fill(uint8_t op, const struct CHSV &c) {
  boolean isTw;
  leds_managePtrGet()->get_ledsIsTw(op, isTw);
  if (isTw)   {CRGBW * array = _CRGBWGET(op); fill_solid(array, c, 0, _num_leds(op));}
  else    {CRGB * array = _CRGBGET(op); fill_solid(array, c, 0, _num_leds(op));} 
} 
void ledsSet::fill(uint8_t op, const struct CRGB &c) {
  boolean isTw;
  leds_managePtrGet()->get_ledsIsTw(op, isTw);
  if (isTw)   {CRGBW * array = _CRGBWGET(op); fill_solid(array, c, 0, _num_leds(op));}
  else        {CRGB * array = _CRGBGET(op); fill_solid(array, c, 0, _num_leds(op));}
}
void ledsSet::fill(uint8_t op, const struct RgbColor &c) {
  boolean isTw;
  leds_managePtrGet()->get_ledsIsTw(op, isTw);
  if (isTw)   {CRGBW * array = _CRGBWGET(op);  fill_solid(array, CRGB(c.R, c.G, c.B), 0, _num_leds(op));}
  else    {CRGB * array = _CRGBGET(op); fill_solid(array, CRGB(c.R, c.G, c.B), 0, _num_leds(op));}
}
void ledsSet::fill(uint8_t op, const struct CRGBW &c) {
  boolean isTw;
  leds_managePtrGet()->get_ledsIsTw(op, isTw);
  if (isTw)   {
    CRGBW * array = _CRGBWGET(op); 
    fill_solidW(array, c, 0, _num_leds(op));  
  } 
}






ledsTools::ledsTools(){
}
ledsTools::~ledsTools(){
};
CRGB ledsTools::splitColor(String Val) {
    String      aVal           = Val;
    byte        firstIndex     = aVal.indexOf('.');
    byte        secondIndex    = aVal.indexOf('.', firstIndex + 1);
    String      red            = aVal.substring(0, firstIndex);
    String      green          = aVal.substring(firstIndex + 1, secondIndex);
    String      blue           = aVal.substring(secondIndex + 1);
    int         r              = red.toInt();
    int         g              = green.toInt();
    int         b              = blue.toInt();
    return CRGB(r,g,b);    
}
int* ledsTools::splitColorToInt(String Val) {
    String      aVal            = Val;
    byte        firstIndex      = aVal.indexOf('.');
    byte        secondIndex     = aVal.indexOf('.', firstIndex + 1);
    String      red             = aVal.substring(0, firstIndex);
    String      green           = aVal.substring(firstIndex + 1, secondIndex);
    String      blue            = aVal.substring(secondIndex + 1);
    int         r               = red.toInt();
    int         g               = green.toInt();
    int         b               = blue.toInt();
    int     *array          = new int[3];
            array[0]        = r;
            array[1]        = g;
            array[2]        = b;
    return array;    
}


#define HSV_SECTION_3 (86)

RgbColor ledsTools::hsv2rgb (uint8_t h, uint8_t s, uint8_t v, int pix_nbr) {
    //long start=micros();

    // fsprintf("\nHSV_SECTION_3: %d\n", HSV_SECTION_3);
    RgbColor rgb;

    uint8_t value = v;
    uint8_t sat = s;

    // The brightness floor is minimum number that all of R, G, and B will be set to.
    uint8_t brightness_floor=(value*(255-sat))/256;

    // The color amplitude is the maximum amount of R, G, and B that will be added on top of the brightness_floor
    uint8_t color_amplitude = value-brightness_floor;

    // Figure out which section of the hue wheel we're in, and how far offset we are within that section
    uint8_t section = h / HSV_SECTION_3;              // 0..2
    uint8_t offset  = h % HSV_SECTION_3;              // 0..63

    uint8_t rampup = offset;                    // 0..63
    uint8_t rampdown = (HSV_SECTION_3 - 1) - offset;  // 63..0

    // compute color amplitude scaled down versions of rampup and rampdown
    uint8_t rampup_amp_adj   = (rampup   * color_amplitude) / (86);
    uint8_t rampdown_amp_adj = (rampdown * color_amplitude) / (86);

    // add brightness_floor offset to everything
    uint8_t rampup_adj_with_floor   = rampup_amp_adj   + brightness_floor;
    uint8_t rampdown_adj_with_floor = rampdown_amp_adj + brightness_floor;
    // fsprintf("\nsection: %d\n", section);

    switch (section) {
        case 0:
            rgb.R = rampdown_adj_with_floor;
            rgb.G = rampup_adj_with_floor;
            rgb.B = brightness_floor;
        break;
        case 1:
            rgb.R = brightness_floor;
            rgb.G = rampdown_adj_with_floor;
            rgb.B = rampup_adj_with_floor;
        break;
        case 2:
            rgb.R = rampup_adj_with_floor;
            rgb.G = brightness_floor;
            rgb.B = rampdown_adj_with_floor;
        break;
    }

        return rgb;
}
HsvColor ledsTools::rgb2hsv(unsigned char r, unsigned char  g, unsigned char  b) {
    HsvColor hsv;
    unsigned char rgb_min, rgb_max;

    rgb_min = r<g ? (r<b?r:b) : (g<b?g:b);
    rgb_max = r>g ? (r>b?r:b) : (g>b?g:b);

    hsv.v = rgb_max;
    if (hsv.v==0)  { hsv.h=0; hsv.s=0; return hsv; }
    hsv.s = 255 * long(rgb_max - rgb_min) / hsv.v;

    if (hsv.s==0)   { hsv.h=0;  return hsv; }

    if (rgb_max==r)         hsv.h =     0+43*(g-b)/(rgb_max-rgb_min);
    else if (rgb_max==g)    hsv.h =     85+43*(b-r)/(rgb_max-rgb_min);
    else                    hsv.h =     171+43*(r-g)/(rgb_max-rgb_min);

    return hsv;
}
uint32_t ledsTools::hsv2_32(uint8_t h, uint8_t s, uint8_t v) {
    RgbColor c1 = hsv2rgb (h, s, v, 0);
    // return CRGB(c1.R, c1.G, c1.B); 
    return ( ((uint32_t)c1.R << 16) | ((uint16_t)c1.G << 8 ) | (uint16_t)c1.B );
}
uint32_t ledsTools::rgb2_32(const RgbColor &c1) {
    return ( ((uint32_t)c1.R << 16) | ((uint16_t)c1.G << 8 ) | (uint16_t)c1.B );
}
uint32_t ledsTools::rgb2_32(uint8_t r, uint8_t g, uint8_t b) {
    return ( ((uint32_t)r << 16) | ((uint16_t)g << 8 ) | (uint16_t)b );
}
CRGB ledsTools::fade2black(CRGB rgb, uint8_t ammount) {
  CRGB result;
  int j = (255-ammount);
  if (j==0) j=1;
  result.r = (rgb.r>j) ? rgb.r-j  : 0 ;
  result.g = (rgb.g>j) ? rgb.g-j  : 0 ;
  result.b = (rgb.b>j) ? rgb.b-j  : 0 ;
  return result;
}

CRGB ledsTools::fade2black(uint8_t r, uint8_t g, uint8_t b, uint8_t ammount) {
  CRGB result;
  int j = (255-ammount);
  if (j==0) j=1;
  result.r = (r>j) ? r-j  : 0 ;
  result.g = (g>j) ? g-j  : 0 ;
  result.b = (b>j) ? b-j  : 0 ;
  return result;
}

CRGB ledsTools::Wheel(byte WheelPos) {
   if(WheelPos < 85) {
      return CRGB(WheelPos * 3, 255 - WheelPos * 3, 0);
   } 
   else if(WheelPos < 170) {
      WheelPos -= 85;
      return CRGB(255 - WheelPos * 3, 0, WheelPos * 3);
   } 
   else {
      WheelPos -= 170;
      return CRGB(0, WheelPos * 3, 255 - WheelPos * 3);
   }
}   
uint8_t ledsTools::WheelPos(int pos){
    CRGB c = Wheel(pos);
    HsvColor hsv = rgb2hsv(c.r, c.g, c.b);
    return hsv.h; 
}
CRGB ledsTools::rgb32_crgb(uint32_t color)
{
  CRGB fastled_col;
  fastled_col.red =   (color >> 16 & 0xFF);
  fastled_col.green = (color >> 8  & 0xFF);
  fastled_col.blue =  (color       & 0xFF);
  return fastled_col;
}
CRGBW ledsTools::rgb32_crgbw(uint32_t color)
{
  CRGBW fastled_col;
  fastled_col.r = (color >> 16 & 0xFF);
  fastled_col.g = (color >> 8  & 0xFF);
  fastled_col.b = (color       & 0xFF);
  fastled_col.w = (color >> 24 & 0xFF);
  return fastled_col;
}


/*
 * color blend function

uint32_t color_blend(uint32_t color1, uint32_t color2, uint8_t blend) {
  if(blend == 0)   return color1;
  if(blend == 255) return color2;

  uint32_t w1 = (color1 >> 24) & 0xff;
  uint32_t r1 = (color1 >> 16) & 0xff;
  uint32_t g1 = (color1 >>  8) & 0xff;
  uint32_t b1 =  color1        & 0xff;

  uint32_t w2 = (color2 >> 24) & 0xff;
  uint32_t r2 = (color2 >> 16) & 0xff;
  uint32_t g2 = (color2 >>  8) & 0xff;
  uint32_t b2 =  color2        & 0xff;

  uint32_t w3 = ((w2 * blend) + (w1 * (255 - blend))) >> 8;
  uint32_t r3 = ((r2 * blend) + (r1 * (255 - blend))) >> 8;
  uint32_t g3 = ((g2 * blend) + (g1 * (255 - blend))) >> 8;
  uint32_t b3 = ((b2 * blend) + (b1 * (255 - blend))) >> 8;

  return ((w3 << 24) | (r3 << 16) | (g3 << 8) | (b3));

}
uint32_t crgb_to_col(CRGB fastled)
{
  return (((uint32_t)fastled.red << 16) | ((uint32_t)fastled.green << 8) | fastled.blue);
}
void setPixelColor(CRGBW * sLedarray, uint16_t n, uint32_t c) {
    uint8_t w = (c >> 24) & 0xFF;
    uint8_t r = (c >> 16) & 0xFF;
    uint8_t g = (c >>  8) & 0xFF;
    uint8_t b =  c        & 0xFF;
    sLedarray[n] = CRGBW(r,g,b,w);
}
 */
uint32_t getPixelColor(CRGBW *leds, uint16_t index) {
  return ( ( (uint32_t)leds[index].r << 16) | ((uint16_t)leds[index].g << 8 ) | (uint16_t)leds[index].b );
}  
uint32_t getPixelColor(CRGB *leds, uint16_t index) {
  return ( ( (uint32_t)leds[index].r << 16) | ((uint16_t)leds[index].g << 8 ) | (uint16_t)leds[index].b );
}
uint8_t addb(uint8_t a, uint8_t b) {
  int s=a+b;
  uint8_t r = (s>255) ? 255 : a+b;
  return r;
}
void led_set_hsv_(CRGB *leds, int led_nbr, int nb_leds , int pixel_offset, uint8_t hue, uint8_t sat, uint8_t val, uint8_t fade  ){
  int     pix_nbr= (led_nbr + pixel_offset)%nb_leds ;
  // uint8_t   coef;

  RgbColor rgb_last;
  rgb_last.R  = leds[pix_nbr].r;
  rgb_last.G  = leds[pix_nbr].g;
  rgb_last.B  = leds[pix_nbr].b;

  ledsTools _tools;
  RgbColor rgb_next  = _tools.hsv2rgb(hue, sat, (val*fade)>>8 , pix_nbr);

  rgb_last.R  = map(rgb_last.R, 0, 255, 0, (255-fade));
  rgb_last.G  = map(rgb_last.G, 0, 255, 0, (255-fade));
  rgb_last.B  = map(rgb_last.B, 0, 255, 0, (255-fade));

  rgb_next.R  = addb(rgb_next.R, rgb_last.R);
  rgb_next.G  = addb(rgb_next.G, rgb_last.G);
  rgb_next.B  = addb(rgb_next.B, rgb_last.B);

  if (pixel_offset==-1) {
    rgb_next.R  = 0;
    rgb_next.G  = 0;
    rgb_next.B  = 0;
  }

  leds[pix_nbr].r = rgb_next.R;
  leds[pix_nbr].g = rgb_next.G;
  leds[pix_nbr].b = rgb_next.B;
}
void led_set_hsv(CRGBW *leds, int led_nbr, int nb_leds , int pixel_offset, uint8_t hue, uint8_t sat, uint8_t val, uint8_t fade  ){
  int     pix_nbr= (led_nbr + pixel_offset)%nb_leds ;
  // uint8_t   coef;

  RgbColor rgb_last;
  rgb_last.R  = leds[pix_nbr].r;
  rgb_last.G  = leds[pix_nbr].g;
  rgb_last.B  = leds[pix_nbr].b;

  ledsTools _tools;
  RgbColor rgb_next  = _tools.hsv2rgb(hue, sat, (val*fade)>>8 , pix_nbr);

  rgb_last.R  = map(rgb_last.R, 0, 255, 0, (255-fade));
  rgb_last.G  = map(rgb_last.G, 0, 255, 0, (255-fade));
  rgb_last.B  = map(rgb_last.B, 0, 255, 0, (255-fade));

  rgb_next.R  = addb(rgb_next.R, rgb_last.R);
  rgb_next.G  = addb(rgb_next.G, rgb_last.G);
  rgb_next.B  = addb(rgb_next.B, rgb_last.B);

  if (pixel_offset==-1) {
    rgb_next.R  = 0;
    rgb_next.G  = 0;
    rgb_next.B  = 0;
  }

  leds[pix_nbr].r = rgb_next.R;
  leds[pix_nbr].g = rgb_next.G;
  leds[pix_nbr].b = rgb_next.B;
}
uint8_t _map8(uint8_t val, uint8_t min, uint8_t max) {
    return map(val, 0, 255, min, max);
}
uint8_t timebase_shift_comp; // shift compensation, helps smooth speed variations when speed is adjusted with slider
uint8_t sawtooth_wave(uint8_t bpm) {
    long scale = timebase_shift_comp + (256 * millis() * bpm  )/ 60000 ;
    return scale;
}
int rotate (uint8_t speed, uint16_t nb_leds){
    uint8_t phase = sawtooth_wave(speed);
    int pos = map(phase, 0, 255, 0, nb_leds);
    return pos;
}