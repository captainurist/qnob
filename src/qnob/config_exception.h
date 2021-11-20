#pragma once

#include <util/exception/exception.h>

// TODO: #exceptions I18n

struct ConfigLocation {
    int line = -1;
    int column = -1;
};

class ConfigException: public Exception {
    Q_DECLARE_TR_FUNCTIONS(ConfigException)
public:
    ConfigException(const ConfigLocation& location, const QString& message) :
        Exception(message),
        m_location(location)
    {}

    bool hasLocation() const {
        return m_location.column != -1 && m_location.line != -1;
    }

    const ConfigLocation& location() const {
        return m_location;
    }

private:
    ConfigLocation m_location;
};


