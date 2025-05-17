#ifndef LISTSEQUENCE_H
#define LISTSEQUENCE_H
#include "../capsule/LinkedList.h"
#include "../../Sequence.h"


template<class T>
class ListSequence final : public Sequence<T> {
public:
    explicit ListSequence(LinkedList<T>&);

    explicit ListSequence(Sequence<T> *);

    ListSequence();

    ListSequence(T *, int);

    ~ListSequence() override;

    T &getFirst() override;

    T &getLast() override;

    T &get(int)const override;

    int getSize()const override;

    Sequence<T> *append(T) override;

    Sequence<T> *prepend(T) override;

    Sequence<T> *insertAt(T, int) override;

    Sequence<T> *getSubsequence(int, int) override;

    Sequence<T> *inGetSubsequence(int, int) override;

    Sequence<T> *construct() override {
        return new ListSequence();;
    }

    Sequence<T> *remove(int) override;

    bool contains(T) override;

    int find(T) override;

    T &operator[](int index) override;


    Sequence<T> *set(int index, T value) override;

    LinkedList<T> *list;

private:
    Sequence<T> *inSet(int index, T item) override;

    void init();

    Sequence<T> *clone() override;

    ListSequence *instance() override;

    Sequence<T> *inAppend(T) override;

    Sequence<T> *inPrepend(T) override;

    Sequence<T> *inRemove(int) override;


    Sequence<T> *inInsertAt(T, int) override;
};

#include "ListSequence.tpp"
#endif
