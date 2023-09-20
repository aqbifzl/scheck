#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include <filesystem>
#include <fstream>
#include <string>
#include <string_view>

class FileHandler {
public:
  FileHandler(const std::filesystem::path &p);
  ~FileHandler();
  static bool checkIfPathExist(std::filesystem::path const &p);
  static bool checkIfPathIsDir(std::filesystem::path const &p);
  static void pathCheck(std::filesystem::path const &p,
                        bool (*checkFunc)(std::filesystem::path const &),
                        bool desiredResult, std::string_view msg);
  std::fstream fileToHandle;

private:
  std::filesystem::path filePath{};
};

#endif // !FILE_HANDLER_H
