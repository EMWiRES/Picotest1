// Command data pin: GPIO14
// Reset pin: GPIO15
#include <pico/stdlib.h>
#include <hardware/gpio.h>

#include "ILI9225.h"
#include "spi.h"
#include "timing.h"

// Pins definition.
extern const uint cmd_pin;
extern const uint reset_pin;

void ILI9225_writeCommand(unsigned char cmd) {
    // Set CMD pin low.
	gpio_put(cmd_pin, 0);
    spi0_write(cmd);
}

void ILI9225_writeData(unsigned char data) {
    // Set CMD pin high.
	gpio_put(cmd_pin, 1);
    spi0_write(data);
}

void ILI9225_reset(void) {
  spi0_select();
  
  timing_sleep(1);
  gpio_put(reset_pin, 0);
  timing_sleep(10);
  gpio_put(reset_pin, 1);
  timing_sleep(50);  
}

void ILI9225_init(void) {
    // Setup command / data pin.
    gpio_init(cmd_pin);
    gpio_set_dir(cmd_pin, GPIO_OUT);
    gpio_put(cmd_pin, 1);
    
    // Setup reset pin.
    gpio_init(reset_pin);
	gpio_set_dir(reset_pin, GPIO_OUT);
    
    ILI9225_reset();
    
    /* Set SS bit and direction output from S528 to S1 */
    
    ILI9225_writeCommand(ILI9225_POWER_CTRL1); // Set SAP,DSTB,STB
    ILI9225_writeData(0x00);
    ILI9225_writeData(0x00);
    
    ILI9225_writeCommand(ILI9225_POWER_CTRL2); // Set APON,PON,AON,VCI1EN,VC
    ILI9225_writeData(0x00);
    ILI9225_writeData(0x00);
    
    ILI9225_writeCommand(ILI9225_POWER_CTRL3); // Set BT,DC1,DC2,DC3
    ILI9225_writeData(0x00);
    ILI9225_writeData(0x00);
    
    ILI9225_writeCommand(ILI9225_POWER_CTRL4); // Set GVDD
    ILI9225_writeData(0x00);
    ILI9225_writeData(0x00);
    
    ILI9225_writeCommand(ILI9225_POWER_CTRL5); // Set VCOMH/VCOML voltage
    ILI9225_writeData(0x00);
    ILI9225_writeData(0x00);
    
    spi0_deselect();
    timing_sleep(40);
    spi0_select();
    
    // Power-on sequence
    ILI9225_writeCommand(ILI9225_POWER_CTRL2); // Set APON,PON,AON,VCI1EN,VC
    ILI9225_writeData(0x00);
    ILI9225_writeData(0x18);
    ILI9225_writeCommand(ILI9225_POWER_CTRL3); // Set BT,DC1,DC2,DC3
    ILI9225_writeData(0x61);
    ILI9225_writeData(0x21);
    ILI9225_writeCommand(ILI9225_POWER_CTRL4); // Set GVDD   /*007F 0088 */
    ILI9225_writeData(0x00);
    ILI9225_writeData(0x6F);
    ILI9225_writeCommand(ILI9225_POWER_CTRL5); // Set VCOMH/VCOML voltage
    ILI9225_writeData(0x49);
    ILI9225_writeData(0x5F);
    ILI9225_writeCommand(ILI9225_POWER_CTRL1); // Set SAP,DSTB,STB
    ILI9225_writeData(0x08);
    ILI9225_writeData(0x00);
    
    spi0_deselect();
    timing_sleep(10);
    spi0_select();
    
    ILI9225_writeCommand(ILI9225_POWER_CTRL2); // Set APON,PON,AON,VCI1EN,VC
    ILI9225_writeData(0x10);
    ILI9225_writeData(0x3B);
    
    spi0_deselect();
    timing_sleep(50);
    spi0_select();
    
    ILI9225_writeCommand(ILI9225_DRIVER_OUTPUT_CTRL); // set the display line number and display direction
    ILI9225_writeData(0x01);
    ILI9225_writeData(0x1C);
    ILI9225_writeCommand(ILI9225_LCD_AC_DRIVING_CTRL); // set 1 line inversion
    ILI9225_writeData(0x01);
    ILI9225_writeData(0x00);
    ILI9225_writeCommand(ILI9225_ENTRY_MODE); // set GRAM write direction and BGR=1.
    ILI9225_writeData(0x10);
    ILI9225_writeData(0x30);
    ILI9225_writeCommand(ILI9225_DISP_CTRL1); // Display off
    ILI9225_writeData(0x00);
    ILI9225_writeData(0x00);
    ILI9225_writeCommand(ILI9225_BLANK_PERIOD_CTRL1); // set the back porch and front porch
    ILI9225_writeData(0x08);
    ILI9225_writeData(0x08);
    ILI9225_writeCommand(ILI9225_FRAME_CYCLE_CTRL); // set the clocks number per line
    ILI9225_writeData(0x11);
    ILI9225_writeData(0x00);
    ILI9225_writeCommand(ILI9225_INTERFACE_CTRL); // CPU interface
    ILI9225_writeData(0x00);
    ILI9225_writeData(0x00);
    ILI9225_writeCommand(ILI9225_OSC_CTRL); // Set Osc  /*0e01*/
    ILI9225_writeData(0x0D);
    ILI9225_writeData(0x01);
    ILI9225_writeCommand(ILI9225_VCI_RECYCLING); // Set VCI recycling
    ILI9225_writeData(0x00);
    ILI9225_writeData(0x20);
    ILI9225_writeCommand(ILI9225_RAM_ADDR_SET1); // RAM Address
    ILI9225_writeData(0x00);
    ILI9225_writeData(0x00);
    ILI9225_writeCommand(ILI9225_RAM_ADDR_SET2); // RAM Address
    ILI9225_writeData(0x00);
    ILI9225_writeData(0x00);

    /* Set GRAM area */
    ILI9225_writeCommand(ILI9225_GATE_SCAN_CTRL); 
    ILI9225_writeData(0x00);
    ILI9225_writeData(0x00);
    ILI9225_writeCommand(ILI9225_VERTICAL_SCROLL_CTRL1); 
    ILI9225_writeData(0x00);
    ILI9225_writeData(0xDB);
    ILI9225_writeCommand(ILI9225_VERTICAL_SCROLL_CTRL2); 
    ILI9225_writeData(0x00);
    ILI9225_writeData(0x00);
    ILI9225_writeCommand(ILI9225_VERTICAL_SCROLL_CTRL3); 
    ILI9225_writeData(0x00);
    ILI9225_writeData(0x00);
    ILI9225_writeCommand(ILI9225_PARTIAL_DRIVING_POS1); 
    ILI9225_writeData(0x00);
    ILI9225_writeData(0xDB);
    ILI9225_writeCommand(ILI9225_PARTIAL_DRIVING_POS2); 
    ILI9225_writeData(0x00);
    ILI9225_writeData(0x00);
    ILI9225_writeCommand(ILI9225_HORIZONTAL_WINDOW_ADDR1); 
    ILI9225_writeData(0x00);
    ILI9225_writeData(0xAF);
    ILI9225_writeCommand(ILI9225_HORIZONTAL_WINDOW_ADDR2); 
    ILI9225_writeData(0x00);
    ILI9225_writeData(0x00);
    ILI9225_writeCommand(ILI9225_VERTICAL_WINDOW_ADDR1); 
    ILI9225_writeData(0x00);
    ILI9225_writeData(0xDB);
    ILI9225_writeCommand(ILI9225_VERTICAL_WINDOW_ADDR2); 
    ILI9225_writeData(0x00);
    ILI9225_writeData(0x00);

    /* Set GAMMA curve */
    ILI9225_writeCommand(ILI9225_GAMMA_CTRL1); 
    ILI9225_writeData(0x00);
    ILI9225_writeData(0x00);
    ILI9225_writeCommand(ILI9225_GAMMA_CTRL2); 
    ILI9225_writeData(0x08);
    ILI9225_writeData(0x08);
    ILI9225_writeCommand(ILI9225_GAMMA_CTRL3); 
    ILI9225_writeData(0x08);
    ILI9225_writeData(0x0A);
    ILI9225_writeCommand(ILI9225_GAMMA_CTRL4);
    ILI9225_writeData(0x00);
    ILI9225_writeData(0x0A);
    ILI9225_writeCommand(ILI9225_GAMMA_CTRL5); 
    ILI9225_writeData(0x0A);
    ILI9225_writeData(0x08);
    ILI9225_writeCommand(ILI9225_GAMMA_CTRL6); 
    ILI9225_writeData(0x08);
    ILI9225_writeData(0x08);
    ILI9225_writeCommand(ILI9225_GAMMA_CTRL7); 
    ILI9225_writeData(0x00);
    ILI9225_writeData(0x00);
    ILI9225_writeCommand(ILI9225_GAMMA_CTRL8); 
    ILI9225_writeData(0x0A);
    ILI9225_writeData(0x00);
    ILI9225_writeCommand(ILI9225_GAMMA_CTRL9); 
    ILI9225_writeData(0x07);
    ILI9225_writeData(0x10);
    ILI9225_writeCommand(ILI9225_GAMMA_CTRL10); 
    ILI9225_writeData(0x07);
    ILI9225_writeData(0x10);

    ILI9225_writeCommand(ILI9225_DISP_CTRL1); 
    ILI9225_writeData(0x00);
    ILI9225_writeData(0x12);
    
    spi0_deselect();
    timing_sleep(50); 
    spi0_select();
    
    ILI9225_writeCommand(ILI9225_DISP_CTRL1);
    ILI9225_writeData(0x10);
    ILI9225_writeData(0x17);
    

    spi0_deselect();
    timing_sleep(50); 
    spi0_select();
    
    ILI9225_writeCommand(0x00);
    ILI9225_writeCommand(0xff);
    ILI9225_writeData(0x00);
    ILI9225_writeData(0x00);
                          
    ILI9225_writeCommand(ILI9225_POWER_CTRL1);
    ILI9225_writeData(0x00);
    ILI9225_writeData(0x00);

    spi0_deselect();
    timing_sleep(50);
    spi0_select();
    
    ILI9225_writeCommand(ILI9225_DISP_CTRL1);
    
    ILI9225_writeData(0x10);
    ILI9225_writeData(0x17);
    spi0_deselect();
}

