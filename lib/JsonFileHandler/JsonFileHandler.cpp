// Copyright © 2026, Daniel Landau - LemurDaniel
// MIT License

#include "jsonFileHandler.h"

namespace JsonFileHandler
{

    /*-------------------------------------------------------------------------------------------------
     *
     * Helper Methods
     *
     **/

    std::string JsonFileHandler::getTempFolder()
    {
        return _tempFolder;
    }

    std::string JsonFileHandler::getTempFile()
    {
        return _tempFolder + "/" + randomString(8);
    }

    std::string JsonFileHandler::randomString(int size)
    {
        std::string charSet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
        std::string result;
        for (int i = 0; i < size; i++)
        {
            int index = random(0, charSet.length());
            result += charSet[index];
        }
        return result;
    }

    Info JsonFileHandler::getInfo(const std::string &path)
    {
        Info info;

        if (!LittleFS.begin())
        {
            Serial.println("Failed to mount LittleFs");
            return info;
        }

        File file = LittleFS.open(path.c_str(), "r");
        if (!file)
        {
            Serial.println(("Failed to open file: " + path).c_str());
            return info;
        }

        info.isDirectory = file.isDirectory();
        info.name = file.name();
        info.path = file.path();

        info.baseName = info.name;
        info.extension = "";

        size_t posDot = info.name.rfind('.');
        if (posDot != std::string::npos && posDot > 0)
        {
            info.extension = info.name.substr(posDot + 1);
            info.baseName = info.name.substr(0, posDot);
        }

        file.close();

        return info;
    }

    /*-------------------------------------------------------------------------------------------------
     *
     * Folder and Folder Contents
     *
     **/

    std::vector<Info> JsonFileHandler::listFiles(const std::string &path)
    {
        std::vector<Info> files;

        if (!LittleFS.begin())
        {
            Serial.println("Failed to mount LittleFs");
            return files;
        }

        File folder = LittleFS.open(path.c_str());
        if (!folder || !folder.isDirectory())
        {
            return files;
        }

        std::string filePath = folder.getNextFileName().c_str();
        while (!filePath.empty())
        {
            Info info = getInfo(filePath);
            files.push_back(info);
            filePath = folder.getNextFileName().c_str();
        }

        return files;
    }

    void JsonFileHandler::clearFolder(const std::string &path)
    {
        if (!LittleFS.begin())
        {
            Serial.println("Failed to mount LittleFs");
            return;
        }

        File dir = LittleFS.open(path.c_str());
        if (!dir || !dir.isDirectory())
        {
            dir.close();
            return;
        }

        File entry = dir.openNextFile();
        while (entry)
        {
            std::string entryPath = entry.path();
            bool isDir = entry.isDirectory();
            entry.close();

            if (isDir)
                removeFolder(entryPath);
            else
                LittleFS.remove(entryPath.c_str());

            entry = dir.openNextFile();
        }
        dir.close();
    }

    void JsonFileHandler::removeFolder(const std::string &path)
    {
        clearFolder(path);
        LittleFS.rmdir(path.c_str());
    }

    /*-------------------------------------------------------------------------------------------------
     *
     *  Files
     *
     **/
    bool JsonFileHandler::exists(const std::string &path)
    {
        if (!LittleFS.begin())
        {
            Serial.println("Failed to mount LittleFs");
            return false;
        }

        return LittleFS.exists(path.c_str());
    }

    bool JsonFileHandler::removeFile(const std::string &path)
    {
        if (!LittleFS.begin())
        {
            Serial.println("Failed to mount LittleFs");
            return false;
        }

        if (!exists(path))
        {
            Serial.printf("ℹ️ No file found at %s to remove.\n", path.c_str());
            return false;
        }

        return LittleFS.remove(path.c_str());
    }

    bool JsonFileHandler::moveFile(const std::string &path, const std::string &destination)
    {
        if (!LittleFS.begin())
        {
            Serial.println("Failed to mount LittleFs");
            return false;
        }

        if (!exists(path))
        {
            Serial.printf("ℹ️ No file found at %s to move.\n", path.c_str());
            return false;
        }

        const Info info = getInfo(path);
        std::string destinationPath = destination + "/" + info.name;

        Serial.printf("ℹ️ Move from %s to %s\n", path.c_str(), destinationPath.c_str());

        return LittleFS.rename(path.c_str(), destinationPath.c_str());
    }

    /*-------------------------------------------------------------------------------------------------
     *
     * Read and write JSON
     *
     **/

    JsonDocument JsonFileHandler::readJson(const std::string &path)
    {
        if (!LittleFS.begin())
        {
            Serial.println("Failed to mount LittleFs");
            return JsonDocument();
        }

        if (!exists(path))
        {
            Serial.printf("❌ CRITICAL: JSON file %s not found!\n", path.c_str());
            return JsonDocument();
        }

        File file = LittleFS.open(path.c_str(), "r");
        if (!file)
        {
            Serial.printf("❌ CRITICAL: Failed to open JSON file %s for reading!\n", path.c_str());
            return JsonDocument();
        }

        size_t size = file.size();
        std::string jsonStr(size, '\0');
        file.readBytes(&jsonStr[0], size);
        file.close();

        JsonDocument doc;
        DeserializationError error = deserializeJson(doc, jsonStr);
        if (error)
        {
            Serial.printf("❌ CRITICAL: Failed to parse JSON file %s! Error: %s\n", path.c_str(), error.c_str());
            return JsonDocument();
        }

        return doc;
    }

    void JsonFileHandler::writeJson(const std::string &path, const JsonDocument &doc)
    {
        if (!LittleFS.begin())
        {
            Serial.println("Failed to mount LittleFs");
            return;
        }

        File file = LittleFS.open(path.c_str(), "w", true);
        if (!file)
        {
            Serial.printf("❌ CRITICAL: Failed to open JSON file %s for writing!\n", path.c_str());
            return;
        }

        std::string jsonStr;
        serializeJson(doc, jsonStr);

        file.print(jsonStr.c_str());
        file.close();

        Serial.printf("✅ Successfully wrote JSON file %s\n", path.c_str());
    }
}
