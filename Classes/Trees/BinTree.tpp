#pragma once

template<class T>
BinTree<T>::BinTree() {
    root = nullptr;
}

template<class T>
BinTree<T>::BinTree(BinTreeNode<T> *root) {
    this->root = root;
}

template<class T>
BinTree<T> &BinTree<T>::copy() {
    if (root == nullptr) return *new BinTree;
    return *new BinTree(root);
}

template<class T>
BinTree<T>::BinTree(const BinTree &other) {
    *this = other.copy();
}

template<class T>
BinTree<T>::BinTree(Sequence<T> *seq) {
    root = new BinTreeNode<T>(seq);
}

template<class T>
bool BinTree<T>::find(T data) const {
    if (root == nullptr) return false;
    return root->find(data);
}

template<class T>
int BinTree<T>::size() const {
    if (root == nullptr) return 0;
    return root->length;
}


template<class T>
BinTree<T> &BinTree<T>::insert(T data) {
    if (root == nullptr)
        root = new BinTreeNode<T>(data);
    else
        root->insert(data);
    return *this;
}

template<class T>
BinTree<T> &BinTree<T>::remove(T data) {
    if (root == nullptr) return *this;
    root->remove(data);
    return *this;
}

// template<class T>
// Sequence<T> *BinTree<T>::toSequence() const {
//     return root->toSequence(new ListSequence<T>);
// }

template<class T>
Sequence<T> *BinTree<T>::toSequence(const char rule[]) const {
    if (root == nullptr) return new MutableArraySequence<T>;
    return root->toSequence(new ListSequence<T>, rule);
}

template<class T>
BinTree<T> &BinTree<T>::map(T f(T)) {
    if (root == nullptr) return *this;
    // root->toSequence("PLR");
    auto seq = root->toSequence(new ListSequence<T>, "PLR");
    auto maped = seq->map(f);
    delete seq;
    auto nw = new BinTree(new BinTreeNode<T>(maped));
    delete maped;
    return *nw;
}

template<class T>
BinTree<T> &BinTree<T>::where(bool f(T)) {
    if (root == nullptr) return *this;
    // root->toSequence("PLR");
    auto seq = root->toSequence(new ListSequence<T>, "PLR");
    auto whered = seq->where(f);
    delete seq;
    auto nw = new BinTree(new BinTreeNode<T>(whered));
    delete whered;
    return *nw;
}


// template<class T>
// BinTree<T>::BinTree(BinTreeNode<T> *root) {
//     this->root = root;
//     if (root == nullptr) this->length = 0;
//     else length = root->length;
// }

template<class T>
BinTree<T> &BinTree<T>::merge(BinTree &other) {
    auto nw = new BinTree<T>(new BinTreeNode<T>(root));
    auto tmp = other.toSequence("PLR");
    for (int i = 0; i < tmp->getSize(); i++) {
        nw->insert(tmp->get(i));
    }
    return *nw;
}


template<class T>
BinTree<T> &BinTree<T>::getSubTree(T data) {
    auto current = root->findP(data);
    if (current == nullptr) return *new BinTree;

    return *new BinTree(new BinTreeNode<T>(current));
}



