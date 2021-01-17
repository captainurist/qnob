#pragma once

#include <utility> /* For std::swap */
#include <type_traits>

#include <combaseapi.h>

#include "com_exception.h"

template<class T>
class ComPtr {
    //static_assert(std::is_base_of_v<IUnknown, T>, "T must be derived from IUnknown."); // TODO
public:
    ComPtr() {}

    ComPtr(T* ptr) {
        ptr_ = ptr;
    }

    ComPtr(const ComPtr& other) {
        ptr_ = other.ptr_;

        if (ptr_)
            ptr_->AddRef();
    }

    ComPtr(ComPtr&& other) {
        ptr_ = other.ptr_;
        other.ptr_ = nullptr;
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
        return ptr_;
    }

    T& operator*() const {
        return *ptr_;
    }

    operator bool() const {
        return ptr_;
    }

    template<class OtherT>
    ComPtr<OtherT> cast() {
        if (!ptr_)
            return ComPtr<OtherT>();

        OtherT* result = nullptr;
        if (!SUCCEEDED(ptr_->QueryInterface(IID_PPV_ARGS(&result))))
            return ComPtr<OtherT>();

        return result;
    }

    void reset() {
        if (!ptr_)
            return;

        ptr_->Release();
        ptr_ = nullptr;
    }

    CLSID staticId() const {
        return __uuidof(T);
    }

    T** mutablePtr() {
        return &ptr_;
    }

    void** mutableVoidPtr() {
        return reinterpret_cast<void**>(&ptr_);
    }

    friend void** IID_PPV_ARGS_Helper(ComPtr* pp) {
        return pp->mutableVoidPtr();
    }

private:
    T* ptr_ = nullptr;
};

