#include "../Classes/Sequences/Lazy/LazySequences.h"
#include "../Classes/Sequences/Array/ArraySequences.h"
#include "Testing.h"
#include <iostream>
#include <cmath>

// Вспомогательные функции для тестов

// Рекуррентное правило для последовательности Фибоначчи
int fibonacciRule(Sequence<int> *history) {
    int size = history->getSize();
    if (size < 2) {
        return 1;
    }
    return history->get(size - 1) + history->get(size - 2);
}

// Рекуррентное правило для факториалов
int factorialRule(Sequence<int> *history) {
    int size = history->getSize();
    if (size == 0) {
        return 1;
    }
    return history->get(size - 1) * (size + 1);
}

// Рекуррентное правило для степеней двойки
int powerOfTwoRule(Sequence<int> *history) {
    int size = history->getSize();
    if (size == 0) {
        return 1;
    }
    return history->get(size - 1) * 2;
}

// Функция для map (умножение на 2)


// Функция для reduce (сумма)
int sum(int a, int b) {
    return a + b;
}

// Тесты для Cardinal
void testCardinal() {
    std::cout << "--- Проверка Cardinal ---\n";
    
    Cardinal c1(5);
    Cardinal c2(10);
    Cardinal inf = Cardinal::Infinity();
    
    testing("Cardinal finite value", c1.getValue() == 5);
    testing("Cardinal isInfinity false", !c1.isInfinity());
    testing("Cardinal isInfinity true", inf.isInfinity());
    
    bool exceptionThrown = false;
    try {
        (void)inf.getValue();  // явно игнорируем возвращаемое значение
    } catch (const std::runtime_error &) {
        exceptionThrown = true;
    }
    testing("Cardinal getValue throws for infinity", exceptionThrown);
    
    testing("Cardinal comparison <", c1 < c2);
    testing("Cardinal comparison >", c2 > c1);
    testing("Cardinal comparison finite < infinity", c1 < inf);
    testing("Cardinal comparison infinity == infinity", inf == Cardinal::Infinity());
    
    Cardinal c3 = c1 + c2;
    testing("Cardinal addition", c3.getValue() == 15);
    
    Cardinal c4 = c1 + inf;
    testing("Cardinal addition with infinity", c4.isInfinity());
    
    Cardinal c5 = c1 * c2;
    testing("Cardinal multiplication", c5.getValue() == 50);
    
    Cardinal c6 = c1 * inf;
    testing("Cardinal multiplication with infinity", c6.isInfinity());
    
    ++c1;
    testing("Cardinal increment", c1.getValue() == 6);
    
    std::cout << std::endl;
}

// Тесты для LazySequence - создание
void testLazySequenceCreation() {
    std::cout << "--- Проверка создания LazySequence ---\n";
    
    // Пустая последовательность
    LazySequence<int> *ls1 = new LazySequence<int>();
    testing("LazySequence empty creation", ls1->GetLength().getValue() == 0);
    delete ls1;
    
    // Из массива
    int arr[] = {1, 2, 3, 4, 5};
    LazySequence<int> *ls2 = new LazySequence<int>(arr, 5);
    testing("LazySequence from array length", ls2->GetLength().getValue() == 5);
    testing("LazySequence from array Get(0)", ls2->Get(0) == 1);
    testing("LazySequence from array Get(4)", ls2->Get(4) == 5);
    delete ls2;
    
    // Из Sequence
    Sequence<int> *seq = new MutableArraySequence<int>(arr, 5);
    LazySequence<int> *ls3 = new LazySequence<int>(seq);
    testing("LazySequence from Sequence length", ls3->GetLength().getValue() == 5);
    testing("LazySequence from Sequence Get(2)", ls3->Get(2) == 3);
    delete ls3;
    delete seq;
    
    // Рекуррентное правило (Фибоначчи)
    Sequence<int> *initial = new MutableArraySequence<int>();
    initial->inAppend(1);
    initial->inAppend(1);
    LazySequence<int> *ls4 = new LazySequence<int>(fibonacciRule, initial);
    testing("LazySequence recurrent is infinite", ls4->GetLength().isInfinity());
    testing("LazySequence recurrent Get(0)", ls4->Get(0) == 1);
    testing("LazySequence recurrent Get(1)", ls4->Get(1) == 1);
    testing("LazySequence recurrent Get(2)", ls4->Get(2) == 2);
    testing("LazySequence recurrent Get(3)", ls4->Get(3) == 3);
    testing("LazySequence recurrent Get(4)", ls4->Get(4) == 5);
    delete ls4;
    delete initial;
    
    std::cout << std::endl;
}

