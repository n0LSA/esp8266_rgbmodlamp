  // uint8_t excluedStart = 0;
  // String  excluedItem = "";
  // uint8_t  excluedExactItem = 0;
  // uint8_t  includeExactItem = 0;

  // boolean exctMatch = false;
  // boolean incluedeExctMatch = false;
  // if (DeviceUserConfig.containsKey(F("list_lb_exclued_cfg"))) exctMatch = true;
  // if (DeviceUserConfig.containsKey(F("list_lb_include_cfg"))) incluedeExctMatch = true;

/*
  for(int i = 0; i < _effeNamIDList_cnt; ++i) {
    String item = al_tools::ch_toString(_effeNamIDList[i]._nameId);
    for (int j = 0; j < _includeFoundItem.size(); ++j) {
      if (!_includeFoundItem.get(j)->_callBack(item)) {
        cnt++;
        break;
      }
    }
  }
*/
/*
    boolean isExclude = false;
    for (int j = 0; j < _includeFoundItem.size(); ++j) {
      if (!_includeFoundItem.get(j)->_callBack(item)) {
        isExclude=true;
        break;
      }
    }
    if (!isExclude) continue;
*/
/*
 *
    excluedItem       = "";
    excluedStart      = 0;
    excluedExactItem  = 0;
    includeExactItem  = 0;

    JsonArray excluedArr  = DeviceUserConfig[F("list_lb_excluedExact")].as<JsonArray>();
    uint8_t   excluedSize = excluedArr.size();
    for(uint8_t j = 0; j < excluedSize; ++j) {
      excluedItem = excluedArr[j].as<String>();
      if(excluedItem != "" && item == excluedItem)  {excluedExactItem++;break;}
    }
    if (excluedExactItem>0) continue;

    excluedArr  = DeviceUserConfig[F("list_lb_include")].as<JsonArray>();
    excluedSize = excluedArr.size();
    for(uint8_t j = 0; j < excluedSize; ++j) {
      excluedItem = excluedArr[j].as<String>();
      if (!incluedeExctMatch) {
        if(excluedItem != "" && item.indexOf(excluedItem) < 0)  includeExactItem++;
      } else {
        if(excluedItem != "" && item != excluedItem)  includeExactItem++;
      }
    }
    if (includeExactItem>0) continue;


    excluedArr  = DeviceUserConfig[F("list_lb_exclued")].as<JsonArray>();
    excluedSize = excluedArr.size();
    for(uint8_t j = 0; j < excluedSize; ++j) {
      excluedItem = excluedArr[j].as<String>();
      if (!exctMatch) {
        if(excluedItem != "" && item.indexOf(excluedItem) > -1)  excluedStart++;
      } else {
        if(excluedItem != "" && item == excluedItem)  excluedStart++;
      }      
    }
    if (excluedStart>0)continue;
*/

#include "../../include/display/effectslist.h"
#include "../../include/display/remote.h"
#include "../../include/display/lamp.h"
#include "../../include/device/device.h"


Effectlist_exclude::Effectlist_exclude() {
  docArrList = new DynamicJsonDocument(1024);
}
void Effectlist_exclude::set_callback(_callback v1) {
  _callBack = std::move(v1);
}
bool Effectlist_exclude::includeFoundItem(String item) {
    JsonArray excluedArr    = (*docArrList)[F("list")].as<JsonArray>();
    uint8_t   excluedSize   = excluedArr.size();
    uint8_t   result        = 0;
    bool      woleWord      = (*docArrList)[F("woleWord")].as<bool>();

    for(uint8_t j = 0; j < excluedSize; ++j) {
      String itemStr = excluedArr[j].as<String>();
      if (!woleWord) {
        if(itemStr != "" && item.indexOf(itemStr) < 0)  result++;
      } else {
        if(itemStr != "" && item != itemStr)            result++;
      }
    }
    if (result>0) return true;
    return false;
}
bool Effectlist_exclude::exludeFoundItem(String item) {
    JsonArray excluedArr    = (*docArrList)[F("list")].as<JsonArray>();
    uint8_t   excluedSize   = excluedArr.size();
    uint8_t   result        = 0;
    bool      woleWord      = (*docArrList)[F("woleWord")].as<bool>();
    bool      skipAll       = (*docArrList)[F("skipAll")].as<bool>();

    if (skipAll) return true;

    for(uint8_t j = 0; j < excluedSize; ++j) {
      String itemStr = excluedArr[j].as<String>();
      if (!woleWord) {
        if(itemStr != "" && item.indexOf(itemStr) > -1)   result++;
      } else {
        if(itemStr != "" && item == itemStr)              result++;
      }
    }
    if (result>0) return true;
    return false;
}



