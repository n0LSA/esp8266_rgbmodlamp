#ifndef MATRIXEFFECT_H
	#define MATRIXEFFECT_H

#ifdef ALS_LAMPMATRIX

  #include "../fastled/base.h"  
  #include "../pixel/effectcore.h"
  #include "../device/device.h"
	#include "pixeleffect.h"
	#include "../pixel/effectmath.h"
	#include "../display/lamp.h"
	#include "../config.h"

#define trackingOBJECT_MAX_COUNT    (WIDTH *3)  // максимальное количество отслеживаемых объектов (очень влияет на расход памяти)
#define enlargedOBJECT_MAX_COUNT    (WIDTH *3) // максимальное количество сложных отслеживаемых объектов (меньше, чем trackingOBJECT_MAX_COUNT)


	class EffectLighters : public EffectFx_comm {
	protected:
	    uint8_t cnt = 1;
	    bool subPix = false;
	    uint16_t lightersIdx;
	    float lightersSpeed[2U][LIGHTERS_AM];
	    uint8_t lightersColor[LIGHTERS_AM];
	    float lightersPos[2U][LIGHTERS_AM];
	    byte light[LIGHTERS_AM];
	private:
	    bool lightersRoutine(CRGB *leds);
	    String setDynCtrl(UIControl*_val) override;
	public:

      void load(uint8_t) override;
      bool run() override;
      void setupEff(uint8_t sP) override {
      	setup(sP);
      }
	};


	class MEffectSparcles : public EffectFx_comm {
	private:
	    hsvloopManage _hsvLoop;
	    uint8_t eff = 1;
	    bool sparklesRoutine(CRGB *leds);
	public:
			MEffectSparcles(uint8_t mod = 0){_mod = mod;};
			void load(uint8_t) override;
			bool run() override;
			void setupEff(uint8_t sP) override {
				setup(sP);
			}
	    String setDynCtrl(UIControl*_val) override;
	};

	class MEffectWhiteColorStripe : public EffectFx_comm {
	private:
	    bool whiteColorStripeRoutine(CRGB *leds);

	public:
			MEffectWhiteColorStripe(uint8_t mod = 0){_mod = mod;};
			void load(uint8_t) override;
			bool run() override;
			void setupEff(uint8_t sP) override {
				setup(sP);
			}
	    String setDynCtrl(UIControl*_val) override;
	};	

	class MEffectEverythingFall : public EffectFx_comm {
	private:
	    byte heat[WIDTH][HEIGHT];
	public:
			MEffectEverythingFall(uint8_t mod = 0){_mod = mod;};
			void load(uint8_t) override;
	    bool run() override;
	    void setupEff(uint8_t sP) override {
	    	setup(sP);
	    }
	    String setDynCtrl(UIControl*_val) override;
	};

	class MEffectPulse : public EffectFx_comm {
	private:
	    uint8_t pulse_hue;
	    uint8_t pulse_step = 0;
	    uint8_t centerX = random8(WIDTH - 5U) + 3U;
	    uint8_t centerY = random8(HEIGHT - 5U) + 3U;
	    uint8_t currentRadius = 4;
	    uint8_t _pulse_hue = 0;
	    uint8_t _pulse_hueall = 0;
	    bool pulseRoutine(CRGB *leds);
	public:
			MEffectPulse(uint8_t mod = 0){_mod = mod;};
			void load(uint8_t) override;
	    bool run() override;
	    void setupEff(uint8_t sP) override {
	    	setup(sP);
	    }
	    String setDynCtrl(UIControl*_val) override;
	};

	class MEffectRainbow : public EffectFx_comm {
	private:
	    float hue; // вещественное для малых скоростей, нужно приведение к uint8_t по месту
	    float twirlFactor;
	    float micCoef;
	    bool rainbowHorVertRoutine(bool isVertical);
	    bool rainbowDiagonalRoutine();
	public:
      MEffectRainbow(uint8_t mod = 0){_mod = mod;};
      void load(uint8_t) override;
      bool run() override;
      void setupEff(uint8_t sP) override {
      	setup(sP);
      }
      String setDynCtrl(UIControl*_val) override;
	};

	class MEffectMatrix : public EffectLighters {
	private:
	    boolean timer = false;
	    boolean colorCycle = false;
	    bool matrixRoutine(CRGB *leds);
	    uint8_t _scale = 1;
	    byte gluk = 1;
	    uint8_t _cMod;
	    uint8_t lightersColorHue;
	    bool randColor = false;
	    bool white = false;
	    float count;
	    String setDynCtrl(UIControl*_val) override;
	public:
			MEffectMatrix(uint8_t mod = 0){_mod = mod;};
      void load(uint8_t) override;
      bool run() override;
      void setupEff(uint8_t sP) override {
      	setup(sP);
      }
	};

	class MEffectStarFall : public EffectLighters {
	private:
	    uint8_t _cMod = 0;
	    uint8_t _cValue = 0;
	    uint8_t _scale = 1;
	    uint8_t effId = 1;
	    bool isNew = true;
	    float fade;
	    bool snowStormStarfallRoutine(CRGB *leds);
	    String setDynCtrl(UIControl*_val) override;
	public:
			MEffectStarFall(uint8_t mod = 0){_mod = mod;};
      void load(uint8_t) override;
      bool run() override;
      void setupEff(uint8_t sP) override {
      	setup(sP);
      }
	};

	#define _AMOUNT 16U
	class MEffectLighterTracers : public EffectFx_comm {
	private:
	    uint8_t _cMod = 0;
	    uint8_t cnt = 1;
	    float vector[_AMOUNT][2U];
	    float coord[_AMOUNT][2U];
	    int16_t ballColors[_AMOUNT];
	    byte light[_AMOUNT];
	    bool lighterTracersRoutine(CRGB *leds);

	public:
			MEffectLighterTracers(uint8_t mod = 0){_mod = mod;};
      void load(uint8_t) override;
      bool run() override;
      void setupEff(uint8_t sP) override {
      	setup(sP);
      }
	    String setDynCtrl(UIControl*_val) override;
	};

	class MEffectLightBalls : public EffectFx_comm {
	private:
	    bool lightBallsRoutine(CRGB *leds);
	    String setDynCtrl(UIControl*_val) override;
	public:
			MEffectLightBalls(uint8_t mod = 0){_mod = mod;};
      void load(uint8_t) override;
      bool run() override;		
      void setupEff(uint8_t sP) override {
      	setup(sP);
      }
  };

	class MEffectBall : public EffectFx_comm {
	private:
	    uint8_t ballSize;
	    CHSV ballColor;
	    float vectorB[2U];
	    float coordB[2U];
	    bool ballRoutine(CRGB *leds);
	    String setDynCtrl(UIControl*_val) override;
	public:
			MEffectBall(uint8_t mod = 0){_mod = mod;};
      void load(uint8_t) override;
      bool run() override;		
      void setupEff(uint8_t sP) override {
      	setup(sP);
      }
	};




