#include "AudioManager.h"
#include <Arduino.h>

AudioManager::AudioManager(FileLoader& loader) 
    : fileLoader(loader), 
      volumeStream(i2sStream) 
{} 

AudioManager::~AudioManager() {
    stopMusic();
}

void AudioManager::begin() {
    auto cfg = i2sStream.defaultConfig();
    cfg.pin_bck = I2S_BCLK_PIN;
    cfg.pin_ws = I2S_LRCK_PIN;
    cfg.pin_data = I2S_DOUT_PIN;
    cfg.channels = 2; 
    cfg.sample_rate = 44100; 
    cfg.bits_per_sample = 16;
    cfg.use_apll = true; 

    i2sStream.begin(cfg);
    
    AudioInfo defaultInfo(cfg.sample_rate, cfg.channels, cfg.bits_per_sample);
    volumeStream.begin(defaultInfo); 
    
    // Aplikujeme výchozí/uloženou hlasitost
    setVolume(getVolume());
}

void AudioManager::setVolume(uint8_t vol) {
    // Převod rozsahu 0-100 (pro UI) na 0.0-1.0 (pro VolumeStream)
    volumeLevel = (vol > 100 ? 100 : vol) / 100.0f;
    
    // Tuto hodnotu pošleme do VolumeStream
    volumeStream.setVolume(volumeLevel);
}

uint8_t AudioManager::getVolume() const {
    // Převod zpět na rozsah 0-100 pro UI
    return (uint8_t)(volumeLevel * 100);
}

void AudioManager::playMusic(const char* path) {
    stopMusic(); 
    currentMusicPath = path;

    audioFile = fileLoader.openFile(path); 
    if (!audioFile) {
        currentMusicPath = "";
        return;
    }

    if (currentMusicPath.endsWith(".mp3")) {
        decoder = new EncodedAudioStream(&volumeStream, new MP3DecoderHelix()); 
    } else if (currentMusicPath.endsWith(".wav")) {
        decoder = new EncodedAudioStream(&volumeStream, new WAVDecoder());
    } else {
        currentMusicPath = "";
        audioFile.close();
        return;
    }

    if (decoder) {
        decoder->begin();
        AudioInfo info = decoder->audioInfo();

        volumeStream.begin(info); 
        volumeStream.setVolume(volumeLevel); // APLIKUJEME AKTUÁLNÍ HLASITOST
        i2sStream.setAudioInfo(info); 
        
        copier.begin(*decoder, audioFile);
    }
}

void AudioManager::stopMusic() {
    if (decoder) {
        copier.end(); 
        decoder->end();
        delete decoder;
        decoder = nullptr;
    }
    if (audioFile) {
        audioFile.close();
    }
    currentMusicPath = "";
}

void AudioManager::handleAudio() {
    if (decoder) {
        copier.copy();
    }
}