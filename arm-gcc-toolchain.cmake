# ARM Cortex-M4 GCC Toolchain file for STM32F407VET6
# Usage: cmake -DCMAKE_TOOLCHAIN_FILE=arm-gcc-toolchain.cmake ...

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

# CPU flags (must be set before project() call)
set(CMAKE_C_FLAGS_INIT
    "-mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard"
)
set(CMAKE_ASM_FLAGS_INIT
    "-mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard"
)

# Prevent CMake from trying to link a test executable during compiler detection
# (bare-metal ARM can't produce Windows executables)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# Use arm-none-eabi tools
find_program(CMAKE_C_COMPILER arm-none-eabi-gcc)
find_program(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
find_program(CMAKE_OBJCOPY arm-none-eabi-objcopy)
find_program(CMAKE_SIZE arm-none-eabi-size)

# Disable CMake's find_library/link features (not used for bare-metal)
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
