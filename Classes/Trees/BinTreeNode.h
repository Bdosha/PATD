#ifndef BINTREENODE_H
#define BINTREENODE_H
#include "../Sequences/Sequence.h"


template<class T>
struct BinTreeNode {
    T data;
    int length;
    BinTreeNode *left;
    BinTreeNode *right;
    BinTreeNode *parent;

    BinTreeNode();

    explicit BinTreeNode(T data);

    BinTreeNode(T data, BinTreeNode *parent);

    explicit BinTreeNode(Sequence<T> *);

    explicit BinTreeNode(BinTreeNode *);

    BinTreeNode *insert(T data);

    const BinTreeNode *findP(T data) const;

    BinTreeNode *findP(T data);

    bool find(T data) const;

    T getMin() const;

    T getMax() const;

    BinTreeNode *remove(T data);


    Sequence<T> *toSequence(Sequence<T> *sequence = new ListSequence<int>, const char *rule = "LPR") const;

    BinTreeNode *copy() const;
};

#include "BinTreeNode.tpp"

#endif
