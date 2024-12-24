#ifndef __CONFIG_H
	#define __CONFIG_H

	#if defined __has_include
	#  if __has_include("user_config.h")
	#    include "user_config.h" 
	#  endif
	#endif

	#define SF(s) __SFSTR(s)
	#define __SFSTR(s) #s

	// #define FASTLED_INTERRUPT_RETRY_COUNT   (0)                
	// #define FASTLED_ESP8266_RAW_PIN_ORDER                       
	
	// #define LAMP_PIN 			5
	#define CONNECTION_ANGLE	(0U)    
	#define STRIP_DIRECTION		(3U)   

	#include <FastLED.h>



	#ifndef SEGMENTS
		#define SEGMENTS              	(1U)                          
	#endif

	//#define VERTGAUGE             (1U)                       
	// #ifndef NUMHOLD_TIME
	// 	#define NUMHOLD_TIME          	(3000U) 
	// #endif

	#ifndef BRIGHTNESS
		#define BRIGHTNESS            	(255U)
	#endif
	#ifndef CURRENT_LIMIT
		#define CURRENT_LIMIT         	(2000U)
	#endif

	#ifndef FADE_STEPTIME
		#define FADE_STEPTIME         	(50U)
	#endif
	#ifndef FADE_TIME
		#define FADE_TIME             	(2000U)
	#endif
	#ifndef FADE_MININCREMENT
		#define FADE_MININCREMENT     	(3U)
	#endif
	#ifndef FADE_MINCHANGEBRT
		#define FADE_MINCHANGEBRT     	(30U)
	#endif

	#ifndef MAX_FPS
		#define MAX_FPS					(60U)
	#endif
	#define EFFECTS_RUN_TIMER       (uint16_t)(1000 / MAX_FPS)

	#ifndef DEFAULT_DEMO_TIMER
		  #define DEFAULT_DEMO_TIMER  (60U)
	#endif




	/*
	#ifndef BTN_PIN
	#define BTN_PIN               (4U)                      
	//#define BTN_PIN               (0U)                       
	#endif

	#ifndef MOSFET_PIN
	#define MOSFET_PIN            (5U)                         
	#endif
	#ifndef ALARM_PIN
	#define ALARM_PIN             (16U)                         
	#endif
	#ifndef MOSFET_LEVEL
	#define MOSFET_LEVEL          (HIGH)                       
	#endif
	#ifndef ALARM_LEVEL
	#define ALARM_LEVEL           (HIGH)                       
	#endif
	*/

	#ifndef COLOR_ORDER
		#define COLOR_ORDER           (GRB)
	#endif
	#ifndef DEFAULT_TEXT_SPEED
		#define DEFAULT_TEXT_SPEED    (100U)
	#endif
	
	#ifdef ALS_LAMPMATRIX

		#ifndef TEXT_OFFSET
			#define TEXT_OFFSET           (4U)
		#endif
		#ifndef LET_WIDTH
			#define LET_WIDTH             (5U)
		#endif
		#ifndef LET_HEIGHT
			#define LET_HEIGHT            (8U)
		#endif
		#ifndef LET_SPACE
			#define LET_SPACE             (1U)
		#endif
		#ifndef LETTER_COLOR
			#define LETTER_COLOR          (CRGB::White)
		#endif
		#ifndef FADETOBLACKVALUE
			#define FADETOBLACKVALUE      (222U)
		#endif


		#ifndef WIDTH
			#define WIDTH                 (16)
		#endif
		#ifndef HEIGHT
			#define HEIGHT                (16)
		#endif



		#ifndef MATRIX_TYPE
			#define MATRIX_TYPE           (0U)
		#endif
		#ifndef CONNECTION_ANGLE
			#define CONNECTION_ANGLE      (1U)
		#endif
		#ifndef STRIP_DIRECTION
			#define STRIP_DIRECTION       (3U) 
		#endif

		#ifndef NUM_LEDS
			#define NUM_LEDS              (uint16_t)(WIDTH * HEIGHT)
		#endif


		#if (CONNECTION_ANGLE == 0 && STRIP_DIRECTION == 0)
			#define _WIDTH WIDTH
			#define THIS_X (MIRR_V ? (WIDTH - x - 1) : x)
			#define THIS_Y (MIRR_H ? (HEIGHT - y - 1) : y)

		#elif (CONNECTION_ANGLE == 0 && STRIP_DIRECTION == 1)
			#define _WIDTH HEIGHT
			#define ROTATED_MATRIX
			#define THIS_X (MIRR_V ? (HEIGHT - y - 1) : y)
			#define THIS_Y (MIRR_H ? (WIDTH - x - 1) : x)

		#elif (CONNECTION_ANGLE == 1 && STRIP_DIRECTION == 0)
			#define _WIDTH WIDTH
			#define THIS_X (MIRR_V ? (WIDTH - x - 1) : x)
			#define THIS_Y (MIRR_H ?  y : (HEIGHT - y - 1))

		#elif (CONNECTION_ANGLE == 1 && STRIP_DIRECTION == 3)
			#define _WIDTH HEIGHT
			#define ROTATED_MATRIX
			#define THIS_X (MIRR_V ? y : (HEIGHT - y - 1))
			#define THIS_Y (MIRR_H ? (WIDTH - x - 1) : x)

		#elif (CONNECTION_ANGLE == 2 && STRIP_DIRECTION == 2)
			#define _WIDTH WIDTH
			#define THIS_X (MIRR_V ?  x : (WIDTH - x - 1))
			#define THIS_Y (MIRR_H ? y : (HEIGHT - y - 1))

		#elif (CONNECTION_ANGLE == 2 && STRIP_DIRECTION == 3)
			#define _WIDTH HEIGHT
			#define ROTATED_MATRIX
			#define THIS_X (MIRR_V ? y : (HEIGHT - y - 1))
			#define THIS_Y (MIRR_H ?  x : (WIDTH - x - 1))

		#elif (CONNECTION_ANGLE == 3 && STRIP_DIRECTION == 2)
			#define _WIDTH WIDTH
			#define THIS_X (MIRR_V ?  x : (WIDTH - x - 1))
			#define THIS_Y (MIRR_H ? (HEIGHT - y - 1) : y)

		#elif (CONNECTION_ANGLE == 3 && STRIP_DIRECTION == 1)
			#define _WIDTH HEIGHT
			#define ROTATED_MATRIX
			#define THIS_X (MIRR_V ? (HEIGHT - y - 1) : y)
			#define THIS_Y (MIRR_H ?  x : (WIDTH - x - 1))

		#else
			#define _WIDTH WIDTH
			#define THIS_X x
			#define THIS_Y y
		#endif

	#define M_HEIGHT HEIGHT + !(HEIGHT % 2)
	#define M_WIDTH WIDTH + !(WIDTH % 2)
	#define M_SHIFT_X !(WIDTH % 2)
	#define M_SHIFT_Y !(HEIGHT % 2)
	#endif

#ifdef ALS_LAMPMATRIX

#if  576U < (WIDTH * HEIGHT)
	#define BIGMATRIX
#endif

// Еще одно представление высоты и ширины матрицы, часто встречаеться последнее время
#define LED_ROWS HEIGHT
#define LED_COLS WIDTH

// И еще одно представление высоты и ширины матрицы, часто встречаеться последнее время
#define NUM_ROWS HEIGHT
#define NUM_COLS WIDTH

// ==== Константы для эффектов ====

static const uint8_t gamma_exp[] PROGMEM = {
0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   1,   1,
1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
1,   2,   2,   2,   2,   2,   2,   2,   2,   2,   3,   3,   3,   3,   3,
4,   4,   4,   4,   4,   5,   5,   5,   5,   5,   6,   6,   6,   7,   7,
7,   7,   8,   8,   8,   9,   9,   9,   10,  10,  10,  11,  11,  12,  12,
12,  13,  13,  14,  14,  14,  15,  15,  16,  16,  17,  17,  18,  18,  19,
19,  20,  20,  21,  21,  22,  23,  23,  24,  24,  25,  26,  26,  27,  28,
28,  29,  30,  30,  31,  32,  32,  33,  34,  35,  35,  36,  37,  38,  39,
39,  40,  41,  42,  43,  44,  44,  45,  46,  47,  48,  49,  50,  51,  52,
53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  63,  64,  65,  66,  67,
68,  70,  71,  72,  73,  74,  75,  77,  78,  79,  80,  82,  83,  84,  85,
87,  89,  91,  92,  93,  95,  96,  98,  99,  100, 101, 102, 105, 106, 108,
109, 111, 112, 114, 115, 117, 118, 120, 121, 123, 125, 126, 128, 130, 131,
133, 135, 136, 138, 140, 142, 143, 145, 147, 149, 151, 152, 154, 156, 158,
160, 162, 164, 165, 167, 169, 171, 173, 175, 177, 179, 181, 183, 185, 187,
190, 192, 194, 196, 198, 200, 202, 204, 207, 209, 211, 213, 216, 218, 220,
222, 225, 227, 229, 232, 234, 236, 239, 241, 244, 246, 249, 251, 253, 254, 255};