// ----------- Эффект "Огненная лампа"
// https://editor.soulmatelights.com/gallery/546-fire
// (c) Stepko 17.06.21
// sparks (c) kostyamat 10.01.2022 https://editor.soulmatelights.com/gallery/1619-fire-with-sparks
class MEffectFire2021 : public EffectFx_comm {
private:
    byte _pal = 8;
    byte _scale = 32;
		byte speedFactor;
    uint32_t t;
    bool withSparks = false;

    const uint8_t sparksCount = WIDTH / 4;
    const uint8_t spacer = HEIGHT/4;

    class Spark : public EffectFx_comm  {
    private:
        CRGB color;
        uint8_t Bri;
        uint8_t Hue;
        float x, y, speedy = 1;
    
    public:
        void addXY(float nx, float ny) {
            EffectMath::drawPixelXYF(x, y, 0);
            x += nx;
            y += ny * speedy;
        }

        float getY() {
        return y;
        }

        void reset(CRGB *leds) {
            uint32_t peak = 0;
            speedy = (float)random(5, 30) / 10;
            y = random((HEIGHT/4) * 5, (HEIGHT /2) * 5) / 5;
            for (uint8_t i=0; i < WIDTH; i++) {
                uint32_t temp = EffectMath::RGBweight(leds, getPixelNumber(i, y));
                if (temp > peak) {
                    x = i;
                    peak = temp;
                }
            }
            color = EffectMath::getPixel(x, y);
        }

        void draw() {
            color.fadeLightBy(256 / (HEIGHT));
            EffectMath::drawPixelXYF(x, y, color);
        }
    }; 

    LList<Spark *> sparks;

		String setDynCtrl(UIControl*_val) override;
	public:
			MEffectFire2021(uint8_t mod = 0){_mod = mod;};
      void load(uint8_t) override;
      bool run() override;	
      void setupEff(uint8_t sP) override {
      	setup(sP);
      }
	    ~MEffectFire2021() {
			  while (sparks.size()) {
			    Spark * item = sparks.shift();
			    delete item;
			  }
			  sparks.clear();      	
	    }
};

/*
#define MAX_DIMENSION ((WIDTH > HEIGHT) ? WIDTH : HEIGHT)
class MEffect3DNoise : public EffectFx_comm {
private:


// The 16 bit version of our coordinates
uint16_t noisex;
uint16_t noisey;
uint16_t noisez;

// We're using the x/y dimensions to map to the x/y pixels on the matrix.  We'll
// use the z-axis for "time".  speed determines how fast time moves forward.  Try
// 1 for a very slow moving effect, or 60 for something that ends up looking like
// water.
// uint32_t __noisespeedx = 1;
uint32_t noisespeedy = 1;
uint32_t noisespeedz = 1;

// Scale determines how far apart the pixels in our noise matrix are.  Try
// changing these values around to see how it affects the motion of the display.  The
// higher the value of scale, the more "zoomed out" the noise will be.  A value
// of 1 will be so zoomed in, you'll mostly see solid colors.
// uint16_t __noisescale = 30; // scale is set dynamically once we've started up

// This is the array that we keep our computed noise values in
uint8_t noise[MAX_DIMENSION][MAX_DIMENSION];

uint8_t colorLoop = 0;
void fillnoise8(uint32_t noisespeedx, uint16_t noisescale);
void mapNoiseToLEDsUsingPalette(CRGBPalette16 palette, uint8_t hueReduce);

    String setDynCtrl(UIControl*_val) override;
public:
			MEffect3DNoise(uint8_t mod = 0){_mod = mod;};
      void load(uint8_t) override;
      bool run() override;		
      void setupEff(uint8_t sP) override {
      	setup(sP);
      }
};
*/
class MEffect3DNoise : public EffectFx_comm {
private:
    void fillNoiseLED();
    void fillnoise8();

    uint8_t ihue;
    bool colorLoop;
		bool blurIm;
    float _speed;             // speed is set dynamically once we've started up
    float _scale;             // scale is set dynamically once we've started up
    float x;
    float y;
    float z;

    uint8_t **noise;