// Тесты для LazySequence - доступ к элементам
void testLazySequenceAccess() {
    std::cout << "--- Проверка доступа к элементам LazySequence ---\n";
    
    int arr[] = {10, 20, 30, 40, 50};
    LazySequence<int> *ls = new LazySequence<int>(arr, 5);
    
    testing("LazySequence GetFirst", ls->GetFirst() == 10);
    testing("LazySequence GetLast", ls->GetLast() == 50);
    testing("LazySequence Get(2)", ls->Get(2) == 30);
    testing("LazySequence operator[]", (*ls)[1] == 20);
    
    // Проверка мемоизации
    size_t count1 = ls->GetMaterializedCount();
    ls->Get(3);
    size_t count2 = ls->GetMaterializedCount();
    testing("LazySequence memoization", count2 >= count1);
    
    // Тест на исключение
    bool exceptionThrown = false;
    try {
        ls->Get(10);
    } catch (const std::out_of_range &) {
        exceptionThrown = true;
    }
    testing("LazySequence Get out of range", exceptionThrown);
    
    delete ls;
    
    std::cout << std::endl;
}

// Тесты для LazySequence - Append
void testLazySequenceAppend() {
    std::cout << "--- Проверка Append для LazySequence ---\n";
    
    // Конечная последовательность
    int arr[] = {1, 2, 3};
    LazySequence<int> *ls1 = new LazySequence<int>(arr, 3);
    LazySequence<int> *ls2 = ls1->Append(4);
    
    testing("LazySequence Append length", ls2->GetLength().getValue() == 4);
    testing("LazySequence Append last element", ls2->GetLast() == 4);
    testing("LazySequence Append original unchanged", ls1->GetLength().getValue() == 3);
    
    delete ls1;
    delete ls2;
    
    // Бесконечная последовательность
    Sequence<int> *initial = new MutableArraySequence<int>();
    initial->inAppend(1);
    LazySequence<int> *ls3 = new LazySequence<int>(powerOfTwoRule, initial);
    LazySequence<int> *ls4 = ls3->Append(999);
    
    testing("LazySequence Append to infinite", ls4->GetLength().isInfinity());
    testing("LazySequence Append to infinite first element", ls4->Get(0) == 1);
    
    delete ls3;
    delete ls4;
    delete initial;
    
    std::cout << std::endl;
}

// Тесты для LazySequence - Prepend и InsertAt
void testLazySequencePrependInsert() {
    std::cout << "--- Проверка Prepend и InsertAt для LazySequence ---\n";
    
    int arr[] = {2, 3, 4};
    LazySequence<int> *ls1 = new LazySequence<int>(arr, 3);
    LazySequence<int> *ls2 = ls1->Prepend(1);
    
    testing("LazySequence Prepend length", ls2->GetLength().getValue() == 4);
    testing("LazySequence Prepend first element", ls2->GetFirst() == 1);
    
    delete ls1;
    delete ls2;
    
    int arr2[] = {1, 3, 4};
    LazySequence<int> *ls3 = new LazySequence<int>(arr2, 3);
    LazySequence<int> *ls4 = ls3->InsertAt(2, 1);
    
    testing("LazySequence InsertAt length", ls4->GetLength().getValue() == 4);
    testing("LazySequence InsertAt element", ls4->Get(1) == 2);
    testing("LazySequence InsertAt next element", ls4->Get(2) == 3);
    
    delete ls3;
    delete ls4;
    
    std::cout << std::endl;
}

// Тесты для LazySequence - Concat
void testLazySequenceConcat() {
    std::cout << "--- Проверка Concat для LazySequence ---\n";
    
    // Конечные последовательности
    int arr1[] = {1, 2, 3};
    int arr2[] = {4, 5, 6};
    LazySequence<int> *ls1 = new LazySequence<int>(arr1, 3);
    LazySequence<int> *ls2 = new LazySequence<int>(arr2, 3);
    LazySequence<int> *ls3 = ls1->Concat(ls2);
    
    testing("LazySequence Concat length", ls3->GetLength().getValue() == 6);
    testing("LazySequence Concat first element", ls3->Get(0) == 1);
    testing("LazySequence Concat last element", ls3->Get(5) == 6);
    testing("LazySequence Concat middle element", ls3->Get(3) == 4);
    
    delete ls1;
    delete ls2;
    delete ls3;
    
    // Бесконечная + конечная
    Sequence<int> *initial = new MutableArraySequence<int>();
    initial->inAppend(1);
    LazySequence<int> *ls4 = new LazySequence<int>(powerOfTwoRule, initial);
    LazySequence<int> *ls5 = new LazySequence<int>(arr2, 3);
    LazySequence<int> *ls6 = ls4->Concat(ls5);
    
    testing("LazySequence Concat infinite + finite", ls6->GetLength().isInfinity());
    testing("LazySequence Concat infinite first element", ls6->Get(0) == 1);
    
    delete ls4;
    delete ls5;
    delete ls6;
    delete initial;
    
    std::cout << std::endl;
}

