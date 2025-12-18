#pragma once
#include <QString>

class Playlist {
public:
    int id;
    int userId;
    QString name;

    Playlist() : id(-1), userId(-1) {}

    Playlist(int id, int uid, QString n)
        : id(id), userId(uid), name(n) {}
};
