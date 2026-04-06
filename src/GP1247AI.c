//////////////////////////////////////////////////////////////////////////////////////
//    o     o  o o o o  o o o       		HardWare: Ver1.0  BY DONGFNEG           //
//    o     o  o        o     o     		SoftWare: Ver1.0  BY DONGFENG           //
//    o     o  o        o     o 	   		VFD Module   : FUTABA GP1247AI 253*63   //
//    o     o  o o o o  o     o     		CONTROL MCU  : STC15W4K61S4         	//
//      o   o   o        o     o        	SYSTEM CLOCK : 24.000MHz            	//
//        o     o        o o o          	Design  Date : 2016/04/16	         	//
//                                			E-mail: dongfeng.dream@126.com          //
//////////////////////////////////////////////////////////////////////////////////////

#include <STC15W.H>
#include <intrins.h>
#include <stdio.h>
#include <string.h>

#include "ASC1224.h"
#include "ASC57.h"
#include "ASC816.h"
#include "CHINESE.h"
#include "GP1247AI.h"
#include "amp_graph.h"

#define FOSC 24000000L

sbit VFD_CS = P2 ^ 1;  // SS   , GP1247AI SPI CS pin
sbit VFD_DAT = P2 ^ 2; // MOSI , GP1247AI SPI DATA pin
sbit VFD_SCK = P2 ^ 0; // SCLK , GP1247AI SPI CLOCK pin
sbit VFD_RST = P2 ^ 3; // RST

// sbit PWR_EN = P3 ^ 7;

sbit RSL_A = P3 ^ 6; // Left encode key pin A
sbit RSL_K = P3 ^ 5; // Left encode key pin K
sbit RSL_B = P3 ^ 4; // Left encode key pin B

sbit RSR_A = P2 ^ 4; // Right encode key pin A
sbit RSR_K = P2 ^ 5; // Right encode key pin K
sbit RSR_B = P2 ^ 6; // Right encode key pin B

sbit IR_DAT = P3 ^ 2; // IR data pin

sbit EXPR_C = P5 ^ 5; // external power relay control

sbit RELAY_1 = P5 ^ 4; // Relay 1 control
sbit RELAY_2 = P1 ^ 7; // Relay 2 control
sbit RELAY_3 = P1 ^ 6; // Relay 3 control
sbit RELAY_4 = P1 ^ 5; // Relay 4 control

sbit VC_MUTE = P1 ^ 4; // volume control mute
sbit VC_DAT = P1 ^ 3;  // Volume control data
sbit VC_CLK = P1 ^ 2;  // Volume control clock
sbit VC_CS = P1 ^ 1;   // Volume control chip select

sbit ADC_AU = P1 ^ 0;  // Audio level meter ADC
sbit EQ_SEL4 = P0 ^ 3; // EQ chip output select 4
sbit EQ_SEL3 = P0 ^ 2; // EQ chip output select 3
sbit EQ_SEL2 = P0 ^ 1; // EQ chip output select 2
sbit EQ_SEL1 = P0 ^ 0; // EQ chip output select 1
sbit EQ_LRSW = P2 ^ 7; // EQ left/right channel switch

//---------------------------------------------------------------------------------------//
/**
 * @brief 延时 1 微秒
 * @note 适用于 24MHz 晶振频率
 */
void Delay_1uS() //@24.000MHz
{
    unsigned char i;
    i = 6;
    while (--i)
        ;
}

/**
 * @brief 延时 10 微秒
 * @note 适用于 24MHz 晶振频率
 */
void Delay_10uS() //@24MHz
{
    unsigned char i;
    i = 78;
    while (--i)
        ;
}

/**
 * @brief 延时 1 毫秒
 * @note 适用于 24MHz 晶振频率
 */
void Delay_1mS() //@24MHz
{
    unsigned char i, j;
    _nop_();
    i = 32;
    j = 40;
    do
    {
        while (--j)
            ;
    } while (--i);
}

