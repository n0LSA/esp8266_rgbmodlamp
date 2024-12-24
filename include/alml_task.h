#ifndef _ALML_TASK_H
#define _ALML_TASK_H

#include <Arduino.h>
#include <functional>
#include "libextern.h"

typedef std::function<void()> TaskSetFunction_t;
typedef std::function<void(uint8_t tp, uint32_t d, TaskSetFunction_t f)> TaskSetFunctionList_t;


// #ifndef _TASK_MICRO_RES

// #define TASK_MILLISECOND       1UL
// #define TASK_SECOND         1000UL
// #define TASK_MINUTE        60000UL
// #define TASK_HOUR        3600000UL

// #else

// #define TASK_MILLISECOND    1000UL
// #define TASK_SECOND      1000000UL
// #define TASK_MINUTE     60000000UL
// #define TASK_HOUR     3600000000UL

// #endif  // _TASK_MICRO_RES

void _timeStamp(uint32_t timestamp, char * time);

/**
 * type d'edittion du timer voulue
 */
typedef enum : uint8_t {
  ETD_TIMER=0,  /**< timer infinie */
  ETD_DELAY,    /**< compte a rebour avant declanchement */
  ETD_TIMEREND  /**< timer de fin */
} ETD;          

/**
 * mode d'execution callback quand la tache est oneshot
 */
typedef enum : uint8_t {
  ETO_S=0,    /**< cb_os */
  ETO_C,      /**< cb */
  ETO_SC,     /**< cb_os + cb */
  ETO_SCE,    /**< cb_os + cb + cb_e */
  ETO_SE,     /**< cb_os + cb_e */
  ETO_CE      /**< cb + _cb_e */
} ETO; 


class TaskDelay
{
  unsigned long _last =      0;
  uint32_t      _delay      = 0;
  boolean       _enabled    = false; 

public:

  TaskDelay();
  TaskDelay(const uint32_t & max);

  ~TaskDelay();

  boolean execute();  
  // void set_terminate(boolean);

  void set_enabled(boolean);
  void set_delay(const uint32_t &);

  void get_delay(uint32_t &);
  boolean isEnabled();

  void reset();
};

class TaskSimple
{
    typedef std::function<void()> TaskCallback;

  ETO _osMode = ETO_S;

  uint8_t _ID = 0;
  String  _name = "";

  uint8_t   _iteration_cnt      = 0;
  int       _iteration_max      = 0;
  boolean   _iteration_next_greater = true;

  boolean   _enabled  = false;
  boolean   _debug    = true;

  TaskCallback _callbackOstart  = nullptr;
  TaskCallback _callbackOend    = nullptr;
  TaskCallback _callback        = nullptr;

  boolean iteration_next();

  void setup_timer(const String & v1, boolean v2 = false);

  boolean execute(TaskDelay * ptr);

public:
  TaskDelay * _delay    = nullptr;
  TaskDelay * _timer    = nullptr;
  TaskDelay * _timerEnd = nullptr;

  TaskSimple();
  ~TaskSimple();

  boolean isEnabled();

  void set_enabled(boolean);
  void set_debug(boolean);

  void set_name(const String &);

  void setup(boolean v1 = false);

  void execute();
  
  void set_callbackOstart(TaskCallback v1);
  void set_callbackOend(TaskCallback);
  void set_callback(TaskCallback);
  void set_iteration_max(const int&);
  void set_oneShot(ETO);
  void set_iteration_next_greater(boolean);

  void set_taskDelay(TaskDelay *& ptr, boolean v1 = true, const uint32_t & v2 = 0, uint8_t v3 = 0);
  void set_taskDelay(ETD mod, boolean v1 = true, const uint32_t & v2 = 0, uint8_t v3 = 0);
  void set_taskDelayEnabled(TaskDelay *& ptr, boolean v1 = true);
  void set_taskDelayEnabled(ETD mod, boolean v1 = true);

  uint8_t get_id(){return _ID;}
  String get_name() const {return _name;}
};

/*
class al_taskScheduler {
public:
  TaskSimple * _TaskArray = nullptr;
  uint8_t _TaskArrayMax = 0;
  uint8_t _TaskArrayCnt = 0;
  al_taskScheduler(uint8_t v1);
  ~al_taskScheduler();

  void loop();
  void setup(boolean v1 = true);

  TaskSimple* get_task(uint8_t);
};
*/

typedef enum : uint8_t {
  TASKNAME_OUTPUT =0, 
  TASKNAME_IR,   
  TASKNAME_UDP,   
  TASKNAME_LAMP,   
  TASKNAME_WIFI,   
  TASKNAME_WEBSERVER,   
  TASKNAME_SERIAL,
  TASKNAME_PROGRAM,
  TASKNAME_NONE 
} TASKNAME_t;          
typedef enum : uint8_t {
  TASKLEVEL_ALLWAYS =0, 
  TASKLEVEL_SET,   
  TASKLEVEL_NONE 
} TASKLEVEL_t;  

