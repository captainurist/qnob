#include "sound.h"

#include <QtCore/QFile>
#include <QtCore/QDebug>

#include <Windows.h>

Sound::Sound(const QString& id, const QString& path):
    Entity(id)
{
    QFile file(path);
    bool isOpen = file.open(QIODevice::ReadOnly);

    // TODO: I think this one should be an exception.
    if (!isOpen)
        qWarning() << "Could not open sound file" << path;

    m_data = file.readAll();
}

#pragma comment(lib, "Winmm.lib") // TODO

void Sound::play() {
    // TODO: Qt6 doesn't have QSound yet, so I'm just throwing a winapi call here.
    // TODO: also this one quacks right now on repeated key events.
    PlaySoundA(m_data.data(), NULL, SND_MEMORY | SND_ASYNC | SND_NODEFAULT);
}

