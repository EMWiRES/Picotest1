/*
 * Connect SPI to GPIO16 .. GPIO19.
 *
 * GPIO16 : MISO
 * GPIO17 : CS
 * GPIO18 : SCK
 * GPIO19 : MOSI
 *
 * Number of convenience functions for interacting with SPI.
 *
 * (c) 2021 EMWiRES Allard van der Bas.
 *
 */
#include <pico/stdlib.h>
#include <hardware/gpio.h>
#include <hardware/spi.h>  
#include "spi.h"

// SPI Pins definition.
const uint cs_pin = 17;
const uint sck_pin = 18;
const uint mosi_pin = 19;
const uint miso_pin = 16;

/*
 * Setup.
 */
void spi0_init(void) {
	unsigned char data;
	
	// Initialize CS pin high
    gpio_init(cs_pin);
    gpio_set_dir(cs_pin, GPIO_OUT);
    gpio_put(cs_pin, 1);
    
    // Set SPI format
    spi_set_format( spi0,   // SPI instance
                    8,      // Number of bits per transfer
                    1,      // Polarity (CPOL)
                    1,      // Phase (CPHA)
                    SPI_MSB_FIRST);
                    
    // Initialize SPI port at 1 MHz
    spi_init(spi0, 28 * 1000 * 1000);
    
    // Initialize SPI pins
    gpio_set_function(sck_pin, GPIO_FUNC_SPI);
    gpio_set_function(mosi_pin, GPIO_FUNC_SPI);
    gpio_set_function(miso_pin, GPIO_FUNC_SPI);
}

/*
 * Change speed.
 */
void spi0_setspeed(unsigned char how) {

  if(how == SPI_SPEED_1_MHZ) {
	spi_init(spi0, 1000 * 1000);
  } else {
	spi_init(spi0, 16*1000 * 1000);
  }

}

/*
 * Set mode.
 */
void spi0_set_mode(unsigned char how) {

  switch(how) {      
    case SPI_MODE1:
      spi_set_format( spi0,   // SPI instance
      					8,      // Number of bits per transfer
                    	0,      // Polarity (CPOL)
                    	1,      // Phase (CPHA)
                    	SPI_MSB_FIRST);
      break;

    case SPI_MODE2:
      spi_set_format( spi0,   // SPI instance
      					8,      // Number of bits per transfer
                    	1,      // Polarity (CPOL)
                    	0,      // Phase (CPHA)
                    	SPI_MSB_FIRST);
      break;

    case SPI_MODE3:
		spi_set_format( spi0,   // SPI instance
      					8,      // Number of bits per transfer
                    	1,      // Polarity (CPOL)
                    	1,      // Phase (CPHA)
                    	SPI_MSB_FIRST);
      break;

    case SPI_MODE0:
    default:
		spi_set_format( spi0,   // SPI instance
      					8,      // Number of bits per transfer
                    	0,      // Polarity (CPOL)
                    	0,      // Phase (CPHA)
                    	SPI_MSB_FIRST);
      break;
  }
  
}

/*
 * Write a single byte.
 */
void spi0_write(unsigned char what) {
	spi_write_blocking(spi0, &what, 1);
}

/*
 * Write multiple consecutive bytes.
 */
void spi0_write_bytes(unsigned char *what,unsigned int howmany) {
	spi_write_blocking(spi0, what, howmany);
}

/*
 * Read a single byte.
 */
unsigned char spi0_read(void) {
  unsigned char data;
  
  spi_read_blocking(spi0, 0, &data, 1);
  
  return(data);
}

/*
 * Read multiple consecutive bytes.
 */
void spi0_read_bytes(unsigned char *what,unsigned int howmany) {
  spi_read_blocking(spi0, 0, what, howmany);
}

/*
 * Write a byte read an answer.
 */
unsigned char spi0_write_read(unsigned char what) {
  unsigned char data;
  
  spi_write_blocking(spi0, &what, 1);
  spi_read_blocking(spi0, 0, &data, 1);
  
  return(data);
}

/*
 * Select or deselect the SPI device.
 */
void spi0_select(void) {
	asm volatile("nop \n nop \n nop");
	gpio_put(cs_pin, 0);
	asm volatile("nop \n nop \n nop");
}

void spi0_deselect(void) {
	asm volatile("nop \n nop \n nop");
	gpio_put(cs_pin, 1);
	asm volatile("nop \n nop \n nop");
}

