#pragma once

template<class T>
ArraySequence<T>::ArraySequence(DynamicArray<T> &other) {
    list = new DynamicArray<T>(other);
}

template<class T>
ArraySequence<T>::ArraySequence(Sequence<T> *other) {
    list = new DynamicArray<T>(other->getSize());
    for (int i = 0; i < list->getSize(); i++) {
        list->set(i, other->get(i));
    }
}


template<class T>
ArraySequence<T>::ArraySequence() {
    list = new DynamicArray<T>;
}

template<class T>
ArraySequence<T>::ArraySequence::ArraySequence(T *items, int count) {
    list = new DynamicArray<T>(items, count);
}

template<class T>
ArraySequence<T>::ArraySequence(int count) {
    list = new DynamicArray<T>(count);
}

template<class T>
T &ArraySequence<T>::get(int index) const {
    return list->get(index);
}

template<class T>
T &ArraySequence<T>::getFirst() {
    return list->get(0);
}

template<class T>
T &ArraySequence<T>::getLast() {
    return list->get(list->getSize() - 1);
}

template<class T>
int ArraySequence<T>::getSize() const {
    return list->getSize();
}

template<class T>
Sequence<T> *ArraySequence<T>::inAppend(T item) {
    list->append(item);
    return this;
}


template<class T>
Sequence<T> *ArraySequence<T>::inInsertAt(T item, const int index) {
    list->insert(index, item);

    return this;
}

template<class T>
Sequence<T> *ArraySequence<T>::inPrepend(T item) {
    list->prepend(item);
    return this;
}

template<class T>
Sequence<T> *ArraySequence<T>::append(T item) {
    return instance()->inAppend(item);
}

template<class T>
Sequence<T> *ArraySequence<T>::prepend(T item) {
    return instance()->inPrepend(item);
}

template<class T>
Sequence<T> *ArraySequence<T>::insertAt(T item, int index) {
    return instance()->inInsertAt(item, index);
}


template<class T>
Sequence<T> *ArraySequence<T>::inGetSubsequence(int startIndex, int endIndex) {
    if (startIndex > endIndex) throw std::out_of_range("index out of range");

    auto *temp = new DynamicArray<T>(endIndex - startIndex + 1);
    for (int i = 0; i < temp->getSize(); i++) {
        temp->set(i, this->get(startIndex + i));
    }
    delete list;

    list = temp;

    return this;
}

template<class T>
Sequence<T> *ArraySequence<T>::getSubsequence(int startIndex, int endIndex) {
    return instance()->inGetSubsequence(startIndex, endIndex);
}


template<class T>
Sequence<T> *ArraySequence<T>::inSet(int index, T item) {
    this->list->set(index, item);
    return this;
}


template<class T>
Sequence<T> *ArraySequence<T>::set(int index, T value) {
    return instance()->inSet(index, value);
}

template<class T>
T &ArraySequence<T>::operator[](int index) {
    if (-index > getSize()) throw std::out_of_range("index out of range");
    if (index < 0) index += getSize();
    return get(index);
}


template<class T>
int ArraySequence<T>::find(T value) {
    return list->find(value);
}

template<class T>
Sequence<T> *ArraySequence<T>::remove(int index) {
    return instance()->inRemove(index);
}

template<class T>
bool ArraySequence<T>::contains(T value) {
    return list->contains(value);
}


template<class T>
Sequence<T> *ArraySequence<T>::inRemove(int index) {
    list->remove(index);
    return this;
}
