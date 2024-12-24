#include "alml_task.h"
#include <altoolslib.h>

uint8_t Task_unique_id = 0;

#define ALML_TASKMICRORES

#ifdef ALML_TASKMICRORES
  #define _ALML_TASK_TIME_FUNCTION() _ALML_task_micros()
#else
  #define _ALML_TASK_TIME_FUNCTION() _ALML_task_millis()
#endif  // ALML_TASKMICRORES

// #ifndef _TASK_EXTERNAL_TIME
  #ifdef ALML_TASKMICRORES
    static uint32_t _ALML_task_micros() {return micros();}
  #else
    static uint32_t _ALML_task_millis() {return millis();}
  #endif    
// #endif  //  _TASK_EXTERNAL_TIME

// #define DEBUG
#ifndef DEBUG
  #ifdef DEBUG_TASK
    #define DEBUG
  #endif
#endif
#ifdef DEBUG
  #define LOG(func, ...) ALT_TRACEC(func, __VA_ARGS__)
#else
  #define LOG(func, ...) ;
#endif

void _timeStamp(uint32_t timestamp, char * time){
uint32_t milliseconds   =       (timestamp / 1000) % 1000;
uint32_t seconds        = ((    (timestamp / 1000) - milliseconds)/1000)%60;
uint32_t minutes        = ((((  (timestamp / 1000) - milliseconds)/1000) - seconds)/60) %60;
uint32_t hours          = ((((( (timestamp / 1000) - milliseconds)/1000) - seconds)/60) - minutes)/60;  
  sprintf(time,"%lu:%02lu:%02lu:%02lu", (unsigned long)hours, (unsigned long)minutes , (unsigned long)seconds, (unsigned long)milliseconds);

}
/*
namespace {
  void seconds2timeD(unsigned long s, char * time) {
    int seconds    = (s /   (1000)         ) % 60   ;
    int minutes    = (s /   (1000*60)      ) % 60   ;
    int hours      = (s /   (1000*3600)    ) % 24   ;
    sprintf(time,"%02d:%02d:%02d", hours , minutes, seconds);
  }
  void on_timeD(String & result) {
    char t[100];
    seconds2timeD(millis(), t);
    result = String(t);
  } 
}
*/

TaskDelay::TaskDelay(){
  _delay      = 0;
  _last       = 0;  
  _enabled    = true; 
  // _terminate   = false;    
}
TaskDelay::TaskDelay(const uint32_t & max){
  _delay      = max;
  _last       = 0;  
  _enabled    = true; 
  // _terminate   = false;    
}
TaskDelay::~TaskDelay(){}
boolean TaskDelay::execute () {
  if (!_enabled)    return false;

  unsigned long now     = _ALML_TASK_TIME_FUNCTION();
  unsigned long delta   = (now >= _last) ? now - _last : (0xffffffff - _last) + now ;
  boolean       done    = (delta >= _delay);

  if (done) {
    _last = now;
    return true;
  }
  return false;
}
// void TaskDelay::set_terminate(boolean v1)      {_terminate = v1;}
void TaskDelay::set_enabled(boolean v1)         {_enabled = v1;}
void TaskDelay::set_delay(const uint32_t & v1)  {_delay = v1;}
void TaskDelay::get_delay(uint32_t & v1)        {v1 = _delay;}
boolean TaskDelay::isEnabled()                  {return _enabled;}

void TaskDelay::reset() {_last = _ALML_TASK_TIME_FUNCTION();}


