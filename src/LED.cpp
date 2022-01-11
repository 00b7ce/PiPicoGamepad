#include "LED.h"
#include "algorithm"
#include "utility"

static Adafruit_NeoPixel pixels(NUM_LED, PIN_LED_SIG, NEO_GRB + NEO_KHZ800);
static uint32_t start_ms = 0;
palette_sat_t palette[NUM_LED];

void led_init()
{
    pixels.begin();
    pixels.setBrightness(128);

    for(int i = 0; i < NUM_LED; i++)
    {
      palette[i].s = 255 - (255 / NUM_LED * i);
      (palette[i].s == 255) ? palette[i].c = -1 : palette[i].c = 1;
    }
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
  static uint16_t base_hue = 0;
  if (to_ms_since_boot(get_absolute_time()) - start_ms < 1) return;
  start_ms++;
  for(uint8_t i = 0; i < NUM_LED; i++)
  {
    pixels.setPixelColor(i, pixels.ColorHSV(base_hue + (0xFFFF / NUM_LED * i), 255, 128));
  }
  base_hue += 50;

  pixels.show();
}

void led_anim_gradient(uint16_t hue, uint8_t sat, uint8_t val)
{
  if (to_ms_since_boot(get_absolute_time()) - start_ms < 10) return;
  start_ms += 10;

  pixels.clear();

  for(int i = 0; i < NUM_LED; i++)
  {
    pixels.setPixelColor(i, pixels.ColorHSV(hue, palette[i].s, 128));

    palette[i].s += palette[i].c;

    if((palette[i].s == 255) || (palette[i].s == 0)) palette[i].c *= -1;
  }

  pixels.show();
}

void led_anim_breath(uint16_t hue, uint8_t sat)
{
  static uint8_t val = 0;
  static bool is_decrement = false;
  if (to_ms_since_boot(get_absolute_time()) - start_ms < DELAYVAL / 10) return;
  start_ms += DELAYVAL / 10;

  pixels.clear();
  pixels.fill(pixels.ColorHSV(hue, sat, val), 0, NUM_LED);
  pixels.show();
  if (is_decrement)
  {
    if(val > 0) val--;
    else is_decrement = false;
  }
  else
  {
    if(val < 128) val++;
    else is_decrement = true;
  }
}

void led_anim_rainbow_solid(void)
{
  static uint16_t base_hue = 0;
  if (to_ms_since_boot(get_absolute_time()) - start_ms < 1) return;
  start_ms++;

  pixels.fill(pixels.ColorHSV(base_hue, 255, 128), 0, NUM_LED);

  base_hue += 15;

  pixels.show();
}
