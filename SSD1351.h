#ifndef __SSD1351_H__
#define __SSD1351_H__

#define SSD1351_CMD_SETCOLUMN      0x15 ///< See datasheet
#define SSD1351_CMD_SETROW         0x75 ///< See datasheet
#define SSD1351_CMD_WRITERAM       0x5C ///< See datasheet
#define SSD1351_CMD_READRAM        0x5D ///< Not currently used
#define SSD1351_CMD_SETREMAP       0xA0 ///< See datasheet
#define SSD1351_CMD_STARTLINE      0xA1 ///< See datasheet
#define SSD1351_CMD_DISPLAYOFFSET  0xA2 ///< See datasheet
#define SSD1351_CMD_DISPLAYALLOFF  0xA4 ///< Not currently used
#define SSD1351_CMD_DISPLAYALLON   0xA5 ///< Not currently used
#define SSD1351_CMD_NORMALDISPLAY  0xA6 ///< See datasheet
#define SSD1351_CMD_INVERTDISPLAY  0xA7 ///< See datasheet
#define SSD1351_CMD_FUNCTIONSELECT 0xAB ///< See datasheet
#define SSD1351_CMD_DISPLAYOFF     0xAE ///< See datasheet
#define SSD1351_CMD_DISPLAYON      0xAF ///< See datasheet
#define SSD1351_CMD_PRECHARGE      0xB1 ///< See datasheet
#define SSD1351_CMD_DISPLAYENHANCE 0xB2 ///< Not currently used
#define SSD1351_CMD_CLOCKDIV       0xB3 ///< See datasheet
#define SSD1351_CMD_SETVSL         0xB4 ///< See datasheet
#define SSD1351_CMD_SETGPIO        0xB5 ///< See datasheet
#define SSD1351_CMD_PRECHARGE2     0xB6 ///< See datasheet
#define SSD1351_CMD_SETGRAY        0xB8 ///< Not currently used
#define SSD1351_CMD_USELUT         0xB9 ///< Not currently used
#define SSD1351_CMD_PRECHARGELEVEL 0xBB ///< Not currently used
#define SSD1351_CMD_VCOMH          0xBE ///< See datasheet
#define SSD1351_CMD_CONTRASTABC    0xC1 ///< See datasheet
#define SSD1351_CMD_CONTRASTMASTER 0xC7 ///< See datasheet
#define SSD1351_CMD_MUXRATIO       0xCA ///< See datasheet
#define SSD1351_CMD_COMMANDLOCK    0xFD ///< See datasheet
#define SSD1351_CMD_HORIZSCROLL    0x96 ///< Not currently used
#define SSD1351_CMD_STOPSCROLL     0x9E ///< Not currently used
#define SSD1351_CMD_STARTSCROLL    0x9F ///< Not currently used

#define SSD1351WIDTH  128
#define SSD1351HEIGHT 128

void SSD1351_init(void);
void SSD1351_WriteData(unsigned char data);
void SSD1351_WriteCommand(unsigned char cmd);

void SSD1351_transfer(unsigned char *buffer,int elements);
#endif
