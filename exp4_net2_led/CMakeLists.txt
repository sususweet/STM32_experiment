cmake_minimum_required(VERSION 3.8)
project(exp4_net2_led)

set(CMAKE_CXX_STANDARD 11)

INCLUDE_DIRECTORIES(../Libraries/CMSIS/Include
        ../Libraries/CMSIS/Device/ST/STM32F4xx/Include
        ../Libraries/STM32F4xx_StdPeriph_Driver/inc
        ../Libraries/STM32F4x7_ETH_Driver/inc
        ../Libraries/CMSIS/Include
        ./Utilities/Third_Party/lwip_v1.3.2
        ./Utilities/Third_Party/lwip_v1.3.2/src/include
        ./Utilities/Third_Party/lwip_v1.3.2/src/include/ipv4
        ./Utilities/Third_Party/lwip_v1.3.2/src/include/lwip
        ./Utilities/Third_Party/lwip_v1.3.2/src/include/netif
        ./Utilities/Third_Party/lwip_v1.3.2/src/netif
        ./Utilities/Third_Party/lwip_v1.3.2/src/netif/ppp
        ./Utilities/Third_Party/lwip_v1.3.2/src/core/ipv4
        ./Utilities/Third_Party/lwip_v1.3.2/src/api
        ./Utilities/Third_Party/lwip_v1.3.2/port/STM32F4x7
        ./Utilities/Third_Party/lwip_v1.3.2/port/arch
        ./Utilities/Third_Party/lwip_v1.3.2/port/STM32F4x7/Standalone
        ./Utilities/STM32_EVAL)

set(SOURCE_FILES
        User/main.c
        User/netconf.c
        User/stm32f4x7_eth_bsp.c
        User/SCI.c
        User/I2C.c
        User/CHINESE.c
        User/stm32f4xx_it.c
        User/system_stm32f4xx.c
        User/UDP_SERVER.C)

add_executable(exp4_net2_led ${SOURCE_FILES})