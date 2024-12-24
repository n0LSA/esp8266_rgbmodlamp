#include "../../include/display/lamp.h"
#include "../../include/device/device.h"
/*
  struct SettingsListValue {
    const char * name;
    void (*getValue)(String & ret);
    uint8_t (*setValue)(const String & value, int mod);
  };  



boolean toBoolean(const String &value){
  if (value ==  "1")      {return true;}        
  if (value ==  "true")   {return true;}
  if (value ==  "TRUE")       {return true;}
  if (value ==  "True")       {return true;}
  if (value ==  "Activer")    {return true;}  
           
  if (value ==  "0")          {return false;}        
  if (value ==  "false")      {return false;}
  if (value ==  "FALSE")      {return false;}
  if (value ==  "False")      {return false;}
  if (value ==  "Desactiver") {return false;}        
  return true;
}   
uint8_t isLampOnSet           (const String & v, int mod) {if (mod==1) LAMPPTRGET()->set_lampOn(toBoolean(v)); return 0;}
uint8_t IsGlobalBrightnessSet (const String & v, int mod) {LAMPPTRGET()->set_globalBrightness(toBoolean(v)); return 0;}
uint8_t isDebugSet            (const String & v, int mod) {LAMPPTRGET()->set_debug(toBoolean(v));        return 0;}
uint8_t isRandDemoSet         (const String & v, int mod) {
  LAMPPTRGET()->set_randDemo(toBoolean(v)); 
  if (mod == 1 && !LAMPPTRGET()->isRandDemoOn()) {
    DevicePtrGet()->eff_load();
    return 1;
  }
  return 0;
}
*/
void LAMP::json_generalSetting(JsonObject & root){ 
  JsonArray arr = root.createNestedArray(F("general"));
  arr.add(isLampOn());
  arr.add(isGlobalBrightnessOn());
  arr.add(isDebugOn());
  arr.add(isRandDemoOn());
}
  

EffectWorker  * _effects[ADS_OC] ;
Device        * _DeviceGet        = nullptr;
LAMP          * LAMPptr           = nullptr;
TaskSimple    * _task_fadeTicker  = nullptr;

LAMP * LAMPPTRGET(){return LAMPptr;};
LAMP::LAMP(uint8_t p){
  LAMPptr = this;

  _task_fadeTicker = new TaskSimple();

  _DeviceGet = DevicePtrGet();

  _DeviceGet->get_outputCount(_outPutCount);

  set_debug             (false);
  set_randDemo          (false);
  set_lampOn            (true);
  set_fader             (false);
  set_globalBrightness  (true);

  for(int i=0; i<_outPutCount; i++){
    _effects[i] = new EffectWorker(&_lampState);
  } 
}

EffectWorker * LAMP::get_effect(uint8_t sp) {return _effects[sp];}

void LAMP::setup() {    
}

void LAMP::loop() {  
  if (effectsLoop_activate() > 0) effectsLoop();
  if(_task_fadeTicker) {if (_task_fadeTicker->isEnabled()) _task_fadeTicker->execute();}
}
/*
void EffectList::playList_name(String & ret) {
  ret = "";
  _playlist->list_name(ret);
  int iPos;
  _playlist->item_pos(iPos);
  ADRI_LOG(-1,2,2,"\niPos: %d", iPos);
  ret = (String)FPSTR(FOPATH_PLAYLISTEFF) + ret + (String)FPSTR(FSLSH) + String(iPos);
}
*/
void LAMP::getStripFolder(uint8_t pos, char buffer[]) {
  DynamicJsonDocument doc(1024);
  JsonObject obj = doc.to<JsonObject>();
  ProgramPtrGet()->get_json_jsInit(obj);
  bool plIsplaying = obj[F("pl")][F("play")].as<bool>();
  if (!plIsplaying) {
    String folderName = "eff/STRIP_" + String(pos);
    sprintf(buffer, "%s", folderName.c_str());    
  } else {
    uint8_t pPos = obj[F("pl")][F("pos")].as<uint8_t>();
    String folderName = "pleff_" + String(pPos) + "/STRIP_" + String(pos);
    sprintf(buffer, "%s", folderName.c_str());    
  }
  // String folderName = "eff/STRIP_" + String(pos);
  // sprintf(buffer, "%s", folderName.c_str());    
  // Serial.print("folderName: ");Serial.println(buffer);
}


