#pragma once


template<class T>
Deque<T>::Deque() {
    data = new ListSequence<T>();
}

template<class T>
Deque<T>::Deque(T *items, int size) {
    data = new ListSequence<T>(items, size);
}

template<class T>
Deque<T>::Deque(Sequence<T> *seq) {
    data = new ListSequence<T>(seq);
}

template<class T>
Sequence<T> *Deque<T>::toSequence() const {
    return new ListSequence<T>(data);
}


template<class T>
Deque<T>::Deque(const Deque &q) {
    Sequence<T> *temp = q.toSequence();
    data = new ListSequence<T>();
    for (int i = 0; i < temp->getSize(); i++) {
        data->append(temp->get(i));
    }
}


template<class T>
Deque<T>::~Deque() {
    delete data;
}

template<class T>
int Deque<T>::size() const {
    return data->getSize();
}

template<class T>
Deque<T> &Deque<T>::push_back(T item) {
    data->append(item);
    return *this;
}

template<class T>
Deque<T> &Deque<T>::push_front(T item) {
    data->prepend(item);
    return *this;
}


template<class T>
T Deque<T>::back() const {
    return data->getLast();
}

template<class T>
T Deque<T>::front() const {
    return data->getFirst();
}


template<class T>
Deque<T> &Deque<T>::pop_front() {
    if (data->getSize() == 0) return *this;
    if (data->getSize() == 1) {
        delete data;
        data = new ListSequence<T>();
        return *this;
    }
    data->getSubsequence(1, data->getSize() - 1);
    return *this;
}

template<class T>
Deque<T> &Deque<T>::pop_back() {
    if (data->getSize() == 0) return *this;
    if (data->getSize() == 1) {
        delete data;
        data = new ListSequence<T>();
        return *this;
    }

    data->getSubsequence(0, data->getSize() - 2);
    return *this;
}


template<class T>
Deque<T> &Deque<T>::map(T F(T)) const {
    return *new Deque(toSequence()->map(F));
}

template<class T>
Deque<T> &Deque<T>::where(bool F(T)) const {
    return *new Deque(toSequence()->where(F));
}

template<class T>
Deque<T> &Deque<T>::concat(Deque &q) const {
    auto result = data->concat(q.toSequence());
    auto temp = Deque(result);

    return *new Deque(result);
}

template<class T>
T Deque<T>::reduce(T F(T, T), T start) const {
    return data->reduce(F, start);
}

template<class T>
Deque<T> &Deque<T>::subDeque(int start, int end) const {
    return *new Deque(data->getSubsequence(start, end));
}

template<class T>
Deque<T>::operator bool() const {
    return data->getSize() > 0;
}


template<class T>
bool operator==(const Deque<T> &d1, const Deque<T> &d2) {
    return *d1.toSequence() == *d2.toSequence();
}

template<class T>
bool operator!=(const Deque<T> &d1, const Deque<T> &d2) {
    return !(d1 == d2);
}

template<class T>
std::ostream &operator<<(std::ostream &os, const Deque<T> &d) {
    os << d.toSequence();
    return os;
}
