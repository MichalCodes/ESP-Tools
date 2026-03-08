# ESP_Tools

### Hlavní Funkce

* **Dotykové ovládání:** Třídy pro `Button`, `Joystick`, `SwipeController`.
* **Ovládání HW:** Abstraktní vrstva pro přehrávání audia (MP3/WAV) a správu SD karty (`FileLoader`, `AudioManager`).
* **Modulární UI:** Základní třída `IAppModule` a hlavní menu (`MainScreen`) pro snadné přepínání mezi aplikacemi/hrami (např. 2048, MazeGame).

## 🧩 Přehled Komponent Knihovny ESP\_Tools

Všechny hlavní moduly se nachází ve složce `src/` a jsou podmiňovány centrálním souborem `tools.h` pro optimalizaci kompilace.

### 1. HARDWARE & SYSTÉM (`src/hw/`)

Moduly pro správu periférií, médií a základního nastavení.

| Modul | Soubory | Popis |
| :--- | :--- | :--- |
| **Konfigurace GFX** | `gfx_conf.h` | Statická definice nastavení displeje, pinů a inicializace LovyanGFX. Klíčový pro správné fungování UI. |
| **Správce Souborů** | `FileLoader.h`, `.cpp` | Abstraktní vrstva pro přístup k souborům na SD kartě nebo jiném filesystému (např. SPIFFS). Zajišťuje seznam souborů a bezpečné čtení. |
| **Správce Audia** | `AudioManager.h`, `.cpp` | Modul pro obsluhu audio přehrávání (přes AudioTools/I2S). Stará se o dekódování, přehrávání hudby na pozadí a ovládání hlasitosti. **Závisí na `FileLoader`**. |

### 2. UŽIVATELSKÝ VSTUP (`src/touch/`)

Komponenty pro interakci uživatele s dotykovým displejem.

| Modul | Soubory | Popis |
| :--- | :--- | :--- |
| **Tlačítko** | `Button.h`, `.cpp` | Základní UI prvek. Zajišťuje vykreslení a detekci stisku v definované oblasti. |
| **Virtuální Joystick** | `Joystick.h`, `.cpp` | Pokročilý dotykový ovladač, který simuluje analogový joystick. Obsluhuje pohyb a automatické centrování. |
| **Kontroler Gesta** | `SwipeController.h`, `.cpp` | Detekuje gesta tažení (švih/swipe) – nahoru, dolů, doleva, doprava. Ideální pro listování v menu nebo pro hry (např. 2048). |
| **Posuvník** | `TrackBar.h`, `.cpp` | UI prvek pro výběr numerické hodnoty v rozsahu (např. hlasitost nebo jas displeje). |

### 3. ZÁKLADNÍ UI & ARCHITEKTURA (`src/baseUI/`)

Definice pro modulární strukturu a navigaci v aplikaci.

| Modul | Soubory | Popis | |
| :--- | :--- | :--- |
| **Modul Aplikace** | `IAppModule.h` | Abstraktní třída (`interface`), ze které musí dědit každá hra/aplikace (`MazeGame`, `Game2048`). Definuje metody jako `run()`, `update()`, `exitGame()`. |
| **Hlavní Obrazovka** | `MainScreen.h`, `.cpp` | Centralizovaný modul pro správu a spouštění přidaných `IAppModule`. Vykresluje hlavní menu a řídí navigaci. |
| **Správce Nastavení** | `Settings.h`, `.cpp` | Modul pro správu stavu a konfigurace aplikace (např. persistentní uložení dat). |

### 4. PŘÍKLADY A DEMO (`src/examples/`)

Ukázky implementace, které demonstrují použití modulů.

| Modul | Soubory | Popis |
| **Hra 2048** | `Game2048.hpp` | Implementace hry, která využívá **`SwipeController`**, **`Button`**. |
| **Hra Rat Maze** | `MazeGame.hpp` | Implementace hry, která využívá **`Joystick`**, **`FileLoader`** a **`AudioManager`**, **`Button`**. |
| **Hra Tetris** | `Tetris.hpp` | Implementace hry, která pravděpodobně využívá **`SwipeController`** (pro pohyb kostek) nebo **`Joystick`** a **`AudioManager`**. |
---

### Inteligentní Include (`tools.h`)

Klíčovým prvkem je soubor **`src/tools.h`**. Ten slouží jako centrální bod, který na základě maker definovaných v `main.cpp` (např. `#define JOYSTICK_H`) podmiňuje, které hlavičkové soubory se skutečně includují. Tím se zajišťuje, že kompilátor zpracuje jen nezbytné `.cpp` soubory, čímž se minimalizuje velikost binárního kódu a zrychlí kompilace.
---

## 🚀 Rychlý Start (PlatformIO)

Tato knihovna je primárně určena pro použití v prostředí **PlatformIO (VS Code)** mohla by fungovat i v dalších ale.

### 1. Vytvoření Nového Projektu

Vytvořte nový projekt v PlatformIO a nastavte jej pro váš ESP32 (např: `board = esp32s3`, `devkit = esp32 Feather`, `framework = arduino` ). Konkrétní konfigurace na které to bylo testováno je dole.

### 2. Přidání Závislosti

Do konfiguračního souboru **`platformio.ini`** vložte odkaz na tento repozitář v sekci `lib_deps`.

```ini
[env:esp32s3_crowpanel]
platform = espressif32@6.5.0
board = esp32-s3-devkitc-1
framework = arduino

board_build.mcu = esp32s3
board_build.f_cpu = 240000000L

board_build.arduino.memory_type = qio_opi
board_build.flash_mode = qio
board_build.f_flash = 80000000L
board_build.psram = enabled
board_upload.flash_size = 4MB
board_build.partitions = partitions/no_ota_2mb_app_2mb_fatfs.csv

monitor_speed = 115200
upload_speed = 921600
monitor_filters = esp32_exception_decoder

build_flags =
    -DARDUINO_USB_CDC_ON_BOOT=1
    -DARDUINO_USB_MODE=1
    -DCONFIG_SPIRAM_CACHE_WORKAROUND

lib_deps =
    # Vaše knihovna ESP_Tools z GitHubu
    [https://github.com/MichalCodes/ESP-Tools.git](https://github.com/MichalCodes/ESP-Tools.git)



Flash partitions na kterých to je testováno:
# Name,   Type, SubType,  Offset,   Size,     Flags
nvs,      data, nvs,      0x9000,   0x5000,
otadata,  data, ota,      0xe000,   0x2000,
app0,     app,  factory,  0x10000,  0x200000,
ffat,     data, fat,      0x210000, 0x1F0000,

(v arduinu volba -> No OTA(2MB APP/2MB FATFS))
