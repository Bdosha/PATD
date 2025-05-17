#pragma once
#include <stdexcept>


template<class T>
DynamicArray<T>::DynamicArray() {
    size = 0;
    capacity = 1;
    begin = 0;
    items = new T[size];
}

template<class T>
void DynamicArray<T>::init(unsigned int size) {
    if (size == 0) {
        this->size = 0;
        capacity = 1;
        begin = 0;
        items = new T[size];

        return;
    }
    this->size = size;
    this->capacity = size / 2 * 4;
    begin = size / 2;

    // items = new T[size];
    items = (T *) malloc(capacity * sizeof(T));
}


template<class T>
void DynamicArray<T>::copy(const T *array, const unsigned int count) {
    for (int i = 0; i < count; i++) items[i + begin] = array[i];
}

template<class T>
unsigned int DynamicArray<T>::getSize() const {
    return this->size;
}

template<class T>
DynamicArray<T>::DynamicArray(const int size) {
    init(size);
}

template<class T>
DynamicArray<T>::~DynamicArray() {
    delete[] items;
}

template<class T>
DynamicArray<T>::DynamicArray(const T *items, const int count) {
    init(count);
    copy(items, count);
}

template<class T>
DynamicArray<T>::DynamicArray(DynamicArray &other) {
    int size = other.getSize();

    init(size);
    for (int i = 0; i < size; i++) {
        get(i) = other[i];
    }
}


template<class T>
bool DynamicArray<T>::check_index(const int index) const {
    return index < 0 || index >= size;
}


template<class T>
T &DynamicArray<T>::get(int index) const {
    if (check_index(index)) throw std::out_of_range("index out of range");

    return items[index + begin];
}


template<class T>
T &DynamicArray<T>::operator[](const int index) const {
    return get(index);
}

// template<class T>
// T DynamicArray<T>::operator[](int& index) const {
//     return get(index);
// }


template<class T>
void DynamicArray<T>::set(int index, T value) {
    if (check_index(index)) throw std::out_of_range("index out of range");
    get(index) = value;
}


template<class T>
DynamicArray<T> &DynamicArray<T>::resize(const unsigned int newSize) {
    if (newSize + begin <= capacity && newSize + begin >= size) {
        size = newSize;
        return *this;
    }
    T *oldItems = items;
    int oldBegin = begin;
    int temp = newSize < size ? newSize : size;

    init(newSize);

    for (int i = 0; i < temp; ++i)
        items[i + begin] = oldItems[i + oldBegin];

    delete[] oldItems;

    return *this;
}


template<class T>
int DynamicArray<T>::find(T value) const {
    for (int i = 0; i < size; i++) {
        if (get(i) == value) return i;
    }
    return size;
}

template<class T>
bool DynamicArray<T>::contains(T value) const {
    return find(value) != size;
}


template<class T>
DynamicArray<T> &DynamicArray<T>::remove(int index) {
    auto newData = (T *) malloc((size - 1) * sizeof(T));


    int flag = 0;
    for (int i = 0; i < size; i++) {
        if (i == index) {
            flag = 1;
            continue;
        }
        newData[i - flag] = get(i);
    }
    delete items;
    begin = 0;
    size--;
    items = newData;
    resize(size);
    return *this;
}

template<class T>
DynamicArray<T> &DynamicArray<T>::append(T value) {
    if (begin + size + 1 > capacity) resize(size + 1);
    else size++;
    get(size - 1) = value;
    return *this;
}

template<class T>
DynamicArray<T> &DynamicArray<T>::prepend(T value) {
    if (size == 0) return append(value);
    if (size == 1) {
        append(value);
        items[1] = items[0];
        items[0] = value;
        return *this;
    }
    resize(size + 1);
    begin--;
    items[begin] = value;
    return *this;
}

template<class T>
DynamicArray<T> &DynamicArray<T>::insert(int index, T value) {
    if (check_index(index - 1) and check_index(index)) throw std::out_of_range("index out of range");
    if (index == size) append(value);
    else if (index == 0) prepend(value);
    else if (index + 1 + begin > capacity / 2) {
        append(T());
        T temp = get(index);
        for (int i = index; i < size - 1; i++) {
            get(i) = value;
            value = temp;
            temp = get(i + 1);
        }
        get(size - 1) = value;
        // temp = get(1);
        // get(1) = value;
        // get(0) = temp;
    } else {
        T temp = get(index - 1);
        for (int i = index - 1; i > 0; i--) {
            get(i) = value;
            value = temp;
            temp = get(i - 1);
        }
        prepend(T());
        temp = get(1);
        get(1) = value;
        get(0) = temp;
        // std::cout << temp << value << std::endl;
        // get(0) = temp;
    }
    return *this;
}
