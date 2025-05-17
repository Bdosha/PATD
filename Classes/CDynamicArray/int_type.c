#include <stdio.h>
#include "typeInfo.h"


int get_int(void *element) {
    return *(int *) element;
}

bool bigger_int(void *a, void *b) {
    return get_int(a) > get_int(b);
}

void print_int(void *element) {
    printf("%d ", get_int(element));
}

// void* sum_int(void*x1, void*x2) {
//     *(int *)x1 + *(int*)x2;
// }

typeInfo Int = {
    .size = sizeof(int),
    .print = print_int,
    .bigger = bigger_int
};