// ==== Константы для эффектов ====

// Эффекты на базе "3D Noise"
#define NOISE_SCALE_AMP  (58.)    // амплификатор шкалы (влияет на машстаб "пятен" эффекта, большие пятна выглядят красивее чем куча мелких)
#define NOISE_SCALE_ADD  (8.)     // корректор шкалы


#define BALLS_AMOUNT           (7U)                 // максимальное количество "шариков"
#define LIGHTERS_AM            (32U)                // светлячки
#define NUM_LAYERS             (1U)                 // The coordinates for 3 16-bit noise spaces.
#define NUM_LAYERS2            (2U)                 // The coordinates for 3 16-bit noise spaces.
#define AVAILABLE_BOID_COUNT   (10U)                // стая, кол-во птиц

// ***** RAINBOW COMET / РАДУЖНАЯ КОМЕТА *****
#define e_com_TAILSPEED             (500)         // скорость смещения хвоста
#define e_com_BLUR                  (12U)         // размытие хвоста
#define e_com_3DCOLORSPEED          (3U)          // скорость случайного изменения цвета (0й - режим)

// ------------- светлячки со шлейфом -------------
//#define BALLS_AMOUNT          (7U)                          // максимальное количество "шариков"
#define CLEAR_PATH            (1U)                          // очищать путь
#define TRACK_STEP            (70U)                         // длина хвоста шарика (чем больше цифра, тем хвост короче)

// --------------------------- эффект мячики ----------------------
#define bballsMaxNUM_BALLS     (WIDTH-1)             // максимальное количество мячиков прикручено при адаптации для бегунка Масштаб
#define bballsGRAVITY          (-9.7f)               // Downward (negative) acceleration of gravity in m/s^2
#define bballsH0               (2)                   // Starting height, in meters, of the ball (strip length)
#define bballsVImpact0         (EffectMath::sqrt(-2 * bballsGRAVITY * bballsH0))

// ------------- метель -------------
#define SNOW_DENSE            (60U)                         // плотность снега
#define SNOW_TAIL_STEP        (100U)                        // длина хвоста
#define SNOW_SATURATION       (0U)                          // насыщенность (от 0 до 255)

// ------------- звездопад -------------
#define STAR_DENSE            (60U)                         // плотность комет
#define STAR_TAIL_STEP        (100U)                        // длина хвоста кометы
#define STAR_SATURATION       (150U)                        // насыщенность кометы (от 0 до 255)

// ============= DRIFT / ДРИФТ ===============
// v1.0 - Updating for GuverLamp v1.7 by SottNick 12.04.2020
// v1.1 - +dither, +phase shifting by PalPalych 12.04.2020
// https://github.com/pixelmatix/aurora/blob/master/PatternIncrementalDrift.h
#define CENTER_max  max(WIDTH / 2, HEIGHT / 2) // Наибольшее значение центра
#define WIDTH_steps  256U / WIDTH   // диапазон значений приходящихся на 1 пиксель ширины матрицы
#define HEIGHT_steps 256U / HEIGHT // диапазон значений приходящихся на 1 пиксель высоты матрицы

#define RANDOM_COLOR          (1U)                          // случайный цвет при отскоке

template <class T>
class Vector2 {
public:
		T x, y;

		Vector2() :x(0), y(0) {}
		Vector2(T x, T y) : x(x), y(y) {}
		Vector2(const Vector2& v) : x(v.x), y(v.y) {}

		Vector2& operator=(const Vector2& v) {
				x = v.x;
				y = v.y;
				return *this;
		}

		bool isEmpty() {
				return x == 0 && y == 0;
		}

		bool operator==(Vector2& v) {
				return x == v.x && y == v.y;
		}

		bool operator!=(Vector2& v) {
				return !(x == y);
		}

