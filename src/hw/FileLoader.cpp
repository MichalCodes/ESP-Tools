#include "FileLoader.h"
#include <Arduino.h>

bool FileLoader::begin(uint8_t csPin, uint8_t sck, uint8_t miso, uint8_t mosi) {
    Serial.print(" Inicializuji SD kartu (SPI mód)... ");

    SPI.begin(sck, miso, mosi);
    if (!SD.begin(csPin)) {
        Serial.println(" Selhalo");
        return false;
    }

    Serial.println(" SD karta mountnuta!");
    return true;
}

File FileLoader::openFile(const char* path) {
    File file = SD.open(path);
    if (!file) {
        Serial.printf(" Soubor nenalezen: %s\n", path);
    }
    return file;
}

void FileLoader::printDirectory(File dir, int numTabs) {
    while(true) {
        File entry = dir.openNextFile();
        if (! entry) {
            break;
        }
        for (uint8_t i=0; i<numTabs; i++) {
            Serial.print('\t');
        }
        Serial.print(entry.name());
        if (entry.isDirectory()) {
            Serial.println("/");
            printDirectory(entry, numTabs+1);
        } else {
            Serial.print("\t\t");
            Serial.println(entry.size(), DEC);
        }
        entry.close();
    }
}

void FileLoader::listFiles(const char* dirname, uint8_t levels) {
    Serial.printf("Listing directory: %s\n", dirname);
    File root = SD.open(dirname);
    if (!root) { Serial.println(" Nelze otevřít"); return; }
    if (!root.isDirectory()) { Serial.println(" Není adresář"); return; }

    File file = root.openNextFile();
    while (file) {
        if (file.isDirectory()) {
            Serial.print("  DIR : ");
            Serial.println(file.name());
            if (levels) {
                printDirectory(file, 1);
            }
        } else {
            Serial.print("  FILE: ");
            Serial.print(file.name());
            Serial.print("  SIZE: ");
            Serial.println(file.size());
        }
        file.close();
        file = root.openNextFile();
    }
}