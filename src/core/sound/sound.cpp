#include "sound.h"

#include <Windows.h>

#include <QtCore/QFileInfo>
#include <QtCore/QDebug>
#include <QtMultimedia/QMediaPlayer>
#include <QtMultimedia/QAudioOutput>

#include <util/exception/exception.h>

Sound::Sound(const QString& id, const QString& path):
    Entity(id)
{
    m_audioFile.reset(new QFile(path));
    if(!m_audioFile->open(QFile::ReadOnly))
        xthrow Exception(Exception::tr("Could not open sound file '%1'").arg(path));
    // TODO: IoException above? First figure out what to do with exceptions in general. I18n, subclassing etc.

    m_player.reset(new QMediaPlayer());
    m_audioOutput.reset(new QAudioOutput());
    m_player->setAudioOutput(m_audioOutput.get());
    m_player->setSourceDevice(m_audioFile.get());
}

Sound::~Sound() {}

void Sound::play() {
    if (m_player->playbackState() != QMediaPlayer::StoppedState)
        return;

    m_player->play();
}