		Vector2 operator+(Vector2& v) {
				return Vector2(x + v.x, y + v.y);
		}
		Vector2 operator-(Vector2& v) {
				return Vector2(x - v.x, y - v.y);
		}

		Vector2& operator+=(Vector2& v) {
				x += v.x;
				y += v.y;
				return *this;
		}
		Vector2& operator-=(Vector2& v) {
				x -= v.x;
				y -= v.y;
				return *this;
		}

		Vector2 operator+(double s) {
				return Vector2(x + s, y + s);
		}
		Vector2 operator-(double s) {
				return Vector2(x - s, y - s);
		}
		Vector2 operator*(double s) {
				return Vector2(x * s, y * s);
		}
		Vector2 operator/(double s) {
				return Vector2(x / s, y / s);
		}

		Vector2& operator+=(double s) {
				x += s;
				y += s;
				return *this;
		}
		Vector2& operator-=(double s) {
				x -= s;
				y -= s;
				return *this;
		}
		Vector2& operator*=(double s) {
				x *= s;
				y *= s;
				return *this;
		}
		Vector2& operator/=(double s) {
				x /= s;
				y /= s;
				return *this;
		}

		void set(T x, T y) {
				this->x = x;
				this->y = y;
		}

		void rotate(double deg) {
				double theta = deg / 180.0 * M_PI;
				double c = cos(theta);
				double s = sin(theta);
				double tx = x * c - y * s;
				double ty = x * s + y * c;
				x = tx;
				y = ty;
		}

		Vector2& normalize() {
				if (length() == 0) return *this;
				*this *= (1.0 / length());
				return *this;
		}

		float dist(Vector2 v) const {
				Vector2 d(v.x - x, v.y - y);
				return d.length();
		}
		float length() const {
				return sqrt(x * x + y * y);
		}

		float mag() const {
				return length();
		}

		float magSq() {
				return (x * x + y * y);
		}

		void truncate(double length) {
				double angle = atan2f(y, x);
				x = length * cos(angle);
				y = length * sin(angle);
		}

		Vector2 ortho() const {
				return Vector2(y, -x);
		}

		static float dot(Vector2 v1, Vector2 v2) {
				return v1.x * v2.x + v1.y * v2.y;
		}
		static float cross(Vector2 v1, Vector2 v2) {
				return (v1.x * v2.y) - (v1.y * v2.x);
		}

		void limit(float max) {
				if (magSq() > max*max) {
						normalize();
						*this *= max;
				}
		}
};

typedef Vector2<float> PVector;

// Flocking
// Daniel Shiffman <http://www.shiffman.net>
// The Nature of Code, Spring 2009

// Boid class
// Methods for Separation, Cohesion, Alignment added

class Boid {
	public:

		PVector location;
		PVector velocity;
		PVector acceleration;
		float maxforce;    // Maximum steering force
		float maxspeed;    // Maximum speed

		float desiredseparation = 4;
		float neighbordist = 8;
		byte colorIndex = 0;
		float mass;

		boolean enabled = true;

		Boid() {}

		Boid(float x, float y) {
			acceleration = PVector(0, 0);
			velocity = PVector(randomf(), randomf());
			location = PVector(x, y);
			maxspeed = 1.5;
			maxforce = 0.05;
		}

		static float randomf() {
			return mapfloat(random(0, 255), 0, 255, -.5, .5);
		}

		static float mapfloat(float x, float in_min, float in_max, float out_min, float out_max) {
			return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
		}

		void run(Boid boids [], uint8_t boidCount) {
			flock(boids, boidCount);
			update();
			// wrapAroundBorders();
			// render();
		}

		// Method to update location
		void update() {
			// Update velocity
			velocity += acceleration;
			// Limit speed
			velocity.limit(maxspeed);
			location += velocity;
			// Reset acceleration to 0 each cycle
			acceleration *= 0;
		}

		void applyForce(PVector force) {
			// We could add mass here if we want A = F / M
			acceleration += force;
		}

		void repelForce(PVector obstacle, float radius) {
			//Force that drives boid away from obstacle.

			PVector futPos = location + velocity; //Calculate future position for more effective behavior.
			PVector dist = obstacle - futPos;
			float d = dist.mag();

			if (d <= radius) {
				PVector repelVec = location - obstacle;
				repelVec.normalize();
				if (d != 0) { //Don't divide by zero.
					// float scale = 1.0 / d; //The closer to the obstacle, the stronger the force.
					repelVec.normalize();
					repelVec *= (maxforce * 7);
					if (repelVec.mag() < 0) { //Don't let the boids turn around to avoid the obstacle.
						repelVec.y = 0;
					}
				}
				applyForce(repelVec);
			}
		}