    String setDynCtrl(UIControl*_val) override;
public:
			MEffect3DNoise(uint8_t mod = 0){_mod = mod;};
			~MEffect3DNoise(){
		    #if (WIDTH > HEIGHT)
					for(int i = 0; i < 2*HEIGHT; ++i) {
					    delete [] noise[i];
					}
					delete [] noise;
		    #else
					for(int i = 0; i < 2*WIDTH; ++i) {
					    delete [] noise[i];
					}
					delete [] noise;
		    #endif			
			}
      void load(uint8_t) override;
      bool run() override;		
      void setupEff(uint8_t sP) override {
      	setup(sP);
      }
};

	class MEffectBBalls : public EffectFx_comm {
	private:
	    // можно переписать на динамческую память
	    uint8_t bballsNUM_BALLS;                            // Number of bouncing balls you want (recommend < 7, but 20 is fun in its own way) ... количество мячиков теперь задаётся бегунком, а не константой
	    byte bballsCOLOR[bballsMaxNUM_BALLS] ;              // прикручено при адаптации для разноцветных мячиков
	    byte bballsBri[bballsMaxNUM_BALLS];                 // --- // ---
	    int8_t bballsX[bballsMaxNUM_BALLS] ;                // прикручено при адаптации для распределения мячиков по радиусу лампы
	    float bballsPos[bballsMaxNUM_BALLS] ;               // The integer position of the dot on the strip (LED index)
	    float bballsHi = 0.0;                               // An array of heights
	    float bballsVImpact[bballsMaxNUM_BALLS] ;           // As time goes on the impact velocity will change, so make an array to store those values
	    uint32_t bballsTCycle = 0;                        // The time since the last time the ball struck the ground
	    float bballsCOR[bballsMaxNUM_BALLS] ;               // Coefficient of Restitution (bounce damping)
	    long  bballsTLast[bballsMaxNUM_BALLS] ;             // The clock time of the last ground strike
	    float bballsShift[bballsMaxNUM_BALLS];
	    float hue;
	    bool halo = false;                                  // ореол
	    uint8_t _scale=1;
	    uint16_t _speed;
	    bool bBallsRoutine(CRGB *leds);
	    void regen();
			String setDynCtrl(UIControl*_val) override;
	public:
			MEffectBBalls(uint8_t mod = 0){_mod = mod;};
      void load(uint8_t) override;
      bool run() override;	
      void setupEff(uint8_t sP) override {
      	setup(sP);
      }
	};

	class MEffectSinusoid3 : public EffectFx_comm {
	private:
	    bool sinusoid3Routine(CRGB *leds);
	    String setDynCtrl(UIControl*_val) override;
	public:
			MEffectSinusoid3(uint8_t mod = 0){_mod = mod;};
      void load(uint8_t) override;
      bool run() override;		
      void setupEff(uint8_t sP) override {
      	setup(sP);
      }
  };

	class MEffectMetaBalls : public EffectFx_comm {
	private:
	    bool metaBallsRoutine(CRGB *leds);
	    String setDynCtrl(UIControl*_val) override;
	public:
			MEffectMetaBalls(uint8_t mod = 0){_mod = mod;};
      void load(uint8_t) override;
      bool run() override;		
      void setupEff(uint8_t sP) override {
      	setup(sP);
      }
	};

// ----------- Эффект "Осцилятор"
// (c) Сотнег (SottNick)
class MEffectOscilator: public EffectFx_comm {
  private:
    uint8_t hue, hue2;                                 // постепенный сдвиг оттенка или какой-нибудь другой цикличный счётчик
    uint8_t deltaHue, deltaHue2;                       // ещё пара таких же, когда нужно много
    uint8_t step;                                      // какой-нибудь счётчик кадров или последовательностей операций
    uint8_t deltaValue;  
    unsigned long timer;

    class oscillatingCell {
        public:
        byte red;
        byte blue;
        byte green;
        byte color;
    };

    oscillatingCell oscillatingWorld[WIDTH][HEIGHT];

    void drawPixelXYFseamless(float x, float y, CRGB color);
    int redNeighbours(uint8_t x, uint8_t y);
    int blueNeighbours(uint8_t x, uint8_t y);
    int greenNeighbours(uint8_t x, uint8_t y);
    void setCellColors(uint8_t x, uint8_t y);
    String setDynCtrl(UIControl*_val) override;
  public:
			MEffectOscilator(uint8_t mod = 0){_mod = mod;};
      void load(uint8_t) override;
      bool run() override;		
      void setupEff(uint8_t sP) override {
      	setup(sP);
      }
};

//------------ Эффект "Шторм" 
// (с) kostyamat 1.12.2020
#define counts  (WIDTH*3)

class MEffectWrain: public EffectFx_comm {
  private:
    static const uint8_t cloudHeight = HEIGHT / 5 + 1;
    float dotPosX[counts];
    float dotPosY[counts];
    float dotChaos;         // сила ветра
    int8_t dotDirect;       // направление ветра 
    byte dotColor[counts];  // цвет капли
    float dotAccel[counts]; // персональное ускорение каждой капли
    byte dotBri[counts];    // яркость капли
    bool clouds = false;
    bool storm = false;
    bool white = false;
    uint8_t _scale=1;
    byte type = 1;
    bool _flash;
    bool randColor = false;
    float windProgress;
	float speedFactor;
    uint8_t *_noise = (uint8_t *)malloc(WIDTH * cloudHeight);
    uint8_t *lightning = (uint8_t *)malloc(WIDTH * HEIGHT);  
    uint32_t timer = 0;
    

    void reload();
    String setDynCtrl(UIControl*_val) override;
    bool Lightning(uint16_t chanse);
    void Clouds(bool flash);

  public:
			MEffectWrain(uint8_t mod = 0){_mod = mod;};
      void load(uint8_t) override;
      bool run() override;		
      void setupEff(uint8_t sP) override {
      	setup(sP);
      }
};

//-------- по мотивам Эффектов Particle System -------------------------
// https://github.com/fuse314/arduino-particle-sys
// https://github.com/giladaya/arduino-particle-sys
// https://www.youtube.com/watch?v=S6novCRlHV8&t=51s
//при попытке вытащить из этой библиотеки только минимально необходимое выяснилось, что там очередной (третий) вариант реализации субпиксельной графики.
//ну его нафиг. лучше будет повторить визуал имеющимися в прошивке средствами.

// ============= ЭФФЕКТ Фея/Источник ===============
// (c) SottNick

class MEffectFairy : public EffectFx_comm {
private:
    float   trackingObjectPosX[trackingOBJECT_MAX_COUNT];
    float   trackingObjectPosY[trackingOBJECT_MAX_COUNT];
    float   trackingObjectSpeedX[trackingOBJECT_MAX_COUNT];
    float   trackingObjectSpeedY[trackingOBJECT_MAX_COUNT];
    float   trackingObjectShift[trackingOBJECT_MAX_COUNT];
    uint8_t trackingObjectHue[trackingOBJECT_MAX_COUNT];
    float   trackingObjectState[trackingOBJECT_MAX_COUNT];
    bool    trackingObjectIsShift[trackingOBJECT_MAX_COUNT];
    uint8_t enlargedObjectNUM;                                       // используемое в эффекте количество объектов

