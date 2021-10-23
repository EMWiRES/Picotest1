/*
 * The peripherals are mapped as follows:
 *
 */
#include "digitalIO.h"

// Pins definition.
const unsigned int cmd_pin = 14;
const unsigned int reset_pin = 15;


void digitalIO_init(void) {  
  // Setup all the IO pins that are not directly controlled by a peripheral.
  // Peripheral IO pins will be setup in their respective source files.
    
}
