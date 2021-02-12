#include "human_readable_name.h"

#include <string_view>

QString humanReadableName(const std::type_info& typeInfo) {
    std::string_view src(typeInfo.name());
    std::string dst(src.size(), '_');
    size_t size = QtPrivate::qNormalizeType(src.data(), src.data() + src.size(), dst.data());
    return QLatin1String(dst.data(), size);
}