/**
 * @brief 延时 10 毫秒
 * @note 适用于 24MHz 晶振频率
 */
void Delay_10mS() //@24MHz
{
    unsigned char i, j, k;
    i = 2;
    j = 56;
    k = 172;
    do
    {
        do
        {
            while (--k)
                ;
        } while (--j);
    } while (--i);
}

/**
 * @brief 延时 100 毫秒
 * @note 适用于 24MHz 晶振频率
 */
void Delay_100mS() //@24MHz
{
    unsigned char i, j, k;
    _nop_();
    _nop_();
    i = 13;
    j = 45;
    k = 214;
    do
    {
        do
        {
            while (--k)
                ;
        } while (--j);
    } while (--i);
}

/**
 * @brief 延时 1 秒
 * @note 适用于 24MHz 晶振频率
 */
void Delay_1S() //@24MHz
{
    unsigned char i, j, k;
    _nop_();
    _nop_();
    i = 122;
    j = 193;
    k = 128;
    do
    {
        do
        {
            while (--k)
                ;
        } while (--j);
    } while (--i);
}

//---------------------------------------------------------------------------------------//
//---------------------------------------------------------------------------------------//
/**
 * @brief 通过软件 SPI 发送一个字节数据
 * @param dat 要发送的 8 位数据（LSB 优先）
 * @note 使用 GPIO 模拟 SPI 时序
 */
void Spi_Send_Byte(unsigned char dat)
{
    unsigned char i;
    for (i = 0; i < 8; i++)
    {
        VFD_SCK = 0;
        VFD_DAT = dat & 0x01; // LSB first
        VFD_SCK = 1;
        dat = dat >> 1;
    }
}

//---------------------------------------------------------------------------------------//

//---------------------------------------------------------------------------------------//
/**
 * @brief 初始化 GP1247AI VFD 显示芯片
 * @note 执行完整的初始化序列，包括软件复位、内存清除、显示模式设置等
 */
void GP1247AI_Init(void)
{
    VFD_RST = 0;
    Delay_10mS();
    VFD_RST = 1;
    Delay_10mS();

    // Software reset
    VFD_CS = 0;
    Spi_Send_Byte(0xaa);
    VFD_CS = 1;

    Delay_1mS();
    Delay_1mS();

    // Memory map clear
    VFD_CS = 0;
    Spi_Send_Byte(0x55);
    VFD_CS = 1;

    Delay_10mS();
    Delay_10mS();

    // VFD mode setting
    VFD_CS = 0;
    Spi_Send_Byte(0xcc);
    Delay_1uS();
    Spi_Send_Byte(0x02);
    Spi_Send_Byte(0x00);
    VFD_CS = 1;

    // Display area setting
    VFD_CS = 0;
    Spi_Send_Byte(0xe0);
    Delay_1uS();
    Spi_Send_Byte(0xfc); // 252
    Spi_Send_Byte(0x3e); // 62
    Spi_Send_Byte(0x00);
    Spi_Send_Byte(0x20);
    Spi_Send_Byte(0x80);
    Spi_Send_Byte(0x80);
    Spi_Send_Byte(0x80);
    VFD_CS = 1;
    // Internal speed setting
    VFD_CS = 0;
    Spi_Send_Byte(0xb1);
    Delay_1uS();
    Spi_Send_Byte(0x20);
    Spi_Send_Byte(0x3f);
    Spi_Send_Byte(0x00);
    Spi_Send_Byte(0x01);
    VFD_CS = 1;
    // DW position setting
    VFD_CS = 0;
    Spi_Send_Byte(0xd0);
    Delay_1uS();
    Spi_Send_Byte(0x00);
    Spi_Send_Byte(0xff); // X position
    Spi_Send_Byte(0x40); // Y position
    VFD_CS = 1;
    // Dimming level setting
    VFD_CS = 0;
    Spi_Send_Byte(0xa0);
    Delay_1uS();
    Spi_Send_Byte(0x0c);
    Spi_Send_Byte(0x40); // Dimming data
    VFD_CS = 1;
    // Display mode setting
    VFD_CS = 0;
    Spi_Send_Byte(0x80);
    Delay_1uS();
    Spi_Send_Byte(0x40); // DW1,DW2,*,SC,HS,LS,*,NP
    // DW1,DW2(00-DW1,01-DW2,10-DW1,11-DW1&DW2 synthesis)
    // SC , 0-scan start  1-scan stop
    // HS , 1-all on segment ,  LS , 1-all off segment
    // NP , 1-output reverse
    VFD_CS = 1;
}

