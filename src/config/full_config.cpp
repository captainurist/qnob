#include "full_config.h"

#include <QtCore/QFile>

#include <toml++/toml.h>

#include <util/map_access.h>

#include "config_exception.h"

static ConfigLocation location(const toml::node& node) {
    return { node.source().begin.line, node.source().begin.column };
}

static VariantMap convertTomlTable(const toml::table& table);

static QVariant convertTomlNode(const toml::node& node) {
    switch (node.type()) {
    case toml::node_type::string:
        return QVariant::fromValue<QString>(QString::fromUtf8(node.as_string()->get()));
    case toml::node_type::integer:
        return QVariant::fromValue<qint64>(node.as_integer()->get());
    case toml::node_type::floating_point:
        return QVariant::fromValue<double>(node.as_floating_point()->get());
    case toml::node_type::boolean:
        return QVariant::fromValue<bool>(node.as_boolean()->get());
    case toml::node_type::array: {
        VariantVector result;
        for (const toml::node& element : *node.as_array())
            result.push_back(convertTomlNode(element));
        return variantFromRValue(std::move(result));
    }
    case toml::node_type::table:
        return variantFromRValue(convertTomlTable(*node.as_table()));
    default:
        qthrow ConfigException(location(node), ConfigException::tr("Expected a value."));
    }
}

static VariantMap convertTomlTable(const toml::table& table) {
    VariantMap result;
    for (const auto& [id, section] : table)
        result.emplace(QString::fromUtf8(id), convertTomlNode(section));
    return result;
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

    return { path, convertTomlTable(table) };
}

