#ifndef __SPI_H__
#define __SPI_H__

#define SPI_SPEED_1_MHZ  1
#define SPI_SPEED_16_MHZ 2

#define SPI_MODE0 0  // POL,PHA 0,0
#define SPI_MODE1 1  //         0,1
#define SPI_MODE2 2  //         1,0
#define SPI_MODE3 3  //         1,1

void spi0_init(void);
void spi0_setspeed(unsigned char how);
void spi0_set_mode(unsigned char how);
void spi0_write(unsigned char what);
void spi0_write_bytes(unsigned char *what,unsigned int howmany);
void spi0_read_bytes(unsigned char *what,unsigned int howmany);
unsigned char spi0_read(void);
unsigned char spi0_write_read(unsigned char what);

void spi0_select(void);
void spi0_deselect(void);
#endif