uint8_t LAMP::effectsLoop_hue(){
  uint8_t cnt=0;
  for (int i = 0; i < _outPutCount; ++i) {
    if (_DeviceGet->get_outputArray(i)->get_onoff() && _DeviceGet->get_outputArray(i)->get_select() ) {
      if ( !_effects[i]->worker ) continue;
      if (_effects[i]->worker->get_effectType() == OET_HUETIMER) cnt++;
      if (_effects[i]->worker->get_effectType() == OET_HTPG) {cnt++; }
    }
  }
  return cnt;
}
uint8_t LAMP::effectsLoop_pal(){
  uint8_t cnt=0;
  for(uint8_t i=0; i<_outPutCount; i++){ 
      if ( !_effects[i]->worker ) continue;
      if (  
        _DeviceGet->get_outputArray(i)->get_eff_activ() 
        && _effects[i]->worker->get_paletteTimer() 
        && _effects[i]->worker->isPalEffect()   
        ) {cnt++;}
      if (  
        _DeviceGet->get_outputArray(i)->get_eff_activ() 
        && _effects[i]->worker->isHuePalEffect() 
        && _effects[i]->worker->getHue_1() == 0
        ) {cnt++;}        
  }
  return cnt;
}

uint8_t LAMP::effectsLoop_activate(){
  uint8_t cnt = 0;
  for (int i = 0; i < _outPutCount; ++i) {
    if (_DeviceGet->get_outputArray(i)->get_onoff() && _DeviceGet->get_outputArray(i)->get_eff_activ() ) {
      cnt++;
    }
  }
  return cnt;
}


void LAMP::effectsLoop_pal_reset(uint8_t p){
  _palPos = p;
  _timer_effectsLoopPal = millis();
}

boolean LAMP::effectsLoop_TimerHue(const String & cmd, const JsonObject & value){
  if ((cmd != FPSTR(req_eff_timerCol))) return false;
  uint32_t v = value[FPSTR(ALMLPT_V)].as<uint32_t>();
  _effectsLoop_timerMax = v;
  return true;  
}
void LAMP::effectsLoop_TimerHue(uint32_t value){
  _effectsLoop_timerMax = value;
}
boolean LAMP::effectsLoop_TimerPal(const String & cmd, const JsonObject & value){
  if ((cmd != FPSTR(req_eff_timerPal))) return false;
  uint16_t v = value[FPSTR(ALMLPT_V)].as<uint16_t>();
  _effectsLoop_timerPalMax = v;  
  return true;
}
void LAMP::effectsLoop_TimerPal(const uint32_t & v){
  _effectsLoop_timerPalMax = v;  
}

boolean LAMP::effectsLoop_Timer(const String & cmd, const JsonObject & value){
  if (effectsLoop_TimerHue(cmd, value))     return true;
  if (effectsLoop_TimerPal(cmd, value))     return true;
  return false;
}


void LAMP::effectsLoop(){
  // if (millis() - RemoteControlPtrGet()->timerUdpSync < 1000) {
  //   _hsvLoop_1.set_hue( RemoteControlPtrGet()->testHue );
  // } else {
    
    if (isHueLoopOn()){
      _hsvLoop_1.hueLoop(_effectsLoop_timerMax);  
      _hsvLoop_2.hueLoop(_effectsLoop_timerMax); 
    }

    if (isPalLoopOn()){
      if ((millis()-_timer_effectsLoopPal)>_effectsLoop_timerPalMax){
          _palPos++;
          _timer_effectsLoopPal = millis();
          // Serial.printf("effectsLoop - pal change: %d\n", _palPos);
      }     
    }
    
  // }
}


