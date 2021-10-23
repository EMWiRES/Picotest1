/*
 * Raspberry Pi Pico port of the Adafruit Uncanny_Eyes code (https://github.com/adafruit/Uncanny_Eyes).
 *
 * This version contains a port to Pico and SSD1351 OLED display.
 *
 * See: www.emwires.com/Picostuff for more information.
 */
#include <pico/stdlib.h>
#include <string.h>

#include "digitalIO.h"
#include "led.h"
#include "timing.h"

#include "spi.h"
// #include "SSD1351.h"
// #include "ST7735.h"
#include "ILI9225.h"
// #include "ILI9163C.h"

void eye_main(void);

  //-------------------------------------------------------------------------
int main() {
	
  digitalIO_init();
  timing_init();
  led_init();
  timing_start_timer();
  
  led_on();

  spi0_init();
  spi0_set_mode(SPI_MODE0);
    
  // SSD1351_init();
  // st7735_init();
  
  // ILI9163C_init();
  // ILI9163C_clrScreen();
  
  ILI9225_init();
  ILI9225_clrScreen();
    
  led_off();
  
  eye_main();
}

