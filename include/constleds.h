#ifndef CONSTLEDS_H
#define CONSTLEDS_H

#include <Arduino.h>
#include <FastLED.h>
#include "defleds.h"

#define COTNROLS_PREFIX "\"nb\":@nb@,\"eff_es\":\"@name@\""


static const char ALMLPT_LEDS_001[] PROGMEM = DFTINTF_084;//Palette
static const char ALMLPT_LEDS_002[] PROGMEM = DFTINTF_020;//Microphone
static const char ALMLPT_LEDS_003[] PROGMEM = "Refresh effects-list";
static const char ALMLPT_LEDS_004[] PROGMEM = "Save";
static const char ALMLPT_LEDS_005[] PROGMEM = "Luminosité";
static const char ALMLPT_LEDS_006[] PROGMEM = "Vitesse";
static const char ALMLPT_LEDS_007[] PROGMEM = DFTINTF_088; //Echelle
static const char ALMLPT_LEDS_008[] PROGMEM = "Hue"; //
static const char ALMLPT_LEDS_009[] PROGMEM = "Couleur statique"; //

static const char ALMLPT_EFF_Pride                            [] PROGMEM = "Pride";
static const char ALMLPT_EFF_PrideSyncro                      [] PROGMEM = "Pride Syncro";
static const char ALMLPT_EFF_ColorWave                        [] PROGMEM = "ColorWave";
static const char ALMLPT_EFF_ColorWaveSyncro                  [] PROGMEM = "ColorWave Syncro";
static const char ALMLPT_EFF_RainbowCycle                     [] PROGMEM = "Rainbow cycle";
static const char ALMLPT_EFF_RainbowCycleSyncro               [] PROGMEM = "Rainbow cycle Syncro";
static const char ALMLPT_EFF_Rainbow                          [] PROGMEM = "Rainbow";
static const char ALMLPT_EFF_RainbowSyncro                    [] PROGMEM = "Rainbow Syncro";
static const char ALMLPT_EFF_ColorWipe                        [] PROGMEM = "ColorWipe";
static const char ALMLPT_EFF_ColorWipeSyncro                  [] PROGMEM = "ColorWipe Syncro";
static const char ALMLPT_EFF_ColorWipeInverse                 [] PROGMEM = "ColorWipe inverse";
static const char ALMLPT_EFF_ColorWipeInverseSyncro           [] PROGMEM = "ColorWipe inverse Syncro";
static const char ALMLPT_EFF_ColorWipeReverseinverse          [] PROGMEM = "ColorWipe reverse inverse";
static const char ALMLPT_EFF_ColorWipeReverseinverseSyncro    [] PROGMEM = "ColorWipe reverse inverse Syncro";
static const char ALMLPT_EFF_ColorWipeRandom                  [] PROGMEM = "ColorWipe random";
static const char ALMLPT_EFF_ColorWipeRandomSyncro            [] PROGMEM = "ColorWipe random Syncro";
static const char ALMLPT_EFF_TricolorChase                    [] PROGMEM = "Tricolor Chase";
static const char ALMLPT_EFF_TricolorChasesyncro              [] PROGMEM = "Tricolor Chase Syncro";
static const char ALMLPT_EFF_Breath                           [] PROGMEM = "Breath";
static const char ALMLPT_EFF_BreathSyncro                     [] PROGMEM = "Breath Syncro";
static const char ALMLPT_EFF_Colortwinkle                     [] PROGMEM = "Colortwinkle";
static const char ALMLPT_EFF_Fade                             [] PROGMEM = "Fade";
static const char ALMLPT_EFF_Scan                             [] PROGMEM = "Scan";
static const char ALMLPT_EFF_fireworks                        [] PROGMEM = "Fireworks";
static const char ALMLPT_EFF_fireworksRnd                     [] PROGMEM = "Fireworks Random";
static const char ALMLPT_EFF_tricolor_wipe                    [] PROGMEM = "tricolor wipe";
static const char ALMLPT_EFF_theater_chase_rainbow            [] PROGMEM = "theater chase rainbow";
static const char ALMLPT_EFF_blends                           [] PROGMEM = "Blends";
static const char ALMLPT_EFF_wavesins                         [] PROGMEM = "Wavesins";
static const char ALMLPT_EFF_chase_blackout                   [] PROGMEM = "Chase blackout";

static const char ALMLPT_EFF_Commets                          [] PROGMEM = "Commets";
static const char ALMLPT_EFF_Commet                           [] PROGMEM = "Commet";
static const char ALMLPT_EFF_CommetSync                       [] PROGMEM = "Commet Syncro";
static const char ALMLPT_EFF_Bounce                           [] PROGMEM = "Bounce";
static const char ALMLPT_EFF_Ruggle                           [] PROGMEM = "Ruggle";
static const char ALMLPT_EFF_RainbowWave                      [] PROGMEM = "Rainbow Wave";
static const char ALMLPT_EFF_fire_2012                        [] PROGMEM = "Fire 2012";
static const char ALMLPT_EFF_fire_2012Syncro                  [] PROGMEM = "Fire 2012 Syncro";
static const char ALMLPT_EFF_EffectFx_fire2014                [] PROGMEM = "Fire 2014";
static const char ALMLPT_EFF_EffectFx_fire2014Syncro          [] PROGMEM = "Fire 2014 Syncro";
static const char ALMLPT_EFF_aurora                           [] PROGMEM = "Aurora";

static const char ALMLPT_EFF_MEffectRainbow                   [] PROGMEM = "Matrix Rainbow";
static const char ALMLPT_EFF_MEffectMatrix                    [] PROGMEM = "Matrix Rain";
static const char ALMLPT_EFF_MEffectSparcles                  [] PROGMEM = "Matrix Confetti";
static const char ALMLPT_EFF_MEffectWhiteColorStripe          [] PROGMEM = "Matrix WhiteColorStripe";
static const char ALMLPT_EFF_MEffectEverythingFall            [] PROGMEM = "Matrix Effectfall";
static const char ALMLPT_EFF_MEffectPulse                     [] PROGMEM = "Matrix Pulse";
static const char ALMLPT_EFF_MEffectStarFall                  [] PROGMEM = "Matrix Blizzard, Meteor Shower, Meteors";
static const char ALMLPT_EFF_MEffectLighterTracers            [] PROGMEM = "Matrix Fireflies with a trace";
static const char ALMLPT_EFF_MEffectLightBalls                [] PROGMEM = "Matrix Paintball";
static const char ALMLPT_EFF_MEffectBall                      [] PROGMEM = "Matrix Wandering cube";
static const char ALMLPT_EFF_MEffect3DNoise                   [] PROGMEM = "Matrix Noise 3D effects";
static const char ALMLPT_EFF_MEffectBBalls                    [] PROGMEM = "Matrix Bouncing balls";
static const char ALMLPT_EFF_MEffectSinusoid3                 [] PROGMEM = "Matrix Sinusoid";
static const char ALMLPT_EFF_MEffectMetaBalls                 [] PROGMEM = "Matrix Metasphere";
static const char ALMLPT_EFF_MEffectSpiro                     [] PROGMEM = "Matrix Spiral";
static const char ALMLPT_EFF_MEffectFlower                    [] PROGMEM = "Matrix Flowering";
static const char ALMLPT_EFF_MEffectSplashBals                [] PROGMEM = "Matrix Splash Balls";
static const char ALMLPT_EFF_MEffectRadialFire                [] PROGMEM = "Matrix Fire";
static const char ALMLPT_EFF_MEffectRadialNuclearNoise        [] PROGMEM = "Matrix RadialNuclearNoise";
static const char ALMLPT_EFF_MEffectWcolor                    [] PROGMEM = "Matrix Aquarelle";
static const char ALMLPT_EFF_MEffectMirage                    [] PROGMEM = "Matrix Mirage";
static const char ALMLPT_EFF_MEffectSmoker                    [] PROGMEM = "Matrix Smoke";
static const char ALMLPT_EFF_MEffectDNA                       [] PROGMEM = "Matrix Adn";
static const char ALMLPT_EFF_MEffectPile                      [] PROGMEM = "Matrix Pile";
static const char ALMLPT_EFF_MEffectPuzzles                   [] PROGMEM = "Matrix Puzzles";
static const char ALMLPT_EFF_MEffectFire2021                  [] PROGMEM = "Matrix Fire 2012";
static const char ALMLPT_EFF_MEffectFlags                     [] PROGMEM = "Matrix Flags";
static const char ALMLPT_EFF_MEffectStarShips                 [] PROGMEM = "Matrix Starship Trooper";
static const char ALMLPT_EFF_MEffectMagma                     [] PROGMEM = "Matrix Magma";
static const char ALMLPT_EFF_MEffectPolarL                    [] PROGMEM = "Matrix PolarL";
static const char ALMLPT_EFF_MEffectRacer                     [] PROGMEM = "Matrix Racer";
static const char ALMLPT_EFF_MEffectFrizzles                  [] PROGMEM = "Matrix Curly Colors";
static const char ALMLPT_EFF_MEffectMaze                      [] PROGMEM = "Matrix Maze";
static const char ALMLPT_EFF_MEffectBalls                     [] PROGMEM = "Matrix Balls";
static const char ALMLPT_EFF_MEffectCircles                   [] PROGMEM = "Matrix Circles";
static const char ALMLPT_EFF_MEffectFairy                     [] PROGMEM = "Matrix Fairy";
static const char ALMLPT_EFF_MEffectBengalL                   [] PROGMEM = "Matrix Bengal Lights";
static const char ALMLPT_EFF_MEffectWrain                     [] PROGMEM = "Matrix Storm";
static const char ALMLPT_EFF_MEffectOscilator                 [] PROGMEM = "Matrix Oscilator";