LList<Effectlist_exclude *>  _includeFoundItem;  
LList<Effectlist_exclude *>  _exludeFoundItem;  

Effectslist * _EffectslistPtr = nullptr;
Effectslist * EffectslistPtrGet() {return _EffectslistPtr;}

extern EffectWorker  * _effects[];


void _Program_cb(const String itemBaseName, const uint16_t& itemBasePos, uint8_t id, boolean updWebserver) {
  // Serial.printf_P(PSTR("[user_callback]\n\t[%-3d] %-35s %d\n"), itemBasePos, itemBaseName.c_str(), id);
  
  if ( LAMPPTRGET()->effectsLoop_activate() <= 0 ) return;

  _EffectslistPtr->eff_byId(id);

  if (!RemoteControlPtrGet()) return;

  uint8_t get_lastOutput = DevicePtrGet()->get_lastOutput();
  Serial.printf_P(PSTR("get_lastOutput: %d\n"), get_lastOutput);
  String out = "";
  DevicePtrGet()->output_toAppi(out, get_lastOutput, 1, 1, 0, 0);
  RemoteControlPtrGet()->Udp::send_toIp(out); 
  yield();

}


Effectslist::Effectslist(){
  ALT_TRACEC("main", "-\n");

  _EffectslistPtr = this;

  uint8_t cnt = 0;

  LList<int> myList = LList<int>();
  LList<int> myListIsInclude = LList<int>();

  for (int j = 0; j < _effeNamIDList_cnt; ++j) {
    for(int i = 0; i < _includeFoundItem.size(); i++) {
      String item = al_tools::ch_toString(_effeNamIDList[j]._nameId);
      if (_includeFoundItem.get(i)->exludeFoundItem(item)) {
        myListIsInclude.add(j);
      }
    }
  }

  Serial.println();
  for(int i=0; i<myListIsInclude.size(); i++){
    int iPos = myListIsInclude.get(i);
    Serial.printf_P(PSTR("myListIsInclude - %4d : %4d - %s\n"), 
      i, iPos, _effeNamIDList[iPos]._nameId);
  }

  for (int j = 0; j < _exludeFoundItem.size(); ++j) {

    for(int i = 0; i < _effeNamIDList_cnt; i++) {

      boolean isInclude = false;
      for(int k = 0; k < myListIsInclude.size(); k++) {
        if (myListIsInclude.get(k) == i) { isInclude = true; break;}
      }
      if (isInclude) continue;

      String item = al_tools::ch_toString(_effeNamIDList[i]._nameId);

      if (_exludeFoundItem.get(j)->exludeFoundItem(item)) {
        myList.add(i);

      }
    }
  }

  Serial.println();
  for(int i=0; i<myList.size(); i++){
    int iPos = myList.get(i);
    Serial.printf_P(PSTR("myListIsExclude  - %4d : %4d - %s\n"), i, iPos, _effeNamIDList[iPos]._nameId);
  }
  Serial.println();

  for(int i = 0; i < _effeNamIDList_cnt; ++i) {
    uint8_t cntExclude = 0;
    for(int j = 0; j < myList.size(); j++) {
      if (myList.get(j) == i) cntExclude++;
    }
    if (cntExclude == 0) cnt++;
  }

  _AP_DEFAULTLIST = new const char*[cnt+1]; 
  _listId         = new uint8_t[cnt+1]; 
  uint8_t pos = 0;

  for(int i = 0; i < _effeNamIDList_cnt; ++i) {
    String item = al_tools::ch_toString(_effeNamIDList[i]._nameId);

      uint8_t cntExclude = 0;
      for(int j = 0; j < myList.size(); j++) {
        if (myList.get(j) == i) cntExclude++;
      }
      if (cntExclude > 0) continue;

    char * buff = new char[item.length()+1];
    strcpy(buff, item.c_str());
    _AP_DEFAULTLIST[pos] = strdup(buff);
    _listId[pos] = i;

    pos++;

    delete buff;
  }

  while (_includeFoundItem.size()) {
    Effectlist_exclude * item = _includeFoundItem.shift();
    delete item;
  }
  _includeFoundItem.clear();  
  while (_exludeFoundItem.size()) {
    Effectlist_exclude * item = _exludeFoundItem.shift();
    delete item;
  }
  _exludeFoundItem.clear();  




  // for(int i = 0; i < cnt; ++i) {
  //   Serial.printf_P(PSTR("[%-3d] %-3d | %-35s\n"), i, _listId[i],_AP_DEFAULTLIST[i]);
  // }
  _Program = new Program(1, true);
  _Program->set_fs_pl(false);  
  _Program->set_callback(_Program_cb);
  _Program->initialize_lb(0, "full", cnt , _AP_DEFAULTLIST);
  _Program->initialize( cnt, _AP_DEFAULTLIST, _listId, "full", apListSortType_t::ST_AB);
  _Program->print();

  uint8_t plC       = 2;
  uint8_t iC[]      = {12, 12};  // nb items max
  const char * Ln[] = {"full", "full"};
  _Program->initialize_playlist(plC, iC, Ln);
  

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

  _Program->print();
  Serial.println();

  DynamicJsonDocument apRequest(1024);
  apRequest[F("op")]    = 0;
  apRequest[F("type")]  = "HTTP_POST";
  apRequest.createNestedArray(F("set"));
  JsonArray getter = apRequest.createNestedArray(F("get"));
  getter.add("list_lbid");
  DynamicJsonDocument apReply(2048);
  _AP_Api.parsingRequest(apRequest, apReply, "");

  JsonArray List = apReply[F("list_lbid")][F("items")].as<JsonArray>();
  // for (JsonVariant value : List) {
  //   String    name = value[0].as<String>();
  //   uint8_t   id = value[1].as<uint8_t>();
  //   Serial.printf("name: %-25s - id: %d\n", name.c_str(), id);
  // }
  // 
  // 

  char bufferFolder[80];
  LAMPPTRGET()->getStripFolder(0, bufferFolder);
// void EffectWorker::copyeffconfig(uint16_t nb, const char *srcFolder, const char *destFolder, String & path)

  if(!FILESYSTEM.exists(F("/playlist/playlist_0.json"))) {
    Serial.printf_P(PSTR("10 rnd items\n"));
    for (int i = 0; i < 5; ++i) {
      uint8_t pos = random(0, List.size());
      JsonArray item  = List[pos];
      String    name  = item[0].as<String>();
      uint8_t   id    = item[1].as<uint8_t>();
      String    src   = _effects[0]->geteffectpathname(id, bufferFolder);

      // String destFolder = "playlist/" + String();
      //void Program::pl_item_toArray(uint8_t pP, uint8_t iP, const String & lbl, const String & itemBase, const String & itemBaseCfg) {

      uint8_t plId = _Program->pl_item_toArray(0, 255, name, name, src, id);

       Serial.printf("name: %-45s - id: %d - pldId: %d - src: %s\n", 
        name.c_str(), id, plId, src.c_str());
      
      uint8_t _itemId;
      uint8_t _id;;
      String  _lbl;;
      String  _itemBase;;
      String  _itemBaseCfg;
      _Program->pl_get(0)->get_array(plId)->get_id(_id);
      _Program->pl_get(0)->get_array(plId)->get_itemId(_itemId);
      _Program->pl_get(0)->get_array(plId)->get_lbl(_lbl);
      _Program->pl_get(0)->get_array(plId)->get_itemBase(_itemBase);
      _Program->pl_get(0)->get_array(plId)->get_itemBaseCfg(_itemBaseCfg);

       Serial.printf("_id: %-5d - _itemId: %-5d - _lbl: %-45s - _itemBase: %-45s - _itemBaseCfg: %s\n", 
        _id, _itemId, _lbl.c_str(), _itemBase.c_str(), _itemBaseCfg.c_str());    

    }
    Serial.println("playlist_0 RESTART REEQUIERED");
    _Program->pl_fs(0);
    yield();
    ESP.restart();
  }
  if(!FILESYSTEM.exists(F("/playlist/playlist_1.json"))) {
    Serial.printf_P(PSTR("10 rnd items\n"));
    for (int i = 0; i < 5; ++i) {
      uint8_t pos = random(0, List.size());
      JsonArray item  = List[pos];
      String    name  = item[0].as<String>();
      uint8_t   id    = item[1].as<uint8_t>();
      String    src   = _effects[0]->geteffectpathname(id, bufferFolder);
      uint8_t   plId  = _Program->pl_item_toArray(1, 255, name, name, src, id);

      Serial.printf("name: %-45s - id: %d - pldId: %d - src: %s\n", 
        name.c_str(), id, plId, src.c_str());
      
      uint8_t _itemId;
      uint8_t _id;;
      String  _lbl;;
      String  _itemBase;;
      String  _itemBaseCfg;
      _Program->pl_get(1)->get_array(plId)->get_id(_id);
      _Program->pl_get(1)->get_array(plId)->get_itemId(_itemId);
      _Program->pl_get(1)->get_array(plId)->get_lbl(_lbl);
      _Program->pl_get(1)->get_array(plId)->get_itemBase(_itemBase);
      _Program->pl_get(1)->get_array(plId)->get_itemBaseCfg(_itemBaseCfg);

      Serial.printf("_id: %-5d - _itemId: %-5d - _lbl: %-45s - _itemBase: %-45s - _itemBaseCfg: %s\n", 
        _id, _itemId, _lbl.c_str(), _itemBase.c_str(), _itemBaseCfg.c_str());    

    }
    Serial.println("playlist_1 RESTART REEQUIERED");
    _Program->pl_fs(1);
    yield();
    ESP.restart();
  }  
  
  _Program->pl_fs_restore();  
   Serial.println();


  // _Program->pl_print(0);
  // Serial.println();

  // serializeJsonPretty(apReply, Serial);Serial.println();

/*
  for(int i = 0; i < _effeNamIDList_cnt; ++i) {
    String cfg  = FPSTR(_effeNamIDList[i]._defaultCfg);
    cfg.replace(F("@name@"), FPSTR(_effeNamIDList[i]._nameId));
    Serial.printf_P(PSTR("[%-3d] %-22s\n"), i, cfg.c_str());
  }  
*/
  _Program->remote_action(apSetter_t::APSET_LSET_PL, "0");
  _Program->remote_action(apSetter_t::APSET_PLAY_LB);
  _Program->remote_action(apSetter_t::APSET_PLAY_DELAY, "180");
  _Program->remote_action(apSetter_t::APSET_PLAY_STOP);
  _Program->remote_action(apSetter_t::APSET_ITEMID, "0");

  //  uint16_t plIcnt = 0;
  // _Program->pl_get(0)->get_items_cnt(plIcnt);
  // if (plIcnt == 0) {
  //   Serial.println(F("Create random playlist"));
  //   _Program->pl_item_new(0);
  //   _Program->remote_action(apSetter_t::APSET_ITEM_NEXT);
  //   _Program->pl_item_new(0);
  //   _Program->remote_action(apSetter_t::APSET_ITEM_NEXT);
  //   _Program->pl_item_new(0);
  //   _Program->pl_fs(0);
  //   _Program->pl_print(0);
  // } 

  ALT_TRACEC("main", "--\n");
}

