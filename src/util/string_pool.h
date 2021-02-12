#pragma once

#include <QtCore/QStringView>

#include "pod_pool.h"

class StringPool : public PodPool {
public:
    StringPool(size_t capacity = 1024) : PodPool(capacity) {}

    using PodPool::insert;

    QStringView insert(QStringView string) {
        void* ptr = insert(string.data(), string.size() * sizeof(QChar), sizeof(QChar));

        return QStringView(static_cast<const QChar*>(ptr), string.size());
    }
};
