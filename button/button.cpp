//
// Created by kubab on 11/11/2023.
//

#include "button.h"

button::button(int pin) {
    this->pin = pin;

    gpio_init(pin);
    gpio_set_dir(pin, GPIO_IN);
    gpio_pull_up(pin);
}

button::~button() {
    gpio_deinit(this->pin);
}

bool button::is_pressed() {
    this->last_state = !gpio_get(this->pin);
    return last_state;
}

bool button::is_released() {
    this->last_state = gpio_get(this->pin);
    return this->last_state;
}

bool button::is_pressed(uint32_t &delay) {
    if (this->is_released()) return false;

    uint32_t start_time = to_ms_since_boot(get_absolute_time());

    while (this->is_pressed()) { ;
    }

    uint32_t current_time = to_ms_since_boot(get_absolute_time());
    delay = start_time - current_time;

    return true;
}

bool button::last_stage() {
    return this->last_state;
}

