#include "com_exception.h"

#include <QtCore/QString>

ComException::ComException(HRESULT result, const char* context) {
    if (result == S_OK) {
        message_ = "Unknown COM exception";
    } else if (context == nullptr) {
        message_ = QStringLiteral("COM exception 0x%1").arg(result, 8, 16, QChar('0')).toLatin1();
    } else {
        message_ = QStringLiteral("COM exception 0x%1 in %2").arg(result, 8, 16, QChar('0')).arg(QLatin1String(context)).toLatin1();
    }
}
