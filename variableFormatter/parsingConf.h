#ifndef PARSING_CONFIG_H
#define PARSING_CONFIG_H

#include "./variableFormatter.h"
#include <algorithm>
#include <cctype>
#include <functional>
#include <string>
#include <vector>

namespace ParsingConfig {
using keywordsType = VariableFormatter::keywordsType;

struct Instruction {
  std::function<bool(const std::string &)> condition{};
  std::function<keywordsType(const std::string &)> parsingFunction{};
};

bool hasUnderscore(const std::string &str);
bool camelPascalCondition(const std::string &str);
bool macroSnakeCondition(const std::string &str);
keywordsType parseMacroSnakeCase(const std::string &str);
keywordsType parseCamelPascalCase(const std::string &str);
char asciiToLower(const char in);

const Instruction camelPascalInstr{camelPascalCondition, parseCamelPascalCase};
const Instruction macroSnakeInstr{macroSnakeCondition, parseMacroSnakeCase};
const std::vector<Instruction> allInstructions{camelPascalInstr,
                                               macroSnakeInstr};
}; // namespace ParsingConfig

#endif // !PARSING_CONFIG_H
