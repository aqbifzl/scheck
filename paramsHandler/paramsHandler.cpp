#include "paramsHandler.h"
#include "../fileHandler/fileHandler.h"
#include <algorithm>
#include <boost/program_options.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/value_semantic.hpp>
#include <boost/program_options/variables_map.hpp>
#include <cctype>
#include <cstdlib>
#include <exception>
#include <filesystem>
#include <iostream>
#include <string>
#include <toml++/impl/path.h>
#include <toml++/toml.h>
#include <tuple>
#include <vector>

namespace po = boost::program_options;

void ParamsHandler::showHelp() const {
  std::cout
      << "Usage:\n"
         "scheck -t [target]\n"
         "-t or --target - set a target file or directory\n"
         "-w or --wordlist - set a wordlist with valid words(optional if set "
         "in config)\n"
         "-i or --ignore - set wordlist of words to be ignored (optional)\n"
         "-e or --extension - set extension to scan, like -e \".ts "
         ".js\"(optional)\n"
         "--min and --max - minimum and maximum length of word(optional)\n"
         "-x or --exclude - exclude specific directory, file or extension like "
         "-x path/ -x path/file.txt -x \".ts\"(optional)\n\n"
         "How to set up a config:\n"
         "Create a .spellconf.toml file in your home directory in such "
         "format\n"
         "[config]\n"
         "wordlist = \"/path/to/your/wordlist\"\n"
         "min = 3\n"
         "max = 10\n";
}

void ParamsHandler::handleArgsException(std::exception &e) {
  std::cerr << "Error: " << e.what() << "\n\n";
  showHelp();
  exit(1);
}

bool ParamsHandler::hasFileConfig() const {
  if (!FileHandler::checkIfPathExist(configPath))
    return false;

  try {
    std::ignore = toml::parse_file(configPath);
  } catch (std::exception &e) {
    std::cerr << e.what() << '\n';
    exit(1);
  }

  return true;
}

void ParamsHandler::getTomlConfig() {
  auto config = toml::parse_file(configPath);
  std::string wordlist = config["config"]["wordlist"].value_or("");
  int minLength = config["config"]["min"].value_or(-1);
  int maxLength = config["config"]["max"].value_or(-1);
  configFromFile = {wordlist, minLength, maxLength};
}

ParamsHandler::parsedExclude
ParamsHandler::parseExclude(excludeType &toExclude) {
  if (toExclude.size() <= 0)
    return {};

  parsedExclude parsedResult{};

  for (const auto &toParse : toExclude) {
    auto validExtension = [](char ch) { return isalpha(ch) || ch == '.'; };
    if (*toParse.begin() == '.' &&
        std::all_of(toParse.begin(), toParse.end(), validExtension)) {
      parsedResult.extensions.emplace_back(toParse);
      continue;
    }
    std::string absolutePathToCheck{std::filesystem::weakly_canonical(toParse)};

    FileHandler::pathCheck(absolutePathToCheck, &FileHandler::checkIfPathExist,
                           true, "this path doesn't exist!");
    parsedResult.paths.emplace_back(absolutePathToCheck);
  }
  return parsedResult;
}

ParamsHandler::checkerConfig &ParamsHandler::getConfig() {
  if (hasFileConfig())
    getTomlConfig();

  excludeType thingsToExclude{};
  desc.add_options()("help,h", "get help")(
      "ignore,i", po::value<std::string>()->default_value(""))(
      "extension,e", po::value<std::string>()->default_value(""))(
      "min", po::value<int>()->default_value(0))(
      "max", po::value<int>()->default_value(100))(
      "exclude,x", po::value<excludeType>(&thingsToExclude));

  if (pipedStdinExists)
    desc.add_options()("target,t", po::value<std::string>()->default_value(""));
  else
    desc.add_options()("target,t", po::value<std::string>()->required());

  if (configFromFile.wordlist.empty())
    desc.add_options()("wordlist,w", po::value<std::string>()->required());
  else
    desc.add_options()("wordlist,w",
                       po::value<std::string>()->default_value(""));

  try {
    po::variables_map vm{};
    po::store(po::parse_command_line(mArgc, mArgv, desc), vm);
    po::notify(vm);

    if (vm.count("help")) {
      showHelp();
      exit(0);
    }

    std::string wordlistPath{vm["wordlist"].as<std::string>()};

    if (!configFromFile.wordlist.empty() &&
        vm["wordlist"].as<std::string>().empty()) {
      wordlistPath = configFromFile.wordlist;
    }

    int minLength{configFromFile.min > -1 ? configFromFile.min
                                          : vm["min"].as<int>()};
    int maxLength{configFromFile.max > -1 ? configFromFile.max
                                          : vm["max"].as<int>()};

    result = {vm["target"].as<std::string>(),
              wordlistPath,
              vm["ignore"].as<std::string>(),
              vm["extension"].as<std::string>(),
              minLength,
              maxLength,
              parseExclude(thingsToExclude)};

    return result;
  } catch (std::exception &e) {
    handleArgsException(e);
    exit(1);
  }
}
