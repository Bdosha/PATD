#ifndef NODE_H
#define NODE_H

template<class T>
class Node {
public:
    T data;
    Node *next;
    Node *prev;

    Node();

    explicit Node(T data);
};

#include "Node.tpp"
#endif
