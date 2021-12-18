#pragma once

#include <sys/stat.h>
#include <experimental/filesystem>

class FileHelper {
  public:
    static bool DoesFileExist(const std::string &name) {
        struct stat buffer;
        return(stat(name.c_str(), &buffer) == 0);
    }

    static void ChangeDirectory(const std::string &newDirectory) {
        std::experimental::filesystem::current_path(newDirectory);
    }
};
