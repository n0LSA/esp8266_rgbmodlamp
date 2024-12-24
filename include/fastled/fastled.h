#ifndef LEDS_FASTLED_H
    #define LEDS_FASTLED_H
    // #define USE_LEDSFASTLED

    #include <Arduino.h>

    #include "base.h"

    template<uint8_t DATA_PIN, EOrder RGB_ORDER> class leds : public ledsBase
    {   

    public:
        void init(uint8_t stripPos) {
            char            buffer[80];
            String          buffer_str;
            int             rSize;
            const char**    list;

            // _pin = DATA_PIN;
            set_ledsPin(DATA_PIN);


            buffer[0]=0;
            sprintf(buffer, "%s", ADS_ONBLEDS);
            buffer_str = String(buffer);
            list = al_tools::explode(buffer_str, '.', rSize);
            if (rSize<0){
                // _num_leds = buffer_str.toInt();
                
                set_numLeds(buffer_str.toInt());
            } else {
                // _num_leds = atoi(list[stripPos]);
                set_numLeds(atoi(list[stripPos]));
                for(int i = 0; i < rSize; ++i) {delete list[i];}delete[] list;              
            }

            // _stripPos    = stripPos;
            set_stripPos(stripPos);
            // _contructor = 1;

            buffer[0]=0;
            sprintf(buffer, "%s", ADS_OTW);
            buffer_str = String(buffer);
            list = al_tools::explode(buffer_str, '.', rSize);   
            if (rSize < 0) {
                if (buffer_str.toInt() == 1)  set_ledsIsTw(true);
                else                          set_ledsIsTw(false);
            }    else {
                if (atoi(list[stripPos]) == 1)set_ledsIsTw(true);
                else                                                set_ledsIsTw(false);
                for(int i = 0; i < rSize; ++i) {delete list[i];}delete[] list;              
            }   
        
            boolean isHackTw;
            get_ledsIsTw(isHackTw);
            if (!isHackTw) {
                uint16_t numLeds;
                get_numLeds(numLeds);
                _leds = new CRGB[numLeds]();
                #ifdef ADS_LEDSFASTLEDS_AUTO    
                    FastLED.addLeds<WS2812B, DATA_PIN, RGB_ORDER>(_leds, numLeds).setCorrection(TypicalLEDStrip);
                #endif

            } else {
                uint16_t numLeds;
                get_numLeds(numLeds);               
                _ledsW      = new CRGBW[numLeds]();
                _ledsRGB    = (CRGB *) &_ledsW[0];
                // _num_ledsW   = getRGBWsize(numLeds);
                set_numLedsW(getRGBWsize(numLeds));
                #ifdef ADS_LEDSFASTLEDS_AUTO
                    get_numLedsW(numLeds);
                    FastLED.addLeds<WS2812B, DATA_PIN, RGB>(_ledsRGB, numLeds).setCorrection(TypicalLEDStrip);
                #endif
            }           
        }       
        leds(uint8_t stripPos) {
            init(stripPos);
            FastLED.setBrightness( 128 );
            FastLED.setMaxPowerInVoltsAndMilliamps( 5, 1000);    
        }
        leds(uint8_t stripPos, uint16_t nbLeds, boolean isHackTw) {
            set_numLeds(nbLeds);
            set_stripPos(nbLeds);
            set_ledsIsTw(isHackTw);
            if (!isHackTw) {
                uint16_t numLeds;
                get_numLeds(numLeds);
                _leds = new CRGB[numLeds]();
                #ifdef ADS_LEDSFASTLEDS_AUTO    
                    FastLED.addLeds<WS2812B, DATA_PIN, RGB_ORDER>(_leds, numLeds);;
                #endif

            } else {
                uint16_t numLeds;
                get_numLeds(numLeds);               
                _ledsW      = new CRGBW[numLeds]();
                _ledsRGB    = (CRGB *) &_ledsW[0];
                // _num_ledsW   = getRGBWsize(numLeds);
                set_numLedsW(getRGBWsize(numLeds));
                #ifdef ADS_LEDSFASTLEDS_AUTO
                    get_numLedsW(numLeds);
                    FastLED.addLeds<WS2812B, DATA_PIN, RGB>(_ledsRGB, numLeds);;
                #endif
            } 
            FastLED.setBrightness( 128 );
            FastLED.setMaxPowerInVoltsAndMilliamps( 5, 1000);    
        }
    };



#endif
