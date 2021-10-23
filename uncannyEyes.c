//--------------------------------------------------------------------------
// Uncanny eyes for Adafruit 1.5" OLED (product #1431) or 1.44" TFT LCD
// (#2088).  Works on PJRC Teensy 3.x and on Adafruit M0 and M4 boards
// (Feather, Metro, etc.).  This code uses features specific to these
// boards and WILL NOT work on normal Arduino or other boards!
//
// SEE FILE "config.h" FOR MOST CONFIGURATION (graphics, pins, display type,
// etc).  Probably won't need to edit THIS file unless you're doing some
// extremely custom modifications.
//
// Adafruit invests time and resources providing this open source code,
// please support Adafruit and open-source hardware by purchasing products
// from Adafruit!
//
// Written by Phil Burgess / Paint Your Dragon for Adafruit Industries.
// MIT license.  SPI FIFO insight from Paul Stoffregen's ILI9341_t3 library.
// Inspired by David Boccabella's (Marcwolf) hybrid servo/OLED eye concept.
//--------------------------------------------------------------------------
// 
// Ported to Raspberry Pi Pico by Allard van der Bas / EMWiRES.
// See www.emwires.com/Picostuff for more information.
//
//--------------------------------------------------------------------------
#include <stdio.h>
#include <pico/stdlib.h>
#include <stdlib.h>
#include <time.h>

#include "types.h"

#include "timing.h"
#include "led.h"

// #include "SSD1351.h"
// #include "ST7735.h"
// #include "ILI9163C.h"
#include "ILI9225.h"

#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 128

unsigned short frame_16bit[DISPLAY_WIDTH * DISPLAY_HEIGHT];

