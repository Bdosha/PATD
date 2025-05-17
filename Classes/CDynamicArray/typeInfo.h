#pragma once

typedef struct {
    unsigned int size;

    void (*print)(void *); //вывод в строку

    bool (*bigger)(void *, void *);
} typeInfo;
