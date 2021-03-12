#include "com.h"

#include <combaseapi.h>

#include "win_error.h"

Com::Com() {
    initialized_ = apicall(CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE));
}

Com::~Com() {
    if (initialized_) {
        CoUninitialize();
        initialized_ = false;
    }
}

