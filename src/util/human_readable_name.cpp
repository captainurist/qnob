#include "human_readable_name.h"

#include <typeindex>

#include <QtCore/QRegularExpression>
#include <QtCore/QMutex>

#include "map_access.h"

class HumanReadable {
public:
    HumanReadable() :
        m_clearRegex(lit(R"XXX(\bclass \b|\benum \b|\bstruct \b)XXX"))
    {}

    QString name(const std::type_info& typeInfo) {
        QMutexLocker locker(&m_mutex);

        QString result = value_or(m_valueByType, typeInfo);
        if (!result.isEmpty())
            return result;

        result = normalize(QLatin1String(typeInfo.name()));
        m_valueByType.emplace(typeInfo, result);
        return result;
    }

    QString signature(const char* signature) {
        return normalize(QLatin1String(signature));
    }

private:
    QString normalize(QString&& src) const {
        return src.replace(m_clearRegex, QString()).replace(lit("> >"), lit(">>"));
    }

private:
    QRegularExpression m_clearRegex;
    QMutex m_mutex;
    std::unordered_map<std::type_index, QString> m_valueByType;
};
Q_GLOBAL_STATIC(HumanReadable, g_humanReadable)

QString humanReadableName(const std::type_info& typeInfo) {
    return g_humanReadable->name(typeInfo);
}

QString humanReadableSignature(const char* signature) {
    return g_humanReadable->signature(signature);
}