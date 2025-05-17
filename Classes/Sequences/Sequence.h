#ifndef SEQUENCE_H
#define SEQUENCE_H

template<class T>
class Sequence {
public:
    virtual ~Sequence() = default;

    virtual T &getFirst() = 0;

    virtual T &getLast() =0;

    virtual T &get(int index) const =0;

    virtual Sequence *set(int index, T value) =0;

    virtual Sequence *inSet(int index, T item) =0;

    [[nodiscard]] virtual int getSize() const =0;

    virtual Sequence *append(T item) = 0;

    virtual Sequence *prepend(T item) =0;

    virtual Sequence *insertAt(T item, int index) =0;

    virtual Sequence *getSubsequence(int startIndex, int endIndex) =0;

    virtual Sequence *inGetSubsequence(int startIndex, int endIndex) =0;

    virtual Sequence *inAppend(T item) =0;

    virtual Sequence *inPrepend(T item) =0;

    virtual Sequence *inInsertAt(T item, int index) =0;

    virtual Sequence *construct() =0;

    virtual Sequence *remove(int) =0;

    virtual Sequence *inRemove(int) =0;


    virtual bool contains(T) =0;

    virtual int find(T) =0;

    Sequence *map(T F(T));

    Sequence *where(bool F(T));

    T reduce(T F(T, T), T start);

    Sequence *concat(Sequence *);

    // virtual void operator=

    virtual T &operator[](int index) = 0;

    //virtual T &operator[](const int &index) const =0;


    // char* toString();

protected:


private:
    virtual Sequence *clone() =0;

    virtual Sequence *instance() =0;
};

template<class T>
Sequence<T> *concat(Sequence<T> *s1, Sequence<T> *s2);


template<class T>
int len(Sequence<T> &seq);

template<class T>
std::ostream &operator<<(std::ostream &os, const Sequence<T> &seq);

template<class T>
bool operator==(const Sequence<T> &s1, const Sequence<T> &s2);

template<class T>
bool operator!=(const Sequence<T> &s1, const Sequence<T> &s2);


#include "Sequence.tpp"
#include "Array/ArraySequences.h"
#include "LinkedList/basic/ListSequence.h"
#endif
