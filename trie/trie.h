#ifndef TRIE_H
#define TRIE_H

#include <filesystem>
#include <string>
#include <unordered_map>

class TrieNode {
public:
  std::unordered_map<char, TrieNode *> children;
  bool isEndOfTheWord{};
};

class Trie {
public:
  Trie() : root(new TrieNode{}){};
  ~Trie();
  void insert(const std::string &word);
  bool search(const std::string &word) const;
  void fillTrieWithFileList(const std::filesystem::path &p);
  void deleteNodesBottomUp(TrieNode *node);

private:
  TrieNode *root;
};

#endif // !TRIE_H
