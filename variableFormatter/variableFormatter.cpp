#include "variableFormatter.h"
#include "../spellChecker/spellChecker.h"
#include "parsingConf.h"
#include <algorithm>
#include <cctype>
#include <string>

void VariableFormatter::parseVariable(
    SpellCheckerN::keywordList &allKeywords,
    SpellCheckerN::keywordList &parsedKeywords) {
  for (auto it = allKeywords.begin(); it != allKeywords.end();) {
    const std::string currentWord{(*it).word};
    bool wasFound{false};

    for (auto &instr : ParsingConfig::allInstructions) {
      if (instr.condition(currentWord)) {
        it = allKeywords.erase(it);
        for (std::string &newValue : instr.parsingFunction(currentWord)) {
          parsedKeywords.push_back({newValue, it->line});
        }
        wasFound = true;
        break;
      }
    }
    if (!wasFound) {
      std::transform(it->word.begin(), it->word.end(), it->word.begin(),
                     ParsingConfig::asciiToLower);
      ++it;
    }
  }
}
