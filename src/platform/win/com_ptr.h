#pragma once

#include <utility> /* For std::swap */
#include <type_traits>

#include <combaseapi.h>

template<class T>
class ComPtr {
    //static_assert(std::is_base_of_v<IUnknown, T>, "T must be derived from IUnknown."); // TODO
public:
    ComPtr() {}

    ComPtr(T* ptr) {
        m_ptr = ptr;
    }

    ComPtr(const ComPtr& other) {
        m_ptr = other.m_ptr;

        if (m_ptr)
            m_ptr->AddRef();
    }

    ComPtr(ComPtr&& other) {
        m_ptr = other.m_ptr;
        other.m_ptr = nullptr;
    }

    ~ComPtr() {
        reset();
    }

    ComPtr& operator=(const ComPtr& other) {
        this->~ComPtr();
        new (this) ComPtr(other);
        return *this;
    }

    ComPtr& operator=(ComPtr&& other) {
        this->~ComPtr();
        new (this) ComPtr(std::move(other));
        return *this;
    }

    T* operator->() const {
        return m_ptr;
    }

    T& operator*() const {
        return *m_ptr;
    }

    operator bool() const {
        return m_ptr;
    }

    template<class OtherT>
    ComPtr<OtherT> cast() {
        if (!m_ptr)
            return ComPtr<OtherT>();

        OtherT* result = nullptr;
        if (!SUCCEEDED(m_ptr->QueryInterface(IID_PPV_ARGS(&result))))
            return ComPtr<OtherT>();

        return result;
    }

    void reset() {
        if (!m_ptr)
            return;

        m_ptr->Release();
        m_ptr = nullptr;
    }

    CLSID staticId() const {
        return __uuidof(T);
    }

    T** mutablePtr() {
        return &m_ptr;
    }

    void** mutableVoidPtr() {
        return reinterpret_cast<void**>(&m_ptr);
    }

    friend void** IID_PPV_ARGS_Helper(ComPtr* pp) {
        return pp->mutableVoidPtr();
    }

private:
    T* m_ptr = nullptr;
};

