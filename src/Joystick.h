#pragma once
#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <math.h>

class Joystick {
public:
    Joystick(int centerX, int centerY, int outerRadius, int innerRadius, LGFX &tft, uint16_t bgColor)
        : centerX(centerX), centerY(centerY), outerRadius(outerRadius), innerRadius(innerRadius),
          joystickX(centerX), joystickY(centerY), tft(tft), bgColor(bgColor) {}

    void drawJoystick() {
        tft.fillCircle(centerX, centerY, outerRadius, TFT_BLACK);
        tft.drawCircle(centerX, centerY, outerRadius, TFT_WHITE);
        tft.fillCircle(joystickX, joystickY, innerRadius, TFT_RED);
    }

    void moveJoystick(int touchX, int touchY) {
        int dx = touchX - centerX;
        int dy = touchY - centerY;
        float distance = sqrt(dx * dx + dy * dy);
        if (distance > outerRadius - innerRadius) {
            float scale = (outerRadius - innerRadius) / distance;
            dx *= scale;
            dy *= scale;
        }

        // Vymaz starou pozici
        tft.fillCircle(joystickX, joystickY, innerRadius, bgColor);

        joystickX = centerX + dx;
        joystickY = centerY + dy;
        tft.fillCircle(joystickX, joystickY, innerRadius, TFT_RED);
    }

    void updateAutoCenter() {
        int dx = joystickX - centerX;
        int dy = joystickY - centerY;

        if (dx == 0 && dy == 0) return; // už ve středu

        // krok návratu
        int step = 3;
        int moveX = (dx > step ? step : (dx < -step ? -step : dx));
        int moveY = (dy > step ? step : (dy < -step ? -step : dy));

        // vymaz starou pozici
        tft.fillCircle(joystickX, joystickY, innerRadius, bgColor);

        joystickX -= moveX;
        joystickY -= moveY;

        tft.fillCircle(joystickX, joystickY, innerRadius, TFT_RED);
    }

    bool isJoystickTouched(int touchX, int touchY) {
        int dx = touchX - centerX;
        int dy = touchY - centerY;
        return (dx * dx + dy * dy <= outerRadius * outerRadius);
    }

    void resetJoystick() {
        tft.fillCircle(joystickX, joystickY, innerRadius, bgColor);
        joystickX = centerX;
        joystickY = centerY;
        tft.fillCircle(joystickX, joystickY, innerRadius, TFT_RED);
    }

    int getJoystickX() const { return joystickX; }
    int getJoystickY() const { return joystickY; }

private:
    int centerX, centerY;
    int outerRadius, innerRadius;
    int joystickX, joystickY;
    LGFX &tft;
    uint16_t bgColor;
};

#endif // JOYSTICK_H
