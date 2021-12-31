cmake_minimum_required(VERSION 3.12)

set(FAMILY rp2040)
set(BOARD pico_sdk)
#set(TINYUSB_FAMILY_PROJECT_NAME_PREFIX "tinyusb_dev_")
#add_subdirectory(${PICO_TINYUSB_PATH}/examples/device tinyusb_device_examples)

add_executable(PiPicoGamepad)

target_sources(PiPicoGamepad PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/src/main.c
        ${CMAKE_CURRENT_LIST_DIR}/src/usb_descriptors.c
        )

# Make sure TinyUSB can find tusb_config.h
target_include_directories(PiPicoGamepad PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/src/)

target_link_libraries(PiPicoGamepad PUBLIC pico_stdlib tinyusb_device tinyusb_board pico_multicore hardware_flash)

pico_add_extra_outputs(PiPicoGamepad)
