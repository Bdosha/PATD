#ifndef CARDINAL_H
#define CARDINAL_H

class Cardinal {
public:
    explicit Cardinal(size_t value = 0);

    static Cardinal Infinity();

    [[nodiscard]] bool isInfinity() const;

    [[nodiscard]] size_t getValue() const;

    bool operator==(const Cardinal &other) const;

    bool operator!=(const Cardinal &other) const;

    bool operator<(const Cardinal &other) const;

    bool operator<=(const Cardinal &other) const;

    bool operator>(const Cardinal &other) const;

    bool operator>=(const Cardinal &other) const;

    Cardinal operator+(const Cardinal &other) const;

    Cardinal operator+(size_t value) const;

    Cardinal operator*(const Cardinal &other) const;

    Cardinal operator*(size_t value) const;

    Cardinal &operator++();

    Cardinal operator++(int);

private:
    bool infinity; // true если значение бесконечное
    size_t value; // конечное значение (игнорируется если infinity == true)
};

#include "Cardinal.tpp"

#endif
