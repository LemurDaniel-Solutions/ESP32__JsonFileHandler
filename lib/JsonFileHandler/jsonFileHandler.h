// https://github.com/LemurDaniel-Solutions/ESP32__JsonFileHandler
// Copyright © 2026, Daniel Landau - LemurDaniel
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

        std::string trim(const std::string &s, const std::string &trimmer = " ");
        std::vector<std::string> split(const std::string &text, const std::string &splitter);

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

        bool removeFile(const std::string &path);
        bool moveFile(const std::string &path, const std::string &destination);

        JsonDocument readJson(const std::string &path);
        void writeJson(const std::string &path, const JsonDocument &doc);
    };

}
