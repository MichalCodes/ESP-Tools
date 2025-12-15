#include "FileLoader.h"
#include <Arduino.h>

bool FileLoader::begin(uint8_t csPin, uint8_t sck, uint8_t miso, uint8_t mosi) {
    SPI.begin(sck, miso, mosi);
    if (!SD.begin(csPin)) {
        Serial.println("SD ERROR: Inicializace selhala!");
        return false;
    }
    return true;
}

File FileLoader::openFile(const char* path) {
    String p = path;
    
    if (!p.startsWith("/")) {
        p = "/" + p;
    }

    File file = SD.open(p.c_str());
    
    if (!file) {
        if (p.endsWith(".mp3") || p.endsWith(".wav")) {
            String p_alt = "/music" + p; 
            file = SD.open(p_alt.c_str());
        }
    }

    return file;
}

void FileLoader::printDirectory(File dir, int numTabs) {
    while(true) {
        File entry = dir.openNextFile();
        if (! entry) {
            break;
        }
        if (entry.isDirectory()) {
            printDirectory(entry, numTabs+1);
        }
        entry.close();
    }
}

void FileLoader::listFiles(const char* dirname, uint8_t levels) {
    File root = SD.open(dirname);
    if (!root || !root.isDirectory()) { return; }

    File file = root.openNextFile();
    while (file) {
        if (file.isDirectory()) {
            if (levels) {
                printDirectory(file, 1);
            }
        }
        file = root.openNextFile();
    }
}