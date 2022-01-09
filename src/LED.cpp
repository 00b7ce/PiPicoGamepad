#include "LED.h"

static Adafruit_NeoPixel pixels(NUM_LED, PIN_LED_SIG, NEO_GRB + NEO_KHZ800);

void led_init()
{
    pixels.begin();
}

void led_test(void) {

  static uint32_t start_ms = 0;
  
  if (to_ms_since_boot(get_absolute_time()) - start_ms < DELAYVAL) return;
  start_ms += DELAYVAL;
  
  pixels.clear();
  pixels.fill(pixels.ColorHSV(193, 255, 128), 0, 15);
  pixels.show();
};