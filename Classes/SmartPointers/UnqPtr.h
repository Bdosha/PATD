#ifndef UNQPTR_H
#define UNQPTR_H

#include <stdexcept>
#include <utility>

// Дефолтный deleter для одного объекта
template<class T>
struct default_delete {
    void operator()(T *ptr) const {
        delete ptr;
    }
};

// Специализация deleter для массивов
template<class T>
struct default_delete<T[]> {
    void operator()(T *ptr) const {
        delete[] ptr;
    }
};

template<class T, class Deleter = default_delete<T>>
class UnqPtr {
    T *ptr;
    Deleter deleter;

    void cleanup() {
        if (ptr != nullptr) {
            deleter(ptr);
            ptr = nullptr;
        }
    }

public:
    UnqPtr() : ptr(nullptr), deleter() {}

    UnqPtr(T *rawPtr) : ptr(rawPtr), deleter() {}

    UnqPtr(T *rawPtr, Deleter d) : ptr(rawPtr), deleter(d) {}

    UnqPtr(const UnqPtr &) = delete;

    UnqPtr(UnqPtr &&other) noexcept : ptr(other.ptr), deleter(std::move(other.deleter)) {
        other.ptr = nullptr;
    }

    template<class U>
    UnqPtr(UnqPtr<U> &&other) noexcept : ptr(nullptr), deleter() {
        if constexpr (std::is_base_of_v<T, U> || std::is_same_v<T, U>) {
            ptr = static_cast<T *>(other.release());
        }
    }

    ~UnqPtr() {
        cleanup();
    }

    UnqPtr &operator=(const UnqPtr &) = delete;

    UnqPtr &operator=(UnqPtr &&other) noexcept {
        if (this != &other) {
            cleanup();
            ptr = other.ptr;
            deleter = std::move(other.deleter);
            other.ptr = nullptr;
        }
        return *this;
    }

    template<class U>
    UnqPtr &operator=(UnqPtr<U> &&other) noexcept {
        if constexpr (std::is_base_of_v<T, U> || std::is_same_v<T, U>) {
            cleanup();
            ptr = static_cast<T *>(other.release());
        }
        return *this;
    }

    T &operator*() const {
        if (ptr == nullptr) throw std::runtime_error("Dereferencing null UnqPtr");
        return *ptr;
    }

    T *operator->() const {
        if (ptr == nullptr) throw std::runtime_error("Accessing null UnqPtr");
        return ptr;
    }

    [[nodiscard]] T *get() const {
        return ptr;
    }

    T *release() {
        T *temp = ptr;
        ptr = nullptr;
        return temp;
    }

    void reset(T *rawPtr = nullptr) {
        cleanup();
        ptr = rawPtr;
    }

    void reset(T *rawPtr, Deleter d) {
        cleanup();
        ptr = rawPtr;
        deleter = d;
    }

    Deleter &get_deleter() {
        return deleter;
    }

    const Deleter &get_deleter() const {
        return deleter;
    }

    [[nodiscard]] explicit operator bool() const {
        return ptr != nullptr;
    }

    bool operator==(const UnqPtr &other) const {
        return ptr == other.ptr;
    }

    bool operator!=(const UnqPtr &other) const {
        return ptr != other.ptr;
    }
};

template<class T, class... Args>
UnqPtr<T> makeUnq(Args &&... args) {
    return UnqPtr<T>(new T(std::forward<Args>(args)...));
}

// Функция для создания UnqPtr из массива
template<class T>
UnqPtr<T, default_delete<T[]>> makeUnqArray(size_t size) {
    return UnqPtr<T, default_delete<T[]>>(new T[size]);
}

// Функция для создания UnqPtr из массива с инициализацией
template<class T>
UnqPtr<T, default_delete<T[]>> makeUnqArray(size_t size, const T &value) {
    T *arr = new T[size];
    for (size_t i = 0; i < size; i++) {
        arr[i] = value;
    }
    return UnqPtr<T, default_delete<T[]>>(arr);
}

#endif

