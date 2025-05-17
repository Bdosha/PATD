#ifndef BINTREE_H
#define BINTREE_H
#include "../Sequences/Sequence.h"

#include "BinTreeNode.h"

template<class T>
class BinTree {
    BinTreeNode<T> *root;

public:
    BinTree();

    BinTree(const BinTree<T> &other);

    BinTree(Sequence<T>* seq);

    // Sequence<T> *toSequence() const;

    Sequence<T> *toSequence(const char *rule = "LPR") const;


    BinTree &insert(T data);

    BinTree &remove(T data);

    bool find(T data) const;

    BinTree &map(T f(T));

    BinTree &where(bool f(T));

    [[nodiscard]] int size() const;

    BinTree& merge(BinTree &other);

    BinTree &getSubTree(T data);

    BinTree& copy();


private:
    explicit BinTree(BinTreeNode<T> *root);
};

#include "BinTree.tpp"

#endif