		// We accumulate a new acceleration each time based on three rules
		void flock(Boid boids [], uint8_t boidCount) {
			PVector sep = separate(boids, boidCount);   // Separation
			PVector ali = align(boids, boidCount);      // Alignment
			PVector coh = cohesion(boids, boidCount);   // Cohesion
			// Arbitrarily weight these forces
			sep *= 1.5;
			ali *= 1.0;
			coh *= 1.0;
			// Add the force vectors to acceleration
			applyForce(sep);
			applyForce(ali);
			applyForce(coh);
		}

		// Separation
		// Method checks for nearby boids and steers away
		PVector separate(Boid boids [], uint8_t boidCount) {
			PVector steer = PVector(0, 0);
			int count = 0;
			// For every boid in the system, check if it's too close
			for (int i = 0; i < boidCount; i++) {
				Boid other = boids[i];
				if (!other.enabled)
					continue;
				float d = location.dist(other.location);
				// If the distance is greater than 0 and less than an arbitrary amount (0 when you are yourself)
				if ((d > 0) && (d < desiredseparation)) {
					// Calculate vector pointing away from neighbor
					PVector diff = location - other.location;
					diff.normalize();
					diff /= d;        // Weight by distance
					steer += diff;
					count++;            // Keep track of how many
				}
			}
			// Average -- divide by how many
			if (count > 0) {
				steer /= (float) count;
			}

			// As long as the vector is greater than 0
			if (steer.mag() > 0) {
				// Implement Reynolds: Steering = Desired - Velocity
				steer.normalize();
				steer *= maxspeed;
				steer -= velocity;
				steer.limit(maxforce);
			}
			return steer;
		}

		// Alignment
		// For every nearby boid in the system, calculate the average velocity
		PVector align(Boid boids [], uint8_t boidCount) {
			PVector sum = PVector(0, 0);
			int count = 0;
			for (int i = 0; i < boidCount; i++) {
				Boid other = boids[i];
				if (!other.enabled)
					continue;
				float d = location.dist(other.location);
				if ((d > 0) && (d < neighbordist)) {
					sum += other.velocity;
					count++;
				}
			}
			if (count > 0) {
				sum /= (float) count;
				sum.normalize();
				sum *= maxspeed;
				PVector steer = sum - velocity;
				steer.limit(maxforce);
				return steer;
			}
			else {
				return PVector(0, 0);
			}
		}

		// Cohesion
		// For the average location (i.e. center) of all nearby boids, calculate steering vector towards that location
		PVector cohesion(Boid boids [], uint8_t boidCount) {
			PVector sum = PVector(0, 0);   // Start with empty vector to accumulate all locations
			int count = 0;
			for (int i = 0; i < boidCount; i++) {
				Boid other = boids[i];
				if (!other.enabled)
					continue;
				float d = location.dist(other.location);
				if ((d > 0) && (d < neighbordist)) {
					sum += other.location; // Add location
					count++;
				}
			}
			if (count > 0) {
				sum /= count;
				return seek(sum);  // Steer towards the location
			}
			else {
				return PVector(0, 0);
			}
		}

		// A method that calculates and applies a steering force towards a target
		// STEER = DESIRED MINUS VELOCITY
		PVector seek(PVector target) {
			PVector desired = target - location;  // A vector pointing from the location to the target
			// Normalize desired and scale to maximum speed
			desired.normalize();
			desired *= maxspeed;
			// Steering = Desired minus Velocity
			PVector steer = desired - velocity;
			steer.limit(maxforce);  // Limit to maximum steering force
			return steer;
		}

		// A method that calculates a steering force towards a target
		// STEER = DESIRED MINUS VELOCITY
		void arrive(PVector target) {
			PVector desired = target - location;  // A vector pointing from the location to the target
			float d = desired.mag();
			// Normalize desired and scale with arbitrary damping within 100 pixels
			desired.normalize();
			if (d < 4) {
				float m = map(d, 0, 100, 0, maxspeed);
				desired *= m;
			}
			else {
				desired *= maxspeed;
			}

			// Steering = Desired minus Velocity
			PVector steer = desired - velocity;
			steer.limit(maxforce);  // Limit to maximum steering force
			applyForce(steer);
			//Serial.println(d);
		}

