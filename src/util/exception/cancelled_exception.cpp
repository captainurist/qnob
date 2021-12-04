#include "cancelled_exception.h"

CancelledException::CancelledException():
    Exception(tr("Operation was canceled."))
{}

CancelledException::CancelledException(const QString& message):
    Exception(message)
{}
