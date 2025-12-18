#include "datastore.h"
#include <algorithm>
#include <QVariantMap>

DataStore::DataStore() {
    preloadSampleData();
}

void DataStore::preloadSampleData() {

    // ---------- USERS ----------
    users.clear();
    users.push_back(User(
        1,
        "Game Changer",
        "gamechanger1234@gmail.com",
        22,
        "file:///D:/spotify_clone/abcd.jpg"
        ));

    // ---------- SONGS ----------
    songs.clear();

    // You ONLY have 1 audio file: trial.mp3
    // So all songs must temporarily use the same audio source

    songs.push_back(Song(
        1,
        "Bam Bholle",
        "Unknown Artist",
        "file:///D:/spotify_clone/trial.mp3",
        "file:///D:/spotify_clone/abcd.jpg",
        0
        ));

    songs.push_back(Song(
        2,
        "Jhoom Sharabi",
        "Artist B",
        "file:///D:/spotify_clone/jhoom.mp3",
        "file:///D:/spotify_clone/trial2.jpg",
        0
        ));

    songs.push_back(Song(
        3,
        "Monica",
        "Artist A",
        "file:///D:/spotify_clone/monica.mp3",
        "file:///D:/spotify_clone/abcd.jpg",
        0
        ));

    songs.push_back(Song(
        4,
        "Ghafoor",
        "Artist C",
        "file:///D:/spotify_clone/ghafoor.mp3",
        "file:///D:/spotify_clone/trial2.jpg",
        0
        ));
    songs.push_back(Song(
        5,
        "Phoogdi Dance",
        "Artist B",
        "file:///D:/spotify_clone/phoogdi.mp3",
        "file:///D:/spotify_clone/abcd.jpg",
        0
        ));

    // ---------- TRIE ----------
    for (const Song &s : songs)
        songTrie.insert(s.title.toStdString(), s.id);

    // ---------- PLAYLISTS ----------
    playlists.clear();
    playlists.push_back(Playlist(1, 1, "My Playlist"));

    // ---------- PLAYLIST SONGS ----------
    playlistSongs.clear();
    playlistSongs.push_back(PlaylistSong(1, 1, 0));
    playlistSongs.push_back(PlaylistSong(1, 2, 1));
    playlistSongs.push_back(PlaylistSong(1, 3, 2));
    playlistSongs.push_back(PlaylistSong(1, 4, 3));
    playlistSongs.push_back(PlaylistSong(1, 5, 4));
    playlistGraph.clear();
}

const Song* DataStore::findSongById(int id) const {
    for (const Song &s : songs)
        if (s.id == id) return &s;
    return nullptr;
}
QVariantMap DataStore::getUserInfo(int id)
{
    QVariantMap map;

    for (const User &u : users)
    {
        if                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   (u.id == id)
        {
            map["id"] = u.id;
            map["name"] = u.name;
            map["email"] = u.email;
            map["age"] = u.age;
            map["profilePic"] = u.profilePic;
            return map;
        }
    }

    return map;   // empty map if user not found
}
std::vector<Song> DataStore::getSongsInPlaylist(int playlistId) const {
    std::vector<Song> out;

    std::vector<PlaylistSong> rows;
    for (const PlaylistSong &ps : playlistSongs)
        if (ps.playlistId == playlistId)
            rows.push_back(ps);

    std::sort(rows.begin(), rows.end(),
              [](const PlaylistSong &a, const PlaylistSong &b) {
                  return a.position < b.position;
              });

    for (const PlaylistSong &r : rows) {
        const Song* s = findSongById(r.songId);
        if (s) out.push_back(*s);
    }

    return out;
}
