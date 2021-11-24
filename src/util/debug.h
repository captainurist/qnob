#pragma once

#include <source_location>
#include <string_view>

#include <QtCore/QDebug>

#include "format.h"

// TODO: rename log.h / logging.h

template<class WideStringView, class... Args>
void xMessageOutput(QtMsgType messageType, const std::source_location& location, const char* category, WideStringView pattern, Args&&... args) {
    QString message;
    QMessageLogContext ctx(location.file_name(), location.line(), location.function_name(), category);
    try {
        message = sformat(pattern, std::forward<Args>(args)...);
    } catch (std::format_error& e) {
        messageType = QtWarningMsg;
        message = sformat(L"Could not format pattern \"{}\": {}", pattern, QByteArrayView(e.what()));
    }

    qt_message_output(messageType, ctx, message);
}

#define X_MESSAGE_OUTPUT(TYPE, PATTERN, ... /* ARGS */) \
    xMessageOutput(TYPE, std::source_location::current(), nullptr, std::wstring_view(L"" PATTERN) __VA_OPT__(,) __VA_ARGS__)

#define xDebug(PATTERN, ... /* ARGS */) \
    X_MESSAGE_OUTPUT(QtDebugMsg, PATTERN __VA_OPT__(,) __VA_ARGS__)

#define xInfo(PATTERN, ... /* ARGS */) \
    X_MESSAGE_OUTPUT(QtInfoMsg, PATTERN __VA_OPT__(,) __VA_ARGS__)

#define xWarning(PATTERN, ... /* ARGS */) \
    X_MESSAGE_OUTPUT(QtWarningMsg, PATTERN __VA_OPT__(,) __VA_ARGS__)

#define xCritical(PATTERN, ... /* ARGS */) \
    X_MESSAGE_OUTPUT(QtCriticalMsg, PATTERN __VA_OPT__(,) __VA_ARGS__)

#define xFatal(PATTERN, ... /* ARGS */) \
    X_MESSAGE_OUTPUT(QtFatalMsg, PATTERN __VA_OPT__(,) __VA_ARGS__)

