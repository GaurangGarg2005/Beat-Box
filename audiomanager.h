#pragma once

#include <QObject>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QVariantList>
#include <QVariantMap>
#include <vector>

#include "doublylinkedlist.h"
#include "song.h"
#include "datastore.h"

class AudioManager : public QObject {
    Q_OBJECT
public:
    explicit AudioManager(DataStore *store, QObject *parent = nullptr);
    ~AudioManager();

    Q_PROPERTY(QString currentTitle READ currentTitle NOTIFY currentChanged)

    // QML SLIDER EXPECTS THESE TWO:
    Q_PROPERTY(qint64 pos READ position NOTIFY positionChanged)
    Q_PROPERTY(qint64 dur READ duration NOTIFY durationChanged)

    // Keep original names also (safe)
    Q_PROPERTY(qint64 position READ position NOTIFY positionChanged)
    Q_PROPERTY(qint64 duration READ duration NOTIFY durationChanged)

    Q_PROPERTY(bool playing READ playing NOTIFY playStateChanged)

    Q_INVOKABLE void loadPlaylistByIds(const QVariantList &ids);
    Q_INVOKABLE void loadSongsVector(const QVariantList &songs);
    Q_INVOKABLE void play();
    Q_INVOKABLE void pause();
    Q_INVOKABLE void next();
    Q_INVOKABLE void previous();
    Q_INVOKABLE void seek(qint64 ms);
    Q_INVOKABLE void setVolume(int v);
    Q_INVOKABLE QVariantMap getCurrentSongInfo() const;
    Q_INVOKABLE void playSongById(int id);

    QString currentTitle() const;
    qint64 position() const;
    qint64 duration() const;
    bool playing() const;

signals:
    void currentChanged();
    void positionChanged();
    void durationChanged();
    void playStateChanged();

private slots:
    void handleStateChanged(QMediaPlayer::PlaybackState st);
    void handlePosition(qint64 pos);
    void handleDuration(qint64 dur);
    void handleMediaEnded();

private:
    QMediaPlayer *m_player = nullptr;
    QAudioOutput *m_audioOut = nullptr;
    DoublyLinkedList m_list;
    DataStore *m_store = nullptr;
    int m_volume = 80;
    void ensurePlayerHasSourceFromCurrent();
};
