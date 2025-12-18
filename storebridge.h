#pragma once
#include <QObject>
#include <QVariantList>
#include <QVariantMap>
#include "datastore.h"

class StoreBridge : public QObject {
    Q_OBJECT
public:
    explicit StoreBridge(DataStore *store, QObject *parent = nullptr);

    Q_INVOKABLE QVariantList getAllSongs();
    Q_INVOKABLE QVariantMap getSong(int id);
    Q_INVOKABLE QVariantList searchSongs(const QString &text);
    Q_INVOKABLE QVariantList getPlaylistSongs(int playlistId);
    Q_INVOKABLE QVariantMap getUserInfo(int id);
    Q_INVOKABLE QVariantList getUserPlaylists(int userId);


private:
    DataStore *m_store;
};
