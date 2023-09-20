#ifndef VARIABLE_FORMATTER_H
#define VARIABLE_FORMATTER_H

#include "../spellChecker/spellChecker.h"
#include <string>
#include <vector>

class VariableFormatter {
public:
  using keywordsType = std::vector<std::string>;
  static void parseVariable(SpellCheckerN::keywordList &allKeywords,
                            SpellCheckerN::keywordList &parsedKeywords);
};
#endif // !VARIABLE_FORMATTER_H