    Boid boids[2];

    uint8_t hue;
    uint8_t hue2;
    uint8_t step;
    uint8_t deltaValue;
    uint8_t deltaHue;
    uint8_t deltaHue2;
    float speedFactor;
    byte type = false;
    byte blur;
    uint8_t _video = 255;
    uint8_t gain;

    void particlesUpdate(uint8_t i);
    void fairyEmit(uint8_t i);
    void fountEmit(uint8_t i);
    bool fairy(CRGB *leds);
    void fount(CRGB *leds);
    //void setscl(const byte _scl) override; // перегрузка для масштаба
    String setDynCtrl(UIControl*_val) override;

public:
			MEffectFairy(uint8_t mod = 0){_mod = mod;};
      void load(uint8_t) override;
      bool run() override;		
      void setupEff(uint8_t sP) override {
      	setup(sP);
      }
};

// ----------- Эффект "Пятнашки"
// https://editor.soulmatelights.com/gallery/1471-puzzles-subpixel
// (c) Stepko 10.12.21
class MEffectPuzzles : public EffectFx_comm {
private:
    byte PSizeX = 4;
    byte PSizeY = 4;
    float Ecols = (WIDTH / PSizeX);
    float Erows = (HEIGHT / PSizeY);
    bool Ca = (WIDTH % PSizeX)? 1 : 0;
    bool Ra = (HEIGHT % PSizeY)? 1 : 0;
    uint8_t PCols = round(Ecols) + Ca;
    uint8_t PRows = round(Erows) + Ra;
    float speedFactor = 0.1;
    std::vector<std::vector<uint8_t>> puzzle;
    byte color;
    byte z_dot[2];
    byte step;
    int8_t move[2];
    float shift[2] = { 0, 0 };
    bool XorY;


    void draw_square(byte x1, byte y1, byte x2, byte y2, byte col);
    void draw_squareF(float x1, float y1, float x2, float y2, byte col);
    void regen();

    String setDynCtrl(UIControl*_val) override;

public:
			MEffectPuzzles(uint8_t mod = 0){_mod = mod;};
      void load(uint8_t) override;
      bool run() override;		
      void setupEff(uint8_t sP) override {
      	setup(sP);
      }
};


// ---------- Эффект "Бульбулятор"
// "Circles" (C) Elliott Kember https://editor.soulmatelights.com/gallery/11
// адаптация и переделка - kostyamat
#define NUMBER_OF_CIRCLES (NUM_LEDS / 16U)
class MEffectCircles : public EffectFx_comm {
private:
    byte color;
    byte count;
    float speedFactor;
    byte _video = 255;
    byte gain;
    class Circle
    {
    public:
        //uint16_t offset;
        int16_t centerX;
        int16_t centerY;
        byte hue;
        float bpm = random(0, 255);

        void move() {
            centerX = random(0, WIDTH-1);
            centerY = random(0, HEIGHT-1);
        }
        
        void reset() {
            centerX = random(0, WIDTH-1);
            centerY = random(0, HEIGHT-1);
            hue = random(0, 255);
        }

        float radius() {
            float radius = EffectMath::fmap(triwave8(bpm), 0, 254, 0, 5); //beatsin16(bpm, 0, 500, 0, offset) / 100.0;
            return radius;
        }
    };

    // Circle circles[NUMBER_OF_CIRCLES] = {};
     LList<Circle *> circles;

    void drawCircle(CRGB *leds, Circle circle);
    String setDynCtrl(UIControl*_val) override;

public:
			MEffectCircles(uint8_t mod = 0){_mod = mod;};
      void load(uint8_t) override;
      bool run() override;		
      void setupEff(uint8_t sP) override {
      	setup(sP);
      }
	    ~MEffectCircles() {
			  while (circles.size()) {
			    Circle * item = circles.shift();
			    delete item;
			  }
			  circles.clear();      	
	    }
};

// ----------- Эффект "Бенгальские Огни"
// (c) stepko https://wokwi.com/arduino/projects/289797125785520649
// 06.02.2021
class MEffectBengalL : public EffectFx_comm {
private:
    #define sparksNum  WIDTH*4

    float sparksPos[2][sparksNum];
    float sparksSpeed[2][sparksNum];
    byte sparksColor[sparksNum];
    float sparksSat[sparksNum];
    float sparksFade[sparksNum];
    uint8_t gPos[2];

    bool centerRun = true;
    byte period = 10;
    byte _x = WIDTH/2;
    byte _y = HEIGHT/2;
    float speedFactor;

    void regen(byte id);
    void phisics(byte id);
    String setDynCtrl(UIControl*_val) override;


public:
			MEffectBengalL(uint8_t mod = 0){_mod = mod;};
      void load(uint8_t) override;
      bool run() override;		
      void setupEff(uint8_t sP) override {
      	setup(sP);
      }
};

// ----------- Эффект "Шары"
// (c) stepko and kostyamat https://wokwi.com/arduino/projects/289839434049782281
// 07.02.2021
class MEffectBalls : public EffectFx_comm {
private:
#if WIDTH >= HEIGHT
    #define ballsAmount WIDTH
#else
    #define ballsAmount HEIGHT
#endif
    float ball[ballsAmount][4]; //0-PosY 1-PosX 2-SpeedY 3-SpeedX
    float radius[ballsAmount];
    bool rrad[ballsAmount];
    byte color[ballsAmount];
    const float radiusMax = (float)ballsAmount /5;

    float speedFactor;
    //void setspd(const byte _spd) override;
    String setDynCtrl(UIControl*_val) override;
public:
			MEffectBalls(uint8_t mod = 0){_mod = mod;};
      void load(uint8_t) override;
      bool run() override;		
      void setupEff(uint8_t sP) override {
      	setup(sP);
      }
};


// ---------- Лабіринт
//remade by stepko
class MEffectMaze : public EffectFx_comm {
private:
    const uint16_t maxSolves = MAZE_WIDTH * MAZE_WIDTH * 5;
    char *maze = (char*)malloc(MAZE_WIDTH * MAZE_HEIGHT * sizeof(char));
    int8_t playerPos[2];
    uint32_t labTimer;
    bool mazeMode = false;
    bool mazeStarted = false;
    uint8_t hue;
    CRGB color;
    CRGB playerColor = CRGB::White;

