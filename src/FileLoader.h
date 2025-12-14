#pragma once
#include <SD.h>
#include <SPI.h>

class FileLoader {
public:
    bool begin(uint8_t csPin = 10, uint8_t sck = 12, uint8_t miso = 13, uint8_t mosi = 11);
    File openFile(const char* path);
    void listFiles(const char* dirname = "/", uint8_t levels = 0);
private:
    void printDirectory(File dir, int numTabs);
};