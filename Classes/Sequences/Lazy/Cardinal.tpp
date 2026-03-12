#pragma once
#include <stdexcept>

inline Cardinal::Cardinal(size_t value) : infinity(false), value(value) {
}

inline Cardinal Cardinal::Infinity() {
    Cardinal result(0);
    result.infinity = true;
    return result;
}

inline bool Cardinal::isInfinity() const {
    return infinity;
}

inline size_t Cardinal::getValue() const {
    if (infinity) {
        throw std::runtime_error("Cannot get value of infinite Cardinal");
    }
    return value;
}

inline bool Cardinal::operator==(const Cardinal &other) const {
    if (infinity && other.infinity) return true;
    if (infinity || other.infinity) return false;
    return value == other.value;
}

inline bool Cardinal::operator!=(const Cardinal &other) const {
    return !(*this == other);
}

inline bool Cardinal::operator<(const Cardinal &other) const {
    if (infinity && other.infinity) return false;
    if (infinity) return false; // бесконечность не меньше конечного
    if (other.infinity) return true; // конечное меньше бесконечности
    return value < other.value;
}

inline bool Cardinal::operator<=(const Cardinal &other) const {
    return *this < other || *this == other;
}

inline bool Cardinal::operator>(const Cardinal &other) const {
    return other < *this;
}

inline bool Cardinal::operator>=(const Cardinal &other) const {
    return other <= *this;
}

inline Cardinal Cardinal::operator+(const Cardinal &other) const {
    if (infinity || other.infinity) {
        return Infinity();
    }
    return Cardinal(value + other.value);
}

inline Cardinal Cardinal::operator+(size_t val) const {
    if (infinity) {
        return Infinity();
    }
    return Cardinal(value + val);
}

inline Cardinal Cardinal::operator*(const Cardinal &other) const {
    if (infinity || other.infinity) {
        // Проверяем, является ли один из операндов нулем
        if ((!infinity && value == 0) || (!other.infinity && other.value == 0)) {
            return Cardinal(0); // 0 * ω = 0, ω * 0 = 0
        }
        return Infinity();
    }
    return Cardinal(value * other.value);
}

inline Cardinal Cardinal::operator*(size_t val) const {
    if (infinity) {
        if (val == 0) {
            return Cardinal(0);
        }
        return Infinity();
    }
    return Cardinal(value * val);
}

inline Cardinal &Cardinal::operator++() {
    if (!infinity)
        value++;

    return *this;
}

inline Cardinal Cardinal::operator++(int) {
    const Cardinal temp = *this;
    ++(*this);
    return temp;
}
