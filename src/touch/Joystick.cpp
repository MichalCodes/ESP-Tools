#include "Joystick.h"

Joystick::Joystick(int centerX, int centerY, int outerRadius, int innerRadius, LGFX &tft, uint16_t bgColor)
    : centerX(centerX), centerY(centerY), outerRadius(outerRadius), innerRadius(innerRadius),
      joystickX(centerX), joystickY(centerY), tft(tft), bgColor(bgColor) {}

void Joystick::drawJoystick() {
    tft.fillCircle(centerX, centerY, outerRadius, TFT_BLACK);
    tft.drawCircle(centerX, centerY, outerRadius, TFT_WHITE);
    tft.fillCircle(joystickX, joystickY, innerRadius, TFT_RED);
}

void Joystick::moveJoystick(int touchX, int touchY) {
    int dx = touchX - centerX;
    int dy = touchY - centerY;
    float distance = sqrt(dx * dx + dy * dy);

    if (distance > outerRadius - innerRadius) {
        float scale = (outerRadius - innerRadius) / distance;
        dx *= scale;
        dy *= scale;
    }

    tft.fillCircle(joystickX, joystickY, innerRadius, bgColor);

    joystickX = centerX + dx;
    joystickY = centerY + dy;
    tft.fillCircle(joystickX, joystickY, innerRadius, TFT_RED);
}

void Joystick::updateAutoCenter() {
    int dx = joystickX - centerX;
    int dy = joystickY - centerY;

    if (dx == 0 && dy == 0) return; 

    int step = 20; 
    int moveX = (dx > step ? step : (dx < -step ? -step : dx));
    int moveY = (dy > step ? step : (dy < -step ? -step : dy));

    tft.fillCircle(joystickX, joystickY, innerRadius, bgColor);

    joystickX -= moveX;
    joystickY -= moveY;

    tft.fillCircle(joystickX, joystickY, innerRadius, TFT_RED);
}

bool Joystick::isJoystickTouched(int touchX, int touchY) {
    int dx = touchX - centerX;
    int dy = touchY - centerY;
    return (dx * dx + dy * dy <= outerRadius * outerRadius);
}

void Joystick::resetJoystick() {
    tft.fillCircle(joystickX, joystickY, innerRadius, bgColor);
    joystickX = centerX;
    joystickY = centerY;
    tft.fillCircle(joystickX, joystickY, innerRadius, TFT_RED);
}

int Joystick::getJoystickX() const {
    return joystickX;
}

int Joystick::getJoystickY() const {
    return joystickY;
}