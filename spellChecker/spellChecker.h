#ifndef SPELL_CHECKER_H
#define SPELL_CHECKER_H

#include "../paramsHandler/paramsHandler.h"
#include "../trie/trie.h"
#include <filesystem>
#include <string>
#include <vector>

namespace SpellCheckerN {
struct Keyword {
  std::string word{};
  int line{};
};
using keywordList = std::vector<SpellCheckerN::Keyword>;
} // namespace SpellCheckerN

class SpellChecker {
public:
  explicit SpellChecker(ParamsHandler::checkerConfig &config);

private:
  void checkAll(const std::filesystem::path &path);
  void checkAll();
  void parseAndCheck();
  void getAllWordsFromLine(const std::string &line);
  void validKeywords();
  void printAndCheck() const;
  SpellCheckerN::keywordList allKeywords{};
  const std::filesystem::path targetPath{};
  const std::filesystem::path wordlistPath{};
  const std::filesystem::path ignorePath{};
  const std::string allowedExtensions{};
  const int minWordLength{};
  const int maxWordLength{};
  int lineCounter{1};
  Trie wordlistTrie{};
  Trie ignoreListTrie{};
};

#endif
