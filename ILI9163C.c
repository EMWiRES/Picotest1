// Command data pin: GPIO14
// Reset pin: GPIO15
#include <pico/stdlib.h>
#include <hardware/gpio.h>

#include "ILI9163C.h"
#include "spi.h"
#include "timing.h"

// Pins definition.
extern const uint cmd_pin;
extern const uint reset_pin;

void ILI9163C_writeCommand(unsigned char cmd) {
    // Set CMD pin low.
	gpio_put(cmd_pin, 0);
    spi0_write(cmd);
}

void ILI9163C_writeData(unsigned char data) {
    // Set CMD pin high.
	gpio_put(cmd_pin, 1);
    spi0_write(data);
}

void ILI9163C_reset(void) {
  spi0_select();
  
  timing_sleep(50);
  gpio_put(reset_pin, 0);
  timing_sleep(50);
  gpio_put(reset_pin, 1);
  timing_sleep(50);  
}

void write_command(unsigned char which) {
  ILI9163C_writeCommand(which);
}

void write_data(unsigned char what) {
  ILI9163C_writeData(what);
}

void write_data16(unsigned short what) {
  ILI9163C_writeData(what >> 8);
  ILI9163C_writeData(what & 0xFF);
}

void delay_ms(unsigned short what) {
  timing_sleep(what);
}

void ILI9163C_init(void) {
    unsigned char i;

#if defined(__GAMMASET1)
    const unsigned char pGammaSet[15]= {0x36,0x29,0x12,0x22,0x1C,0x15,0x42,0xB7,0x2F,0x13,0x12,0x0A,0x11,0x0B,0x06};
    const unsigned char nGammaSet[15]= {0x09,0x16,0x2D,0x0D,0x13,0x15,0x40,0x48,0x53,0x0C,0x1D,0x25,0x2E,0x34,0x39};
#elif defined(__GAMMASET2)
    const unsigned char pGammaSet[15]= {0x3F,0x21,0x12,0x22,0x1C,0x15,0x42,0xB7,0x2F,0x13,0x02,0x0A,0x01,0x00,0x00};
    const unsigned char nGammaSet[15]= {0x09,0x18,0x2D,0x0D,0x13,0x15,0x40,0x48,0x53,0x0C,0x1D,0x25,0x2E,0x24,0x29};
#elif defined(__GAMMASET3)
    const unsigned char pGammaSet[15]= {0x3F,0x26,0x23,0x30,0x28,0x10,0x55,0xB7,0x40,0x19,0x10,0x1E,0x02,0x01,0x00};
    const unsigned char nGammaSet[15]= {0x09,0x18,0x2D,0x0D,0x13,0x15,0x40,0x48,0x53,0x0C,0x1D,0x25,0x2E,0x24,0x29};
#else
    const unsigned char pGammaSet[15]= {0x3F,0x25,0x1C,0x1E,0x20,0x12,0x2A,0x90,0x24,0x11,0x00,0x00,0x00,0x00,0x00};
    const unsigned char nGammaSet[15]= {0x20,0x20,0x20,0x20,0x05,0x15,0x00,0xA7,0x3D,0x18,0x25,0x2A,0x2B,0x2B,0x3A};
#endif
    
    // Setup command / data pin.
    gpio_init(cmd_pin);
    gpio_set_dir(cmd_pin, GPIO_OUT);
    gpio_put(cmd_pin, 1);
    
    // Setup reset pin.
    gpio_init(reset_pin);
	gpio_set_dir(reset_pin, GPIO_OUT);
	    
    ILI9163C_reset();
    
    write_command(CMD_SWRESET);//software reset
    delay_ms(500);
    write_command(CMD_SLPOUT);//exit sleep
    delay_ms(5);
    write_command(CMD_PIXFMT);//Set Color Format 16bit   
    write_data(0x05);
    delay_ms(5);
    write_command(CMD_GAMMASET);//default gamma curve 3
    write_data(0x04);//0x04
    delay_ms(1);
    write_command(CMD_GAMRSEL);//Enable Gamma adj    
    write_data(0x01); 
    delay_ms(1);
    write_command(CMD_NORML);
    write_command(CMD_DFUNCTR);

    write_data(0x0F);
    write_data(0x06);

    write_command(CMD_PGAMMAC);//Positive Gamma Correction Setting
    for (i=0;i<15;i++){
        write_data(pGammaSet[i]);
    }
    write_command(CMD_NGAMMAC);//Negative Gamma Correction Setting
    for (i=0;i<15;i++){
        write_data(nGammaSet[i]);
    }

    write_command(CMD_FRMCTR1);//Frame Rate Control (In normal mode/Full colors)
    write_data(0x08);//0x0C//0x08
    write_data(0x02);//0x14//0x08
    delay_ms(1);
    write_command(CMD_DINVCTR);//display insversion 
    write_data(0x07);
    delay_ms(1);
    write_command(CMD_PWCTR1);//Set VRH1[4:0] & VC[2:0] for VCI1 & GVDD   
    write_data(0x0A);//4.30 - 0x0A
    write_data(0x02);//0x05
    delay_ms(1);
    write_command(CMD_PWCTR2);//Set BT[2:0] for AVDD & VCL & VGH & VGL   
    write_data(0x02);
    delay_ms(1);
    write_command(CMD_VCOMCTR1);//Set VMH[6:0] & VML[6:0] for VOMH & VCOML   
    write_data(0x50);//0x50
    write_data(99);//0x5b
    delay_ms(1);
    write_command(CMD_VCOMOFFS);
    write_data(0);//0x40
    delay_ms(1);

    write_command(CMD_CLMADRS);//Set Column Address  
    write_data16(0x00); 
    write_data16(ILI9163C_LCD_WIDTH); 

    write_command(CMD_PGEADRS);//Set Page Address  
    write_data16(0X00); 
    write_data16(ILI9163C_LCD_HEIGHT);
    
    // set scroll area (thanks Masuda)
    write_command(CMD_VSCLLDEF);
    write_data16(0x0000);
    write_data16(ILI9163C_LCD_HEIGHT);
    write_data16(0);


    write_command(CMD_DISPON);//display ON 
    delay_ms(1);
    write_command(CMD_RAMWR);//Memory Write
    
    spi0_deselect();

    delay_ms(5);
}

void ILI9163C_start_transfer(void) {
    spi0_select();
    
    write_command(CMD_RAMWR);//Memory Write    
}

void ILI9163C_stop_transfer(void) {
    spi0_deselect();
}

void ILI9163C_transfer(unsigned char *buffer,int elements) {
    int count;
        
    ILI9163C_start_transfer();

    write_command(CMD_PGEADRS);//Set Page Address  
    write_data16(16); 
    write_data16(ILI9163C_LCD_HEIGHT);
    
    write_command(CMD_RAMWR);//Memory Write    

    for(count=0;count<elements/2;count++) {
      ILI9163C_writeData(buffer[count*2 + 1]);
      ILI9163C_writeData(buffer[count*2]);
    }
    
    ILI9163C_stop_transfer();
}

void ILI9163C_clrScreen(void) {
    int count;
    
    ILI9163C_start_transfer();
    
    for(count=0;count<ILI9163C_LCD_WIDTH*ILI9163C_LCD_HEIGHT*2;count++) {
      ILI9163C_writeData(0);
    }
    
    ILI9163C_stop_transfer();
}

