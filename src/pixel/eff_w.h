#include "../../include/pixeleffect/pixeleffect.h"
#ifdef ALS_LAMPMATRIX
  #include "../../include/pixeleffect/matrixeffect.h"  
#endif
#include "../../include/display/lamp.h"
#include "../../include/device/device.h"


EffectWorker::EffectWorker(LAMPSTATE * lampstate){
  _lampstate = lampstate;
}
EffectWorker::~EffectWorker(){
    ALT_TRACEC(ALML_DEBUGREGION_EFFECT, "\n[DELETE EffectWorker]\n");
    clearControlsList();
}
void EffectWorker::clearControlsList()
{
  ALT_TRACEC(ALML_DEBUGREGION_EFFECT, "-\n");
  while (controls.size()) {
      UIControl *ctrl = controls.shift();
      delete ctrl;
  }
  controls.clear();
  ALT_TRACEC(ALML_DEBUGREGION_EFFECT, "--\n");
}

void EffectWorker::savedefaulteffconfig(const uint16_t nb, const char *folder) {
  String filename = geteffectpathname(nb,folder);
  savedefaulteffconfig(nb, filename); 
}
boolean EffectWorker::loadeffconfig(const uint16_t nb, const char *folder) {
  if(worker==nullptr) return false;  

  boolean setDefault = false;

  String filename = geteffectpathname(nb,folder);

  #ifdef ALT_DEBUG_TARCE
  ALT_TRACEC(ALML_DEBUGREGION_EFFECT, "\n\tfilename: %s\n\tnb: %d\n\tname: %s\n", filename.c_str(), nb, effectName.c_str());
  #else
  Serial.printf_P(PSTR("EffectWorker::loadeffconfig\n\tfilename: %s\n\tnb: %d\n\tname: %s\n"), filename.c_str(), nb, effectName.c_str());
  #endif

  DynamicJsonDocument doc(2048);

  if (!deserializeFile(doc, filename.c_str() )){
    ALT_TRACEC(ALML_DEBUGREGION_EFFECT, "&c:1&s:\tERROR deserializeFile from filesystem file\n");
    doc.clear();
    FILESYSTEM.remove(filename);
    savedefaulteffconfig(nb, filename);   
    if (!deserializeFile(doc, filename.c_str() )) {
      ALT_TRACEC(ALML_DEBUGREGION_EFFECT, "&c:1&s:\tERROR deserializeFile from default file\n");
      FILESYSTEM.remove(filename); 
      return false;
    } 
    setDefault = true; 
  }

  // serializeJsonPretty(doc, Serial);Serial.println();

  curEff = doc[F("nb")].as<uint16_t>();
  const char* name = doc[F("eff_es")];
  
  ALT_TRACEC(ALML_DEBUGREGION_EFFECT, "&c:1&s:\tname from struct(%s) - id eff(%d) - JSON name(%s) - JSON id eff(%d)\n", 
    FPSTR(_effeNamIDList[(uint8_t)nb]._nameId),
    nb,
    al_tools::ch_toString(name).c_str(),
    curEff
  );

  uint8_t resetJson = 0;
  if (al_tools::ch_toString(name) != FPSTR(_effeNamIDList[(uint8_t)nb]._nameId))  resetJson = 1;
  if (curEff != nb)                                                               resetJson = (resetJson==1) ? 3 : 2;
  if (resetJson>0) {
    if (resetJson == 1) {
      #ifdef ALT_DEBUG_TARCE
        ALT_TRACEC(ALML_DEBUGREGION_EFFECT, "&c:1&s:\tERROR from JSON : name from struct(%s)') != JSON name(%s)\n", 
            FPSTR(_effeNamIDList[(uint8_t)nb]._nameId),  
            al_tools::ch_toString(name).c_str());
      #else
        Serial.printf_P(PSTR("\tERROR from JSON : name from struct(%s)') != JSON name(%s)\n"), 
            FPSTR(_effeNamIDList[(uint8_t)nb]._nameId),  
            al_tools::ch_toString(name).c_str());
      #endif
    }
    if (resetJson == 2) {
      #ifdef ALT_DEBUG_TARCE
        ALT_TRACEC(ALML_DEBUGREGION_EFFECT, "&c:1&s:\tERROR from JSON : id eff(%d) != JSON id eff(%d)\n", nb, curEff);
      #else
        Serial.printf_P(PSTR("\tERROR from JSON : id eff(%d) != JSON id eff(%d)\n"), nb, curEff);
      #endif
    }
    if (resetJson == 3) {
      #ifdef ALT_DEBUG_TARCE
        ALT_TRACEC(ALML_DEBUGREGION_EFFECT, "&c:1&s:\tERROR from JSON : name from struct(%s) && id eff(%d) != JSON name(%s) && JSON id eff(%d)\n", 
            FPSTR(_effeNamIDList[(uint8_t)nb]._nameId),
            nb,
            al_tools::ch_toString(name).c_str(),
            curEff
            );
      #else
        Serial.printf_P(PSTR("\tERROR from JSON : name from struct(%s) && id eff(%d) != JSON name(%s) && JSON id eff(%d)\n"), 
            FPSTR(_effeNamIDList[(uint8_t)nb]._nameId),
            nb,
            al_tools::ch_toString(name).c_str(),
            curEff
            );
      #endif
    }
    doc.clear();
    FILESYSTEM.remove(filename);
    savedefaulteffconfig(nb, filename);   
    if (!deserializeFile(doc, filename.c_str() )) {
      ALT_TRACEC(ALML_DEBUGREGION_EFFECT, "&c:1&s:\tERROR deserializeFile from default file\n");
      FILESYSTEM.remove(filename); 
      return false;
    } 
    setDefault = true;     
    // serializeJsonPretty(doc, Serial);Serial.println();
  }

  // effectName = name ? name : (String)(FPSTR(_effeNamIDList[(uint8_t)nb]._nameId));
  effectName = (String)(FPSTR(_effeNamIDList[(uint8_t)nb]._nameId));
  ALT_TRACEC(ALML_DEBUGREGION_EFFECT, "&c:1&s:\teffname from STRUCT: %s\n", FPSTR(_effeNamIDList[(uint8_t)nb]._nameId));
  ALT_TRACEC(ALML_DEBUGREGION_EFFECT, "&c:1&s:\teffname from JSON: %s\n", al_tools::ch_toString(name).c_str());


  JsonArray arr = doc[F("ctrls")].as<JsonArray>();
  clearControlsList();
  uint8_t id_tst = 0x0; 
  for (size_t i = 0; i < arr.size(); i++) {
      JsonObject item = arr[i];
      uint8_t id = item[F("id")].as<uint8_t>();
      if(!(id_tst&(1<<id))){ 
          id_tst |= 1<<item[F("id")].as<uint8_t>();
          String name = item.containsKey(F("name")) ?
              item[F("name")].as<String>()
              : id == 0 ? String(FPSTR(ALMLPT_LEDS_005))
              : id == 1 ? String(FPSTR(ALMLPT_LEDS_006))
              : id == 2 ? String(FPSTR(ALMLPT_LEDS_007))
              : String(F("NNID"))+String(id);
          String val = item.containsKey(F("val")) ? item[F("val")].as<String>() : String(1);
          String min = item.containsKey(F("min")) && id>1 ? item[F("min")].as<String>() : String(1);
          String max = item.containsKey(F("max")) && id>1 ? item[F("max")].as<String>() : String(255);
          String step = item.containsKey(F("step")) && id>2 ?  item[F("step")].as<String>() : String(1);
          CONTROL_TYPE type = item.containsKey(F("type")) && (id>2 || (id<=2 && (item[F("type")].as<CONTROL_TYPE>())==CONTROL_TYPE::RANGE)) ? item[F("type")].as<CONTROL_TYPE>() : CONTROL_TYPE::RANGE;
          CONTROL_CASE typeState = item.containsKey(F("state")) ? item[F("state")].as<CONTROL_CASE>() : CONTROL_CASE::ALWAYS;
          // ALT_TRACEC(ALML_DEBUGREGION_EFFECT, "&c:1&s:\t\t%3d - %s: %d  %s: %d - %s: %d - %s: %s - %s: %s - %s: %s - %s: %s - %s: %s\n", 
          //   i, 
          //   "id", id , 
          //   "type", type , 
          //   "typeState", typeState , 
          //   "name", name.c_str() , 
          //   "val", val.c_str() ,
          //   "min", min.c_str() , 
          //   "max", max.c_str() , 
          //   "step", step.c_str() 
          //   );
          controls.add(new UIControl(
              id,             // id
              type,           // type
              typeState,      // type
              name,           // name
              val,            // value
              min,            // min
              max,            // max
              step            // step
          ));
      }
  }
  doc.clear();
  
  for(int8_t id=0;id<3;id++){
      if(!((id_tst>>id)&1)){ 
          controls.add(new UIControl(
              id,                                     // id
              CONTROL_TYPE::RANGE,                    // type
              CONTROL_CASE::ALWAYS,                   // typeState
              id==0 ? FPSTR(ALMLPT_LEDS_005) : id==1 ? FPSTR(ALMLPT_LEDS_006) : FPSTR(ALMLPT_LEDS_007),           // name
              String(127),                            // value
              String(1),                              // min
              String(255),                            // max
              String(1)                               // step
          ));
      }
  }
  controls.sort([](UIControl *&a, UIControl *&b){ return a->getId() - b->getId();}); 

  if (setDefault) {
    
    int defSpd = _effeNamIDList[(uint8_t)nb]._spd;
    if (defSpd > 0) {
      ALT_TRACEC(ALML_DEBUGREGION_EFFECT, "&c:1&s:\tspd from struct: %d\n", _effeNamIDList[(uint8_t)nb]._spd);
      controls[1]->setVal(String(defSpd));
      worker->setspd(defSpd);
    }
  }
  

  return true; 
}

