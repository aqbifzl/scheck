#ifndef PARAMS_HANDLER_H
#define PARAMS_HANDLER_H

#include <boost/program_options.hpp>
#include <cstdlib>
#include <string>
#include <string_view>
#include <unistd.h>
#include <vector>

class ParamsHandler {
public:
  using excludeType = std::vector<std::string>;
  explicit ParamsHandler(int argc, char **argv)
      : pipedStdinExists(!isatty(STDIN_FILENO)), mArgc(argc), mArgv(argv){};
  struct parsedExclude {
    std::vector<std::string> extensions{};
    std::vector<std::string> paths{};
  };
  struct checkerConfig {
    std::string target{};
    std::string wordlist{};
    std::string ignore{};
    std::string extension{};
    int min{};
    int max{};
    parsedExclude exclude{};
  };
  struct tomlConfig {
    std::string wordlist{};
    int min{-1};
    int max{-1};
  };
  checkerConfig &getConfig();

private:
  void showHelp() const;
  void handleArgsException(std::exception &e);
  bool hasFileConfig() const;
  void getTomlConfig();
  parsedExclude parseExclude(excludeType &toExclude);
  const std::string configName{"/.spellconf.toml"};
  tomlConfig configFromFile{};
  checkerConfig result{};
  bool pipedStdinExists{!isatty(STDIN_FILENO)};
  int mArgc{};
  char **mArgv;
  const std::string configPath{std::string(getenv("HOME")) + configName};
  boost::program_options::options_description desc{};
};

#endif // !PARAMS_HANDLER_H
