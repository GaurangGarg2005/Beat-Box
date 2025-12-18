#pragma once

class PlaylistGraph {
public:
    int id;
    int sourcePlaylist;
    int copiedPlaylist;

    PlaylistGraph() : id(-1), sourcePlaylist(-1), copiedPlaylist(-1) {}

    PlaylistGraph(int id, int src, int cp)
        : id(id), sourcePlaylist(src), copiedPlaylist(cp) {}
};
