#ifndef DEVICE_H
  #define DEVICE_H

  #include "../fastled/base.h"  
  #include "output.h" 
  #include "../alml_task.h" 

  extern StaticJsonDocument<1024> DeviceUserConfig;


class DeviceSave_item
{
  boolean _succes = false;
  uint8_t _pin = 0;
  uint8_t _operation = 0;
  char    * _request = nullptr;
public:


  DeviceSave_item(boolean, uint8_t, const char *, uint8_t operation = 0);
  ~DeviceSave_item();

  DeviceSave_item &operator = (const char * const & other);
  DeviceSave_item &operator = (uint8_t const & other);

  void    get_request(const char * & ret);
  uint8_t get_pin();
  uint8_t get_operation();
  boolean get_succes();
};


/*
  req - succes - result
  op

  -
  reset
  statu
  sav

*/
  class DeviceSave {
    LList<DeviceSave_item *> _currentList;
    LList<DeviceSave_item *> _lastList;
  public:
    DeviceSave();
    ~DeviceSave();

    DeviceSave_item * get_item(boolean, uint8_t);
    LList<DeviceSave_item*>& get_list(boolean);
    void sav();
    void new_req(boolean s, uint8_t p, const char * r, uint8_t operation = 0);
    const String get_lastRequest() ;
    uint8_t get_lastOutput();
    uint8_t get_lastOperation();
    boolean get_lastSucces();
  };

/* 
  classe device

  class principal

  de cette classe nous  initialiseront le system

  attributation des variables principal

  instance statique pour de la class leds_manage
    pour l'initialisation et settting des strip

  instance statique EffectList
    pour la gestion des effect sour forme de list

  instance statique LAMP
    pour l'annimation des leds en genral

  instance statique Logger
    pour debug

  instance statique Deviceserver
    pour la connection wifi
    serveur http, udp
    gestion en ammont des requette

  INITIALISATION DES OUTPUTS

  la classe gere de manierre genral 
  le contenu (effect, color, sauvegarde, etc..) des requette prefiltrer par Deviceserver

    deterine le type de requette
      passe le resulta a la class output si besion
      gerr directemn le changement d'effet

  generalise le paramettre d'effet (rotation des palette, couleures, etc..) pour une meilleur syncronisation au niveau du device
    par la suite recevra les infos direcetemnt d'un module/server maitre
 */

  /**
   * @class     Device
   * 
   * @brief   class pour la gestion global du programmes
   * @see         https://github.com/AdriLighting/
   * 
   * @author      Adrilighting
   * @date      mar. 28 déc. 2021 16:25:30
   */
  class Device 
  {
      // al_taskScheduler * _TaskScheduler;

      Program * _Program = nullptr;

      /** \brief current esp ip */
      IPAddress _ip = {192, 168, 0, 1};   

      /** \brief nombre total de strip */
      uint8_t _output_count = 0;          

      /** \brief nom unique servant de hostname */
      const char *  _name = "DEVICENAME";     

      /** \brief class Output  par strip */
      Output * _outputArray  = nullptr;        

      /** \brief chipset strip */
      leds_manage * _leds_manage  = nullptr;  

      uint8_t _effLoad = true;


      boolean isColorRequest(const String & cmd);
      boolean isEffectRequest(const String & cmd);
      boolean isEffectLoadRequest(const String & cmd);


      boolean parseJson_output(uint8_t p , String & nextEffect, const String & req, JsonObject val);

      unsigned long _timerOutPutsSav  = 0;

      alml_taskSheduler * get_taskSheduler();

    public:
      Device(const char * name, uint8_t oc);

      String get_lastRequest() const;
      uint8_t get_lastOutput();
      uint8_t get_lastOperation();
      boolean get_lastSucces();

      TaskSimple * get_task(uint8_t);
      

      void    get_outputCount(uint8_t & ret);
      Output  * get_outputArray(uint8_t p);

      const char * get_name();
      
      void loop();

      void parseJson_outpitListByDn(DynamicJsonDocument & doc);
      void parseJson_output(DynamicJsonDocument & doc);
      void parseJson_device(DynamicJsonDocument & doc);
      
      void output_toJson(uint8_t pos, JsonObject & oject, boolean shortKey = false, boolean fs = true, boolean eff = false);
      void outputs_toJson(JsonObject & oject, boolean shortKey = true, boolean fs = true, boolean eff = false);
      void outputs_effect_toJson(JsonObject & oject);
      void output_effect_toJson(uint8_t pos, JsonObject & oject);

      void outputs_toNode(String & out);
      void outputs_toAppi(String & out, boolean eff, boolean prog, uint8_t effLoad = 0, boolean sJson = 0);
      void output_toAppi(String & out, uint8_t pos, boolean eff, boolean prog, uint8_t effLoad, boolean sJson);      
      void outputs_effect_toAppi(String & out);
      void outputs_effect_toNode(String & out);
      boolean outputs_effect_toNode(String & out, uint8_t oP);

      void outputs_savForce();
      void outputs_sav();
      boolean outputs_requestEffect();
      boolean outputs_EffectLoadRequest();

      void output_toSpiff(uint8_t pos, const String & path = "/outputs/lamp.txt" );
      void armoiretofs(uint8_t pos);
      
      void preset_sav(DynamicJsonDocument & doc);
      void preset_load(DynamicJsonDocument & doc);


      void eff_load();

      void bp_effPrev();
      void bp_effNext();
      void bp_briLoop();
      void bp_toggle(boolean sav);
      void bp_set_tw(boolean sav);
      void bp_set_brightnessDown(boolean sav);
      void bp_set_brightnessUp(boolean sav);
      void bp_onoff(uint8_t value, boolean sav);

      void restart_requiered();
      void format(uint8_t);


  };
  Device * DevicePtrGet();


#endif // DEVICE_H