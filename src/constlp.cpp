#include "../include/constlp.h"

const char ALMLPT_REQ_001[] PROGMEM = "{\"arg\":[\"v\"]}";
const char ALMLPT_REQ_002[] PROGMEM = "{\"arg\":[]}";
const char ALMLPT_REQ_003[] PROGMEM = "{\"arg\":[\"r\",\"g\",\"b\",\"h\",\"s\",\"v\"]}";
const char ALMLPT_REQ_004[] PROGMEM = "{\"arg\":[\"c\",\"v\"]}";
const char ALMLPT_REQ_005[] PROGMEM = "{\"arg\":[\"v\",\"id\"]}";

PROGMEM reqNamIDList _reqNamIDList [] = {
  {req_s_output,          ALMLPT_REQ_001},
  {req_toglle,            ALMLPT_REQ_002},
  {req_lampOnOff,         ALMLPT_REQ_001},
  {req_ledBri,            ALMLPT_REQ_001},
  {req_ledBriIncr,        ALMLPT_REQ_001},
  {req_lampHue,           ALMLPT_REQ_001},
  {req_lampHueIncr,       ALMLPT_REQ_001},
  {req_lampSat,           ALMLPT_REQ_001},
  {req_lampBri,           ALMLPT_REQ_001},
  {req_lampWhite,         ALMLPT_REQ_001},
  {req_lampWhite_f,       ALMLPT_REQ_002},
  {req_lampWhite_v,       ALMLPT_REQ_001},
  {req_lampSetColor,      ALMLPT_REQ_003},
  {req_lampSetColor_f,    ALMLPT_REQ_003},
  {req_etage,             ALMLPT_REQ_001},
  {req_etages,            ALMLPT_REQ_001},
  {req_eff_autoplay,      ALMLPT_REQ_001},
  {req_eff_activ,         ALMLPT_REQ_001},
  {req_eff_next,          ALMLPT_REQ_002},
  {req_eff_prev,          ALMLPT_REQ_002},
  {req_eff_load,          ALMLPT_REQ_001},
  {req_eff_f,             ALMLPT_REQ_004},
  {req_eff_speed,         ALMLPT_REQ_001},
  {req_eff_speedIncr,     ALMLPT_REQ_001},
  {req_eff_bri,           ALMLPT_REQ_001},
  // {req_eff_briIncr,        "{\"arg\":[]}"},
  {req_eff_scale,         ALMLPT_REQ_001},
  {req_eff_id,            ALMLPT_REQ_005},
  {req_eff_timerCol,      ALMLPT_REQ_001},
  // {req_eff_timerCol2,      "{\"arg\":[]}"},
  {req_eff_timerPal,      ALMLPT_REQ_001},
  {req_eff_timerPalGrad,  ALMLPT_REQ_001},
  {req_eff_effColorMod,   ALMLPT_REQ_001},
  {req_eff_sav,           ALMLPT_REQ_002}
};

uint8_t _reqNamIDList_cnt = ARRAY_SIZE(_reqNamIDList);

