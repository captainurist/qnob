#include "sound.h"

#include <Windows.h>

Sound::Sound(const QString& id, const QString& path):
    Entity(id)
{
    m_path = path;
}

#pragma comment(lib, "Winmm.lib") // TODO

void Sound::play() {
    // TODO: Qt6 doesn't have QSound yet, so I'm just throwing a winapi call here.
    // TODO: also this one quacks right now on repeated key events.
    PlaySoundW(reinterpret_cast<LPCWSTR>(m_path.utf16()), NULL, SND_FILENAME | SND_ASYNC);
}