void LAMP::effect_setspd(uint8_t stripPos, uint8_t value) {
  uint8_t min = _effects[stripPos]->getControls()[1]->getMin().toInt();
  uint8_t max = _effects[stripPos]->getControls()[1]->getMax().toInt();
  if (value>max) value = map(value, 0, 255, min, max);
  _effects[stripPos]->getControls()[1]->setVal(String(value));
  _effects[stripPos]->worker->setspd(value);
  #ifdef ALT_DEBUG_TARCE
    // _effects[stripPos]->getControls()[1]->print();  
  #endif
}
void LAMP::effect_setspd_incr(uint8_t stripPos, int8_t amout) {
  uint8_t min = _effects[stripPos]->getControls()[1]->getMin().toInt();
  uint8_t max = _effects[stripPos]->getControls()[1]->getMax().toInt();
  uint8_t current = _effects[stripPos]->getControls()[1]->getVal().toInt();
  // uint8_t mValue = map((uint8_t)current + amout, 0, 255, min, max);

  int fb = (current+amout);
  if ((amout>0) && (fb > max) ) return;
  if ((amout<0) && (fb < min) ) return; 

  uint8_t value = map((uint8_t)current + amout, 0, 255, min, max);

  _effects[stripPos]->getControls()[1]->setVal(String(value));
  _effects[stripPos]->worker->setspd(value);
  #ifdef ALT_DEBUG_TARCE
    _effects[stripPos]->getControls()[1]->print();  
  #endif
}

    
void LAMP::effect_setbri_incr(uint8_t stripPos, int8_t amout) {
  
  uint8_t current;
  current = _effects[stripPos]->getControls()[0]->getVal().toInt();
  if (isGlobalBrightnessOn()) current = get_globalBrightness(stripPos, current);
  int fb = (current+amout);
  current = (uint8_t)(current + amout);
  if ((amout>0) && (fb > 255) ) return;
  if ((amout<0) && (fb < 0) ) return;  
  effect_setbri(stripPos, current);

}
void LAMP::effect_setbri(uint8_t stripPos, uint8_t value) {
  boolean state = LAMPPTRGET()->_hsvLoop_1.hueChangeGet();
  _hsvLoop_1.hueChangeSet(true);  
  if (isGlobalBrightnessOn()) DevicePtrGet()->get_outputArray(stripPos)->set_briGlobal(value);
  _effects[stripPos]->getControls()[0]->setVal(String(value));
  _effects[stripPos]->worker->setbrt(value);
  #ifdef ALT_DEBUG_TARCE
    // _effects[stripPos]->getControls()[0]->print();  
  #endif
  _hsvLoop_1.hueChangeSet(state);
}

void LAMP::effect_setscale(uint8_t stripPos, uint8_t value) {
  uint8_t min = _effects[stripPos]->getControls()[2]->getMin().toInt();
  uint8_t max = _effects[stripPos]->getControls()[2]->getMax().toInt();
  if (value>max) value = map(value, 0, 255, min, max);
  // uint8_t mValue = map(value, 0, 255, min, max);
  // Serial.printf_P(PSTR("min: %d - max: %d - get: %d - set: %d\n"), min, max, value, mValue);
  _effects[stripPos]->getControls()[2]->setVal(String(value));
  _effects[stripPos]->worker->setscl(value);
  #ifdef ALT_DEBUG_TARCE
    // _effects[stripPos]->getControls()[2]->print();  
  #endif
}

void LAMP::effect_id(uint8_t stripPos, uint8_t id, uint8_t value) {
  // Serial.printf_P(PSTR("id: %d - value: %d\n"), id, value);
  LList<UIControl*>&controls = get_effect(stripPos)->getControls();
  if      (id == 2) {effect_setscale(stripPos, value);} // 2                        
  else if (id == 0) {effect_setbri(stripPos,   value);} // 0                          
  else if (id == 1) {effect_setspd(stripPos,   value);} // 1   
  // else  {
    for(int i = 0; i < controls.size(); ++i) {
      if (controls[i]->getId() == id) {
        controls[i]->setVal(String(value));
        get_effect(stripPos)->worker->setDynCtrl(controls[i]);   
      }
    }    
  // }   

  // #ifdef ALT_DEBUG_TARCE
  //   _effects[stripPos]->getControls()[id]->print();  
  // #endif
}