TaskSimple::TaskSimple() {
  if (_ID==0) {
    Task_unique_id++;
    _ID = Task_unique_id;
  } 
}
TaskSimple::~TaskSimple(){}
boolean TaskSimple::execute(TaskDelay * ptr){
  return (ptr && ptr->execute());
}
boolean TaskSimple::iteration_next(){
  if (_iteration_next_greater)
    return (_iteration_cnt > _iteration_max);
  else
    return (_iteration_cnt >= _iteration_max);
}
void TaskSimple::execute(){
  if (execute(_delay)){
      delete _delay;
      _delay=nullptr;
      setup_timer("[DELAY]", true);
  }
  if (!_timer)              {return;}
  if (!_timer->isEnabled()) {return;}

  if (execute(_timerEnd)) {
    if (_debug) LOG(ALML_DEBUGREGION_TASK, "[ID: %d][_timerEnd]\n", _ID);
    if (_timer) _timer->set_enabled(false);
    _timerEnd->set_enabled(false);
    if (_callbackOend) _callbackOend();
  }

  if (execute(_timer)) {
    // if (_debug) LOG(printf_P, PSTR("[ID: %d][TaskSimple::execute] [RUNNIG][_iteration_max: %d][_iteration_cnt: %d]\n"), _ID, _iteration_max, _iteration_cnt);

    if (_iteration_max>0) {

      _iteration_cnt++;
      
      if (iteration_next()) {
        _iteration_cnt = 0;
        _timer->set_enabled(false);
        if (_callback && !_iteration_next_greater) {_callback();
          yield();
        }
        if (_callbackOend) _callbackOend();
      } else {
        if (_callback) {_callback();
          yield();
        }
      }
    } else {
      if (_callback) {
        uint32_t checkTimer = millis();
        _callback();
        yield();
        uint32_t diff = millis()-checkTimer;
        if (diff >= 10) LOG(ALML_DEBUGREGION_TASK, "[ID: %3d][NAME: %10s][%d]\n", _ID, _name.c_str(), diff);
      }
    }
  }
  // if (_debug) LOG(printf, "\n---------------\n[loop]\n---------------\n"); 
  // delay(1000); 
}
void TaskSimple::setup(boolean v1){
  if (!_delay) {
    if (_debug) LOG(ALML_DEBUGREGION_TASK, "[ID: %d]\n", _ID);
    setup_timer("[NODELAY]", v1);
  }
}
void TaskSimple::setup_timer(const String & v1, boolean v2){
  char time[12];
  _timeStamp(micros(), time);
  if (_debug) LOG(ALML_DEBUGREGION_TASK, "[ID: %d]%s[S]\n\t[_iteration_max: %d]\n\t[%s]\n", 
    _ID, v1.c_str(), _iteration_max, time);
  if (_iteration_max == 0 ) {
    if (v2){
      if (_debug) LOG(ALML_DEBUGREGION_TASK, "&c:1&s:\t[oneshot]\n");
      switch (_osMode) {
        case ETO_S:   if (_callbackOstart)  _callbackOstart();  break;
        case ETO_C:   if (_callback)        _callback();        break;
        case ETO_SC:  if (_callbackOstart)  _callbackOstart();  if (_callback)      _callback();      break;
        case ETO_SCE: if (_callbackOstart)  _callbackOstart();  if (_callback)      _callback(); if (_callbackOend) _callbackOend(); break;
        case ETO_SE:  if (_callbackOstart)  _callbackOstart();  if (_callbackOend)  _callbackOend();  break;
        case ETO_CE:  if (_callbackOstart)  _callback();        if (_callbackOend)  _callbackOend();  break;
        default: break;
      }         
      if (_timer && _timer->isEnabled()) {
        if (_debug) LOG(ALML_DEBUGREGION_TASK, "[_timer: stop]\n");
        _timer->set_enabled(false);
      }
    }
  } 
  else  {
    if (_callbackOstart)_callbackOstart();
    if (_delay) {
      if (_debug) LOG(ALML_DEBUGREGION_TASK, "&c:1&s:\t[_delay: instanced][starting: %d]\n", v2);
      _delay->set_enabled(v2);
      _delay->reset();
    } else {
      if (_debug) LOG(ALML_DEBUGREGION_TASK, "&c:1&s:\t[_delay: not instanced]\n");
    }       
    if (_timer) {
      if (_debug) LOG(ALML_DEBUGREGION_TASK, "&c:1&s:\t[_timer: instanced][starting: %d]\n", v2);
      _timer->set_enabled(v2);
      _timer->reset();
    } else {
      if (_debug) LOG(ALML_DEBUGREGION_TASK, "&c:1&s:\t[_timer: not instanced]\n");
    }
    if (_timerEnd) {
      char t[12];
      char t2[12];
      uint32_t duration;
      _timerEnd->get_delay(duration);
      _timeStamp(_ALML_TASK_TIME_FUNCTION()+(duration), t);
      _timeStamp(_ALML_TASK_TIME_FUNCTION(), t2);
      if (_debug) LOG(ALML_DEBUGREGION_TASK, "&c:1&s:\t[_timerEnd: instanced][starting: %d][%s -> %s]\n", v2, t2, t);
      _timerEnd->set_enabled(v2);
      _timerEnd->reset();
    } else {
      if (_debug) LOG(ALML_DEBUGREGION_TASK, "&c:1&s:\t[_timerEnd: not instanced]\n");
    }       
  }
  if (_debug) LOG(ALML_DEBUGREGION_TASK, "&c:1&s:\t[E]\n");
}

void TaskSimple::set_callbackOstart(TaskCallback v1){_callbackOstart = std::move(v1);}
void TaskSimple::set_callbackOend(TaskCallback v1)  {_callbackOend = std::move(v1);}
void TaskSimple::set_callback(TaskCallback v1)      {_callback = std::move(v1);}
void TaskSimple::set_iteration_max(const int &v1)   {_iteration_max = v1;}
void TaskSimple::set_oneShot(ETO v1)                {_osMode = v1;}
void TaskSimple::set_iteration_next_greater(boolean v1) {_iteration_next_greater = v1;}