static const char ALMLPT_EFF_ChaseBlackoutrainbow             [] PROGMEM = "Chase blackout rainbow";
static const char ALMLPT_EFF_TheaterChaserainbow              [] PROGMEM = "Theater chase rainbow";
static const char ALMLPT_EFF_ChaseBlackout                    [] PROGMEM = "Chase blackout";
static const char ALMLPT_EFF_ColorSweeprandom                 [] PROGMEM = "Color sweep random";
static const char ALMLPT_EFF_LarsonScanner                    [] PROGMEM = "Larson scanner";
static const char ALMLPT_EFF_Glow                             [] PROGMEM = "Glow";
static const char ALMLPT_EFF_Pulse                            [] PROGMEM = "Pulse";
static const char ALMLPT_EFF_SineWave                         [] PROGMEM = "SineWave";
static const char ALMLPT_EFF_TriWave                          [] PROGMEM = "TriWave";
static const char ALMLPT_EFF_Wave                             [] PROGMEM = "Wave";
static const char ALMLPT_EFF_ColorPalette                     [] PROGMEM = "ColorPalette";
static const char ALMLPT_EFF_ColorPaletteSyncro               [] PROGMEM = "ColorPalette syncro";
static const char ALMLPT_EFF_Twinklefox                       [] PROGMEM = "Twinklefox";
static const char ALMLPT_EFF_ArmoireStatiquehue               [] PROGMEM = "Armoire statique hue";
static const char ALMLPT_EFF_ArmoireStatiquernd               [] PROGMEM = "Armoire statique rnd";
static const char ALMLPT_EFF_ArmoireCyclehue                  [] PROGMEM = "Armoire cycle hue";
static const char ALMLPT_EFF_ArmoireOutsideToInside           [] PROGMEM = "Armoire OutsideToInside";

/*
0   bri
1   speed
2   scale
3   fade
4   Cyle timer 1
5   variation
6   Taille
7   collor mod      rangew0
8   collor 1        rangew0
9   collor 2        rangew0
10  rep
11  palette 0: cycle > pos-1
12  palette timer
*/
 
static const char E_DEFUI[] PROGMEM = "{" COTNROLS_PREFIX ",\"ctrls\":[]}";
static const char E_FX_2[] PROGMEM = 
  "{" COTNROLS_PREFIX ",\"ctrls\":["
  "{\"id\":2,\"state\":3,\"type\":0,\"val\":127,\"min\":0,\"max\":0}"
  "]}";
//   
static const char E_FX_ScalSpeed []  PROGMEM = 
  "{" COTNROLS_PREFIX ",\"ctrls\":["
  " {\"id\":2,\"type\":0,\"val\":125,\"min\":0,\"max\":255},"
  " {\"id\":1,\"type\":0,\"val\":125,\"min\":0,\"max\":255}"
  "]}"; 

static const char E_4PAL[] PROGMEM = 
  "{" COTNROLS_PREFIX ",\"ctrls\":["
  "{\"id\":2,   \"type\":0,\"val\":0,\"min\":0,\"max\":255},"
  "{\"id\":1,   \"type\":0,\"val\":0,\"min\":0,\"max\":255},"  
  "{\"id\":11,  \"type\":0,\"min\":0,\"max\":" SF(FASTLED_PALETTS_COUNT) ",\"name\":\"" DFTINTF_120 "\"},"
  "{\"id\":12,  \"type\":0,\"val\":20,\"min\":1,\"max\":500,\"name\":\"" DFTINTF_121 "\"}"
  "]}";

static const char E_4PALDUO[] PROGMEM = 
  "{" COTNROLS_PREFIX ",\"ctrls\":["
  "{\"id\":2,   \"type\":0,\"val\":127,\"min\":0,\"max\":255},"
  "{\"id\":1,   \"type\":0,\"val\":127,\"min\":0,\"max\":255},"  
  "{\"id\":7,   \"type\":2,\"val\":0,\"min\":0,\"max\":255,\"name\":\"" DFTINTF_131 "\"},"  
  "{\"id\":11,  \"type\":0,\"min\":0,\"max\":" SF(GRADIENT_PALETTS_COUNT) ",\"name\":\"" DFTINTF_120 "\"},"
  "{\"id\":12,  \"type\":0,\"val\":20,\"min\":1,\"max\":500,\"name\":\"" DFTINTF_121 "\"}"
  "]}";

static const char E_3PALDUO[] PROGMEM = 
  "{" COTNROLS_PREFIX ",\"ctrls\":["
  "{\"id\":2,\"state\":3,\"type\":0,\"val\":0,\"min\":0,\"max\":0},"
  "{\"id\":1,   \"type\":0,\"val\":127,\"min\":0,\"max\":255},"  
  "{\"id\":7,   \"type\":2,\"val\":0,\"min\":0,\"max\":255,\"name\":\"" DFTINTF_131 "\"},"  
  "{\"id\":11,  \"type\":0,\"min\":0,\"max\":" SF(GRADIENT_PALETTS_COUNT) ",\"name\":\"" DFTINTF_120 "\"},"
  "{\"id\":12,  \"type\":0,\"val\":20,\"min\":1,\"max\":500,\"name\":\"" DFTINTF_121 "\"}"
  "]}";

static const char E_FX_Wave []  PROGMEM = 
  "{" COTNROLS_PREFIX ",\"ctrls\":["
  " {\"id\":1,\"val\":100,\"min\":1,\"max\":255},"
  " {\"id\":2,\"state\":3,\"val\":0,\"min\":0,\"max\":0}"
  "]}";

//   
//   
//   
//   
//   
static const char E_FX_Colorwave []  PROGMEM = 
  "{" COTNROLS_PREFIX ",\"ctrls\":["
  " {\"id\":1,\"val\":255,\"min\":1,\"max\":255},"
  " {\"id\":2,\"state\":3,\"val\":0,\"min\":0,\"max\":0},"
  " {\"id\":7,   \"type\":2,\"val\":0,\"min\":0,\"max\":255,\"name\":\"" DFTINTF_131 "\"},"  
  " {\"id\":11,  \"type\":0,\"min\":0,\"max\":" SF(GRADIENT_PALETTS_COUNT) ",\"name\":\"" DFTINTF_120 "\"},"
  " {\"id\":12,  \"type\":0,\"val\":20,\"min\":1,\"max\":500,\"name\":\"" DFTINTF_121 "\"}"  
  "]}";

static const char E_FX_RainbowCycle []  PROGMEM = 
  "{" COTNROLS_PREFIX ",\"ctrls\":["
  " {\"id\":1,\"val\":60,\"min\":0,\"max\":255},"
  " {\"id\":2,\"type\":0,\"val\":255,\"min\":0,\"max\":255},"
  " {\"id\":3,\"type\":0,\"val\":0,\"min\":0,\"max\":255,\"name\":\"" DFTINTF_0ED "\"}"
  "]}";

static const char E_FX_RainbowCycleSync []  PROGMEM = 
  "{" COTNROLS_PREFIX ",\"ctrls\":["
  " {\"id\":1,\"val\":60,\"min\":0,\"max\":255},"
  " {\"id\":2,\"type\":0,\"val\":255,\"min\":0,\"max\":255},"
  " {\"id\":3,\"type\":0,\"val\":30,\"min\":0,\"max\":255,\"name\":\"" DFTINTF_0ED "\"}"
  "]}";

static const char E_FX_Rainbow []  PROGMEM = 
  "{" COTNROLS_PREFIX ",\"ctrls\":["
  " {\"id\":2,\"val\":0,\"min\":0,\"max\":0,\"state\":3},"
  " {\"id\":1,\"type\":3,\"val\":16,\"min\":0,\"max\":255}"
  "]}";  

static const char E_FX_ColorWipe []  PROGMEM = 
  "{" COTNROLS_PREFIX ",\"ctrls\":["
  " {\"id\":1,\"val\":105,\"min\":0,\"max\":255},"
  " {\"id\":2,\"val\":0,\"min\":0,\"max\":0,\"state\":3},"
  " {\"id\":4,\"type\":0,\"val\":40,\"min\":1,\"max\":255,\"name\":\"" DFTINTF_122 "\"},"
  " {\"id\":5,\"type\":0,\"val\":60,\"min\":0,\"max\":255,\"name\":\"" DFTINTF_123 "\"},"
  // " {\"id\":7,\"type\":3,\"val\":1,\"min\":0,\"max\":2,\"name\":\"" DFTINTF_119 "\"},"
  " {\"id\":8,\"type\":3,\"val\":0,\"min\":0,\"max\":255,\"name\":\"" DFTINTF_101 "\"},"
  " {\"id\":9,\"type\":3,\"val\":0,\"min\":0,\"max\":255,\"name\":\"" DFTINTF_113 "\"}"
  "]}";

static const char E_FX_ColorWipeRnd []  PROGMEM = 
  "{" COTNROLS_PREFIX ",\"ctrls\":["
  " {\"id\":1,\"val\":105,\"min\":0,\"max\":255},"
  " {\"id\":2,\"val\":0,\"min\":0,\"max\":0,\"state\":3}"
  "]}";
static const char E_FX_TricolorChase []  PROGMEM = 
  "{" COTNROLS_PREFIX ",\"ctrls\":["
  " {\"id\":1,\"val\":60,\"min\":0,\"max\":255},"
  " {\"id\":2,\"val\":0,\"min\":0,\"max\":0,\"state\":3},"
  " {\"id\":6,\"type\":0,\"val\":2,\"min\":0,\"max\":5,\"name\":\"" DFTINTF_126 "\"},"
  " {\"id\":5,\"type\":0,\"val\":30,\"min\":0,\"max\":255,\"name\":\"" DFTINTF_123 "\"},"
  " {\"id\":4,\"type\":0,\"val\":250,\"min\":1,\"max\":255,\"name\":\"" DFTINTF_122 "\"},"
  // " {\"id\":7,\"type\":3,\"val\":1,\"min\":0,\"max\":2,\"name\":\"" DFTINTF_119 "\"},"
  " {\"id\":8,\"type\":3,\"val\":80,\"min\":0,\"max\":255,\"name\":\"" DFTINTF_101 "\"},"
  " {\"id\":9,\"type\":3,\"val\":130,\"min\":0,\"max\":255,\"name\":\"" DFTINTF_113 "\"}"  
  "]}";
