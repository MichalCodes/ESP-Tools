#pragma once

#include "hw/gfx_conf.h" 
#include "touch/Button.h" 
#include "hw/AudioManager.h" 
#include "touch/TrackBar.h"

class SettingsManager {
private:
    AudioManager& musicManager;
    
    bool active = false; 
    uint8_t defaultVolume; 

    TrackBar volumeTrackBar;

    Button muteButton;
    Button backButton; 

public:
    SettingsManager(AudioManager& audioManager);

    bool isActive() const;
    void show();
    void hide();
    void drawScreen();
    void update(); 

private:
    void drawVolumeValue(int volume);
    bool handleTouch(int touchX, int touchY);
};