#pragma once

#include <exception>

#include <QtCore/QByteArray>

#include <combaseapi.h>


class ComException : public std::exception {
public:
    ComException(HRESULT result = S_OK, const char* context = nullptr);

    virtual char const* what() const override {
        return message_.data();
    }

private:
    QByteArray message_;
};

#define CHECKED_COM_CALL(x) if(bool ok = SUCCEEDED(x); !ok) throw ComException(result, #x);
