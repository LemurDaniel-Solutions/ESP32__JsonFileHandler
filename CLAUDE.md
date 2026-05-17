# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build & Flash (PlatformIO)

```bash
# Build
pio run -e freenove_esp32_wrover
pio run -e ESP32-WROOM-32

# Flash firmware
pio run -e freenove_esp32_wrover -t upload

# Upload LittleFS filesystem image (data/ directory)
pio run -e freenove_esp32_wrover -t uploadfs

# Serial monitor
pio device monitor
```

## Architecture

This is simultaneously a **PlatformIO library** (`library.json`) and a **PlatformIO project** (`platformio.ini`) — the project in `src/` serves as a dev/test harness for the library in `lib/`.

**Library code** lives in `lib/`:
- `jsonFileHandler.h` — declares the `Info` struct and `JsonFileHandler` class inside `namespace JsonFileHandler`
- `JsonFileHandler.cpp` — implements all methods; must use `JsonFileHandler::MethodName` prefix for every method (the namespace wrapping alone is not sufficient)

**String types:** use `std::string` throughout — not Arduino `String`. All LittleFS API calls require `.c_str()` since LittleFS expects `const char*`.

**Key constraints:**
- LittleFS is mounted via `LittleFS.begin()` at the top of each public method — there is no persistent mount state in the class
- Always null-check `File` objects after `LittleFS.open()` before accessing them
- Store `entry.path()` and `entry.isDirectory()` before calling `entry.close()` — the data is unavailable after close
- `std::string::rfind()` returns `std::string::npos` (not -1) when not found — always check against `npos` before using the result

**Filesystem layout on device:**
- `data/` → uploaded to LittleFS root via `uploadfs`; contains web assets (`data/web/`)
- `_tempFolder` defaults to `/temp` — used for staging files before atomic moves

**Dependencies:** ArduinoJson 7.x (`bblanchon/ArduinoJson@~7.4.3`), LittleFS (built into ESP32 Arduino core)
