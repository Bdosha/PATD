#include <stdio.h>


double get_double(void *element) {
    return *(double *) element;
}

bool bigger_double(void *a, void *b) {
    return get_double(a) > get_double(b);
}

void print_double(void *element) {
    printf("%f ", get_double(element));
}

typeInfo Double = {
    .size = sizeof(double),
    .print = print_double,
    .bigger = bigger_double
};
