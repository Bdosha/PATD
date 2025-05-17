#ifndef DYNAMICARRAY_H
#define DYNAMICARRAY_H

template<class T>
class DynamicArray {
public:
    DynamicArray(DynamicArray &other);

    DynamicArray(const T *items, int count);

    explicit DynamicArray(int size); // по стандартному значению

    DynamicArray();

    ~DynamicArray();

    T &get(int index) const;

    T &operator[](int index) const;

    unsigned int getSize() const;

    void set(int index, T value);

    DynamicArray& resize(unsigned int newSize);

    DynamicArray& remove(int);

    bool contains(T) const;

    int find(T) const;


    DynamicArray& insert(int index, T value);

    DynamicArray& append(T value);

    DynamicArray& prepend(T value);



private:
    T *items;
    unsigned int begin;
    unsigned int capacity;
    unsigned int size;


    void init(unsigned int size);

    void copy(const T *array, unsigned int count);

    bool check_index(int index) const;
};

#include "DynamicArray.tpp"
#endif
