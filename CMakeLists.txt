# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.1)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.1.1)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
cmake_minimum_required(VERSION 3.12)


set(PICO_BOARD pico2 CACHE STRING "Board type")

set (VERSION_MAJOR 0)
set (VERSION_MINOR 1)
set (VERSION_PATCH 26)
set (SOFT_VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})
set (SOFT_VERSION_FN ${VERSION_MAJOR}_${VERSION_MINOR}_${VERSION_PATCH})

configure_file ("src/config.h.in" "../src/config.h" @ONLY)

# Pull in SDK (must be before project)
include(pico_sdk_import.cmake)
# include(pico_extras_import_optional.cmake)

SET(PROJECT_NAME "Mutalator_GnW_tft_p2350a_ws")

project(Mutalator_GnW_tft_p2350a_ws C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)


if (PICO_SDK_VERSION_STRING VERSION_LESS "2.1.0")
    message(FATAL_ERROR "Raspberry Pi Pico SDK version 2.1.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

# set(PICO_EXAMPLES_PATH ${PROJECT_SOURCE_DIR})

# If you want debug output from USB (pass -DPICO_STDIO_USB=1) this ensures you don't lose any debug output while USB is set up
# if (NOT DEFINED PICO_STDIO_USB_CONNECT_WAIT_TIMEOUT_MS)
#     set(PICO_STDIO_USB_CONNECT_WAIT_TIMEOUT_MS 3000)
# endif()

# Initialize the SDK
pico_sdk_init()


add_compile_options(
        -Wall
        -Wno-format          # int != int32_t as far as the compiler is concerned because gcc has int32_t as long int
        -Wno-unused-function # we have some for the docs that aren't called
        )
if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
    add_compile_options(-Wno-maybe-uninitialized)
endif()

# Hardware-specific examples in subdirectories:
add_subdirectory(lib/bsp)
add_subdirectory(lib/fatfs)
add_subdirectory(lib/sdcard)
add_subdirectory(lib/joysticks)
add_subdirectory(lib/sound)

#add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/cpus)
#add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/system)




add_executable(${PROJECT_NAME})

file(GLOB_RECURSE SRC "src/*.cpp" "src/*.c")
message(STATUS "Add source files:")
foreach (SRC_FILE IN LISTS SRC)
    message(STATUS "${SRC_FILE}")
endforeach ()
message(STATUS "")

# Source files
file(GLOB_RECURSE SOURCES src/*.c src/*.cpp)
message(STATUS "${SOURCES}")
target_sources(${PROJECT_NAME} PRIVATE ${SOURCES})
pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)

# pull in common dependencies
target_link_libraries(${PROJECT_NAME} PRIVATE
    pico_stdlib
    bsp
    fatfs
    sdcard
    joysticks
    sound

    pico_stdlib 
    hardware_spi 
    hardware_i2c 
    hardware_pwm 
    hardware_adc 
    hardware_pio 
    hardware_irq 
    hardware_dma 
    pico_time 
    hardware_timer 
    pico_rand 
    pico_stdio 
    pico_stdio_usb
    pico_multicore	
    )

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(${PROJECT_NAME})
target_link_options(${PROJECT_NAME} PRIVATE -Xlinker --print-memory-usage --data-sections --function-sections)
#target_link_options(pico_examples PRIVATE -Xlinker --print-memory-usage --data-sections --function-sections)