#pragma once

#include <wtypes.h>

#include <compare>

#include <QtCore/QMetaType>

struct WinDdcTriplet {
    DWORD min = 0;
    DWORD current = 0;
    DWORD max = 0;

    friend std::strong_ordering operator<=>(const WinDdcTriplet& l, const WinDdcTriplet& r) = default;
};
Q_DECLARE_METATYPE(WinDdcTriplet)