void ILI9225_start_transfer(void) {
    spi0_select();    
        
    // write to RAM
    ILI9225_writeCommand(ILI9225_GRAM_DATA_REG); // WRITERAM    
}

void ILI9225_stop_transfer(void) {
    spi0_deselect();
}

void ILI9225_transfer(unsigned char *buffer,int elements) {
    int count;
    
    ILI9225_start_transfer();
    
    ILI9225_writeCommand(ILI9225_HORIZONTAL_WINDOW_ADDR1);
    ILI9225_writeData(0);
    ILI9225_writeData(127+24);
    ILI9225_writeCommand(ILI9225_HORIZONTAL_WINDOW_ADDR2);
    ILI9225_writeData(0);
    ILI9225_writeData(24);

    ILI9225_writeCommand(ILI9225_VERTICAL_WINDOW_ADDR1);
    ILI9225_writeData(0);
    ILI9225_writeData(127+46);
    ILI9225_writeCommand(ILI9225_VERTICAL_WINDOW_ADDR2);
    ILI9225_writeData(0);
    ILI9225_writeData(46);
    
    ILI9225_writeCommand(ILI9225_RAM_ADDR_SET1);
    ILI9225_writeData(0);
    ILI9225_writeData(24);

    ILI9225_writeCommand(ILI9225_RAM_ADDR_SET2);
    ILI9225_writeData(0);
    ILI9225_writeData(46);
    
    // write to RAM
    ILI9225_writeCommand(ILI9225_GRAM_DATA_REG); // WRITERAM   
    
    for(count=0;count<elements/2;count++) {
      ILI9225_writeData(buffer[count*2 + 1]);
      ILI9225_writeData(buffer[count*2]);
    }
    
    ILI9225_stop_transfer();
}

void ILI9225_clrScreen(void) {
    int count;
    ILI9225_start_transfer();
    
    for(count=0;count<ILI9225_LCD_WIDTH*ILI9225_LCD_HEIGHT*2;count++) {
      ILI9225_writeData(0);
    }
    
    ILI9225_stop_transfer();
}
