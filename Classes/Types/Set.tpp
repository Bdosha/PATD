#pragma once

template<class T>
Set<T>::Set() {
    data = new MutableArraySequence<T>;
}

template<class T>
Set<T>::Set(Sequence<T> *seq) {
    data = new MutableArraySequence<T>(seq);
}


template<class T>
Sequence<T> *Set<T>::toSequence() const {
    return new MutableArraySequence<T>(data);
}


template<class T>
int Set<T>::search(T value) const {
    int k = 0;
    const int n = size();
    for (int b = n / 2; b > 0; b /= 2) {
        while (k + b < n && get(k + b) <= value)
            k += b;
    }
    return k;
}


template<class T>
Set<T> &Set<T>::add(T value) {
    int k = search(value);
    if (k == size()) {
        data->append(value);
        return *this;
    }
    if (get(k) == value)
        return *this;
    if (get(k) < value)
        ++k;

    data->insertAt(value, k);
    return *this;
}

template<class T>
Set<T> &Set<T>::remove(T value) {
    int k = search(value);
    if (value != get(k))
        return *this;
    data->remove(k);
    return *this;
}


template<class T>
bool Set<T>::contains(T value) const {
    return value == data->get(search(value));
}

template<class T>
bool Set<T>::contains(const Set &st) const {
    if (st.size() > size())
        return false;
    for (int i = 0; i < st.size(); i++)
        if (!contains(st.get(i)))
            return false;
    return true;
}


template<class T>
int Set<T>::size() const {
    return data->getSize();
}


template<class T>
Set<T> &Set<T>::unite(const Set &st) const {
    auto result = new Set;
    for (int i = 0; i < data->getSize(); i++) {
        result->add(data->get(i));
    }
    for (int i = 0; i < st.size(); i++) {
        result->add(st.get(i));
    }
    return *result;
}

template<class T>
Set<T> &Set<T>::operator|(const Set &st) const {
    return unite(st);
}

template<class T>
T Set<T>::get(int index) const {
    return data->get(index);
}

template<class T>
Set<T> &Set<T>::intersect(const Set &st) const {
    auto contains = size() >= st.size() ? *this : st;
    auto smaller = size() >= st.size() ? st : *this;
    auto result = new Set;
    for (int i = 0; i < smaller.size(); i++) {
        if (contains.contains(smaller.get(i))) result->add(smaller.get(i));
    }

    return *result;
}


template<class T>
Set<T> &Set<T>::operator&(const Set &st) const {
    return intersect(st);
}

template<class T>
Set<T> &Set<T>::subtract(const Set &st) const {
    auto result = new Set;
    for (int i = 0; i < size(); i++) {
        if (st.contains(get(i))) continue;
        result->add(get(i));
    }
    return *result;
}

template<class T>
Set<T> &Set<T>::operator-(const Set &st) const {
    return subtract(st);
}


template<class T>
bool Set<T>::operator>(const Set &st) const {
    return contains(st);
}

template<class T>
bool Set<T>::operator<(const Set &st) const {
    return st.contains(*this);
}

template<class T>
bool Set<T>::operator>=(const Set &st) const {
    return contains(st) or st == *this;
}

template<class T>
bool Set<T>::operator<=(const Set &st) const {
    return *this < st or st == *this;
}

template<class T>
Set<T> &Set<T>::map(T f(T)) {
    return *new Set(data->map(f));
}

template<class T>
Set<T> &Set<T>::where(bool f(T)) {
    return *new Set(data->where(f));
}


template<class T> // TODO ?
bool operator==(const Set<T> &set1, const Set<T> &set2) {
    return set1.size() == set2.size() and set1.contains(set2);
}

template<class T> // TODO ?
bool operator!=(const Set<T> &set1, const Set<T> &set2) {
    return !(set1 == set2);
}

template<class T>
Set<T>::operator bool() const {
    return size() > 0;
}

template<class T>
std::ostream &operator<<(std::ostream &os, const Set<T> &set) {
    os << "{";
    for (int i = 0; i < set.size() - 1; ++i) {
        os << set.get(i) << ", ";
    }
    if (set.size() > 0) os << set.get(set.size() - 1);
    os << "}";
    return os;
}

template<class T>
Set<T> &Set<T>::clear() {
    delete data;
    data = new MutableArraySequence<T>;
    return *this;
}
