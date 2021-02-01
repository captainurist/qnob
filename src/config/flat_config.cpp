#include "flat_config.h"

#include <QtCore/QFile>
#include <QtCore/QCoreApplication>

#include <toml++/toml.h>

#include "config_exception.h"

namespace {

struct ConfigError {
    Q_DECLARE_TR_FUNCTIONS(ConfigError)
};

ConfigLocation location(const toml::node& node) {
    return { node.source().begin.line, node.source().begin.column };
}

QVariant getValue(const toml::node& node) {
    switch (node.type()) {
    case toml::node_type::string:
        return QVariant(QByteArray::fromStdString(node.as_string()->get()));
    case toml::node_type::integer:
        return QVariant(node.as_integer()->get());
    case toml::node_type::floating_point:
        return QVariant(node.as_floating_point()->get());
    case toml::node_type::boolean:
        return QVariant(node.as_boolean()->get());
    default:
        qthrow ConfigException(location(node), ConfigError::tr("Expected a value."));
    }
}

}

FlatConfig FlatConfig::fromTomlFile(const QString& path) {
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        qthrow Exception(ConfigError::tr("Could not open config file '%1'.").arg(path));

    if (file.size() > 1024 * 1024 * 1024)
        qthrow Exception(ConfigError::tr("Config file '%1' too large (exceeds 1Mb).").arg(path));

    QByteArray bytes = file.readAll();

    toml::table table;
    try {
        table = toml::parse(std::string_view(bytes.data(), bytes.size()));
    } catch (const toml::parse_error& e) {
        std::string_view description = e.description();
        qthrow ConfigException(
            { e.source().begin.line, e.source().begin.column },
            ConfigError::tr("Toml parse error: %1.").arg(QString::fromLatin1(description.data(), description.size()))
        );
    }

    FlatConfig result;
    for (auto section : table) {
        if (!section.second.is_table())
            qthrow ConfigException(location(section.second), ConfigError::tr("Values outside of sections are not allowed."));

        for (auto instance : *section.second.as_table()) {
            if (!instance.second.is_table())
                qthrow ConfigException(location(instance.second), ConfigError::tr("Section format is [type.id], found only [type] instead."));

            FlatConfigRecord record;
            record.type = QByteArray::fromStdString(section.first);
            record.id = QByteArray::fromStdString(instance.first);
            for (auto field : *instance.second.as_table())
                record.data[QByteArray::fromStdString(field.first)] = getValue(field.second);
            result.records.push_back(record);
        }
    }

    return result;
}