void LAMP::frameShow(const uint32_t ticktime){
  // if ( !fader && !isLampOn() && !isAlarm() ) return;
  int32_t delay;

  for(uint8_t i=0; i<_outPutCount; i++){ 
    
    leds_managePtrGet()->showLeds(i, _DeviceGet->get_outputArray(i)->get_bri());
    // FastLED.show();
    
    delay = (ticktime + EFFECTS_RUN_TIMER) - millis();
    if (delay < LED_SHOW_DELAY || !(_effects[i]->worker ? _effects[i]->worker->status() : 1)) 
      delay = LED_SHOW_DELAY;
    effectsTimer(i, T_ENABLE, delay);
  }
  
}
void LAMP::effectsTick(){
  
  uint32_t _begin =  millis();
  for(uint8_t i=0; i<_outPutCount; i++){
    // CHECK OUTPUT STAT (SELECTED||EFFECT)
    // ??????????????

    if (_DeviceGet->get_outputArray(i)->get_eff_activ()) {
      if (_DeviceGet->get_outputArray(i)->get_onoff() && _effects[i]->worker) {
        _effects[i]->worker->run();
      }
    }
    else _DeviceGet->get_outputArray(i)->set_color();

    if (!isLampShow) {
      effectsTimer(i, T_FRAME_ENABLE, _begin);
      // _effectsTicker.once_ms(LED_SHOW_DELAY, std::bind(&LAMP::frameShow, this, _begin));
    } else if(isLampOn()) {
      effectsTimer(i, T_ENABLE); 
    }  

  }
}

void LAMP::fader(uint8_t sPos, const uint8_t _tgtbrt, std::function<void(void)> callback){
  uint8_t bri =  _DeviceGet->get_outputArray(sPos)->get_bri();
  if (_steps) {   
    bri -= 2;  
    _DeviceGet->get_outputArray(sPos)->set_bri(bri);
  } else {
    bri += 2; 
    _DeviceGet->get_outputArray(sPos)->set_bri(bri);
  }

  if (_steps && _DeviceGet->get_outputArray(sPos)->get_bri() <= _tgtbrt)  {

    _steps = false;

    leds_managePtrGet()->showLeds(sPos, 0);

    _DeviceGet->get_outputArray(sPos)->clear();

    callback();

    RemoteControlPtrGet()->send_appi(true, true, true, 2);

    return;
  }

  if (!_steps && _DeviceGet->get_outputArray(sPos)->get_bri() >= _DeviceGet->get_outputArray(sPos)->get_briOld()) {
    _task_fadeTicker->set_enabled(false);
  }
  // leds_managePtrGet()->showLeds(sPos, _DeviceGet->get_outputArray(sPos)->get_bri());
}   
void LAMP::fadelight(uint8_t sPos, const uint8_t _targetbrightness, const uint32_t _duration, std::function<void(void)> callback) {
  _steps = true;
  _task_fadeTicker->set_enabled(false); 
  _task_fadeTicker->set_callback(std::bind(&LAMP::fader, this, sPos, 0, callback));
  _task_fadeTicker->set_callbackOend(callback);
  _task_fadeTicker->set_iteration_max(-1);
  _task_fadeTicker->set_taskDelay(ETD::ETD_TIMER, true, 50, 1);
  _task_fadeTicker->setup(true); 
  _task_fadeTicker->set_enabled(true); 
}


void LAMP::effectsTimer(uint8_t sP, SCHEDULER action, uint32_t time) {
//  LOG.printf_P(PSTR("effectsTimer: %u\n"), action);
  switch (action)
  {
  case SCHEDULER::T_DISABLE :
    _DeviceGet->get_task(sP)->set_enabled(false); 
    break;
  case SCHEDULER::T_ENABLE :
    _DeviceGet->get_task(sP)->set_callbackOstart([&](){effectsTick();});
    _DeviceGet->get_task(sP)->set_iteration_max(0);
    _DeviceGet->get_task(sP)->set_taskDelay(ETD::ETD_DELAY, true, time?time:EFFECTS_RUN_TIMER, 1);
    _DeviceGet->get_task(sP)->set_taskDelayEnabled(ETD::ETD_DELAY, true);
    _DeviceGet->get_task(sP)->set_enabled(true); 
    break;
  case SCHEDULER::T_RESET :
  case SCHEDULER::T_FRAME_ENABLE :
    _DeviceGet->get_task(sP)->set_callbackOstart([&](){frameShow(time);});
    _DeviceGet->get_task(sP)->set_iteration_max(0);
    _DeviceGet->get_task(sP)->set_taskDelay(ETD::ETD_DELAY, true, LED_SHOW_DELAY, 1);
    _DeviceGet->get_task(sP)->set_taskDelayEnabled(ETD::ETD_DELAY, true);
    _DeviceGet->get_task(sP)->set_enabled(true); 
    break;
  default:
    return;
  }
}


