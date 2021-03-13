#pragma once

#include <QtCore/QVariant>

#include <util/variant.h>

class FullConfig {
public:
    QString path;
    VariantMap records;

    /**
     * Loads a config from a toml file.
     *
     * \param path                      Path to the toml file.
     * \returns                         Loaded config.
     * \throws ConfigException
     */
    static FullConfig loadFromTomlFile(const QString& path);
};
