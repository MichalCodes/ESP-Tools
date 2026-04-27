# 🛠️ ESP_Tools Framework

Framework pro ESP32 zaměřený na modulární tvorbu uživatelského rozhraní, správu hardwaru a multimédií. Knihovna je navržena pro efektivní vývoj aplikací a her s dotykovým ovládáním.

---

## 🌟 Hlavní Funkce

* **Interaktivní UI:** Třídy pro `Button`, `Joystick`, `SwipeController` a `TrackBar`.
* **Ovládání HW:** Abstraktní vrstva pro přehrávání audia (MP3/WAV) a správu SD karty (`FileLoader`, `AudioManager`).
* **Modulární UI:** Základní třída `IAppModule` a hlavní menu (`MainScreen`) pro snadné přepínání mezi aplikacemi/hrami.

---

## 🧩 Přehled Komponent Knihovny

Všechny hlavní moduly se nachází ve složce `src/` a jsou podmiňovány centrálním souborem `tools.h` pro optimalizaci kompilace.

### 1. HARDWARE & SYSTÉM (`src/hw/`)
| Modul | Soubory | Popis |
| :--- | :--- | :--- |
| **Konfigurace GFX** | `gfx_conf.h` | Statická definice nastavení displeje, pinů a inicializace LovyanGFX. |
| **Správce Souborů** | `FileLoader.h/.cpp` | Abstraktní vrstva pro přístup k souborům na SD kartě či SPIFFS. |
| **Správce Audia** | `AudioManager.h/.cpp` | Obsluha audio přehrávání (I2S). Závisí na `FileLoader`. |

### 2. UŽIVATELSKÝ VSTUP (`src/touch/`)
| Modul | Soubory | Popis |
| :--- | :--- | :--- |
| **Tlačítko** | `Button.h/.cpp` | Zajišťuje vykreslení a detekci stisku v oblasti. |
| **Virtuální Joystick** | `Joystick.h/.cpp` | Simuluje analogový joystick s automatickým centrováním. |
| **Kontroler Gesta** | `SwipeController.h/.cpp` | Detekuje švihy (nahoru, dolů, vlevo, vpravo). |
| **Posuvník** | `TrackBar.h/.cpp` | UI prvek pro výběr numerické hodnoty (např. hlasitost). |

### 3. ZÁKLADNÍ UI & ARCHITEKTURA (`src/baseUI/`)
| Modul | Soubory | Popis |
| :--- | :--- | :--- |
| **Modul Aplikace** | `IAppModule.h` | Interface, ze kterého dědí aplikace (`run`, `update`, `exit`). |
| **Hlavní Obrazovka** | `MainScreen.h/.cpp` | Správa a spouštění modulů, vykreslování menu. |
| **Správce Nastavení** | `Settings.h/.cpp` | Správa stavu a konfigurace aplikace. |

### 4. PŘÍKLADY A DEMO (`src/examples/`)
* **Hra 2048** (`Game2048.hpp`): Využívá `SwipeController` a `Button`.
* **Hra Rat Maze** (`MazeGame.hpp`): Využívá `Joystick`, `FileLoader`, `AudioManager` a `Button`.
* **Hra Tetris** (`Tetris.hpp`): Využívá `SwipeController` nebo `Joystick` a `AudioManager`.

---

### 💡 Inteligentní Include (`tools.h`)
Klíčovým prvkem je soubor **`src/tools.h`**. Ten na základě maker definovaných v `main.cpp` (např. `#define USE_JOYSTICK`) podmiňuje, které hlavičkové soubory se skutečně includují. Tím se minimalizuje velikost binárního kódu a zrychlí kompilace.

---

## 🚀 Rychlý Start (PlatformIO)

### 1. Konfigurační soubor `platformio.ini`
Vložte následující konfiguraci (testováno na ESP32-S3):

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
    # Knihovna ESP_Tools z GitHubu
    [https://github.com/MichalCodes/ESP-Tools.git](https://github.com/MichalCodes/ESP-Tools.git)
