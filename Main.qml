import QtQuick
import QtQuick.Controls
import QtMultimedia
import QtQuick.Layouts

Window {
    id: window
    width: 800
    height: 500
    visible: true
    color: "#121212"
    title: "Spotify Clone 🎵"

    // NOTE: C++ context objects expected:
    //  - DB (DatabaseManager) with Q_INVOKABLE init() and getAllSongs() returning a QVariantList
    //  - Audio (AudioManager) with Q_INVOKABLE playSong(id), play(), pause(), next(), previous(), seek(ms)
    //           and properties: pos (position in ms) and dur (duration in ms)
    //
    // Make sure in main.cpp you set:
    // engine.rootContext()->setContextProperty("DB", &db);
    // engine.rootContext()->setContextProperty("Audio", &audio);

    AudioOutput {
        id: audioOut
        volume: 0.8
    }

    // MediaPlayer is left in place to match your original structure.
    // Primary playback is handled by the C++ Audio manager; the MediaPlayer here is not used
    // for core playback but remains so the layout and existing ids remain unchanged.
    MediaPlayer {
        id: player
        // source intentionally left as-is (user may set a file manually) but we will use Audio.* functions
        // source: "file:///D:/spotify_clone/trial.mp3"
        audioOutput: audioOut
    }

    // Song storage used by QML lists.
    // We'll populate this list from DB on Component.onCompleted so your existing layout
    // and model ids remain exactly the same.
    ListModel {
        id: songDB

        ListElement { songId: 1; title: "Test Song"; filePath: "file:///D:/spotify_clone/trial.mp3"; imagePath: "file:///D:/spotify_clone/abcd.jpg" }
        ListElement { songId: 2; title: "Test Song"; filePath: "file:///D:/spotify_clone/trial.mp3"; imagePath: "" }
        ListElement { songId: 3; title: "Test Song"; filePath: "file:///D:/spotify_clone/trial.mp3"; imagePath: "" }
        ListElement { songId: 4; title: "Test Song"; filePath: "file:///D:/spotify_clone/trial.mp3"; imagePath: "" }
    }


    ListModel { id: filteredSongs }
    ListModel { id: upcomingSongs }

    function updateUpcomingList() {
        upcomingSongs.clear()

        if (songDB.count === 0)
            return

        var info = Audio.getCurrentSongInfo()
        if (!info || info.id === undefined)
            return

        var currentId = info.id
        var foundCurrent = false

        for (var i = 0; i < songDB.count; i++) {
            var s = songDB.get(i)

            if (s.songId === currentId) {
                foundCurrent = true
                continue
            }

            if (foundCurrent) {
                upcomingSongs.append({
                    "songId": s.songId,
                    "title": s.title,
                    "filePath": s.filePath,
                    "imagePath": s.imagePath
                })
            }
        }
    }

    RowLayout {
        anchors.fill: parent
        anchors.margins: 18
        spacing: 25

        // LEFT MUSIC PANEL
        Item {
            Layout.fillHeight: true
            Layout.preferredWidth: parent.width * 0.45

            ColumnLayout {
                anchors.fill: parent
                spacing: 14

                Image {
                    id: albumArt
                    // default album art uses the uploaded file
                    source: "file:///D:/spotify_clone/abcd.jpg"
                    Layout.preferredWidth: parent.width * 0.5
                    Layout.preferredHeight: parent.height * 0.5
                    fillMode: Image.PreserveAspectFit
                    Layout.alignment: Qt.AlignLeft
                }

                Text {
                    id: currentlyPlayingTitle
                    text: "No song playing"
                    color: "white"
                    font.pixelSize: 22
                    wrapMode: Text.WordWrap
                }

                Slider {
                    id: slider
                    Layout.preferredWidth: parent.width * 0.5
                    from: 0
                    to: (typeof Audio !== "undefined" && Audio.dur ? Audio.dur : 1)
                    value: (typeof Audio !== "undefined" && Audio.pos ? Audio.pos : 0)

                    property bool userIsDragging: false

                    // User starts dragging
                    onPressedChanged: {
                        if (pressed) {
                            userIsDragging = true
                        } else {
                            // when user releases = perform seek ONCE
                            userIsDragging = false
                            Audio.seek(Math.round(value))
                        }
                    }

                    // While dragging → DO NOT seek
                    onMoved: {
                        if (userIsDragging) {
                            // allow slider to move freely without forcing playback seek
                        }
                    }
                }


                // keep a timer to ensure UI updates even if binding is delayed
                Timer {
                    interval: 300
                    running: true
                    repeat: true
                    onTriggered: {
                        if (typeof Audio !== "undefined") {

                            // ⛔ DO NOT update slider while dragging
                            if (!slider.userIsDragging) {
                                slider.to = Audio.dur ? Audio.dur : 1
                                slider.value = Audio.pos ? Audio.pos : 0
                            }

                            var s = Math.floor((Audio.pos ? Audio.pos : 0) / 1000)
                            var d = Math.floor((Audio.dur ? Audio.dur : 0) / 1000)
                        }
                    }
                }


                RowLayout {
                    spacing: 20

                    Button { text: "⏮"; onClicked: { if (typeof Audio !== "undefined") Audio.previous() } }
                    Button {
                        id: playBtn
                        // We can't directly inspect C++ playing state safely here without a bound property,
                        // so toggle play/pause using a small JS state variable.
                        property bool isPlaying: false
                        text: isPlaying ? "⏸" : "▶️"
                        onClicked: {
                            if (typeof Audio === "undefined") return;
                            if (isPlaying) { Audio.pause(); isPlaying = false }
                            else { Audio.play(); isPlaying = true }
                        }
                    }
                    Button { text: "⏭"; onClicked: { if (typeof Audio !== "undefined") Audio.next() } }
                }

                Text {
                    text: "Upcoming Songs"
                    color: "lightgray"
                    font.pixelSize: 18
                    Layout.topMargin: 10
                }

                ScrollView {
                    id: scrollView
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    clip: true

                    Column {
                        width: scrollView.width   // FIX: keeps items inside ScrollView
                        spacing: 6

                        Repeater {
                            model: upcomingSongs
                            delegate: Rectangle {
                                width: parent.width - 20
                                height: 32
                                radius: 4
                                color: "#1d1d1d"

                                Text {
                                    text: title
                                    anchors.centerIn: parent
                                    color: "white"
                                    font.pixelSize: 14
                                }

                                MouseArea {
                                    anchors.fill: parent
                                    onClicked: {
                                        Audio.playSongById(songId)
                                        currentlyPlayingTitle.text = title
                                        if (imagePath && imagePath !== "")
                                            albumArt.source = imagePath
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        // RIGHT SIDE WITH SEARCH + USER PANEL
        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true

            ColumnLayout {
                anchors.fill: parent
                spacing: 15

                // 🔍 SEARCH BAR
                TextField {
                    id: searchField
                    placeholderText: "Search songs..."
                    Layout.fillWidth: true

                    onTextChanged: {
                        filteredSongs.clear()

                        var needle = text.trim().toLowerCase()

                        if (needle.length === 0) {
                            return   // empty OR whitespace → hide results
                        }


                        var results = []
                            try {
                                results = Store.searchSongs(text)
                            } catch (e) {
                                console.log("Store.searchSongs() threw:", e)
                            }
                            console.log("search for:", text, " -> results length:", results.length)
                            for (var i = 0; i < results.length; ++i) {
                                console.log("  result", i, "id:", results[i].id, "title:", results[i].title)
                                filteredSongs.append({
                                    "songId": results[i].id !== undefined ? results[i].id : -1,
                                    "title": results[i].title !== undefined ? results[i].title : "",
                                    "filePath": results[i].filePath !== undefined ? results[i].filePath : "",
                                    "imagePath": results[i].imagePath !== undefined ? results[i].imagePath : "",
                                    "artist": results[i].artist !== undefined ? results[i].artist : ""
                                })
                                console.log("---- ROLE DEBUG ----")
                                if (filteredSongs.count > 0) {
                                    var item = filteredSongs.get(0)
                                    for (var key in item) {
                                        console.log("role:", key, "value:", item[key])
                                    }
                                }
                                console.log("---------------------")

                            }
                    }
                }



                // SEARCH RESULTS LIST
                ScrollView {
                    id: searchScrollView
                    Layout.fillWidth: true
                    Layout.preferredHeight: 120
                    visible: filteredSongs.count > 0
                    clip: true

                    Column {
                        width: searchScrollView.width
                        spacing: 6

                        Repeater {
                            model: filteredSongs
                            delegate: Rectangle {
                                width: parent.width
                                height: 40
                                radius: 6
                                color: "#1d1d1d"

                                property int songId: model.songId
                                property string title: model.title
                                property string filePath: model.filePath
                                property string imagePath: model.imagePath
                                property string artist: model.artist

                                Text {
                                    text: title
                                    anchors.verticalCenter: parent.verticalCenter
                                    anchors.left: parent.left
                                    anchors.leftMargin: 10
                                    color: "white"
                                }

                                MouseArea {
                                    anchors.fill: parent
                                    onClicked: {
                                        console.log("Clicked search result:", songId)

                                        Audio.playSongById(songId)
                                        currentlyPlayingTitle.text = title

                                        if (imagePath && imagePath !== "")
                                            albumArt.source = imagePath
                                        else
                                            albumArt.source = "file:///D:/spotify_clone/abcd.jpg"

                                        // 100% FIX — USE ROOT
                                        filteredSongs.clear()
                                        searchField.text = ""
                                    }
                                }
                            }
                        }
                    }
                }


                // 👤 USER PANEL
                Rectangle {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    radius: 6
                    color: "#0d0d0d"

                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: 12
                        spacing: 8


                        Text {
                            text: "User Panel"
                            color: "white"
                            font.pixelSize: 20
                            font.bold: true
                        }

                        Image {
                            id: userPic
                            Layout.preferredWidth: 560
                            Layout.preferredHeight: 360
                            fillMode: Image.PreserveAspectFit
                            source: ""
                            clip: true
                        }



                        Text {
                            id: userName
                            text: ""
                            color: "white"
                            font.pixelSize: 16
                        }
                        Text {
                            id: userEmail
                            text: ""
                            color: "lightgray"
                            font.pixelSize: 14
                        }
                        Text {
                            id: userAge
                            text: ""
                            color: "lightgray"
                            font.pixelSize: 14
                        }


                        Text {
                            text: "Playlists:"
                            color: "white"
                            font.pixelSize: 18
                            Layout.topMargin: 10
                        }
                        // PLAYLIST LIST
                        ScrollView {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            clip: true

                            Column {
                                width: parent.width
                                spacing: 6
                                Rectangle {
                                        width: parent.width - 20
                                        height: 32
                                        radius: 4
                                        color: "#1d1d1d"
                                        Text {
                                            text: "My Playlist"
                                            anchors.left: parent.left
                                            anchors.leftMargin: 10
                                            anchors.verticalCenter: parent.verticalCenter
                                            color: "white"
                                            font.pixelSize: 14
                                        }
                                    }
                            }
                        }
                    }
                }
            }
        }
    }
    Connections {
        target: Audio
        function onCurrentChanged() {
            var info = Audio.getCurrentSongInfo()

            // ⭐ UPDATE TITLE + ALBUM ART
            if (info && info.title !== undefined) {
                currentlyPlayingTitle.text = info.title

                if (info.image !== undefined && info.image !== "")
                    albumArt.source = info.image
                else
                    albumArt.source = "file:///D:/spotify_clone/abcd.jpg"
            }

            // ⭐ UPDATE UPCOMING LIST (looped)
            upcomingSongs.clear()

            if (songDB.count === 0) return

            var currentId = info.id
            var currentIndex = -1

            for (var i = 0; i < songDB.count; i++) {
                if (songDB.get(i).songId === currentId) {
                    currentIndex = i
                    break
                }
            }
            if (currentIndex === -1) return

            for (var step = 1; step < songDB.count; step++) {
                var nextIndex = (currentIndex + step) % songDB.count
                var s = songDB.get(nextIndex)

                upcomingSongs.append({
                    "title": s.title,
                    "songId": s.songId,
                    "filePath": s.filePath,
                    "imagePath": s.imagePath,
                    "artist": s.artist
                })
            }
        }
    }



    // Populate the songDB at startup from the DB C++ manager without changing layout.
    Component.onCompleted: {
        // Using the new in-memory StoreBridge instead of database
        if (typeof Store !== "undefined" && Store !== null) {

            var all = []
            try {
                all = Store.getAllSongs()
            } catch (e) {
                console.log("Store.getAllSongs() failed:", e)
            }

            if (all && all.length > 0) {
                songDB.clear()
                // clear QML list and reload all songs
                for (var i = 0; i < all.length; ++i) {
                    songDB.append({
                        "songId": all[i].id !== undefined ? all[i].id : -1,
                        "title": all[i].title !== undefined ? all[i].title : "Unknown",
                        "filePath": all[i].filePath !== undefined ? all[i].filePath : "",
                        "imagePath": all[i].imagePath !== undefined ? all[i].imagePath : "",
                        "artist": all[i].artist !== undefined ? all[i].artist : ""
                    })
                }

                // ⭐ Load default song info into UI (first song in playlist)
                var info = Audio.getCurrentSongInfo()
                if (info && info.title !== undefined) {
                    currentlyPlayingTitle.text = info.title

                    if (info.image !== undefined && info.image !== "")
                        albumArt.source = info.image
                }

            } else {
                filteredSongs.clear()
            }

        } else {
            console.log("Store context property not found. Using default QML songDB.")
        }
        var u = Store.getUserInfo(1)

            if (u && u.name !== undefined) {
                userName.text = u.name
                userEmail.text = u.email
                userAge.text = "Age: " + u.age
                userPic.source = u.profilePic   // ⭐ load user image
            }
        updateUpcomingList()
    }
}
