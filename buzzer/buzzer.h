//
// Created by kubab on 08/01/2024.
//

#ifndef PROJECTPMC_BUZZER_H
#define PROJECTPMC_BUZZER_H
#include "pico/stdlib.h"

class buzzer {
protected:
    int pin;
public:
    buzzer(int pin);
    ~buzzer();

    void beep(int time_ms);

    void on();
    void off();
};


#endif //PROJECTPMC_BUZZER_H