static const char E_FX_TricolorChaseSync []  PROGMEM = 
  "{" COTNROLS_PREFIX ",\"ctrls\":["
  " {\"id\":1,\"val\":80,\"min\":0,\"max\":255},"
  " {\"id\":2,\"val\":0,\"min\":0,\"max\":0,\"state\":3},"
  " {\"id\":6,\"type\":0,\"val\":3,\"min\":0,\"max\":5,\"name\":\"" DFTINTF_126 "\"},"
  " {\"id\":5,\"type\":0,\"val\":30,\"min\":0,\"max\":255,\"name\":\"" DFTINTF_123 "\"},"
  " {\"id\":4,\"type\":0,\"val\":255,\"min\":1,\"max\":255,\"name\":\"" DFTINTF_122 "\"},"
  // " {\"id\":7,\"type\":3,\"val\":1,\"min\":0,\"max\":2,\"name\":\"" DFTINTF_119 "\"},"
  " {\"id\":8,\"type\":3,\"val\":0,\"min\":0,\"max\":255,\"name\":\"" DFTINTF_101 "\"},"
  " {\"id\":9,\"type\":3,\"val\":0,\"min\":0,\"max\":255,\"name\":\"" DFTINTF_113 "\"}"  
  "]}";
static const char E_FX_Breath []  PROGMEM = 
  "{" COTNROLS_PREFIX ",\"ctrls\":["
  " {\"id\":1,\"val\":100,\"min\":0,\"max\":255,\"state\":3},"
  " {\"id\":2,\"val\":0,\"min\":0,\"max\":0,\"state\":3},"
  " {\"id\":4,\"type\":0,\"val\":255,\"min\":1,\"max\":255,\"name\":\"" DFTINTF_122 "\"},"
  " {\"id\":5,\"type\":0,\"val\":60,\"min\":0,\"max\":255,\"name\":\"" DFTINTF_123 "\"},"
  // " {\"id\":7,\"type\":3,\"val\":1,\"min\":0,\"max\":2,\"name\":\"" DFTINTF_119 "\"},"
  " {\"id\":8,\"type\":3,\"val\":0,\"min\":0,\"max\":255,\"name\":\"" DFTINTF_101 "\"},"
  " {\"id\":9,\"type\":3,\"val\":0,\"min\":0,\"max\":255,\"name\":\"" DFTINTF_113 "\"}"
  "]}";
static const char E_FX_Colortwinkle []  PROGMEM = 
  "{" COTNROLS_PREFIX ",\"ctrls\":["
  " {\"id\":1,\"val\":42,\"min\":0,\"max\":255},"
  " {\"id\":2,\"val\":0,\"min\":0,\"max\":0,\"state\":3},"
  " {\"id\":3,\"type\":0,\"val\":255,\"min\":0,\"max\":255,\"name\":\"" DFTINTF_0ED "\"},"
  " {\"id\":11,  \"type\":0,\"min\":0,\"max\":" SF(GRADIENT_PALETTS_COUNT) ",\"name\":\"" DFTINTF_120 "\"},"
  " {\"id\":12,  \"type\":0,\"val\":20,\"min\":1,\"max\":500,\"name\":\"" DFTINTF_121 "\"}"
  "]}";  
static const char E_FX_Fade []  PROGMEM = 
  "{" COTNROLS_PREFIX ",\"ctrls\":["
  " {\"id\":1,\"val\":105,\"min\":0,\"max\":255},"
  " {\"id\":2,\"val\":0,\"min\":0,\"max\":0,\"state\":3},"
  " {\"id\":4,\"type\":0,\"val\":40,\"min\":1,\"max\":255,\"name\":\"" DFTINTF_122 "\"},"
  // " {\"id\":7,\"type\":3,\"val\":1,\"min\":0,\"max\":2,\"name\":\"" DFTINTF_119 "\"},"
  " {\"id\":8,\"type\":3,\"val\":0,\"min\":0,\"max\":255,\"name\":\"" DFTINTF_101 "\"}"
  "]}";
static const char E_FX_FIRE2012[] PROGMEM = 
  "{" COTNROLS_PREFIX ",\"ctrls\":["
  "{\"id\":0,   \"type\":0,\"val\":125,\"min\":1,\"max\":255},"
  "{\"id\":1,   \"type\":0,\"val\":125,\"min\":1,\"max\":255},"
  "{\"id\":2,   \"type\":0,\"val\":127,\"min\":0,\"max\":255 ,\"name\":\"" DFTINTF_088 "\"}," // "Echelle"
  // "{\"id\":5,   \"type\":2,\"val\":0,\"min\":0,\"max\":255 ,\"name\":\"" DFTINTF_0F9 "\"}," // "Comportement: 1/2"
  "{\"id\":11,  \"type\":0,\"min\":0,\"max\":" SF(FIRE_PALETTS_COUNT) ",\"name\":\"" DFTINTF_120 "\"},"
  "{\"id\":12,  \"type\":0,\"val\":20,\"min\":1,\"max\":500,\"name\":\"" DFTINTF_121 "\"}"
  "]}";  
static const char E_FX_Scan []  PROGMEM = 
  "{" COTNROLS_PREFIX ",\"ctrls\":["
  " {\"id\":1,\"val\":105,\"min\":0,\"max\":255},"
  " {\"id\":2,\"val\":0,\"min\":0,\"max\":255}"
  "]}";
static const char E_FX_fireworks []  PROGMEM = 
  "{" COTNROLS_PREFIX ",\"ctrls\":["
  " {\"id\":1,\"val\":105,\"min\":0,\"max\":255},"
  " {\"id\":8,   \"type\":0,\"val\":80,\"min\":0,\"max\":255 ,\"name\":\"" DFTINTF_124 "\"}," // "couleur 1"
  " {\"id\":9,   \"type\":0,\"val\":127,\"min\":0,\"max\":255 ,\"name\":\"" DFTINTF_125 "\"}," // "couleur 2"
  " {\"id\":2,\"val\":0,\"min\":0,\"max\":255}"
  "]}";
static const char E_FX_fire2014[] PROGMEM = 
  "{" COTNROLS_PREFIX ",\"ctrls\":["
  "{\"id\":0,   \"type\":0,\"val\":125,\"min\":1,\"max\":255},"
  "{\"id\":1,   \"type\":0,\"val\":125,\"min\":1,\"max\":255},"
  "{\"id\":2,\"state\":3,   \"type\":0,\"val\":50,\"min\":0,\"max\":100 ,\"name\":\"" DFTINTF_088 "\"},"
  "{\"id\":11,  \"type\":0,\"min\":0,\"max\":" SF(FIRE_PALETTS_COUNT) ",\"name\":\"" DFTINTF_120 "\"},"
  "{\"id\":12,  \"type\":0,\"val\":20,\"min\":1,\"max\":500,\"name\":\"" DFTINTF_121 "\"}"
  "]}";
static const char E_FX_theater_chase_rainbow []  PROGMEM = 
  "{" COTNROLS_PREFIX ",\"ctrls\":["
  " {\"id\":1,\"val\":105,\"min\":0,\"max\":255},"
  " {\"id\":2,\"val\":0,\"min\":0,\"max\":0,\"state\":3},"
  " {\"id\":4,\"type\":0,\"val\":40,\"min\":1,\"max\":255,\"name\":\"" DFTINTF_122 "\"},"
  " {\"id\":5,\"type\":0,\"val\":60,\"min\":0,\"max\":255,\"name\":\"" DFTINTF_123 "\"},"
  // " {\"id\":7,\"type\":3,\"val\":1,\"min\":0,\"max\":2,\"name\":\"" DFTINTF_119 "\"},"
  " {\"id\":8,\"type\":3,\"val\":0,\"min\":0,\"max\":255,\"name\":\"" DFTINTF_101 "\"}"
  // " {\"id\":9,\"type\":3,\"val\":0,\"min\":0,\"max\":255,\"name\":\"" DFTINTF_113 "\"}"
  "]}";

static const char E_FX_blends []  PROGMEM = 
  "{" COTNROLS_PREFIX ",\"ctrls\":["
  " {\"id\":1,\"val\":245,\"min\":1,\"max\":255},"
  " {\"id\":2,\"val\":38,\"min\":0,\"max\":255},"
  " {\"id\":7,   \"type\":2,\"val\":0,\"min\":0,\"max\":0,\"name\":\"" DFTINTF_131 "\"},"  
  " {\"id\":11,  \"type\":0,\"val\":0,\"min\":0,\"max\":" SF(GRADIENT_PALETTS_COUNT) ",\"name\":\"" DFTINTF_120 "\"},"
  " {\"id\":12,  \"type\":0,\"val\":25,\"min\":1,\"max\":500,\"name\":\"" DFTINTF_121 "\"}"  
  "]}";

 
static const char E_FX_wavesins []  PROGMEM = 
  "{" COTNROLS_PREFIX ",\"ctrls\":["
  " {\"id\":1,\"val\":127,\"min\":1,\"max\":255},"
  " {\"id\":3,\"val\":127,\"min\":0,\"max\":255,\"name\":\"" DFTINTF_137 "\"},"
  " {\"id\":2,\"val\":127,\"min\":0,\"max\":255,\"name\":\"" DFTINTF_138 "\"},"
  " {\"id\":6,\"val\":127,\"min\":0,\"max\":255,\"name\":\"" DFTINTF_139 "\"},"
  " {\"id\":5,\"val\":127,\"min\":0,\"max\":255,\"name\":\"" DFTINTF_140 "\"},"
  " {\"id\":7,\"type\":2,\"val\":0,\"min\":0,\"max\":0,\"name\":\"" DFTINTF_131 "\"},"  
  " {\"id\":11,\"type\":0,\"val\":0,\"min\":0,\"max\":" SF(GRADIENT_PALETTS_COUNT) ",\"name\":\"" DFTINTF_120 "\"},"
  " {\"id\":12,\"type\":0,\"val\":25,\"min\":1,\"max\":500,\"name\":\"" DFTINTF_121 "\"}"  
  "]}";

