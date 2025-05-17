#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "typeInfo.h"

typedef struct {
    unsigned int len;
    unsigned int cap;
    void *element;
    typeInfo *type;
} List;

List *createList(unsigned int len, typeInfo *type) {
    unsigned int cap;
    if (len == 0) cap = 1;
    else cap = (unsigned int) pow(2, log2((float) len) + 1);
    void *list = malloc(type->size * cap);

    List *l = (List *) malloc(sizeof(List));

    l->element = list;
    l->len = len;
    l->cap = cap;
    l->type = type;
    return l;
}


bool check_list(const List *list) {
    // if (list->len == 0) return true;
    return !(list != NULL && (list->element != NULL));
}

void swap(void *a, void *b, unsigned int size_of) {
    void *tmp = malloc(size_of);
    memcpy(tmp, a, size_of);
    memcpy(a, b, size_of);
    memcpy(b, tmp, size_of);
    free(tmp);
}


void *getElement(const List *l, unsigned int index) {
    if (l == NULL || index > l->len) return NULL;

    void *elem = (char *) l->element + index * l->type->size;
    return elem;
}

void setElement(const List *l, const unsigned int index, const void *value) {
    if (check_list(l)) return;
    if (index > l->len || index < 0) return;

    void *elem = (char *) l->element + index * l->type->size;
    memcpy(elem, value, l->type->size);
}

void sort(const List *l) {
    if (check_list(l)) return;
    for (int i = 0; i < l->len; i++) {
        for (int j = i + 1; j < l->len; j++) {
            if (l->type->bigger(getElement(l, i), getElement(l, j))) {
                swap(getElement(l, i), getElement(l, j), l->type->size);
            }
        }
    }
}


void append(List *l, const void *value) {
    if (check_list(l)) return;

    if (value == NULL) return;

    if (l->len + 1 == l->cap) {
        l->cap *= 2;
        l->element = realloc(l->element, l->cap * l->type->size); //маллок
    }

    setElement(l, l->len, value);
    l->len++;
}


void delete_list(List *l) {
    if (l != NULL) free(l); // проверка листа
}


List *concat(const List *l1, const List *l2) {
    if (check_list(l1) || check_list(l2)) return NULL;

    if (l1->type != l2->type) return NULL;
    List *l = createList(l1->len + l2->len, l1->type);
    for (int i = 0; i < l1->len; i++) {
        setElement(l, i, getElement(l1, i));
    }
    for (int i = 0; i < l2->len; i++) {
        setElement(l, i + l1->len, getElement(l2, i));
    }
    return l;
}


void map(const List *l, void *(*f)(void *)) {
    if (check_list(l)) return;
    if (f == NULL) return;

    for (int i = 0; i < l->len; i++) {
        void *answer = f(getElement(l, i));
        setElement(l, i, answer);
        free(answer);
    }
}

void where(List *l, bool (*f)(void *)) {
    if (check_list(l)) return;
    if (f == NULL) return;

    List *tmp = createList(0, l->type);
    for (int i = 0; i < l->len; i++) {
        //printf("%d < %d = %d", i, l->len, i < l->len);
        void *value = getElement(l, i);

        if (f(value)) {
            append(tmp, value);
        }
    }
    *l = *tmp;
    free(tmp);
}


void *reduce(const List *l, void *(*f)(void *, void *), void *x) {
    if (check_list(l)) return NULL;

    if (f == NULL || l->len < 1) { return NULL; }

    void *answer = f(getElement(l, 0), x);
    void *tmp = malloc(l->type->size);
    memcpy(tmp, answer, l->type->size);

    free(answer);

    for (int i = 1; i < l->len; i++) {
        answer = f(getElement(l, i), tmp);
        memcpy(tmp, answer, l->type->size);
        free(answer);
    }

    return tmp;
}
