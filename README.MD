# scheck

🚀 scheck - C++ Spell Checker 🚀

scheck is a highly customizable C++ spell checker designed to empower you to catch and correct spelling errors in any document or codebase. scheck has 2 built-in functions to parse keywords in PascalCase, camelCase, snake_case and MACRO_CASE

Key Features:

✨ Universal Compatibility: Our spell checker is designed to work seamlessly with all types of files like text documents and source code.

🌐 Custom Wordlists: Use any wordlist you prefer, and the spell checker will adapt. No need to rely on proprietary or limited dictionaries.

⚙️ Highly Configurable: Follow the usage to set behavior matching your preferences. 

🚀 Fast and Efficient: Perform spell checks quickly and efficiently, even on large codebases or documents.

## Installation and build
### Install dependencies
#### Arch-based distro
```
sudo pacman -S extra/tomlplusplus extra/boost
```
#### Debian-based distro
```
sudo apt install tomlplusplus libboost-program-options-dev
```

### Clone and build
```
git clone https://github.com/aqbifzl/scheck && mkdir scheck/build && cd scheck/build && cmake .. -DCMAKE_BUILD_TYPE=Release && make
```

## Usage
```
scheck -t [target]
-t or --target - set a target file or directory
-w or --wordlist - set a wordlist with valid words(optional if set in config)
-i or --ignore - set wordlist of words to be ignored (optional)
-e or --extension - set extension to scan, like -e ".ts .js"(optional)
--min and --max - minimum and maximum length of word(optional)
-x or --exclude - exclude specific directory, file or extension like -x path/ -x path/file.txt -x ".ts"(optional)

How to set up a config:
Create a .spellconf.toml file in your home directory in such format
[config]
wordlist = "/path/to/your/wordlist"
min = 3
max = 10
```
## Example
```
scheck -t ~/Documents/myproject/ -w ./wordlist.txt -i ~/Documents/myproject/toIgnore.txt -e ".js .tsx" --min 3 -x ~/Documents/myproject/node_modules/
```

## Add custom naming convention

Adding your naming convention is very easy. Add it to variableFormatter/parsingConf.h defining a function that checks if the keyword is of this convention and a function that parses it returning a vector of strings. Use struct Instruction for your convention and add it to allInstructions. You may probably also want to change the getAllWordsFromLine function from spellChecker/spellChecker.cpp which retrieves the correct words


> **Warning**
> Wordlist can contains only lowercase words without special characters!
