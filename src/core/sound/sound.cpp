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
    // Also note that this call actually BLOCKS while playing the sound. Doh!
    PlaySoundW(reinterpret_cast<LPCWSTR>(m_path.utf16()), NULL, SND_FILENAME);
}

