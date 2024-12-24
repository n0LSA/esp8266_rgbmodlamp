#include "../include/constleds.h"
#include "libextern.h"

/*
 ordre a respecter
 fx -> EffectFx_mode::load(uint8_t strip_pos) - line 166
 al -> void EffectWorker::workerset(...) - line 296

*/

PROGMEM effeNamIDList _effeNamIDList [] = {
{ALMLPT_EFF_Pride,                          E_FX_Wave,                  "NONE",         "1", "0", -1},        // 0 
{ALMLPT_EFF_PrideSyncro,                    E_FX_Wave,                  "NONE",         "1", "1", -1},        // 1 
{ALMLPT_EFF_ColorWave,                      E_FX_Colorwave,             "PALDUO",       "1", "0", -1},        // 2 
{ALMLPT_EFF_ColorWaveSyncro,                E_FX_Colorwave,             "PALDUO",       "1", "1", -1},        // 3 
{ALMLPT_EFF_RainbowCycle,                   E_FX_RainbowCycle,          "NONE",         "1", "0", -1},        // 4 
{ALMLPT_EFF_RainbowCycleSyncro,             E_FX_RainbowCycleSync,      "NONE",         "1", "1", -1},        // 5 
{ALMLPT_EFF_Rainbow,                        E_FX_Rainbow,               "NONE",         "1", "0", -1},        // 6 
{ALMLPT_EFF_ColorWipe,                      E_FX_ColorWipe,             "HUETIMER",     "1", "0", -1},        // 7 
{ALMLPT_EFF_ColorWipeSyncro,                E_FX_ColorWipe,             "HUETIMER",     "1", "1", -1},        // 8 
{ALMLPT_EFF_ColorWipeReverseinverse,        E_FX_ColorWipe,             "HUETIMER",     "1", "0", -1},        // 9 
{ALMLPT_EFF_ColorWipeReverseinverseSyncro,  E_FX_ColorWipe,             "HUETIMER",     "1", "1", -1},        // 10 
{ALMLPT_EFF_ColorWipeRandom,                E_FX_ColorWipeRnd,          "NONE",         "1", "0", -1},        // 11 
{ALMLPT_EFF_ColorWipeRandomSyncro,          E_FX_ColorWipeRnd,          "NONE",         "1", "1", -1},        // 12 
{ALMLPT_EFF_TricolorChase,                  E_FX_TricolorChase,         "HTPG",         "1", "0", -1},        // 13 
{ALMLPT_EFF_TricolorChasesyncro,            E_FX_TricolorChaseSync,     "HTPG",         "1", "1", -1},        // 14 
{ALMLPT_EFF_Breath,                         E_FX_Breath,                "HTPG",         "1", "0", -1},        // 15 
{ALMLPT_EFF_Colortwinkle,                   E_FX_Colortwinkle,          "PALGRAD",      "1", "0", -1},        // 16 
{ALMLPT_EFF_Fade,                           E_FX_Fade,                  "HUETIMER",     "1", "0", -1},        // 17 
{ALMLPT_EFF_Scan,                           E_FX_Scan,                  "NONE",         "1", "0", -1},        // 18 
{ALMLPT_EFF_fireworks,                      E_FX_ColorWipe,             "HUETIMER",     "1", "0", -1},        // 19 
{ALMLPT_EFF_fireworksRnd,                   E_FX_Scan,                  "NONE",         "1", "0", -1},        // 20 
{ALMLPT_EFF_tricolor_wipe,                  E_FX_ColorWipe,             "HUETIMER",     "1", "0", -1},        // 21 
{ALMLPT_EFF_theater_chase_rainbow,          E_FX_theater_chase_rainbow, "HUETIMER",     "1", "1", -1},        // 22 
{ALMLPT_EFF_blends,                         E_FX_blends,                "PALDUO",       "1", "0", -1},        // 23
{ALMLPT_EFF_wavesins,                       E_FX_wavesins,              "PALDUO",       "1", "0", -1},        // 24
{ALMLPT_EFF_chase_blackout,                 E_FX_chase_blackout,        "HUETIMER",     "1", "0", -1},        // 25


{ALMLPT_EFF_Commets,                        E_FX_Commets,           "HUETIMER",     "2", "0", -1}, 
{ALMLPT_EFF_Commet,                         E_FX_Commet,            "HUETIMER",     "2", "0", -1}, 
{ALMLPT_EFF_Ruggle,                         E_FX_Move,              "HUETIMER",     "2", "0", -1}, 
{ALMLPT_EFF_RainbowWave,                    E_FX_RainbowWave,       "NONE",         "2", "1", -1}, 
{ALMLPT_EFF_ArmoireOutsideToInside,         E_FX_Armoire_1,         "HUETIMER",     "2", "0", -1}, 
{ALMLPT_EFF_EffectFx_fire2014Syncro,        E_FX_fire2014,          "PALFIRE",      "2", "1", -1}, 
{ALMLPT_EFF_aurora,                         E_FX_Aurora,            "HUETIMER",     "2", "0", -1}, 


#ifdef ALS_LAMPMATRIX
{ALMLPT_EFF_MEffectSparcles,                E_FX_Sparcles,          "HUETIMER",     "2", "0", -1},  
/* MEffectSparcles                      - 0 - 2 */
{ALMLPT_EFF_MEffectWhiteColorStripe,        E_FX_WhiteColorStripe,  "NONE",         "2", "0", -1},  
/* MEffectWhiteColorStrip               - 0 - 2 */
{ALMLPT_EFF_MEffectEverythingFall,          E_FX_EverythingFall,    "PAL",          "2", "0", -1},  
/* MEffectEverythingFall Matrix Effectfall  - 0 - 2 */
{ALMLPT_EFF_MEffectPulse,                   E_FX_Pulse,             "NONE",         "2", "0", -1}, 
/* MEffectPulse Matrix Pulse            - 0 - 2 */
{ALMLPT_EFF_MEffectRainbow,                 E_FX_RAINBOW1,          "NONE",         "2", "0", -1},  
/* MEffectRainbow Matrix Rainbow                     - 0 - 2 */
{ALMLPT_EFF_MEffectMatrix,                  E_FX_matrix,            "HUETIMER",     "2", "0", -1},  
/* MEffectMatrix                        - 0 - 2 */
{ALMLPT_EFF_MEffectStarFall,                E_FX_StarFall,          "HUETIMER",     "2", "0", -1},  
/* MEffectStarFall Matrix Blizzard, Meteor Shower, Meteors  - 0 - 2 */
{ALMLPT_EFF_MEffectLighterTracers,          E_FX_LIGHT2,            "HUETIMER",     "2", "0", -1},  
/* MEffectLighterTracers Matrix Fireflies with a trace                       - 0 - 2 */ 
{ALMLPT_EFF_MEffectLightBalls,              E_FX_ScalSpeed,         "NONE",         "2", "0", -1},  
/* MEffectLightBalls Paintball            - 0 - 2 */
{ALMLPT_EFF_MEffectBall,                    E_FX_ScalSpeed,         "NONE",         "2", "0", -1},  
/* MEffectBall                            - 0 - 2 */
{ALMLPT_EFF_MEffect3DNoise,                 E_FX_3DNOISE,           "PAL",          "2", "0", -1}, 
/* MEffect3DNoise                         - 0 - 2 */
{ALMLPT_EFF_MEffectBBalls,                  E_BBalls,               "NONE",         "2", "0", -1}, 
/* MEffectBBalls                          - 0 - 2 */
{ALMLPT_EFF_MEffectSinusoid3,               E_FX_ScalSpeed,         "NONE",         "2", "0", -1}, 
/* MEffectSinusoid3                       - 0 - 2 */
{ALMLPT_EFF_MEffectMetaBalls,               E_FX_ScalSpeed,         "NONE",         "2", "0", -1}, 
/* MEffectMetaBalls                       - 0 - 2 */
{ALMLPT_EFF_MEffectSpiro,                   E_FX_SPIROP,            "PALDUO",       "2", "0", -1}, 
/* MEffectSpiro                           - 0 - 2 */
{ALMLPT_EFF_MEffectFlower,                  E_FX_Wave,              "NONE",         "2", "0", -1}, 
/* MEffectFlower                          - 0 - 2 */
{ALMLPT_EFF_MEffectSplashBals,              E_FX_SPLASHBALS,        "PAL",          "2", "0", -1}, 
/* MEffectSplashBals                      - 0 - 2 */
{ALMLPT_EFF_MEffectRadialFire,              E_FX_NFIRE,             "PAL",          "2", "0", -1}, 
/* MEffectRadialFire                      - 0 - 2 */
{ALMLPT_EFF_MEffectRadialNuclearNoise,      E_FX_BSS,               "NONE",         "2", "0", -1}, 
/* MEffectRadialNuclearNoise              - 0 - 2 */
{ALMLPT_EFF_MEffectWcolor,                  E_FX_WCOLOR,            "NONE",         "2", "0", -1}, 
/* MEffectWcolor                          - 0 - 2 */
{ALMLPT_EFF_MEffectMirage,                  E_FX_MIRAGE,            "NONE",         "2", "0", -1}, 
/* MEffectMirage                          - 0 - 2 */
// {ALMLPT_EFF_MEffectSmoker,                  E_FX_SMOKER,            "HUETIMER",     "2", "0", -1}, 
/* MEffectSmoker                          - 0 - 2 */
{ALMLPT_EFF_MEffectDNA,                     E_FX_DNA,               "NONE",         "2", "0", -1}, 
/* MEffectDNA                             - 0 - 2 */
{ALMLPT_EFF_MEffectPile,                    E_4PALDUO,              "PALDUO",       "2", "0", -1}, 
/* MEffectPile                            - 0 - 2 */
{ALMLPT_EFF_MEffectFire2021,                E_FX_NFIRE2021,         "PALFIRE",      "2", "0", -1}, 
/* MEffectFire2021                        - 0 - 2 */
{ALMLPT_EFF_MEffectFlags,                   E_FX_FLAGS,             "NONE",         "2", "0", -1}, 
/* MEffectFlags                           - 0 - 2 */
{ALMLPT_EFF_MEffectStarShips,               E_FX_TROOPER,           "PALDUO",       "2", "0", -1}, 
/* MEffectStarShips                       - 0 - 2 */
{ALMLPT_EFF_MEffectMagma,                   E_FX_MAGMA,             "NONE",         "2", "0", -1}, 
/* MEffectMagma                           - 0 - 2 */
{ALMLPT_EFF_MEffectPolarL,                  E_FX_PolarL,            "NONE",         "2", "0", -1}, 
/* MEffectPolarL                          - 0 - 2 */
{ALMLPT_EFF_MEffectRacer,                   E_3PALDUO,              "PALDUO",       "2", "0", -1}, 
/* MEffectRacer                           - 0 - 2 */
{ALMLPT_EFF_MEffectFrizzles,                E_DEFUI,                "NONE",         "2", "0", -1}, 
/* MEffectFrizzles                        - 0 - 2 */
{ALMLPT_EFF_MEffectMaze,                    E_FX_2,                 "NONE",         "2", "0", -1}, 
/* MEffectMaze                            - 0 - 2 */
{ALMLPT_EFF_MEffectBalls,                   E_FX_Balls,             "PALDUO",       "2", "0", -1}, 
/* MEffectBalls                           - 0 - 2 */
{ALMLPT_EFF_MEffectCircles,                 E_4PALDUO,              "PALDUO",       "2", "0", -1}, 
/* MEffectCircles                         - 0 - 2 */
{ALMLPT_EFF_MEffectFairy,                   E_FX_FAIRY,             "NONE",       "2", "0", -1}, 
/* MEffectFairy                           - 0 - 2 */
{ALMLPT_EFF_MEffectBengalL,                 E_FX_BengalL,           "NONE",       "2", "0", -1}, 
/* MEffectBengalL                         - 0 - 2 */
{ALMLPT_EFF_MEffectWrain,                   E_FX_Wrain,             "PAL",       "2", "0", -1}, 
/* MEffectWrain                           - 0 - 2 */
{ALMLPT_EFF_MEffectOscilator,               E_FX_OSCI,              "PALDUO",       "2", "0", -1}, 
/* MEffectOscilator                           - 0 - 2 */




#endif
};

