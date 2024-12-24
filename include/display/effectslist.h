#ifndef EFFECTSLIST_H
#define EFFECTSLIST_H

  #include "../libextern.h"

  class Effectlist_exclude {
      typedef std::function<bool(String item)> _callback;
      DynamicJsonDocument * docArrList = nullptr; // 
    public:
      Effectlist_exclude();
      ~Effectlist_exclude() {
        if (docArrList) delete docArrList;
      }
      _callback _callBack = nullptr;
      void set_callback(_callback v1);
      DynamicJsonDocument *  getDoc() {return docArrList;}
      bool includeFoundItem(String item);
      bool exludeFoundItem(String item);
  };

  class Effectslist {
    const char**  _list    = nullptr; 
    uint8_t *     _listId  = nullptr; 
    Program *     _Program = nullptr;

  public:



    Effectslist();
    ~Effectslist(){};
    Program * get_program();

    void eff_next();
    void eff_prev();
    void eff_byId(uint8_t);
    void eff_byName(const char*);
    void toggle();
    
  };
  Effectslist * EffectslistPtrGet();

  extern LList<Effectlist_exclude *>  _includeFoundItem;  
  extern LList<Effectlist_exclude *>  _exludeFoundItem;  

  
#endif // EFFECTSLIST_H