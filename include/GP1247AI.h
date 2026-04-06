#ifndef __GP1247AI_H__
#define __GP1247AI_H__

#include <STC15W.H>

// Global variables
extern unsigned char xdata DP_BUF[2048];
extern bit Uart_Snd_Flag;

// Power control - defined in GP1247AI.c
#define PWR_EN P37

// Bitmap data extern declarations
extern unsigned char code logo_12864[];
extern unsigned char code mute_3232[];
extern unsigned char code space_3232[];
extern unsigned char code set_1616[];
extern unsigned char code narrow_l[];
extern unsigned char code narrow_r[];
extern unsigned char code space_1616[];

// Delay functions
void Delay_1uS();
void Delay_10uS();
void Delay_1mS();
void Delay_10mS();
void Delay_100mS();
void Delay_1S();

// SPI functions
void Spi_Send_Byte(unsigned char dat);
void GP1247AI_Init(void);
void GP1247AI_Bright_Set(unsigned int VFD_BRT_DATA);

// Display functions
void GP1247AI_Disp_ASC816(unsigned char VFD_X, unsigned char VFD_Y, unsigned char DAT, bit BK);
void GP1247AI_Disp_ASC816_Str(unsigned char VFD_X, unsigned char VFD_Y, unsigned char *Str);
void GP1247AI_Disp_Word(unsigned char VFD_X, unsigned char VFD_Y, unsigned char *DAT, bit BK);
void GP1247AI_Disp_Word_Str(unsigned char VFD_X, unsigned char VFD_Y, unsigned char *Str, bit BK);
void GP1247AI_Disp_Word_19(unsigned char VFD_X, unsigned char VFD_Y, unsigned char *DAT, bit BK);
void GP1247AI_Disp_Word_Str_19(unsigned char VFD_X, unsigned char VFD_Y, unsigned char *Str, bit BK);
void GP1247AI_Disp_Pic(unsigned char VFD_X, unsigned char VFD_Y, unsigned char PIC_W, unsigned char PIC_H, unsigned char *PIC_DAT);

// Initialization functions
void Gpio_Init(void);
void Spi_Init(void);
void Uart_1_Init(void);

// UART functions
void Uart_Send_Data(unsigned char dat);
void Uart_Send_String(unsigned char *str);

#endif
