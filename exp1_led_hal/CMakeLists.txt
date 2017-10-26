cmake_minimum_required(VERSION 3.8)
project(exp1_led_hal)

set(CMAKE_CXX_STANDARD 11)

INCLUDE_DIRECTORIES(./Inc
        ./Drivers/STM32F4xx_HAL_Driver/Inc
        ./Drivers/STM32F4xx_HAL_Driver/Inc/Legacy
        ./Drivers/CMSIS/Device/ST/STM32F4xx/Include
        ./Drivers/CMSIS/Include)

set(SOURCE_FILES
        Inc/main.h
        Inc/stm32f4xx_hal_conf.h
        Inc/stm32f4xx_it.h
        Src/main.c
        Src/stm32f4xx_hal_msp.c
        Src/stm32f4xx_it.c
        Src/system_stm32f4xx.c
        Src/LED.C
        Src/Keyboard.c
        Inc/LED.H
        Inc/Keyboard.h)

add_executable(exp1_led_hal ${SOURCE_FILES})