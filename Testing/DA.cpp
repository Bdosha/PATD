#include "../Classes/CDynamicArray/CynamicArray.h"
// #include "../Sequences/Sequence.h"

// template<class T>
// Sequence<T>* toSeq(List* list) {
//     auto a = new Sequence<T>();
//     for (int i = 0; i < list->len; i++) {
//         a->append(list->element[i])
//     }
// }

void print_C(const List *l) {
    if (check_list(l)) return;


    for (int i = 0; i < l->len; i++) {
        l->type->print(getElement(l, i));
    }
    printf("\n");
}


void *pwo(void *x) {
    int *y = (int *) malloc(sizeof(int));

    *y = *(int *) x * 2;
    return y;
}

bool filter(void *x) {
    return *(int *) x > 10;
}

void *sums(void *a, void *b) {
    int *result = (int *) malloc(sizeof(int));
    *result = *(int *) a + *(int *) b;;
    return result;
}

int test_C() {
    std::cout << "--- Проверка DynamicArray на С ---\n";
    // Связанный список

    // print(list);


    constexpr double x = 1, y = 2, z = 3;
    List *list_double = createList(0, &Double);
    append(list_double, &x);
    append(list_double, &y);
    append(list_double, &z);

    testing("Создание массива с double и append",
            *(double *) getElement(list_double, 0) == 1 and list_double->len == 3);

    List *list2 = createList(0, &Int);


    for (int i = 10; i > 0; i--) append(list2, &i);

    sort(list2);

    testing("Создание массива с int и сортировка", *(int *) getElement(list2, 0) == 1 and list2->len == 10);


    map(list2, &pwo);
    testing("Map", *(int *) getElement(list2, 3) == 8);


    where(list2, &filter);
    testing("Where", *(int *) getElement(list2, 0) > 10);

    int zero = 0;
    int *ans = (int *) reduce(list2, &sums, &zero);
    testing("Reduce", *ans == 80);

    free(ans);

    List *list1 = createList(0, &Int);
    const int ad = 8, bd = 2, cd = 3;
    append(list1, &ad);
    append(list1, &bd);
    append(list1, &cd);



    List *list3 = concat(list1, list2);


    testing("Concat", list3->len == 8);

    delete_list(list1);
    delete_list(list2);
    delete_list(list3);
    return 0;
}
