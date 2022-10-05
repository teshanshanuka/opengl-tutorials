#pragma once

#include <string>
#include <filesystem>

namespace utils {
    std::string getFilePath(const std::string& file){
        return file.substr(0, file.rfind("/"));
    }

    void asserFileExists(const std::string& file){
        if (!std::filesystem::exists(file))
            throw std::runtime_error("File '" + file + "' does not exist");
    }
}