// Command data pin: GPIO14
// Reset pin: GPIO15

#include <pico/stdlib.h>
#include <hardware/gpio.h>

#include "SSD1351.h"
#include "spi.h"
#include "timing.h"

// Pins definition.
extern const uint cmd_pin;
extern const uint reset_pin;

void SSD1351_WriteCommand(unsigned char cmd) {
    // Set CMD pin low.
	gpio_put(cmd_pin, 0);
    spi0_write(cmd);
}

void SSD1351_WriteData(unsigned char data) {
    // Set CMD pin high.
	gpio_put(cmd_pin, 1);
    spi0_write(data);
}

// Setup the D/C pin correctly.
void SSD1351_init(void) {
    // Setup command / data pin.
    gpio_init(cmd_pin);
    gpio_set_dir(cmd_pin, GPIO_OUT);
    gpio_put(cmd_pin, 1);
    
    // Setup reset pin.
    gpio_init(reset_pin);
	gpio_set_dir(reset_pin, GPIO_OUT);
    
	gpio_put(reset_pin, 1);
	timing_sleep(10);
	gpio_put(reset_pin, 0);
    timing_sleep(500);
    gpio_put(reset_pin, 1);
    
    spi0_select();
    
    SSD1351_WriteCommand(0xFD); // COMMANDLOCK
    SSD1351_WriteData(0x12);

    SSD1351_WriteCommand(0xFD); // COMMANDLOCK
    SSD1351_WriteData(0xB1);
    
    SSD1351_WriteCommand(0xAE); // DISPLAYOFF
    SSD1351_WriteCommand(0xB3); // CLOCKDIV
    SSD1351_WriteCommand(0xF1); // 7:4 = Oscillator Frequency, 3:0 = CLK Div Ratio (A[3:0]+1 = 1..16)
    SSD1351_WriteCommand(0xCA); // MUXRATIO
    SSD1351_WriteData(0x7F);

    SSD1351_WriteCommand(0xA0); // SETREMAP
    SSD1351_WriteData(0x74);

    SSD1351_WriteCommand(0x15); // SETCOLUMN
    SSD1351_WriteData(0x00);
    SSD1351_WriteData(0x7F);

    SSD1351_WriteCommand(0x75); // SETROW
    SSD1351_WriteData(0x00);
    SSD1351_WriteData(0x7F);

    SSD1351_WriteCommand(0xA1); // STARTLINE
    SSD1351_WriteData(0x00);

    SSD1351_WriteCommand(0xA2); // DISPLAYOFFSET
    SSD1351_WriteData(0x00);

    SSD1351_WriteCommand(0xB5); // SETGPIO
    SSD1351_WriteData(0x00);

    SSD1351_WriteCommand(0xAB); // FUNCTIONSELECT
    SSD1351_WriteData(0x01);

    SSD1351_WriteCommand(0xB1); // PRECHARGE
    SSD1351_WriteData(0x32);

    SSD1351_WriteCommand(0xBE); // VCOMH
    SSD1351_WriteData(0x05);

    SSD1351_WriteCommand(0xA6); // NORMALDISPLAY (don't invert)
    SSD1351_WriteCommand(0xC1); // CONTRASTABC
    SSD1351_WriteData(0xC8);
    SSD1351_WriteData(0x80);
    SSD1351_WriteData(0xC8);

    SSD1351_WriteCommand(0xC7); // CONTRASTMASTER
    SSD1351_WriteData(0x0F);

    SSD1351_WriteCommand(0xB4); // SETVSL
    SSD1351_WriteData(0xA0);
    SSD1351_WriteData(0xB5);
    SSD1351_WriteData(0x55);

    SSD1351_WriteCommand(0xB6); // PRECHARGE2
    SSD1351_WriteData(0x01);

    SSD1351_WriteCommand(0xAF); // DISPLAYON
    spi0_deselect();
}

void SSD1351_start_transfer(void) {
    spi0_select();    

    SSD1351_WriteCommand(SSD1351_CMD_SETCOLUMN);
    SSD1351_WriteData(0x00);                            // Start Address
    SSD1351_WriteData(0x7F);                            // Start Address
    
    SSD1351_WriteCommand(SSD1351_CMD_SETROW);
    SSD1351_WriteData(0x00);                            // Start Address
    SSD1351_WriteData(0x7F);                            // Start Address

    // write to RAM
    SSD1351_WriteCommand(0x5C); // WRITERAM    
}

void SSD1351_stop_transfer(void) {
    spi0_deselect();    
}

void SSD1351_transfer(unsigned char *buffer,int elements) {
    int count;
    
    SSD1351_start_transfer();
    
    for(count=0;count<elements/2;count++) {
      SSD1351_WriteData(buffer[count*2 + 1]);
      SSD1351_WriteData(buffer[count*2]);
    }
    
    SSD1351_stop_transfer();
}

