#pragma once

class QString;

namespace detail {
    QString translate(const char* context, const char* sourceText, const char* disambiguation, int n);
}

/**
 * Same as `Q_DECLARE_TR_FUNCTIONS`, but doesn't require `QCoreApplication` include.
 *
 * \param context                       Class name.
 */
#define X_DECLARE_TR_FUNCTIONS(context)                                                                                 \
public:                                                                                                                 \
    static inline QString tr(const char* sourceText, const char* disambiguation = nullptr, int n = -1)                  \
    { return detail::translate(#context, sourceText, disambiguation, n); }                                              \
private:
