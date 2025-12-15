#include "Settings.h"
#include <LovyanGFX.hpp>

extern LGFX tft; 

SettingsManager::SettingsManager(AudioManager& audioManager) : musicManager(audioManager),
      volumeTrackBar(tft, 150, 200, 500, 40, 0, 100, musicManager.getVolume()),
      muteButton(150, 350, 200, 80, "MUTE / ON", tft, TFT_GREEN, TFT_BLACK),
      backButton(750, 10, 40, 40, "X", tft, TFT_RED, TFT_WHITE) 
{
    defaultVolume = musicManager.getVolume() > 0 ? musicManager.getVolume() : 50; 
}

bool SettingsManager::isActive() const { 
    return active; 
}

void SettingsManager::show() { 
    active = true;
    drawScreen();
}

void SettingsManager::hide() { 
    active = false;
}

void SettingsManager::drawScreen() {
    tft.fillScreen(TFT_BLUE);
    tft.fillRect(0, 0, 800, 70, TFT_GREEN);
    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(3);
    tft.setCursor(50, 33);
    tft.println("Settings");

    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(2);
    tft.setCursor(50, 150);
    tft.println("Volume:");

    uint16_t muteColor = musicManager.getVolume() > 0 ? TFT_GREEN : TFT_RED;
    muteButton.setColor(muteColor); 
    
    volumeTrackBar.draw();
    muteButton.draw();

    backButton.draw();
    
    drawVolumeValue(musicManager.getVolume());
}

void SettingsManager::drawVolumeValue(int volume) {
    tft.fillRect(680, 140, 100, 30, TFT_BLUE);
    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(2);
    tft.setCursor(680, 150);
    tft.printf("%d", volume);
}

void SettingsManager::update() { 
    if (!active) return;
    
    int touchX, touchY;
    if (tft.getTouch(&touchX, &touchY)) {
        handleTouch(touchX, touchY);
    }
}


bool SettingsManager::handleTouch(int touchX, int touchY) {
    if (volumeTrackBar.handleTouch(touchX, touchY)) {
        int newVolume = volumeTrackBar.getValue();
        musicManager.setVolume(newVolume);
        drawVolumeValue(newVolume);
        
        if (newVolume > 0) {
            defaultVolume = newVolume;
            muteButton.setColor(TFT_GREEN);
        } else {
            muteButton.setColor(TFT_RED);
        }
        
        return true;
    }

    if (muteButton.isTouched()) {
        if (musicManager.getVolume() > 0) {
            defaultVolume = musicManager.getVolume();
            musicManager.setVolume(0);
            volumeTrackBar.setValue(0);
            muteButton.setColor(TFT_RED);
        } else {
            musicManager.setVolume(defaultVolume);
            volumeTrackBar.setValue(defaultVolume);
            muteButton.setColor(TFT_GREEN);
        }
        drawVolumeValue(musicManager.getVolume());
        return true;
    }

    if (backButton.isTouched()) {
        hide();
        return true;
    }

    return false;
}