/*
 * включает/выключает таймер обработки эффектов
 * @param SCHEDULER enable/disable/reset - вкл/выкл/сброс
 */
/*
void LAMP::effectsTimer(uint8_t sP, SCHEDULER action, uint32_t time) {
  //  LOG.printf_P(PSTR("effectsTimer: %u\n"), action);
  switch (action) {
  case SCHEDULER::T_DISABLE :
    _effectsTicker.detach();
  break;
  case SCHEDULER::T_ENABLE :
    #ifdef ESP8266
      _effectsTicker.once_ms_scheduled(EFFECTS_RUN_TIMER, std::bind(&LAMP::effectsTick, this));
    #elif defined ESP32
      _effectsTicker.once_ms(EFFECTS_RUN_TIMER, std::bind(&LAMP::effectsTick, this));
    #endif
  break;
  case SCHEDULER::T_RESET :
  case SCHEDULER::T_FRAME_ENABLE :
    _effectsTicker.once_ms_scheduled(LED_SHOW_DELAY, std::bind(&LAMP::frameShow, this, time));
    break;  
  // case SCHEDULER::T_RESET :
  //   if (_effectsTicker.active() ) effectsTimer(sP, T_ENABLE, time);
  break;
  default:
    return;
  }
} 
*/

void LAMP::changePower(uint8_t sP) {changePower(sP, !_flags.ONflag);}
void LAMP::changePower(uint8_t sP, bool flag) {
  if (flag == _flags.ONflag) return;  // пропускаем холостые вызовы
  ALT_TRACEC("main", "Lamp powering: %s\n", flag ? "On": "Off"); 
  _flags.ONflag = flag;

  if(_mode == LAMPMODE::MODE_OTA)
    _mode = LAMPMODE::MODE_NORMAL;

  if (flag){
    effectsTimer(sP, SCHEDULER::T_ENABLE);
    // if(_mode == LAMPMODE::MODE_DEMO)
      // demoTimer(T_ENABLE);
  } else  {
    // brightness(0);
    effectsTimer(sP, SCHEDULER::T_DISABLE);
    // _lampState.isOffAfterText = false;
    // _lampState.isStringPrinting = false;
    // demoTimer(T_DISABLE);     // гасим Демо-таймер
  }
}

void LAMP::effectChangeByPos(uint8_t stripPos, uint16_t pos, boolean fade, boolean effSav, boolean skip){
  if (!isLampOn()) return;
  if (_effects[stripPos]  == nullptr) return;
  if (fade && !skip && _effects[stripPos]->worker) {
    fadelight(stripPos, FADE_MINCHANGEBRT, FADE_TIME, std::bind(&LAMP::effectChangeByPos, this, stripPos, pos, fade, true, true));
    return;
  }
  if (!fade) _DeviceGet->get_outputArray(stripPos)->clear();
  // Serial.printf_P(PSTR("BRI: %d"), _DeviceGet->get_outputArray(sPos)->get_bri());
  char bufferFolder[80];
  getStripFolder(stripPos, bufferFolder);
  _effects[stripPos]->workerset(stripPos, pos, true, true, bufferFolder, effSav ? ((isRandDemoOn()) ? false : true ) : effSav);
}
void LAMP::effectSav(uint8_t stripPos){
  if (!isLampOn()) return;
  if (_effects[stripPos]  == nullptr) return;

  char bufferFolder[80];
  getStripFolder(stripPos, bufferFolder);
  _effects[stripPos]->saveeffconfig(_effects[stripPos]->getEn(), (char*)bufferFolder);
}
void LAMP::effectSetDefault(uint8_t stripPos){
  if (!isLampOn()) return;
  if (_effects[stripPos]  == nullptr) return;

  char bufferFolder[80];
  getStripFolder(stripPos, bufferFolder);

  // String filename = _effects[stripPos]->geteffectpathname(_effects[stripPos]->getEn(), (char*)bufferFolder);

  _effects[stripPos]->savedefaulteffconfig(_effects[stripPos]->getEn(), (char*)bufferFolder);

  _effects[stripPos]->workerset(
    stripPos, 
    _effects[stripPos]->getEn(), 
    true, true, bufferFolder, false);

}
uint8_t LAMP::get_globalBrightness(uint8_t sP, uint8_t v)  {
  return isGlobalBrightnessOn() ? DevicePtrGet()->get_outputArray(sP)->get_briEff() : v;
}


