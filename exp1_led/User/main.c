/***********************************************************************
�ļ����ƣ�main.C
��    �ܣ�
��дʱ�䣺
�� д �ˣ�
ע    �⣺
***********************************************************************/
#include "main.h"


int main(void) {
    /*
        ST�̼����е������ļ��Ѿ�ִ���� SystemInit() �������ú����� system_stm32f4xx.c �ļ�����Ҫ������
        ����CPUϵͳ��ʱ�ӣ��ڲ�Flash����ʱ������FSMC�����ⲿSRAM�ȡ�
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
