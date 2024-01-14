//
// Created by kubab on 11/11/2023.
//

#include "rgb_diode.h"

rgb_diode::rgb_diode(int pin1, int pin2, int pin3) {
    this->pin1 = pin1;
    this->pin2 = pin2;
    this->pin3 = pin3;
}

void rgb_diode::set_color(bool ct1, bool ct2, bool ct3) const {
    gpio_put(this->pin1, ct1);
    gpio_put(this->pin2, ct2);
    gpio_put(this->pin3, ct3);
}

void rgb_diode::off() const {
    gpio_put(this->pin1, false);
    gpio_put(this->pin2, false);
    gpio_put(this->pin3, false);
}

void rgb_diode::white() const {
    gpio_put(this->pin1, true);
    gpio_put(this->pin2, true);
    gpio_put(this->pin3, true);
}

rgb_diode::~rgb_diode() {
    gpio_deinit(this->pin1);
    gpio_deinit(this->pin2);
    gpio_deinit(this->pin3);
}

void rgb_diode::init() const {
    gpio_init(pin1);
    gpio_set_dir(pin1, GPIO_OUT);
    gpio_put(pin1, false);
    gpio_init(pin2);
    gpio_set_dir(pin2, GPIO_OUT);
    gpio_put(pin2, false);
    gpio_init(pin3);
    gpio_set_dir(pin3, GPIO_OUT);
    gpio_put(pin3, false);
}


