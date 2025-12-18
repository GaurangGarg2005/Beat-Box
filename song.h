#pragma once
#include <QString>

class Song {
public:
    int id;
    QString title;
    QString artist;
    QString filePath;
    QString imagePath;
    int durationMs;

    Song() : id(-1), durationMs(0) {}

    Song(int id, QString t, QString art, QString fp, QString img, int d)
        : id(id), title(t), artist(art), filePath(fp), imagePath(img), durationMs(d) {}
};
