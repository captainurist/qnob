#pragma once

#include <vector>
#include <unordered_map>

#include <QtCore/QByteArray>
#include <QtCore/QVariant>

struct FlatConfigRecord {
    QByteArray type;
    QByteArray id;
    std::unordered_map<QByteArray, QVariant> data;
};

struct FlatConfig {
    std::vector<FlatConfigRecord> records;

    static FlatConfig fromTomlFile(const QString& path);
};
