//////////////////////////////////////////////////////////////////////////////////////
//    o     o  o o o o  o o o       		HardWare: Ver1.0  BY DONGFNEG           //
//    o     o  o        o     o     		SoftWare: Ver1.0  BY DONGFENG           //
//    o     o  o        o     o 	   		VFD Module   : FUTABA GP1247AI 253*63   //
//    o     o  o o o o  o     o     		CONTROL MCU  : STC15W4K61S4         	//
//      o   o   o        o     o    		SYSTEM CLOCK : 33.1776MHz            	//
//        o     o        o o o      		Design  Date : 2016/04/16	         	//
//                                			E-mail: dongfeng.dream@126.com          //
//////////////////////////////////////////////////////////////////////////////////////

#include "GP1247AI.h"

void main(void)
{
    unsigned int i;
    Gpio_Init();
    Uart_1_Init(); // uart 1 initialization
    GP1247AI_Init();
    Delay_100mS();
    PWR_EN = 1;
    GP1247AI_Bright_Set(50);

    // GP1247AI_Disp_Pic(0, 0, 128, 64, logo_12864);

    // GP1247AI_Disp_Pic(0, 180, 32, 32, mute_3232);

    GP1247AI_Disp_ASC816_Str(1, 1, "NengTianshi is NO.1!");
    GP1247AI_Disp_Word_Str(1, 7, "电视在看谁", 1);

    while (1)
    {
        for (i = 0; i <= 1024; i++)
        {
            Delay_10mS();
            Delay_10mS();
            GP1247AI_Disp_ASC816(0, 27, i / 1000 + 0x30, 0);
            GP1247AI_Disp_ASC816(0, 28, i % 1000 / 100 + 0x30, 0);
            GP1247AI_Disp_ASC816(0, 29, i % 1000 % 100 / 10 + 0x30, 0);
            GP1247AI_Disp_ASC816(0, 30, i % 1000 % 100 % 10 + 0x30, 0);
        }
    }
}
