#pragma once

#include <vector>
#include <QString>
#include <QVariantMap>
#include "song.h"
#include "user.h"
#include "playlist.h"
#include "playlistsong.h"
#include "playlistgraph.h"
#include "trie.h"

class DataStore {
public:
    std::vector<User> users;
    std::vector<Song> songs;
    std::vector<Playlist> playlists;
    std::vector<PlaylistSong> playlistSongs;
    std::vector<PlaylistGraph> playlistGraph;

    Trie songTrie;

    DataStore();
    void preloadSampleData();
    Q_INVOKABLE QVariantMap getUserInfo(int id);

    // helpers
    const Song* findSongById(int id) const;
    std::vector<Song> getSongsInPlaylist(int playlistId) const;

};
