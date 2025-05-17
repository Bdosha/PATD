#pragma once

#include "ListSequence.h"

template<class T>
void ListSequence<T>::init() {
    list = new LinkedList<T>();
}


template<class T>
ListSequence<T>::ListSequence(Sequence<T> *other) {
    list = new LinkedList<T>();
    for (int i = 0; i < other->getSize(); i++) {
        list->append(other->get(i));
    }
}


template<class T>
ListSequence<T>::ListSequence() {
    list = new LinkedList<T>();
}

template<class T>
ListSequence<T>::~ListSequence() {
    list->~LinkedList<T>();
}

template<class T>
ListSequence<T>::ListSequence(T *items, int count) {
    list = new LinkedList<T>(items, count);
}

template<class T>
ListSequence<T>::ListSequence(LinkedList<T> &l) {
    list = new LinkedList<T>(l);
}

template<class T>
T &ListSequence<T>::getFirst() {
    return list->getFirst();
}

template<class T>
T &ListSequence<T>::getLast() {
    return list->getLast();
}

template<class T>
ListSequence<T> *ListSequence<T>::instance() {
    return this;
}

template<class T>
Sequence<T> *ListSequence<T>::inAppend(T item) {
    list->append(item);
    return this;
}


template<class T>
Sequence<T> *ListSequence<T>::inPrepend(T item) {
    list->prepend(item);
    return this;
}

template<class T>
Sequence<T> *ListSequence<T>::inInsertAt(T item, int index) {
    list->insertAt(item, index);
    return this;
}

template<class T>
Sequence<T> *ListSequence<T>::append(T item) {
    return instance()->inAppend(item);
}


template<class T>
Sequence<T> *ListSequence<T>::prepend(T item) {
    return instance()->inPrepend(item);
}

template<class T>
Sequence<T> *ListSequence<T>::insertAt(T item, int index) {
    return instance()->inInsertAt(item, index);
}

template<class T>
int ListSequence<T>::getSize()const {
    return list->getSize();
}

template<class T>
T &ListSequence<T>::get(int index) const{
    return list->get(index);
}

template<class T>
Sequence<T> *ListSequence<T>::inGetSubsequence(int startIndex, int endIndex) {
    this->list->getSubList(startIndex, endIndex);
    return this;
}


template<class T>
Sequence<T> *ListSequence<T>::getSubsequence(int startIndex, int endIndex) {
    return instance()->inGetSubsequence(startIndex, endIndex);
}


template<class T>
Sequence<T> *ListSequence<T>::clone() {
    Sequence<T> *temp = new ListSequence(*this);

    return temp;
}

template<class T>
Sequence<T> *ListSequence<T>::inSet(int index, T item) {
    this->list->set(index, item);
    return this;
}


template<class T>
Sequence<T> *ListSequence<T>::set(int index, T value) {
    return instance()->inSet(index, value);
}


template<class T>
T &ListSequence<T>::operator[](int index) {
    if (-index > getSize()) throw std::out_of_range("index out of range");
    if (index < 0) index += getSize();
    return get(index);
}

template<class T>
bool ListSequence<T>::contains(T value) {
    return list->contains(value);
}

template<class T>
int ListSequence<T>::find(T value) {
    return list->find(value);
}

template<class T>
Sequence<T> *ListSequence<T>::remove(int index) {
    return instance()->inRemove(index);
}

template<class T>
Sequence<T> *ListSequence<T>::inRemove(int index) {
    this->list->remove(index);
    return this;
}
