#pragma once

#include "gfx_conf.h" 
#include "Button.h" 
#include "AudioManager.h" 
#include "TrackBar.h"

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
    // TATO METODA ŘEŠÍ DOTYK A NAHRAZUJE CHYBNÉ VOLÁNÍ Z main.cpp
    void update(); 

private:
    void drawVolumeValue(int volume);
    // TATO METODA JE NYNÍ PRIVÁTNÍ A VOLANÁ Z update()
    bool handleTouch(int touchX, int touchY);
};