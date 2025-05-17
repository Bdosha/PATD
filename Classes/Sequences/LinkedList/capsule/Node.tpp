#pragma once
template<class T>
Node<T>::Node() {
    prev = nullptr;
    next = nullptr;
}

template<class T>
Node<T>::Node(T data) {
    prev = nullptr;
    this->data = data;
    next = nullptr;
}
