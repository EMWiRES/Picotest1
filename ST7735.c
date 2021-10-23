// Command data pin: GPIO14
// Reset pin: GPIO15
#include <pico/stdlib.h>
#include <hardware/gpio.h>

#include "ST7735.h"
#include "spi.h"
#include "timing.h"

// Pins definition.
extern const uint cmd_pin;
extern const uint reset_pin;

// For some reason it does not start at 0.0 (?)
unsigned char st7735_column_start = 2;
unsigned char st7735_row_start = 1;

void st7735_writeCommand(unsigned char cmd) {
    // Set CMD pin low.
	gpio_put(cmd_pin, 0);
    spi0_write(cmd);
}

void st7735_writeData(unsigned char data) {
    // Set CMD pin high.
	gpio_put(cmd_pin, 1);
    spi0_write(data);
}

void st7735_reset(void) {
  spi0_select();

  timing_sleep(50);
  gpio_put(reset_pin, 0);
  timing_sleep(50);
  gpio_put(reset_pin, 1);
  timing_sleep(50);  
}

void st7735_init(void) {
    // Setup command / data pin.
    gpio_init(cmd_pin);
    gpio_set_dir(cmd_pin, GPIO_OUT);
    gpio_put(cmd_pin, 1);
    
    // Setup reset pin.
    gpio_init(reset_pin);
	gpio_set_dir(reset_pin, GPIO_OUT);
          
    st7735_reset();
    
    st7735_writeCommand(ST7735_SWRESET);        //  1: Software reset, 0 args, w/delay
    timing_sleep(150);                          //     150 ms delay
    st7735_writeCommand(ST7735_SLPOUT);         //  2: Out of sleep mode, 0 args, w/delay
    timing_sleep(500);                          //     500 ms delay
    st7735_writeCommand(ST7735_FRMCTR1);        //  3: Frame rate ctrl - normal mode, 3 args:
    st7735_writeData(0x01);                     //     Rate = fosc/(1x2+40) * (LINE+2C+2D)
    st7735_writeData(0x2C);
    st7735_writeData(0x2D);                 
    st7735_writeCommand(ST7735_FRMCTR2);        //  4: Frame rate control - idle mode, 3 args:
    st7735_writeData(0x01);                     //     Rate = fosc/(1x2+40) * (LINE+2C+2D)
    st7735_writeData(0x2C);
    st7735_writeData(0x2D);                 
    st7735_writeCommand(ST7735_FRMCTR3);        //  5: Frame rate ctrl - partial mode, 6 args:
    st7735_writeData(0x01);                     //     Rate = fosc/(1x2+40) * (LINE+2C+2D)
    st7735_writeData(0x2C);
    st7735_writeData(0x2D);                     //     Dot inversion mode
    st7735_writeData(0x01);                     //     Rate = fosc/(1x2+40) * (LINE+2C+2D)
    st7735_writeData(0x2C);
    st7735_writeData(0x2D);                     //     Line inversion mode
    st7735_writeCommand(ST7735_INVCTR);         //  6: Display inversion ctrl, 1 arg, no delay:
    st7735_writeData(0x07);                     //     No inversion
    st7735_writeCommand(ST7735_PWCTR1);         //  7: Power control, 3 args, no delay:
    st7735_writeData(0xA2);
    st7735_writeData(0x02);                     //     -4.6V
    st7735_writeData(0x84);                     //     AUTO mode
    st7735_writeCommand(ST7735_PWCTR2);         //  8: Power control, 1 arg, no delay:
    st7735_writeData(0xC5);                     //     VGH25 = 2.4C VGSEL = -10 VGH = 3 * AVDD
    st7735_writeCommand(ST7735_PWCTR3);         //  9: Power control, 2 args, no delay:
    st7735_writeData(0x0A);                     //     Opamp current small
    st7735_writeData(0x00);                     //     Boost frequency
    st7735_writeCommand(ST7735_PWCTR4);         // 10: Power control, 2 args, no delay:
    st7735_writeData(0x8A);                     //     BCLK/2, Opamp current small & Medium low
    st7735_writeData(0x2A);
    st7735_writeCommand(ST7735_PWCTR5);         // 11: Power control, 2 args, no delay:
    st7735_writeData(0x8A);
    st7735_writeData(0xEE);
    st7735_writeCommand(ST7735_VMCTR1);  	// 12: Power control, 1 arg, no delay:
    st7735_writeData(0x0E);
    st7735_writeCommand(ST7735_INVOFF);         // 13: Don't invert display, no args, no delay

    st7735_writeCommand(ST7735_MADCTL);         // 14: Memory access control (directions), 1 arg:
    st7735_writeData(0x48);                     //     row addr/col addr, bottom to top refresh RGB order.
    // st7735_writeData(0xC0);                     //     row addr/col addr, bottom to top refresh BGR order.
    
    st7735_writeCommand(ST7735_COLMOD);  	// 15: set color mode, 1 arg, no delay:
    st7735_writeData(0x05);		        //     16-bit color
    
    st7735_writeCommand(ST7735_CASET);          //  1: Column addr set, 4 args, no delay:
    st7735_writeData(0x00);                     //     XSTART = 0
    st7735_writeData(0x00);         
    st7735_writeData(0x00);                     //     
    st7735_writeData(0x7F);                     //     XEND = 127
    st7735_writeCommand(ST7735_RASET);          //  2: Row addr set, 4 args, no delay:
    st7735_writeData(0x00);                     //     XSTART = 0
    st7735_writeData(0x00);         
    st7735_writeData(0x00);                     //     
    st7735_writeData(0x7F);                     //     XEND = 127

    st7735_writeCommand(ST7735_GMCTRP1);        //  1: Magical unicorn dust, 16 args, no delay:
    st7735_writeData(0x02);
    st7735_writeData(0x1c);
    st7735_writeData(0x07);
    st7735_writeData(0x12);
    st7735_writeData(0x37);
    st7735_writeData(0x32);
    st7735_writeData(0x29);
    st7735_writeData(0x2d);
    st7735_writeData(0x29);
    st7735_writeData(0x25);
    st7735_writeData(0x2B);
    st7735_writeData(0x39);
    st7735_writeData(0x00);
    st7735_writeData(0x01);
    st7735_writeData(0x03);
    st7735_writeData(0x10);
    
    st7735_writeCommand(ST7735_GMCTRN1);        //  2: Sparkles and rainbows, 16 args, no delay:
    st7735_writeData(0x03);
    st7735_writeData(0x1d);
    st7735_writeData(0x07);
    st7735_writeData(0x06);
    st7735_writeData(0x2E);
    st7735_writeData(0x2C);
    st7735_writeData(0x29);
    st7735_writeData(0x2D);
    st7735_writeData(0x2E);
    st7735_writeData(0x2E);
    st7735_writeData(0x37);
    st7735_writeData(0x3F);
    st7735_writeData(0x00);
    st7735_writeData(0x00);
    st7735_writeData(0x02);
    st7735_writeData(0x10);
    
    st7735_writeCommand(ST7735_NORON);          //  3: Normal display on, no args, w/delay
    timing_sleep(10);                     	//     10 ms delay
    st7735_writeCommand(ST7735_DISPON);         //  4: Main screen turn on, no args w/delay
    timing_sleep(100);                          //     100 ms delay
}

void st7735_start_transfer(void) {
    spi0_select();    
        
    st7735_writeCommand(ST7735_CASET);                 // Column addr set
    st7735_writeData(0x00);                            
    st7735_writeData(0 + st7735_column_start);	       // XSTART
    st7735_writeData(0x00);                            
    st7735_writeData(0x7F + st7735_column_start);      // XEND
    
    st7735_writeCommand(ST7735_RASET);                  // Row addr set
    st7735_writeData(0x00);                            // YSTART
    st7735_writeData(0x00 + st7735_row_start);         // 
    st7735_writeData(0x00);                            // 
    st7735_writeData(0x7F + st7735_row_start);         // YEND

    // write to RAM
    st7735_writeCommand(ST7735_RAMWR); // WRITERAM    
}

void st7735_stop_transfer(void) {
    spi0_deselect();    
}

void st7735_transfer(unsigned char *buffer,int elements) {
    int count;
    
    st7735_start_transfer();
    
    for(count=0;count<elements/2;count++) {
      st7735_writeData(buffer[count*2 + 1]);
      st7735_writeData(buffer[count*2]);
    }
    
    st7735_stop_transfer();
}