void reqNamIDList_json(const String & search, DynamicJsonDocument & doc) { 
  JsonObject  obj_1;  
  JsonArray   arr_1;

  if (!doc.containsKey(F("requestnameid"))){
    obj_1 = doc.createNestedObject(F("requestnameid"));
    arr_1 = obj_1.createNestedArray(F("arg_search"));
  } else {
    obj_1 = doc[F("requestnameid")];
    arr_1 = doc[F("requestnameid")][F("arg_search")];
  }

  for (uint8_t i = 0; i < _reqNamIDList_cnt; i++){
    if (search == FPSTR(_reqNamIDList[i]._nameId)) {
      
      JsonObject obj_2 = arr_1.createNestedObject();

      obj_2[FPSTR(ALMLPT_REQ)] = search;
      JsonArray arr = obj_2.createNestedArray(FPSTR(ALMLPT_ARG));

          DynamicJsonDocument arg(255);
          deserializeJson(arg, FPSTR(_reqNamIDList[i]._arg));

          JsonArray argArr = arg[F("arg")].as<JsonArray>();
          if (argArr.size()<=0)continue;
          for (String item : argArr) {
            if (!item)continue;
            arr.add(item);
          }
    }
    
  } 
}
void reqList_json(JsonObject & obj) { 
  JsonArray arr = obj.createNestedArray(F("req"));
  for (uint8_t i = 0; i < _reqNamIDList_cnt; i++){
    arr.add(FPSTR(_reqNamIDList[i]._nameId));
  }
}
void reqList_json(DynamicJsonDocument & obj) { 
  JsonArray arr = obj.createNestedArray(F("req"));
  for (uint8_t i = 0; i < _reqNamIDList_cnt; i++){
    arr.add(FPSTR(_reqNamIDList[i]._nameId));
  }
}
void reqNamIDList_json(uint8_t mod, DynamicJsonDocument & doc) {
  JsonArray arr;
  JsonObject oObj_apip;
  JsonObject oObj_apip_3;

  JsonObject obj ;  

  obj = doc.createNestedObject(F("http"));
  obj.createNestedObject(F("api"));
  obj.createNestedObject(F("kodular"));
  obj.createNestedObject(F("api_effect"));
  obj.createNestedObject(F("api_system"));
  oObj_apip = obj.createNestedObject(F("wcapi"));
  oObj_apip[F("args")] = F("api, set_cm, to_fs");
  obj = doc.createNestedObject(F("post"));
  oObj_apip = obj.createNestedObject(F("apapi"));
  oObj_apip[F("syntax")]  = F("{op:0, type:HTTP_POST, set:[..], get:[..]}");
  oObj_apip[F("arg")]     = F("list_req, list_ra");
  oObj_apip = obj.createNestedObject(F("almlapi"));
  oObj_apip[F("syntax_")]     = F("{apip:1}");
  oObj_apip[F("syntax_get")]  = F("{apip:4, get:[{n:,v:}]}");
  oObj_apip[F("syntax_set")]  = F("{apip:3, apio:{}");

  if (!doc.containsKey(F("almlapi"))){
    obj = doc.createNestedObject(F("almlapi"));
  } else {
    obj = doc[F("almlapi")];
  }
  switch (mod) {
      case 0:
        arr = obj.createNestedArray(F("req"));
        for (uint8_t i = 0; i < _reqNamIDList_cnt; i++){
          arr.add(FPSTR(_reqNamIDList[i]._nameId));
        }

        oObj_apip = obj.createNestedObject(F("apip"));
        oObj_apip.createNestedObject(F("0")); 
        {
          JsonObject oOjbect;
          oOjbect = oObj_apip.createNestedObject(F("1")); 
          oOjbect[F("desc")] = "restart esp";
        }
        {
          JsonObject oOjbect = oObj_apip.createNestedObject(F("2")); 
          oOjbect[F("desc")] = F("filesystem format, key=format:value=0->3");
          JsonArray items = oOjbect.createNestedArray("format");
          items.add(F("0 - /outputs"));
          items.add(F("1 - /eff"));
          items.add(F("2 - /wcevo_config.json"));
          items.add(F("3 - /"));
        }
        oObj_apip_3 = oObj_apip.createNestedObject(F("3")); 
        {
          JsonObject oOjbect, oOjbect_2;

          oOjbect = oObj_apip_3.createNestedObject(F("aipo:{ object example 1}"));
          oOjbect_2 = oOjbect.createNestedObject(F("op")); 
          oOjbect_2[F("3")] = F("selection via: device name && output pos");
          oOjbect.createNestedObject(F("dn")); 
          oOjbect.createNestedObject(F("output")); 
          oOjbect.createNestedObject(F("req")); 
          oOjbect_2 = oOjbect.createNestedObject(F("val")); 
          oOjbect_2[F("v")] = "";
          oOjbect.createNestedObject(F("from")); 

          oOjbect = oObj_apip_3.createNestedObject(F("aipo:{ object example 2}")); // F("aipo:{ object example 2}")
          oOjbect_2 = oOjbect.createNestedObject(F("op")); 
          oOjbect_2[F("2")] = F("selection via: all device");
          oOjbect.createNestedObject(F("req")); 
          oOjbect_2 = oOjbect.createNestedObject(F("val")); 
          oOjbect_2[F("v")] = "";
          oOjbect.createNestedObject(F("from")); 

          oOjbect = oObj_apip_3.createNestedObject(F("aipo:{ object example 3}")); // F("aipo:{ object example 3}")
          oOjbect_2 = oOjbect.createNestedObject(F("op")); 
          oOjbect_2[F("1")] = F("selection via: device name");
          oOjbect.createNestedObject(F("dn")); 
          oOjbect.createNestedObject(F("req")); 
          oOjbect_2 = oOjbect.createNestedObject(F("val")); 
          oOjbect_2[F("v")] = "";
          oOjbect_2 = oOjbect.createNestedObject(F("from")); 
          oOjbect_2[F("0")] = "nodejs";
          oOjbect_2[F("1")] = "kodular";

          oOjbect = oObj_apip_3.createNestedObject(F("aipo:{ object example 4}"));
          oOjbect_2 = oOjbect.createNestedObject(F("op")); 
          oOjbect_2[F("1")] = F("selection via: device name");
          oOjbect_2 = oOjbect.createNestedObject(F("req")); 
          oOjbect_2 = oOjbect.createNestedObject(F("val")); 
          oOjbect_2[F("v")] = "";
          oOjbect_2 = oOjbect.createNestedObject(F("array")); 
          oOjbect[F("EX")] = F("A:[{dn:devicename, A:[op1,op2]},{...}]");
        } 
        {
          JsonArray arr_2 =  oObj_apip.createNestedArray(F("4")); 
          uint8_t cnt = ARRAY_SIZE(ALMLPT_KKEY_ALL);
          for (uint8_t i = 0; i < cnt; i++){
            if ( FPSTR(ALMLPT_KKEY_ALL[i]) == FPSTR(ALMLPT_KKEY_006) ) {
              DynamicJsonDocument jsonBuffer(255);
              JsonArray root = jsonBuffer.to<JsonArray>();
              root.add(FPSTR(ALMLPT_KKEY_ALL[i]));
              root.add(F("v"));
              arr_2.add(root);
            } else {
              arr_2.add(FPSTR(ALMLPT_KKEY_ALL[i]));
            }
          }        
        }        

      break;
      case 1:
        arr = obj.createNestedArray(F("listarg"));
        for (uint8_t i = 0; i < _reqNamIDList_cnt; i++){
          JsonArray arr_2 = arr.createNestedArray();

          DynamicJsonDocument arg(255);
          deserializeJson(arg, FPSTR(_reqNamIDList[i]._arg));

          arr_2.add(FPSTR(_reqNamIDList[i]._nameId));
          JsonArray arr_3 = arr_2.createNestedArray();

          JsonArray argArr = arg[F("arg")].as<JsonArray>();
          for (String item : argArr) {
            if (!item)continue;
            Serial.println(item);
            arr_3.add(item);
          }
        }       
      break;
      default:
      break;
  }
}