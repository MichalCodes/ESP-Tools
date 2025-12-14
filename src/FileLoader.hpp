#pragma once
#include <SD.h>
#include <SPI.h>

class FileLoader {
public:
    /**
     * @brief Inicializuje SD kartu přes SPI.
     * @return true pokud je OK
     */
    bool begin(uint8_t csPin = 10, uint8_t sck = 12, uint8_t miso = 13, uint8_t mosi = 11) {
        Serial.print("📀 Inicializuji SD kartu (SPI mód)... ");

        SPI.begin(sck, miso, mosi);
        if (!SD.begin(csPin)) {
            Serial.println("❌ Selhalo");
            return false;
        }

        Serial.println("✅ SD karta mountnuta!");
        return true;
    }

    /**
     * @brief Otevře soubor pro čtení
     */
    File openFile(const char* path) {
        File file = SD.open(path);
        if (!file) {
            Serial.printf("❌ Soubor nenalezen: %s\n", path);
        }
        return file;
    }

    /**
     * @brief Vypíše obsah adresáře
     */
    void listFiles(const char* dirname = "/", uint8_t levels = 0) {
        Serial.printf("Listing directory: %s\n", dirname);
        File root = SD.open(dirname);
        if (!root) { Serial.println("❌ Nelze otevřít"); return; }
        if (!root.isDirectory()) { Serial.println("❌ Není adresář"); return; }

        File file = root.openNextFile();
        while (file) {
            if (file.isDirectory()) {
                Serial.printf("📁  %s/\n", file.name());
                if (levels) listFiles(file.name(), levels - 1);
            } else {
                Serial.printf("📄  %s (%u bytes)\n", file.name(), (unsigned int)file.size());
            }
            file = root.openNextFile();
        }
    }
};