		void wrapAroundBorders() {
			if (location.x < 0) location.x = WIDTH - 1;
			if (location.y < 0) location.y = HEIGHT - 1;
			if (location.x >= WIDTH) location.x = 0;
			if (location.y >= HEIGHT) location.y = 0;
		}

		void avoidBorders() {
			PVector desired = velocity;

			if (location.x < 8) desired = PVector(maxspeed, velocity.y);
			if (location.x >= WIDTH - 8) desired = PVector(-maxspeed, velocity.y);
			if (location.y < 8) desired = PVector(velocity.x, maxspeed);
			if (location.y >= HEIGHT - 8) desired = PVector(velocity.x, -maxspeed);

			if (desired != velocity) {
				PVector steer = desired - velocity;
				steer.limit(maxforce);
				applyForce(steer);
			}

			if (location.x < 0) location.x = 0;
			if (location.y < 0) location.y = 0;
			if (location.x >= WIDTH) location.x = WIDTH - 1;
			if (location.y >= HEIGHT) location.y = HEIGHT - 1;
		}

		bool bounceOffBorders(float bounce) {
			bool bounced = false;

			if (location.x >= WIDTH) {
				location.x = WIDTH - 1;
				velocity.x *= -bounce;
				bounced = true;
			}
			else if (location.x < 0) {
				location.x = 0;
				velocity.x *= -bounce;
				bounced = true;
			}

			if (location.y >= HEIGHT) {
				location.y = HEIGHT - 1;
				velocity.y *= -bounce;
				bounced = true;
			}
			else if (location.y < 0) {
				location.y = 0;
				velocity.y *= -bounce;
				bounced = true;
			}

			return bounced;
		}

		void render() {
			// // Draw a triangle rotated in the direction of velocity
			// float theta = velocity.heading2D() + radians(90);
			// fill(175);
			// stroke(0);
			// pushMatrix();
			// translate(location.x,location.y);
			// rotate(theta);
			// beginShape(TRIANGLES);
			// vertex(0, -r*2);
			// vertex(-r, r*2);
			// vertex(r, r*2);
			// endShape();
			// popMatrix();
			// backgroundLayer.drawPixel(location.x, location.y, CRGB::Blue);
		}
};

// ------------------------------ ЭФФЕКТ КУБИК 2D ----------------------
// (c) SottNick
// refactored by Vortigont
#define CUBE2D_MAX_SIZE 7       // максимальный размер обрабатываемых квадратов (AxA)
#define CUBE2D_PAUSE_FRAMES 20   // число кадров паузы между трансформациями

// ----------- Эффект "Аквариум" (с) SottNick
// This is the array that we keep our computed noise values in
#define CAUSTICS_BR                     (100U)                // яркость бликов в процентах (от чистого белого света)

// ----------- Эффект "Звезды" адаптация SottNick
#define CENTER_DRIFT_SPEED 6U         // скорость перемещения плавающего центра возникновения звёзд
#define STAR_BLENDER 128U             // хз что это

//---------- Эффект "Фейерверк" адаптация kostyamat
//https://gist.github.com/jasoncoon/0cccc5ba7ab108c0a373

#define MODEL_BORDER (HEIGHT - 4U)  // как далеко за экран может вылетить снаряд, если снаряд вылетает за экран, то всышка белого света (не особо логично)
#define MODEL_WIDTH  (MODEL_BORDER + WIDTH  + MODEL_BORDER) // не трогать, - матиматика
#define MODEL_HEIGHT (MODEL_BORDER + HEIGHT + MODEL_BORDER) // -//-
#define PIXEL_X_OFFSET ((MODEL_WIDTH  - WIDTH ) / 2) // -//-
#define PIXEL_Y_OFFSET ((MODEL_HEIGHT - HEIGHT) / 2) // -//-

#define SPARK 8U // максимальное количество снарядов
#define NUM_SPARKS WIDTH // количество разлитающихся питард (частей снаряда)
const saccum78 gGravity = 10;
const fract8  gBounce = 127;
const fract8  gDrag = 255;

