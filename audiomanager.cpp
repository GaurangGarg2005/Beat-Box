#include "audiomanager.h"
#include <QDebug>
#include <QUrl>
#include <QVariant>

AudioManager::AudioManager(DataStore *store, QObject *parent)
    : QObject(parent),
    m_store(store)
{
    m_player = new QMediaPlayer(this);
    m_audioOut = new QAudioOutput(this);
    m_player->setAudioOutput(m_audioOut);
    m_audioOut->setVolume(m_volume / 100.0);

    connect(m_player, &QMediaPlayer::playbackStateChanged,
            this, &AudioManager::handleStateChanged);
    connect(m_player, &QMediaPlayer::positionChanged,
            this, &AudioManager::handlePosition);
    connect(m_player, &QMediaPlayer::durationChanged,
            this, &AudioManager::handleDuration);

    // Simple end-of-track detection (tolerant)
    connect(m_player, &QMediaPlayer::positionChanged,
            this, [this](qint64 pos) {
                if (m_player->duration() > 0 &&
                    pos >= m_player->duration() - 800) {
                    handleMediaEnded();
                }
            });
}

AudioManager::~AudioManager() {
    // parent deletes m_player and m_audioOut
}

void AudioManager::ensurePlayerHasSourceFromCurrent() {
    if (!m_list.current) return;
    const QString &fp = m_list.current->data.filePath;
    if (!fp.isEmpty()) {
        // prefer local file URL if starts with file:// otherwise use as-is
        QUrl url = QUrl::fromUserInput(fp);
        m_player->setSource(url);
    }
}

void AudioManager::loadPlaylistByIds(const QVariantList &ids) {
    std::vector<Song> svec;
    for (const QVariant &v : ids) {
        int id = v.toInt();
        const Song* s = m_store->findSongById(id);
        if (s) svec.push_back(*s);
    }
    m_list.buildFromVector(svec);
    if (m_list.current) {
        ensurePlayerHasSourceFromCurrent();
        emit currentChanged();
    }
}

void AudioManager::loadSongsVector(const QVariantList &songs) {
    std::vector<Song> svec;
    for (const QVariant &v : songs) {
        QVariantMap m = v.toMap();
        Song ss;
        ss.id = m.value("id").toInt();
        ss.title = m.value("title").toString();
        ss.artist = m.value("artist").toString();
        ss.filePath = m.value("filePath").toString();
        ss.imagePath = m.value("imagePath").toString();
        ss.durationMs = m.value("duration").toInt();
        svec.push_back(ss);
    }
    m_list.buildFromVector(svec);
    if (m_list.current) {
        ensurePlayerHasSourceFromCurrent();
        emit currentChanged();
    }
}

void AudioManager::playSongById(int id) {
    const Song* s = m_store->findSongById(id);
    if (!s) return;
    m_list.setCurrentById(id);

    ensurePlayerHasSourceFromCurrent();
    m_player->play();
    emit currentChanged();
    emit playStateChanged();
}

void AudioManager::play() {
    if (m_player->source().isEmpty() && m_list.current) {
        ensurePlayerHasSourceFromCurrent();
    }
    m_player->play();
    emit playStateChanged();
}

void AudioManager::pause() {
    m_player->pause();
    emit playStateChanged();
}

void AudioManager::next() {
    SongNode *n = m_list.nextSong();
    if (n) {
        ensurePlayerHasSourceFromCurrent();
        m_player->play();
        emit currentChanged();
        emit playStateChanged();
    }
}

void AudioManager::previous() {
    SongNode *n = m_list.prevSong();
    if (n) {
        ensurePlayerHasSourceFromCurrent();
        m_player->play();
        emit currentChanged();
        emit playStateChanged();
    }
}

void AudioManager::seek(qint64 ms) {
    if (!m_player) return;

    // Do NOT reset source
    if (ms < 0) ms = 0;

    // Clamp to duration
    qint64 dur = m_player->duration();
    if (dur > 0 && ms > dur - 50)
        ms = dur - 50;

    m_player->setPosition(ms);
}


void AudioManager::setVolume(int v) {
    m_volume = v;
    if (m_audioOut) m_audioOut->setVolume(v / 100.0);
}

QVariantMap AudioManager::getCurrentSongInfo() const {
    QVariantMap out;
    if (m_list.current) {
        out["id"] = m_list.current->data.id;
        out["title"] = m_list.current->data.title;
        out["artist"] = m_list.current->data.artist;
        out["image"] = m_list.current->data.imagePath;
        out["filePath"] = m_list.current->data.filePath;
    }
    return out;
}

QString AudioManager::currentTitle() const {
    if (m_list.current) return m_list.current->data.title;
    return {};
}

qint64 AudioManager::position() const {
    return m_player ? m_player->position() : 0;
}

qint64 AudioManager::duration() const {
    return m_player ? m_player->duration() : 0;
}

bool AudioManager::playing() const {
    return m_player && m_player->playbackState() == QMediaPlayer::PlayingState;
}

void AudioManager::handleStateChanged(QMediaPlayer::PlaybackState) {
    emit playStateChanged();
}

void AudioManager::handlePosition(qint64) {
    emit positionChanged();
}

void AudioManager::handleDuration(qint64) {
    emit durationChanged();
}

void AudioManager::handleMediaEnded() {
    next();
}
