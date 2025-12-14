#include "Settings.h"

extern LGFX tft; 

SettingsManager::SettingsManager(AudioManager& audioManager) : musicManager(audioManager),
      volumeTrackBar(tft, 150, 200, 500, 40, 0, 21, musicManager.getVolume()),
      muteButton(150, 350, 200, 80, "MUTE / ON", tft, TFT_ORANGE, TFT_BLACK),
      backButton(650, 300, 100, 50, "ZPET", tft, TFT_BLUE, TFT_WHITE)
{
    defaultVolume = musicManager.getVolume() > 0 ? musicManager.getVolume() : 10;
}

bool SettingsManager::isActive() const { return active; }

void SettingsManager::show() {
    active = true;
    drawScreen();
}

void SettingsManager::hide() {
    active = false;
}

void SettingsManager::drawScreen() {
    tft.fillScreen(TFT_DARKGREY);
    tft.fillRect(0, 0, 800, 70, TFT_BLUE);
    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(3);
    tft.setCursor(50, 30);
    tft.println("Settings");

    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(2);
    tft.setCursor(50, 150);
    tft.println("Volume:");

    volumeTrackBar.draw();
    muteButton.draw();
    backButton.draw();
    
    drawVolumeValue(musicManager.getVolume());
}

void SettingsManager::drawVolumeValue(int volume) {
    tft.fillRect(680, 140, 100, 30, TFT_DARKGREY);
    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(2);
    tft.setCursor(680, 150);
    tft.printf("%d", volume);
}

void SettingsManager::update() {
    if (!active) return;
    
    int touchX, touchY;
    if (tft.getTouch(&touchX, &touchY)) {
        // Volání přejmenované metody
        handleTouch(touchX, touchY);
    }
}

// Přejmenováno z handleInput na handleTouch
bool SettingsManager::handleTouch(int touchX, int touchY) {
    if (volumeTrackBar.handleTouch(touchX, touchY)) {
        int newVolume = volumeTrackBar.getValue();
        musicManager.setVolume(newVolume);
        drawVolumeValue(newVolume);
        
        if (newVolume > 0) {
            defaultVolume = newVolume;
        }
        return true;
    }

    if (muteButton.isTouched()) {
        if (musicManager.getVolume() > 0) {
            musicManager.setVolume(0);
            volumeTrackBar.setValue(0);
            drawVolumeValue(0);
        } else {
            uint8_t vol = defaultVolume > 0 ? defaultVolume : 10;
            musicManager.setVolume(vol);
            volumeTrackBar.setValue(vol);
            drawVolumeValue(vol);
        }
        return true;
    }
    
    if (backButton.isTouched()) {
        hide();
        return false; 
    }

    return true; 
}