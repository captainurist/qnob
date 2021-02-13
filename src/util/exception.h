#pragma once

#include <exception>

#include <QtCore/QString>
#include <QtCore/QCoreApplication>

#include "source_location.h"

class QDebug;

/**
 * Custom exception that supports source location & QString-based exception messages.
 * Base class for all qnob exceptions.
 */
class Exception : public std::exception {
    Q_DECLARE_TR_FUNCTIONS(Exception)
public:
    /**
     * Creates an exception with the provided message.
     *
     * Note that derived class is expected to put _all exception information_ into this message, and (optionally)
     * provide additional accessors in case the client wants to format the message himself.
     *
     * Also note that if an exception is constructed inside an exception handler, then the current exception is
     * automatically captured and can be later accessed by calling `cause`.
     *
     * Format notice to make life easier: provided exception message should not include the trailing dot.
     *
     * \param message                   Exception message.
     */
    Exception(const QString& message) {
        m_message = message;
        m_utf8Message = message.toUtf8();
        m_cause = std::current_exception();
    }

    virtual char const* what() const override {
        return m_utf8Message.data();
    }

    /**
     * Note that this method is not virtual, and this is intentional. Otherwise `what()` becomes messy.
     *
     * Also note that the message returned does not contain code location information, or any tied exceptions.
     *
     * If you plan to display the returned message for the user, please note that it is expected not to include the
     * trailing dot.
     *
     * \returns                         Exception message as provided in the class constructor.
     */
    const QString& message() const {
        return m_message;
    }

    /**
     * \returns                         Source location where this exception was thrown from.
     */
    const std::source_location& sourceLocation() const {
        return m_sourceLocation;
    }

    /**
     * \returns                         Exception that caused this exception to be thrown.
     */
    const std::exception_ptr& cause() const {
        return m_cause;
    }

    friend QDebug&& operator<<(QDebug&& stream, const Exception& e);

private:
    template<class OtherException>
    friend OtherException&& operator+(const std::source_location& sourceLocation, OtherException&& other) {
        other.m_sourceLocation = sourceLocation;
        return std::forward<OtherException>(other);
    }

private:
    QString m_message;
    QByteArray m_utf8Message;
    std::source_location m_sourceLocation;
    std::exception_ptr m_cause;
};

/**
 * Same as `throw`, but adds source location information to the thrown `Exception`.
 */
#define qthrow throw __LOCATION__ +
