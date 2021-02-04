#pragma once

#include <utility/exception.h>

struct ConfigLocation {
    int line;
    int column;
};

class ConfigException: public Exception {
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


