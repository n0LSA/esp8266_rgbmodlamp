#ifndef __COLOR_PALETTE_H
#define __COLOR_PALETTE_H
// переместил в platformio.ini
// #ifdef ESP8266
// #define FASTLED_USE_PROGMEM             (1)
// #endif

#include <FastLED.h>
#include <LinkedList.h>


DECLARE_GRADIENT_PALETTE(MBVioletColors_gp);

//////////////////////////////

DECLARE_GRADIENT_PALETTE(ib_jul01_gp);
DECLARE_GRADIENT_PALETTE(es_vintage_57_gp);
DECLARE_GRADIENT_PALETTE(es_vintage_01_gp);
DECLARE_GRADIENT_PALETTE(es_rivendell_15_gp);
DECLARE_GRADIENT_PALETTE(rgi_15_gp);
DECLARE_GRADIENT_PALETTE(retro2_16_gp);
DECLARE_GRADIENT_PALETTE(Analogous_1_gp);
DECLARE_GRADIENT_PALETTE(es_pinksplash_08_gp);
DECLARE_GRADIENT_PALETTE(es_pinksplash_07_gp);
DECLARE_GRADIENT_PALETTE(Coral_reef_gp);
DECLARE_GRADIENT_PALETTE(es_ocean_breeze_068_gp);
DECLARE_GRADIENT_PALETTE(es_ocean_breeze_036_gp);
DECLARE_GRADIENT_PALETTE(departure_gp);
DECLARE_GRADIENT_PALETTE(es_landscape_64_gp);
DECLARE_GRADIENT_PALETTE(es_landscape_33_gp);
DECLARE_GRADIENT_PALETTE(rainbowsherbet_gp);
DECLARE_GRADIENT_PALETTE(gr65_hult_gp);
DECLARE_GRADIENT_PALETTE(gr64_hult_gp);
DECLARE_GRADIENT_PALETTE(GMT_drywet_gp);
DECLARE_GRADIENT_PALETTE(ib15_gp);
DECLARE_GRADIENT_PALETTE(Fuschia_7_gp);
DECLARE_GRADIENT_PALETTE(es_emerald_dragon_08_gp);
DECLARE_GRADIENT_PALETTE(lava_gp);
DECLARE_GRADIENT_PALETTE(fire_gp);
DECLARE_GRADIENT_PALETTE(Colorfull_gp);
DECLARE_GRADIENT_PALETTE(Magenta_Evening_gp);
DECLARE_GRADIENT_PALETTE(Pink_Purple_gp);
DECLARE_GRADIENT_PALETTE(Sunset_Real_gp);
DECLARE_GRADIENT_PALETTE(es_autumn_19_gp);
DECLARE_GRADIENT_PALETTE(BlacK_Blue_Magenta_White_gp);
DECLARE_GRADIENT_PALETTE(BlacK_Magenta_Red_gp);
DECLARE_GRADIENT_PALETTE(BlacK_Red_Magenta_Yellow_gp);
DECLARE_GRADIENT_PALETTE(Blue_Cyan_Yellow_gp);

DECLARE_GRADIENT_PALETTE(Sunset_Yellow_gp);
DECLARE_GRADIENT_PALETTE(Another_Sunset_gp);
DECLARE_GRADIENT_PALETTE(Beech_gp);
DECLARE_GRADIENT_PALETTE(Tertiary_01_gp);
DECLARE_GRADIENT_PALETTE(fierce_ice_gp);

DECLARE_GRADIENT_PALETTE(Orange_Teal_gp);
DECLARE_GRADIENT_PALETTE(Tiamat_gp);
DECLARE_GRADIENT_PALETTE(April_Night_gp);
DECLARE_GRADIENT_PALETTE(Orangery_gp);
DECLARE_GRADIENT_PALETTE(C9_gp);
DECLARE_GRADIENT_PALETTE(Sakura_gp);
DECLARE_GRADIENT_PALETTE(Aurora_gp);
DECLARE_GRADIENT_PALETTE(Atlantica_gp);


