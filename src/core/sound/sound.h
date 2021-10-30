#pragma once

#include <memory>

#include <core/entity/entity.h>

class QIODevice;
class QMediaPlayer;
class QAudioOutput;

class Sound : public Entity {
    Q_OBJECT
public:
    Sound(const QString& id, const QString& path);
    virtual ~Sound();

    Q_SLOT void play();

private:
    QByteArray m_data;
    std::unique_ptr<QIODevice> m_audioFile;
    std::unique_ptr<QMediaPlayer> m_player;
    std::unique_ptr<QAudioOutput> m_audioOutput;
};