/**
 * @brief 设置 VFD 显示亮度
 * @param VFD_BRT_DATA 亮度值，范围 0~1024
 * @note 0 为最暗，1024 为最亮
 */
void GP1247AI_Bright_Set(unsigned int VFD_BRT_DATA)
{
    VFD_CS = 0;
    Spi_Send_Byte(0xa0); //
    Delay_1uS();
    Spi_Send_Byte((VFD_BRT_DATA >> 8) & 0x3); //[9:8] of brightness data
    Spi_Send_Byte(VFD_BRT_DATA & 0x00ff);     //[7:0] of brightness data
    VFD_CS = 1;
}

/**
 * @brief 在指定位置显示一个 8x16 点阵 ASCII 字符
 * @param VFD_X X 坐标（字符位置，从 0 开始）
 * @param VFD_Y Y 坐标（行位置，从 0 开始）
 * @param DAT 要显示的 ASCII 字符
 * @param BK 背景模式：1-正常显示，0-反色显示
 */
void GP1247AI_Disp_ASC816(unsigned char VFD_X, unsigned char VFD_Y, unsigned char DAT, bit BK)
{
    unsigned char i;

    VFD_CS = 0;
    Spi_Send_Byte(0xf0); // write display data cmd
    Delay_1uS();
    Spi_Send_Byte(VFD_Y * 8); // start Y ,
    Spi_Send_Byte(VFD_X * 4); // start X ,
    Spi_Send_Byte(15);        // Y+1 , after 16 bits received

    for (i = 0; i < 16; i++)
    {
        if (BK)
            Spi_Send_Byte(ASC816[((DAT - 0x20) << 4) + i]);
        else
            Spi_Send_Byte(~(ASC816[((DAT - 0x20) << 4) + i]));
    }
    VFD_CS = 1;
}

/**
 * @brief 在指定位置显示 8x16 点阵 ASCII 字符串
 * @param VFD_X 起始 X 坐标（字符位置）
 * @param VFD_Y 起始 Y 坐标（行位置）
 * @param Str 要显示的 ASCII 字符串指针
 * @note 字符串会自动换行显示
 */
void GP1247AI_Disp_ASC816_Str(unsigned char VFD_X, unsigned char VFD_Y, unsigned char *Str)
{
    while (*Str)
    {
        GP1247AI_Disp_ASC816(VFD_X, VFD_Y++, *Str++, 1);
    }
}

/**
 * @brief 在指定位置显示一个 16x16 点阵汉字
 * @param VFD_X X 坐标（字位置，从 0 开始）
 * @param VFD_Y Y 坐标（行位置，从 0 开始）
 * @param DAT 要显示的汉字内码指针（2 字节）
 * @param BK 背景模式：1-正常显示，0-反色显示
 */
