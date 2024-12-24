
void UIControl::print(boolean keyboard){
  String        s_n     = getName();
  uint8_t       s_i     = getId();
  String        s_v     = getVal();   
  String        s_mi    = getMin();      
  String        s_ma    = getMax();      
  String        s_st    = getStep();      
  uint8_t       s_t     = getType();
  uint8_t       s_state = getTypeState();

  String        s_ts  = "NONE";
  String        s_tst  = "NONE";

  switch(s_t){
    case CONTROL_TYPE::RANGEw0 :    s_ts = "RANGEw0";   break;
    case CONTROL_TYPE::RANGE :      s_ts = "RANGE";     break;
    case CONTROL_TYPE::EDIT :       s_ts = "EDIT";      break;
    case CONTROL_TYPE::CHECKBOX :   s_ts = "CHECKBOX";  break;
    default: break;
  }
  switch(s_state){
    case CONTROL_CASE::ALWAYS :     s_tst = "ALWAYS";    break;
    case CONTROL_CASE::HIDE :       s_tst = "HIDE";      break;
    case CONTROL_CASE::ISMICON :    s_tst = "ISMICON";   break;
    case CONTROL_CASE::ISMICOFF :   s_tst = "ISMICOFF";  break;
    default: break;
  }
  // ALT_TRACEC(ALML_DEBUGREGION_EFFECT, "-\n");
  if (!keyboard){
    if (ALT_debugPrint(ALML_DEBUGREGION_EFFECT))
      Serial.printf_P(PSTR("[%-5s: %-30s][%-5s: %-5d][%-5s: %-5s][%-5s: %-5s][%-5s: %-5s][%-5s: %-5s][%-5s: %-5s][%5s]\n"),
        ALMLPT_NAME, s_n.c_str(), ALMLPT_ID, s_i, ALMLPT_VAL, s_v.c_str(), ALMLPT_MI, s_mi.c_str(), ALMLPT_MA, s_ma.c_str(), ALMLPT_ST, s_st.c_str(), ALMLPT_GETTYPE, s_ts.c_str(), s_tst.c_str()   );
  } else {
      Serial.printf_P(PSTR("[%-5s: %-30s][%-5s: %-5d][%-5s: %-5s][%-5s: %-5s][%-5s: %-5s][%-5s: %-5s][%-5s: %-5s][%5s]\n"),
        ALMLPT_NAME, s_n.c_str(), ALMLPT_ID, s_i, ALMLPT_VAL, s_v.c_str(), ALMLPT_MI, s_mi.c_str(), ALMLPT_MA, s_ma.c_str(), ALMLPT_ST, s_st.c_str(), ALMLPT_GETTYPE, s_ts.c_str(), s_tst.c_str()   );
  }
  // ALT_TRACEC(ALML_DEBUGREGION_EFFECT, "--\n");

} 

/*
#define ADS_LOGGER
void _print(UIControl*_val, String value){
  String        s_n     = _val->getName();
  uint8_t       s_i     = _val->getId();
  String        s_v     = value;      
  String        s_mi    = _val->getMin();      
  String        s_ma    = _val->getMax();      
  String        s_st    = _val->getStep();      
  uint8_t       s_t     = _val->getType();
  uint8_t       s_state = _val->getTypeState();

  String        s_ts  = "NONE";

  switch(s_t){
    case CONTROL_TYPE::RANGEw0 :    s_ts = "RANGEw0";   break;
    case CONTROL_TYPE::RANGE :      s_ts = "RANGE";     break;
    case CONTROL_TYPE::EDIT :       s_ts = "EDIT";      break;
    case CONTROL_TYPE::CHECKBOX :   s_ts = "CHECKBOX";  break;
    default: break;
  }
  switch(s_state){
    case CONTROL_CASE::ALWAYS :     s_ts = "ALWAYS";    break;
    case CONTROL_CASE::HIDE :       s_ts = "HIDE";      break;
    case CONTROL_CASE::ISMICON :    s_ts = "ISMICON";   break;
    case CONTROL_CASE::ISMICOFF :   s_ts = "ISMICOFF";  break;
    default: break;
  }
  // ALT_TRACEC(ALML_DEBUGREGION_EFFECT, "-\n");
  Serial.printf_P(PSTR("[%-5s: %-30s][%-5s: %-5d][%-5s: %-5s][%-5s: %-5s][%-5s: %-5s][%-5s: %-5s][%-5s: %-5s][%5d]\n"),
    ALMLPT_NAME, s_n.c_str(), ALMLPT_ID, s_i, ALMLPT_VAL, s_v.c_str(), ALMLPT_MI, s_mi.c_str(), ALMLPT_MA, s_ma.c_str(), ALMLPT_ST, s_st.c_str(), ALMLPT_GETTYPE, s_ts.c_str(), s_t   );
  // ALT_TRACEC(ALML_DEBUGREGION_EFFECT, "--\n");

} 
// #endif
*/

