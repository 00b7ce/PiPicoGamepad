#ifndef PIPICOGAMEPAD
#define PIPICOGAMEPAD

#include "bsp/board.h"
#include "tusb.h"
#include "pico/multicore.h"
#include "hardware/flash.h"
#include "usb_descriptors.h"

//--------------------------------------------------------------------+
// Report struct
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

typedef struct TU_ATTR_PACKED
{
  uint8_t direction_type;
  uint8_t is_socd;
  uint16_t debounce_interval;
  uint8_t led_mode;
  uint8_t led_h;
  uint8_t led_s;
  uint8_t led_v;
}setting_report_t;

//--------------------------------------------------------------------+
// Interface number
//--------------------------------------------------------------------+
enum
{
  ITF_GAMEPAD = ITF_NUM_HID1,
  ITF_SETTING = ITF_NUM_HID2
};

//--------------------------------------------------------------------+
// Blink builtin LED
//--------------------------------------------------------------------+
enum
{
  BLINK_NOT_MOUNTED = 250,
  BLINK_MOUNTED = 1000,
  BLINK_SUSPENDED = 2500
};

//--------------------------------------------------------------------+
// Flash
//--------------------------------------------------------------------+
#define FLASH_TARGET_OFFSET (256 * 1024)

//--------------------------------------------------------------------+
// Input PIO
//--------------------------------------------------------------------+
enum Button_pins
{
  PIN_BUTTON1  = 0,
  PIN_BUTTON2  = 1,
  PIN_BUTTON3  = 2,
  PIN_BUTTON4  = 3,
  PIN_BUTTON5  = 4,
  PIN_BUTTON6  = 5,
  PIN_BUTTON7  = 6,
  PIN_BUTTON8  = 7,
  PIN_BUTTON9  = 8,
  PIN_BUTTON10 = 9,
  PIN_BUTTON11 = 10,
  PIN_BUTTON12 = 11,
  PIN_BUTTON13 = 12,
  PIN_BUTTON14 = 13,
  PIN_BUTTON15 = 14,
  PIN_BUTTON16 = 15,
  PIN_UP       = 21,
  PIN_RIGHT    = 20,
  PIN_DOWN     = 18,
  PIN_LEFT     = 18
};

enum
{
  BUTTON_1,
  BUTTON_2,
  BUTTON_3,
  BUTTON_4,
  BUTTON_5,
  BUTTON_6,
  BUTTON_7,
  BUTTON_8,
  BUTTON_9,
  BUTTON_10,
  BUTTON_11,
  BUTTON_12,
  BUTTON_13,
  BUTTON_14,
  BUTTON_15,
  BUTTON_16,
  BUTTON_UP,
  BUTTON_RIGHT,
  BUTTON_DOWN,
  BUTTON_LEFT,
  BUTTON_ALL
};

#endif /* PIPICOGAMEPAD */