static const char E_FX_chase_blackout []  PROGMEM = 
  "{" COTNROLS_PREFIX ",\"ctrls\":["
  " {\"id\":1,\"val\":105,\"min\":0,\"max\":255},"
  " {\"id\":2,\"val\":0,\"min\":0,\"max\":0,\"state\":3},"
  " {\"id\":4,\"type\":0,\"val\":40,\"min\":1,\"max\":255,\"name\":\"" DFTINTF_122 "\"},"
  " {\"id\":8,\"type\":3,\"val\":0,\"min\":0,\"max\":255,\"name\":\"" DFTINTF_101 "\"}"
  "]}";


// #################################################################################################
// #################################################################################################
// #################################################################################################
// #################################################################################################
static const char E_FX_Commets []  PROGMEM = 
  "{" COTNROLS_PREFIX ",\"ctrls\":["
  " {\"id\":1,\"val\":127,\"min\":0,\"max\":255},"
  " {\"id\":2,\"state\":3,\"val\":0,\"min\":0,\"max\":0},"
  " {\"id\":6,\"type\":0,\"val\":127,\"min\":0,\"max\":255,\"name\":\"" DFTINTF_129 "\"},"
  " {\"id\":4,\"type\":0,\"val\":255,\"min\":1,\"max\":255,\"name\":\"" DFTINTF_122 "\"},"
  " {\"id\":8,\"type\":3,\"val\":0,\"min\":0,\"max\":255,\"name\":\"" DFTINTF_101 "\"}"
  "]}";

static const char E_FX_Commet []  PROGMEM = 
  "{" COTNROLS_PREFIX ",\"ctrls\":["
  " {\"id\":1,\"val\":127,\"min\":0,\"max\":255},"
  " {\"id\":2,\"val\":0,\"min\":0,\"max\":1},"
  "{\"id\":5,   \"type\":2,\"val\":0,\"min\":0,\"max\":255 ,\"name\":\"" DFTINTF_0F9 "\"},"
  " {\"id\":4,\"type\":0,\"val\":255,\"min\":1,\"max\":255,\"name\":\"" DFTINTF_122 "\"},"
  " {\"id\":8,\"type\":3,\"val\":0,\"min\":0,\"max\":255,\"name\":\"" DFTINTF_101 "\"}"
  "]}";   

static const char E_FX_Move []  PROGMEM = 
  "{" COTNROLS_PREFIX ",\"ctrls\":["
  " {\"id\":1,\"val\":127,\"min\":0,\"max\":255},"
  " {\"id\":2,\"type\":0,\"val\":3,\"min\":1,\"max\":12,\"name\":\"" DFTINTF_135 "\"},"
  " {\"id\":6,\"type\":0,\"val\":120,\"min\":1,\"max\":255,\"name\":\"" DFTINTF_0ED "\"},"
  " {\"id\":5,\"type\":0,\"val\":60,\"min\":0,\"max\":255,\"name\":\"" DFTINTF_123 "\"},"
  " {\"id\":4,\"type\":0,\"val\":40,\"min\":1,\"max\":255,\"name\":\"" DFTINTF_122 "\"},"
  " {\"id\":8,\"type\":3,\"val\":80,\"min\":0,\"max\":255,\"name\":\"" DFTINTF_101 "\"}"
  "]}";

static const char E_FX_RainbowWave []  PROGMEM = 
  "{" COTNROLS_PREFIX ",\"ctrls\":["
  " {\"id\":2,\"val\":69,\"min\":0,\"max\":255},"
  " {\"id\":1,\"val\":40,\"min\":0,\"max\":255}"
  "]}";

static const char E_FX_Armoire_1 []  PROGMEM = 
  "{" COTNROLS_PREFIX ",\"ctrls\":["
  " {\"id\":1,\"val\":0,\"min\":0,\"max\":0,\"state\":3},"
  " {\"id\":2,\"val\":0,\"min\":0,\"max\":0,\"state\":3},"
  " {\"id\":5,\"type\":0,\"val\":30,\"min\":0,\"max\":255,\"name\":\"" DFTINTF_123 "\"},"
  " {\"id\":4,\"type\":0,\"val\":255,\"min\":1,\"max\":255,\"name\":\"" DFTINTF_122 "\"},"
  " {\"id\":8,\"type\":3,\"val\":0,\"min\":0,\"max\":255,\"name\":\"" DFTINTF_101 "\"}"
  "]}";

static const char E_FX_Aurora []  PROGMEM = 
  "{" COTNROLS_PREFIX ",\"ctrls\":["
  " {\"id\":1,\"val\":127,\"min\":0,\"max\":255},"
  " {\"id\":2,\"val\":0,\"min\":0,\"max\":255},"
  " {\"id\":5,\"type\":2,\"val\":0,\"min\":0,\"max\":255 ,\"name\":\"" DFTINTF_0F9 "\"},"
  " {\"id\":4,\"type\":0,\"val\":255,\"min\":1,\"max\":255,\"name\":\"" DFTINTF_122 "\"},"
  " {\"id\":8,\"type\":3,\"val\":0,\"min\":0,\"max\":255,\"name\":\"" DFTINTF_101 "\"}"
  "]}";  

// #################################################################################################
// #################################################################################################
// #################################################################################################
// #################################################################################################
static const char E_FX_Sparcles[]  PROGMEM = 
  "{" COTNROLS_PREFIX ",\"ctrls\":["
    " {\"id\":1,\"type\":0,\"val\":57,\"min\":1,\"max\":255},"      // 
    " {\"id\":5,\"type\":0,\"val\":0,\"min\":0,\"max\":2,\"name\":\"" DFTINTF_130 "\"},"      // Couleur (0: aléatoire, 1: couleurs, 2: blanc)
    " {\"id\":4,\"type\":0,\"val\":255,\"min\":1,\"max\":255,\"name\":\"" DFTINTF_122 "\"},"  // Cyle
    " {\"id\":8,\"type\":3,\"val\":0,\"min\":0,\"max\":255,\"name\":\"" DFTINTF_101 "\"}"    // "Couleur 1 (0: cyclique)"
  "]}";

static const char E_FX_WhiteColorStripe []  PROGMEM = 
  "{" COTNROLS_PREFIX ",\"ctrls\":["
  " {\"id\":2,\"type\":0,\"val\":0,\"min\":0,\"max\":255},"
  " {\"id\":1,\"type\":0,\"state\":3 ,\"val\":0,\"min\":0,\"max\":255}"
  "]}"; 

static const char E_FX_EverythingFall[]  PROGMEM = 
  "{" COTNROLS_PREFIX ",\"ctrls\":["
    " {\"id\":1,\"type\":0,\"val\":125,\"min\":1,\"max\":255},"      // 
    " {\"id\":2,\"type\":0,\"val\":107,\"min\":0,\"max\":255},"      //
    " {\"id\":11,  \"type\":0,\"val\":17,\"min\":0,\"max\":" SF(FASTLED_PALETTS_COUNT) ",\"name\":\"" DFTINTF_120 "\"},"
    " {\"id\":12,  \"type\":0,\"val\":20,\"min\":1,\"max\":500,\"name\":\"" DFTINTF_121 "\"}"
  "]}";

static const char E_FX_Pulse []  PROGMEM = 
  "{" COTNROLS_PREFIX ",\"ctrls\":["
  " {\"id\":2,\"type\":0,\"val\":41,\"min\":0,\"max\":255},"
  " {\"id\":1,\"type\":0,\"val\":9,\"min\":0,\"max\":255}"
  "]}"; 

static const char E_FX_RAINBOW1 []  PROGMEM = 
  "{" COTNROLS_PREFIX ",\"ctrls\":["
  " {\"id\":2,\"type\":0,\"val\":128,\"min\":0,\"max\":255},"
  " {\"id\":1,\"type\":0,\"val\":19,\"min\":0,\"max\":255}"
  "]}"; 

static const char E_FX_matrix []  PROGMEM = 
  "{" COTNROLS_PREFIX ",\"ctrls\":["
  " {\"id\":1,\"val\":127,\"min\":0,\"max\":255},"
  " {\"id\":2,\"val\":0,\"min\":0,\"max\":32},"
  " {\"id\":5,\"type\":0,\"val\":0,\"min\":0,\"max\":2,\"name\":\"" DFTINTF_130 "\"},"      // Couleur (0: aléatoire, 1: couleurs, 2: blanc)
  " {\"id\":4,\"type\":0,\"val\":255,\"min\":1,\"max\":255,\"name\":\"" DFTINTF_122 "\"},"  // Cyle
  " {\"id\":8,\"type\":3,\"val\":0,\"min\":0,\"max\":255,\"name\":\"" DFTINTF_101 "\"},"    // "Couleur 1 (0: cyclique)"
  " {\"id\":7,\"type\":3,\"val\":20,\"min\":0,\"max\":60,\"name\":\"" DFTINTF_10A "\"}" // 
  "]}";

