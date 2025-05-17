#ifndef SET_H
#define SET_H
#include "../Sequences/Sequence.h"

template<class T>
class Set {
    Sequence<T> *data;

public:
    Set();

    explicit Set(Sequence<T> *);

    ~Set() = default;

    Sequence<T> *toSequence() const;

    Set &add(T);

    bool contains(T) const;

    bool contains(const Set &) const; //TODO name

    [[nodiscard]] int size() const;

    T get(int) const;


    Set &remove(T);

    Set &unite(const Set &) const;

    Set &operator|(const Set &) const;

    Set &intersect(const Set &) const;

    Set &operator&(const Set &) const;

    Set &subtract(const Set &) const;

    Set &operator-(const Set &) const;


    bool operator>(const Set &) const;

    bool operator<(const Set &) const;

    bool operator>=(const Set &) const;

    bool operator<=(const Set &) const;

    Set &map(T (T));

    Set &where(bool (T));

    explicit operator bool() const;

    Set &clear();


private:
    int search(T) const; //поиск индекса с минимальным числом больше или равным заданного (+1 - само число значит)
};

template<class T>
bool operator==(const Set<T> &set1, const Set<T> &set2);

template<class T>
bool operator!=(const Set<T> &set1, const Set<T> &set2);

template<class T>
std::ostream &operator<<(std::ostream &os, const Set<T> &set);


#include "Set.tpp"
#endif
