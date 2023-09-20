#include "fileHandler.h"
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <string_view>

namespace fs = std::filesystem;

FileHandler::FileHandler(const std::filesystem::path &p) : filePath(p) {
  if (fileToHandle.is_open()) {
    std::cerr << "Error, file is already opened\n";
    exit(1);
  }
  fileToHandle.open(filePath);
  if (!fileToHandle.is_open()) {
    std::cerr << "Error opening a file";
    exit(1);
  }
};

FileHandler::~FileHandler() {
  if (fileToHandle.is_open()) {
    fileToHandle.close();
  }
}

bool FileHandler::checkIfPathExist(fs::path const &p) { return fs::exists(p); }
bool FileHandler::checkIfPathIsDir(fs::path const &p) {
  return fs::is_directory(p);
}
void FileHandler::pathCheck(fs::path const &p,
                            bool (*checkFunc)(fs::path const &),
                            bool desiredResult, std::string_view msg) {
  if (checkFunc(p) != desiredResult) {
    std::cerr << "Error with " << p << ' ' << msg;
    exit(1);
  }
}
