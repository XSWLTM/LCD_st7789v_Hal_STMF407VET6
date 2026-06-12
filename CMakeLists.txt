cmake_minimum_required(VERSION 3.16)
project(LCD_HAL_Port C ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# ============================================================
# MCU: STM32F407VET6
# CPU flags are in arm-gcc-toolchain.cmake
# ============================================================
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -fdata-sections -ffunction-sections -include stm32f4xx.h")
set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -x assembler-with-cpp")

add_definitions(
    -DSTM32F407xx
    -DUSE_HAL_DRIVER
    -DHSE_VALUE=8000000
    -DHSE_STARTUP_TIMEOUT=100
)

# ============================================================
# HAL Driver sources (subset needed for this project)
# ============================================================
set(HAL_SOURCES
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.c
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.c
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc_ex.c
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_spi.c
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_uart.c
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.c
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim.c
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim_ex.c
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr.c
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr_ex.c
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash.c
    Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ex.c
)

# ============================================================
# Application sources
# ============================================================
set(APP_SOURCES
    Core/Src/main.c
    Core/Src/stm32f4xx_it.c
    Core/Src/stm32f4xx_hal_msp.c
    Core/Src/system_stm32f4xx.c
    Core/Src/startup_stm32f407xx.s

    System/delay/delay.c
    System/sys/sys.c

    Hardware/interface/interface.c
    Hardware/lcd/lcd.c
    Hardware/spi/spi.c
    Hardware/flash/flash.c
    Hardware/usart/usart.c
)

# ============================================================
# Include paths
# ============================================================
set(INCLUDE_DIRS
    Core/Inc
    Drivers/CMSIS/Include
    Drivers/CMSIS/Device/ST/STM32F4xx/Include
    Drivers/STM32F4xx_HAL_Driver/Inc
    System/delay
    System/sys
    Hardware/lcd
    Hardware/lcd/init
    Hardware/lcd/blockwrite
    Hardware/interface
    Hardware/spi
    Hardware/flash
    Hardware/usart
)

# ============================================================
# Executable
# ============================================================
add_executable(${PROJECT_NAME} ${APP_SOURCES} ${HAL_SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIRS})

# Linker script
set(LINKER_SCRIPT "${CMAKE_SOURCE_DIR}/STM32F407VETx_FLASH.ld")
target_link_options(${PROJECT_NAME} PRIVATE
    -T${LINKER_SCRIPT}
    -Wl,-Map=${PROJECT_NAME}.map,--cref
    -Wl,--gc-sections
    --specs=nano.specs
)

# Post-build: generate hex and bin
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${PROJECT_NAME}> ${PROJECT_NAME}.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${PROJECT_NAME}> ${PROJECT_NAME}.bin
    COMMENT "Generating .hex and .bin files"
)

# ============================================================
# Flash target: upload firmware via ST-LINK/V2 + CubeProgrammer
# ============================================================
set(CUBE_PROG_PATH "D:/STM32CubeCLT_1.21.0/STM32CubeProgrammer/bin/STM32_Programmer_CLI.exe"
    CACHE FILEPATH "Path to STM32CubeProgrammer CLI")

add_custom_target(flash
    COMMAND "${CUBE_PROG_PATH}"
        -c port=SWD reset=HWrst
        -d "${CMAKE_BINARY_DIR}/${PROJECT_NAME}.hex"
        -v
        -s
    DEPENDS ${PROJECT_NAME}
    COMMENT "Flashing firmware via ST-LINK/V2 CubeProgrammer"
)

# Debug symbols
set(CMAKE_C_FLAGS_DEBUG "-Og -g3")
set(CMAKE_C_FLAGS_RELEASE "-Os -g0")