static const char E_FX_StarFall[] PROGMEM = 
  "{" COTNROLS_PREFIX ",\"ctrls\":["
  "{\"id\":1,\"val\":100,\"min\":1,\"max\":255},"
  "{\"id\":2,\"val\":4,\"min\":0,\"max\":10,\"name\":\"" DFTINTF_0FE "\"},"
  "{\"id\":3,\"val\":2,\"max\":3,\"name\":\"" DFTINTF_0FD "\"} ," 
  "{\"id\":6,\"val\":0,\"min\":0,\"max\":2,\"name\":\"" DFTINTF_130 "\"}," 
  "{\"id\":4,\"type\":0,\"val\":75,\"min\":1,\"max\":255,\"name\":\"" DFTINTF_122 "\"},"  // Cyle
  "{\"id\":8,\"type\":0,\"val\":0,\"min\":0,\"max\":255,\"name\":\"" DFTINTF_101 "\"},"    // "Couleur 1 (0: cyclique)"
  "{\"id\":5,\"type\":2,\"val\":0,\"name\":\"" DFTINTF_108 "\"}" 
  "]}"; 

static const char E_FX_LIGHT2[] PROGMEM = 
  "{" COTNROLS_PREFIX ",\"ctrls\":["
  "{\"id\":2,\"val\":8,\"max\":16,\"name\":\"" DFTINTF_0D5 "\"},"
  "{\"id\":6,\"val\":0,\"min\":0,\"max\":2,\"name\":\"" DFTINTF_130 "\"},"
  "{\"id\":4,\"type\":0,\"val\":255,\"min\":1,\"max\":255,\"name\":\"" DFTINTF_122 "\"},"  // Cyle
  "{\"id\":8,\"type\":3,\"val\":0,\"min\":0,\"max\":255,\"name\":\"" DFTINTF_101 "\"}"    // "Couleur 1 (0: cyclique)"
  "]}";

static const char E_FX_3DNOISE[] PROGMEM = 
  "{" COTNROLS_PREFIX ",\"ctrls\":["
  "{\"id\":2,   \"type\":0,\"val\":0,\"min\":0,\"max\":255},"
  "{\"id\":1,   \"type\":0,\"val\":0,\"min\":0,\"max\":255},"  
  "{\"id\":7,   \"type\":2,\"val\":1,\"min\":0,\"max\":0,\"name\":\"" DFTINTF_0F5 "\"},"  
  "{\"id\":5,   \"type\":2,\"val\":1,\"min\":0,\"max\":0,\"name\":\"" DFTINTF_0E6 "\"},"  
  "{\"id\":11,  \"type\":0,\"min\":0,\"max\":" SF(FASTLED_PALETTS_COUNT) ",\"name\":\"" DFTINTF_120 "\"},"
  "{\"id\":12,  \"type\":0,\"val\":20,\"min\":1,\"max\":500,\"name\":\"" DFTINTF_121 "\"}"
  "]}";

static const char E_BBalls[] PROGMEM = 
  "{" COTNROLS_PREFIX ",\"ctrls\":["
  "{\"id\":2,\"state\":3,\"type\":48},"
  "{\"id\":3,\"val\":16,\"max\":32,\"name\":\"" DFTINTF_088 "\"},"
  "{\"id\":4,\"val\":0,\"type\":2,\"val\":0,\"name\":\"" DFTINTF_0E1 "\"}"
  "]}";

static const char E_FX_SPIROP[] PROGMEM = 
  "{" COTNROLS_PREFIX ",\"ctrls\":["
  "{\"id\":2,   \"state\":3, \"type\":0,\"val\":0,\"min\":0,\"max\":255},"
  "{\"id\":1,   \"type\":0,\"val\":151,\"min\":0,\"max\":255},"  
  "{\"id\":7,   \"type\":2,\"val\":1,\"min\":0,\"max\":255,\"name\":\"" DFTINTF_131 "\"},"  
  "{\"id\":11,  \"type\":0,\"val\":0,\"min\":0,\"max\":" SF(GRADIENT_PALETTS_COUNT) ",\"name\":\"" DFTINTF_120 "\"},"
  "{\"id\":12,  \"type\":0,\"val\":7,\"min\":1,\"max\":500,\"name\":\"" DFTINTF_121 "\"}"
  "]}";
static const char E_FX_SPIRO[] PROGMEM = 
  "{" COTNROLS_PREFIX ",\"ctrls\":["
  "{\"id\":2,   \"state\":3, \"type\":0,\"val\":0,\"min\":0,\"max\":255},"
  "{\"id\":1,   \"type\":0,\"val\":0,\"min\":0,\"max\":255},"  
  "{\"id\":11,  \"type\":0,\"min\":0,\"max\":" SF(FASTLED_PALETTS_COUNT) ",\"name\":\"" DFTINTF_120 "\"},"
  "{\"id\":12,  \"type\":0,\"val\":20,\"min\":1,\"max\":500,\"name\":\"" DFTINTF_121 "\"}"
  "]}";

static const char E_FX_SPLASHBALS[] PROGMEM = 
  "{" COTNROLS_PREFIX ",\"ctrls\":["
  "{\"id\":2,   \"type\":0,\"state\":3 ,\"val\":0,\"min\":0,\"max\":255},"
  "{\"id\":1,   \"type\":0,\"val\":80,\"min\":0,\"max\":255},"  
  "{\"id\":6,   \"type\":0,\"val\":3,\"min\":0,\"max\":6,\"name\":\"" DFTINTF_0D5 "\"},"  
  "{\"id\":11,  \"type\":0,\"val\":30,\"min\":0,\"max\":" SF(FASTLED_PALETTS_COUNT) ",\"name\":\"" DFTINTF_120 "\"},"
  "{\"id\":12,  \"type\":0,\"val\":20,\"min\":1,\"max\":500,\"name\":\"" DFTINTF_121 "\"}"
  "]}";

static const char E_FX_NFIRE[] PROGMEM = 
  "{" COTNROLS_PREFIX ",\"ctrls\":["
  "{\"id\":0,   \"type\":0,\"val\":125,\"min\":1,\"max\":255},"
  "{\"id\":1,   \"type\":0,\"val\":125,\"min\":1,\"max\":255},"
  "{\"id\":2,   \"type\":0,\"val\":50,\"min\":0,\"max\":100 ,\"name\":\"" DFTINTF_088 "\"},"
  "{\"id\":5,   \"type\":2,\"val\":0,\"min\":0,\"max\":255 ,\"name\":\"" DFTINTF_0F9 "\"},"
  "{\"id\":11,  \"type\":0,\"min\":0,\"max\":" SF(FASTLED_PALETTS_COUNT) ",\"name\":\"" DFTINTF_120 "\"},"
  "{\"id\":12,  \"type\":0,\"val\":20,\"min\":1,\"max\":500,\"name\":\"" DFTINTF_121 "\"}"
  "]}";

static const char E_FX_BSS []  PROGMEM = 
  "{" COTNROLS_PREFIX ",\"ctrls\":["
  " {\"id\":0,\"val\":127,\"min\":1,\"max\":255},"
  " {\"id\":1,\"val\":100,\"min\":1,\"max\":255},"
  " {\"id\":2,\"val\":2,\"min\":1,\"max\":6}"
  "]}";

static const char E_FX_WCOLOR []  PROGMEM = 
  "{" COTNROLS_PREFIX ",\"ctrls\":["
  " {\"id\":0,\"val\":127,\"min\":1,\"max\":255},"
  " {\"id\":1,\"val\":100,\"min\":1,\"max\":255},"
  " {\"id\":2,\"val\":4,\"min\":1,\"max\":8,\"name\":\"" DFTINTF_0EF "\"},"
  " {\"id\":5,\"type\":2,\"val\":0,\"min\":0,\"max\":255 ,\"name\":\"" DFTINTF_0F9 "\"}"
  "]}";

static const char E_FX_MIRAGE []  PROGMEM = 
  "{" COTNROLS_PREFIX ",\"ctrls\":["
  " {\"id\":0,\"val\":127,\"min\":1,\"max\":255},"
  " {\"id\":1,\"val\":127,\"min\":1,\"max\":255},"
  " {\"id\":2,\"val\":160,\"min\":0,\"max\":255,\"name\":\"" DFTINTF_101 "\"}"
  "]}";

static const char E_FX_DNA []  PROGMEM = 
  "{" COTNROLS_PREFIX ",\"ctrls\":["
  " {\"id\":0,\"val\":127,\"min\":1,\"max\":255},"
  " {\"id\":1,\"val\":100,\"min\":1,\"max\":255},"
  " {\"id\":3,\"val\":1,\"min\":0,\"max\":3,\"name\":\"" DFTINTF_0DC "\"},"
  " {\"id\":2,\"val\":10,\"min\":2,\"max\":16,\"name\":\"" DFTINTF_0D2 "\"},"
  " {\"id\":5,\"type\":2,\"val\":0,\"min\":0,\"max\":0,\"name\":\"" DFTINTF_0F9 "\"}"
  "]}";

static const char E_FX_NFIRE2021[] PROGMEM = 
  "{" COTNROLS_PREFIX ",\"ctrls\":["
  "{\"id\":0,   \"type\":0,\"val\":125,\"min\":1,\"max\":255},"
  "{\"id\":1,   \"type\":0,\"val\":125,\"min\":1,\"max\":255},"
  "{\"id\":2,   \"type\":0,\"val\":50,\"min\":0,\"max\":100 ,\"name\":\"" DFTINTF_088 "\"},"
  "{\"id\":5,   \"type\":2,\"val\":0,\"min\":0,\"max\":255 ,\"name\":\"" DFTINTF_0F9 "\"},"
  "{\"id\":11,  \"type\":0,\"min\":0,\"max\":" SF(FIRE_PALETTS_COUNT) ",\"name\":\"" DFTINTF_120 "\"},"
  "{\"id\":12,  \"type\":0,\"val\":20,\"min\":1,\"max\":500,\"name\":\"" DFTINTF_121 "\"}"
  "]}";

