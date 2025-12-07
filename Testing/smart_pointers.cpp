#include "../Classes/SmartPointers/SmartPointers.h"
#include "../Classes/Sequences/Array/ArraySequences.h"
#include "../Classes/Sequences/LinkedList/basic/ListSequence.h"
#include <iostream>

class Base {
public:
    int value;
    Base(int v) : value(v) {}
    virtual ~Base() = default;
    virtual int getValue() const { return value; }
};

class Derived : public Base {
public:
    int extra;
    Derived(int v, int e) : Base(v), extra(e) {}
    int getValue() const override { return value + extra; }
};

void testSmrtPtrBasic() {
    std::cout << "--- Проверка SmrtPtr (базовые операции) ---\n";

    SmrtPtr<int> ptr1 = makeSmrt<int>(42);
    testing("SmrtPtr create", *ptr1 == 42);
    testing("SmrtPtr useCount", ptr1.useCount() == 1);

    {
        SmrtPtr<int> ptr2 = ptr1;
        testing("SmrtPtr copy constructor", ptr2.useCount() == 2);
        testing("SmrtPtr equality", ptr1 == ptr2);
    }

    testing("SmrtPtr after scope", ptr1.useCount() == 1);

    SmrtPtr<int> ptr3 = std::move(ptr1);
    testing("SmrtPtr move constructor", ptr3.useCount() == 1);
    testing("SmrtPtr moved from", !ptr1);

    SmrtPtr<int> ptr4;
    ptr4 = ptr3;
    testing("SmrtPtr assignment", ptr4.useCount() == 2);

    ptr4.reset();
    testing("SmrtPtr reset", !ptr4 && ptr3.useCount() == 1);

    std::cout << std::endl;
}

void testSmrtPtrSubtyping() {
    std::cout << "--- Проверка SmrtPtr (подтипизация) ---\n";

    SmrtPtr<Derived> derivedPtr = makeSmrt<Derived>(10, 5);
    testing("SmrtPtr Derived create", derivedPtr->getValue() == 15);

    SmrtPtr<Base> basePtr = derivedPtr;
    testing("SmrtPtr Derived to Base", basePtr->getValue() == 15);
    testing("SmrtPtr subtyping useCount", basePtr.useCount() == 2);

    SmrtPtr<Base> basePtr2 = makeSmrt<Base>(20);
    testing("SmrtPtr Base create", basePtr2->getValue() == 20);

    std::cout << std::endl;
}

void testUnqPtrBasic() {
    std::cout << "--- Проверка UnqPtr (базовые операции) ---\n";

    UnqPtr<int> ptr1 = makeUnq<int>(100);
    testing("UnqPtr create", *ptr1 == 100);

    UnqPtr<int> ptr2 = std::move(ptr1);
    testing("UnqPtr move", *ptr2 == 100 && !ptr1);

    UnqPtr<int> ptr3;
    ptr3 = std::move(ptr2);
    testing("UnqPtr move assignment", *ptr3 == 100 && !ptr2);

    int *raw = ptr3.release();
    testing("UnqPtr release", raw != nullptr && !ptr3);
    delete raw;

    ptr3.reset(new int(200));
    testing("UnqPtr reset", *ptr3 == 200);

    std::cout << std::endl;
}

void testUnqPtrSubtyping() {
    std::cout << "--- Проверка UnqPtr (подтипизация) ---\n";

    UnqPtr<Derived> derivedPtr = makeUnq<Derived>(30, 7);
    testing("UnqPtr Derived create", derivedPtr->getValue() == 37);

    UnqPtr<Base> basePtr = std::move(derivedPtr);
    testing("UnqPtr Derived to Base", basePtr->getValue() == 37 && !derivedPtr);

    std::cout << std::endl;
}

