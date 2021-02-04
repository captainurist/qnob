#pragma once

#include <vector>

#include "entity_config.h"

class FullConfig {
public:
    std::vector<EntityConfig> records;

    /**
     * Loads a config from a toml file.
     *
     * \param path                      Path to the toml file.
     * \returns                         Loaded config.
     * \throws ConfigException
     */
    static FullConfig loadFromTomlFile(const QString& path);
};
