#pragma once

#include <QtCore/QVariant>

#include <util/variant.h>

class QnobConfig {
public:
    std::unordered_map<QString, VariantMap> records;

    /**
     * Loads a config from a toml file.
     *
     * \param path                      Path to the toml file.
     * \returns                         Loaded config.
     * \throws ConfigException
     */
    static QnobConfig loadFromTomlFile(const QString& path);
};