void GP1247AI_Disp_Word(unsigned char VFD_X, unsigned char VFD_Y, unsigned char *DAT, bit BK)
{
    unsigned char Index_Count, Index_Size;
    unsigned char *CODE;
    unsigned char i;

    VFD_CS = 0;
    Spi_Send_Byte(0xf0); // write display data cmd
    Delay_1uS();
    Spi_Send_Byte(VFD_Y * 16); // start X ,
    Spi_Send_Byte(VFD_X * 16); // start Y ,
    Spi_Send_Byte(15);         // Y+1 , after 16 bits received

    Index_Size = sizeof(GB16) / 34; // Calculate the chinese word table size,34=Msk size + Index size=32+2

    for (Index_Count = 0; Index_Count < Index_Size; Index_Count++)
    {
        if ((GB16[Index_Count].Index[0] == *DAT) && (GB16[Index_Count].Index[1] == *(DAT + 1)))
        {
            CODE = GB16[Index_Count].Msk;
            break;
        }
    }

    for (i = 0; i < 32; i++)
    {
        if (BK)
            Spi_Send_Byte(*(CODE + i));
        else
            Spi_Send_Byte(~*(CODE + i));
    }

    VFD_CS = 1;
}

/**
 * @brief 在指定位置显示 16x16 点阵汉字字符串
 * @param VFD_X 起始 X 坐标（字位置）
 * @param VFD_Y 起始 Y 坐标（行位置）
 * @param Str 要显示的汉字字符串指针
 * @param BK 背景模式：1-正常显示，0-反色显示
 * @note 字符串会自动换行显示
 */
void GP1247AI_Disp_Word_Str(unsigned char VFD_X, unsigned char VFD_Y, unsigned char *Str, bit BK)
{
    while (*Str)
    {
        GP1247AI_Disp_Word(VFD_X, VFD_Y++, Str, BK);
        Str += 2;
    }
}

/**
 * @brief 在指定位置显示一个 19x19 点阵汉字
 * @param VFD_X X 坐标（字位置，从 0 开始）
 * @param VFD_Y Y 坐标（行位置，从 0 开始）
 * @param DAT 要显示的汉字内码指针（2 字节）
 * @param BK 背景模式：1-正常显示，0-反色显示
 */
void GP1247AI_Disp_Word_19(unsigned char VFD_X, unsigned char VFD_Y, unsigned char *DAT, bit BK)
{
    unsigned char Index_Count, Index_Size;
    unsigned char *CODE;
    unsigned char i;

    VFD_CS = 0;
    Spi_Send_Byte(0xf0); // write display data cmd
    Delay_1uS();
    Spi_Send_Byte(VFD_Y * 19); // start X ,
    Spi_Send_Byte(VFD_X * 24); // start Y ,
    Spi_Send_Byte(23);         // Y+1 , after 24 bits received

    Index_Size = sizeof(GB19) / 59; // Calculate the chinese word table size,59=Msk size + Index size=57+2

    for (Index_Count = 0; Index_Count < Index_Size; Index_Count++)
    {
        if ((GB19[Index_Count].Index[0] == *DAT) && (GB19[Index_Count].Index[1] == *(DAT + 1)))
        {
            CODE = GB19[Index_Count].Msk;
            break;
        }
    }

    for (i = 0; i < 57; i++)
    {
        if (BK)
            Spi_Send_Byte(*(CODE + i));
        else
            Spi_Send_Byte(~*(CODE + i));
    }

    VFD_CS = 1;
}

/**
 * @brief 在指定位置显示 19x19 点阵汉字字符串
 * @param VFD_X 起始 X 坐标（字位置）
 * @param VFD_Y 起始 Y 坐标（行位置）
 * @param Str 要显示的汉字字符串指针
 * @param BK 背景模式：1-正常显示，0-反色显示
 * @note 字符串会自动换行显示
 */
void GP1247AI_Disp_Word_Str_19(unsigned char VFD_X, unsigned char VFD_Y, unsigned char *Str, bit BK)
{
    while (*Str)
    {
        GP1247AI_Disp_Word_19(VFD_X, VFD_Y++, Str, BK);
        Str += 2;
    }
}

