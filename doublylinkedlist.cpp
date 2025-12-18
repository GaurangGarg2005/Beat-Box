#include "doublylinkedlist.h"

DoublyLinkedList::~DoublyLinkedList() {
    clear();
}

void DoublyLinkedList::pushBack(const Song &s) {
    SongNode *node = new SongNode(s);
    if (!head) {
        head = tail = node;
    } else {
        tail->next = node;
        node->prev = tail;
        tail = node;
    }
}

void DoublyLinkedList::clear() {
    SongNode *cur = head;
    while (cur) {
        SongNode *n = cur->next;
        delete cur;
        cur = n;
    }
    head = tail = current = nullptr;
}

void DoublyLinkedList::buildFromVector(const std::vector<Song> &v) {
    clear();
    for (const auto &s : v)
        pushBack(s);
    setCurrentToFirst();
}

void DoublyLinkedList::setCurrentToFirst() {
    current = head;
}

SongNode *DoublyLinkedList::nextSong() {
    if (!current)
        return nullptr;
    if (current->next)
        current = current->next;
    else if (loop)
        current = head;
    else
        current = nullptr;
    return current;
}

SongNode *DoublyLinkedList::prevSong() {
    if (!current)
        return nullptr;
    if (current->prev)
        current = current->prev;
    else if (loop)
        current = tail;
    else
        current = nullptr;
    return current;
}
bool DoublyLinkedList::setCurrentById(int id) {
    SongNode* n = head;
    while (n) {
        if (n->data.id == id) {
            current = n;
            return true;
        }
        n = n->next;
    }
    return false;
}
