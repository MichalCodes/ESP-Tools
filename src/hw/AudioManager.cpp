#include "AudioManager.h"
#include <SD.h>
#include <Arduino.h> // pro Serial.println

// --- Implementace AudioManager ---

AudioManager::AudioManager(FileLoader& loader) : fileLoader(loader) {}

AudioManager::~AudioManager() {}

void AudioManager::begin() {
    Serial.println("AudioManager: Inicializace Audio odložena. Pinout je definován.");
    // Zde bude později konfigurace I2S pinů:
    // Např. audioOutput = new AudioOutputI2SNoDAC();
    // audioOutput->SetPinout(I2S_BCLK_PIN, I2S_LRCK_PIN, I2S_DOUT_PIN);
}

void AudioManager::setVolume(uint8_t vol) {
    // PLACEHOLDER: Bude implementováno později
    volumeLevel = (vol > 100 ? 100 : vol) / 100.0f;
    Serial.printf("AudioManager: Hlasitost nastavena na %d%% (placeholder).\n", (int)(volumeLevel * 100));
}

uint8_t AudioManager::getVolume() const {
    // PLACEHOLDER: Bude implementováno později
    return (uint8_t)(volumeLevel * 100);
}

void AudioManager::playMusic(const char* path) {
    // PLACEHOLDER: Bude implementováno později
    currentMusicPath = path;
    Serial.printf("AudioManager: Požadavek na přehrávání hudby: %s (placeholder).\n", path);
}

void AudioManager::stopMusic() {
    // PLACEHOLDER: Bude implementováno později
    Serial.println("AudioManager: Požadavek na zastavení hudby (placeholder).");
    currentMusicPath = "";
}

void AudioManager::handleAudio() {
    // PLACEHOLDER: Bude implementováno později
    // Zde bude později audio.loop() nebo mp3Generator->loop()
}