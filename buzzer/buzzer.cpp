//
// Created by kubab on 08/01/2024.
//

#include "buzzer.h"

buzzer::buzzer(int pin) {
    this->pin = pin;

    gpio_init(pin);
    gpio_set_dir(pin, GPIO_OUT);
    gpio_put(pin, false);

}

buzzer::~buzzer() {
    gpio_deinit(this->pin);
}

void buzzer::beep(int time_ms) {
    gpio_put(this->pin, true);
    sleep_ms(time_ms);
    gpio_put(this->pin, false);
}

void buzzer::on() {
    gpio_put(this->pin, true);
}

void buzzer::off() {
    gpio_put(this->pin, false);
}
