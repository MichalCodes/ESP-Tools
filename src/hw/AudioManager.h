#pragma once
#include "FileLoader.h"
#include <AudioTools.h>
#include <AudioTools/AudioCodecs/CodecMP3Helix.h> 
#include <AudioTools/AudioCodecs/CodecWAV.h>      
#include <SD.h>

#define I2S_BCLK_PIN 42 
#define I2S_LRCK_PIN 18
#define I2S_DOUT_PIN 17

/**
 * @class AudioManager
 * @brief Třída pro správu přehrávání audia (MP3/WAV) z SD karty přes I2S rozhraní.
 * * @details Instance vyžaduje existující objekt FileLoader. Příklad inicializace:
 * @code
 * FileLoader loader;
 * AudioManager audio(loader);
 * * void setup() {
 * loader.begin();
 * audio.begin();
 * audio.playMusic("/track.mp3");
 * }
 * @endcode
 */
class AudioManager {
private:
    FileLoader& fileLoader;
    String currentMusicPath = "";
    float volumeLevel = 0.5;

    I2SStream i2sStream;                
    VolumeStream volumeStream;          
    EncodedAudioStream *decoder = nullptr; 
    StreamCopy copier;                       
    File audioFile;                          

public:
    /**
     * @brief Konstruktor správce audia.
     * @param loader Reference na inicializovanou instanci FileLoaderu.
     */
    explicit AudioManager(FileLoader& loader); 
    
    /**
     * @brief Destruktor zajišťující uvolnění prostředků dekodéru.
     */
    ~AudioManager();

    /**
     * @brief Inicializuje I2S sběrnici a audio řetězec.
     * @note Nastavuje piny definované makry I2S_BCLK_PIN, I2S_LRCK_PIN a I2S_DOUT_PIN.
     */
    void begin();

    /**
     * @brief Nastaví úroveň hlasitosti.
     * @param vol Hodnota hlasitosti (typicky 0-100 nebo 0-255 dle implementace).
     */
    void setVolume(uint8_t vol); 

    /**
     * @brief Získá aktuálně nastavenou hlasitost.
     * @return Aktuální úroveň hlasitosti.
     */
    uint8_t getVolume() const;

    /**
     * @brief Spustí přehrávání souboru ze zadané cesty.
     * @details Podporuje formáty .mp3 a .wav na základě přípony souboru.
     * @param path Cesta k audio souboru na SD kartě.
     */
    void playMusic(const char* path);

    /**
     * @brief Zastaví aktuální přehrávání a zavře soubor.
     */
    void stopMusic();

    /**
     * @brief Obslužná metoda, kterou je nutné volat v hlavní smyčce.
     * @details Zajišťuje kopírování dat z dekodéru do I2S streamu.
     */
    void handleAudio();
};

/**
 * @brief Alias pro AudioManager pro zpětnou kompatibilitu nebo sémantickou přehlednost.
 */
using MusicManager = AudioManager;