static const char E_FX_FLAGS []  PROGMEM = 
  "{" COTNROLS_PREFIX ",\"ctrls\":["
  " {\"id\":0,\"val\":127,\"min\":1,\"max\":255},"
  " {\"id\":1,\"val\":127,\"min\":1,\"max\":255},"
  " {\"id\":2,\"state\":3,\"val\":0,\"min\":0,\"max\":0},"
  " {\"id\":3,\"val\":8,\"min\":0,\"max\":10,\"name\":\"" DFTINTF_00A "\"}"
  "]}";

static const char E_FX_TROOPER[] PROGMEM = 
  "{" COTNROLS_PREFIX ",\"ctrls\":["
  "{\"id\":1,   \"type\":0,\"val\":138,\"min\":0,\"max\":255},"  
  "{\"id\":2,   \"type\":0,\"val\":6,\"min\":0,\"max\":15,\"name\":\"" DFTINTF_0EF "\"},"  
  "{\"id\":5,   \"type\":0,\"val\":7,\"min\":0,\"max\":8,\"name\":\"" DFTINTF_132 "\"},"  
  "{\"id\":7,   \"type\":2,\"val\":0,\"min\":0,\"max\":255,\"name\":\"" DFTINTF_131 "\"},"  
  "{\"id\":11,  \"type\":0,\"val\":1,\"min\":0,\"max\":" SF(GRADIENT_PALETTS_COUNT) ",\"name\":\"" DFTINTF_120 "\"},"
  "{\"id\":12,  \"type\":0,\"val\":20,\"min\":1,\"max\":500,\"name\":\"" DFTINTF_121 "\"}"
  "]}";

static const char E_FX_MAGMA[] PROGMEM = 
  "{" COTNROLS_PREFIX ",\"ctrls\":["
  "{\"id\":1,   \"type\":0,\"val\":125,\"min\":0,\"max\":255},"  
  "{\"id\":2,   \"type\":0,\"val\":50,\"min\":0,\"max\":100,\"name\":\"" DFTINTF_0F9 "\"},"  
  "{\"id\":5,   \"type\":0,\"min\":0,\"min\":0,\"max\":12,\"name\":\"" DFTINTF_084 "\"},"
  "{\"id\":12,  \"type\":0,\"val\":20,\"min\":1,\"max\":500,\"name\":\"" DFTINTF_121 "\"}"
  "]}";

static const char E_FX_PolarL []  PROGMEM = 
  "{" COTNROLS_PREFIX ",\"ctrls\":["
  " {\"id\":3,\"val\":1,\"min\":0,\"max\":14,\"name\":\"" DFTINTF_084 "\"}"
  "]}";

static const char E_FX_Balls[] PROGMEM = 
  "{" COTNROLS_PREFIX ",\"ctrls\":["
  "{\"id\":2,   \"type\":0,\"val\":15,\"min\":0,\"max\":255,\"name\":\"" DFTINTF_0FE "\"},"
  "{\"id\":1,   \"type\":0,\"val\":90,\"min\":0,\"max\":255},"  
  "{\"id\":7,   \"type\":2,\"val\":1,\"min\":0,\"max\":255,\"name\":\"" DFTINTF_131 "\"},"  
  "{\"id\":11,  \"type\":0,\"val\":0,\"min\":0,\"max\":" SF(GRADIENT_PALETTS_COUNT) ",\"name\":\"" DFTINTF_120 "\"},"
  "{\"id\":12,  \"type\":0,\"val\":5,\"min\":1,\"max\":500,\"name\":\"" DFTINTF_121 "\"}"
  "]}";

static const char E_FX_FAIRY[]  PROGMEM = 
  "{" COTNROLS_PREFIX ",\"ctrls\":["
  " {\"id\":2,\"type\":0,\"val\":125,\"min\":0,\"max\":255},"
  " {\"id\":1,\"type\":0,\"val\":125,\"min\":0,\"max\":255},"
  // " {\"id\":4,\"type\":0,\"val\":125,\"min\":0,\"max\":255},"
  " {\"id\":3,\"type\":2,\"val\":1,\"min\":0,\"max\":0,\"name\":\"" DFTINTF_0F9 "\"}"
  "]}"; 

static const char E_FX_BengalL[]  PROGMEM = 
  "{" COTNROLS_PREFIX ",\"ctrls\":["
  " {\"id\":2,\"type\":0,\"val\":41,\"min\":0,\"max\":255,\"name\":\"" DFTINTF_0EF "\"},"
  " {\"id\":1,\"type\":0,\"val\":80,\"min\":0,\"max\":255},"
  " {\"id\":3,\"type\":2,\"val\":0,\"min\":0,\"max\":0,\"name\":\"" DFTINTF_134 "\"}"
  "]}"; 

static const char E_FX_Wrain[] PROGMEM = 
  "{" COTNROLS_PREFIX ",\"ctrls\":["
  "{\"id\":1,   \"type\":0,\"val\":125,\"min\":0,\"max\":255},"  
  "{\"id\":2,   \"type\":0,\"val\":25,\"min\":0,\"max\":45,\"name\":\"" DFTINTF_0EF "\"},"  
  "{\"id\":5,   \"type\":2,\"val\":0,\"min\":0,\"max\":8,\"name\":\"" DFTINTF_102 "\"},"  
  "{\"id\":6,   \"type\":2,\"val\":0,\"min\":0,\"max\":255,\"name\":\"" DFTINTF_10B "\"},"  
  "{\"id\":10,  \"type\":0,\"val\":1,\"min\":1,\"max\":4,\"name\":\"" DFTINTF_115 "\"},"  
  "{\"id\":3,   \"type\":0,\"val\":0,\"min\":0,\"max\":3,\"name\":\"" DFTINTF_133 "\"},"  
  // "{\"id\":7,   \"type\":2,\"val\":0,\"min\":0,\"max\":255,\"name\":\"" DFTINTF_131 "\"},"  
  "{\"id\":11,  \"type\":0,\"val\":4,\"min\":0,\"max\":" SF(FASTLED_PALETTS_COUNT) ",\"name\":\"" DFTINTF_120 "\"},"
  "{\"id\":12,  \"type\":0,\"val\":20,\"min\":1,\"max\":500,\"name\":\"" DFTINTF_121 "\"}"
  "]}";
  
static const char E_FX_OSCI[] PROGMEM = 
  "{" COTNROLS_PREFIX ",\"ctrls\":["
  "{\"id\":1,   \"type\":0,\"val\":125,\"min\":0,\"max\":255},"  
  "{\"id\":2,   \"state\":3,\"type\":0,\"val\":25,\"min\":0,\"max\":45,\"name\":\"" DFTINTF_0EF "\"},"  
  "{\"id\":7,   \"type\":2,\"val\":0,\"min\":0,\"max\":255,\"name\":\"" DFTINTF_131 "\"},"  
  "{\"id\":11,  \"type\":0,\"val\":4,\"min\":0,\"max\":" SF(GRADIENT_PALETTS_COUNT) ",\"name\":\"" DFTINTF_120 "\"},"
  "{\"id\":12,  \"type\":0,\"val\":20,\"min\":1,\"max\":500,\"name\":\"" DFTINTF_121 "\"}"
  "]}";



//#############################################################
//#############################################################
//#############################################################
static const char E_FX_PUZZLE[] PROGMEM = 
  "{" COTNROLS_PREFIX ",\"ctrls\":["
  "{\"id\":2,   \"state\":3,\"type\":0,\"val\":127,\"min\":0,\"max\":255},"
  "{\"id\":1,   \"type\":0,\"val\":127,\"min\":0,\"max\":255},"  
  "{\"id\":3,   \"type\":0,\"val\":4,\"min\":4,\"max\":8,\"name\":\"" DFTINTF_10D "\"},"  
  "{\"id\":7,   \"type\":2,\"val\":0,\"min\":0,\"max\":255,\"name\":\"" DFTINTF_131 "\"},"  
  "{\"id\":11,  \"type\":0,\"min\":0,\"max\":" SF(GRADIENT_PALETTS_COUNT) ",\"name\":\"" DFTINTF_120 "\"},"
  "{\"id\":12,  \"type\":0,\"val\":20,\"min\":1,\"max\":500,\"name\":\"" DFTINTF_121 "\"}"
  "]}";

static const char E_FX_SMOKER[] PROGMEM = 
"{" COTNROLS_PREFIX ",\"ctrls\":["
" {\"id\":0,\"val\":127,\"min\":1,\"max\":255},"
" {\"id\":1,\"val\":127,\"min\":1,\"max\":255},"
" {\"id\":2,\"val\":0,\"min\":0,\"max\":255},"
" {\"id\":3,\"val\":85,\"min\":0,\"max\":255,\"name\":\"" DFTINTF_0DA "\"},"
" {\"id\":6,\"type\":3,\"val\":0,\"min\":0,\"max\":2,\"name\":\"" DFTINTF_119 "\"},"
" {\"id\":4,\"type\":0,\"val\":255,\"min\":1,\"max\":255,\"name\":\"" DFTINTF_122 "\"},"  // Cyle
" {\"id\":8,\"type\":3,\"val\":0,\"min\":0,\"max\":255,\"name\":\"" DFTINTF_101 "\"}"    // "Couleur 1 (0: cyclique)"
"]}";


static const char E_FX_Firework []  PROGMEM = 
  "{" COTNROLS_PREFIX ",\"ctrls\":["
  " {\"id\":1,\"val\":127,\"min\":0,\"max\":255},"
  " {\"id\":2,\"state\":3,\"type\":0,\"val\":3,\"min\":1,\"max\":12,\"name\":\"" DFTINTF_135 "\"},"
  // " {\"id\":6,\"type\":0,\"val\":120,\"min\":1,\"max\":255,\"name\":\"" DFTINTF_0ED "\"},"
  " {\"id\":5,\"type\":0,\"val\":60,\"min\":0,\"max\":255,\"name\":\"" DFTINTF_123 "\"},"
  " {\"id\":4,\"type\":0,\"val\":40,\"min\":1,\"max\":255,\"name\":\"" DFTINTF_122 "\"},"
  " {\"id\":8,\"type\":3,\"val\":80,\"min\":0,\"max\":255,\"name\":\"" DFTINTF_101 "\"}"
  "]}";

