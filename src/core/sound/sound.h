#pragma once

#include <memory>

#include <core/entity/entity.h>

class QIODevice;
class QMediaPlayer;
class QAudioOutput;

class Sound : public Entity {
    Q_OBJECT
public:
    Sound(QObject* parent);
    virtual ~Sound();

    Q_SLOT void play();

protected:
    virtual void loadFromConfig(const EntityCreationContext& ctx) override;

private:
    QByteArray m_data;
    std::unique_ptr<QIODevice> m_audioFile;
    std::unique_ptr<QMediaPlayer> m_player;
    std::unique_ptr<QAudioOutput> m_audioOutput;
};
