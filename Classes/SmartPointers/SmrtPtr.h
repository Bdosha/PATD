#ifndef SMRTPTR_H
#define SMRTPTR_H

#include "SmartPtrStorage.h"
#include <type_traits>
#include <stdexcept>

template<class T>
class SmrtPtr {
private:
    T *ptr;
    SmartPtrStorageBase *storage;

    template<class U>
    friend class SmrtPtr;

    void cleanup() {
        if (ptr != nullptr && storage != nullptr) {
            SmartPtrStorage<T> wrapper;
            wrapper.release(ptr);
        }
        ptr = nullptr;
        storage = nullptr;
    }

public:
    SmrtPtr() : ptr(nullptr), storage(nullptr) {}

    explicit SmrtPtr(T *rawPtr) : ptr(nullptr), storage(nullptr) {
        if (rawPtr != nullptr) {
            storage = &SmartPtrStorageBase::getInstance();
            SmartPtrStorage<T> wrapper;
            ptr = wrapper.store(rawPtr);
        }
    }

    SmrtPtr(const SmrtPtr &other) : ptr(other.ptr), storage(other.storage) {
        if (ptr != nullptr && storage != nullptr) {
            SmartPtrStorage<T> wrapper;
            wrapper.addRef(ptr);
        }
    }

    SmrtPtr(SmrtPtr &&other) noexcept : ptr(other.ptr), storage(other.storage) {
        other.ptr = nullptr;
        other.storage = nullptr;
    }

    template<class U>
    SmrtPtr(const SmrtPtr<U> &other) : ptr(nullptr), storage(nullptr) {
        if constexpr (std::is_base_of_v<T, U> || std::is_same_v<T, U>) {
            ptr = static_cast<T *>(other.get());
            if (ptr != nullptr) {
                // Используем общее хранилище для всех типов
                storage = &SmartPtrStorageBase::getInstance();
                SmartPtrStorage<T> wrapper;
                wrapper.addRef(ptr);
            }
        }
    }

    template<class U>
    SmrtPtr(SmrtPtr<U> &&other) noexcept : ptr(nullptr), storage(nullptr) {
        if constexpr (std::is_base_of_v<T, U> || std::is_same_v<T, U>) {
            ptr = static_cast<T *>(other.get());
            storage = &SmartPtrStorage<T>::getInstance();
            other.reset();
        }
    }

    ~SmrtPtr() {
        cleanup();
    }

    SmrtPtr &operator=(const SmrtPtr &other) {
        if (this != &other) {
            cleanup();
            ptr = other.ptr;
            storage = other.storage;
            if (ptr != nullptr && storage != nullptr) {
                SmartPtrStorage<T> wrapper;
                wrapper.addRef(ptr);
            }
        }
        return *this;
    }

    SmrtPtr &operator=(SmrtPtr &&other) noexcept {
        if (this != &other) {
            cleanup();
            ptr = other.ptr;
            storage = other.storage;
            other.ptr = nullptr;
            other.storage = nullptr;
        }
        return *this;
    }

    template<class U>
    SmrtPtr &operator=(const SmrtPtr<U> &other) {
        if constexpr (std::is_base_of_v<T, U> || std::is_same_v<T, U>) {
            cleanup();
            ptr = static_cast<T *>(other.get());
            storage = &SmartPtrStorage<T>::getInstance();
            if (ptr != nullptr) {
                storage->addRef(ptr);
            }
        }
        return *this;
    }

    template<class U>
    SmrtPtr &operator=(SmrtPtr<U> &&other) noexcept {
        if constexpr (std::is_base_of_v<T, U> || std::is_same_v<T, U>) {
            cleanup();
            ptr = static_cast<T *>(other.get());
            storage = &SmartPtrStorage<T>::getInstance();
            other.reset();
        }
        return *this;
    }

    T &operator*() const {
        if (ptr == nullptr) throw std::runtime_error("Dereferencing null SmrtPtr");
        return *ptr;
    }

    T *operator->() const {
        if (ptr == nullptr) throw std::runtime_error("Accessing null SmrtPtr");
        return ptr;
    }

    [[nodiscard]] T *get() const {
        return ptr;
    }

    void reset(T *rawPtr = nullptr) {
        cleanup();
        if (rawPtr != nullptr) {
            storage = &SmartPtrStorageBase::getInstance();
            SmartPtrStorage<T> wrapper;
            ptr = wrapper.store(rawPtr);
        }
    }

    [[nodiscard]] size_t useCount() const {
        if (ptr == nullptr || storage == nullptr) return 0;
        SmartPtrStorage<T> wrapper;
        return wrapper.getRefCount(ptr);
    }

    [[nodiscard]] explicit operator bool() const {
        return ptr != nullptr;
    }

    bool operator==(const SmrtPtr &other) const {
        return ptr == other.ptr;
    }

    bool operator!=(const SmrtPtr &other) const {
        return ptr != other.ptr;
    }

    template<class U>
    bool operator==(const SmrtPtr<U> &other) const {
        return ptr == other.get();
    }

    template<class U>
    bool operator!=(const SmrtPtr<U> &other) const {
        return ptr != other.get();
    }
};

template<class T, class... Args>
SmrtPtr<T> makeSmrt(Args &&... args) {
    return SmrtPtr<T>(new T(std::forward<Args>(args)...));
}

#endif