typedef struct _DOTS_STORE {
		accum88 gBurstx;
		accum88 gBursty;
		saccum78 gBurstxv;
		saccum78 gBurstyv;
		CRGB gBurstcolor;
		bool gSkyburst = false;
} DOTS_STORE;

class Dot {    // класс для создания снарядов и питард
private:
	CRGB empty = CRGB(0,0,0);
public:
	byte    show;
	byte    theType;
	accum88 x;
	accum88 y;
	saccum78 xv;
	saccum78 yv;
	accum88 r;
	CRGB color;

	Dot() {
		show = 0;
		theType = 0;
		x =  0;
		y =  0;
		xv = 0;
		yv = 0;
		r  = 0;
		color.setRGB( 0, 0, 0);
	}

	void Draw(CRGB *leds);
	void Move(DOTS_STORE &store, bool Flashing);
	void GroundLaunch(DOTS_STORE &store);
	void Skyburst( accum88 basex, accum88 basey, saccum78 basedv, CRGB& basecolor, uint8_t dim);
	CRGB &piXY(CRGB *leds, byte x, byte y);

	int16_t scale15by8_local( int16_t i, fract8 _scale )
	{
		int16_t result;
		result = (int32_t)((int32_t)i * _scale) / 256;
		return result;
	};

	void screenscale(accum88 a, byte N, byte &screen, byte &screenerr)
	{
		byte ia = a >> 8;
		screen = scale8(ia, N);
		byte m = screen * (256 / N);
		screenerr = (ia - m) * scale8(255, N);
		return;
	};
};

// ------------ Эффект "Тихий Океан"
//  "Pacifica" перенос кода kostyamat
//  Gentle, blue-green ocean waves.
//  December 2019, Mark Kriegsman and Mary Corey March.
//  For Dan.
// https://raw.githubusercontent.com/FastLED/FastLED/master/examples/Pacifica/Pacifica.ino
static const TProgmemRGBPalette16 pacifica_palette_1 FL_PROGMEM =
		{ 0x000507, 0x000409, 0x00030B, 0x00030D, 0x000210, 0x000212, 0x000114, 0x000117,
			0x000019, 0x00001C, 0x000026, 0x000031, 0x00003B, 0x000046, 0x14554B, 0x28AA50 };
static const TProgmemRGBPalette16 pacifica_palette_2 FL_PROGMEM =
		{ 0x000507, 0x000409, 0x00030B, 0x00030D, 0x000210, 0x000212, 0x000114, 0x000117,
			0x000019, 0x00001C, 0x000026, 0x000031, 0x00003B, 0x000046, 0x0C5F52, 0x19BE5F };
static const TProgmemRGBPalette16 pacifica_palette_3 FL_PROGMEM =
		{ 0x000208, 0x00030E, 0x000514, 0x00061A, 0x000820, 0x000927, 0x000B2D, 0x000C33,
			0x000E39, 0x001040, 0x001450, 0x001860, 0x001C70, 0x002080, 0x1040BF, 0x2060FF };

// ---- Эффект "Мотыльки"
// (с) Сотнег, https://community.alexgyver.ru/threads/wifi-lampa-budilnik-obsuzhdenie-proekta.1411/post-49262
#define BUTTERFLY_MAX_COUNT           (64U) // максимальное количество мотыльков
#define BUTTERFLY_FIX_COUNT           (28U) // количество мотыльков для режима, конда бегунок Масштаб регулирует цвет

//------------ Эффект "Змеиный Остров"
#define SNAKE_LENGTH  (HEIGHT / 2U)
#define SNAKE2_LENGTH (HEIGHT / 2U + WIDTH / 4U)
#define MAX_SNAKES    (16U) 

// SottNick константы
//константы размера матрицы вычисляется только здесь и не меняется в эффектах
#ifdef ALS_LAMPMATRIX

