#ifndef MEMORYSPAN_H
#define MEMORYSPAN_H

#include "UnqPtr.h"
#include "ShrdPtr.h"
#include <stdexcept>

template<class T>
class MsPtr;

template<class T>
class MemorySpan {
private:
    T *data;
    size_t size;
    size_t capacity;

    void cleanup() {
        if (data != nullptr) {
            for (size_t i = 0; i < size; i++) {
                data[i].~T();
            }
            delete[] reinterpret_cast<char *>(data);
            data = nullptr;
        }
    }

public:
    explicit MemorySpan(size_t count) : size(0), capacity(count) {
        if (count == 0) {
            data = nullptr;
            return;
        }
        data = reinterpret_cast<T *>(new char[count * sizeof(T)]);
    }

    MemorySpan(const MemorySpan &) = delete;

    MemorySpan(MemorySpan &&other) noexcept : data(other.data), size(other.size), capacity(other.capacity) {
        other.data = nullptr;
        other.size = 0;
        other.capacity = 0;
    }

    ~MemorySpan() {
        cleanup();
    }

    MemorySpan &operator=(const MemorySpan &) = delete;

    MemorySpan &operator=(MemorySpan &&other) noexcept {
        if (this != &other) {
            cleanup();
            data = other.data;
            size = other.size;
            capacity = other.capacity;
            other.data = nullptr;
            other.size = 0;
            other.capacity = 0;
        }
        return *this;
    }

    template<class... Args>
    T &emplace(size_t index, Args &&... args) {
        if (index >= capacity) {
            throw std::out_of_range("MemorySpan index out of range");
        }
        if (index >= size) {
            size = index + 1;
        }
        new (data + index) T(std::forward<Args>(args)...);
        return data[index];
    }

    ShrdPtr<T> Copy(size_t index) {
        if (index >= size) {
            throw std::out_of_range("MemorySpan index out of range");
        }
        return ShrdPtr<T>(new T(data[index]));
    }

    UnqPtr<T> Get(size_t index) {
        if (index >= size) {
            throw std::out_of_range("MemorySpan index out of range");
        }
        return UnqPtr<T>(new T(data[index]));
    }

    MsPtr<T> Locate(size_t index) {
        if (index >= capacity) {
            throw std::out_of_range("MemorySpan index out of range");
        }
        return MsPtr<T>(this, index);
    }

    [[nodiscard]] size_t getSize() const {
        return size;
    }

    [[nodiscard]] size_t getCapacity() const {
        return capacity;
    }

    T &operator[](size_t index) {
        if (index >= size) {
            throw std::out_of_range("MemorySpan index out of range");
        }
        return data[index];
    }

    const T &operator[](size_t index) const {
        if (index >= size) {
            throw std::out_of_range("MemorySpan index out of range");
        }
        return data[index];
    }

    template<class U>
    friend class MsPtr;
};

#include "MsPtr.h"

#endif

