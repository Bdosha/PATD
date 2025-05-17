#pragma once
#include <stdexcept>


template<class T>
void LinkedList<T>::init() {
    size = 0;
    head = nullptr;
    tail = nullptr;
}

template<class T>
void LinkedList<T>::remove() {
    if (head == nullptr) return;
    Node<T> *current = head;
    while (current->next != nullptr) {
        current = current->next;

        delete current->prev;
    }
    delete current;
}

template<class T>
bool LinkedList<T>::check_index(const int index) const {
    return index < 0 || index >= size;
}

template<class T>
LinkedList<T>::LinkedList() {
    init();
}

template<class T>
LinkedList<T>::~LinkedList() {
    remove();
}


template<class T>
LinkedList<T>::LinkedList(T *items, int count) {
    init();
    for (int i = 0; i < count; i++) {
        append(items[i]);
    }
}

template<class T>
LinkedList<T>::LinkedList(LinkedList &list) {
    init();
    for (int i = 0; i < list.getSize(); i++) {
        append(list[i]);
    }
}

template<class T>
T &LinkedList<T>::getFirst() {
    if (check_index(0)) throw std::out_of_range("index out of range");

    return head->data;
}

template<class T>
T &LinkedList<T>::getLast() {
    if (check_index(0)) throw std::out_of_range("index out of range");

    return tail->data;
}

template<class T>
Node<T> *LinkedList<T>::getP(int index) {
    if (check_index(index)) throw std::out_of_range("index out of range");

    Node<T> *current = head;
    for (int i = 0; i < index; i++) {
        current = current->next;
    }
    return current;
}

template<class T>
T &LinkedList<T>::get(const int index) {
    return getP(index)->data;
}

template<class T>
T &LinkedList<T>::operator[](const int index) {
    return get(index);
}


template<class T>
LinkedList<T> &LinkedList<T>::append(T item) {
    auto *newNode = new Node<T>(item);
    size++;

    if (size == 1) {
        head = newNode;
        tail = newNode;
        return *this;
    }


    newNode->prev = tail;

    tail->next = newNode;
    tail = newNode;
    return *this;
}

template<class T>
LinkedList<T> &LinkedList<T>::prepend(T item) {
    auto *newNode = new Node<T>(item);
    size++;

    if (size == 1) {
        head = newNode;
        tail = newNode;
        return *this;
    }


    newNode->next = head;

    head->prev = newNode;
    head = newNode;
    return *this;
}


template<class T>
LinkedList<T> &LinkedList<T>::getSubList(int startIndex, int endIndex) {
    if (check_index(startIndex) || check_index(endIndex) || startIndex > endIndex)
        throw std::out_of_range(
            "index out of range");

    Node<T> *current = getP(startIndex);
    T *newData = new T[endIndex - startIndex + 1];
    for (int i = startIndex; i <= endIndex; i++) {
        newData[i - startIndex] = current->data;
        current = current->next;
    }

    remove();
    init();

    for (int i = 0; i <= endIndex - startIndex; i++) {
        append(newData[i]);
    }
    delete []newData;
    return *this;
}

template<class T>
unsigned int LinkedList<T>::getSize() const {
    return size;
}


template<class T>
LinkedList<T> &LinkedList<T>::insertAt(T item, int index) {
    if (size == 0 and index == 0) append(item);
    if (index == size) append(item);
    if (check_index(index)) {
        throw std::out_of_range("index out of range");
    }
    Node<T> *Next = getP(index);
    Node<T> *Prev = Next->prev;
    auto *newNode = new Node<T>(item);
    Next->prev = newNode;
    newNode->next = Next;
    Prev->next = newNode;
    newNode->prev = Prev;
    size++;
    return *this;
}

template<class T>
LinkedList<T> &LinkedList<T>::concat(LinkedList<T> *list) {
    auto *newList = new LinkedList<T>();
    Node<T> *current = head;
    for (int i = 0; i < size; i++) {
        newList->append(current->data);
        current = current->next;
    }
    current = list->head;
    for (int i = 0; i < list->getSize(); i++) {
        newList->append(current->data);
        current = current->next;
    }
    return *newList;
}

template<class T>
LinkedList<T> &LinkedList<T>::set(int index, T value) {
    getP(index)->data = value;
    return *this;
}


template<class T>
int LinkedList<T>::find(T value) {
    Node<T> *current = head;
    for (int i = 0; i < size; i++) {
        if (current->data == value) return i;
        current = current->next;
    }
    return size;
}

template<class T>
bool LinkedList<T>::contains(T value) {
    return find(value) != size;
}


template<class T>
LinkedList<T> &LinkedList<T>::remove(int index) {
    size--;

    if (index == 0) {
        Node<T> *temp = head->next;
        delete head;
        head = temp;
        return *this;
    }
    if (index == size) {
        Node<T> *temp = tail->prev;
        delete tail;
        tail = temp;
        return *this;
    }
    Node<T> *current = getP(index);
    current->prev->next = current->next;
    current->next->prev = current->prev;
    delete current;
    return *this;
}
