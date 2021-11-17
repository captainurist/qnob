#pragma once

#include <util/exception/exception.h>

struct ConfigLocation {
    int line;
    int column;
};

class ConfigException: public Exception {
    Q_DECLARE_TR_FUNCTIONS(ConfigException)
public:
    ConfigException(const ConfigLocation& location, const QString& message) :
        Exception(message),
        m_location(location)
    {}

    const ConfigLocation& location() const {
        return m_location;
    }

private:
    ConfigLocation m_location;
};