    bool loadingFlag = true;
    bool gameOverFlag = false;
    bool gameDemo = true;
    bool gamePaused = false;
    bool track = random8(0,2);  // будет ли трек игрока
    uint8_t buttons;

    timerMinim gameTimer = 200;         // Таймер скорости игр

    void newGameMaze();
    void buttonsTickMaze();
    void movePlayer(int8_t nowX, int8_t nowY, int8_t prevX, int8_t prevY);
    void demoMaze();
    bool checkPath(int8_t x, int8_t y);
    void CarveMaze(char *maze, int width, int height, int x, int y);
    void GenerateMaze(char *maze, int width, int height);
    void SolveMaze(char *maze, int width, int height);

    bool checkButtons()
    {
        if (buttons != 4)
            return true;
        return false;
    }

    //String setDynCtrl(UIControl*_val) override;
    void setspd(const byte _spd) override; // перегрузка для скорости
public:
			MEffectMaze(uint8_t mod = 0){_mod = mod;};
      void load(uint8_t) override;

      bool run() override;		
      void setupEff(uint8_t sP) override {
      	setup(sP);
      }

}; 

// --------- Эффект "Вьющийся Цвет"
// (c) Stepko https://wokwi.com/arduino/projects/283705656027906572
class MEffect_Frizzles : public EffectFx_comm {
private:
    float _speed;
    float _scale;
    //String setDynCtrl(UIControl*_val) override;

public:
			MEffect_Frizzles(uint8_t mod = 0){_mod = mod;};
      bool run() override;		
      void setupEff(uint8_t sP) override {
      	setup(sP);
      }
};


// --------- Эффект "Северное Сияние"
// (c) kostyamat 05.02.2021
// идеи подсмотрены тут https://www.reddit.com/r/FastLED/comments/jyly1e/challenge_fastled_sketch_that_fits_entirely_in_a/

// Палитры, специально созданные под этот эффект, огромная благодарность @Stepko
static const TProgmemRGBPalette16 GreenAuroraColors_p FL_PROGMEM ={0x000000, 0x003300, 0x006600, 0x009900, 0x00cc00,0x00ff00, 0x33ff00, 0x66ff00, 0x99ff00,0xccff00, 0xffff00, 0xffcc00, 0xff9900, 0xff6600, 0xff3300, 0xff0000};
static const TProgmemRGBPalette16 BlueAuroraColors_p FL_PROGMEM ={0x000000, 0x000033, 0x000066, 0x000099, 0x0000cc,0x0000ff, 0x3300ff, 0x6600ff, 0x9900ff,0xcc00ff, 0xff00ff, 0xff33ff, 0xff66ff, 0xff99ff, 0xffccff, 0xffffff};
static const TProgmemRGBPalette16 NeonAuroraColors_p FL_PROGMEM ={0x000000, 0x003333, 0x006666, 0x009999, 0x00cccc,0x00ffff, 0x33ffff, 0x66ffff, 0x99ffff,0xccffff, 0xffffff, 0xffccff, 0xff99ff, 0xff66ff, 0xff33ff, 0xff00ff};

class MEffectPolarL : public EffectFx_comm {
private:
    const byte numpalettes = 14;
    unsigned long timer;
    float adjastHeight;
    uint16_t adjScale;
    byte pal;
    uint16_t _scale;
    byte flag;
		byte _speed;

    void palettemap(std::vector<PGMPalette*> &_pals, const uint8_t _val, const uint8_t _min, const uint8_t _max) override;
    void palettesload() override;
    String setDynCtrl(UIControl*_val) override;
public:
			MEffectPolarL(uint8_t mod = 0){_mod = mod;};
      void load(uint8_t) override;
      bool run() override;		
      void setupEff(uint8_t sP) override {
      	setup(sP);
      }
};

// --------- Эффект "Космо-Гонщик"
// (c) Stepko + kostyamat https://editor.soulmatelights.com/my-patterns/655
class MEffectRacer: public EffectFx_comm {
private:
    uint16_t posX = random(0, WIDTH-1)*10;
    uint16_t posY = random(0, HEIGHT-1)*10;
    uint16_t aimX = random(0, WIDTH-1)*10;
    uint16_t aimY = random(0, HEIGHT-1)*10;
		int16_t deltaX, deltaY, signX, signY, error;
    float radius = 0;
    byte hue = millis()>>1; //random(0, 255);
    CRGB color;
    float speedFactor;
    float addRadius;
    float angle;
    byte starPoints = random(3, 7);

    const float _speed = (float)NUM_LEDS / 256; // Нормализация скорости для разных размеров матриц
    const float _addRadius = (float)NUM_LEDS / 4000;   // Нормализация скорости увеличения радиуса круга для разных матриц


    void aimChange();
    void drawStarF(float x, float y, float biggy, float little, int16_t points, float dangle, CRGB color);
    //void setspd(const byte _spd) override;
    String setDynCtrl(UIControl*_val) override;

public:
			MEffectRacer(uint8_t mod = 0){_mod = mod;};
      void load(uint8_t) override;
      bool run() override;		
      void setupEff(uint8_t sP) override {
      	setup(sP);
      }
};

// ----------------- Эффект "Магма"
// (c) Сотнег (SottNick) 2021
// адаптация и доводка до ума - kostyamat
class MEffect_Magma: public EffectFx_comm {
private:
    //uint16_t ff_x;
    float ff_y, ff_z;                         // большие счётчики
    //control magma bursts
    const byte deltaValue = 6U; // 2-12 
    const byte deltaHue = 8U; // высота языков пламени должна уменьшаться не так быстро, как ширина
    const float Gravity = 0.1;
    uint8_t step, ObjectNUM = WIDTH; 
    uint8_t shiftHue[HEIGHT];
    float trackingObjectPosX[enlargedOBJECT_MAX_COUNT];
    float trackingObjectPosY[enlargedOBJECT_MAX_COUNT];
    uint8_t trackingObjectHue[enlargedOBJECT_MAX_COUNT];
    float trackingObjectSpeedX[enlargedOBJECT_MAX_COUNT];
    float trackingObjectShift[enlargedOBJECT_MAX_COUNT];
    float speedFactor;

