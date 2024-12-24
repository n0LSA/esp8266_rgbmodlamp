#ifndef CONSTLP_H
#define CONSTLP_H

  #include <Arduino.h>

  #include "libextern.h"


struct reqNamIDList {
  const char * _nameId;
  const char * _arg;
} ;

static const char req_sys     [] PROGMEM = "sys";
static const char req_sys_1   [] PROGMEM = "system";
static const char req_sys_2   [] PROGMEM = "network";
static const char req_sys_3   [] PROGMEM = "firmware";
static const char req_output  [] PROGMEM = "output";

// static const char FNPATH_EFFLIST        [] PROGMEM = "/effect_list/eff_index.json";
// static const char FOPATH_EFFLIST        [] PROGMEM = "/eff";
// static const char FNPATH_PATTERNLIST    [] PROGMEM = "/effect_list/patternList.json";
// static const char FNPATH_PLAYLIST       [] PROGMEM = "/effect_list/playList.json";
// static const char FNPATH_CURRENTSETTING [] PROGMEM = "/lamp/sav/currentSetting.json";
// static const char FOPATH_PLAYLISTEFF    [] PROGMEM = "/effect_list/eff/";
// static const char FNPATH_COLORPICKER    [] PROGMEM = "/effect_list/colopicker.json";
// // static const char FNPATH_COLORPICKERC  [] PROGMEM = "/effect_list/colopicker_c.json";
// static const char FOPATH_PLAYLIST       [] PROGMEM = "/effect_list/playlist/";
// static const char FSLSH                 [] PROGMEM = "/";
// static const char FOPATH_SCREENSHOT   [] PROGMEM = "/screenshot/";

static const char req_s_output            [] PROGMEM = "sel"; 
static const char req_toglle              [] PROGMEM = "tog";           
static const char req_lampOnOff           [] PROGMEM = "onoff";                
static const char req_ledBri              [] PROGMEM = "bri";                
static const char req_ledBriIncr          [] PROGMEM = "briIncr";                
static const char req_lampHue             [] PROGMEM = "hc";
static const char req_lampHueIncr         [] PROGMEM = "hcIncr";
static const char req_lampSat             [] PROGMEM = "hs";
static const char req_lampBri             [] PROGMEM = "hv";
static const char req_lampWhite           [] PROGMEM = "tw";
static const char req_lampWhite_f         [] PROGMEM = "tw_f";
static const char req_lampWhite_v         [] PROGMEM = "tw_v";
static const char req_lampSetColor        [] PROGMEM = "rgb";    
static const char req_lampSetColor_f      [] PROGMEM = "rgb_f";    
static const char req_etage               [] PROGMEM = "etage";    
static const char req_etages              [] PROGMEM = "etages";    
static const char req_eff_autoplay        [] PROGMEM = "eff_autoplay";
static const char req_eff_activ           [] PROGMEM = "eff_avtiv";
static const char req_eff_next            [] PROGMEM = "eff_next";
static const char req_eff_prev            [] PROGMEM = "eff_prev";
static const char req_eff_load            [] PROGMEM = "eff_load";
static const char req_eff_f               [] PROGMEM = "eff_f";
static const char req_eff_speed           [] PROGMEM = "eff_speed";
static const char req_eff_speedIncr       [] PROGMEM = "eff_speedIncr";
static const char req_eff_bri             [] PROGMEM = "eff_bri";
static const char req_eff_briIncr         [] PROGMEM = "eff_briIncr";
static const char req_eff_scale           [] PROGMEM = "eff_scale";
static const char req_eff_id              [] PROGMEM = "eff_id";
static const char req_eff_timerCol        [] PROGMEM = "eff_timerCol";
static const char req_eff_timerCol2       [] PROGMEM = "eff_timerCol2";
static const char req_eff_timerPal        [] PROGMEM = "eff_timerPal";
static const char req_eff_timerPalGrad    [] PROGMEM = "eff_timerPalGrad";
static const char req_eff_effColorMod     [] PROGMEM = "eff_colorMod";
static const char req_eff_sav             [] PROGMEM = "eff_sav";

