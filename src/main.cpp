/* 
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Ha Thach (tinyusb.org)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#include "PiPicoGamepad.h"
#include "Debounce.h"
#include "HIDTask.h"
#include "LED.h"

//--------------------------------------------------------------------+
// MACRO CONSTANT TYPEDEF PROTYPES
//--------------------------------------------------------------------+
Debounce button[BUTTON_ALL];
const uint8_t pin[BUTTON_ALL] = { PIN_BUTTON1, PIN_BUTTON2,  PIN_BUTTON3,  PIN_BUTTON4, \
                                  PIN_BUTTON5, PIN_BUTTON6,  PIN_BUTTON7,  PIN_BUTTON8, \
                                  PIN_BUTTON9, PIN_BUTTON10, PIN_BUTTON11, PIN_BUTTON12,\
                                  PIN_BUTTON13,PIN_BUTTON14, PIN_BUTTON15, PIN_BUTTON16,\
                                  PIN_UP,      PIN_RIGHT,    PIN_DOWN,     PIN_LEFT    
                                };
static uint32_t blink_interval_ms = BLINK_NOT_MOUNTED;
static bool is_updated = false;
gamepad_report_t gamepad_report; // Set neutral in all button and axis
setting_report_t setting_report;                                        // Set setting at startup setting;

void core1_main(void);
gamepad_report_t gen_gamepad_report(int8_t, int8_t, int8_t, int8_t, uint8_t, uint16_t);
void update_input(gamepad_report_t*);
void init_input(uint16_t);
void read_flash(setting_report_t*);
void led_blinking_task(void);

//--------------------------------------------------------------------+
// Main(Core 0)
//--------------------------------------------------------------------+
int main(void)
{
  board_init();
  tusb_init();

  multicore_launch_core1(core1_main);

  HIDTask itf_gamepad(ITF_GAMEPAD, REPORT_INTERVAL_HID1);
  HIDTask itf_setting(ITF_SETTING, REPORT_INTERVAL_HID2);

  gamepad_report = gen_gamepad_report(0, 0, 0, 0, 0, 0);

  read_flash(&setting_report);
  init_input(setting_report.debounce_interval);

  while(1)
  {
    tud_task(); // tinyusb device task
    update_input(&gamepad_report);
    itf_gamepad.send_report(REPORT_ID_GAMEPAD,          &gamepad_report, sizeof(gamepad_report));
    if(is_updated)
    {
      read_flash(&setting_report);
      for(uint8_t i = 0; i < BUTTON_ALL; i++)
      {
        button[i].set_interval(setting_report.debounce_interval);
      }
    }
    itf_setting.send_report(REPORT_ID_SETTING, &setting_report, sizeof(setting_report));
  }

  return 0;
}

//--------------------------------------------------------------------+
// Main(Core 1)
//--------------------------------------------------------------------+
void core1_main(void)
{
  led_init();
  while(1)
  {
    led_blinking_task();
    switch(setting_report.led_mode)
    {
      case SOLID_STATIC:
        led_solid(setting_report.h, setting_report.s, setting_report.v);
        break;
      case SOLID_GRADIENT:
        led_anim_gradient(setting_report.h, setting_report.s, setting_report.v);
        break;
      case SOLID_BREATH:
        led_anim_breath(setting_report.h, setting_report.s);
        break;
      case RAINBOW_GRADIENT:
        led_anim_rainbow();
        break;
      case RAINBOW_SOLID:
        led_anim_rainbow_solid();
        break;
      default:
        led_off();
        break;
    }
  }
}

//--------------------------------------------------------------------+
// Generate report struct for gamepad
//--------------------------------------------------------------------+
gamepad_report_t gen_gamepad_report(int8_t x, int8_t y, int8_t rx, int8_t ry, uint8_t hat, uint16_t buttons)
{
  gamepad_report_t report =
  {
    .x       = x,
    .y       = y,
    .rx      = rx,
    .ry      = ry,
    .hat     = hat,
    .buttons = buttons,
  };

  return report;
}

//--------------------------------------------------------------------+
// Check input state and generate report
//--------------------------------------------------------------------+
void init_input(uint16_t interval)
{
  for(uint8_t i = 0; i < BUTTON_ALL; i++)
  {
    button[i].init(pin[i], interval);
  }
}

//--------------------------------------------------------------------+
// Check input state and generate report
//--------------------------------------------------------------------+
void update_input(gamepad_report_t* report)
{
  for(uint8_t i = 0; i < BUTTON_ALL; i++)
  {
    button[i].update();
  }
  for(int8_t i = BUTTON_16; i >= BUTTON_1; i--)
  {
    report->buttons = (report->buttons << 1) + button[i].read();
  }
}

//--------------------------------------------------------------------+
// Read setting on flash, return struct
//--------------------------------------------------------------------+
void read_flash(setting_report_t* setting)
{
  memcpy(setting, (const setting_report_t *) (XIP_BASE + FLASH_TARGET_OFFSET), sizeof(setting_report_t));
  is_updated = false;
}

//--------------------------------------------------------------------+
// Device callbacks
//--------------------------------------------------------------------+

// Invoked when device is mounted
void tud_mount_cb(void)
{
  blink_interval_ms = BLINK_MOUNTED;
}

// Invoked when device is unmounted
void tud_umount_cb(void)
{
  blink_interval_ms = BLINK_NOT_MOUNTED;
}

// Invoked when usb bus is suspended
// remote_wakeup_en : if host allow us  to perform remote wakeup
// Within 7ms, device must draw an average of current less than 2.5 mA from bus
void tud_suspend_cb(bool remote_wakeup_en)
{
  (void) remote_wakeup_en;
  blink_interval_ms = BLINK_SUSPENDED;
}

// Invoked when usb bus is resumed
void tud_resume_cb(void)
{
  blink_interval_ms = BLINK_MOUNTED;
}

//--------------------------------------------------------------------+
// USB HID callbacks
//--------------------------------------------------------------------+
// Invoked when received GET_REPORT control request
// Application must fill buffer report's content and return its length.
// Return zero will cause the stack to STALL request
uint16_t tud_hid_get_report_cb(uint8_t itf, uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen)
{
  // TODO not Implemented
  (void) itf;
  (void) report_id;
  (void) report_type;
  (void) buffer;
  (void) reqlen;

  return 0;
}

// Invoked when received SET_REPORT control request or
// received data on OUT endpoint ( Report ID = 0, Type = 0 )
void tud_hid_set_report_cb(uint8_t itf, uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize)
{
  // Write received data to flash
  uint32_t ints = save_and_disable_interrupts();
  flash_range_erase(FLASH_TARGET_OFFSET, FLASH_SECTOR_SIZE);
  restore_interrupts(ints);
  flash_range_program(FLASH_TARGET_OFFSET, buffer, FLASH_PAGE_SIZE);
  is_updated = true;
}

//--------------------------------------------------------------------+
// BLINKING TASK
//--------------------------------------------------------------------+
void led_blinking_task(void)
{
  static uint32_t start_ms = 0;
  static bool led_state = false;

  // Blink every interval ms
  if ( board_millis() - start_ms < blink_interval_ms) return; // not enough time
  start_ms += blink_interval_ms;

  board_led_write(led_state);
  led_state = 1 - led_state; // toggle
}