const uint8_t CENTER_X_MINOR =  (WIDTH / 2) -  ((WIDTH - 1) & 0x01); // центр матрицы по ИКСУ, сдвинутый в меньшую сторону, если ширина чётная
const uint8_t CENTER_Y_MINOR = (HEIGHT / 2) - ((HEIGHT - 1) & 0x01); // центр матрицы по ИГРЕКУ, сдвинутый в меньшую сторону, если высота чётная
const uint8_t CENTER_X_MAJOR =   WIDTH / 2  + (WIDTH % 2);           // центр матрицы по ИКСУ, сдвинутый в большую сторону, если ширина чётная
const uint8_t CENTER_Y_MAJOR =  HEIGHT / 2  + (HEIGHT % 2);          // центр матрицы по ИГРЕКУ, сдвинутый в большую сторону, если высота чётная

	
#endif
#define  GLOBAL_COLOR_1 CRGB::Green         // основной цвет №1 для игр
#define  GLOBAL_COLOR_2 CRGB::Orange        // основной цвет №2 для игр
// Эффект Тетрис
// самая важная часть программы! Координаты пикселей фигур
//  0 - палка
//  1 - кубик
//  2 - Г
//  3 - Г обратная
//  4 - Z
//  5 - Z обратная
//  6 - Т

const int8_t figures[7][12][2] PROGMEM = {
	{
		{ -1, 0}, {1, 0}, {2, 0},
		{0, 1}, {0, 2}, {0, 3},
		{ -1, 0}, {1, 0}, {2, 0},
		{0, 1}, {0, 2}, {0, 3},
	},
	{
		{0, 1}, {1, 0}, {1, 1},
		{0, 1}, {1, 0}, {1, 1},
		{0, 1}, {1, 0}, {1, 1},
		{0, 1}, {1, 0}, {1, 1},
	},
	{
		{ -1, 0}, { -1, 1}, {1, 0},
		{0, 1}, {0, 2}, {1, 2},
		{ -2, 1}, { -1, 1}, {0, 1},
		{ -1, 0}, {0, 1}, {0, 2},
	},
	{
		{ -1, 0}, {1, 0}, {1, 1},
		{0, 1}, {0, 2}, {1, 0},
		{0, 1}, {1, 1}, {2, 1},
		{0, 1}, {0, 2}, { -1, 2},
	},
	{
		{ -1, 0}, {0, 1}, {1, 1},
		{0, 1}, { -1, 1}, { -1, 2},
		{ -1, 0}, {0, 1}, {1, 1},
		{0, 1}, { -1, 1}, { -1, 2},
	},
	{
		{ -1, 1}, {0, 1}, {1, 0},
		{0, 1}, {1, 1}, {1, 2},
		{ -1, 1}, {0, 1}, {1, 0},
		{0, 1}, {1, 1}, {1, 2},
	},
	{
		{ -1, 0}, {0, 1}, {1, 0},
		{0, 1}, {0, 2}, {1, 1},
		{ -1, 1}, {0, 1}, {1, 1},
		{ -1, 1}, {0, 1}, {0, 2},
	}
};

// **************** НАСТРОЙКИ ARKAN ****************
#define SHELF_LENGTH 5    // длина полки
#define VELOCITY 5        // скорость шара
#define BALL_SPEED 50     // период интегрирования

#define BLOCKS_H 4        // высота кучи блоков
#define LINE_NUM 8        // количество "линий" с блоками других уровней
// #define LINE_MAX 4        // макс. длина линии

// цвета блоков по крутости
#define BLOCK_COLOR_1 CRGB::Aqua
#define BLOCK_COLOR_2 CRGB::Amethyst
#define BLOCK_COLOR_3 CRGB::Blue

// ***************** НАСТРОЙКИ ГЕНЕРАЦИИ ЛАБИРИНТА *****************
#define GAMEMODE 0        // режим игры: 0 - видим весь лабиринт, 1 - видим вокруг себя часть
#define FOV 3             // область видимости в режиме игры 1

// размеры лабиринта ДОЛЖНЫ БЫТЬ НЕЧЁТНЫЕ независимо от размеров матрицы!
// при SHIFT 1 размер лабиринта можно ставить на 1 длиннее матрицы (матрица 16х16 лабиринт 17х17)
#if (WIDTH % 2 == 0)
#define MAZE_WIDTH (WIDTH-1)      // ширина лабиринта
#else
#define MAZE_WIDTH WIDTH          // ширина лабиринта
#endif

#if (HEIGHT % 2 == 0)
#define MAZE_HEIGHT (HEIGHT-1)    // высота лабиринта
#else
#define MAZE_HEIGHT HEIGHT        // высота лабиринта
#endif
#define MAZE_SHIFT 0                   // (1 да / 0 нет) смещение лабиринта (чтобы не видеть нижнюю и левую стену)



#endif
	

#endif

