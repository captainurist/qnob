#include "platform_initializer.h"

#include <cassert>

#include "platform.h"

static Platform* g_platform = nullptr;

Platform* createPlatform(); /* Defined in a platform-specific source file. */

PlatformInitializer::PlatformInitializer() {
    assert(!g_platform);

    g_platform = createPlatform();
}

PlatformInitializer::~PlatformInitializer() {
    assert(g_platform);

    delete g_platform;
    g_platform = nullptr;
}

Platform* Platform::instance() {
    return g_platform;
}
