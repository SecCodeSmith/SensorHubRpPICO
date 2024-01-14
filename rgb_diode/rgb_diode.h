//
// Created by kubab on 11/11/2023.
//

#ifndef PROJECTPMC_RGB_DIODE_H
#define PROJECTPMC_RGB_DIODE_H

#include "pico/stdlib.h"

class rgb_diode {
    int pin1, pin2, pin3;
public:
    rgb_diode(int pin1, int pin2, int pin3);
    ~rgb_diode();

    void init() const;
    void set_color(bool ct1, bool ct2, bool ct3) const;

    void off() const;

    void white() const;
};


#endif //PROJECTPMC_RGB_DIODE_H