    void palettesload();
    void regen();
    void LeapersMove_leaper(uint8_t l);
    void LeapersRestart_leaper(uint8_t l);
    String setDynCtrl(UIControl*_val) override;
public:
			MEffect_Magma(uint8_t mod = 0){_mod = mod;};
      void load(uint8_t) override;
      bool run() override;		
      void setupEff(uint8_t sP) override {
      	setup(sP);
      }
};

// --------------------- Эффект "Звездный Десант"
// Starship Troopers https://editor.soulmatelights.com/gallery/839-starship-troopers
// Based on (c) stepko`s codes https://editor.soulmatelights.com/gallery/639-space-ships
// reworked (c) kostyamat (subpixel, shift speed control, etc) 08.04.2021
class MEffectStarShips: public EffectFx_comm {
private:
    byte _scale = 8;
    const byte DIR_CHARGE = 2; // Chance to change direction 1-5
    uint16_t chance = 4096;
	
    byte _dir =1;
    uint8_t _SSfade =15;
	int8_t zF;int8_t zD;
	float speedFactor;
	
	void MoveX(uint8_t am, int8_t amplitude, float shift);
	void MoveY(uint8_t am, int8_t amplitude, float shift);
	void draw(float x, float y, CRGB color);
	String setDynCtrl(UIControl*_val) override;
public:
			MEffectStarShips(uint8_t mod = 0){_mod = mod;};
      void load(uint8_t) override;
      bool run() override;		
      void setupEff(uint8_t sP) override {
      	setup(sP);
      }
};

// ------------- Эффект "Флаги"
// (c) Stepko + kostyamat
// 17.03.21
// https://editor.soulmatelights.com/gallery/739-flags
class MEffectFlags: public EffectFx_comm {
private:
    const float DEVIATOR = 512. / WIDTH;
    float counter;
    uint8_t flag = 0;
    uint8_t _flag;
    uint8_t count;
    uint8_t _speed; // 1 - 16
    const uint8_t CHANGE_FLAG = 30; // >= 10 Autochange

    uint8_t thisVal;
    uint8_t thisMax;
	
	uint8_t mix(uint8_t a1, uint8_t a2, uint8_t l){
	return ((a1*l)+(a2*(255-l)))/255;}

    //Germany
    void germany(uint8_t i, uint8_t j)
    {
            EffectMath::getPixel(i, j) += 
            (j < thisMax - HEIGHT / 4) ? CHSV(68, 255, thisVal) : (j < thisMax + HEIGHT / 4) ? CHSV(0, 255, thisVal)
            : CHSV(0, 0, thisVal / 2.5);
    }

    //Ukraine
    void ukraine(uint8_t i, uint8_t j)
    {
            EffectMath::getPixel(i, j) += 
            (j < thisMax) ? CHSV(50, 255, thisVal) : CHSV(150, 255, thisVal);
    }

    //Belarus
    void belarus(uint8_t i, uint8_t j)
    {
            EffectMath::getPixel(i, j) += 
            (j < thisMax - HEIGHT / 4) ? CHSV(0, 0, thisVal) : (j < thisMax + HEIGHT / 4) ? CHSV(0, 224, thisVal)
            : CHSV(0, 0, thisVal);
    }

    //Poland
    void poland(uint8_t i, uint8_t j)
    {
            EffectMath::getPixel(i, j) += 
            (j < thisMax + 1) ? CHSV(248, 214, (float)thisVal * 0.83) : CHSV(25, 3, (float)thisVal * 0.91);
    }

    //The USA
    void usa(uint8_t i, uint8_t j)
    {
            EffectMath::getPixel(i, j) +=
            ((i <= WIDTH / 2) && (j + thisMax > HEIGHT - 1 + HEIGHT / 16)) ? 
            ((i % 2 && ((int)j - HEIGHT / 16 + thisMax) % 2) ? 
            CHSV(160, 0, thisVal) : CHSV(160, 255, thisVal)) 
            : ((j + 1 + thisMax) % 6 < 3 ? CHSV(0, 0, thisVal) : CHSV(0, 255, thisVal));
    }

    //Italy
    void italy(uint8_t i, uint8_t j)
    {
            EffectMath::getPixel(i, j) += 
            (i < WIDTH / 3) ? CHSV(90, 255, thisVal) : (i < WIDTH - 1 - WIDTH / 3) ? CHSV(0, 0, thisVal)
            : CHSV(0, 255, thisVal);
    }

    //France
    void france(uint8_t i, uint8_t j)
    {
            EffectMath::getPixel(i, j) += 
            (i < WIDTH / 3) ? CHSV(160, 255, thisVal) : (i < WIDTH - 1 - WIDTH / 3) ? CHSV(0, 0, thisVal)
            : CHSV(0, 255, thisVal);
    }

    //UK
    void uk(uint8_t i, uint8_t j)
    {
            EffectMath::getPixel(i, j) += 
            (
                (
                    (i > WIDTH / 2 + 1 || i < WIDTH / 2 - 2) && ((i - (int)(j + thisMax - (HEIGHT * 2 - WIDTH) / 2) > -2) && (i - (j + thisMax - (HEIGHT * 2 - WIDTH) / 2) < 2))
                )
                    ||
                (
                    (i > WIDTH / 2 + 1 || i < WIDTH / 2 - 2) && ( (((int)WIDTH - 1 - i - ((int)j + thisMax - (int)(HEIGHT * 2 - WIDTH) / 2) > -2) && (WIDTH - 1 - i - (int)(j + thisMax - (HEIGHT * 2 - WIDTH) / 2) < 2)) )
                )
            || 
            (WIDTH / 2 - i == 0) || (WIDTH / 2 - 1 - i == 0) 
            || 
            ((HEIGHT - (j + thisMax)) == 0) || ((HEIGHT - 1 - (j + thisMax)) == 0)) ? 
            CHSV(0, 255, thisVal) 
            : 
            (((i - (int)(j + thisMax - (HEIGHT * 2 - WIDTH) / 2) > -4) 
            && (i - (j + thisMax - (HEIGHT * 2 - WIDTH) / 2) < 4)) 
            || 
            (((int)WIDTH - 1 - i - (int)(j + thisMax - (HEIGHT * 2 - WIDTH) / 2) > -4) 
            && (WIDTH - 1 - i - (int)(j + thisMax - (HEIGHT * 2 - WIDTH) / 2) < 4)) 
            || (WIDTH / 2 + 1 - i == 0) || (WIDTH / 2 - 2 - i == 0) 
            || (HEIGHT + 1 - (j + thisMax) == 0) || (HEIGHT - 2 - (int)(j + thisMax) == 0)) ? 
            CHSV(0, 0, thisVal) : CHSV(150, 255, thisVal);
    }

