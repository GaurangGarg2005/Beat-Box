#pragma once
#include <QString>

class User {
public:
    int id;
    QString name;
    QString email;
    int age;
    QString profilePic;

    User() : id(-1), age(0) {}

    User(int id, const QString& n, const QString& e, int a, const QString& p)
        : id(id), name(n), email(e), age(a), profilePic(p) {}
};