static const char ALMLPT_ARG              [] PROGMEM = "arg";
static const char ALMLPT_GET              [] PROGMEM = "get";
static const char ALMLPT_SET              [] PROGMEM = "set";
static const char ALMLPT_OP               [] PROGMEM = "op";
static const char ALMLPT_FROM             [] PROGMEM = "from";
static const char ALMLPT_ARRAY            [] PROGMEM = "array";
static const char ALMLPT_DN               [] PROGMEM = "dn";
static const char ALMLPT_PRESET           [] PROGMEM = "preset";
static const char ALMLPT_SAV              [] PROGMEM = "sav";
static const char ALMLPT_LOAD             [] PROGMEM = "load";
static const char ALMLPT_RGB              [] PROGMEM = "rgb";
static const char ALMLPT_RELAY_AC         [] PROGMEM = "relay_ac";
static const char ALMLPT_PLUG             [] PROGMEM = "plug";
static const char ALMLPT_REQ              [] PROGMEM = "req";
static const char ALMLPT_VAL              [] PROGMEM = "val";
static const char ALMLPT_OUTPUT           [] PROGMEM = "output";
static const char ALMLPT_MOD              [] PROGMEM = "mod";
static const char ALMLPT_REQUEST          [] PROGMEM = "request";
static const char ALMLPT_OUTPUTS          [] PROGMEM = "outputs";
static const char ALMLPT_A                [] PROGMEM = "A";
static const char ALMLPT_V                [] PROGMEM = "v";
static const char ALMLPT_N                [] PROGMEM = "n";
static const char ALMLPT_C                [] PROGMEM = "c";
static const char ALMLPT_IP               [] PROGMEM = "ip";
static const char ALMLPT_OC               [] PROGMEM = "oc";
static const char ALMLPT_CLI              [] PROGMEM = "cli";
static const char ALMLPT_API_OP           [] PROGMEM = "apip";
static const char ALMLPT_API_OUTPUT       [] PROGMEM = "apio";

static const char ALMLPT_NAME             [] PROGMEM = "Name";
static const char ALMLPT_ID               [] PROGMEM = "Id";
static const char ALMLPT_MI               [] PROGMEM = "Min";
static const char ALMLPT_MA               [] PROGMEM = "Max";
static const char ALMLPT_ST               [] PROGMEM = "Step";
static const char ALMLPT_GETTYPE          [] PROGMEM = "Type";

static const char ALMLPT_EFFECT           [] PROGMEM = "effect";
static const char ALMLPT_EFFECTS          [] PROGMEM = "effects";
static const char ALMLPT_C1               [] PROGMEM = "c1";
static const char ALMLPT_COFF             [] PROGMEM = "coff";
static const char ALMLPT_R                [] PROGMEM = "r";
static const char ALMLPT_G                [] PROGMEM = "g";
static const char ALMLPT_B                [] PROGMEM = "b";

static const char ALMLPT_                 [] PROGMEM = "from";


  const char ALMLPT_KKEY_000[] PROGMEM = "list_lbid";
  const char ALMLPT_KKEY_001[] PROGMEM = "json_effect";
  const char ALMLPT_KKEY_002[] PROGMEM = "json_output";
  const char ALMLPT_KKEY_003[] PROGMEM = "json_outputEffect";
  const char ALMLPT_KKEY_004[] PROGMEM = "list_request";
  const char ALMLPT_KKEY_005[] PROGMEM = "user_cfg";
  const char ALMLPT_KKEY_006[] PROGMEM = "arg_search";
  const char ALMLPT_KKEY_007[] PROGMEM = "list_alllb";
  const char ALMLPT_KKEY_008[] PROGMEM = "list_pl_0";
  const char ALMLPT_KKEY_009[] PROGMEM = "effect_ctrls";
  const char ALMLPT_KKEY_010[] PROGMEM = "api";
  static const char* const ALMLPT_KKEY_ALL[] PROGMEM = {
    ALMLPT_KKEY_000,
    ALMLPT_KKEY_001,
    ALMLPT_KKEY_002,
    ALMLPT_KKEY_003,
    ALMLPT_KKEY_004,
    ALMLPT_KKEY_005,
    ALMLPT_KKEY_006,
    ALMLPT_KKEY_007,
    ALMLPT_KKEY_008,
    ALMLPT_KKEY_009,
    ALMLPT_KKEY_010
  };

void reqNamIDList_json(uint8_t, DynamicJsonDocument & doc);
void reqNamIDList_json(const String &, DynamicJsonDocument & doc);
void reqList_json(JsonObject & obj);
void reqList_json(DynamicJsonDocument & obj);


class timerMinim
{
  public:
  timerMinim() {_interval = 0;} // по дефолту - отключен

    timerMinim(uint32_t interval)                         // объявление таймера с указанием интервала
    {
      _interval = interval;
      _timer = millis();
    }

    uint32_t getInterval()                                 // получение интервала работы таймера
    {
      return _interval;
    }

    void setInterval(uint32_t interval)                    // установка интервала работы таймера
    {
      _interval = interval;
    }

    bool isReady()                                         // возвращает true, когда пришло время. Сбрасывается в false сам (AUTO) или вручную (MANUAL)
    {
      if ((uint32_t)millis() - _timer >= _interval && _interval!=0){
        _timer = millis();
        return true;
      }
      else {
        return false;
      }
    }

    bool isReadyManual()                                   // возвращает true, когда пришло время. Без сбороса
    {
      if ((uint32_t)millis() - _timer >= _interval && _interval!=0){
        return true;
      }
      else {
        return false;
      }
    }

    void reset()                                            // ручной сброс таймера на установленный интервал
    {
      _timer = millis();
    }

  private:
    uint32_t _timer = 0;
    uint32_t _interval = 0;
};
#endif // CONSTLP_H