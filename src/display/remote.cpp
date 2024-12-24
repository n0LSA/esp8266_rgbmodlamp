#include "../../include/display/remote.h"
#include "../../include/device/device.h"
#include "../../include/pixel/effectcore.h"
#include "../../include/display/lamp.h"

extern EffectWorker  * _effects[];
extern DeviceSave _DeviceSave;

RemoteControl * RemoteControlPtr = nullptr;
RemoteControl * RemoteControlPtrGet(){return RemoteControlPtr;}

RemoteControl::RemoteControl(){
    RemoteControlPtr = this;
}

void RemoteControl::handle_udp()  {RemoteControl_udp::handle();}

void RemoteControl::begin()       {RemoteControl_udp::begin();}

void RemoteControl_udp::begin(){
    #ifdef ALS_LAMPUDPMULTI
    UdpMulti::stop();  
    #endif
    Udp::stop();  
    #ifdef ALS_LAMPUDPMULTI
    UdpMulti::begin();  
    #endif
    Udp::begin();     
}
void RemoteControl_udp::send_toIp(const String & transmit_buffer, IPAddress ip, uint16_t port){ 
  Udp::send_toIp(transmit_buffer, ip, port); 
}
void RemoteControl_udp::send_appi(boolean eff, boolean prog, boolean reset, uint8_t effLoad){ 
    String out = "";
    DevicePtrGet()->outputs_toAppi(out, eff, prog, effLoad);
    Udp::send_toIp(out);  
    if (reset) timerMultiSend_jsonDevice = millis();
}
void RemoteControl::send_update(boolean eff, boolean reset){ 
    if (eff)  {
      String out = "";
      DevicePtrGet()->outputs_toAppi(out, true, true, 0, 0);
      Udp::send_toIp(out); 

      #ifdef ALS_LAMPUDPMULTI
      out = "";
      DevicePtrGet()->outputs_toAppi(out, false, true, 0, 0);
      UdpMulti::send(out) ;

      uint8_t oC = 0;
      DevicePtrGet()->get_outputCount(oC);
      for (uint8_t i = 0; i < oC; i++) {
        out = "";
        if (DevicePtrGet()->outputs_effect_toNode(out, i)) {
          UdpMulti::send(out);
          yield();
        }
      } 
      #endif
    }
    else {
      String out = "";
      
      DevicePtrGet()->outputs_toAppi(out, false,  true, 0, 0);
      Udp::send_toIp(out); 
      #ifdef ALS_LAMPUDPMULTI
      UdpMulti::send(out);
      #endif
    }
    if (reset) timerMultiSend_jsonDevice = millis();
}