static const char E_FX_PULS[] PROGMEM = 
  "{" COTNROLS_PREFIX ",\"ctrls\":["
  "{\"id\":2,\"name\":\"" DFTINTF_0D6 "\"},"
  "{\"id\":7,\"type\":18,\"val\":1,\"name\":\"" DFTINTF_020 "\"}"
  "]}";



struct effeNamIDList {
  const char * _nameId;
  const char * _defaultCfg;
  const char * _effType;
  const char *  _cat;
  const char * _segment;
  int _spd;
} ;
extern PROGMEM effeNamIDList _effeNamIDList [] ;
extern uint8_t _effeNamIDList_cnt ;

void effectType_toString(OUTPUT_EFFECTTYPE type, String & result);
OUTPUT_EFFECTTYPE effectType_toMod(const char * name);

static const TProgmemRGBPalette16 MagmaColor_p FL_PROGMEM = {CRGB::Black, 0x240000, 0x480000, 0x660000, 0x9a1100, 0xc32500, 0xd12a00, 0xe12f17, 0xf0350f, 0xff3c00, 0xff6400, 0xff8300, 0xffa000, 0xffba00, 0xffd400, 0xffffff};
static const TProgmemRGBPalette16 NormalFire3_p FL_PROGMEM = {CRGB::Black, 0x330000, 0x660000, 0x990000, 0xcc0000, CRGB::Red, 0xff0c00, 0xff1800, 0xff2400, 0xff3010, 0xff3c20, 0xff4835, 0xff5440, 0xff6055, 0xff6c60, 0xff7866};                             // пытаюсь сделать что-то более приличное
static const TProgmemRGBPalette16 HeatColors2_p FL_PROGMEM = {    0x000000, 0x330000, 0x660000, 0x990000, 0xCC0000, 0xFF0000, 0xFF3300, 0xFF6600, 0xFF9900, 0xFFCC00, 0xFFFF00, 0xFFFF33, 0xFFFF66, 0xFFFF99, 0xFFFFCC, 0xFFFFFF};
static const TProgmemRGBPalette16 WoodFireColors_p FL_PROGMEM = {CRGB::Black, 0x330e00, 0x661c00, 0x992900, 0xcc3700, CRGB::OrangeRed, 0xff5800, 0xff6b00, 0xff7f00, 0xff9200, CRGB::Orange, 0xffaf00, 0xffb900, 0xffc300, 0xffcd00, CRGB::Gold};             //* Orange
static const TProgmemRGBPalette16 NormalFire_p FL_PROGMEM = {CRGB::Black, 0x330000, 0x660000, 0x990000, 0xcc0000, CRGB::Red, 0xff0c00, 0xff1800, 0xff2400, 0xff3000, 0xff3c00, 0xff4800, 0xff5400, 0xff6000, 0xff6c00, 0xff7800};                             // пытаюсь сделать что-то более приличное
static const TProgmemRGBPalette16 NormalFire2_p FL_PROGMEM = {CRGB::Black, 0x560000, 0x6b0000, 0x820000, 0x9a0011, CRGB::FireBrick, 0xc22520, 0xd12a1c, 0xe12f17, 0xf0350f, 0xff3c00, 0xff6400, 0xff8300, 0xffa000, 0xffba00, 0xffd400};                      // пытаюсь сделать что-то более приличное
static const TProgmemRGBPalette16 LithiumFireColors_p FL_PROGMEM = {CRGB::Black, 0x240707, 0x470e0e, 0x6b1414, 0x8e1b1b, CRGB::FireBrick, 0xc14244, 0xd16166, 0xe08187, 0xf0a0a9, CRGB::Pink, 0xff9ec0, 0xff7bb5, 0xff59a9, 0xff369e, CRGB::DeepPink};        //* Red
static const TProgmemRGBPalette16 SodiumFireColors_p FL_PROGMEM = {CRGB::Black, 0x332100, 0x664200, 0x996300, 0xcc8400, CRGB::Orange, 0xffaf00, 0xffb900, 0xffc300, 0xffcd00, CRGB::Gold, 0xf8cd06, 0xf0c30d, 0xe9b913, 0xe1af1a, CRGB::Goldenrod};           //* Yellow
static const TProgmemRGBPalette16 CopperFireColors_p FL_PROGMEM = {CRGB::Black, 0x001a00, 0x003300, 0x004d00, 0x006600, CRGB::Green, 0x239909, 0x45b313, 0x68cc1c, 0x8ae626, CRGB::GreenYellow, 0x94f530, 0x7ceb30, 0x63e131, 0x4bd731, CRGB::LimeGreen};     //* Green
static const TProgmemRGBPalette16 AlcoholFireColors_p FL_PROGMEM = {CRGB::Black, 0x000033, 0x000066, 0x000099, 0x0000cc, CRGB::Blue, 0x0026ff, 0x004cff, 0x0073ff, 0x0099ff, CRGB::DeepSkyBlue, 0x1bc2fe, 0x36c5fd, 0x51c8fc, 0x6ccbfb, CRGB::LightSkyBlue};  //* Blue
static const TProgmemRGBPalette16 RubidiumFireColors_p FL_PROGMEM = {CRGB::Black, 0x0f001a, 0x1e0034, 0x2d004e, 0x3c0068, CRGB::Indigo, CRGB::Indigo, CRGB::Indigo, CRGB::Indigo, CRGB::Indigo, CRGB::Indigo, 0x3c0084, 0x2d0086, 0x1e0087, 0x0f0089, CRGB::DarkBlue};        //* Indigo
static const TProgmemRGBPalette16 PotassiumFireColors_p FL_PROGMEM = {CRGB::Black, 0x0f001a, 0x1e0034, 0x2d004e, 0x3c0068, CRGB::Indigo, 0x591694, 0x682da6, 0x7643b7, 0x855ac9, CRGB::MediumPurple, 0xa95ecd, 0xbe4bbe, 0xd439b0, 0xe926a1, CRGB::DeepPink}; //* Violet
static const TProgmemRGBPalette16 WaterfallColors_p FL_PROGMEM = {0x000000, 0x060707, 0x101110, 0x151717, 0x1C1D22, 0x242A28, 0x363B3A, 0x313634, 0x505552, 0x6B6C70, 0x98A4A1, 0xC1C2C1, 0xCACECF, 0xCDDEDD, 0xDEDFE0, 0xB2BAB9};        //* Orange
static const TProgmemRGBPalette16 ZeebraColors_p FL_PROGMEM = {CRGB::White, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::White, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::White, CRGB::Black, CRGB::Black, CRGB::Black, CRGB::White, CRGB::Black, CRGB::Black, CRGB::Black};
// Добавил "белую" палитру для "Огонь 2012", в самом конце 4-го ползунка, огонь горит белым цветом, - красиво для белой динамической подсветки
static const TProgmemRGBPalette16 WhiteBlackColors_p FL_PROGMEM = {CRGB::Black, CRGB::Gray, CRGB::White, CRGB::WhiteSmoke};
// stepko нашел и перерисовал палитры в https://colorswall.com/
static const TProgmemRGBPalette16 AcidColors_p FL_PROGMEM = {0xffff00, 0xd2b328, 0xd25228, 0x711323, 0x6c000c, 0x5a0c00, 0x6d373a, 0xaa5a62, 0x604564, 0x313164, 0x332765, 0x3a2465, 0x4b1665, 0x4b0069, 0x31004c, 0x200046};
static const TProgmemRGBPalette16 StepkosColors_p FL_PROGMEM = {0x0000ff, 0x0f00f0, 0x1e00e1, 0x2d00d2, 0x3c00c3, 0x4b00b4, 0x5a00a5, 0x690096, 0x780087, 0x870078, 0x9600cd, 0xa50050, 0xb40041, 0xc30032, 0xd20023, 0xe10014};
static const TProgmemRGBPalette16 AutumnColors_p FL_PROGMEM = {0xbc2300, 0xc84416, 0xdc642c, 0xe69664, 0xfbb979, 0xca503d, 0x882c1c, 0x9a3631, 0xa9624e, 0xcc9762, 0xdcc0b5, 0xc1a29f, 0x826468, 0x4a3334, 0x231a1a, 0x161113};
static const TProgmemRGBPalette16 NeonColors_p FL_PROGMEM = {0x00b1d0, 0x0f93ec, 0x3572ff, 0x4157ff, 0x6162ff, 0x686cff, 0x7473ff, 0x8689e5, 0x9e9dc6, 0x9694ac, 0x979b9b, 0x888b8c, 0x767680, 0x596160, 0x6c736f, 0x7b7359};
static const TProgmemRGBPalette16 EveningColors_p FL_PROGMEM = {0x1e0443, 0x6d0081, 0x8200ac, 0x8200ac, 0x8200ac, 0x8200ac, 0x8200ac, 0x8200ac, 0x7900a1, 0x820055, 0xc80000, 0xe57b00, 0xff9d5a, 0xc58b32, 0xd8d400, 0xffff00};
static const TProgmemRGBPalette16 OrangeColors_p FL_PROGMEM = {0xffff00, 0xfff100, 0xffe100, 0xffd100, 0xffc100, 0xffb100, 0xffa100, 0xff9100, 0xff8100, 0xff7100, 0xff6100, 0xff5100, 0xff4100, 0xff3100, 0xff2100, 0xff1100};

