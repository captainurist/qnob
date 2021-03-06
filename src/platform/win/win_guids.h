#pragma once

#include <guiddef.h>

constexpr GUID GUID_TrayIconClock = { 0x7820ae72, 0x23e3, 0x4229, { 0x82, 0xc1, 0xe4, 0x1c, 0xb6, 0x7d, 0x5b, 0x9c } };
constexpr GUID GUID_TrayIconVolume = { 0x7820ae73, 0x23e3, 0x4229, { 0x82, 0xc1, 0xe4, 0x1c, 0xb6, 0x7d, 0x5b, 0x9c } };
constexpr GUID GUID_TrayIconNetwork = { 0x7820ae74, 0x23e3, 0x4229, { 0x82, 0xc1, 0xe4, 0x1c, 0xb6, 0x7d, 0x5b, 0x9c } };
constexpr GUID GUID_TrayIconPower = { 0x7820ae75, 0x23e3, 0x4229, { 0x82, 0xc1, 0xe4, 0x1c, 0xb6, 0x7d, 0x5b, 0x9c } };
constexpr GUID GUID_TrayIconActionCenter = { 0x7820ae76, 0x23e3, 0x4229, { 0x82, 0xc1, 0xe4, 0x1c, 0xb6, 0x7d, 0x5b, 0x9c } };
constexpr GUID GUID_TrayIconInputFlag = { 0xa59b00b9, 0xf6cd, 0x4fed, { 0xa1, 0xdc, 0x0f, 0x40, 0x64, 0xa1, 0x28, 0x31 } };

#if 0
/**
 * Helper function that was used to generate initializers above from GUID strings.
 */
QString uuidToGuidInitializer(const QString& string) {
    QUuid uuid(string);
    QChar zero = QLatin1Char('0');

    return lit("{0x%1, 0x%2, 0x%3, {0x%4, 0x%5, 0x%6, 0x%7, 0x%8, 0x%9, 0x%10, 0x%11}}")
        .arg(uuid.data1, 8, 16, zero)
        .arg(uuid.data2, 4, 16, zero)
        .arg(uuid.data3, 4, 16, zero)
        .arg(uuid.data4[0], 2, 16, zero)
        .arg(uuid.data4[1], 2, 16, zero)
        .arg(uuid.data4[2], 2, 16, zero)
        .arg(uuid.data4[3], 2, 16, zero)
        .arg(uuid.data4[4], 2, 16, zero)
        .arg(uuid.data4[5], 2, 16, zero)
        .arg(uuid.data4[6], 2, 16, zero)
        .arg(uuid.data4[7], 2, 16, zero);
}
#endif