class alml_taskItem
{
    typedef std::function<void()> TaskCallback_t;
    TASKNAME_t _name = TASKNAME_NONE;
    TASKLEVEL_t _lvl = TASKLEVEL_NONE;  
public:
    TaskSetFunctionList_t _cbSetup = nullptr;
    TaskCallback_t _cb = nullptr;
    uint32_t _duration = 0;    


  alml_taskItem(TaskCallback_t cb, uint32_t duration);
  alml_taskItem(TaskCallback_t cb, uint32_t duration, TASKNAME_t name, TASKLEVEL_t lvl);
  alml_taskItem(TaskCallback_t cb, uint32_t duration, TASKNAME_t name, TASKLEVEL_t lvl, TaskSetFunctionList_t s);
  ~alml_taskItem(){};
  TASKNAME_t get_name() {return _name;}
  void setup(uint8_t p);
  
};



class alml_taskSheduler
{
  LList<TaskSimple *>     _list;
  LList<alml_taskItem *>  _listItem;
public:
  alml_taskSheduler(){};
  ~alml_taskSheduler(){};
  TaskSimple* get_task(uint8_t);
  LList<TaskSimple*>& get_list() { return _list; }
  LList<alml_taskItem*>& get_listItem() { return _listItem; }
  void loop();
  TaskSimple* get_taskByName(TASKNAME_t);
  void set_enabled_byTaskName(TASKNAME_t, boolean v);
  void set_debug_byTaskName(TASKNAME_t, boolean v);
  void set_name_byTaskName(TASKNAME_t, const String & v);

  // void set_new();
};


  // _taskList.add(new alml_taskItem(taskCb_ir,        50000));
  // _taskList.add(new alml_taskItem(taskCb_udp,       100000));
  // _taskList.add(new alml_taskItem(taskCb_lamp,      5000));
  // _taskList.add(new alml_taskItem(taskCb_program,   100000));
  // _taskList.add(new alml_taskItem(taskCb_wifi,      100000));
  // _taskList.add(new alml_taskItem(taskCb_webserver, 100000));
  // _taskList.add(new alml_taskItem(taskCb_serial,    100000));

#endif

/*


  void _TaskScheduler_cStart_1(){Serial.printf(">>>[_TaskScheduler_cStart_1]<<<\n");}
  void _TaskScheduler_cStart_2(){Serial.printf(">>>[_TaskScheduler_cStart_2]<<<\n");}
  void _TaskScheduler_cend_1(){
    Serial.printf(">>>[_TaskScheduler_cend_1]<<<\n");
    _TaskScheduler->get_task(1)->set_enabled(true);
    _TaskScheduler->get_task(1)->set_taskDelayEnabled(ETD::ETD_DELAY, true);
  }
  void _TaskScheduler_cend_2(){Serial.printf(">>>[_TaskScheduler_cend_2]<<<\n");}
  void _TaskScheduler_c_1(){Serial.printf(">>>[_TaskScheduler_c_1]<<<\n");}
  void _TaskScheduler_c_2(){Serial.printf(">>>[_TaskScheduler_c_2]<<<\n");}
  _TaskScheduler->get_task(0)->set_callbackOstart(_TaskScheduler_cStart_1);
  _TaskScheduler->get_task(0)->set_callbackOend(_TaskScheduler_cend_1);
  _TaskScheduler->get_task(0)->set_callback(_TaskScheduler_c_1);
  _TaskScheduler->get_task(0)->set_taskDelay(ETD::ETD_TIMER, true, 3000, 1);
  _TaskScheduler->get_task(0)->set_iteration_max(1);
  _TaskScheduler->get_task(0)->setup(true);
  _TaskScheduler->get_task(0)->set_enabled(true);

  _TaskScheduler->get_task(1)->set_callbackOstart(_TaskScheduler_cStart_2);
  _TaskScheduler->get_task(1)->set_callbackOend(_TaskScheduler_cend_2);
  _TaskScheduler->get_task(1)->set_callback(_TaskScheduler_c_2);
  _TaskScheduler->get_task(1)->set_taskDelay(ETD::ETD_TIMER, true, 3000, 1);
  _TaskScheduler->get_task(1)->set_taskDelay(ETD::ETD_DELAY, true, 10000, 1);
  _TaskScheduler->get_task(1)->set_taskDelayEnabled(ETD::ETD_TIMER, false);
  _TaskScheduler->get_task(1)->set_taskDelayEnabled(ETD::ETD_DELAY, true);
  _TaskScheduler->get_task(1)->set_iteration_max(1);
  _TaskScheduler->get_task(1)->set_enabled(false);

*/