// Тесты для LazySequence - Map, Where, Reduce
void testLazySequenceFunctional() {
    std::cout << "--- Проверка функциональных операций LazySequence ---\n";
    
    int arr[] = {1, 2, 3, 4, 5};
    LazySequence<int> *ls1 = new LazySequence<int>(arr, 5);
    
    // Map
    LazySequence<int> *ls2 = ls1->Map(multiplyByTwo);
    testing("LazySequence Map length", ls2->GetLength().getValue() == 5);
    testing("LazySequence Map element", ls2->Get(0) == 2);
    testing("LazySequence Map element 2", ls2->Get(2) == 6);
    
    delete ls1;
    delete ls2;
    
    // Where
    int arr2[] = {1, 2, 3, 4, 5, 6, 7, 8};
    LazySequence<int> *ls3 = new LazySequence<int>(arr2, 8);
    LazySequence<int> *ls4 = ls3->Where(isEven);
    
    testing("LazySequence Where length", ls4->GetLength().getValue() == 4);
    testing("LazySequence Where first element", ls4->Get(0) == 2);
    testing("LazySequence Where second element", ls4->Get(1) == 4);
    
    delete ls3;
    delete ls4;
    
    // Reduce
    int arr3[] = {1, 2, 3, 4};
    LazySequence<int> *ls5 = new LazySequence<int>(arr3, 4);
    int result = ls5->Reduce(sum, 0);
    testing("LazySequence Reduce", result == 10);
    
    delete ls5;
    
    std::cout << std::endl;
}

// Тесты для LazySequence - GetSubsequence
void testLazySequenceSubsequence() {
    std::cout << "--- Проверка GetSubsequence для LazySequence ---\n";
    
    int arr[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    LazySequence<int> *ls1 = new LazySequence<int>(arr, 10);
    LazySequence<int> *ls2 = ls1->GetSubsequence(2, 5);
    
    testing("LazySequence GetSubsequence length", ls2->GetLength().getValue() == 4);
    testing("LazySequence GetSubsequence first", ls2->Get(0) == 2);
    testing("LazySequence GetSubsequence last", ls2->Get(3) == 5);
    
    delete ls1;
    delete ls2;
    
    std::cout << std::endl;
}

// Тесты для бесконечных последовательностей
void testInfiniteSequences() {
    std::cout << "--- Проверка бесконечных последовательностей ---\n";
    
    // Фибоначчи
    Sequence<int> *fibInit = new MutableArraySequence<int>();
    fibInit->inAppend(1);
    fibInit->inAppend(1);
    LazySequence<int> *fib = new LazySequence<int>(fibonacciRule, fibInit);
    
    testing("Infinite sequence length is infinity", fib->GetLength().isInfinity());
    testing("Infinite sequence Get(0)", fib->Get(0) == 1);
    testing("Infinite sequence Get(5)", fib->Get(5) == 8);
    testing("Infinite sequence Get(10)", fib->Get(10) == 89);
    
    // Проверка мемоизации
    size_t count1 = fib->GetMaterializedCount();
    fib->Get(15);
    size_t count2 = fib->GetMaterializedCount();
    testing("Infinite sequence memoization", count2 > count1);
    
    // Степени двойки
    Sequence<int> *powInit = new MutableArraySequence<int>();
    powInit->inAppend(1);
    LazySequence<int> *powers = new LazySequence<int>(powerOfTwoRule, powInit);
    
    testing("Powers of two Get(0)", powers->Get(0) == 1);
    testing("Powers of two Get(1)", powers->Get(1) == 2);
    testing("Powers of two Get(5)", powers->Get(5) == 32);
    testing("Powers of two Get(10)", powers->Get(10) == 1024);
    
    delete fib;
    delete powers;
    delete fibInit;
    delete powInit;
    
    std::cout << std::endl;
}

// Тесты для граничных случаев
void testLazyEdgeCases() {
    std::cout << "--- Проверка граничных случаев ---\n";
    
    // Пустая последовательность
    LazySequence<int> *ls1 = new LazySequence<int>();
    bool exceptionThrown = false;
    try {
        ls1->GetFirst();
    } catch (const std::out_of_range &) {
        exceptionThrown = true;
    }
    testing("Empty sequence GetFirst throws", exceptionThrown);
    
    delete ls1;
    
    // Один элемент
    int arr[] = {42};
    LazySequence<int> *ls2 = new LazySequence<int>(arr, 1);
    testing("Single element GetFirst == GetLast", ls2->GetFirst() == ls2->GetLast());
    delete ls2;
    
    // Отрицательный индекс
    int arr2[] = {1, 2, 3};
    LazySequence<int> *ls3 = new LazySequence<int>(arr2, 3);
    exceptionThrown = false;
    try {
        ls3->Get(-1);
    } catch (const std::out_of_range &) {
        exceptionThrown = true;
    }
    testing("Negative index throws", exceptionThrown);
    delete ls3;
    
    std::cout << std::endl;
}

// Главная функция тестирования
inline void mainLazyTest() {
    testCardinal();
    testLazySequenceCreation();
    testLazySequenceAccess();
    testLazySequenceAppend();
    testLazySequencePrependInsert();
    testLazySequenceConcat();
    testLazySequenceFunctional();
    testLazySequenceSubsequence();
    testInfiniteSequences();
    testLazyEdgeCases();
}