void testUnqPtrArray() {
    std::cout << "--- Проверка UnqPtr (массивы) ---\n";

    // Тест создания массива через makeUnqArray
    auto arr1 = makeUnqArray<int>(10);
    testing("UnqPtr array create", arr1.get() != nullptr);
    
    // Инициализация массива
    for (int i = 0; i < 10; i++) {
        arr1.get()[i] = i * 10;
    }
    testing("UnqPtr array access", arr1.get()[0] == 0 && arr1.get()[9] == 90);

    // Тест создания массива с инициализацией
    auto arr2 = makeUnqArray<int>(5, 42);
    bool allEqual = true;
    for (int i = 0; i < 5; i++) {
        if (arr2.get()[i] != 42) {
            allEqual = false;
            break;
        }
    }
    testing("UnqPtr array with init value", allEqual);

    // Тест move для массива
    auto arr3 = std::move(arr1);
    testing("UnqPtr array move", arr3.get() != nullptr && arr1.get() == nullptr);
    testing("UnqPtr array move data", arr3.get()[9] == 90);

    // Тест reset для массива
    int *newArr = new int[3];
    newArr[0] = 100;
    newArr[1] = 200;
    newArr[2] = 300;
    arr3.reset(newArr);
    testing("UnqPtr array reset", arr3.get()[0] == 100 && arr3.get()[2] == 300);

    // Тест с кастомным deleter
    bool deleterCalled = false;
    auto customDeleter = [&deleterCalled](int *p) {
        deleterCalled = true;
        delete[] p;
    };
    int *customArr = new int[2];
    customArr[0] = 1;
    customArr[1] = 2;
    UnqPtr<int, decltype(customDeleter)> arr4(customArr, customDeleter);
    arr4.reset();
    testing("UnqPtr array custom deleter", deleterCalled);

    std::cout << std::endl;
}

void testShrdPtrBasic() {
    std::cout << "--- Проверка ShrdPtr (базовые операции) ---\n";

    ShrdPtr<int> ptr1 = makeShrd<int>(300);
    testing("ShrdPtr create", *ptr1 == 300);
    testing("ShrdPtr useCount", ptr1.useCount() == 1);

    {
        ShrdPtr<int> ptr2 = ptr1;
        testing("ShrdPtr copy", ptr2.useCount() == 2);
        testing("ShrdPtr equality", ptr1 == ptr2);
    }

    testing("ShrdPtr after scope", ptr1.useCount() == 1);

    ShrdPtr<int> ptr3 = std::move(ptr1);
    testing("ShrdPtr move", ptr3.useCount() == 1 && !ptr1);

    std::cout << std::endl;
}

void testShrdPtrFromUnqPtr() {
    std::cout << "--- Проверка ShrdPtr (из UnqPtr) ---\n";

    UnqPtr<int> unqPtr = makeUnq<int>(400);
    ShrdPtr<int> shrdPtr(unqPtr);
    testing("ShrdPtr from UnqPtr", *shrdPtr == 400 && !unqPtr);
    testing("ShrdPtr useCount", shrdPtr.useCount() == 1);

    ShrdPtr<int> shrdPtr2 = shrdPtr;
    testing("ShrdPtr copy from UnqPtr", shrdPtr2.useCount() == 2);

    std::cout << std::endl;
}

void testShrdPtrSubtyping() {
    std::cout << "--- Проверка ShrdPtr (подтипизация) ---\n";

    ShrdPtr<Derived> derivedPtr = makeShrd<Derived>(50, 8);
    testing("ShrdPtr Derived create", derivedPtr->getValue() == 58);

    ShrdPtr<Base> basePtr = derivedPtr;
    testing("ShrdPtr Derived to Base", basePtr->getValue() == 58);
    testing("ShrdPtr subtyping useCount", basePtr.useCount() == 2);

    std::cout << std::endl;
}

void testMemorySpanBasic() {
    std::cout << "--- Проверка MemorySpan (базовые операции) ---\n";

    MemorySpan<int> span(10);
    testing("MemorySpan create", span.getCapacity() == 10 && span.getSize() == 0);

    span.emplace(0, 100);
    span.emplace(1, 200);
    span.emplace(2, 300);
    testing("MemorySpan emplace", span[0] == 100 && span[1] == 200 && span[2] == 300);
    testing("MemorySpan size", span.getSize() == 3);

    std::cout << std::endl;
}

