#include "storebridge.h"

StoreBridge::StoreBridge(DataStore *store, QObject *parent)
    : QObject(parent), m_store(store) {}

QVariantList StoreBridge::getAllSongs() {
    QVariantList list;
    for (const Song &s : m_store->songs) {
        QVariantMap m;
        m["id"] = s.id;
        m["title"] = s.title;
        m["artist"] = s.artist;
        m["filePath"] = s.filePath;
        m["imagePath"] = s.imagePath;
        m["duration"] = s.durationMs;
        list.push_back(m);
    }
    return list;
}
QVariantMap StoreBridge::getUserInfo(int id) {
    return m_store->getUserInfo(id);
}

QVariantList StoreBridge::getUserPlaylists(int userId) {
    QVariantList list;

    for (const Playlist &p : m_store->playlists) {
        if (p.userId == userId) {
            QVariantMap m;
            m["id"] = p.id;
            m["name"] = p.name;
            list.push_back(m);
        }
    }

    return list;
}


QVariantMap StoreBridge::getSong(int id) {
    for (const Song &s : m_store->songs) {
        if (s.id == id) {
            QVariantMap m;
            m["id"] = s.id;
            m["title"] = s.title;
            m["artist"] = s.artist;
            m["filePath"] = s.filePath;
            m["imagePath"] = s.imagePath;
            m["duration"] = s.durationMs;
            return m;
        }
    }
    return QVariantMap();
}

QVariantList StoreBridge::searchSongs(const QString &text) {
    QVariantList list;
    if (text.isEmpty()) return list;

    std::vector<int> ids = m_store->songTrie.searchPrefix(text.toStdString());
    for (int id : ids) {
        const Song* s = m_store->findSongById(id);
        if (s) {
            QVariantMap m;
            m["id"] = s->id;
            m["title"] = s->title;
            m["artist"] = s->artist;
            m["filePath"] = s->filePath;
            m["imagePath"] = s->imagePath;
            m["duration"] = s->durationMs;
            list.push_back(m);
        }
    }
    return list;
}


QVariantList StoreBridge::getPlaylistSongs(int playlistId) {
    QVariantList list;
    std::vector<Song> songs = m_store->getSongsInPlaylist(playlistId);
    for (const Song &s : songs) {
        QVariantMap m;
        m["id"] = s.id;
        m["title"] = s.title;
        m["artist"] = s.artist;
        m["filePath"] = s.filePath;
        m["imagePath"] = s.imagePath;
        m["duration"] = s.durationMs;
        list.push_back(m);
    }
    return list;
}