String EffectWorker::geteffconfig(uint16_t nb, uint8_t replaceBright)
{
    DynamicJsonDocument doc(3200);
    doc[F("nb")]      = nb;
    doc[F("eff_en")]  = curEff;
    doc[F("eff_es")]  = effectName;
    JsonArray arr = doc.createNestedArray(F("ctrls"));
    for (int i = 0; i < controls.size(); i++)
    {
        JsonObject var = arr.createNestedObject();
        var[F("id")]      = controls[i]->getId();
        var[F("type")]    = controls[i]->getType();
        var[F("state")]   = controls[i]->getTypeState();
        var[F("name")]    = controls[i]->getName();
        var[F("val")]     = (controls[i]->getId()==0 && replaceBright) ? String(replaceBright) : controls[i]->getVal();
        var[F("min")]     = controls[i]->getMin();
        var[F("max")]     = controls[i]->getMax();
        var[F("step")]    = controls[i]->getStep();
    }
    String cfg_str;
    serializeJson(doc, cfg_str);
    doc.clear();
    return cfg_str;
}

void EffectWorker::geteffconfig(uint16_t nb, JsonObject & doc, uint8_t replaceBright) {
    doc[F("nb")]      = nb;
    doc[F("eff_en")]  = curEff;
    doc[F("eff_es")]  = effectName;
    worker->geteffJsonconfig(doc, replaceBright);
}