/**
 * @brief 在指定位置显示图片
 * @param VFD_X 起始 X 坐标（像素位置）
 * @param VFD_Y 起始 Y 坐标（像素位置）
 * @param PIC_W 图片宽度（像素）
 * @param PIC_H 图片高度（像素）
 * @param PIC_DAT 图片数据指针
 * @note 图片数据格式：纵向取模，高位在下
 */
void GP1247AI_Disp_Pic(unsigned char VFD_X, unsigned char VFD_Y, unsigned char PIC_W, unsigned char PIC_H, unsigned char *PIC_DAT)
{
    unsigned int i;
    VFD_CS = 0;
    Spi_Send_Byte(0xf0);
    Delay_1uS();
    Spi_Send_Byte(VFD_Y);
    Spi_Send_Byte(VFD_X);
    Spi_Send_Byte(PIC_H - 1);
    for (i = 0; i < ((PIC_H / 8) + ((PIC_H % 8) ? 1 : 0)) * PIC_W; i++)
        Spi_Send_Byte(*PIC_DAT++);
    VFD_CS = 1;
}

//---------------------------------------------------------------------------------------//

//---------------------------------------------------------------------------------------//
/**
 * @brief 初始化 GPIO 端口模式
 * @note 配置 P0-P5 为准双向 IO 模式
 */
void Gpio_Init(void)
{
    P0M1 = 0x00;
    P0M0 = 0x00; // P0 准双向 IO
    P1M1 = 0x00;
    P1M0 = 0xc4; // P1 准双向 IO
    P2M1 = 0x00;
    P2M0 = 0x00; // P2 准双向 IO
    P3M1 = 0x00;
    P3M0 = 0x00; // P3 准双向 IO
    P4M1 = 0x00;
    P4M0 = 0x00; // P4 准双向 IO
    P5M1 = 0x00;
    P5M0 = 0x00; // P5 准双向 IO
}

/**
 * @brief 初始化硬件 SPI 接口
 * @note 配置 SPI 为主机模式，时钟为 Fosc/4
 */
void Spi_Init(void)
{
    SPCTL = 0xfd; // SSIG，SPEN，DORD，MSTR，CPOL，CPHA，SPR1，SPR0。1111,1100
    // SPCTL = SSIG | SPEN | DORD | MSTR | CPOL | CPHA | 0x00;
    // clear flags
    SPSTAT = 0xc0; // clear flag SPIF and WCOL
    // AUXR |= 0x08;   //enable SPI interrupt , ESPI = 1  , 0000,1000
}

//---------------------------------------------------------------------------------------//
// UART functions
/**
 * @brief 初始化串口 1，波特率 9600bps@24MHz
 * @note 使用定时器 1 作为波特率发生器，8 位数据，无校验位
 */
void Uart_1_Init(void) // 9600bps@24.000MHz
{
    SCON = 0x50;  // 8 位数据，可变波特率
    AUXR |= 0x40; // 定时器 1 时钟为 Fosc，即 1T
    AUXR &= 0xFE; // 串口 1 选择定时器 1 为波特率发生器
    TMOD &= 0x0F; // 设定定时器 1 为 16 位自动重装方式
    TL1 = 0x8F;   // 设定定时初值
    TH1 = 0xFD;   // 设定定时初值
    ET1 = 0;      // 禁止定时器 1 中断
    TR1 = 1;      // 启动定时器 1
}

/**
 * @brief 通过串口发送一个字节数据
 * @param dat 要发送的 8 位数据
 * @note 阻塞式发送，等待发送完成后才返回
 */
void Uart_Send_Data(unsigned char dat)
{
    SBUF = dat;
    while (!TI)
        ;
    TI = 0;
}

/**
 * @brief 通过串口发送字符串
 * @param str 要发送的字符串指针
 * @note 自动发送字符串中的所有字符，直到遇到结束符'\0'
 */
void Uart_Send_String(unsigned char *str)
{
    while (*str)
    {
        Uart_Send_Data(*str++);
    }
}
