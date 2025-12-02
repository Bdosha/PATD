#ifndef SHRDPTR_H
#define SHRDPTR_H

#include "UnqPtr.h"
#include <stdexcept>

template<class T>
class ShrdPtr {
    T *ptr;
    size_t *referenceCount;

    void cleanup() {
        if (ptr != nullptr && referenceCount != nullptr) {
            (*referenceCount)--;
            if (*referenceCount == 0) {
                delete ptr;
                delete referenceCount;
            }
        }
        ptr = nullptr;
        referenceCount = nullptr;
    }

    void initFromUnqPtr(UnqPtr<T> &unqPtr) {
        if (unqPtr) {
            ptr = unqPtr.release();
            referenceCount = new size_t(1);
        } else {
            ptr = nullptr;
            referenceCount = nullptr;
        }
    }

public:
    ShrdPtr() : ptr(nullptr), referenceCount(nullptr) {}

    explicit ShrdPtr(T *rawPtr) : ptr(rawPtr), referenceCount(nullptr) {
        if (ptr != nullptr) {
            referenceCount = new size_t(1);
        }
    }

    explicit ShrdPtr(UnqPtr<T> &unqPtr) : ptr(nullptr), referenceCount(nullptr) {
        initFromUnqPtr(unqPtr);
    }

    ShrdPtr(const ShrdPtr &other) : ptr(other.ptr), referenceCount(other.referenceCount) {
        if (ptr != nullptr && referenceCount != nullptr) {
            (*referenceCount)++;
        }
    }

    ShrdPtr(ShrdPtr &&other) noexcept : ptr(other.ptr), referenceCount(other.referenceCount) {
        other.ptr = nullptr;
        other.referenceCount = nullptr;
    }

    template<class U>
    ShrdPtr(const ShrdPtr<U> &other) : ptr(nullptr), referenceCount(nullptr) {
        if constexpr (std::is_base_of_v<T, U> || std::is_same_v<T, U>) {
            ptr = static_cast<T *>(other.get());
            if (ptr != nullptr) {
                referenceCount = other.getRefCount();
                if (referenceCount != nullptr) {
                    (*referenceCount)++;
                }
            }
        }
    }

    template<class U>
    ShrdPtr(ShrdPtr<U> &&other) noexcept : ptr(nullptr), referenceCount(nullptr) {
        if constexpr (std::is_base_of_v<T, U> || std::is_same_v<T, U>) {
            ptr = static_cast<T *>(other.get());
            referenceCount = other.getRefCount();
            other.reset();
        }
    }

    ~ShrdPtr() {
        cleanup();
    }

    ShrdPtr &operator=(const ShrdPtr &other) {
        if (this != &other) {
            cleanup();
            ptr = other.ptr;
            referenceCount = other.referenceCount;
            if (ptr != nullptr && referenceCount != nullptr) {
                (*referenceCount)++;
            }
        }
        return *this;
    }

    ShrdPtr &operator=(ShrdPtr &&other) noexcept {
        if (this != &other) {
            cleanup();
            ptr = other.ptr;
            referenceCount = other.referenceCount;
            other.ptr = nullptr;
            other.referenceCount = nullptr;
        }
        return *this;
    }

    ShrdPtr &operator=(UnqPtr<T> &unqPtr) {
        cleanup();
        initFromUnqPtr(unqPtr);
        return *this;
    }

    template<class U>
    ShrdPtr &operator=(const ShrdPtr<U> &other) {
        if constexpr (std::is_base_of_v<T, U> || std::is_same_v<T, U>) {
            cleanup();
            ptr = static_cast<T *>(other.get());
            if (ptr != nullptr) {
                referenceCount = other.getRefCount();
                if (referenceCount != nullptr) {
                    (*referenceCount)++;
                }
            }
        }
        return *this;
    }

    template<class U>
    ShrdPtr &operator=(ShrdPtr<U> &&other) noexcept {
        if constexpr (std::is_base_of_v<T, U> || std::is_same_v<T, U>) {
            cleanup();
            ptr = static_cast<T *>(other.get());
            referenceCount = other.getRefCount();
            other.reset();
        }
        return *this;
    }

    T &operator*() const {
        if (ptr == nullptr) throw std::runtime_error("Dereferencing null ShrdPtr");
        return *ptr;
    }

    T *operator->() const {
        if (ptr == nullptr) throw std::runtime_error("Accessing null ShrdPtr");
        return ptr;
    }

    [[nodiscard]] T *get() const {
        return ptr;
    }

    [[nodiscard]] size_t *getRefCount() const {
        return referenceCount;
    }

    void reset(T *rawPtr = nullptr) {
        cleanup();
        if (rawPtr != nullptr) {
            ptr = rawPtr;
            referenceCount = new size_t(1);
        }
    }

    [[nodiscard]] size_t useCount() const {
        if (referenceCount == nullptr) return 0;
        return *referenceCount;
    }

    [[nodiscard]] explicit operator bool() const {
        return ptr != nullptr;
    }

    bool operator==(const ShrdPtr &other) const {
        return ptr == other.ptr;
    }

    bool operator!=(const ShrdPtr &other) const {
        return ptr != other.ptr;
    }

    template<class U>
    bool operator==(const ShrdPtr<U> &other) const {
        return ptr == other.get();
    }

    template<class U>
    bool operator!=(const ShrdPtr<U> &other) const {
        return ptr != other.get();
    }
};

template<class T, class... Args>
ShrdPtr<T> makeShrd(Args &&... args) {
    return ShrdPtr<T>(new T(std::forward<Args>(args)...));
}

#endif