// Arduino compatible functions.
int map(int x, int in_min, int in_max, int out_min, int out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int single_random(int maxVal);

int range_random(int minVal, int maxVal);

// returns a random integar between minVal and maxVal
int range_random(int minVal, int maxVal)
{
  // int rand(void); included by default from newlib
  return ( rand() % (maxVal-minVal) ) + minVal;
}

// returns a random integar between 0 and maxVal
int single_random(int maxVal)
{
  return range_random( 0, maxVal);
}

unsigned long millis(void) {
    return( time_us_32() * 1000 );
}

unsigned long micros(void) {
    return( time_us_32() );
}

typedef struct {        // Struct is defined before including config.h --
  int8_t  select;       // pin numbers for each eye's screen select line
  int8_t  wink;         // and wink button (or -1 if none) specified there,
  uint8_t rotation;     // also display rotation.
} eyeInfo_t;

#include "config.h"     // ****** CONFIGURATION IS DONE IN HERE ******

// A simple state machine is used to control eye blinks/winks:
#define NOBLINK 0       // Not currently engaged in a blink
#define ENBLINK 1       // Eyelid is currently closing
#define DEBLINK 2       // Eyelid is currently opening

typedef struct {
  uint8_t  state;       // NOBLINK/ENBLINK/DEBLINK
  uint32_t duration;    // Duration of blink state (micros)
  uint32_t startTime;   // Time (micros) of last state change
} eyeBlink;

#define NUM_EYES (sizeof eyeInfo / sizeof eyeInfo[0]) // config.h pin list

struct {                // One-per-eye structure
  eyeBlink     blink;   // Current blink/wink state
} eye[NUM_EYES];

uint32_t startTime;  // For FPS indicator
uint32_t frames   = 0; // Used in frame rate calculation
uint8_t  eyeIndex = 0; // eye[] array counter

int8_t  eyeInMotion      = false;
int16_t  eyeOldX=512, eyeOldY=512, eyeNewX=512, eyeNewY=512;
uint32_t eyeMoveStartTime = 0L;
int32_t  eyeMoveDuration  = 0L;
uint8_t uThreshold = 128;
uint32_t timeOfLastBlink = 0L, timeToNextBlink = 0L;
  
// INITIALIZATION -- runs once at startup ----------------------------------

void setup(void) {
  uint8_t e; // Eye index, 0 to NUM_EYES-1

  // Initialize eye objects based on eyeInfo list in config.h:
  for(e=0; e<NUM_EYES; e++) {
  }
  
  srand(0x7ee1);   // Initialization, should only be called once.  
  startTime = millis(); // For frame-rate calculation
  led_on();
  
  frames   = 0; // Used in frame rate calculation
  eyeIndex = 0; // eye[] array counter

  eyeInMotion      = false;
  eyeOldX=512, eyeOldY=512, eyeNewX=512, eyeNewY=512;
  eyeMoveStartTime = 0L;
  eyeMoveDuration  = 0L;
  uThreshold = 128;
  timeOfLastBlink = 0L;
  timeToNextBlink = 0L;
}

/* void saveFile(void) {
	FILE *f;
	char fileName[128];
	
	unsigned int count;
	unsigned short pixel;
	unsigned char R;
	unsigned char G;
	unsigned char B;
	
	sprintf(fileName,"rawFile%d.raw",frames);
	
	f = fopen(fileName,"wb");
	
	if (f) {
		for (count=0;count < 128*128;count++) {
			pixel = frame_16bit[count];
			R = (pixel >> 11) & 0x1F;
			R <<= 3;
			G = (pixel >> 5) & 0x3F;
			G <<= 2;
			B = pixel & 0x1F;
			B <<= 3;
			
			fwrite(&R,1,1,f);
			fwrite(&G,1,1,f);
			fwrite(&B,1,1,f);
		}
	}
	
	fclose(f);
} */

// EYE-RENDERING FUNCTION --------------------------------------------------

void drawEye( // Renders one eye.  Inputs must be pre-clipped & valid.
  uint8_t  e,       // Eye array index; 0 or 1 for left/right
  uint16_t iScale,  // Scale factor for iris (0-1023)
  uint8_t  scleraX, // First pixel X offset into sclera image
  uint8_t  scleraY, // First pixel Y offset into sclera image
  uint8_t  uT,      // Upper eyelid threshold value
  uint8_t  lT) {    // Lower eyelid threshold value

  uint8_t  screenX, screenY, scleraXsave;
  int16_t  irisX, irisY;
  uint16_t p, a;
  uint32_t d;

  uint8_t  irisThreshold = (128 * (1023 - iScale) + 512) / 1024;
  uint32_t irisScale     = IRIS_MAP_HEIGHT * 65536 / irisThreshold;

  // Now just issue raw 16-bit values for every pixel...

  scleraXsave = scleraX; // Save initial X value to reset on each line
  irisY       = scleraY - (SCLERA_HEIGHT - IRIS_HEIGHT) / 2;
  for(screenY=0; screenY<SCREEN_HEIGHT; screenY++, scleraY++, irisY++) {
    scleraX = scleraXsave;
    irisX   = scleraXsave - (SCLERA_WIDTH - IRIS_WIDTH) / 2;
    for(screenX=0; screenX<SCREEN_WIDTH; screenX++, scleraX++, irisX++) {
      if((lower[screenY][screenX] <= lT) ||
         (upper[screenY][screenX] <= uT)) {             // Covered by eyelid
        p = 0;
      } else if((irisY < 0) || (irisY >= IRIS_HEIGHT) ||
                (irisX < 0) || (irisX >= IRIS_WIDTH)) { // In sclera
        p = sclera[scleraY][scleraX];
      } else {                                          // Maybe iris...
        p = polar[irisY][irisX];                        // Polar angle/dist
        d = p & 0x7F;                                   // Distance from edge (0-127)
        if(d < irisThreshold) {                         // Within scaled iris area
          d = d * irisScale / 65536;                    // d scaled to iris image height
          a = (IRIS_MAP_WIDTH * (p >> 7)) / 512;        // Angle (X)
          p = iris[d][a];                               // Pixel = iris
        } else {                                        // Not in iris
          p = sclera[scleraY][scleraX];                 // Pixel = sclera
        }
      }
      
      // Do something with p
      frame_16bit[screenY*DISPLAY_WIDTH + screenX] = p;
      
    } // end column
  } // end scanline
  
  // Do something with the frame.
  
  // SSD1351_transfer((unsigned char *)frame_16bit,32768);
  // st7735_transfer((unsigned char *)frame_16bit,32768);
  // ILI9163C_transfer((unsigned char *)frame_16bit,32768);
  ILI9225_transfer((unsigned char *)frame_16bit,32768);
  
  led_toggle();
}

// EYE ANIMATION -----------------------------------------------------------

const uint8_t ease[] = { // Ease in/out curve for eye movements 3*t^2-2*t^3
    0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  2,  2,  2,  3,   // T
    3,  3,  4,  4,  4,  5,  5,  6,  6,  7,  7,  8,  9,  9, 10, 10,   // h
   11, 12, 12, 13, 14, 15, 15, 16, 17, 18, 18, 19, 20, 21, 22, 23,   // x
   24, 25, 26, 27, 27, 28, 29, 30, 31, 33, 34, 35, 36, 37, 38, 39,   // 2
   40, 41, 42, 44, 45, 46, 47, 48, 50, 51, 52, 53, 54, 56, 57, 58,   // A
   60, 61, 62, 63, 65, 66, 67, 69, 70, 72, 73, 74, 76, 77, 78, 80,   // l
   81, 83, 84, 85, 87, 88, 90, 91, 93, 94, 96, 97, 98,100,101,103,   // e
  104,106,107,109,110,112,113,115,116,118,119,121,122,124,125,127,   // c
  128,130,131,133,134,136,137,139,140,142,143,145,146,148,149,151,   // J
  152,154,155,157,158,159,161,162,164,165,167,168,170,171,172,174,   // a
  175,177,178,179,181,182,183,185,186,188,189,190,192,193,194,195,   // c
  197,198,199,201,202,203,204,205,207,208,209,210,211,213,214,215,   // o
  216,217,218,219,220,221,222,224,225,226,227,228,228,229,230,231,   // b
  232,233,234,235,236,237,237,238,239,240,240,241,242,243,243,244,   // s
  245,245,246,246,247,248,248,249,249,250,250,251,251,251,252,252,   // o
  252,253,253,253,254,254,254,254,254,255,255,255,255,255,255,255 }; // n

void frame( // Process motion for a single frame of left or right eye
  uint16_t        iScale) {     // Iris scale (0-1023) passed in
    
  int16_t         eyeX, eyeY;
  uint32_t        t = micros(); // Time at start of function

  if(++eyeIndex >= NUM_EYES) eyeIndex = 0; // Cycle through eyes, 1 per call
  
  // Autonomous X/Y eye motion
  //Periodically initiates motion to a new random point, random speed,

  // holds there for random period until next motion.
  int32_t dt = t - eyeMoveStartTime;      // uS elapsed since last eye event
  if(eyeInMotion) {                       // Currently moving?
    
    if(dt >= eyeMoveDuration) {           // Time up?  Destination reached.
      eyeInMotion      = false;           // Stop moving
      eyeMoveDuration  = single_random(3000000); // 0-3 sec stop
      eyeMoveStartTime = t;               // Save initial time of stop
      eyeX = eyeOldX = eyeNewX;           // Save position
      eyeY = eyeOldY = eyeNewY;
    } else { // Move time's not yet fully elapsed -- interpolate position
      int16_t e = ease[255 * dt / eyeMoveDuration] + 1;   // Ease curve
      eyeX = eyeOldX + (((eyeNewX - eyeOldX) * e) / 256); // Interp X
      eyeY = eyeOldY + (((eyeNewY - eyeOldY) * e) / 256); // and Y
    }
    
  } else {                                // Eye stopped
    eyeX = eyeOldX;
    eyeY = eyeOldY;
    
    if(dt > eyeMoveDuration) {            // Time up?  Begin new move.
      int16_t  dx, dy;
      uint32_t d;
      
      do {                                // Pick new dest in circle
        eyeNewX = single_random(1024);
        eyeNewY = single_random(1024);
        dx      = (eyeNewX * 2) - 1023;
        dy      = (eyeNewY * 2) - 1023;
        d = (dx * dx + dy * dy);
      } while( d > (1023 * 1023) ); // Keep trying
      
      eyeMoveDuration  = range_random(72000, 144000); // ~1/14 - ~1/7 sec

      eyeMoveStartTime = t;               // Save initial time of move
      eyeInMotion      = true;            // Start move on next frame
    }
    
  }

#ifdef NOBLINKING
  
#else
  // Similar to the autonomous eye movement above -- blink start times
  // and durations are random (within ranges).
  if( (t - timeOfLastBlink) >= timeToNextBlink) { // Start new blink?
    timeOfLastBlink = t;
    
    uint32_t blinkDuration = range_random(36000, 72000); // ~1/28 - ~1/14 sec
    
    // Set up durations for both eyes (if not already winking)
    for(uint8_t e=0; e<NUM_EYES; e++) {
      
      if(eye[e].blink.state == NOBLINK) {
        eye[e].blink.state     = ENBLINK;
        eye[e].blink.startTime = t;
        eye[e].blink.duration  = blinkDuration;
      }
      
    }
    
    timeToNextBlink = blinkDuration * 3 + single_random(4000000);
  }

  if(eye[eyeIndex].blink.state) { // Eye currently blinking?
    
    // Check if current blink state time has elapsed
    if((t - eye[eyeIndex].blink.startTime) >= eye[eyeIndex].blink.duration) {
      
      // Yes -- increment blink state, unless...
      if( (eye[eyeIndex].blink.state == ENBLINK) ) {  // Enblinking and...
        
        if( ++eye[eyeIndex].blink.state > DEBLINK) { // Deblinking finished?
          eye[eyeIndex].blink.state = NOBLINK;      // No longer blinking
        } else { // Advancing from ENBLINK to DEBLINK mode
          eye[eyeIndex].blink.duration *= 2; // DEBLINK is 1/2 ENBLINK speed
          eye[eyeIndex].blink.startTime = t;
        }
        
      } else {
        if( ++eye[eyeIndex].blink.state > DEBLINK) { // Deblinking finished?
          eye[eyeIndex].blink.state = NOBLINK;      // No longer blinking
        }
      }
      
    }
    
  } else { // Not currently blinking...check buttons!
      // eye[eyeIndex].blink.state = NOBLINK;
  }
#endif
  
  // Process motion, blinking and iris scale into renderable values

  // Scale eye X/Y positions (0-1023) to pixel units used by drawEye()
  eyeX = map(eyeX, 0, 1023, 0, SCLERA_WIDTH  - 128);
  eyeY = map(eyeY, 0, 1023, 0, SCLERA_HEIGHT - 128);
  
  if(eyeIndex == 1) eyeX = (SCLERA_WIDTH - 128) - eyeX; // Mirrored display

  // Horizontal position is offset so that eyes are very slightly crossed
  // to appear fixated (converged) at a conversational distance.  Number
  // here was extracted from my posterior and not mathematically based.
  // I suppose one could get all clever with a range sensor, but for now...
  if(NUM_EYES > 1) eyeX += 4;
  
  if(eyeX > (SCLERA_WIDTH - 128)) eyeX = (SCLERA_WIDTH - 128);

  // Eyelids are rendered using a brightness threshold image.  This same
  // map can be used to simplify another problem: making the upper eyelid
  // track the pupil (eyes tend to open only as much as needed -- e.g. look
  // down and the upper eyelid drops).  Just sample a point in the upper
  // lid map slightly above the pupil to determine the rendering threshold.
  uint8_t        lThreshold, n;
  
#ifdef TRACKING
  int16_t sampleX = SCLERA_WIDTH  / 2 - (eyeX / 2), // Reduce X influence
          sampleY = SCLERA_HEIGHT / 2 - (eyeY + IRIS_HEIGHT / 4);
  
  // Eyelid is slightly asymmetrical, so two readings are taken, averaged
  if(sampleY < 0) n = 0;
  else            n = (upper[sampleY][sampleX] +
                       upper[sampleY][SCREEN_WIDTH - 1 - sampleX]) / 2;
  uThreshold = (uThreshold * 3 + n) / 4; // Filter/soften motion
  // Lower eyelid doesn't track the same way, but seems to be pulled upward
  // by tension from the upper lid.
  lThreshold = 254 - uThreshold;
#else // No tracking -- eyelids full open unless blink modifies them
  
  uThreshold = lThreshold = 0;
#endif

  // The upper/lower thresholds are then scaled relative to the current
  // blink position so that blinks work together with pupil tracking.
  if(eye[eyeIndex].blink.state) { // Eye currently blinking?
    uint32_t s = (t - eye[eyeIndex].blink.startTime);
    if(s >= eye[eyeIndex].blink.duration) s = 255;   // At or past blink end
    else s = 255 * s / eye[eyeIndex].blink.duration; // Mid-blink
    s          = (eye[eyeIndex].blink.state == DEBLINK) ? 1 + s : 256 - s;
    n          = (uThreshold * s + 254 * (257 - s)) / 256;
    lThreshold = (lThreshold * s + 254 * (257 - s)) / 256;
  } else {
    n          = uThreshold;
  }

  // Pass all the derived values to the eye-rendering function:
  drawEye(eyeIndex, iScale, eyeX, eyeY, n, lThreshold);
}

// AUTONOMOUS IRIS SCALING (if no photocell or dial) -----------------------

// Autonomous iris motion uses a fractal behavior to similate both the major
// reaction of the eye plus the continuous smaller adjustments that occur.
uint16_t oldIris = (IRIS_MIN + IRIS_MAX) / 2, newIris;

void split( // Subdivides motion path into two sub-paths w/randimization
  int16_t  startValue, // Iris scale value (IRIS_MIN to IRIS_MAX) at start
  int16_t  endValue,   // Iris scale value at end
  uint32_t startTime,  // micros() at start
  int32_t  duration,   // Start-to-end time, in microseconds
  int16_t  range) {    // Allowable scale value variance when subdividing

  if(range >= 8) {     // Limit subdvision count, because recursion
    range    /= 2;     // Split range & time in half for subdivision,
    duration /= 2;     // then pick random center point within range:
    int16_t  midValue = (startValue + endValue - range) / 2 + single_random(range);
    uint32_t midTime  = startTime + duration;
    split(startValue, midValue, startTime, duration, range); // First half
    split(midValue  , endValue, midTime  , duration, range); // Second half
  } else {             // No more subdivisons, do iris motion...
    int32_t dt;        // Time (micros) since start of motion
    int16_t v;         // Interim value
    
    while((dt = (micros() - startTime)) < duration) {
      v = startValue + (((endValue - startValue) * dt) / duration);
      
	  if(v < IRIS_MIN)      v = IRIS_MIN; // Clip just in case
      else if(v > IRIS_MAX) v = IRIS_MAX;
      
      frame(v);        // Draw frame w/interim iris scale value
    }
  }
}


// MAIN LOOP -- runs continuously after setup() ----------------------------

void loop() {
  newIris = range_random(IRIS_MIN, IRIS_MAX);
  split(oldIris, newIris, micros(), 10000000L, IRIS_MAX - IRIS_MIN);
  oldIris = newIris;
}

void eye_main(void) {
    setup();
    
    while(1) {
      loop();
    }
}
