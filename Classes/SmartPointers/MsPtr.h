#ifndef MSPTR_H
#define MSPTR_H

#include "MemorySpan.h"
#include <stdexcept>

template<class T>
class MsPtr {
private:
    MemorySpan<T> *span;
    size_t index;

    void checkBounds() const {
        if (span == nullptr) {
            throw std::runtime_error("MsPtr is not bound to MemorySpan");
        }
        if (index >= span->capacity) {
            throw std::out_of_range("MsPtr index out of range");
        }
    }

public:
    MsPtr() : span(nullptr), index(0) {}

    MsPtr(MemorySpan<T> *s, size_t idx) : span(s), index(idx) {
        if (span != nullptr && index >= span->capacity) {
            throw std::out_of_range("MsPtr index out of range");
        }
    }

    MsPtr(const MsPtr &other) : span(other.span), index(other.index) {}

    MsPtr(MsPtr &&other) noexcept : span(other.span), index(other.index) {
        other.span = nullptr;
        other.index = 0;
    }

    ~MsPtr() = default;

    MsPtr &operator=(const MsPtr &other) {
        if (this != &other) {
            span = other.span;
            index = other.index;
        }
        return *this;
    }

    MsPtr &operator=(MsPtr &&other) noexcept {
        if (this != &other) {
            span = other.span;
            index = other.index;
            other.span = nullptr;
            other.index = 0;
        }
        return *this;
    }

    T &operator*() const {
        checkBounds();
        if (index >= span->size) {
            throw std::out_of_range("MsPtr dereferencing uninitialized element");
        }
        return span->data[index];
    }

    T *operator->() const {
        checkBounds();
        if (index >= span->size) {
            throw std::out_of_range("MsPtr accessing uninitialized element");
        }
        return &span->data[index];
    }

    T &operator[](ptrdiff_t offset) const {
        checkBounds();
        size_t newIndex = index + offset;
        if (newIndex >= span->capacity) {
            throw std::out_of_range("MsPtr index out of range after offset");
        }
        if (newIndex >= span->size) {
            throw std::out_of_range("MsPtr dereferencing uninitialized element");
        }
        return span->data[newIndex];
    }

    MsPtr &operator++() {
        checkBounds();
        if (index + 1 >= span->capacity) {
            throw std::out_of_range("MsPtr increment out of range");
        }
        index++;
        return *this;
    }

    MsPtr operator++(int) {
        MsPtr temp = *this;
        ++(*this);
        return temp;
    }

    MsPtr &operator--() {
        checkBounds();
        if (index == 0) {
            throw std::out_of_range("MsPtr decrement out of range");
        }
        index--;
        return *this;
    }

    MsPtr operator--(int) {
        MsPtr temp = *this;
        --(*this);
        return temp;
    }

    MsPtr operator+(ptrdiff_t offset) const {
        checkBounds();
        size_t newIndex = index + offset;
        if (newIndex >= span->capacity) {
            throw std::out_of_range("MsPtr addition out of range");
        }
        return MsPtr(span, newIndex);
    }

    MsPtr operator-(ptrdiff_t offset) const {
        checkBounds();
        if (offset > static_cast<ptrdiff_t>(index)) {
            throw std::out_of_range("MsPtr subtraction out of range");
        }
        size_t newIndex = index - offset;
        return MsPtr(span, newIndex);
    }

    ptrdiff_t operator-(const MsPtr &other) const {
        if (span != other.span) {
            throw std::runtime_error("MsPtr subtraction from different MemorySpan");
        }
        return static_cast<ptrdiff_t>(index) - static_cast<ptrdiff_t>(other.index);
    }

    MsPtr &operator+=(ptrdiff_t offset) {
        *this = *this + offset;
        return *this;
    }

    MsPtr &operator-=(ptrdiff_t offset) {
        *this = *this - offset;
        return *this;
    }

    bool operator==(const MsPtr &other) const {
        return span == other.span && index == other.index;
    }

    bool operator!=(const MsPtr &other) const {
        return !(*this == other);
    }

    bool operator<(const MsPtr &other) const {
        if (span != other.span) {
            throw std::runtime_error("MsPtr comparison from different MemorySpan");
        }
        return index < other.index;
    }

    bool operator>(const MsPtr &other) const {
        return other < *this;
    }

    bool operator<=(const MsPtr &other) const {
        return !(other < *this);
    }

    bool operator>=(const MsPtr &other) const {
        return !(*this < other);
    }

    [[nodiscard]] size_t getIndex() const {
        return index;
    }

    [[nodiscard]] explicit operator bool() const {
        return span != nullptr;
    }
};

template<class T>
MsPtr<T> operator+(ptrdiff_t offset, const MsPtr<T> &ptr) {
    return ptr + offset;
}

#endif

