#pragma once

#include <vector>
#include "song.h"

struct SongNode {
    Song data;
    SongNode *prev = nullptr;
    SongNode *next = nullptr;

    explicit SongNode(const Song &s) : data(s) {}
};

class DoublyLinkedList {
public:
    SongNode *head = nullptr;
    SongNode *tail = nullptr;
    SongNode *current = nullptr;
    bool loop = true;

    ~DoublyLinkedList();

    void pushBack(const Song &s);
    void clear();
    void buildFromVector(const std::vector<Song> &v);
    SongNode *nextSong();
    SongNode *prevSong();
    void setCurrentToFirst();
    bool setCurrentById(int id);

};
