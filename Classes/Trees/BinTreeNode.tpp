#pragma once

template<class T>
BinTreeNode<T>::BinTreeNode() {
    left = nullptr;
    right = nullptr;
    parent = nullptr;
    length = 1;
}


template<class T>
BinTreeNode<T>::BinTreeNode(T data) {
    this->data = data;
    this->left = nullptr;
    this->right = nullptr;
    this->parent = nullptr;
    this->length = 1;
    // this->L = false;
}

template<class T>
BinTreeNode<T>::BinTreeNode(T data, BinTreeNode *parent) {
    this->data = data;
    this->left = nullptr;
    this->right = nullptr;
    this->parent = parent;
    this->length = 1;
    // this->L = false;
}

template<class T>
BinTreeNode<T>::BinTreeNode(Sequence<T> *seq) {
    data = seq->get(0);
    length = 1;
    left = nullptr;
    right = nullptr;
    parent = nullptr;
    for (int i = 1; i < seq->getSize(); i++)
        insert(seq->get(i));
}

template<class T>
BinTreeNode<T>::BinTreeNode(BinTreeNode *root) {
    *this = *root->copy();
}

template<class T>
BinTreeNode<T> *BinTreeNode<T>::insert(T data) {
    if (data == this->data) return nullptr;


    if (data < this->data) {
        if (left == nullptr) {
            left = new BinTreeNode(data, this);
            ++length;
        } else if (left->insert(data) != nullptr) {
            ++length;
        }
    } else {
        if (right == nullptr) {
            right = new BinTreeNode(data, this);
            ++length;
        } else if (right->insert(data) != nullptr) {
            ++length;
        }
    }
    return this;
    // return nullptr;
}


template<class T>
const BinTreeNode<T> *BinTreeNode<T>::findP(T data) const {
    if (data == this->data) return this;
    if (data < this->data && left != nullptr) return left->findP(data);
    if (data > this->data && right != nullptr) return right->findP(data);
    return nullptr;
}

// Неконстантная версия
template<class T>
BinTreeNode<T> *BinTreeNode<T>::findP(T data) {
    return const_cast<BinTreeNode<T>*>(
        static_cast<const BinTreeNode<T>*>(this)->findP(data)
    );
}

template<class T>
bool BinTreeNode<T>::find(T data) const {
    return findP(data) != nullptr;
}


template<class T>
void updateLength(BinTreeNode<T> *parent) {
    while (parent != nullptr) {
        --parent->length;
        parent = parent->parent;
    }
}

template<class T>
T BinTreeNode<T>::getMin() const {
    const BinTreeNode *current = this;
    while (current->left != nullptr) current = current->left;
    return current->data;
}


template<class T>
T BinTreeNode<T>::getMax() const {
    BinTreeNode<T> *current = this;
    while (current->left != nullptr) current = current->right;
    return current->data;
}

template<class T>
BinTreeNode<T> *BinTreeNode<T>::remove(T data) {
    BinTreeNode *current = findP(data);
    if (current == nullptr) return this;

    // if (current == root) {
    //     delete root;
    //     return nullptr;
    // }

    if (current->left == nullptr && current->right == nullptr) {
        current->data < current->parent->data ? current->parent->left = nullptr : current->parent->right = nullptr;
        updateLength(current->parent);
        delete current;
    }
    /*--------------*/
    else if (current->left == nullptr or current->right == nullptr) {
        BinTreeNode<T> *parent = current->parent;
        BinTreeNode<T> *child = current->left != nullptr ? current->left : current->right;
        child->parent = parent;
        child->data < parent->data ? parent->left = child : parent->right = child;
        updateLength(parent);
        delete current;
    } else {
        current->data = current->right->getMin();
        current->right->remove(current->data);
    }
    return this;
}


template<class T>
Sequence<T> *BinTreeNode<T>::toSequence(Sequence<T> *sequence, const char rule[]) const {
    if (rule == nullptr)
        return nullptr;

    int r = 0, l = 0, p = 0;
    for (int i = 0; i < 3; i++) {
        if (rule[i] == 'R') r++;
        if (rule[i] == 'L') l++;
        if (rule[i] == 'P') p++;
    }
    if (r != 1 or l != 1 or p != 1)
        throw std::invalid_argument("rule must contain only LPR permunation");


    for (int i = 0; i < 3; i++) {
        if (rule[i] == 'R' and right)
            right->toSequence(sequence, rule);
        if (rule[i] == 'L' and left)
            left->toSequence(sequence, rule);
        if (rule[i] == 'P')
            sequence->append(data);
    }
    return sequence;
}


template<class T>
BinTreeNode<T> *BinTreeNode<T>::copy() const {
    auto tmp = toSequence(new ListSequence<T>, "PLR");
    auto copy = new BinTreeNode<T>(tmp);
    delete tmp;
    return copy;
}
