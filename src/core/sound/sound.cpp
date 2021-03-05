#include "sound.h"

#include <Windows.h>

#include <QtCore/QFile>
#include <QtCore/QDebug>

#include <util/exception.h>

Sound::Sound(const QString& id, const QString& path):
    Entity(id)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
        qthrow Exception(Exception::tr("Could not open sound file '%1'").arg(path)); // TODO: IoException?

    m_data = file.readAll();
}

#pragma comment(lib, "Winmm.lib") // TODO

void Sound::play() {
    // TODO: Qt6 doesn't have QSound yet, so I'm just throwing a winapi call here.
    // TODO: also this one quacks right now on repeated key events.
    PlaySoundA(m_data.data(), NULL, SND_MEMORY | SND_ASYNC | SND_NODEFAULT);
}

