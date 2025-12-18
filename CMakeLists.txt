cmake_minimum_required(VERSION 3.16)

project(spotify_clone VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Quick Multimedia)


qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(appspotify_clone
    main.cpp
    # models and core
    song.h song.cpp
    user.h user.cpp
    playlist.h playlist.cpp
    playlistsong.h playlistsong.cpp
    playlistgraph.h playlistgraph.cpp
    doublylinkedlist.h doublylinkedlist.cpp
    trie.h trie.cpp
    # DataStore + bridge + audio
    datastore.h datastore.cpp
    storebridge.h storebridge.cpp
    audiomanager.h audiomanager.cpp
)

qt_add_qml_module(appspotify_clone
    URI spotify_clone
    VERSION 1.0
    QML_FILES
        Main.qml
)

target_link_libraries(appspotify_clone
    PRIVATE
        Qt6::Quick
        Qt6::Multimedia
)

set_target_properties(appspotify_clone PROPERTIES
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS appspotify_clone
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
