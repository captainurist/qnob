#pragma once

#include <cassert>

#include <QtCore/Qt>
#include <QtCore/QThread>

// TODO: move to debug/

/**
 * Asserts when events are delivered into a nested event loop in any thread.
 *
 * Does nothing in release.
 */
class DisableNestedEventLoops {
public:
    DisableNestedEventLoops() {
#ifndef NDEBUG
        QInternal::registerCallback(QInternal::EventNotifyCallback, &DisableNestedEventLoops::callback);
#endif
    }

    ~DisableNestedEventLoops() {
#ifndef NDEBUG
        QInternal::unregisterCallback(QInternal::EventNotifyCallback, &DisableNestedEventLoops::callback);
#endif
    }

private:
    static bool callback(void** args) {
        /* Arguments are QObject* receiver, QEvent* event, bool* result, but we don't need them here. */

        assert(QThread::currentThread()->loopLevel() <= 1);
        return false;
    }
};
