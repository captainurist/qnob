#include "qnob_config.h"

#include <QtCore/QFile>

#include <toml++/toml.h>

#include <util/map_access.h>
#include <util/format.h>

#include "config_exception.h"

static ConfigLocation location(const toml::node& node) {
    return { node.source().begin.line, node.source().begin.column };
}

static VariantMap convertTomlTable(const toml::table& table, bool tablesOnly);

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
        return variantFromRValue(convertTomlTable(*node.as_table(), false));
    default:
        xthrow ConfigException(location(node), ConfigException::tr("Expected a value."));
    }
}

static VariantMap convertTomlTable(const toml::table& table, bool tablesOnly) {
    VariantMap result;
    for (const auto& [id, node] : table) {
        if (tablesOnly && !node.is_table())
            xthrow ConfigException(location(node), ConfigException::tr("Free-standing values are not supported here."));

        result.emplace(QString::fromUtf8(id), convertTomlNode(node));
    }
    return result;
}

QnobConfig QnobConfig::loadFromTomlFile(const QString& path) {
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        xthrow ConfigException(ConfigLocation(), sformat(ConfigException::tr("Could not open config file '{}'."), path));

    if (file.size() > 1024 * 1024 * 1024)
        xthrow ConfigException(ConfigLocation(), sformat(ConfigException::tr("Config file '{}' is too large (exceeds 1Mb)."), path));

    QByteArray bytes = file.readAll();

    toml::table table;
    try {
        table = toml::parse(std::string_view(bytes.data(), bytes.size()));
    } catch (const toml::parse_error& e) {
        std::string_view description = e.description();
        xthrow ConfigException(
            { e.source().begin.line, e.source().begin.column },
            sformat(ConfigException::tr("Toml parse error: {}."), QLatin1String(description.data(), description.size()))
        );
    }

    QnobConfig result;
    for (auto&& [id, record] : convertTomlTable(table, true)) {
        assert(record.typeId() == MetaType::VariantMap);
        result.records.emplace(std::move(id), variantValueRef<VariantMap>(std::move(record)));
    }
    return result;
}