void EffectWorker::geteffconfig(uint16_t nb, JsonObject & doc, uint8_t replaceBright, int id) {
    worker->geteffJsonconfig(doc, replaceBright, id);
}

boolean EffectWorker::saveeffconfig(uint16_t nb, char *folder){
  if(millis()<10000) {return false;} 

  ALT_TRACEC(ALML_DEBUGREGION_EFFECT, "-\n");

  #ifdef ALT_DEBUG_TARCE
  uint32_t mS = millis();  
  #endif

  File configFile;
  String filename = geteffectpathname(nb, folder);

  ALT_TRACEC(ALML_DEBUGREGION_EFFECT, "&c:1&s:\teffectName : %s\n",         effectName.c_str());
  ALT_TRACEC(ALML_DEBUGREGION_EFFECT, "&c:1&s:\tgeteffectpathname : %s\n",  filename.c_str());

  configFile = FILESYSTEM.open(filename, "w"); 
  configFile.print(geteffconfig(nb));
  configFile.close();

  #ifdef ALT_DEBUG_TARCE
  uint32_t mSdiff = millis()-mS;  
  #endif
  ALT_TRACEC(ALML_DEBUGREGION_EFFECT, "&c:1&s:\ttook: %u ms\n",mSdiff);
  ALT_TRACEC(ALML_DEBUGREGION_EFFECT, "--\n");
  return true;
}

