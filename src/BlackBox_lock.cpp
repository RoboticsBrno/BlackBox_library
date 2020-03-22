#include "BlackBox_lock.hpp"

namespace BlackBox {
void BlackBox_Lock::set(const gpio_num_t Hall, const gpio_num_t Motor)
{
    gpio_set_direction(Hall, GPIO_MODE_INPUT);
    gpio_set_pull_mode(Hall, GPIO_PULLUP_ONLY);

    gpio_set_direction(Motor, GPIO_MODE_OUTPUT);
    gpio_set_level(Motor, false);
}
}
