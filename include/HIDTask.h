#ifndef HIDTASK
#define HIDTASK

#include <stdint.h>
#include "pico/time.h"
#include "pico/stdlib.h"
#include "tusb.h"

class HIDTask
{
private:
  uint8_t itf;
  uint8_t interval_ms;
  uint32_t start_ms;
public:
  HIDTask(uint8_t, uint8_t);
  void send_report(uint8_t, void const*, uint8_t);
};

#endif /* HIDTASK */