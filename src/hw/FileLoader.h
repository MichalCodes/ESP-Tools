#pragma once
#include <SD.h>
#include <SPI.h>

/**
 * @class FileLoader
 * @brief Třída pro zjednodušenou práci se soubory na SD kartě.
 * * @details Instance se vytváří standardním konstruktorem:
 * @code
 * FileLoader loader;
 * if (loader.begin()) {
 * // SD karta připravena
 * }
 * @endcode
 */
class FileLoader {
public:
    /**
     * @brief Inicializuje rozhraní SPI a připojí SD kartu.
     * @param csPin Chip Select pin (výchozí 10).
     * @param sck  Serial Clock pin (výchozí 12).
     * @param miso Master In Slave Out pin (výchozí 13).
     * @param mosi Master Out Slave In pin (výchozí 11).
     * @return true pokud byla inicializace úspěšná, jinak false.
     */
    bool begin(uint8_t csPin = 10, uint8_t sck = 12, uint8_t miso = 13, uint8_t mosi = 11);

    /**
     * @brief Otevře soubor na zadané cestě.
     * @param path Cesta k souboru (např. "/data.txt").
     * @return Objekt File pro další manipulaci.
     */
    File openFile(const char* path);

    /**
     * @brief Vypíše seznam souborů a adresářů do sériové linky.
     * @param dirname Výchozí adresář (výchozí kořen "/").
     * @param levels Úroveň zanoření pro rekurzivní výpis (výchozí 0).
     */
    void listFiles(const char* dirname = "/", uint8_t levels = 0);

    /**
     * @brief Zapíše textový řetězec do souboru.
     * @param path Cesta k souboru.
     * @param message Text k zápisu.
     * @param append Pokud je true, připíše text na konec; pokud false, soubor přepíše.
     * @return true při úspěchu, false při chybě.
     */
    bool writeFile(const char* path, const char* message, bool append);

    /**
     * @brief Uloží celočíselnou hodnotu do souboru.
     * @param path Cesta k souboru.
     * @param value Hodnota k uložení.
     * @return true při úspěchu, false při chybě.
     */
    bool saveInt(const char* path, int value);

private:
    void printDirectory(File dir, int numTabs);
};