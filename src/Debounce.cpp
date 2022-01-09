#include "Debounce.h"

//--------------------------------------------------------------------+
// Constructor
//--------------------------------------------------------------------+
// debounce interval = _interval[msec](default:1)
void Debounce::init(uint8_t _pin, uint16_t _interval = 1)
{
    this->pin = _pin;
    this->interval_ms = _interval;
    this->start_ms = 0;
    this->is_lock = false;
    this->state[STATE_LAST] = this->state[STATE_CURRENT] = false;
    gpio_init(this->pin);
    gpio_set_dir(this->pin, GPIO_IN);
    gpio_pull_up(this->pin);
}

//--------------------------------------------------------------------+
// Update button state
//--------------------------------------------------------------------+
void Debounce::update(void)
{
    if(this->is_lock)
    {
        if((to_ms_since_boot(get_absolute_time()) - this->start_ms) < this->interval_ms) return;
        this->is_lock = false;
    }
    else
    {
        this->state[STATE_CURRENT] = !gpio_get(this->pin);
        if(this->state[STATE_LAST] != this->state[STATE_CURRENT])
        {
            this->is_lock = true;
            this->start_ms = to_ms_since_boot(get_absolute_time());
        }
        this->state[STATE_LAST] = this->state[STATE_CURRENT];
    }
}

//--------------------------------------------------------------------+
// Read button state
//--------------------------------------------------------------------+
bool Debounce::read(void)
{
    return this->state[STATE_LAST];
}

//--------------------------------------------------------------------+
// Change interval setting
//--------------------------------------------------------------------+
void Debounce::set_interval(uint16_t interval)
{
    this->interval_ms = interval;
}
