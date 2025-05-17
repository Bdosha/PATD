#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "typeinfo.h"

typedef struct {
    unsigned int len;
    unsigned int cap;
    void *element;
    typeInfo *type;
} List;

List *createList(unsigned int len, typeInfo *type);

bool check_list(const List *list);

void swap(void *a, void *b, unsigned int size_of) ;

void *getElement(const List *l, unsigned int index);

void setElement(const List *l, const unsigned int index, const void *value) ;

void sort(const List *l) ;

void append(List *l, const void *value) ;

void delete_list(List *l) ;

List *concat(const List *l1, const List *l2);

void map(const List *l, void *(*f)(void *)) ;

void where(List *l, bool (*f)(void *));

void *reduce(const List *l, void *(*f)(void *, void *), void *x) ;