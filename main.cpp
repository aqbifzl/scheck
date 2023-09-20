#include "./spellChecker/spellChecker.h"
#include "paramsHandler/paramsHandler.h"

int main(int argc, char *argv[]) {
  ParamsHandler paramsHandler{argc, argv};
  const SpellChecker checker{paramsHandler.getConfig()};
}
