#include "trie.h"
#include "../fileHandler/fileHandler.h"
#include <fstream>
#include <unistd.h>

void Trie::insert(const std::string &word) {
  TrieNode *node{root};
  for (const auto &c : word) {
    if (!node->children.count(c)) {
      node->children[c] = new TrieNode;
    }
    node = node->children[c];
  }
  node->isEndOfTheWord = {true};
}

bool Trie::search(const std::string &word) const {
  TrieNode *node{root};
  for (const auto &c : word) {
    if (!node->children.count(c))
      return false;
    node = node->children[c];
  }
  return node->isEndOfTheWord;
}

void Trie::fillTrieWithFileList(const std::filesystem::path &p) {
  FileHandler fileToFill(p);
  std::string str{};
  while (std::getline(fileToFill.fileToHandle, str)) {
    insert(str);
  }
}

Trie::~Trie() {
  if (root) {
    deleteNodesBottomUp(root);
    root = nullptr;
  }
}

void Trie::deleteNodesBottomUp(TrieNode *node) {
  if (!node)
    return;
  for (auto &child : node->children) {
    deleteNodesBottomUp(child.second);
  }
  delete node;
}
