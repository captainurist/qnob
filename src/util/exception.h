#pragma once

#include <exception>

#include <QtCore/QString>
#include <QtCore/QCoreApplication>

#include "source_location.h"

/**
 * Custom exception that supports source location & QString-based exception messages.
 * Base class for all qnob exceptions.
 */
class Exception : public std::exception {
    Q_DECLARE_TR_FUNCTIONS(Exception)
public:
    Exception(const QString& message) {
        m_message = message;
        m_latin1Message = message.toLatin1();
    }

    virtual char const* what() const override {
        return m_latin1Message.data();
    }

    /**
     * Note that this method is not virtual, and this is intentional. Otherwise `what()` becomes messy.
     *
     * \returns                         Exception message.
     */
    const QString& message() const {
        return m_message;
    }

    const std::source_location& sourceLocation() const {
        return m_sourceLocation;
    }

    template<class OtherException>
    friend OtherException&& operator+(const std::source_location& sourceLocation, OtherException&& other) {
        other.m_sourceLocation = sourceLocation;
        return std::forward<OtherException>(other);
    }

private:
    QString m_message;
    QByteArray m_latin1Message;
    std::source_location m_sourceLocation;
};


#define qthrow throw __LOCATION__ +