const TProgmemRGBGradientPalettePtr gGradientPalettes[] = {
  // ib_jul01_gp,               //13-00 Sunset s
  // es_vintage_57_gp,           //14-01 Rivendell
  // es_vintage_01_gp,       //15-02 Breeze
  // es_rivendell_15_gp,                    //16-03 Red & Blue
  // rgi_15_gp,                 //17-04 Yellowout
  // retro2_16_gp,               //18-05 Analogous
  // Analogous_1_gp,          //19-06 Splash
  // es_pinksplash_08_gp,             //20-07 Pastel
  // es_pinksplash_07_gp,            //21-08 Sunset2
  // Coral_reef_gp,                     //22-09 Beech
  // es_ocean_breeze_068_gp,             //23-10 Vintage
  // es_ocean_breeze_036_gp,                 //24-11 Departure
  // departure_gp,           //25-12 Landscape
  // es_landscape_64_gp,           //26-13 Beach
  // es_landscape_33_gp,            //27-14 Sherbet
  // rainbowsherbet_gp,                 //28-15 Hult
  // gr65_hult_gp,                 //29-16 Hult64
  // gr64_hult_gp,                //30-17 Drywet
  // GMT_drywet_gp,                  //31-18 Jul
  // ib15_gp,             //32-19 Grintage
  // Fuschia_7_gp,                      //33-20 Rewhi
  // es_emerald_dragon_08_gp,                     
  // lava_gp,               //34-21 Tertiary
  // fire_gp,                      
  // Colorfull_gp,                      //35-22 Fire
  // Magenta_Evening_gp,                                     
  // Pink_Purple_gp,                //36-23 Icefire
  // Sunset_Real_gp, 
  // es_autumn_19_gp,
  // BlacK_Blue_Magenta_White_gp, 
  // BlacK_Magenta_Red_gp,
  // BlacK_Red_Magenta_Yellow_gp,  //40-27 Magenta
  // Blue_Cyan_Yellow_gp,         //41-28 Magred

  Sunset_Real_gp,               //13-00 Sunset
  es_rivendell_15_gp,           //14-01 Rivendell
  es_ocean_breeze_036_gp,       //15-02 Breeze
  rgi_15_gp,                    //16-03 Red & Blue
  retro2_16_gp,                 //17-04 Yellowout
  Analogous_1_gp,               //18-05 Analogous
  es_pinksplash_08_gp,          //19-06 Splash
  Sunset_Yellow_gp,             //20-07 Pastel
  Another_Sunset_gp,            //21-08 Sunset2
  Beech_gp,                     //22-09 Beech
  es_vintage_01_gp,             //23-10 Vintage
  departure_gp,                 //24-11 Departure
  es_landscape_64_gp,           //25-12 Landscape
  es_landscape_33_gp,           //26-13 Beach
  rainbowsherbet_gp,            //27-14 Sherbet
  gr65_hult_gp,                 //28-15 Hult
  gr64_hult_gp,                 //29-16 Hult64
  GMT_drywet_gp,                //30-17 Drywet
  ib_jul01_gp,                  //31-18 Jul
  es_vintage_57_gp,             //32-19 Grintage
  ib15_gp,                      //33-20 Rewhi
  Fuschia_7_gp,                     
  Tertiary_01_gp,               //34-21 Tertiary
  es_emerald_dragon_08_gp,                      
  lava_gp,                      //35-22 Fire
  fire_gp,                                     
  fierce_ice_gp,                //36-23 Icefire
  Colorfull_gp,                 //37-24 Cyane
  Pink_Purple_gp,               //38-25 Light Pink  
  es_autumn_19_gp,              //39-26 Autumn
  Magenta_Evening_gp,
  BlacK_Blue_Magenta_White_gp,  //40-27 Magenta
  BlacK_Magenta_Red_gp,         //41-28 Magred
  BlacK_Red_Magenta_Yellow_gp,  //42-29 Yelmag
  Blue_Cyan_Yellow_gp,          //43-30 Yelblu
  Orange_Teal_gp,               //44-31 Orange & Teal
  Tiamat_gp,                    //45-32 Tiamat
  April_Night_gp,               //46-33 April Night
  Orangery_gp,                  //47-34 Orangery
  C9_gp,                        //48-35 C9
  Sakura_gp,                    //49-36 Sakura
  Aurora_gp,                    //50-37 Aurora
  Atlantica_gp,                 //51-38 Atlantica
};


class GradientPalette{
    CRGBPalette32 pl;
    int sh;
    int mn;
    int mx;
    public:
    GradientPalette(CRGBPalette32 pallete, uint8_t shift = 0, uint8_t min = 0, uint8_t max = 0) {
        pl = pallete; sh = shift; mn = min; mx = max;
    }
    CRGB GetColor(uint8_t idx, uint8_t br) {
        if (mn && idx < mn) idx = mn;
        if (mx && idx > mx) idx = mx;
        return ColorFromPalette(pl, (uint8_t)idx + sh, br);
    }
};

class GradientPaletteList{
    LList<GradientPalette*> palletes;
    public:
    GradientPaletteList(): palletes(){

    }
    ~GradientPaletteList(){
        while (palletes.size()) {
            GradientPalette *pl = palletes.shift();
            delete pl;
        }
    }

    GradientPalette *operator[](int i){ return palletes[i]; }

    int size(){ return palletes.size(); }
    void del(int idx){ palletes.remove(idx); }
    int add(CRGBPalette32 pallete, int shift, uint8_t min = 0, uint8_t max = 0) {
        palletes.add(new GradientPalette(pallete, shift, min, max));
        return size();
    }
    int add(int idx, CRGBPalette32 pallete, int shift, uint8_t min = 0, uint8_t max = 0) {
        GradientPalette *pl = palletes.get(idx);
        if (pl) {
            delete pl;
            palletes.set(idx, new GradientPalette(pallete, shift, min, max));
        }
        return size();
    }
};



#endif
