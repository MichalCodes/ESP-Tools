#pragma once
#ifndef JOYSTICK_H
#define JOYSTICK_H

#include "hw/gfx_conf.h"
#include <math.h>
#include <Arduino.h>

class Joystick {
public:
    Joystick(int centerX, int centerY, int outerRadius, int innerRadius, LGFX &tft, uint16_t bgColor);

    void drawJoystick();
    
    void moveJoystick(int touchX, int touchY);
    
    void updateAutoCenter();
    
    bool isJoystickTouched(int touchX, int touchY);

    void resetJoystick();

    int getJoystickX() const;
    int getJoystickY() const;

private:
    int centerX, centerY;
    int outerRadius, innerRadius;
    int joystickX, joystickY;
    LGFX &tft;
    uint16_t bgColor;
};

#endif