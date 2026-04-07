//////////////////////////////////////////////////////////////////////////////////////
//    o     o  o o o o  o o o           HardWare: Ver1.0  BY DONGFNEG           //
//    o     o  o        o     o          SoftWare: Ver1.0  BY DONGFENG           //
//    o     o  o        o     o          VFD Module   : FUTABA GP1247AI 253*63   //
//    o     o  o o o o  o     o          CONTROL MCU  : STC15W4K61S4             //
//      o   o   o        o     o          SYSTEM CLOCK : 33.1776MHz               //
//        o     o        o o o            Design  Date : 2016/04/16               //
//                                 E-mail: dongfeng.dream@126.com          //
//////////////////////////////////////////////////////////////////////////////////////

#include "GP1247AI.h"

void main(void)
{
    Gpio_Init();
    GP1247AI_Init();
    Delay_100mS();
    PWR_EN = 1;

    // 测试：逐个像素点亮显示（1ms 间隔）
     //GP1247AI_Test_All_Pixels(1);

    // 测试：逐行扫描显示（100ms 间隔）
     GP1247AI_Test_Scan_Lines(1000);

    // 打字机效果显示标题
    //GP1247AI_English_Typewriter("NengTianshi is NO.1!", 0, 0, 10);

    // 像素级平滑滚动显示（使用显存）
    //GP1247AI_English_Scroll_Left_Smooth("NengTianshi is NO.1!", 3, 20, 0);
}
