# ESP32 JsonFileHandler

<div align="center">

![ESP32](https://img.shields.io/badge/ESP32-000000?style=for-the-badge&logo=Espressif&logoColor=white)
![Arduino](https://img.shields.io/badge/Arduino-00979D?style=for-the-badge&logo=Arduino&logoColor=white)
![PlatformIO](https://img.shields.io/badge/PlatformIO-FF6000?style=for-the-badge&logo=PlatformIO&logoColor=white)
![C++](https://img.shields.io/badge/C++-00599C?style=for-the-badge&logo=cplusplus&logoColor=white)

### ⚠️ Prototype — Personal Project

</div>

## Overview

A lightweight PlatformIO library for reading and writing JSON files on ESP32 via LittleFS. Wraps ArduinoJson 7.x and LittleFS into a single class with file management utilities.

Compatible boards:
- ESP32 (WROOM, WROVER, CAM, ...)
- ESP32-S2, ESP32-S3

May not work on RISC-V variants (ESP32-C3, ESP32-C6, ESP32-H2).

## Installation

Add to your `platformio.ini`:

```ini
[env:your_board]
platform = espressif32
framework = arduino
board = <your-board>
board_build.filesystem = littlefs

lib_deps =
    https://github.com/LemurDaniel/ESP32__JsonFileHandler.git
    bblanchon/ArduinoJson@~7.4.3
```

## Usage

```cpp
#include <jsonFileHandler.h>

JsonFileHandler::JsonFileHandler fs;

void setup() {
    Serial.begin(115200);

    // Read JSON
    JsonDocument doc = fs.readJson("/config.json");
    const char* name = doc["name"];

    // Write JSON
    JsonDocument out;
    out["status"] = "ok";
    fs.writeJson("/status.json", out);

    // List files
    std::vector<JsonFileHandler::Info> files = fs.listFiles("/");
    for (const auto& f : files) {
        Serial.println(f.path);
    }
}
```

## API

### File Info

```cpp
Info getInfo(const String &path);
```

Returns an `Info` struct:

| Field | Type | Description |
|-------|------|-------------|
| `name` | `String` | Filename (e.g. `config.json`) |
| `path` | `String` | Full path (e.g. `/data/config.json`) |
| `baseName` | `String` | Name without extension |
| `extension` | `String` | Extension without dot (e.g. `json`) |
| `isDirectory` | `int` | Non-zero if the entry is a directory |

### Filesystem Operations

```cpp
bool exists(const String &path);

std::vector<Info> listFiles(const String &path);

void clearFolder(const String &path);   // deletes all contents, keeps the folder
void removeFolder(const String &path);  // deletes folder and all contents

void moveFile(const String &path, const String &destination);
void removeFile(const String &path);
```

### JSON

```cpp
JsonDocument readJson(const String &path);
void writeJson(const String &path, const JsonDocument &doc);
```

### Temp Files

Useful for atomic writes — write to a temp path, then move it into place:

```cpp
String getTempFolder();          // returns "/temp"
String getTempFile();            // returns "/temp/<random8chars>"
```