void EffectWorker::copyeffconfig(uint16_t nb, const char *srcFolder, const char *destFolder, String & path){

    String src      = geteffectpathname(nb, srcFolder);
    String filename = geteffectpathname(nb, destFolder);

    File desFile;  
    File srcFile;  

    if (FILESYSTEM.exists(filename) == true) FILESYSTEM.remove(filename);

    srcFile = FILESYSTEM.open(src, "r"); 
    if (!srcFile) return;
    desFile = FILESYSTEM.open(filename, "w"); 
    if (!desFile) return;

    #ifdef ESP8266
      char ibuffer[64];  
    #endif
    while (srcFile.available() > 0)
    {
      #if defined(ESP8266)
        byte i = srcFile.readBytes(ibuffer, 64); // i = number of bytes placed in buffer from file f1
        desFile.write(ibuffer, i);               // write i bytes from buffer to file f2  
      #elif defined(ESP32)
        // uint8_t i = srcFile.readBytes(ibuffer, 64); // i = number of bytes placed in buffer from file f1
        // desFile.write(ibuffer, srcFile.read());               // write i bytes from buffer to file f2    
        desFile.print(srcFile.read());     
      #endif

    }

    srcFile.close();
    desFile.close();
    path = filename;
}

void EffectWorker::savedefaulteffconfig(uint16_t nb, String filename){
  ALT_TRACEC(ALML_DEBUGREGION_EFFECT, "-\n");

  const String efname(FPSTR(_effeNamIDList[nb]._nameId)); 
  ALT_TRACEC(ALML_DEBUGREGION_EFFECT, "&c:1&s:\twrite default config file\n\t\t%d - %s\n\t\t%s\n", nb, efname.c_str(), filename.c_str());

  String cfg(FPSTR(FPSTR(_effeNamIDList[nb]._defaultCfg)));   
  cfg.replace(F("@name@"), efname);
  cfg.replace(F("@nb@"), String(nb));

  // #ifdef ALT_DEBUG_TARCE
    DynamicJsonDocument doc(3200);
    deserializeJson(doc, cfg);
    serializeJsonPretty(doc, Serial);Serial.println();  
  // #endif

  File configFile = FILESYSTEM.open(filename, "w"); 
  if (configFile){
      configFile.print(cfg.c_str());
      configFile.close();
      ALT_TRACEC(ALML_DEBUGREGION_EFFECT, "&c:1&s:\tSUCCESS\n");
  } else ALT_TRACEC(ALML_DEBUGREGION_EFFECT, "&c:1&s:\tFAILE writing file\n");
   ALT_TRACEC(ALML_DEBUGREGION_EFFECT, "--\n");
}

