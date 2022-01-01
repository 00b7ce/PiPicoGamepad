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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "PiPicoGamepad.h"

//--------------------------------------------------------------------+
// MACRO CONSTANT TYPEDEF PROTYPES
//--------------------------------------------------------------------+
/* Blink pattern
 * - 250 ms  : device not mounted
 * - 1000 ms : device mounted
 * - 2500 ms : device is suspended
 */
enum  {
  BLINK_NOT_MOUNTED = 250,
  BLINK_MOUNTED = 1000,
  BLINK_SUSPENDED = 2500,
};

static uint32_t blink_interval_ms = BLINK_NOT_MOUNTED;

void led_blinking_task(void);

//--------------------------------------------------------------------+
// HID report
//--------------------------------------------------------------------+
bool mkb_hid_n_gamepad_report(uint8_t instance, uint8_t report_id, int8_t x, int8_t y, int8_t rx, int8_t ry, uint8_t hat, uint16_t buttons)
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

  return tud_hid_n_report(instance, report_id, &report, sizeof(report));
}


//--------------------------------------------------------------------+
// HID report class
//--------------------------------------------------------------------+
class HIDTask
{
private:
  uint8_t itf;
  uint8_t interval_ms;
  uint32_t start_ms;
public:
  HIDTask(uint8_t, uint8_t);
  void send_report();
};

HIDTask::HIDTask(uint8_t _itf, uint8_t _interval)
{
  this->itf = _itf;
  this->interval_ms = _interval;
  this->start_ms = 0;
}

void HIDTask::send_report()
{
  if (board_millis() - this->start_ms < this->interval_ms) return; // not enough time
  this->start_ms += this->interval_ms;

  if (tud_hid_n_ready(this->itf))
  {
    // use to avoid send multiple consecutive zero report for keyboard
    mkb_hid_n_gamepad_report(this->itf, REPORT_ID_GAMEPAD, 0, 0, 0, 0, GAMEPAD_HAT_UP, GAMEPAD_BUTTON_A);
  }
}

//--------------------------------------------------------------------+
// Main(Core 1)
//--------------------------------------------------------------------+
void core1_main(void)
{
  while(1)
  {
    led_blinking_task();
  }
}

//--------------------------------------------------------------------+
// Main(Core 0)
//--------------------------------------------------------------------+
int main(void)
{
  board_init();
  tusb_init();

  HIDTask itf_gamepad(ITF_GAMEPAD, REPORT_INTERVAL_HID1);
  HIDTask itf_data(ITF_DATA, REPORT_INTERVAL_HID2);

  multicore_launch_core1(core1_main);

  while (1)
  {
    tud_task(); // tinyusb device task
    itf_gamepad.send_report();
    itf_data.send_report();
  }

  return 0;
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
// USB HID
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
  // This example doesn't use multiple report and report ID
  // (void) itf;
  // (void) report_id;
  // (void) report_type;
  // static bool led_state = false;

  // echo back anything we received from host
  // while (!tud_hid_n_ready(itf));
  // tud_hid_n_report(itf, 0, buffer, 64);
  // tud_hid_report(0, buffer, bufsize);

  // board_led_write(led_state);
  // led_state = 1 - led_state; // toggle
  // is_received = true;
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
