// ESP32 MiniWebServer Framework - https://github.com/LemurDaniel/ESP32__MiniWebServer-Framework
// Copyright © 2026, Daniel Landau
// MIT License

#pragma once

#include <Arduino.h>
#include <LittleFS.h>
#include <ArduinoJson.h>
#include <string>
#include <vector>

namespace JsonFileHandler
{
    struct Info
    {
        int isDirectory;
        std::string name;
        std::string path;
        std::string extension;
        std::string baseName;
    };

    class JsonFileHandler
    {

    private:
        std::string _tempFolder = "/temp";

    public:
        /*-------------------------------------------------------------------------------------------------
         *
         * Helper Methods
         *
         **/
        std::string getTempFolder();
        std::string getTempFile();
        std::string randomString(int size);
        Info getInfo(const std::string &path);

        /*-------------------------------------------------------------------------------------------------
         *
         * Folder and Folder Contents
         *
         **/

        std::vector<Info> listFiles(const std::string &path);
        void clearFolder(const std::string &path);
        void removeFolder(const std::string &path);

        /*-------------------------------------------------------------------------------------------------
         *
         *  Files
         *
         **/
        bool exists(const std::string &path);

        void removeFile(const std::string &path);
        void moveFile(const std::string &path, const std::string &destination, const std::string &relativePath = "");

        JsonDocument readJson(const std::string &path);
        void writeJson(const std::string &path, const JsonDocument &doc);
    };

}