/*

  CLASS PARENT EFFECT UTILISER POUR CHAQUE EFFETS
    setter/sav/getter parametres
    json parametres
    palettes

*/
EffectCalc::EffectCalc(){
}

/**
 * @brief      initialisation des parmetres avec vaelur par de fault de la class
 *
 * @param[in]  strip_pos  postion de larray leds
 * @param[in]  _eff       postion de l'effect "workerset"
 * @param      controls   passage par reference de linstance UIControl
 * @param      lampstate  The lampstate
 */
void EffectCalc::init(uint8_t strip_pos, uint8_t _eff, LList<UIControl*>* controls, LAMPSTATE * lampstate){
  
  effect      = _eff;
  _lampstate  = lampstate;
  _stripPos   = strip_pos;

  ALT_TRACEC(ALML_DEBUGREGION_EFFECT, "-\n");
  ALT_TRACEC(ALML_DEBUGREGION_EFFECT, "isRandDemo: %d\n", isRandDemo());

  setupEff(_stripPos);

  loadDefault();

  // if (atoi(_effeNamIDList[_eff]._cat) == 1) load(strip_pos);

  for(int i=0; i<controls->size(); i++){
    switch(i){
      case 0:
        if(isRandDemo()){
          brightness = random((*controls)[i]->getMin().toInt(),(*controls)[i]->getMax().toInt()+1);
        } else
          brightness = (*controls)[i]->getVal().toInt();
        (*controls)[i]->setVal(String(brightness));
        break;
      case 1:
        if(isRandDemo()){
          speed = random((*controls)[i]->getMin().toInt(),(*controls)[i]->getMax().toInt()+1);
        } else
        speed = (*controls)[i]->getVal().toInt();
        (*controls)[i]->setVal(String(speed));
        break;
      case 2:
        if(isRandDemo()){
          scale = random((*controls)[i]->getMin().toInt(),(*controls)[i]->getMax().toInt()+1);
        } else
          scale = (*controls)[i]->getVal().toInt();
        (*controls)[i]->setVal(String(scale));
        break;
      default:
        setDynCtrl((*controls)[i]);
        break;
    }
  }

  active=true;

  load(strip_pos);

  ALT_TRACEC(ALML_DEBUGREGION_EFFECT, "--\n");
  // LOG(DEBUG_ALS_E, 1, 2, "", "");
}

void EffectCalc::loadDefault(){  

}
void EffectCalc::setupEff(uint8_t strip_pos){  

}
void EffectCalc::load( uint8_t pos){  

}

/**
 * @brief      handle de l'effect enfant
 *
 * @return     
 */
bool EffectCalc::run(){
  return false;
}


/**
 * @brief      delay l'iteration des fonction de l'effet enfant
 *
 * @param[in]  n      duration
 * @param[in]  delay  current duration
 *
 * @return     renvoie false si suffisamment de temps ne s'est pas écoulé depuis EFFECTS_RUN_TIMER 
 */
bool EffectCalc::dryrun(float n, uint8_t delay){
  if((millis() - lastrun - delay) < (unsigned)(float(255 - speed) / n)) {
    active=false;
  } else {
    lastrun = millis();
    active=true;
  }
  return !active;
}


bool EffectCalc::status(){return active;}

/**
 * @brief      set luminositée/brillance pour l'effet
 *
 * @param[in]  _brt  The value
 */
