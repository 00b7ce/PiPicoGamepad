#include "HIDTask.h"

//--------------------------------------------------------------------+
// Constructor
//--------------------------------------------------------------------+
HIDTask::HIDTask(uint8_t _itf, uint8_t _interval)
{
  this->itf = _itf;
  this->interval_ms = _interval;
  this->start_ms = 0;
}

//--------------------------------------------------------------------+
// Send report at interval
//--------------------------------------------------------------------+
void HIDTask::send_report(uint8_t _report_id, void const* _report, uint8_t _len)
{
  if (to_ms_since_boot(get_absolute_time()) - this->start_ms < this->interval_ms) return; // not enough time
  this->start_ms += this->interval_ms;

  if (tud_hid_n_ready(this->itf))
  {
    tud_hid_n_report(this->itf, _report_id, _report, _len);
  }
}
