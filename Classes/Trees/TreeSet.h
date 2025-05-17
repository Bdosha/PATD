#ifndef TREETreeSet_H
#define TREETreeSet_H
#include <ostream>

#include "BinTree.h"


template<class T>
class TreeSet {
    BinTree<T> *data;

public:
    TreeSet();

    explicit TreeSet(Sequence<T> *);

    // explicit TreeSet(BinTree<T> *);


    ~TreeSet() = default;

    Sequence<T> *toSequence() const;

    TreeSet &add(T);

    bool contains(T) const;

    bool contains(const TreeSet &) const;

    [[nodiscard]] int size() const;

    TreeSet &remove(T);

    TreeSet &unite(const TreeSet &) const;

    TreeSet &operator|(const TreeSet &) const;

    TreeSet &intersect(const TreeSet &) const;

    TreeSet &operator&(const TreeSet &) const;

    TreeSet &subtract(const TreeSet &) const;

    TreeSet &operator-(const TreeSet &) const;


    bool operator>(const TreeSet &) const;

    bool operator<(const TreeSet &) const;

    bool operator>=(const TreeSet &) const;

    bool operator<=(const TreeSet &) const;

    TreeSet &map(T (T));

    TreeSet &where(bool (T));

    explicit operator bool() const;

    TreeSet &clear();
};

template<class T>
bool operator==(const TreeSet<T> &TreeSet1, const TreeSet<T> &TreeSet2);

template<class T>
bool operator!=(const TreeSet<T> &TreeSet1, const TreeSet<T> &TreeSet2);

template<class T>
std::ostream &operator<<(std::ostream &os, const TreeSet<T> &TreeSet);



#include "TreeSet.tpp"
#endif //TREETreeSet_H
