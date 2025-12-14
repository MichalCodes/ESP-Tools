#pragma once
#include "FileLoader.h"

// Piny I2S (ověřené - ponecháme, jsou to hardwarové konstanty)
#define I2S_BCLK_PIN 42
#define I2S_LRCK_PIN 18
#define I2S_DOUT_PIN 17

class AudioManager {
private:
    // *** Placeholder proměnné ***
    // (Původní proměnné audio knihoven jsou odstraněny)
    
    FileLoader& fileLoader;
    String currentMusicPath = "";
    float volumeLevel = 0.5; // 0.0 až 1.0
    
public:
    explicit AudioManager(FileLoader& loader);
    ~AudioManager();

    void begin();
    
    // Placeholder metody (prozatím bez implementace audio)
    void setVolume(uint8_t vol);
    uint8_t getVolume() const;

    void playMusic(const char* path);
    void stopMusic();

    void handleAudio();
};

using MusicManager = AudioManager;