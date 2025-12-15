#pragma once
#include "FileLoader.h"
#include <AudioTools.h>
#include <AudioTools/AudioCodecs/CodecMP3Helix.h> 
#include <AudioTools/AudioCodecs/CodecWAV.h>      
#include <SD.h>

#define I2S_BCLK_PIN 42 
#define I2S_LRCK_PIN 18
#define I2S_DOUT_PIN 17

class AudioManager {
private:
    FileLoader& fileLoader;
    String currentMusicPath = "";
    float volumeLevel = 0.5; // Stále ukládáme hodnotu v rozsahu 0.0 - 1.0

    I2SStream i2sStream;                
    VolumeStream volumeStream;          
    EncodedAudioStream *decoder = nullptr; 
    StreamCopy copier;                       
    File audioFile;                          

public:
    explicit AudioManager(FileLoader& loader); 
    ~AudioManager();

    void begin();
    
    // setVolume a getVolume stále používají rozsah 0-100 pro uživatelské rozhraní
    void setVolume(uint8_t vol); 
    uint8_t getVolume() const;

    void playMusic(const char* path);
    void stopMusic();

    void handleAudio();
};

using MusicManager = AudioManager;