    //Spain
    void spain(uint8_t i, uint8_t j)
    {
            EffectMath::getPixel(i, j) += 
            (j < thisMax - HEIGHT / 3) ? 
            CHSV(250, 224, (float)thisVal * 0.68) : (j < thisMax + HEIGHT / 3) ? CHSV(64, 255, (float)thisVal * 0.98)
            : CHSV(250, 224, (float)thisVal * 0.68);
    }


    void changeFlags();
    String setDynCtrl(UIControl*_val) override;
public:
			MEffectFlags(uint8_t mod = 0){_mod = mod;};
      bool run() override;		
      void setupEff(uint8_t sP) override {
      	setup(sP);
      }
};


// ============= Ефект Кольорові драже ===============
// (c)stepko
class MEffectPile : public EffectFx_comm {
private:
    uint8_t pcnt = 0U, _scale;
    String setDynCtrl(UIControl*_val) override;
    void changeFrame();
    byte F[2][WIDTH][HEIGHT];
    int shift;
public:
			MEffectPile(uint8_t mod = 0){_mod = mod;};
      void load(uint8_t) override;
      bool run() override;		
      void setupEff(uint8_t sP) override {
      	setup(sP);
      }
};

// ============= Эффект ДНК ===============
// (c) Stepko
// https://editor.soulmatelights.com/gallery/1520-dna
//по мотивам визуала эффекта by Yaroslaw Turbin
//https://vk.com/ldirko программный код которого он запретил брать
class MEffectDNA : public EffectFx_comm {
private:
    float a = (256.0 / (float)WIDTH);
    float t = 0.0;
    float speedFactor = 0.5;
    bool flag = true; 
    bool bals = false;
    uint8_t type = 0, _type = 1, _scale = 16;
    
    String setDynCtrl(UIControl*_val) override;
public:
			MEffectDNA(uint8_t mod = 0){_mod = mod;};
      void load(uint8_t) override;
      bool run() override;		
      void setupEff(uint8_t sP) override {
      	setup(sP);
      }
};

// ----------- Эффект "Дым"
// based on cod by @Stepko (c) 23/12/2021
class MEffectSmoker : public EffectFx_comm {
private:
		uint8_t _cMod = 0;
		hsvloopManage _hsvLoop;
    byte color, saturation;
    byte _scale = 30, glitch;
		float speedFactor;
    float t;

    String setDynCtrl(UIControl*_val) override;

public:
			MEffectSmoker(uint8_t mod = 0){_mod = mod;};
      void load(uint8_t) override;
      bool run() override;		
      void setupEff(uint8_t sP) override {
      	setup(sP);
      }
};

// ----------- Эффект "Мираж"
// based on cod by @Stepko (c) 23/12/2021
class MEffectMirage : public EffectFx_comm {
private:
    const float div = 10.;
    const uint16_t width = (WIDTH - 1) * div, height = HEIGHT * div;
    uint16_t _speed;
    byte color;
    bool colorShift = false;
    byte buff[WIDTH + 2][HEIGHT + 2];
    bool a = true;
		float speedFactor;


    String setDynCtrl(UIControl*_val) override;
    void drawDot(float x, float y, byte a);
    void blur();

public:
			MEffectMirage(uint8_t mod = 0){_mod = mod;};
      void load(uint8_t) override;
      bool run() override;		
      void setupEff(uint8_t sP) override {
      	setup(sP);
      }
};

// -------------------- Эффект "Акварель"
// (c) kostyamat 26.12.2021
// https://editor.soulmatelights.com/gallery/1587-oil
#define BLOT_SIZE WIDTH/2
class MEffectWcolor : public EffectFx_comm {
private:
    float speedFactor;
    uint8_t bCounts = 1;
    uint8_t blur;
    bool mode = false;
    float t;

    class Blot : public EffectFx_comm  {
    private:
        byte hue, sat;
        float bri;
        int x0, y0;
        float x[BLOT_SIZE]; 
        float y[BLOT_SIZE];

    public:

        void appendXY(float nx, float ny) {
            for (byte i = 0; i < BLOT_SIZE; i++) {
                x[i] += nx;
                y[i] += ny; 
            }
        }
        

        void reset(byte num, byte Counts) {  
            x0 = random(-5, WIDTH - 5);
            float y0 = EffectMath::randomf(-1, HEIGHT+1);
            uint8_t dy;
            for (uint8_t i = 0; i < BLOT_SIZE; i++) {
                bool f = random(0,2);
                dy = random(0, 2); 
                x[i] = x0 + i;
                if (f)
                y[i] = float((i ? y[i-1] : y0) + dy);
                else 
                y[i] = float((i ? y[i-1] : y0) - dy);
            }
            hue = random(0, 256);
            sat = random(160, 256);
            bri = random(128, 256);
            
        }

        double getY() {
            double result = y[0];
            for (uint8_t i = 1; i < BLOT_SIZE; i++) {
                if (y[i] > result) result = y[i];
            }
            return result;
        }

