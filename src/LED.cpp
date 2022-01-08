#include "LED.h"

static Adafruit_NeoPixel pixels(NUM_LED, PIN_LED_SIG, NEO_GRB + NEO_KHZ800);

void led_test(void) {
  pixels.begin();

  for (int l ; l <4 ; l++) {
    pixels.clear();
    for(int i=0; i<NUM_LED; i++) {
        pixels.setPixelColor(i, pixels.Color((i%3)*150,((i+1)%3)*150 ,((i+2)%3)*150));
        pixels.show();  
        sleep_ms(DELAYVAL);
    }
  }
};