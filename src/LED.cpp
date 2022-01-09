#include "LED.h"

static Adafruit_NeoPixel pixels(NUM_LED, PIN_LED_SIG, NEO_GRB + NEO_KHZ800);
static uint32_t start_ms = 0;

void led_init()
{
    pixels.begin();
}

void led_solid(uint16_t hue, uint8_t sat, uint8_t val)
{
  if (to_ms_since_boot(get_absolute_time()) - start_ms < DELAYVAL) return;
  start_ms += DELAYVAL;
  
  pixels.clear();
  pixels.fill(pixels.ColorHSV(hue, sat, val), 0, NUM_LED);
  pixels.show();
}

void led_anim_rainbow(void)
{
  if (to_ms_since_boot(get_absolute_time()) - start_ms < DELAYVAL) return;
  start_ms += DELAYVAL;
}

void led_anim_gradient(uint16_t hue, uint8_t sat, uint8_t val)
{
  if (to_ms_since_boot(get_absolute_time()) - start_ms < DELAYVAL) return;
  start_ms += DELAYVAL;
}

void led_anim_breath(uint16_t hue, uint8_t sat, uint8_t val)
{
  if (to_ms_since_boot(get_absolute_time()) - start_ms < DELAYVAL) return;
  start_ms += DELAYVAL;
}