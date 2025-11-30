#include "../Classes/Sequences/Lazy/LazySequence.h"
#include "../Classes/Sequences/Lazy/Generator.h"
#include "../Classes/Sequences/Array/ArraySequences.h"
#include "Testing.h"
#include <iostream>

int lazyPlus2(int a) {
    return a + 2;
}

bool isEven(int a) {
    return a % 2 == 0;
}

int square(int a) {
    return a * a;
}

void testLazySequenceBasic() {
    std::cout << "--- Проверка базовой функциональности LazySequence ---\n";

    int testData[] = {1, 2, 3, 4, 5};
    auto *seq = new MutableArraySequence<int>(testData, 5);
    auto *lazy = new LazySequence<int>(seq);

    testing("LazySequence создание", lazy->getSize() == 5);
    testing("LazySequence get(0)", lazy->get(0) == 1);
    testing("LazySequence get(4)", lazy->get(4) == 5);
    testing("LazySequence getFirst", lazy->getFirst() == 1);
    testing("LazySequence getLast", lazy->getLast() == 5);

    std::cout << std::endl;
}

void testLazyMap() {
    std::cout << "--- Проверка ленивого map ---\n";

    int testData[] = {1, 2, 3, 4, 5};
    auto *seq = new MutableArraySequence<int>(testData, 5);
    auto *lazy = new LazySequence<int>(seq);

    // Применяем map, но еще не вычисляем
    lazy->lazyMap(&lazyPlus2);

    // Теперь вычисляем - значения должны быть увеличены на 2
    testing("Lazy map get(0)", lazy->get(0) == 3);
    testing("Lazy map get(1)", lazy->get(1) == 4);
    testing("Lazy map get(4)", lazy->get(4) == 7);

    // Проверяем, что исходная последовательность не изменилась
    testing("Source unchanged", seq->get(0) == 1);

    std::cout << std::endl;
}

void testLazyWhere() {
    std::cout << "--- Проверка ленивого where ---\n";

    int testData[] = {1, 2, 3, 4, 5, 6, 7, 8};
    auto *seq = new MutableArraySequence<int>(testData, 8);
    auto *lazy = new LazySequence<int>(seq);

    // Фильтруем четные числа
    lazy->lazyWhere(&isEven);

    testing("Lazy where size", lazy->getSize() == 4);
    testing("Lazy where get(0)", lazy->get(0) == 2);
    testing("Lazy where get(1)", lazy->get(1) == 4);
    testing("Lazy where get(2)", lazy->get(2) == 6);
    testing("Lazy where get(3)", lazy->get(3) == 8);

    std::cout << std::endl;
}

void testLazyCombined() {
    std::cout << "--- Проверка комбинации ленивых операций ---\n";

    int testData[] = {1, 2, 3, 4, 5};
    auto *seq = new MutableArraySequence<int>(testData, 5);
    auto *lazy = new LazySequence<int>(seq);

    // Сначала фильтруем четные, потом применяем map
    lazy->lazyWhere(&isEven);
    lazy->lazyMap(&square);

    testing("Combined lazy size", lazy->getSize() == 2);
    testing("Combined lazy get(0)", lazy->get(0) == 4); // 2^2
    testing("Combined lazy get(1)", lazy->get(1) == 16); // 4^2

    std::cout << std::endl;
}

void testGeneratorRange() {
    std::cout << "--- Проверка генератора range ---\n";

    auto *gen = Generator<int>::range(0, 1);

    testing("Generator range get(0)", gen->get(0) == 0);
    testing("Generator range get(5)", gen->get(5) == 5);
    testing("Generator range get(10)", gen->get(10) == 10);
    testing("Generator range getSize", gen->getSize() == -1); // Бесконечная

    auto *gen2 = Generator<int>::range(10, 2);
    testing("Generator range(10, 2) get(0)", gen2->get(0) == 10);
    testing("Generator range(10, 2) get(1)", gen2->get(1) == 12);
    testing("Generator range(10, 2) get(2)", gen2->get(2) == 14);

    std::cout << std::endl;
}

void testGeneratorRepeat() {
    std::cout << "--- Проверка генератора repeat ---\n";

    auto *gen = Generator<int>::repeat(42);

    testing("Generator repeat get(0)", gen->get(0) == 42);
    testing("Generator repeat get(5)", gen->get(5) == 42);
    testing("Generator repeat get(100)", gen->get(100) == 42);
    testing("Generator repeat getSize", gen->getSize() == -1);

    std::cout << std::endl;
}

void testGeneratorWithLazy() {
    std::cout << "--- Проверка генератора с ленивыми операциями ---\n";

    auto *gen = Generator<int>::range(0, 1);
    Sequence<int> *seqGen = gen;
    auto *lazy = new LazySequence<int>(seqGen);

    lazy->lazyMap(&square);

    testing("Generator with lazy map get(0)", lazy->get(0) == 0); // 0^2
    testing("Generator with lazy map get(1)", lazy->get(1) == 1); // 1^2
    testing("Generator with lazy map get(5)", lazy->get(5) == 25); // 5^2
    testing("Generator with lazy map get(10)", lazy->get(10) == 100); // 10^2

    std::cout << std::endl;
}

void testLazyCache() {
    std::cout << "--- Проверка кэширования в ленивой последовательности ---\n";

    int testData[] = {1, 2, 3, 4, 5};
    auto *seq = new MutableArraySequence<int>(testData, 5);
    auto *lazy = new LazySequence<int>(seq);

    lazy->lazyMap(&plus2);

    // Первый вызов - вычисляется
    int val1 = lazy->get(0);
    // Второй вызов - должно быть из кэша
    int val2 = lazy->get(0);

    testing("Lazy cache consistency", val1 == val2 && val1 == 3);

    std::cout << std::endl;
}

void testLazyFromArray() {
    std::cout << "--- Проверка создания LazySequence из массива ---\n";

    int testData[] = {10, 20, 30};
    auto *lazy = new LazySequence<int>(testData, 3);

    testing("Lazy from array size", lazy->getSize() == 3);
    testing("Lazy from array get(0)", lazy->get(0) == 10);
    testing("Lazy from array get(2)", lazy->get(2) == 30);

    std::cout << std::endl;
}

void testLazyOperators() {
    std::cout << "--- Проверка операторов LazySequence ---\n";

    int testData[] = {1, 2, 3, 4, 5};
    auto *seq = new MutableArraySequence<int>(testData, 5);
    auto *lazy = new LazySequence<int>(seq);

    testing("Lazy operator[]", lazy->operator[](2) == 3);
    testing("Lazy operator[] negative", lazy->operator[](-1) == 5);

    std::cout << std::endl;
}

void test_lazy() {
    testLazySequenceBasic();
    testLazyMap();
    testLazyWhere();
    testLazyCombined();
    testGeneratorRange();
    testGeneratorRepeat();
    testGeneratorWithLazy();
    testLazyCache();
    testLazyFromArray();
    testLazyOperators();
}

