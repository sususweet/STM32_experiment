/**
  ******************************************************************************
  * File Name          : stm32f4xx_hal_msp.c
  * Description        : This file provides code for the MSP Initialization 
  *                      and de-Initialization codes.
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2017 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include <stm32f4xx.h>
#include "stm32f4xx_hal.h"
#include <stm32f407xx.h>
#include <string.h>


/*Experiment Config Settings*/
//#define EXP2_1
#define EXP2_2
/*Experiment Config Settings*/

#define USART1_RXDATASIZE 40
uint8_t rx_value = 'F';
uint8_t usart1_aRxData[USART1_RXDATASIZE];
unsigned char usart1_aRxData_Index = 0;

unsigned char usart1_check_flag = 0;

__IO ITStatus UartReady;

extern void _Error_Handler(char *, int);

void UART_Send(UART_HandleTypeDef *huart, char *msg);
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */
/**
  * Initializes the Global MSP.
  */
void HAL_MspInit(void) {
    /* USER CODE BEGIN MspInit 0 */

    /* USER CODE END MspInit 0 */

    HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

    /* System interrupt init*/
    /* MemoryManagement_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(MemoryManagement_IRQn, 0, 0);
    /* BusFault_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(BusFault_IRQn, 0, 0);
    /* UsageFault_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(UsageFault_IRQn, 0, 0);
    /* SVCall_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(SVCall_IRQn, 0, 0);
    /* DebugMonitor_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(DebugMonitor_IRQn, 0, 0);
    /* PendSV_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(PendSV_IRQn, 0, 0);
    /* SysTick_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);

    /* USER CODE BEGIN MspInit 1 */

    /* USER CODE END MspInit 1 */
}

void HAL_UART_MspInit(UART_HandleTypeDef *huart) {

    GPIO_InitTypeDef GPIO_InitStruct;
    if (huart->Instance == USART1) {
        /* USER CODE BEGIN USART1_MspInit 0 */

        /* USER CODE END USART1_MspInit 0 */
        /* Peripheral clock enable */
        __HAL_RCC_USART1_CLK_ENABLE();

        /**USART1 GPIO Configuration
        PA9     ------> USART1_TX
        PA10     ------> USART1_RX
        */
        GPIO_InitStruct.Pin = GPIO_PIN_9 | GPIO_PIN_10;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        /* USART1 interrupt Init */
        HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(USART1_IRQn);
        /* USER CODE BEGIN USART1_MspInit 1 */

        /* USER CODE END USART1_MspInit 1 */
    }

}

void HAL_UART_MspDeInit(UART_HandleTypeDef *huart) {

    if (huart->Instance == USART1) {
        /* USER CODE BEGIN USART1_MspDeInit 0 */

        /* USER CODE END USART1_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_USART1_CLK_DISABLE();

        /**USART1 GPIO Configuration
        PA9     ------> USART1_TX
        PA10     ------> USART1_RX
        */
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9 | GPIO_PIN_10);

        /* USART1 interrupt DeInit */
        HAL_NVIC_DisableIRQ(USART1_IRQn);
        /* USER CODE BEGIN USART1_MspDeInit 1 */

        /* USER CODE END USART1_MspDeInit 1 */
    }

}

/* USER CODE BEGIN 1 */

void UART_Send(UART_HandleTypeDef *huart, char *msg) {
    if (huart->Instance == USART1) {
        /*if (HAL_UART_Transmit_IT(huart, (uint8_t *) msg, (uint16_t) (strlen(msg))) != HAL_OK) {
            _Error_Handler(__FILE__, __LINE__);
        }*/
        uint8_t index = 9;
        for (; index >= 0; index--) {
            if (HAL_UART_Transmit_IT(huart, (uint8_t *) (msg + index), 1) != HAL_OK) {
                _Error_Handler(__FILE__, __LINE__);
            }
            // delay(1000);
        }
    } else if (huart->Instance == USART3) {

    }

    /*##-3- Wait for the end of the transfer ###################################*/
    while (UartReady != SET);

    /* Reset transmission flag */
    UartReady = RESET;

}

/**
  * @brief  Tx Transfer completed callback
  * @param  UartHandle: UART handle.
  * @note   This example shows a simple way to report end of IT Tx transfer, and
  *         you can add your own implementation.
  * @retval None
  */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *UartHandle) {
    /* Set transmission flag: transfer complete */
    UartReady = SET;
}

/**
  * @brief  Rx Transfer completed callback
  * @param  UartHandle: UART handle
  * @note   This example shows a simple way to report end of DMA Rx transfer, and
  *         you can add your own implementation.
  * @retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART1) {

        HAL_UART_Receive_IT(huart, &rx_value, 1);

#ifdef EXP2_1
        if (HAL_UART_Transmit_IT(huart, &rx_value, 1) != HAL_OK) {
            _Error_Handler(__FILE__, __LINE__);
        }
        /*##-3- Wait for the end of the transfer ###################################*/
        while (UartReady != SET);
        /* Reset transmission flag */
        UartReady = RESET;

        if (usart1_aRxData_Index >= USART1_RXDATASIZE) {
            usart1_aRxData_Index = 0;
        } else {
            usart1_aRxData[usart1_aRxData_Index++] = rx_value;
        }
#endif

#ifdef EXP2_2
        if (usart1_aRxData_Index >= 9) {
            usart1_aRxData[usart1_aRxData_Index++] = rx_value;
            if (rx_value == 0xff){
                UART_Send(huart, (char *) usart1_aRxData);
            }
            usart1_check_flag = 0;
            usart1_aRxData_Index = 0;
        } else if (usart1_check_flag == 1){
            usart1_aRxData[usart1_aRxData_Index++] = rx_value;
        } else if (rx_value == 0xaa){
            usart1_check_flag = 1;
            usart1_aRxData_Index = 0;
            usart1_aRxData[usart1_aRxData_Index++] = rx_value;
        }
#endif

    } else if (huart->Instance == USART3) {

        HAL_UART_Receive_IT(huart, (uint8_t *) (&rx_value), 1);


    }
    UartReady = SET;
    // HAL_UART_Receive_IT(UartHandle, (uint8_t *)&aRxBuffer, 1) ;
    /* Set transmission flag: transfer complete */
}

/**
  * @brief  UART error callbacks
  * @param  UartHandle: UART handle
  * @note   This example shows a simple way to report transfer error, and you can
  *         add your own implementation.
  * @retval None
  */
void HAL_UART_ErrorCallback(UART_HandleTypeDef *UartHandle) {
    _Error_Handler(__FILE__, __LINE__);
}

/* USER CODE END 1 */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
