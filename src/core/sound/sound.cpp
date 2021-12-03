#include "sound.h"

#include <Windows.h>

#include <QtCore/QFileInfo>
#include <QtCore/QDebug>
#include <QtMultimedia/QMediaPlayer>
#include <QtMultimedia/QAudioOutput>

#include <util/exception/exception.h>
#include <util/format.h>

#include <core/entity/entity_config.h>
#include <core/setting/setting.h>

Sound::Sound(QObject* parent) :
    Entity(parent)
{}

Sound::~Sound() {}

void Sound::loadFromConfig(const EntityConfig& cfg) {
    QString path = cfg.require<QString, AsPath>(lit("path"));
    Setting* target = cfg.require<Setting*>(lit("target"));

    QObject::connect(target, &Setting::activated, this, &Sound::play);

    m_audioFile.reset(new QFile(path));
    if (!m_audioFile->open(QFile::ReadOnly))
        xthrow Exception(sformat(Exception::tr("Could not open sound file '{}'"), path));
    // TODO: IoException above? First figure out what to do with exceptions in general. I18n, subclassing etc.

    m_player.reset(new QMediaPlayer());
    m_audioOutput.reset(new QAudioOutput());
    m_player->setAudioOutput(m_audioOutput.get());
    m_player->setSourceDevice(m_audioFile.get());
}

void Sound::play() {
    if (!m_player || m_player->playbackState() != QMediaPlayer::StoppedState)
        return;

    m_player->play();
}

