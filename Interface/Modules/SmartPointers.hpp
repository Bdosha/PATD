#pragma once

#include "../../Classes/SmartPointers/SmartPointers.h"
#include "../../Classes/Sequences/SmartSequence.h"
#include "../../Classes/Sequences/Array/ArraySequences.h"
#include "../../Testing/Testing.h"
#include "../../Testing/smart_pointers_perf.cpp"
#include "../utils.hpp"
#include <iostream>

namespace SmartPointersModule {

    inline void showMenu() {
        clearConsole();
        std::cout << "\n=== Умные указатели (Smart Pointers) ===\n";
        std::cout << "1. Демонстрация SmrtPtr\n";
        std::cout << "2. Демонстрация UnqPtr\n";
        std::cout << "3. Демонстрация ShrdPtr\n";
        std::cout << "4. Демонстрация MemorySpan и MsPtr\n";
        std::cout << "5. Демонстрация SmartSequence\n";
        std::cout << "6. Запустить функциональные тесты\n";
        std::cout << "7. Запустить тесты производительности\n";
        std::cout << "0. Назад\n";
    }

    inline void demoSmrtPtr() {
        clearConsole();
        std::cout << "\n=== Демонстрация SmrtPtr ===\n";

        SmrtPtr<int> ptr1 = makeSmrt<int>(42);
        std::cout << "Создан SmrtPtr с значением: " << *ptr1 << "\n";
        std::cout << "Количество ссылок: " << ptr1.useCount() << "\n";

        {
            SmrtPtr<int> ptr2 = ptr1;
            std::cout << "После копирования, количество ссылок: " << ptr1.useCount() << "\n";
        }

        std::cout << "После выхода из области видимости, количество ссылок: " << ptr1.useCount() << "\n";

        SmrtPtr<int> ptr3 = std::move(ptr1);
        std::cout << "После перемещения, ptr3 имеет значение: " << *ptr3 << "\n";
        std::cout << "ptr1 теперь пуст: " << (!ptr1 ? "да" : "нет") << "\n";

        pause();
    }

    inline void demoUnqPtr() {
        clearConsole();
        std::cout << "\n=== Демонстрация UnqPtr ===\n";

        UnqPtr<int> ptr1 = makeUnq<int>(100);
        std::cout << "Создан UnqPtr с значением: " << *ptr1 << "\n";

        UnqPtr<int> ptr2 = std::move(ptr1);
        std::cout << "После перемещения, ptr2 имеет значение: " << *ptr2 << "\n";
        std::cout << "ptr1 теперь пуст: " << (!ptr1 ? "да" : "нет") << "\n";

        int *raw = ptr2.release();
        std::cout << "Освобожден сырой указатель со значением: " << *raw << "\n";
        delete raw;

        pause();
    }

    inline void demoShrdPtr() {
        clearConsole();
        std::cout << "\n=== Демонстрация ShrdPtr ===\n";

        ShrdPtr<int> ptr1 = makeShrd<int>(200);
        std::cout << "Создан ShrdPtr с значением: " << *ptr1 << "\n";
        std::cout << "Количество ссылок: " << ptr1.useCount() << "\n";

        {
            ShrdPtr<int> ptr2 = ptr1;
            std::cout << "После копирования, количество ссылок: " << ptr1.useCount() << "\n";
        }

        std::cout << "После выхода из области видимости, количество ссылок: " << ptr1.useCount() << "\n";

        UnqPtr<int> unqPtr = makeUnq<int>(300);
        ShrdPtr<int> ptr3(unqPtr);
        std::cout << "ShrdPtr из UnqPtr со значением: " << *ptr3 << "\n";
        std::cout << "UnqPtr теперь пуст: " << (!unqPtr ? "да" : "нет") << "\n";

        pause();
    }

    inline void demoMemorySpan() {
        clearConsole();
        std::cout << "\n=== Демонстрация MemorySpan и MsPtr ===\n";

        MemorySpan<int> span(10);
        std::cout << "Создан MemorySpan с емкостью: " << span.getCapacity() << "\n";

        for (int i = 0; i < 5; i++) {
            span.emplace(i, i * 10);
        }
        std::cout << "Добавлено 5 элементов. Размер: " << span.getSize() << "\n";

        std::cout << "Элементы: ";
        for (int i = 0; i < 5; i++) {
            std::cout << span[i] << " ";
        }
        std::cout << "\n";

        MsPtr<int> ptr = span.Locate(2);
        std::cout << "MsPtr указывает на элемент с индексом 2: " << *ptr << "\n";

        ++ptr;
        std::cout << "После инкремента: " << *ptr << "\n";

        --ptr;
        std::cout << "После декремента: " << *ptr << "\n";

        MsPtr<int> ptr2 = ptr + 2;
        std::cout << "MsPtr + 2 указывает на: " << *ptr2 << "\n";

        ShrdPtr<int> shrd = span.Copy(1);
        std::cout << "ShrdPtr из Copy(1): " << *shrd << "\n";

        UnqPtr<int> unq = span.Get(3);
        std::cout << "UnqPtr из Get(3): " << *unq << "\n";

        pause();
    }

    inline void demoSmartSequence() {
        clearConsole();
        std::cout << "\n=== Демонстрация SmartSequence ===\n";

        Sequence<int> *seq = new MutableArraySequence<int>();
        seq->append(1);
        seq->append(2);
        seq->append(3);
        seq->append(4);
        seq->append(5);

        SmartSequence<int> smartSeq(seq);
        std::cout << "Создан SmartSequence с размером: " << smartSeq.getSize() << "\n";
        std::cout << "Количество ссылок на Sequence: " << smartSeq.getSequence().useCount() << "\n";

        std::cout << "Элементы: ";
        for (int i = 0; i < smartSeq.getSize(); i++) {
            std::cout << smartSeq.get(i) << " ";
        }
        std::cout << "\n";

        smartSeq.append(6);
        std::cout << "После добавления элемента, размер: " << smartSeq.getSize() << "\n";

        SmartSequence<int> smartSeq2 = smartSeq;
        std::cout << "После копирования, количество ссылок: " << smartSeq.getSequence().useCount() << "\n";

        pause();
    }

    inline void runFunctionalTests() {
        clearConsole();
        std::cout << "\n=== Запуск функциональных тестов ===\n";
        mainSmartPointersTest();
        pause();
    }

    inline void runPerformanceTests() {
        clearConsole();
        std::cout << "\n=== Запуск тестов производительности ===\n";
        std::cout << "Это может занять некоторое время...\n";
        ::runPerformanceTests();
        pause();
    }

    inline void run() {
        int choice;
        do {
            showMenu();
            choice = getIntInput("Ваш выбор: ");
            switch (choice) {
                case 1:
                    demoSmrtPtr();
                    break;
                case 2:
                    demoUnqPtr();
                    break;
                case 3:
                    demoShrdPtr();
                    break;
                case 4:
                    demoMemorySpan();
                    break;
                case 5:
                    demoSmartSequence();
                    break;
                case 6:
                    runFunctionalTests();
                    break;
                case 7:
                    runPerformanceTests();
                    break;
                case 0:
                    break;
                default:
                    std::cout << "Неверный выбор!\n";
                    pause();
            }
        } while (choice != 0);
    }
}

