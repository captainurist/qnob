#include "tr_functions.h"

#include <QtCore/QCoreApplication>

QString detail::translate(const char* context, const char* sourceText, const char* disambiguation, int n) {
    return QCoreApplication::translate(context, sourceText, disambiguation, n);
}
