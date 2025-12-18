#pragma once

#include <string>
#include <vector>
#include "song.h"

struct TrieNode {
    bool end;
    std::vector<int> songIds;
    TrieNode *next[256];

    TrieNode() : end(false), songIds() {
        for (auto &p : next)
            p = nullptr;
    }
};

class Trie {
public:
    Trie();
    ~Trie();

    void insert(const std::string &key, int songId);
    std::vector<int> searchPrefix(const std::string &prefix);

private:
    TrieNode *root;

    void deleteNode(TrieNode *node);
};
