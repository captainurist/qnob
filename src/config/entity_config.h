#pragma once

#include <QtCore/QString>
#include <QtCore/QVariant>

#include <unordered_map>

class EntityConfig {
public:
    QString id;
    QString type;
    std::unordered_map<QString, QVariant> data;
};
