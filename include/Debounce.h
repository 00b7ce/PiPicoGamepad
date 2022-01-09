#ifndef DEBOUNCE
#define DEBOUNCE

#include <stdint.h>
#include "pico/time.h"
#include "pico/stdlib.h"

class Debounce
{
private:
  uint8_t pin;
  uint16_t interval_ms;
  uint32_t start_ms;
  bool is_lock;
  bool state[2];
public:
  void init(uint8_t, uint16_t);
  void update(void);
  bool read(void);
  void set_interval(uint16_t);
};

enum
{
  STATE_LAST,
  STATE_CURRENT
};

#endif /* DEBOUNCE */
