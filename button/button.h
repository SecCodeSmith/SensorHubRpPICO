//
// Created by kubab on 11/11/2023.
//

#ifndef PROJECTPMC_BUTTON_H
#define PROJECTPMC_BUTTON_H

#include "pico/stdlib.h"

class button {
    int pin;
    bool last_state = false;
public:
    button(int pin);

    ~button();

    bool is_pressed();

    bool is_pressed(uint32_t &delay);

    bool is_released();

    bool last_stage();
};


#endif //PROJECTPMC_BUTTON_H