void EffectCalc::setbrt(const byte _brt){
    // if (LAMPPTRGET()->isGlobalBrightnessOn()) {
    //   brightness = _brt;
    // } else {
    //   brightness = _brt;
    // }
  brightness = _brt;  
}

/**
 * @brief      set vitesse de l'effet
 *
 * @param[in]  _spd  The value
 */
void EffectCalc::setspd(const byte _spd){
  if(isRandDemo()){
    speed = random((*ctrls)[1]->getMin().toInt(),(*ctrls)[1]->getMax().toInt()+1);
  } else
    speed = _spd;
}


void EffectCalc::setscl(byte _scl){
  //LOG(printf_P, PSTR("Worker scale: %d\n"), scale);
  if(isRandDemo()){
    scale = random((*ctrls)[2]->getMin().toInt(),(*ctrls)[2]->getMax().toInt()+1);
  } else
  scale = _scl;
}

void EffectCalc::setPalette(UIControl* _val){
  if (usepalettes){ // Начинается с Палитра
    Serial.println("EffectCalc setpalette");
    if(isRandDemo()){
      paletteIdx = random(_val->getMin().toInt(),_val->getMax().toInt()+1);
    } else {
      paletteIdx = _val->getVal().toInt();
    }
    palettemap(palettes, paletteIdx, _val->getMin().toInt(), _val->getMax().toInt());
    isCtrlPallete = true;
    _val->setVal(String(paletteIdx));
  }
}


void EffectCalc::setDynCtrl(UIControl*_val, boolean & v1, String & v2){

  v2 = _val->getVal();

  if(isRandDemo()){ 
    if (_val->getType() == CHECKBOX) {
      uint8_t rnd = random(0, 250);
      (rnd >= 125) ? v2 = "0" : v2 = String(rnd);
    } else if (_val->getType() == RANGEw0) {
      uint8_t rnd = random(0, 250);
      (rnd >= 125) ? v2 = "0" : v2 = random(_val->getMin().toInt(), _val->getMax().toInt()+1);
    }else{
      v2 = random(_val->getMin().toInt(), _val->getMax().toInt()+1);  
    }
  } else {
    v2 = _val->getVal().toInt();
  }

}
String EffectCalc::setDynCtrl(UIControl*_val){
    if(!_val) return String();

    boolean valIsSet = false;

    String ret_val = _val->getVal();

    setDynCtrl(_val, valIsSet, ret_val);

    if (!valIsSet) _val->setVal(String(ret_val));

    return ret_val;
}
void EffectCalc::setDynCtrlNoSet(UIControl*_val){
    if(!_val) return ;

    boolean valIsSet = false;
    String ret_val = _val->getVal();

    this->setDynCtrl(_val, valIsSet, ret_val);
}

void EffectCalc::palettemap(std::vector<PGMPalette*> &_pals, const uint8_t _val, const uint8_t _min,  const uint8_t _max){
  if (!_pals.size() || _val>_max) {
    Serial.println(F("No palettes loaded or wrong value!"));
    return;
  }
  ptPallete = (_max+0.1)/_pals.size();     // сколько пунктов приходится на одну палитру; 255.1 - диапазон ползунка, не включая 255, т.к. растягиваем только нужное :)
  palettepos = (uint8_t)(_max ? (float)_val/ptPallete : 0);
  curPalette = _pals.at(palettepos);
  palettescale = _val-ptPallete*(palettepos); // разбиваю на поддиапазоны внутри диапазона, будет уходить в 0 на крайней позиции поддиапазона, ну и хрен с ним :), хотя нужно помнить!
  
  Serial.printf_P(PSTR("Mapping value to pallete: Psize=%d, POS=%d, ptPallete=%4.2f, palettescale=%d\n"), _pals.size(), palettepos, ptPallete, palettescale);
}

void EffectCalc::scale2pallete(){
  if (!usepalettes)
    return;

  // if (curPalette==nullptr){
  //   if(isRandDemo())
  //     palettemap(palettes, random(0, FASTLED_PALETTS_COUNT), 0,  FASTLED_PALETTS_COUNT);
  //   else
  //     palettemap(palettes, 0, 0,  FASTLED_PALETTS_COUNT);    
  // }

  setbrt((*ctrls)[0]->getVal().toInt());
  setspd((*ctrls)[1]->getVal().toInt());
  setscl((*ctrls)[2]->getVal().toInt());
  for(int i=3;i<ctrls->size();i++){
    setDynCtrl((*ctrls)[i]);
    #ifdef ADS_LOGGER
      _print((*ctrls)[i], (*ctrls)[i]->getVal());  
    #endif
  }
}