void effectType_toString(OUTPUT_EFFECTTYPE type, String & result){
  switch (type) {
      case OUTPUT_EFFECTTYPE::OET_NONE:     result = "OET_NONE"; break;
      case OUTPUT_EFFECTTYPE::OET_PALGRAD:  result = "OET_PALGRAD"; break;
      case OUTPUT_EFFECTTYPE::OET_PAL:      result = "OET_PAL"; break;
      case OUTPUT_EFFECTTYPE::OET_HTPG:     result = "OET_HTPG"; break;
      case OUTPUT_EFFECTTYPE::OET_HUETIMER: result = "OET_HUETIMER"; break;
      case OUTPUT_EFFECTTYPE::OET_PALDUO:   result = "OET_PALDUO"; break;
      case OUTPUT_EFFECTTYPE::OET_PALFIRE:  result = "OET_PALFIRE"; break;
      default: result = ""; break;
  }
}
OUTPUT_EFFECTTYPE effectType_toMod(const char * name){
  if (al_tools::ch_toString(name) == "NONE")      return  OET_NONE;
  if (al_tools::ch_toString(name) == "PALGRAD")   return  OET_PALGRAD;
  if (al_tools::ch_toString(name) == "PAL")       return  OET_PAL;
  if (al_tools::ch_toString(name) == "PALDUO")    return  OET_PALDUO;
  if (al_tools::ch_toString(name) == "PALFIRE")   return  OET_PALFIRE;
  if (al_tools::ch_toString(name) == "HTPG")      return  OET_HTPG;
  if (al_tools::ch_toString(name) == "HUETIMER")  return  OET_HUETIMER;
  return  OET_NONE;
}
uint8_t _effeNamIDList_cnt = ARRAY_SIZE(_effeNamIDList);