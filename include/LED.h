#ifndef PIPICOGAMEPAD_NEOPIXEL
#define PIPICOGAMEPAD_NEOPIXEL

#include "pico/stdio.h"
#include "pico/time.h"
#include "Adafruit_NeoPixel.hpp"

#define PIN_LED_SIG 22
#define NUM_LED 15
#define DELAYVAL 200

void led_init(void);
void led_solid(uint16_t, uint8_t, uint8_t);
void led_anim_rainbow(void);
void led_anim_gradient(uint16_t, uint8_t, uint8_t);
void led_anim_breath(uint16_t, uint8_t);
void led_anim_rainbow_solid(void);

typedef struct {
  uint8_t s;
   int8_t c;
} palette_sat_t;

#endif /* PIPICOGAMEPAD_NEOPIXEL */
