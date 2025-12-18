#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "datastore.h"
#include "storebridge.h"
#include "audiomanager.h"

int main(int argc, char **argv) {
    QGuiApplication app(argc, argv);

    DataStore store;          // in-memory data source (preloaded)
    StoreBridge bridge(&store);
    AudioManager audio(&store);
    // load default playlist at startup
    QVariantList pList;
    auto vec = store.getSongsInPlaylist(1);
    for (const Song &s : vec)
        pList << s.id;
    audio.loadPlaylistByIds(pList);

    QQmlApplicationEngine engine;
    // expose multiple names so your QML will find whichever name it expects
    engine.rootContext()->setContextProperty("Store", &bridge);
    engine.rootContext()->setContextProperty("DB", &bridge);
    engine.rootContext()->setContextProperty("StoreBridge", &bridge);
    engine.rootContext()->setContextProperty("Audio", &audio);
    engine.rootContext()->setContextProperty("AudioManager", &audio);

    // load QML (assuming your QML module is spotify_clone/Main)
    engine.loadFromModule("spotify_clone", "Main");
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