/*
void LAMP::effectsTimer(uint8_t sP, SCHEDULER action, uint32_t _begin) {
//  LOG.printf_P(PSTR("effectsTimer: %u\n"), action);
  switch (action)
  {
  case SCHEDULER::T_DISABLE :
    if(_effectsTask[sP]){
      _effectsTask[sP]->cancel();
      // embui.taskRecycle(effectsTask);
      _effectsTask[sP] = nullptr;
    }
    break;
  case SCHEDULER::T_ENABLE :
    if(!_effectsTask[sP]){
      _effectsTask[sP] = new Task(_begin?_begin:EFFECTS_RUN_TIMER, TASK_ONCE, [this](){effectsTick();}, &ts, false);
    } else {
      _effectsTask[sP]->set(_begin?_begin:EFFECTS_RUN_TIMER, TASK_ONCE, [this](){effectsTick();});
    }
    _effectsTask[sP]->restartDelayed();
    break;
  case SCHEDULER::T_FRAME_ENABLE :
    if(!_effectsTask[sP]){
      _effectsTask[sP] = new Task(LED_SHOW_DELAY, TASK_ONCE, [this, _begin](){frameShow(_begin);}, &ts, false);
    } else {
      _effectsTask[sP]->set(LED_SHOW_DELAY, TASK_ONCE, [this, _begin](){frameShow(_begin);});
    }
    _effectsTask[sP]->restartDelayed();
    break;
  case SCHEDULER::T_RESET :
    if (_effectsTask[sP])
      _effectsTask[sP]->restartDelayed();
    break;
  default:
    return;
  }
}
*/

/*
void LAMP::frameShow(const uint32_t ticktime){
  for(int i=0; i<_outPutCount; i++){

      // CHECK OUTPUT STAT (SELECTED||EFFECT)
      // ??????????????
      leds_managePtrGet()->showLeds(i, _DeviceGet->get_outputArray(i)->get_bri());
  }

  if (!_effectsTicker.active() ) return;

  int32_t delay = (ticktime + EFFECTS_RUN_TIMER) - millis();
  if (delay < LED_SHOW_DELAY) delay = LED_SHOW_DELAY;
    _effectsTicker.once_ms_scheduled(delay, std::bind(&LAMP::effectsTick, this));
} 

void LAMP::effectsTick(){
  _begin = millis();
  if (_effectsTicker.active() ) {
    for(int i=0; i<_outPutCount; i++){
      // CHECK OUTPUT STAT (SELECTED||EFFECT)
      // ??????????????
      if (_DeviceGet->get_outputArray(i)->get_eff_activ()) {
        if (_DeviceGet->get_outputArray(i)->get_onoff()) _effects[i]->worker->run();
      }
      else _DeviceGet->get_outputArray(i)->set_color();
    }
  }
  if (!isLampShow) {
    _effectsTicker.once_ms(LED_SHOW_DELAY, std::bind(&LAMP::frameShow, this, _begin));
  } else if(isLampOn) {
    _effectsTicker.once_ms_scheduled(EFFECTS_RUN_TIMER, std::bind(&LAMP::effectsTick, this));  
  }  
}

void LAMP::effectsTimer(SCHEDULER action) {
  //  LOG.printf_P(PSTR("effectsTimer: %u\n"), action);
  switch (action) {
  case SCHEDULER::T_DISABLE :
    _effectsTicker.detach();
  break;
  case SCHEDULER::T_ENABLE :
    #ifdef ESP8266
      _effectsTicker.once_ms_scheduled(EFFECTS_RUN_TIMER, std::bind(&LAMP::effectsTick, this));
    #elif defined ESP32
      _effectsTicker.once_ms(EFFECTS_RUN_TIMER, std::bind(&LAMP::effectsTick, this));
    #endif
  break;
  case SCHEDULER::T_RESET :
    if (_effectsTicker.active() ) effectsTimer(T_ENABLE);
  break;
  default:
    return;
  }
}

*/