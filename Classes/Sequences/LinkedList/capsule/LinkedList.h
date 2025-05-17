#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include "Node.h"

template<class T>
class LinkedList {
public:
    LinkedList();

    LinkedList(T *items, int count);

    LinkedList(LinkedList &list); // константная ссылка

    ~LinkedList();

    T &getFirst();

    T &getLast();

    T &get(int index);

    T &operator[](int index);


    LinkedList &getSubList(int startIndex, int endIndex);

    unsigned int getSize() const;

    LinkedList &append(T item);

    LinkedList &prepend(T item);

    LinkedList &insertAt(T item, int index);

    LinkedList &concat(LinkedList *list);

    LinkedList &set(int index, T value);

    LinkedList &remove(int);

    bool contains(T);

    int find(T);

private:
    Node<T> *head;
    Node<T> *tail;
    unsigned int size{};

    Node<T> *getP(int index);

    bool check_index(int index) const;

    void init();

    void remove();
};

#include "LinkedList.tpp"
#endif
