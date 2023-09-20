#include "parsingConf.h"
#include <algorithm>
#include <string>

namespace ParsingConfig {
bool hasUnderscore(const std::string &str) {
  return str.find("_") != std::string::npos;
}

bool camelPascalCondition(const std::string &str) {
  if (str.empty())
    return false;

  bool lowercaseFound{false};
  bool uppercaseFound{false};
  bool wasPreviousUpper{false};

  for (const auto c : str) {
    if (islower(c)) {
      wasPreviousUpper = false;
      lowercaseFound = true;
    } else if (isupper(c)) {
      if (wasPreviousUpper) {
        return false;
      }
      wasPreviousUpper = true;
      uppercaseFound = true;
    } else if (!isdigit(c)) {
      return false;
    }
  }

  return lowercaseFound && uppercaseFound;
}

bool macroSnakeCondition(const std::string &str) {
  bool hasLower{false};
  bool hasUpper{false};

  for (char c : str) {
    if (islower(c))
      hasLower = {true};
    if (isupper(c))
      hasUpper = {true};

    if ((!isalpha(c) && c != '_') || (hasLower && hasUpper))
      return false;
  }
  return hasUnderscore(str);
}

keywordsType parseMacroSnakeCase(const std::string &str) {
  std::stringstream ss(str);
  std::string parsingBuffer{};
  keywordsType result{};

  while (std::getline(ss, parsingBuffer, '_')) {
    std::transform(parsingBuffer.begin(), parsingBuffer.end(),
                   parsingBuffer.begin(), asciiToLower);
    result.emplace_back(parsingBuffer);
  }
  return result;
}

keywordsType parseCamelPascalCase(const std::string &str) {
  keywordsType result{};
  std::string parsingBuffer{};

  for (char c : str) {
    if (std::isupper(c)) {
      if (!parsingBuffer.empty()) {
        std::transform(parsingBuffer.begin(), parsingBuffer.end(),
                       parsingBuffer.begin(), asciiToLower);
        result.emplace_back(parsingBuffer);
      }
      parsingBuffer.clear();
    }
    parsingBuffer += c;
  }

  if (!parsingBuffer.empty()) {
    std::transform(parsingBuffer.begin(), parsingBuffer.end(),
                   parsingBuffer.begin(), asciiToLower);
    result.emplace_back(parsingBuffer);
  }
  return result;
}
char asciiToLower(const char in) {
  if (in <= 'Z' && in >= 'A')
    return in - ('Z' - 'z');
  return in;
}

} // namespace ParsingConfig
