cmake_minimum_required(VERSION 3.12)

set(FAMILY rp2040)
set(BOARD pico_sdk)

add_executable(PiPicoGamepad)

target_sources(PiPicoGamepad PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/Debounce.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/HIDTask.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/usb_descriptors.c
        )

# Make sure TinyUSB can find tusb_config.h
target_include_directories(PiPicoGamepad PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/include/
        )

target_link_libraries(PiPicoGamepad PUBLIC
        pico_stdlib
        tinyusb_device
        tinyusb_board
        pico_multicore
        hardware_flash
        hardware_sync
        )

pico_add_extra_outputs(PiPicoGamepad)
