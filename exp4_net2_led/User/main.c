/***********************************************************************
文件名称：main.C
功    能：
编写时间：
编 写 人：
注    意：
***********************************************************************/
#include "main.h"

/* Private define ------------------------------------------------------------*/
#define SYSTEMTICK_PERIOD_MS  10


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint32_t LocalTime = 0; /* this variable is used to create a time reference incremented by 10ms */
uint32_t timingdelay;

int main(void) {
    /*
        ST固件库中的启动文件已经执行了 SystemInit() 函数，该函数在 system_stm32f4xx.c 文件，主要功能是
        配置CPU系统的时钟，内部Flash访问时序，配置FSMC用于外部SRAM等。
    */
    /* configure ethernet (GPIOs, clocks, MAC, DMA) */
    ETH_BSP_Config();
    /* Initilaize the LwIP stack */
    LwIP_Init();
    UDP_server_init();
    /* Infinite loop */

    I2C_Configuration();
    OLED_Init();
    DelayMs(10);
    OLED_CLS();//清屏
    OLED_ShowStr(0, 0, "zzzzzzzzzzzz", 2);
    while (1) {

        /* handle periodic timers for LwIP */
        LwIP_Periodic_Handle(LocalTime);
    }
}

/**
  * @brief  Inserts a delay time.
  * @param  nCount: number of 10ms periods to wait for.
  * @retval None
  */
void Delay(uint32_t nCount) {
    /* Capture the current local time */
    timingdelay = LocalTime + nCount;

    /* wait until the desired delay finish */
    while (timingdelay > LocalTime) {
    }
}


void Time_Update(void) {
    LocalTime += SYSTEMTICK_PERIOD_MS;
}


#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {}
}
#endif


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