        void drawing() {
            for (uint8_t i = 0; i < BLOT_SIZE; i++) {
                byte bright = constrain((float)bri / (float)HEIGHT * (y[i] + HEIGHT - y0), 32, 255);
                uint8_t globalBri = LAMPPTRGET()->get_globalBrightness(0, brightness);
								if (bright > globalBri) bright = globalBri;      
                if (y[i] > -0.1)
                    EffectMath::drawPixelXYF(x[i], y[i], CHSV(hue, sat, bright), 0);
            }
        }

    };


    std::vector<Blot> blots;

    String setDynCtrl(UIControl*_val) override;

public:
			MEffectWcolor(uint8_t mod = 0){_mod = mod;};
      void load(uint8_t) override;
      bool run() override;	
      void setupEff(uint8_t sP) override {
      	setup(sP);
      }
};

	/*
	 * Aurora: https://github.com/pixelmatix/aurora
	 * https://github.com/pixelmatix/aurora/blob/sm3.0-64x64/PatternSpiro.h
	 * Copyright (c) 2014 Jason Coon
	 * Неполная адаптация SottNick
	 */
/*
	class MEffectSpiro : public EffectFx_comm {
	private:
	  const uint8_t spiroradiusx = WIDTH / 4;
	  const uint8_t spiroradiusy = HEIGHT / 4;

	  const uint8_t spirocenterX = WIDTH / 2;
	  const uint8_t spirocenterY = HEIGHT / 2;

	  const uint8_t spirominx = spirocenterX - spiroradiusx;
	  const uint8_t spiromaxx = spirocenterX + spiroradiusx;// + 1;
	  const uint8_t spirominy = spirocenterY - spiroradiusy;
	  const uint8_t spiromaxy = spirocenterY + spiroradiusy; // + 1;

	  bool spiroincrement = false;
	  bool spirohandledChange = false;
	  float spirohueoffset = 0;
	  uint8_t spirocount = 1;
	  float spirotheta1 = 0;
	  float spirotheta2 = 0;
	  uint8_t internalCnt = 0;

	  bool spiroRoutine(CRGB *leds);
	  String setDynCtrl(UIControl*_val) override;
	public:
			MEffectSpiro(uint8_t mod = 0){_mod = mod;};
      void load(uint8_t) override;
      bool run() override;	
      void setupEff(uint8_t sP) override {
	setup(sP);
      }
	};
*/
	class MEffectSpiro : public EffectFx_comm {
	private:
	  const float spirocenterX = WIDTH /2-0.5; //(!(WIDTH & 1) ? (WIDTH -1) : WIDTH) / 2;
	  const float spirocenterY = HEIGHT /2-1; //(!(HEIGHT & 1) ? (HEIGHT-1) : HEIGHT) / 2;

		byte AM = 1;
		bool incenter;
		float Angle;
		bool change = false;
		float speedFactor;

	  String setDynCtrl(UIControl*_val) override;

	public:
			MEffectSpiro(uint8_t mod = 0){_mod = mod;};
      void load(uint8_t) override;
      bool run() override;	
      void setupEff(uint8_t sP) override {
      	setup(sP);
      }
	};

	class MEffectFlower : public EffectFx_comm {
		private:
			uint8_t effTimer;
			float ZVoffset = 0;
			const float COLS_HALF = WIDTH * .5;
			const float ROWS_HALF = HEIGHT * .5;
			int16_t ZVcalcRadius(int16_t x, int16_t y);
			int16_t ZVcalcDist(uint8_t x, uint8_t y, float center_x, float center_y);
			String setDynCtrl(UIControl*_val) override;
		public:
			MEffectFlower(uint8_t mod = 0){_mod = mod;};
      void load(uint8_t) override;
      bool run() override;	
      void setupEff(uint8_t sP) override {
      	setup(sP);
      }
	};

	//RadialFire
	// (c) Stepko and Sutaburosu https://editor.soulmatelights.com/gallery/1570-radialfire
	//23/12/21
	class MEffectRadialFire : public EffectFx_comm {
	private:
	    const int8_t MIN_MAX = max(WIDTH, HEIGHT);
	    const int8_t C_X = (WIDTH / 2);
		const int8_t C_Y = (HEIGHT / 2);
	    std::vector<std::vector<int>> XY_angle;
	    std::vector<std::vector<byte>> XY_radius;
	    float t;
	    float speedFactor;
	    uint8_t _scale;
	    bool mode = false;


	    String setDynCtrl(UIControl*_val) override;
	    // void palettesload();

	public:
			MEffectRadialFire(uint8_t mod = 0){_mod = mod;};
      void load(uint8_t) override;
      bool run() override;	
      void setupEff(uint8_t sP) override {
      	setup(sP);
      }
	};

	class MEffectSplashBals : public EffectFx_comm {
	private:
	    uint8_t count = 3;
	    uint8_t hue;
	    const uint8_t DEV = 5;
	    const float R = (float)NUM_LEDS/160;

	    float x[6];
	    float y[6];
	    uint8_t iniX[6];
	    uint8_t iniY[6];

	    float speedFactor;


	    float dist(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2) {
	        float dist;
	        int a = y2 - y1;
	        int b = x2 - x1;
	        a *= a;
	        b *= b;
	        a += b;
	        dist = EffectMath::sqrt(a);
	        return dist;
	    };
	    

	    String setDynCtrl(UIControl*_val) override;

	public:
			MEffectSplashBals(uint8_t mod = 0){_mod = mod;};
      void load(uint8_t) override;
      void loadDefault() override;
      bool run() override;	
      void setupEff(uint8_t sP) override {
      	setup(sP);
      }
	};	


	class MEffectRadialNuclearNoise : public EffectFx_comm {
	private:
			bool setupm = 1;
			const uint8_t C_X = WIDTH / 2;
			const uint8_t C_Y = HEIGHT / 2;
			const uint8_t mapp = 255 / WIDTH;
	    float speedFactor;
	    String setDynCtrl(UIControl*_val) override;
	    // void palettesload();
	public:
			MEffectRadialNuclearNoise(uint8_t mod = 0){_mod = mod;};
      void load(uint8_t) override;
      bool run() override;	
      void setupEff(uint8_t sP) override {
      	setup(sP);
      }
	};	
#endif

#endif // MATRIXEFFECT_H