const String EffectWorker::geteffectpathname(const uint16_t nb, const char *folder){
    uint16_t swapnb = nb>>8|nb<<8; 
    String filename;
    char buffer[5];
    if (folder) {
        filename.concat(F("/"));
        filename.concat(folder);
    }
    filename.concat(F("/eff/"));
    sprintf_P(buffer,PSTR("%04x"), swapnb);
    filename.concat(buffer);
    filename.concat(F(".json"));
    return filename;
}

bool EffectWorker::deserializeFile(DynamicJsonDocument& doc, const char* filepath){
    if (!filepath || !*filepath)
        return false;

    File jfile = FILESYSTEM.open(filepath, "r");
    DeserializationError error;
    if (jfile){
      error = deserializeJson(doc, jfile);
      jfile.close();
    } else {
      Serial.print(F("EffectWorker::deserializeFile open(r) filepath: "));
      Serial.println(filepath);        
      return false;
    }

    if (error) {
      Serial.print(F("EffectWorker::deserializeFile open(r), deserializeJson error: "));
      Serial.print(error.c_str());    
      Serial.print(", filepath: ");
      Serial.println(filepath);  
      return false;
    }
    return true;
}

void EffectWorker::workerset(uint8_t stripPos, uint16_t effect, const bool isCfgProceed, const bool effStart, const char *folder, const bool effSav){

  #ifndef ALT_DEBUG_TARCE
    String time; 
    String heap;
    al_tools::on_time_d(time);
    HeapStatuPtrGet()->update();
    HeapStatuPtrGet()->print(heap);       
    Serial.printf_P(PSTR("%12s - %s\n"), time.c_str(), heap.c_str());  
  #endif

  ALT_TRACEC(ALML_DEBUGREGION_EFFECT, "-\n");

  String folderStr = al_tools::ch_toString(folder);
  ALT_TRACEC(ALML_DEBUGREGION_EFFECT, "&c:1&s:\tLOAD EFFECT\n\t\teffect nb: %d\n\t\tisCfgProceed: %d\n\t\teffStart: %d\n\t\tfolder: %s\n\t\teffSav: %d\n", 
     effect, isCfgProceed, effStart, folderStr.c_str(), effSav);

  if (worker) {

    ALT_TRACEC(ALML_DEBUGREGION_EFFECT, "&c:1&s:\t\tOLD EFFECT\n\t\t\teffect nb: %d\n\t\t\teffect name: %s\n", curEff, effectName.c_str());

    if(isCfgProceed && effSav) saveeffconfig(curEff, (char *)folder);

    worker.reset();
    yield();

  }

  uint8_t cat = atoi(_effeNamIDList[(uint8_t)effect]._cat);
  switch (cat) {
    case 1: worker = std::unique_ptr<EffectFx_mode> (new EffectFx_mode(effect)); break;
    case 2:
      switch (effect) {
        case 26   : worker = std::unique_ptr<EffectFx_comets>                         (new EffectFx_comets                      (effect)           ); break;
        case 27   : worker = std::unique_ptr<EffectFx_Comet>                          (new EffectFx_Comet                       (effect)           ); break;
        case 28   : worker = std::unique_ptr<EffectFx_ruggle>                         (new EffectFx_ruggle                      (effect)           ); break;
        case 29   : worker = std::unique_ptr<EffectFx_rainbowWave>                    (new EffectFx_rainbowWave                 (effect)           ); break;
        case 30   : worker = std::unique_ptr<EffectFx_ArmoireOutsideToInside>         (new EffectFx_ArmoireOutsideToInside      (effect)           ); break;
        case 31   : worker = std::unique_ptr<EffectFx_fire2014>                       (new EffectFx_fire2014                    (effect)           ); break;
        case 32   : worker = std::unique_ptr<EffectFx_aurora>                         (new EffectFx_aurora                      (effect)           ); break;
        #ifdef ALS_LAMPMATRIX
        case 33   : worker = std::unique_ptr<MEffectSparcles>                         (new MEffectSparcles                      (effect)           ); break;
        case 34   : worker = std::unique_ptr<MEffectWhiteColorStripe>                 (new MEffectWhiteColorStripe              (effect)           ); break;
        case 35   : worker = std::unique_ptr<MEffectEverythingFall>                   (new MEffectEverythingFall                (effect)           ); break;
        case 36   : worker = std::unique_ptr<MEffectPulse>                            (new MEffectPulse                         (effect)           ); break;
        case 37   : worker = std::unique_ptr<MEffectRainbow>                          (new MEffectRainbow                       (effect)           ); break;
        case 38   : worker = std::unique_ptr<MEffectMatrix>                           (new MEffectMatrix                        (effect)           ); break;
        case 39   : worker = std::unique_ptr<MEffectStarFall>                         (new MEffectStarFall                      (effect)           ); break;
        case 40   : worker = std::unique_ptr<MEffectLighterTracers>                   (new MEffectLighterTracers                (effect)           ); break;
        case 41   : worker = std::unique_ptr<MEffectLightBalls>                       (new MEffectLightBalls                    (effect)           ); break;
        case 42   : worker = std::unique_ptr<MEffectBall>                             (new MEffectBall                          (effect)           ); break;
        case 43   : worker = std::unique_ptr<MEffect3DNoise>                          (new MEffect3DNoise                       (effect)           ); break;
        case 44   : worker = std::unique_ptr<MEffectBBalls>                           (new MEffectBBalls                        (effect)           ); break;
        case 45   : worker = std::unique_ptr<MEffectSinusoid3>                        (new MEffectSinusoid3                     (effect)           ); break;
        case 46   : worker = std::unique_ptr<MEffectMetaBalls>                        (new MEffectMetaBalls                     (effect)           ); break;
        case 47   : worker = std::unique_ptr<MEffectSpiro>                            (new MEffectSpiro                         (effect)           ); break;
        case 48   : worker = std::unique_ptr<MEffectFlower>                           (new MEffectFlower                        (effect)           ); break;
        case 49   : worker = std::unique_ptr<MEffectSplashBals>                       (new MEffectSplashBals                    (effect)           ); break;
        case 50   : worker = std::unique_ptr<MEffectRadialFire>                       (new MEffectRadialFire                    (effect)           ); break;
        case 51   : worker = std::unique_ptr<MEffectRadialNuclearNoise>               (new MEffectRadialNuclearNoise            (effect)           ); break;
        case 52   : worker = std::unique_ptr<MEffectWcolor>                           (new MEffectWcolor                        (effect)           ); break;
        case 53   : worker = std::unique_ptr<MEffectMirage>                           (new MEffectMirage                        (effect)           ); break;
        case 54   : worker = std::unique_ptr<MEffectDNA>                              (new MEffectDNA                           (effect)           ); break;
        case 55   : worker = std::unique_ptr<MEffectPile>                             (new MEffectPile                          (effect)           ); break;
        case 56   : worker = std::unique_ptr<MEffectFire2021>                         (new MEffectFire2021                      (effect)           ); break;
        case 57   : worker = std::unique_ptr<MEffectFlags>                            (new MEffectFlags                         (effect)           ); break;
        case 58   : worker = std::unique_ptr<MEffectStarShips>                        (new MEffectStarShips                     (effect)           ); break;
        case 59   : worker = std::unique_ptr<MEffect_Magma>                           (new MEffect_Magma                        (effect)           ); break;
        case 60   : worker = std::unique_ptr<MEffectPolarL>                           (new MEffectPolarL                        (effect)           ); break;
        case 61   : worker = std::unique_ptr<MEffectRacer>                            (new MEffectRacer                         (effect)           ); break;
        case 62   : worker = std::unique_ptr<MEffect_Frizzles>                        (new MEffect_Frizzles                     (effect)           ); break;
        case 63   : worker = std::unique_ptr<MEffectMaze>                             (new MEffectMaze                          (effect)           ); break;
        case 64   : worker = std::unique_ptr<MEffectBalls>                            (new MEffectBalls                         (effect)           ); break;
        case 65   : worker = std::unique_ptr<MEffectCircles>                          (new MEffectCircles                       (effect)           ); break;
        case 66   : worker = std::unique_ptr<MEffectFairy>                            (new MEffectFairy                         (effect)           ); break;
        case 67   : worker = std::unique_ptr<MEffectBengalL>                          (new MEffectBengalL                       (effect)           ); break;
        case 68   : worker = std::unique_ptr<MEffectWrain>                            (new MEffectWrain                         (effect)           ); break;
        case 69   : worker = std::unique_ptr<MEffectOscilator>                        (new MEffectOscilator                     (effect)           ); break;
        // case 48   : worker = std::unique_ptr<MEffectPuzzles>                          (new MEffectPuzzles                        (effect)           ); break;
        
        #endif       
        default   : worker = std::unique_ptr<EffectFx_comets>                         (new EffectFx_comets                      (19)           ); break;
        }      
      break;
    default: break;
  }

  if(worker){
    worker->pre_init((uint8_t)effect, this, &(getControls()), _lampstate);
    originalName = effectName = FPSTR(_effeNamIDList[(uint8_t)effect]._nameId);
    if(isCfgProceed){ 
      ALT_TRACEC(ALML_DEBUGREGION_EFFECT, "&c:1&s:\t\tNEW EFFECT\n\t\t\teffect name: %s - strip: %d\n", originalName.c_str(), stripPos);
      
      LAMPPTRGET()->_hsvLoop_1.set_hueRnd(1);
      LAMPPTRGET()->_hsvLoop_2.set_hueRnd(1);

      loadeffconfig(effect, folder);
      if (effStart) {
        worker->init(stripPos, effect, &(getControls()), _lampstate); 
      }

      #ifdef ALML_TFT
        _eff_bri._value   = -1;
        _eff_speed._value = -1;
        _eff_scale._value = -1;  
      #endif


      LAMPPTRGET()->set_hueLoop     (LAMPPTRGET()->effectsLoop_hue());
      LAMPPTRGET()->set_palLoop     (LAMPPTRGET()->effectsLoop_pal());

      FastLED.clear();
      FastLED.show();

    }
    
    #ifdef ALT_DEBUG_TARCE    
      ALT_TRACEC(ALML_DEBUGREGION_EFFECT, "&c:1&s:\tPRINT CONTROLS\n");
      LList<UIControl*>&controlsPrint = LAMPPTRGET()->get_effect(0)->getControls();
      for(int i = 0; i < controlsPrint.size(); ++i) {
        controlsPrint[i]->print();
      }  
    #endif
    // ALT_TRACEC(ALML_DEBUGREGION_EFFECT, "&c:1&s:\tPRINT OUTPUT\n");
    // DynamicJsonDocument doc(2048);
    // JsonObject root = doc.to<JsonObject>();
    // JsonObject oOjbect = root.createNestedObject("outputs");
    // DevicePtrGet()->output_toJson(stripPos, oOjbect, false, true);
    // serializeJsonPretty(doc, Serial);Serial.println();
/*
      value[F("eff_activ")]   = _outputArray[pos].get_eff_activ();
      if (fs) {
      if (_effects[pos]) {
      value[F("eff_nb")]      = _effects[pos]->getEn();      
      }
      value[F("eff_colorMod")] = _outputArray[pos].get_eff_colorMod();
      RgbColor c1;
      _outputArray[pos].eff_get_c1(c1);
      col = value.createNestedObject("eff_c1");
      col[F("r")] = c1.R;
      col[F("g")] = c1.G;
      col[F("b")] = c1.B;
      _outputArray[pos].eff_get_c2(c1);
      col = value.createNestedObject("eff_c2");
      col[F("r")] = c1.R;
      col[F("g")] = c1.G;
      col[F("b")] = c1.B;      
      }
*/
  }

  ALT_TRACEC(ALML_DEBUGREGION_EFFECT, "--\n");
}







