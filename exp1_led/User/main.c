/***********************************************************************
文件名称：main.C
功    能：
编写时间：
编 写 人：
注    意：
***********************************************************************/
#include "main.h"


int main(void) {
    /*
        ST固件库中的启动文件已经执行了 SystemInit() 函数，该函数在 system_stm32f4xx.c 文件，主要功能是
        配置CPU系统的时钟，内部Flash访问时序，配置FSMC用于外部SRAM等。
    */
    uint8_t keycode = 0;
    LED_Configuration();
    Key_Configuration();

    Multiple_LED_ON(0);
  //  LED1_ON;
  //  LED3_ON;
    while (1) {
        /* keycode = Key_Operate();
         Multiple_LED_ON(keycode);*/

        keycode = Key_Operate();
        //LED_Blink();
        if (keycode) {
            Multiple_LED_ON(keycode);
        }
    }
}