void RemoteControl_udp::handle(){
  // if (!WCEVO_CONNECTED) return;

  if (Udp::receive()){
    DynamicJsonDocument doc(2048);  
    String sData = "";
    Udp::get_packetBuffer(sData); 
    Serial.printf_P(PSTR("UDP\n")); 
    Serial.println(sData);
    DeserializationError error = deserializeJson(doc, sData);
    if (error) {
    }  else {
      handleJson(doc, false);
      yield();
    }  
  }

  #ifdef ALS_LAMPUDPMULTI
    if (UdpMulti::receive()){
      DynamicJsonDocument doc(2048);  
      String sData = "";
      UdpMulti::get_packetBuffer(sData);  
      DeserializationError error = deserializeJson(doc, sData);
      if (error) {
      }  else {
        handleJson(doc);
        yield();
      }       
    }  
  #endif

  if (multiSend_jsonDevice && (millis() - timerMultiSend_jsonDevice > 1500)) {
        // send_appi(false, true, false, 3);

    #ifdef ALS_LAMPUDPMULTI
      String out("");
      DevicePtrGet()->outputs_toAppi(out, false,  true, 0, 0);
      UdpMulti::send(out) ;
      yield();  

      uint8_t oC = 0;
      DevicePtrGet()->get_outputCount(oC);
      for (uint8_t i = 0; i < oC; i++) {
        out = "";
        if (DevicePtrGet()->outputs_effect_toNode(out, i)) {
          UdpMulti::send(out);
          yield();
        }
      } 
    #endif

    timerMultiSend_jsonDevice = millis(); 

  }

}
void RemoteControl_udp::handleJson(uint8_t op){
  ALT_TRACEC("main", "&c:2");

  if (op == 4 || op == 7 || op == 8) DevicePtrGet()->outputs_sav();

  String out;
  uint8_t get_lastOutput    = DevicePtrGet()->get_lastOutput();
  uint8_t get_lastSucces    = DevicePtrGet()->get_lastSucces();
  uint8_t get_lastOperation = DevicePtrGet()->get_lastOperation();

  Serial.printf_P(PSTR("op: %d - lastrequest: %s\n"), op, DevicePtrGet()->get_lastRequest().c_str());
  Serial.printf_P(PSTR("op: %d - get_lastOutput: %d\n"), op, get_lastOutput);
  Serial.printf_P(PSTR("op: %d - get_lastSucces: %d\n"), op, get_lastSucces);
  Serial.printf_P(PSTR("op: %d - get_lastOperation: %d\n"), op, get_lastOperation);
  Serial.printf_P(PSTR("op: %d - outputs_requestEffect: %d\n"), op, DevicePtrGet()->outputs_requestEffect());
  Serial.printf_P(PSTR("op: %d - outputs_EffectLoadRequest: %d\n"), op, DevicePtrGet()->outputs_EffectLoadRequest());
  if (op == 4) { 
      out = "";
      boolean sDegug = 0;
      switch (get_lastOperation) {
        case 0:
        case 1:
        case 2:
          if (DevicePtrGet()->outputs_EffectLoadRequest()) 
            DevicePtrGet()->outputs_toAppi(out, 0, 1, 0, sDegug);
          else if (DevicePtrGet()->outputs_requestEffect()) 
            DevicePtrGet()->outputs_toAppi(out, 0, 1, 0, sDegug);
          else 
            DevicePtrGet()->outputs_toAppi(out, 0, 1, 0, sDegug);
        break;
        case 3:
          // output_toAppi
          // (String & out, uint8_t pos, boolean eff, boolean prog, uint8_t effLoad, boolean sJson);      
          if (DevicePtrGet()->outputs_EffectLoadRequest())
            DevicePtrGet()->output_toAppi(out, get_lastOutput, 1, 1, 0, sDegug);
          else if (DevicePtrGet()->outputs_requestEffect()) 
            DevicePtrGet()->output_toAppi(out, get_lastOutput, 1, 0, 0, sDegug);    
          else 
            DevicePtrGet()->output_toAppi(out, get_lastOutput, 0, 1, 0, sDegug);
        break;
        default :
          DevicePtrGet()->outputs_toAppi(out, 1, 1, 0, sDegug);
        break;
      }

      Udp::send_toIp(out);   
      yield();
      
      #ifdef ALS_LAMPUDPMULTI
        UdpMulti::send(out) ;  
        yield(); 
      #endif   
      
  }
  else if (op == 7) { 
    DevicePtrGet()->outputs_effect_toAppi(out);
    Udp::send_toIp(out);  
    yield();
    #ifdef ALS_LAMPUDPMULTI
      UdpMulti::send(out) ;   
    #endif   
  }    
  else if (op == 8) {
    out = "";
    // outputs_toAppi
    // (String & out, boolean eff, boolean prog, uint8_t effLoad = 0, boolean sJson = 0);
    DevicePtrGet()->outputs_toAppi(out, false, true, 0); 
    Udp::send_toIp(out);  
    yield();
    #ifdef ALS_LAMPUDPMULTI
      UdpMulti::send(out);   
    #endif 
  }  

  multiSend_jsonDevice = true;  
  timerMultiSend_jsonDevice = millis(); 

  ALT_TRACEC("main", "--\n");
}
void RemoteControl_udp::handleJson(DynamicJsonDocument & doc, boolean udpMulti){

/*
  JSON 
    obj ->  op  

    op 0 || 1 || 2 || 3 = handle from controlid = besion d'interompre la mise jour aux serveur
    op == 0 : group d'outputs 
      obj ->  req     | string  | "commande a effectuer pour tous le grp"  
      obj ->  val     | obj     | "valeure de la commande" (for colorpicker(rgb hsv) : .r .g .b .h .s .v)(other : .v)
      obj ->  from    | byte    |
      obj ->  array   | byte    |
      array == 0 : liste des output par position trier par device          
        array ->  A     |         | array of devices 
          obj   -> dn   | string  |"device name" 
          array -> A    | Array   | array of output position  
    op == 1 : device   
      obj ->  dn      | string  |
      obj ->  req     | string  |
      obj ->  val     | obj     |
      obj ->  from    | byte    |
    op == 2 : all device   
      obj ->  req     | string  |
      obj ->  val     | obj     |
      obj ->  from    | byte    |
    op == 3 : output   
      obj ->  dn      | string  |
      obj ->  output  | byte    |
      obj ->  req     | string  |
      obj ->  val     | obj     |
      obj ->  from    | byte    |

    op == 4 : handle control id realesed 
      sauvegarde FS
      reponse udpmulti
      reponse udp suivant la dernierre requette reçu
      réactivation timerAuto mise a a jour serveur
     op == 5
      preset
        load from fs
        sav to fs
        send update server
    op == 6
      device management     
    op == 7 : handle control id realesed 
      sauvegarde FS
      reponse udp effect
      réactivation timerAuto mise a a jour serveur
    op == 8 : handle control id realesed 
      sauvegarde FS
      reponse udpmulti
      reponse udp effect+outputs
      réactivation timerAuto mise a a jour serveur



*/
  

  if (doc.containsKey(FPSTR(ALMLPT_GET)) && doc.containsKey(FPSTR(ALMLPT_SET))) {
    String reply = "";
    _AP_Api.parsingRequest(doc, reply, "");
    if (ProgramPtrGet()->isPlaying()) {
      ProgramPtrGet()->remote_action(apSetter_t::APSET_ITEM);
    } else {
      handleJson(8); 
    }
    multiSend_jsonDevice = false;
    
    #ifdef DEBUG_ALMLREMOTE
      if (ALT_debugPrint(ALML_DEBUGREGION_REMOTE)) {String s;serializeJsonPretty(doc, Serial);Serial.println();}
    #endif
    return;
  }

  if (doc.containsKey(FPSTR(ALMLPT_CLI))) {
    if ( doc[FPSTR(ALMLPT_CLI)].as<String>() == FPSTR(ALMLPT_RGB))        {return;}
    if ( doc[FPSTR(ALMLPT_CLI)].as<String>() == FPSTR(ALMLPT_RELAY_AC))   {return;}
    if ( doc[FPSTR(ALMLPT_CLI)].as<String>() == FPSTR(ALMLPT_PLUG))       {return;}
  }
  if (doc.containsKey(FPSTR(ALMLPT_FROM))) {
    // if ( doc[FPSTR(ALMLPT_FROM)].as<uint8_t>() != 0 && doc[FPSTR(ALMLPT_FROM)].as<uint8_t>() != 1) {return;}
    if      ( doc[FPSTR(ALMLPT_FROM)].as<uint8_t>() == 0 ) {;}
    else if ( doc[FPSTR(ALMLPT_FROM)].as<uint8_t>() == 1 ) {;}
    else return;
  }

  uint8_t op = 0;
  if (doc.containsKey(FPSTR(ALMLPT_OP))) {
    op = doc[FPSTR(ALMLPT_OP)];
    if ( doc.containsKey(FPSTR(ALMLPT_FROM)) && doc[FPSTR(ALMLPT_FROM)].as<uint8_t>() == 1 ) Udp::client_add(); 
  } else {
    return;
  }

  #ifdef DEBUG_ALMLREMOTE
    if (ALT_debugPrint(ALML_DEBUGREGION_REMOTE)) {String s;serializeJsonPretty(doc, Serial);Serial.println();}
  #endif

  if (op == 9) { 
    timerUdpSync = millis();
    testHue = doc[FPSTR(ALMLPT_VAL)][F("v")].as<uint8_t>();
    return;    
  }    
  else if (op == 4) { 
    doc.clear(); doc.garbageCollect();
    handleJson(op);
    return;    
  }
  else if (op == 7) { 
    doc.clear(); doc.garbageCollect();
    handleJson(op); 
    return;    
  }    
  else if (op == 8) {
    doc.clear(); doc.garbageCollect();
    handleJson(op);  
    return;
  } else {

    multiSend_jsonDevice = false;

    if ((op == 0) && doc.containsKey(FPSTR(ALMLPT_ARRAY))) {
      uint8_t array = doc[FPSTR(ALMLPT_ARRAY)];
      if (array == 0) {
        DevicePtrGet()->parseJson_outpitListByDn(doc);
      } 
    } else if ((op == 1) && doc.containsKey(FPSTR(ALMLPT_DN)) ) {
      String dn       = doc[FPSTR(ALMLPT_DN)].as<String>();
      String thisDn   = al_tools::ch_toString(DevicePtrGet()->get_name());
      if (dn ==  thisDn) {
        if (doc.containsKey(FPSTR(ALMLPT_API_OP))) {
          String almlapiReponse="";
          _Webserver.device_api(doc, almlapiReponse);  
        }        
        DevicePtrGet()->parseJson_device(doc);
      }

    } else if (op == 2)  {
      if (doc.containsKey(FPSTR(ALMLPT_API_OP))) {
        String almlapiReponse="";
        _Webserver.device_api(doc, almlapiReponse);  
      }    
      DevicePtrGet()->parseJson_device(doc);

    } else if ((op == 3) && doc.containsKey(FPSTR(ALMLPT_DN))) {
      String dn = doc[FPSTR(ALMLPT_DN)].as<String>();
      String thisDn = al_tools::ch_toString(DevicePtrGet()->get_name());
      if (dn ==  thisDn) {
        DevicePtrGet()->parseJson_output(doc);
      }
      
    } else if (op == 5)  {
      if (doc.containsKey(FPSTR(ALMLPT_PRESET))) {
        if (doc[FPSTR(ALMLPT_PRESET)].as<String>() == FPSTR(ALMLPT_SAV))   DevicePtrGet()->preset_sav(doc);
        if (doc[FPSTR(ALMLPT_PRESET)].as<String>() == FPSTR(ALMLPT_LOAD))  DevicePtrGet()->preset_load(doc);
      }
    }  
    
       
  }
}
void api_getter(DynamicJsonDocument & doc, const char * in) {
  char    * key = nullptr;
  boolean cpy   = true;
  uint8_t count = ARRAY_SIZE(ALMLPT_KKEY_ALL);
  if(isDigit(al_tools::ch_toString(in).charAt(0))) {
    for(int i = 0; i < count; ++i) {
      if (al_tools::ch_toString(in).toInt() == i) {
        key = new char[255];
        strcpy(key, ALMLPT_KKEY_ALL[i]);
        cpy=false;
        break;
      }  
    }    
  }
  if (cpy) {
    key = new char[strlen(in)+1];
    strcpy(key, in);
  }

  if ( al_tools::ch_toString(key) == FPSTR(ALMLPT_KKEY_000) ) 
    EffectslistPtrGet()->get_program()->print();

  if ( al_tools::ch_toString(key) == FPSTR(ALMLPT_KKEY_007) )  {
    Serial.printf_P(PSTR("ALL Effectslist\n"));
    for(int i = 0; i < _effeNamIDList_cnt; ++i) {
      String name = FPSTR(_effeNamIDList[i]._nameId);
      String type = FPSTR(_effeNamIDList[i]._effType);
      String cat  = FPSTR(_effeNamIDList[i]._cat);
      String seg  = FPSTR(_effeNamIDList[i]._segment);
      Serial.printf_P(PSTR("[%-3d] %-40s %-10s %-3s %-3s\n"), 
        i, 
        name.c_str(), 
        type.c_str(), 
        cat.c_str(), 
        seg.c_str());

    }
    Serial.println();
  }

  if ( al_tools::ch_toString(key) == FPSTR(ALMLPT_KKEY_009) )  {
    Serial.println(F("PRINT CONTROLS"));
    LList<UIControl*>&controlsPrint = LAMPPTRGET()->get_effect(0)->getControls();
    for(int i = 0; i < controlsPrint.size(); ++i) {
      controlsPrint[i]->print(true);
    }  
  }
  

  if ( al_tools::ch_toString(key) == FPSTR(ALMLPT_KKEY_008) )  {
    EffectslistPtrGet()->get_program()->pl_print(0);
  }
  

  if ( al_tools::ch_toString(key) == FPSTR(ALMLPT_KKEY_001) ) {
    uint8_t oP;
    DevicePtrGet()->get_outputCount(oP);
    for(int i = 0; i < oP; ++i) {
      JsonObject effectObj = doc.createNestedObject("opeff_"+String(i));
      _effects[i]->geteffconfig(i, effectObj, LAMPPTRGET()->get_globalBrightness(i, 0));       
    }
  }

  if ( al_tools::ch_toString(key) == FPSTR(ALMLPT_KKEY_002) ) {
    JsonObject oOjbect = doc.createNestedObject(FPSTR(ALMLPT_OUTPUTS));
    DevicePtrGet()->outputs_toJson(oOjbect, false, true, false);
  }
  if ( al_tools::ch_toString(key) == FPSTR(ALMLPT_KKEY_003) ) {
    JsonObject oOjbect = doc.createNestedObject(FPSTR(ALMLPT_OUTPUTS));
    DevicePtrGet()->outputs_toJson(oOjbect, false, true, true);
  }

  if ( al_tools::ch_toString(key) == FPSTR(ALMLPT_KKEY_010) ) 
    reqNamIDList_json(0, doc);  

  if ( al_tools::ch_toString(key) == FPSTR(ALMLPT_KKEY_004) ) { 
    reqList_json(doc);
  }  

  if ( al_tools::ch_toString(key) == FPSTR(ALMLPT_KKEY_005) ) 
    {serializeJsonPretty(DeviceUserConfig, Serial);Serial.println();}

  if (key) delete key;

}
void keyboard_getter(const String & v1) {
  int rSize = 0;
  DynamicJsonDocument doc(3500);

  LList<SplitItem *> _SplitItem;
  splitText(v1, "&",  ':', &_SplitItem);

  for(int j = 0; j < _SplitItem.size(); ++j) {
    const char** split = al_tools::explode(_SplitItem[j]->_value, ',', rSize);
    if (split) {
      for(int i = 0; i < rSize; ++i) {
        Serial.printf_P(PSTR("[%d] %s\n"), i , split[i]);
        if (strcmp_P(_SplitItem[j]->_cmd, "JSON") == 0)     api_getter(doc, split[i]);                           
        // #ifdef ALSI_ENABLED
        // if (strcmp_P(_SplitItem[j]->_cmd, "ALSI") == 0)   ALSYSINFO_getterByCat(doc, split[i]);                           
        // if (strcmp_P(_SplitItem[j]->_cmd, "ALSII") == 0)  ALSYSINFO_getterByKey(doc, split[i]);   
        // #endif                          
      }
      for(int i = 0; i < rSize; ++i) {
        delete split[i];
      }
      delete[] split; 
    } else {
        if (strcmp_P(_SplitItem[j]->_cmd, "JSON") == 0)     api_getter(doc, _SplitItem[j]->_value);                           
        // #ifdef ALSI_ENABLED
        // if (strcmp_P(_SplitItem[j]->_cmd, "ALSI") == 0)   ALSYSINFO_getterByCat(doc, _SplitItem[j]->_value);                           
        // if (strcmp_P(_SplitItem[j]->_cmd, "ALSII") == 0)  ALSYSINFO_getterByKey(doc, _SplitItem[j]->_value);   
        // #endif        
    }
  }
  while (_SplitItem.size()) {
    SplitItem *eff = _SplitItem.shift();
    delete eff;
  }
  _SplitItem.clear();

  serializeJsonPretty(doc,Serial);Serial.println(); 
   
}
void keyboard_print() {
  Serial.printf_P(PSTR("!&JSON:0= !&JSON:0,json_effect=\n"));
  uint8_t count = ARRAY_SIZE(ALMLPT_KKEY_ALL);
  for(int i = 0; i < count; ++i) {
    Serial.printf_P(PSTR("\t[%-3d] %s\n"), i, ALMLPT_KKEY_ALL[i]);
  }    
}

