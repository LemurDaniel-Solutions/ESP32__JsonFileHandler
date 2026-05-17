#include <Arduino.h>
#include <jsonFileHandler.h>
#include <LittleFS.h>

JsonFileHandler::JsonFileHandler jfs;

void setup()
{
    Serial.begin(115200);
    delay(1000);

    LittleFS.begin(true);
    LittleFS.format();

    Serial.println("\n=== JsonFileHandler Tests ===\n");

    // randomString
    std::string rnd = jfs.randomString(8);
    Serial.printf("[randomString] length=%d value=%s\n", rnd.length(), rnd.c_str());

    // getTempFolder / getTempFile
    Serial.printf("[getTempFolder] %s\n", jfs.getTempFolder().c_str());
    Serial.printf("[getTempFile]   %s\n", jfs.getTempFile().c_str());

    // exists (nicht vorhanden)
    Serial.printf("[exists] /test.json vor write: %s\n", jfs.exists("/test.json") ? "true" : "false");

    // writeJson
    JsonDocument doc;
    doc["name"] = "ESP32";
    doc["value"] = 42;
    jfs.writeJson("/test.json", doc);

    // exists (vorhanden)
    Serial.printf("[exists] /test.json nach write: %s\n", jfs.exists("/test.json") ? "true" : "false");

    // readJson
    JsonDocument loaded = jfs.readJson("/test.json");
    Serial.printf("[readJson] name=%s value=%d\n",
        loaded["name"].as<const char *>(), loaded["value"].as<int>());

    // getInfo
    JsonFileHandler::Info info = jfs.getInfo("/test.json");
    Serial.printf("[getInfo] name=%s baseName=%s ext=%s isDir=%d\n",
        info.name.c_str(), info.baseName.c_str(), info.extension.c_str(), info.isDirectory);

    // listFiles
    jfs.writeJson("/a.json", doc);
    jfs.writeJson("/b.json", doc);
    std::vector<JsonFileHandler::Info> files = jfs.listFiles("/");
    Serial.printf("[listFiles] %d Eintraege gefunden\n", files.size());
    for (const auto &f : files)
        Serial.printf("  - %s\n", f.path.c_str());

    // moveFile
    LittleFS.mkdir("/dest");
    bool moved = jfs.moveFile("/a.json", "/dest");
    Serial.printf("[moveFile] a.json -> /dest: %s\n", moved ? "true" : "false");
    Serial.printf("[exists] /dest/a.json: %s\n", jfs.exists("/dest/a.json") ? "true" : "false");

    // removeFile
    bool removed = jfs.removeFile("/test.json");
    Serial.printf("[removeFile] test.json geloescht: %s\n", removed ? "true" : "false");
    Serial.printf("[exists] /test.json nach remove: %s\n", jfs.exists("/test.json") ? "true" : "false");

    // removeFile auf nicht-vorhandene Datei
    bool removedAgain = jfs.removeFile("/test.json");
    Serial.printf("[removeFile] nicht-vorhandene Datei: %s\n", removedAgain ? "true" : "false");

    // clearFolder
    jfs.writeJson("/dest/c.json", doc);
    jfs.clearFolder("/dest");
    std::vector<JsonFileHandler::Info> afterClear = jfs.listFiles("/dest");
    Serial.printf("[clearFolder] Dateien in /dest nach clear: %d\n", afterClear.size());

    // removeFolder
    jfs.writeJson("/todel/x.json", doc);
    jfs.removeFolder("/todel");
    Serial.printf("[removeFolder] /todel existiert nach remove: %s\n", jfs.exists("/todel") ? "true" : "false");

    Serial.println("\n=== Tests abgeschlossen ===");
}

void loop()
{
    while (true)
    {
        delay(10);
    }
}
