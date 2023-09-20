#include "spellChecker.h"
#include "../fileHandler/fileHandler.h"
#include "../variableFormatter/variableFormatter.h"
#include <algorithm>
#include <bits/fs_fwd.h>
#include <cctype>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <sys/types.h>
#include <toml++/impl/path.h>
#include <unistd.h>
#include <unordered_set>
#include <vector>

namespace fs = std::filesystem;

bool doesSubstringExist(const std::string &str,
                        const std::string &searchedWord) {
  auto index = str.find(searchedWord);
  if (index == std::string::npos)
    return false;

  auto notPartOfWord = [&str](int wIndex) {
    if (wIndex < 0 || wIndex >= static_cast<int>(str.size()))
      return true;
    if (std::isspace(str[wIndex]) || std::ispunct(str[wIndex]))
      return true;
    return false;
  };
  return notPartOfWord(index - 1) && notPartOfWord(index + searchedWord.size());
}

void SpellChecker::parseAndCheck() {
  SpellCheckerN::keywordList parsedKeywordsToAppend{};
  VariableFormatter::parseVariable(allKeywords, parsedKeywordsToAppend);
  allKeywords.insert(allKeywords.end(), parsedKeywordsToAppend.begin(),
                     parsedKeywordsToAppend.end());
  printAndCheck();
  allKeywords.clear();
}

void SpellChecker::checkAll() {
  std::string line{};

  while (std::getline(std::cin, line)) {
    getAllWordsFromLine(line);
    ++lineCounter;
  }
  parseAndCheck();
}

void SpellChecker::checkAll(const std::filesystem::path &path) {
  FileHandler handler(path);
  std::string line{};
  while (std::getline(handler.fileToHandle, line)) {
    getAllWordsFromLine(line);
    ++lineCounter;
  }
  parseAndCheck();
}

void SpellChecker::printAndCheck() const {
  for (const auto &keyword : allKeywords) {
    int keywordLength{static_cast<int>(keyword.word.length())};

    if (minWordLength <= keywordLength && maxWordLength >= keywordLength &&
        !wordlistTrie.search(keyword.word) &&
        !ignoreListTrie.search(keyword.word)) {
      std::cout << keyword.word << " on the line "
                << std::to_string(keyword.line) << '\n';
    }
  }
}

std::vector<fs::path> getDesiredFiles(const fs::path &p,
                                      const std::string &desiredExtensions,
                                      ParamsHandler::parsedExclude &toExclude) {
  std::vector<fs::path> paths{};
  const std::unordered_set<std::string> excludeSet(toExclude.extensions.begin(),
                                                   toExclude.extensions.end());
  const auto excludePath{toExclude.paths};
  for (const auto &entry : fs::recursive_directory_iterator(
           p, std::filesystem::directory_options::skip_permission_denied)) {
    std::filesystem::path entryCanonical{
        std::filesystem::weakly_canonical(entry)};
    std::string entryExtension{entry.path().extension()};

    bool isExtensionOk{!desiredExtensions.length() ||
                       doesSubstringExist(desiredExtensions, entryExtension)};

    if (!toExclude.extensions.empty() && excludeSet.count(entryExtension) > 0) {
      continue;
    }

    auto isInDir = [&entryCanonical](auto &dir) {
      return (entryCanonical.string().find(dir) == 0);
    };

    if (std::find_if(excludePath.begin(), excludePath.end(), isInDir) !=
        excludePath.end())
      continue;

    if (fs::is_regular_file(entryCanonical) && isExtensionOk &&
        entryExtension.length()) {
      paths.emplace_back(entryCanonical);
    }
  }
  return paths;
}

SpellChecker::SpellChecker(ParamsHandler::checkerConfig &config)
    : targetPath(config.target), wordlistPath(config.wordlist),
      ignorePath(config.ignore), allowedExtensions(config.extension),
      minWordLength(config.min), maxWordLength(config.max) {
  bool pipedStdinExists{!isatty(STDIN_FILENO)};

  if (!pipedStdinExists)
    FileHandler::pathCheck(targetPath, &FileHandler::checkIfPathExist, true,
                           "this path doesn't exist!");

  FileHandler::pathCheck(wordlistPath, &FileHandler::checkIfPathExist, true,
                         "this path doesn't exist!");
  FileHandler::pathCheck(wordlistPath, &FileHandler::checkIfPathIsDir, false,
                         "wordlist must be a file!");

  if (config.ignore.length()) {
    FileHandler::pathCheck(ignorePath, &FileHandler::checkIfPathExist, true,
                           "ignore list doesn't exist!");
    FileHandler::pathCheck(ignorePath, &FileHandler::checkIfPathIsDir, false,
                           "ignore list must be a file!");
  }

  wordlistTrie.fillTrieWithFileList(wordlistPath);
  if (config.ignore.length()) {
    ignoreListTrie.fillTrieWithFileList(ignorePath);
  }

  if (pipedStdinExists) {
    checkAll();
  } else if (FileHandler::checkIfPathIsDir(targetPath)) {
    for (const auto &p :
         getDesiredFiles(targetPath, allowedExtensions, config.exclude)) {
      std::cout << p.string() << '\n';
      lineCounter = {1};
      checkAll(p);
    }
  } else {
    checkAll(targetPath);
  }
}

bool isValidChar(char c) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
         (c >= '0' && c <= '9') || (c == '_');
}

void SpellChecker::getAllWordsFromLine(const std::string &line) {
  std::string currentWord;
  bool inWord = false;
  bool hasLetter{false};
  for (char c : line) {
    if (isValidChar(c)) {
      currentWord += c;
      inWord = true;
      if (isalpha(c))
        hasLetter = true;
    } else if (inWord) {
      if (hasLetter)
        allKeywords.push_back({currentWord, lineCounter});
      currentWord.clear();
      inWord = false;
      hasLetter = false;
    }
  }

  if (inWord) {
    allKeywords.push_back({currentWord, lineCounter});
  }
}
