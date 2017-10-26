cmake_minimum_required(VERSION 3.8)
project(exp1_led)

set(CMAKE_CXX_STANDARD 11)

INCLUDE_DIRECTORIES(../Libraries/CMSIS/Include
        ../Libraries/CMSIS/Device/ST/STM32F4xx/Include
        ../Libraries/STM32F4xx_StdPeriph_Driver/inc
        ../Libraries/CMSIS/Device/ST/STM32F4xx/Include
        ../Libraries/CMSIS/Include)


set(SOURCE_FILES
        User/LED.C
        User/LED.H
        User/main.c
        User/main.h
        User/stm32f4xx_assert.c
        User/stm32f4xx_conf.h
        User/stm32f4xx_it.c
        User/stm32f4xx_it.h
        User/system_stm32f4xx.c User/Keyboard.c User/Keyboard.h)

add_executable(exp1_led ${SOURCE_FILES})