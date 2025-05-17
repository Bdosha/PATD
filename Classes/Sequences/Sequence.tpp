#pragma once

template<class T>
Sequence<T> *concat(Sequence<T> *s1, Sequence<T> *s2) {
    Sequence<T> *result = s1->construct();
    for (int i = 0; i < s1->getSize(); i++) {
        result->inAppend(s1->get(i));
    }
    for (int i = 0; i < s2->getSize(); i++) {
        result->inAppend(s2->get(i));
    }
    return result;
}


template<class T>
Sequence<T> *Sequence<T>::concat(Sequence *seq) {
    Sequence *result = clone();

    int size = seq->getSize();
    for (int i = 0; i < size; i++) {
        result->inAppend(seq->get(i));
    }

    return result;
}

template<class T>
Sequence<T> *Sequence<T>::map(T F(T)) {
    Sequence *result = construct();
    for (int i = 0; i < getSize(); i++)
        result = result->append(F(get(i)));

    return result;
}

template<class T>
Sequence<T> *Sequence<T>::where(bool F(T)) {
    Sequence *result = construct();
    for (int i = 0; i < getSize(); i++)
        if (F(get(i)))
            result = result->append(get(i));

    return result;
}


template<class T>
T Sequence<T>::reduce(T F(T, T), T start) {
    if (getSize() == 0)
        return start;
    T result = F(get(0), start);

    for (int i = 1; i < getSize(); i++)
        result = F(get(i), result);

    return result;
}


template<class T>
int len(Sequence<T> &seq) {
    return seq.getSize();
}


template<class T>
std::ostream &operator<<(std::ostream &os, const Sequence<T> &seq) {
    os << "[";
    for (int i = 0; i < seq.getSize() - 1; ++i)
        os << seq.get(i) << ", ";

    if (seq.getSize() > 0)
        os << seq.get(seq.getSize() - 1);
    os << "]";
    return os;
}


template<class T>
bool operator==(const Sequence<T> &s1, const Sequence<T> &s2) {
    if (s1.getSize() != s2.getSize())
        return false;
    for (int i = 0; i < s1.getSize(); i++)
        if (s1.get(i) != s2.get(i))
            return false;

    return true;
}

template<class T>
bool operator!=(const Sequence<T> &s1, const Sequence<T> &s2) {
    return !(s1 == s2);
}