// Удачная палитра от Stepko заменю ка я ею RainbowStripeColors_p, которая "рябит" на большинстве эффектов
static const TProgmemRGBPalette16 AuroraColors_p FL_PROGMEM = {0x00ff00, 0x00c040, 0x008080, 0x0040c0, 0x0000ff, 0x4000c0, 0x800080, 0xc00040, 0xff0000, 0xff4000, 0xff8000, 0xd6c000, 0xffff00, 0xc0ff00, 0x80ff00, 0x40ff00};
static const TProgmemRGBPalette16 HolyLightsColors_p FL_PROGMEM = {0xff0000, 0xff4000, 0xff8000, 0xd6c000, 0xffff00, 0xc0ff00, 0x80ff00, 0x40ff00, 0x00ff00, 0x00c040, 0x008080, 0x0040c0, 0x0000ff, 0x4000c0, 0x800080, 0xc00040};


// static const TProgmemRGBPalette16 alcohol_p FL_PROGMEM = {0x000000, 0x000033, 0x000066, 0x000099, 0x0000CC, 0x0000FF, 0x0026FF, 0x004CFF, 0x0072FF, 0x0098FF, 0x00BFFF, 0x1BC2FE, 0x36C5FD, 0x51C8FC, 0x6CCBFB, 0x87CEFA};
// static const TProgmemRGBPalette16 copper_p FL_PROGMEM = {0x000000, 0x001900, 0x003300, 0x004C00, 0x006600, 0x008000, 0x229909, 0x45B212, 0x67CC1C, 0x8AE525, 0xADFF2F, 0x94F52F, 0x7BEB30, 0x63E130, 0x4AD731, 0x32CD31};
// static const TProgmemRGBPalette16 lithium_p FL_PROGMEM = {0x000000, 0x230606, 0x470D0D, 0x6A1414, 0x8E1B1B, 0xB22222, 0xC14143, 0xD06165, 0xE08087, 0xEFA0A9, 0xFFC0CB, 0xFF9DBF, 0xFF7BB4, 0xFF58A9, 0xFF369E, 0xFF1493};
// static const TProgmemRGBPalette16 potassium_p FL_PROGMEM = {0x000000, 0x0F001A, 0x1E0034, 0x2D004E, 0x3C0068, 0x4B0082, 0x591693, 0x672CA5, 0x7643B7, 0x8459C9, 0x9370DB, 0xA85DCC, 0xBE4BBE, 0xD338AF, 0xE926A1, 0xFE1493};
// static const TProgmemRGBPalette16 rubidium_p FL_PROGMEM = {0x000000, 0x0F001A, 0x1E0034, 0x2D004E, 0x3C0068, 0x4B0082, 0x4B0082, 0x4B0082, 0x4B0082, 0x4B0082, 0x4B0082, 0x3C0083, 0x2D0085, 0x1E0087, 0x0F0089, 0x00008A};
// static const TProgmemRGBPalette16 sodium_p FL_PROGMEM = {0x000000, 0x332100, 0x664200, 0x996300, 0xCC8400, 0xFFA500, 0xFFAF00, 0xFFB900, 0xFFC300, 0xFFCD00, 0xFFD700, 0xF7CD06, 0xF0C30C, 0xE8B913, 0xE1AF19, 0xDAA51F};
// static const TProgmemRGBPalette16 wood_p FL_PROGMEM = {0x000000, 0x330D00, 0x661B00, 0x992900, 0xCC3700, 0xFF4500, 0xFF5800, 0xFF6B00, 0xFF7E00, 0xFF9100, 0xFFA500, 0xFFAF00, 0xFFB900, 0xFFC300, 0xFFCD00, 0xFFD700};
// static const TProgmemRGBPalette16 bluewhite_p_p FL_PROGMEM = {0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF, 0x808080, 0x808080, 0x808080};
// static const TProgmemRGBPalette16 cloudcolors_p_p FL_PROGMEM = {0x0000FF, 0x00008B, 0x00008B, 0x00008B, 0x00008B, 0x00008B, 0x00008B, 0x00008B, 0x0000FF, 0x00008B, 0x87CEEB, 0x87CEEB, 0xADD8E6, 0xFFFFFF, 0xADD8E6, 0x87CEEB};

// static const TProgmemRGBPalette16 fairylight_p_p FL_PROGMEM = {0xFFE42D, 0xFFE42D, 0xFFE42D, 0xFFE42D, 0x7F7216, 0x7F7216, 0xFFE42D, 0xFFE42D, 0x3F390B, 0x3F390B, 0xFFE42D, 0xFFE42D, 0xFFE42D, 0xFFE42D, 0xFFE42D, 0xFFE42D};
static const TProgmemRGBPalette16 forestcolors_p_p FL_PROGMEM = {0x006400, 0x006400, 0x556B2F, 0x006400, 0x008000, 0x228B22, 0x6B8E23, 0x008000, 0x2E8B57, 0x66CDAA, 0x32CD32, 0x9ACD32, 0x90EE90, 0x7CFC00, 0x66CDAA, 0x228B22};
// static const TProgmemRGBPalette16 heatcolors_p_p FL_PROGMEM = {0x000000, 0x330000, 0x660000, 0x990000, 0xCC0000, 0xFF0000, 0xFF3300, 0xFF6600, 0xFF9900, 0xFFCC00, 0xFFFF00, 0xFFFF33, 0xFFFF66, 0xFFFF99, 0xFFFFCC, 0xFFFFFF};
// static const TProgmemRGBPalette16 holly_p_p FL_PROGMEM = {0x00580C, 0x00580C, 0x00580C, 0x00580C, 0x00580C, 0x00580C, 0x00580C, 0x00580C, 0x00580C, 0x00580C, 0x00580C, 0x00580C, 0x00580C, 0x00580C, 0x00580C, 0xB00402};
// static const TProgmemRGBPalette16 icecolors_p_p FL_PROGMEM = {0x000000, 0x000033, 0x000066, 0x000099, 0x0000CC, 0x0000FF, 0x0033FF, 0x0066FF, 0x0099FF, 0x00CCFF, 0x00FFFF, 0x33FFFF, 0x66FFFF, 0x99FFFF, 0xCCFFFF, 0xFFFFFF};
// static const TProgmemRGBPalette16 icyblue_p_p FL_PROGMEM = {0x0C1040, 0x0C1040, 0x0C1040, 0x0C1040, 0x0C1040, 0x0C1040, 0x0C1040, 0x0C1040, 0x0C1040, 0x0C1040, 0x0C1040, 0x0C1040, 0x182080, 0x182080, 0x182080, 0x5080C0};
// static const TProgmemRGBPalette16 lavacolors_p_p FL_PROGMEM = {0x000000, 0x800000, 0x000000, 0x800000, 0x8B0000, 0x800000, 0x8B0000, 0x8B0000, 0x8B0000, 0xFF0000, 0xFFA500, 0xFFFFFF, 0xFFA500, 0xFF0000, 0x8B0000, 0x000000};
// static const TProgmemRGBPalette16 oceancolors_p_p FL_PROGMEM = {0x191970, 0x00008B, 0x191970, 0x000080, 0x00008B, 0x0000CD, 0x2E8B57, 0x008080, 0x5F9EA0, 0x0000FF, 0x008B8B, 0x6495ED, 0x7FFFD4, 0x2E8B57, 0x00FFFF, 0x87CEFA};
// static const TProgmemRGBPalette16 partycolors_p_p FL_PROGMEM = {0x5500AB, 0x84007C, 0xB5004B, 0xE5001B, 0xE81700, 0xB84700, 0xAB7700, 0xABAB00, 0xAB5500, 0xDD2200, 0xF2000E, 0xC2003E, 0x8F0071, 0x5F00A1, 0x2F00D0, 0x0007F9};
// static const TProgmemRGBPalette16 rainbowcolors_p_p FL_PROGMEM = {0xFF0000, 0xD52A00, 0xAB5500, 0xAB7F00, 0xABAB00, 0x56D500, 0x00FF00, 0x00D52A, 0x00AB55, 0x0056AA, 0x0000FF, 0x2A00D5, 0x5500AB, 0x7F0081, 0xAB0055, 0xD5002B};
// static const TProgmemRGBPalette16 rainbowstripecolors_p_p FL_PROGMEM = {0xFF0000, 0x000000, 0xAB5500, 0x000000, 0xABAB00, 0x000000, 0x00FF00, 0x000000, 0x00AB55, 0x000000, 0x0000FF, 0x000000, 0x5500AB, 0x000000, 0xAB0055, 0x000000};
// static const TProgmemRGBPalette16 redgreenwhite_p_p FL_PROGMEM = {0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0xFF0000, 0x808080, 0x808080, 0x008000, 0x008000, 0x008000, 0x008000};
// static const TProgmemRGBPalette16 redwhite_p_p FL_PROGMEM = {0xFF0000, 0xFF0000, 0x808080, 0x808080, 0xFF0000, 0xFF0000, 0x808080, 0x808080, 0xFF0000, 0xFF0000, 0x808080, 0x808080, 0xFF0000, 0xFF0000, 0x808080, 0x808080};
// static const TProgmemRGBPalette16 retroc9_p_p FL_PROGMEM = {0xB80400, 0x902C02, 0xB80400, 0x902C02, 0x902C02, 0xB80400, 0x902C02, 0xB80400, 0x046002, 0x046002, 0x046002, 0x046002, 0x070758, 0x070758, 0x070758, 0x606820};
// static const TProgmemRGBPalette16 snowcolors_p FL_PROGMEM = {0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0xFFFFFF, 0x555555, 0x555555, 0x555555, 0x555555, 0x555555, 0x555555, 0x555555, 0x555555, 0x555555, 0x555555, 0x555555, 0x555555};
// static const TProgmemRGBPalette16 snow_p_p FL_PROGMEM = {0x304048, 0x304048, 0x304048, 0x304048, 0x304048, 0x304048, 0x304048, 0x304048, 0x304048, 0x304048, 0x304048, 0x304048, 0x304048, 0x304048, 0x304048, 0xE0F0FF};


#endif // CONSTLEDS_H