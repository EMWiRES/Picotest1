// Pin selections here are based on the original Adafruit Learning System
// guide for the Teensy 3.x project.  Some of these pin numbers don't even
// exist on the smaller SAMD M0 & M4 boards, so you may need to make other
// selections:

#define IRIS_MIN      120 
#define IRIS_MAX      550

#define SYMMETRICAL_EYELID 1
// #define TRACKING 1

#define NOBLINKING 1

// GRAPHICS SETTINGS (appearance of eye) -----------------------------------

// Enable ONE of these #includes -- HUGE graphics tables for various eyes:
#include "defaultEye.h"      // Standard human-ish hazel eye -OR-
// #include "defaultEye2.h"
// #include "dragonEye.h"     // Slit pupil fiery dragon/demon eye -OR-
// #include "noScleraEye.h"   // Large iris, no sclera -OR-
// #include "goatEye.h"       // Horizontal pupil goat/Krampus eye -OR-
// #include "newtEye.h"       // Eye of newt -OR-
// #include "terminatorEye.h" // Git to da choppah!
// #include "catEye.h"        // Cartoonish cat (flat "2D" colors)

// #include "owlEye.h"        // Minerva the owl (DISABLE TRACKING)
// #include "naugaEye.h"      // Nauga googly eye (DISABLE TRACKING)
// #include "doeEye.h"        // Cartoon deer eye (DISABLE TRACKING)


// EYE LIST ----------------------------------------------------------------

// This table contains ONE LINE PER EYE.  The table MUST be present with
// this name and contain ONE OR MORE lines.  Each line contains THREE items:
// a pin number for the corresponding TFT/OLED display's SELECT line, a pin
// pin number for that eye's "wink" button (or -1 if not used), and a screen
// rotation value (0-3) for that eye.

eyeInfo_t eyeInfo[] = {
  {  0, -1, 0 } // LEFT EYE display-select and wink pins, no rotation
};