void TaskSimple::set_taskDelay(TaskDelay *& ptr, boolean v1, const uint32_t &v2,  uint8_t v3) {
  if (v1){
    if (!ptr) ptr = new TaskDelay(3000);
    uint32_t sDelay = 0;
    if (v3==0) sDelay = v2;
    if (v3==1) sDelay = (v2*1000);
    if (v3==2) sDelay = (v2*1000000);
    (ptr)->set_delay(sDelay);
  } else {
    if (ptr) {
      delete ptr;
      ptr = nullptr;      
    }
  }
}
void TaskSimple::set_taskDelay(ETD mod, boolean v1, const uint32_t &v2,  uint8_t v3) {
  switch (mod) {
    case ETD_TIMER:     set_taskDelay(_timer, v1, v2, v3);    break;
    case ETD_DELAY:     set_taskDelay(_delay, v1, v2, v3);    break;
    case ETD_TIMEREND:  set_taskDelay(_timerEnd, v1, v2, v3); break;
    default: break;
  }
}
void TaskSimple::set_taskDelayEnabled(TaskDelay *& ptr, boolean v1) {
  if (ptr) {
    (ptr)->set_enabled(v1);
    if (v1) (ptr)->reset();
  }
}
void TaskSimple::set_taskDelayEnabled(ETD mod, boolean v1) {
  switch (mod) {
    case ETD_TIMER:     set_taskDelayEnabled(_timer, v1);     break;
    case ETD_DELAY:     set_taskDelayEnabled(_delay, v1);     break;
    case ETD_TIMEREND:  set_taskDelayEnabled(_timerEnd, v1);  break;
    default: break;
  }
}
void TaskSimple::set_enabled(boolean v1) {
  _enabled = v1;
}
 
boolean TaskSimple::isEnabled() {return _enabled;}

void TaskSimple::set_debug(boolean v1) {
  _debug = v1;
}

void TaskSimple::set_name(const String & v1) {
  _name = v1;
}
/*
al_taskScheduler::al_taskScheduler(uint8_t v1) {
  _TaskArrayMax = v1;
  _TaskArray = new TaskSimple[v1];
}
al_taskScheduler::~al_taskScheduler() {}
void al_taskScheduler::loop(){
  for(int i=0; i<_TaskArrayMax; i++){if (_TaskArray[i].isEnabled()) _TaskArray[i].execute(); }
}
void al_taskScheduler::setup(boolean v1){
  for(int i=0; i<_TaskArrayMax; i++){_TaskArray[i].setup(v1); } 
}
TaskSimple * al_taskScheduler::get_task(uint8_t p){return &_TaskArray[p];}
*/

TaskSimple * alml_taskSheduler::get_task(uint8_t p){return _list[p];}
TaskSimple * alml_taskSheduler::get_taskByName(TASKNAME_t p){
  for(uint8_t i=0; i<_listItem.size(); i++){
    if (_listItem[i]->get_name() == p) return _list[i];; 
  }
  return nullptr;
}
void alml_taskSheduler::set_enabled_byTaskName(TASKNAME_t p, boolean v){
  for(uint8_t i=0; i<_listItem.size(); i++){
    if (_listItem[i]->get_name() == p) _list[i]->set_enabled(v); 
  }
}
void alml_taskSheduler::set_debug_byTaskName(TASKNAME_t p, boolean v){
  for(uint8_t i=0; i<_listItem.size(); i++){
    if (_listItem[i]->get_name() == p) _list[i]->set_debug(v); 
  }
}
void alml_taskSheduler::set_name_byTaskName(TASKNAME_t p, const String & v){
  for(uint8_t i=0; i<_listItem.size(); i++){
    if (_listItem[i]->get_name() == p) _list[i]->set_name(v); 
  }
}


void alml_taskSheduler::loop(){
  for(int i=0; i<_list.size(); i++){if (_list[i]->isEnabled()) _list[i]->execute(); }
}

alml_taskItem::alml_taskItem(TaskCallback_t cb, uint32_t duration) {
  if (cb) _cb = std::move(cb);
  _duration = duration;
}
alml_taskItem::alml_taskItem(TaskCallback_t cb, uint32_t duration, TASKNAME_t name, TASKLEVEL_t lvl) {
  if (cb) _cb = std::move(cb);
  _duration = duration;
  _name = name;
  _lvl  = lvl;
}
alml_taskItem::alml_taskItem(TaskCallback_t cb, uint32_t duration, TASKNAME_t name, TASKLEVEL_t lvl, TaskSetFunctionList_t s) {
  if (cb) _cb       = std::move(cb);
  if (s)  _cbSetup  = std::move(s);
  _duration = duration;
  _name = name;
  _lvl  = lvl;
}
void alml_taskItem::setup(uint8_t p){
  if (!_cb) return;
  if (!_cbSetup) return;
  _cbSetup(p, _duration, _cb);
}

// void alml_taskSheduler::set_new(){
//   get_list().add(new TaskSimple());
// }

