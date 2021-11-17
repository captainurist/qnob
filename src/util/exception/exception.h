#pragma once

#include <exception>
#include <source_location>

#include <QtCore/QString>
#include <QtCore/QCoreApplication>

class QDebug;

// TODO: do we even need i18n in #exceptions?
// TODO: and actually most of the fields in derived classes are also not needed...

/**
 * Custom exception that supports source location & QString-based exception messages.
 * Base class for all qnob exceptions.
 */
class Exception : public std::exception {
    Q_DECLARE_TR_FUNCTIONS(Exception)
public:
    enum ChainMode {
        DontChain = 0,
        AutoChain = 1,
    };

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
     * \param message                   Exception message. Should not include the trailing dot.
     * \param chainMode                 Chain mode, pass `DontChain` so that the current exception is not automatically
     *                                  assigned as a cause for this exception.
     */
    Exception(const QString& message, ChainMode chainMode = AutoChain) {
        m_message = message;
        m_utf8Message = message.toUtf8();
        if (chainMode == AutoChain)
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

    // TODO: actually fix all places where message is passed! To comply with the comment above. #exceptions

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

    friend QDebug operator<<(QDebug stream, const Exception& e);

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
#define xthrow throw std::source_location::current() +
