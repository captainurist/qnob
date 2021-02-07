#include "full_config.h"

#include <QtCore/QFile>
#include <QtCore/QCoreApplication>

#include <toml++/toml.h>

#include <util/map_access.h>

#include "config_exception.h"

namespace {

ConfigLocation location(const toml::node& node) {
    return { node.source().begin.line, node.source().begin.column };
}

QVariant getValue(const toml::node& node) {
    switch (node.type()) {
    case toml::node_type::string:
        return QVariant::fromValue<QString>(QString::fromUtf8(node.as_string()->get()));
    case toml::node_type::integer:
        return QVariant::fromValue<int64_t>(node.as_integer()->get());
    case toml::node_type::floating_point:
        return QVariant::fromValue<double>(node.as_floating_point()->get());
    case toml::node_type::boolean:
        return QVariant::fromValue<bool>(node.as_boolean()->get());
    case toml::node_type::array: {
        QVariantList result;
        for (const toml::node& element : *node.as_array())
            result.push_back(getValue(element));
        return QVariant::fromValue<QVariantList>(result);
    }
    default:
        qthrow ConfigException(location(node), ConfigException::tr("Expected a value."));
    }
}

}

FullConfig FullConfig::loadFromTomlFile(const QString& path) {
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        qthrow Exception(ConfigException::tr("Could not open config file '%1'.").arg(path));

    if (file.size() > 1024 * 1024 * 1024)
        qthrow Exception(ConfigException::tr("Config file '%1' is too large (exceeds 1Mb).").arg(path));

    QByteArray bytes = file.readAll();

    toml::table table;
    try {
        table = toml::parse(std::string_view(bytes.data(), bytes.size()));
    } catch (const toml::parse_error& e) {
        std::string_view description = e.description();
        qthrow ConfigException(
            { e.source().begin.line, e.source().begin.column },
            ConfigException::tr("Toml parse error: %1.").arg(QString::fromLatin1(description.data(), description.size()))
        );
    }

    FullConfig result;
    for (auto section : table) {
        if (!section.second.is_table())
            qthrow ConfigException(location(section.second), ConfigException::tr("Values outside of sections are not allowed."));

        EntityConfig entity;
        entity.id = QString::fromUtf8(section.first);
        for (auto line : *section.second.as_table())
            entity.data[QString::fromUtf8(line.first)] = getValue(line.second);

        entity.type = value(entity.data, lit("type")).toString();
        if (entity.type.isEmpty())
            qthrow ConfigException(location(section.second), ConfigException::tr("Type is not specified for section '%1'.").arg(entity.id));

        result.records.push_back(entity);
    }

    return result;
}

