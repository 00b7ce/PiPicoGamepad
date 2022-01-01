#ifndef PIPICOGAMEPAD
#define PIPICOGAMEPAD

#include "bsp/board.h"
#include "tusb.h"
#include "pico/multicore.h"
#include "hardware/flash.h"
#include "usb_descriptors.h"

//--------------------------------------------------------------------+
// MACRO CONSTANT TYPEDEF PROTYPES
//--------------------------------------------------------------------+
typedef struct TU_ATTR_PACKED
{
  int8_t  x;         ///< Delta x  movement of left analog-stick
  int8_t  y;         ///< Delta y  movement of left analog-stick
  int8_t  rx;        ///< Delta Rx movement of analog left trigger
  int8_t  ry;        ///< Delta Ry movement of analog right trigger
  uint8_t hat;       ///< Buttons mask for currently pressed buttons in the DPad/hat
  uint16_t buttons;  ///< Buttons mask for currently pressed buttons
}gamepad_report_t;

enum
{
  ITF_GAMEPAD = ITF_NUM_HID1,
  ITF_DATA = ITF_NUM_HID2
};

#endif /* PIPICOGAMEPAD */