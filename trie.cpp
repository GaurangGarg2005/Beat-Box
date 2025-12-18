#include "trie.h"
#include <algorithm>
#include <cctype>

Trie::Trie() {
    root = new TrieNode();
}

Trie::~Trie() {
    deleteNode(root);
}

void Trie::deleteNode(TrieNode *node) {
    if (!node)
        return;
    for (auto &child : node->next) {
        if (child)
            deleteNode(child);
    }
    delete node;
}

void Trie::insert(const std::string &key, int songId) {
    TrieNode *cur = root;
    for (char ch : key) {
        unsigned char c = static_cast<unsigned char>(std::tolower(ch));
        if (!cur->next[c])
            cur->next[c] = new TrieNode();
        cur = cur->next[c];
        if (std::find(cur->songIds.begin(), cur->songIds.end(), songId) == cur->songIds.end())
            cur->songIds.push_back(songId);
    }
    cur->end = true;
}

std::vector<int> Trie::searchPrefix(const std::string &prefix) {
    TrieNode *cur = root;
    for (char ch : prefix) {
        unsigned char c = static_cast<unsigned char>(std::tolower(ch));
        if (!cur->next[c])
            return {};
        cur = cur->next[c];
    }
    return cur->songIds;
}
