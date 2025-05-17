#pragma once
#include "TreeSet.h"
template<class T>

TreeSet<T>::TreeSet() {
    data = new BinTree<T>;
}

template<class T>
TreeSet<T>::TreeSet(Sequence<T> *seq) {
    data = new BinTree<T>(seq);
}


template<class T>
Sequence<T> *TreeSet<T>::toSequence() const {
    return data->toSequence();
}



template<class T>
TreeSet<T> &TreeSet<T>::add(T value) {
    data->insert(value);
    return *this;
}

template<class T>
TreeSet<T> &TreeSet<T>::remove(T value) {
    data->remove(value);
    return *this;
}


template<class T>
bool TreeSet<T>::contains(T value) const {
    return data->find(value);
}

template<class T>
bool TreeSet<T>::contains(const TreeSet &st) const {
    if (st.size() > size())
        return false;

    auto tmp = st.toSequence();
    for (int i = 0; i < st.size(); i++)
        if (!contains(tmp->get(i)))
            return false;
    return true;
}


template<class T>
int TreeSet<T>::size() const {
    return data->size();
}


template<class T>
TreeSet<T> &TreeSet<T>::unite(const TreeSet &st) const {
    auto result = new TreeSet;
    auto tmp = toSequence();

    for (int i = 0; i < size(); i++) {
        result->add(tmp->get(i));
    }
    auto tmp2 = st.toSequence();

    for (int i = 0; i < st.size(); i++) {
        result->add(tmp->get(i));
    }
    return *result;
}

template<class T>
TreeSet<T> &TreeSet<T>::operator|(const TreeSet &st) const {
    return unite(st);
}


template<class T>
TreeSet<T> &TreeSet<T>::intersect(const TreeSet &st) const {
    auto contains = size() >= st.size() ? *this : st;
    auto smaller = size() >= st.size() ? st : *this;
    auto result = new TreeSet;
    auto tmp = smaller.toSequence();

    for (int i = 0; i < tmp->getSize(); i++) {
        if (contains.contains(tmp->get(i))) result->add(tmp->get(i));
    }

    return *result;
}


template<class T>
TreeSet<T> &TreeSet<T>::operator&(const TreeSet &st) const {
    return intersect(st);
}

template<class T>
TreeSet<T> &TreeSet<T>::subtract(const TreeSet &st) const {
    auto result = new TreeSet;
    auto tmp = toSequence();

    for (int i = 0; i < size(); i++) {
        if (st.contains(tmp->get(i))) continue;
        result->add(tmp->get(i));
    }
    return *result;
}

template<class T>
TreeSet<T> &TreeSet<T>::operator-(const TreeSet &st) const {
    return subtract(st);
}


template<class T>
bool TreeSet<T>::operator>(const TreeSet &st) const {
    return contains(st);
}

template<class T>
bool TreeSet<T>::operator<(const TreeSet &st) const {
    return st.contains(*this);
}

template<class T>
bool TreeSet<T>::operator>=(const TreeSet &st) const {
    return contains(st) or st == *this;
}

template<class T>
bool TreeSet<T>::operator<=(const TreeSet &st) const {
    return *this < st or st == *this;
}

template<class T>
TreeSet<T> &TreeSet<T>::map(T f(T)) {
    return *new TreeSet(data->map(f));
}

template<class T>
TreeSet<T> &TreeSet<T>::where(bool f(T)) {
    return *new TreeSet(data->where(f));
}


template<class T>
bool operator==(const TreeSet<T> &TreeSet1, const TreeSet<T> &TreeSet2) {
    return TreeSet1.size() == TreeSet2.size() and TreeSet1.contains(TreeSet2);
}

template<class T>
bool operator!=(const TreeSet<T> &TreeSet1, const TreeSet<T> &TreeSet2) {
    return !(TreeSet1 == TreeSet2);
}

template<class T>
TreeSet<T>::operator bool() const {
    return size() > 0;
}

template<class T>
std::ostream &operator<<(std::ostream &os, const TreeSet<T> &TreeSet) {
    os << "{";
    auto tmp = TreeSet.toSequence();

    for (int i = 0; i < TreeSet.size() - 1; ++i) {
        os << tmp->get(i) << ", ";
    }
    if (TreeSet.size() > 0) os << tmp->get(TreeSet.size() - 1);
    os << "}";
    return os;
}

template<class T>
TreeSet<T> &TreeSet<T>::clear() {
    delete data;
    data = new BinTree<T>;
    return *this;
}