void EffectCalc::geteffJsonconfig(JsonObject & doc, uint8_t replaceBright)
{
  doc[F("size")] = ctrls->size();
  JsonObject arr = doc.createNestedObject(F("ctrls"));  
  for(int i =0;i<ctrls->size();i++){
    // JsonObject var = arr.createNestedObject(String(i));
    // var[F("0")] = (*ctrls)[i]->getId();
    // var[F("1")] = (*ctrls)[i]->getType();
    // var[F("2")] = (*ctrls)[i]->getName();
    // var[F("3")] = ((*ctrls)[i]->getId()==0 && replaceBright) ? String(replaceBright) : (*ctrls)[i]->getVal();
    // var[F("4")] = (*ctrls)[i]->getMin();
    // var[F("5")] = (*ctrls)[i]->getMax();
    // var[F("6")] = (*ctrls)[i]->getStep();
    // var[F("7")] = (*ctrls)[i]->getTypeState();
    JsonArray arr2 = arr.createNestedArray(String(i));
    arr2.add((*ctrls)[i]->getId());
    arr2.add((*ctrls)[i]->getType());
    arr2.add((*ctrls)[i]->getName());
    arr2.add(((*ctrls)[i]->getId()==0 && replaceBright) ? String(replaceBright) : (*ctrls)[i]->getVal());
    arr2.add((*ctrls)[i]->getMin());
    arr2.add((*ctrls)[i]->getMax());
    arr2.add((*ctrls)[i]->getStep());
    arr2.add((*ctrls)[i]->getTypeState());
  } 
}
void EffectCalc::geteffJsonconfig(JsonObject & doc, uint8_t replaceBright, int id)
{
  doc[F("id")] = id;
  for(int i =0;i<ctrls->size();i++){
    if ((*ctrls)[i]->getId()==id) {
      doc[F("val")] = ((*ctrls)[i]->getId()==0 && replaceBright) ? String(replaceBright) : (*ctrls)[i]->getVal(); 
      break;
    }
  } 
}


void EffectCalc::palettesload(){
    palettes.reserve(FASTLED_PALETTS_COUNT);
    palettes.push_back(&AuroraColors_p/*RainbowStripeColors_p*/);
    palettes.push_back(&ForestColors_p);
    palettes.push_back(&NormalFire_p);
    palettes.push_back(&LavaColors_p);
    palettes.push_back(&OceanColors_p);
    palettes.push_back(&PartyColors_p);
    palettes.push_back(&RainbowColors_p);
    palettes.push_back(&HeatColors_p);
    palettes.push_back(&CloudColors_p);
    palettes.push_back(&EveningColors_p);
    palettes.push_back(&LithiumFireColors_p);
    palettes.push_back(&WoodFireColors_p);
    palettes.push_back(&SodiumFireColors_p);
    palettes.push_back(&CopperFireColors_p);
    palettes.push_back(&AlcoholFireColors_p);
    palettes.push_back(&RubidiumFireColors_p);
    palettes.push_back(&PotassiumFireColors_p);
    palettes.push_back(&WaterfallColors_p);
    palettes.push_back(&AutumnColors_p);
    palettes.push_back(&AcidColors_p);
    palettes.push_back(&StepkosColors_p);
    palettes.push_back(&HolyLightsColors_p/*OrangeColors_p NeonColors_p*/);

    usepalettes = true; 
    scale2pallete();    
}
void EffectCalc::setPalette(uint8_t p){
  // LOG(-1,0,2,"","");
    if (usepalettes) {
      // LOG(-1,2,2,"","");
      palettemap(palettes, map(p, 0, 255, 0, FASTLED_PALETTS_COUNT), 0,  FASTLED_PALETTS_COUNT);
    }
  // LOG(-1,1,2,"","");
}



