#ifndef EFFECTCORE_H
#define EFFECTCORE_H

  #include <Arduino.h>
  #include <ArduinoJson.h>
  #ifdef FILESYSTEM 
    #if defined USE_LITTLEFS
      #if defined(ESP8266)
        #include <LittleFS.h> 
      #elif defined(ESP32)
        #include <FS.h>
        #include <LITTLEFS.h>
      #endif
    #elif defined USE_SPIFFS
      #include <FS.h>
    #endif
  #endif
  #ifdef ESP8266
    extern "C"
    {
      #include <user_interface.h>
    } 
  #endif
  #include <LinkedList.h>
  #include "../fastled/base.h"  
  #include "set.h"  


  typedef enum : uint8_t {ALWAYS=0,ISMICON,ISMICOFF,HIDE} CONTROL_CASE; // старшие 4 бита
  typedef enum : uint8_t {RANGE=0,EDIT,CHECKBOX,RANGEw0,LIST} CONTROL_TYPE; // младшие 4 бита

  typedef const TProgmemRGBPalette16 PGMPalette;


  class UIControl{
    private:
      uint8_t id;
      CONTROL_TYPE  ctype         = CONTROL_TYPE::RANGE;
      CONTROL_CASE  ctypeState    = CONTROL_CASE::ALWAYS;
      String        control_name  = "ctrl";
      String        val           = "128";
      String        min           = "1";
      String        max           = "255";
      String        step          = "1";
    public:
      UIControl(
          uint8_t _id = 0,
          CONTROL_TYPE _ctype       = CONTROL_TYPE::RANGE,
          CONTROL_CASE _ctypeState  = CONTROL_CASE::ALWAYS,
          const String _name        = "ctrl",
          const String _val         = "128",
          const String _min         = "1",
          const String _max         = "255",
          const String _step        = "1"
          ) : 
          id(_id), ctype(_ctype), ctypeState(_ctypeState),
          control_name(_name),
          val(_val), min(_min), max(_max), step(_step) {}

      const uint8_t getId()               {return id;}
      const CONTROL_TYPE getType()        {return ctype;}
      const CONTROL_CASE getTypeState()   {return ctypeState;}
      const String &getName()             {return control_name;}
      const String &getVal()              {return val;}
      const String &getMin()              {return min;}
      const String &getMax()              {return max;}
      const String &getStep()             {return step;}

      void setVal(const String &_val) {val=_val;}
      void print(boolean keyb = false);
  };





  class EffectWorker;

  /*

    CLASS PARENT EFFECT UTILISER POUR CHAQUE EFFETS
      setter/sav/getter parametres
      json parametres
      palettes

  */
  class EffectCalc : public ledsTools {
    EffectWorker        * pworker     = nullptr; 
    LAMPSTATE           * _lampstate  = nullptr;

    LList<UIControl *>  * ctrls;  

    uint8_t             _stripPos;

    String dummy;             
    bool  active          = false;  
    bool  isCtrlPallete   = false;  
    bool  isMicActive     = false;  

  protected:
    uint8_t   effect; 

    boolean   debugDynEffect  = true;

    uint8_t   palettescale;
    float     ptPallete;      
    uint8_t   palettepos      = 1;    
    uint8_t   paletteIdx      = 1;        
    uint32_t  lastrun         = 0;        
    boolean   _pTimer         = false;
    boolean   _hue_palette    = false;

    uint8_t   speed           = 127;
    uint8_t   scale           = 127;
    uint8_t   brightness      = 255;

    float speedfactor=1.0;      // коэффициент скорости эффекта

    bool isDebug()    {return _lampstate!=nullptr ? _lampstate->isDebug : false;}
    bool isRandDemo() {if (_lampstate==nullptr) Serial.println("_LAMPSTATE NULLPTR") ;return _lampstate!=nullptr ? _lampstate->isRandDemo : false;}

    bool isActive()           {return active;}
    void setActive(bool flag) {active=flag;}

    const String &getCtrlVal(int idx) {
      dummy.clear();
      if(idx<ctrls->size() && idx>=0 && idx<=2 && (*ctrls)[idx]->getId()==idx){
        return (*ctrls)[idx]->getVal();
      } else {
        for(int i = 3; i<ctrls->size(); i++){
          if((*ctrls)[i]->getId()==idx){
            if(isRandDemo()){
              dummy = random((*ctrls)[i]->getMin().toInt(),(*ctrls)[i]->getMax().toInt()+1);
              return dummy;
            } else return (*ctrls)[i]->getVal();
          }
        }
      }
      return dummy;
    }

    bool usepalettes = false;
    std::vector<PGMPalette*> palettes;         
    TProgmemRGBPalette16 const *curPalette = &RainbowColors_p;  
    virtual void palettesload();
    void scale2pallete();  

  public:
    virtual void reset() {};
    boolean get_paletteTimer() {return _pTimer;}
    virtual bool isPalEffect() {return false;}
    virtual bool isHuePalEffect() {return _hue_palette;}
    virtual uint8_t getHue_1() {return 0;}
    virtual void palettemap(std::vector<PGMPalette*> &_pals, const uint8_t _val, const uint8_t _min=1,  const uint8_t _max=255);
    void setPalette(uint8_t p);   
    void setPalette(UIControl* _val);

    virtual OUTPUT_EFFECTTYPE get_effectType() {return OUTPUT_EFFECTTYPE::OET_NONE;}

#ifdef ALS_LAMPMATRIX
    uint8_t wrapX(int8_t x){ return (x + WIDTH) % WIDTH; }
    uint8_t wrapY(int8_t y){ return (y + HEIGHT) % HEIGHT; }  
#endif
    

    void geteffJsonconfig(JsonObject & doc, uint8_t replaceBright);
    void geteffJsonconfig(JsonObject & doc, uint8_t replaceBright, int id);
    
    EffectCalc(); 
    virtual ~EffectCalc() = default;  

    virtual void load( uint8_t strip_pos );
    virtual void loadDefault();
    virtual void setupEff(uint8_t strip_pos);

    /**
    * @brief      handle de l'effect enfant
    *
    * @return     
    */    
    virtual bool run(); 

    /**
    * @brief      delay l'iteration des fonction de l'effet enfant
    *
    * @param[in]  n      duration
    * @param[in]  delay  current duration
    *
    * @return     renvoie false si suffisamment de temps ne s'est pas écoulé depuis EFFECTS_RUN_TIMER 
    */
    bool dryrun(float n=1.0, uint8_t delay = EFFECTS_RUN_TIMER);

    virtual bool status();

    /**
    * @brief      set luminositée/brillance pour l'effet
    *
    * @param[in]  _brt  The value
    */
    virtual void setbrt(const byte _brt);

    /**
    * @brief      set vitesse de l'effet
    *
    * @param[in]  _spd  The value
    */
    virtual void setspd(const byte _spd);

    virtual void setscl(const byte _scl);

    void setDynCtrl(UIControl*_val, boolean&, String&);
    virtual String setDynCtrl(UIControl*_val);
    void setDynCtrlNoSet(UIControl*_val);


    void pre_init(uint8_t _eff, EffectWorker *_pworker, LList<UIControl *> *_ctrls, LAMPSTATE* _state) {
      effect      = _eff; 
      pworker     = _pworker; 
      ctrls       = _ctrls; 
      _lampstate  = _state;
    } 

    /**
     * @brief      initialisation des parmetres avec vaelur par de fault de la class
     *
     * @param[in]  strip_pos  postion de larray leds
     * @param[in]  _eff       postion de l'effect "workerset"
     * @param      controls   passage par reference de linstance UIControl
     * @param      lampstate  The lampstate
     */
    void init( uint8_t strip_pos, uint8_t _eff, LList<UIControl*>* _controls, LAMPSTATE* _state);

  };




  class EffectWorker {
    LAMPSTATE * _lampstate = nullptr; 

    uint16_t curEff = (uint16_t)0;    
    uint16_t selEff = (uint16_t)0;     

    String effectName;      
    uint8_t version;        

    LList<UIControl*> controls; 
    // LList<UIControl*> selcontrols; 

    bool deserializeFile(DynamicJsonDocument& doc, const char* filepath);

    void clearControlsList(); 

  public:
    String originalName;    
    
    EffectWorker(){};
    EffectWorker(LAMPSTATE * lampstate);
    ~EffectWorker();

    String geteffconfig(uint16_t nb, uint8_t replaceBright = 0);    

    void geteffconfig(uint16_t nb, JsonObject & doc, uint8_t replaceBright);
    void geteffconfig(uint16_t nb, JsonObject & doc, uint8_t replaceBright, int id);

    const uint16_t  getEn()           {return curEff;}
    const String    getEs()           {return effectName;}
    const String    &getEffectName()  {return effectName;}


    void workerset(uint8_t stripPos, uint16_t effect, const bool isCfgProceed, const bool effStart, const char *folder, const bool effSav);

    std::unique_ptr<EffectCalc> worker = nullptr;  

    LList<UIControl*>&getControls() { return controls; }

    bool isSelected()               { return (curEff == selEff); }


    void savedefaulteffconfig(uint16_t nb, String filename);

    void savedefaulteffconfig(const uint16_t nb, const char *folder=NULL);

    const String geteffectpathname(const uint16_t nb, const char *folder=NULL);

    boolean saveeffconfig(uint16_t nb, char *folder=NULL);

    boolean loadeffconfig(const uint16_t nb, const char *folder=NULL);    

    void copyeffconfig(uint16_t nb, const char * srcFolder, const char * destFolder, String & path);

  };
#endif // EFFECTCORE_H