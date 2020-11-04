#include "com.h"

#include <combaseapi.h>

Com::Com() {
    initialized_ = SUCCEEDED(CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE));
}

Com::~Com() {
    if (initialized_) {
        CoUninitialize();
        initialized_ = false;
    }
}