void testMemorySpanPointers() {
    std::cout << "--- Проверка MemorySpan (умные указатели) ---\n";

    MemorySpan<int> span(5);
    span.emplace(0, 10);
    span.emplace(1, 20);
    span.emplace(2, 30);

    ShrdPtr<int> shrd = span.Copy(1);
    testing("MemorySpan Copy", *shrd == 20 && shrd.useCount() == 1);

    UnqPtr<int> unq = span.Get(2);
    testing("MemorySpan Get", *unq == 30);

    MsPtr<int> ms = span.Locate(0);
    testing("MemorySpan Locate", *ms == 10);

    std::cout << std::endl;
}

void testMsPtrBasic() {
    std::cout << "--- Проверка MsPtr (базовые операции) ---\n";

    MemorySpan<int> span(10);
    for (int i = 0; i < 5; i++) {
        span.emplace(i, i * 10);
    }

    MsPtr<int> ptr = span.Locate(2);
    testing("MsPtr create", *ptr == 20);

    ++ptr;
    testing("MsPtr increment", *ptr == 30);

    --ptr;
    testing("MsPtr decrement", *ptr == 20);

    MsPtr<int> ptr2 = ptr + 2;
    testing("MsPtr addition", *ptr2 == 40);

    MsPtr<int> ptr3 = ptr2 - 1;
    testing("MsPtr subtraction", *ptr3 == 30);

    testing("MsPtr index operator", ptr[1] == 30);

    std::cout << std::endl;
}

void testMsPtrArithmetic() {
    std::cout << "--- Проверка MsPtr (арифметика указателей) ---\n";

    MemorySpan<int> span(10);
    for (int i = 0; i < 8; i++) {
        span.emplace(i, i * 5);
    }

    MsPtr<int> ptr1 = span.Locate(2);
    MsPtr<int> ptr2 = span.Locate(5);

    ptrdiff_t diff = ptr2 - ptr1;
    testing("MsPtr difference", diff == 3);

    testing("MsPtr comparison", ptr1 < ptr2 && ptr2 > ptr1);

    ptr1 += 2;
    testing("MsPtr += operator", *ptr1 == 20);

    ptr2 -= 1;
    testing("MsPtr -= operator", *ptr2 == 20);

    std::cout << std::endl;
}


void testNullPtrHandling() {
    std::cout << "--- Проверка обработки nullptr ---\n";

    SmrtPtr<int> nullPtr;
    testing("SmrtPtr null", !nullPtr && nullPtr.useCount() == 0);

    UnqPtr<int> nullUnq;
    testing("UnqPtr null", !nullUnq);

    ShrdPtr<int> nullShrd;
    testing("ShrdPtr null", !nullShrd && nullShrd.useCount() == 0);

    SmrtPtr<int> ptr = makeSmrt<int>(42);
    ptr.reset();
    testing("SmrtPtr reset to null", !ptr);

    std::cout << std::endl;
}

void testEdgeCases() {
    std::cout << "--- Проверка граничных случаев ---\n";

    MemorySpan<int> span(1);
    span.emplace(0, 999);

    bool exceptionThrown = false;
    try {
        span[1];
    } catch (const std::out_of_range &) {
        exceptionThrown = true;
    }
    testing("MemorySpan out of range", exceptionThrown);

    exceptionThrown = false;
    try {
        MsPtr<int> ptr = span.Locate(1);
        *ptr;
    } catch (const std::out_of_range &) {
        exceptionThrown = true;
    }
    testing("MsPtr out of range", exceptionThrown);

    std::cout << std::endl;
}

inline void mainSmartPointersTest() {
    testSmrtPtrBasic();
    testSmrtPtrSubtyping();
    testUnqPtrBasic();
    testUnqPtrSubtyping();
    testUnqPtrArray();
    testShrdPtrBasic();
    testShrdPtrFromUnqPtr();
    testShrdPtrSubtyping();
    testMemorySpanBasic();
    testMemorySpanPointers();
    testMsPtrBasic();
    testMsPtrArithmetic();
    testNullPtrHandling();
    testEdgeCases();
}