void Effectslist::eff_next(){
  _Program->remote_action(apSetter_t::APSET_ITEM_NEXT); 
}
void Effectslist::eff_prev(){
  _Program->remote_action(apSetter_t::APSET_ITEM_PREV); 
}
void Effectslist::toggle(){
  _Program->remote_action(apSetter_t::APSET_PLAY_TOGGLE); 
}
void Effectslist::eff_byId(uint8_t p){
  DynamicJsonDocument doc(255);
  doc[FPSTR(ALMLPT_REQ)]   = FPSTR(req_eff_f);
  JsonObject val = doc.createNestedObject(FPSTR(ALMLPT_VAL));
  val[FPSTR(ALMLPT_V)]     = p;
  val[FPSTR(ALMLPT_C)]     = FPSTR(req_eff_load);
  DevicePtrGet()->parseJson_device(doc);
}
void Effectslist::eff_byName(const char * name){
  // uint8_t cnt = ARRAY_SIZE(T_EFFNAMEID);
  for (uint8_t i = 0; i < _effeNamIDList_cnt; ++i) {
    String find = FPSTR(_effeNamIDList[i]._nameId);
    if (find == al_tools::ch_toString(name)) {
      DynamicJsonDocument doc(500);
      doc[FPSTR(ALMLPT_REQ)]   = FPSTR(req_eff_f);
      JsonObject val = doc.createNestedObject(FPSTR(ALMLPT_VAL));
      val[FPSTR(ALMLPT_V)]     = i;
      val[FPSTR(ALMLPT_C)]     = FPSTR(req_eff_load);
      DevicePtrGet()->parseJson_device(doc);
      return;
    }
  }
}
/*
void EffectList::effectSet() {
  String    name;
  uint16_t  effPos;
  _programme->effectCurrentGet(name); 
  if (effects_getByName(name, effPos)) {LAMPPTRGET()->effectChangeByPos(effPos, false, false);}
}
void EffectList::effectSet(boolean dir) {
  String    n;
  uint16_t  effPos;
  _programme->effectSet(dir, n);
  if (effects_getByName(n, effPos)) LAMPPTRGET()->effectChangeByPos(effPos);
}
*/


Program * Effectslist::get_program() {return _Program;}