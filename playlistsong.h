#pragma once

class PlaylistSong {
public:
    int playlistId;
    int songId;
    int position;

    PlaylistSong() : playlistId(-1), songId(-1), position(0) {}

    PlaylistSong(int pid, int sid, int pos)
        : playlistId(pid), songId(sid), position(pos) {}
};
