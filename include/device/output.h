#ifndef OUTPUT_H
#define OUTPUT_H

  #include "../fastled/base.h"  
  #include "../pixel/set.h" 

static const char FNPATH_COLORPICKER    [] PROGMEM = "/outputs/colopicker.json";

  class leds_segments
  {
    
  public:
    uint32_t    _start;
    uint32_t    _end;
    boolean     _reverse;
    leds_segments(uint32_t start, uint32_t end, boolean dir);
    ~leds_segments(){};
    
  };
  class leds_segment
  {
    uint8_t _cnt = 0;
    leds_segments ** _array = nullptr;

  public:

    void segment_new(uint32_t start, uint32_t end, boolean dir);

    boolean isSegment();

    void get_max(uint8_t & max);

    leds_segments * get_segment(uint8_t pos);

    leds_segment(){_array = new leds_segments*[8];};
    ~leds_segment(){};
    
  };


  enum aramoire_side { 
    aramoire_side_left, 
    aramoire_side_right
  }; 
  class aramoire_segemnts
  {
 
  public:
    uint32_t    _start;
    uint32_t    _stop;
    boolean     _reverse;

      // COULEURS STATIQUE
      // 
      /** \brief definit si les couleur sont de type blanche */ 
      boolean _isTw = false; 
      /** \brief definit la brillance du blanc */ 
      uint8_t _tw     = 255;
      /** \brief  si ce n'est pas un strip RGBW definit la saturation de la couleur "jaune" */        
      uint8_t _tw_v   = 255;

      /** \brief format hsv: hue */ 
      uint8_t _hsv_h  = 80;
      /** \brief format hsv: saturation  */ 
      uint8_t _hsv_s  = 255;
      /** \brief format hsv: value */ 
      uint8_t _hsv_v  = 255;

      /** \brief format rgb: red */ 
      uint8_t _rgb_r  = 255;
      /** \brief format rgb: green */ 
      uint8_t _rgb_g  = 255;
      /** \brief format rgb: blue */  
      uint8_t _rgb_b  = 0;

      /** \brief couleur actuelle */    
      CRGB _c1 = CRGB(0, 0, 60);
      /*
        si le strip et a l'état off (_onoff == false)
          _coff = _c1
          _c1 = 0.0.0
      */
      /** \brief sauvegarde de la couleur */        
      CRGB _coff = CRGB(0, 255, 0);;
    // uint8_t _v;

    aramoire_segemnts(uint32_t start, uint32_t end, boolean dir);
    ~aramoire_segemnts(){};
    
  };


  class aramoire_segemnt
  {

    uint8_t _segements_left_pos = 0;
    uint8_t _segements_right_pos = 0;
  public:
    aramoire_segemnts * _segements_left[4];
    aramoire_segemnts * _segements_right[4];

    // uint16_t  * _left = nullptr;
    // uint16_t  * _right = nullptr;

    void segemnt_new(uint32_t start, uint32_t end, boolean dir, const aramoire_side &side);

    void isSegemnt(boolean & ret);
    boolean isSegemnt();

    void get_segmentCnt(uint8_t &  max);

    // void segemnt_leftRight(int nbr);

    // aramoire_segemnts * get_segment(int pos);
    aramoire_segemnts * get_sideSegment(aramoire_side dir, int pos);
    aramoire_segemnts * get_segment(int pos);
    void get_sideSegmentCnt(aramoire_side dir, uint8_t & result);

    aramoire_segemnt();
    ~aramoire_segemnt(){};
    
  };
  aramoire_segemnt * aramoire_segemntPtr_get();

  /*
    class attribuer a chaque pin utiliser "output"
      attribution d'un type fixe du strip 
                  d'une taille fixe du nombre de les 
                  d'un nom sous forme de label modifiable
                  d'une position fixe de l'ordre d'ajout des strip
      sera utiliser por stocker les parametre de ce qu'affiche les leds
    chaque classe creer sa propr intance de la class LAMP pour animer les leds
  */
  /**
   * @class     Output
   * 
   * @brief   class pour la gestion/attribution de chaque strip
   * @details   
   * @see         https://github.com/AdriLighting/
   * 
   * @author      Adrilighting
   * @date      mar. 28 déc. 2021 16:25:30
   */
  class Output 
  {

      // ######################################
      // OUTPUT INFOS
      //      
      /** \brief nom/label pour identifier le strip */
      String  _name     = "device";

      /*
        _type
        0 = strip_rgb 
        1 = strip_rgbw
        2 = plug (LP)
      */    
      /** \brief type de strip/output */    
      uint8_t   _type = 0;
      /** \brief pin utiliser par la board */ 
      uint8_t   _pin  = 0;
      /** \brief nbr de leds du strip */  
      uint16_t  _size = 0; 
      //
      // ######################################

      // ######################################
      // OUTPUT STATU
      //      
      /*
        if (!_select)
          la slectionn et ignorer pour tout changement d'etait sauf pour "_select"
      */
      /** \brief si le strip est selectionner  */ 
      boolean _select = true;

      /*
        if (!onoff)
          la slectionn et ignorer pour tout changement d'etat sauf pour "_select" && "_onoff"
      */
      /** \brief allumer ou eteint */ 
      boolean _onoff  = true; 
      //
      // ######################################
      
      
      /*
        pas encor utiliser, mais sera utiliser pour definir le suille de luminositée
      */
      /** \brief puissance de la brillance total du strip */  
      uint8_t _briEff   = 255;
      uint8_t _bri      = 255;
      uint8_t _briOld   = 255;



      // ######################################
      // COULEURS STATIQUE
      // 
      /** \brief definit si les couleur sont de type blanche */ 
      boolean _isTw = false; 
      /** \brief definit la brillance du blanc */ 
      uint8_t _tw     = 255;
      /** \brief  si ce n'est pas un strip RGBW definit la saturation de la couleur "jaune" */        
      uint8_t _tw_v   = 255;

      /** \brief format hsv: hue */ 
      uint8_t _hsv_h  = 80;
      /** \brief format hsv: saturation  */ 
      uint8_t _hsv_s  = 255;
      /** \brief format hsv: value */ 
      uint8_t _hsv_v  = 255;

      /** \brief format rgb: red */ 
      uint8_t _rgb_r  = 255;
      /** \brief format rgb: green */ 
      uint8_t _rgb_g  = 255;
      /** \brief format rgb: blue */  
      uint8_t _rgb_b  = 0;

      /** \brief couleur actuelle */    
      CRGB _c1;
      /*
        si le strip et a l'état off (_onoff == false)
          _coff = _c1
          _c1 = 0.0.0
      */
      /** \brief sauvegarde de la couleur */        
      CRGB _coff;
      //
      // ######################################

      // ######################################
      // EFFETS
      // 
      // OUTPUT_EFFECTTYPE _output_effecttype; 
      // OUTPUT_EFFECTTYPE _output_defaultPal; 

      /*
        si (_eff_activ == true)
          la slectionn et ignorer pour tout changement d'etat pour les COULEURS STATIQUE
      */
      /** \brief activation/désactivation des effets  */        
      boolean _eff_activ      = true;
      /** \brief  */  
      uint8_t _eff_colorMod   = 0;
      /** \brief  couleur statique pour l'effet (from "rgb" request)*/  
      CRGB _eff_c1;
      /** \brief couleur statique secondaire pour l'effet (PAS DE REQUETTE ATTRIBUER) */  
      CRGB _eff_c2;
      //
      //
    
      //
      // ######################################

      // void onoff();
      void onoff          (const JsonObject & v); 
      void set_color_rgb  (const JsonObject & v, boolean set_bri = true); 
      void set_color_eff_rgb1 (const JsonObject & v); 
      void set_color_h      (const JsonObject & v); 
      void set_color_hIncr  (const JsonObject & v); 
      void set_color_s    (const JsonObject & v); 
      void set_color_v    (const JsonObject & v); 
      void set_color_tw   (const JsonObject & v); 
      void set_color_tw   (uint8_t); 
      void set_color_tw_v (const JsonObject & v); 

      void set_colorBriIncr(uint8_t, int8_t, boolean add = true);
      
      void set_briIncr   (int8_t); 

      CRGB fade2black(CRGB rgb);
      CRGB fade2black(CRGBW rgb);

    public:
      void set_briGen(int8_t amount);
      void set_lastColor();

      // boolean _audio_activ    = true;

      // ######################################

      leds_segment _leds_segment;
      aramoire_segemnt _armoire;
      boolean _isArmoire = false;
      uint8_t _armoireEtage = 0;

      /** \brief position du strip attribuer a la céation de l'instance */
      uint8_t _pos  = 0;

      /** \brief tyoe d'effet en cour de lecture */

      // ######################################

      /**
       * @brief      instance céer dynamicment 
       * @see       https://github.com/AdriLighting/
       * 
       * @author    Adrilighting
       * @date      mar. 28 déc. 2021 16:25:30 
       */
      Output();
      /**
       * @brief      pas encor utiliser car le ptr original n'est pas destiner a etre detruit
       */
      ~Output();



      boolean set_eff_autoplay    (const String & cmd, const JsonObject & v);
      boolean set_eff_bri         (const String & cmd, const JsonObject & v);
      boolean set_eff_scale       (const String & cmd, const JsonObject & v);
      boolean set_eff_speed       (const String & cmd, const JsonObject & v);
      boolean set_eff_speedIncr   (const String & cmd, const JsonObject & v);
      boolean set_eff_prev        (const String & cmd, const JsonObject & v);
      boolean set_eff_activ       (const String & cmd, const JsonObject & v);
      // boolean set_eff_f           (const String & cmd, const JsonObject & v);
      boolean set_eff_id          (const String & cmd, const JsonObject & v);
      boolean set_select          (const String & cmd, const JsonObject & v);
      boolean set_onoff           (const String & cmd, const JsonObject & v);
      boolean set_toggle          (const String & cmd, const JsonObject & v);
      boolean set_bri             (const String & cmd, const JsonObject & v);
      boolean set_briIncr         (const String & cmd, const JsonObject & v);
      boolean set_tw              (const String & cmd, const JsonObject & v);
      boolean set_tw_v            (const String & cmd, const JsonObject & v);
      boolean set_tw_f            (const String & cmd, const JsonObject & v);
      boolean set_hsv_h           (const String & cmd, const JsonObject & v);
      boolean set_hsv_hIncr       (const String & cmd, const JsonObject & v);
      boolean set_hsv_s           (const String & cmd, const JsonObject & v);
      boolean set_hsv_v           (const String & cmd, const JsonObject & v);
      boolean set_rgb             (const String & cmd, const JsonObject & v);   
      boolean set_rgb_f           (const String & cmd, const JsonObject & v);   
      boolean set_eff_rgb1        (const String & cmd, const JsonObject & v);   
      boolean set_eff_colorMod    (const String & cmd, const JsonObject & v);  
      boolean set_etage           (const String & cmd, const JsonObject & v);  
      boolean set_eff_sav         (const String & cmd, const JsonObject & v);

      void set_pos(uint8_t);


      void onoff();

      void set_briArmoire(int8_t amount, boolean add);
      void set_colorArmoire();
      void set_colorArmoireToOutput();
      void set_color(); 
      void set_color(CRGB c, int tw); 
      void clear(); 
      void stripFadeToBlack(boolean isSegment = false);



      boolean eff_isHueTimer();

      boolean eff_colormod_1c();
      boolean eff_colormod_2c();

      void eff_get_c1(RgbColor & c1);
      void eff_get_c2(RgbColor & c1);
      uint8_t eff_get_hue();
      uint8_t get_eff_colorMod();

/*
      OUTPUT_EFFECTTYPE get_effectType(); 
      OUTPUT_EFFECTTYPE get_defaultPal(); 
      void set_effectType(OUTPUT_EFFECTTYPE); 
      void set_defaultPal(OUTPUT_EFFECTTYPE); 
      bool isPalEffect(OUTPUT_EFFECTTYPE value);
*/

      boolean get_select();
      boolean get_onoff();

      uint8_t get_briEff();
      uint8_t get_bri();
      uint8_t get_briOld();

      boolean get_eff_activ();

      boolean get_isTw();
      uint8_t get_tw();
      uint8_t get_tw_v();

      uint8_t get_hsv_h();
      uint8_t get_hsv_s();
      uint8_t get_hsv_v();
      uint8_t get_rgb_r();
      uint8_t get_rgb_g();
      uint8_t get_rgb_b();
      
      CRGB get_coff();
      CRGB get_c1();

      String get_name();
      uint8_t get_type();
      uint8_t get_pin();
      uint16_t get_size();


      void set_type     (uint8_t v);
      void set_pin      (uint8_t v);
      void set_size     (uint16_t v);
      void set_name     (String v);

      void set_bri      (uint8_t v);
      void set_briGlobal (uint8_t v);
      void set_briLoop    ();   

      void set_effActiv (boolean v);



      // void setValueFromJson(DynamicJsonDocument doc);
      void setValueFromSpiff(const String & path = "/outputs/lamp.txt");
      void armoiureFromSpiff();


      // void audio_classique();


  };


#endif // OUTPUT_H