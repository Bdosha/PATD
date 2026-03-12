#pragma once

#include "../../Classes/Sequences/Lazy/LazySequences.h"
#include "../../Classes/Sequences/Array/ArraySequences.h"
#include "../utils.hpp"
#include <iostream>
#include <functional>

namespace LazySequenceModule {

    static LazySequence<int>* currentLazySequence = nullptr;

    // Вспомогательные функции для функциональных операций
    inline int multiplyByTwo(int x) { return x * 2; }
    inline bool isEven(int x) { return x % 2 == 0; }
    inline int sum(int a, int b) { return a + b; }

    inline void showMenu() {
        clearConsole();
        std::cout << "\n=== Ленивая последовательность (LazySequence) ===\n";
        std::cout << "1. Создать пустую последовательность\n";
        std::cout << "2. Создать из массива\n";
        std::cout << "3. Создать из обычной последовательности\n";
        std::cout << "4. Создать бесконечную (Фибоначчи)\n";
        std::cout << "5. Создать бесконечную (степени двойки)\n";
        std::cout << "6. Создать бесконечную (факториалы)\n";
        std::cout << "7. Получить элемент по индексу\n";
        std::cout << "8. Получить первый элемент\n";
        std::cout << "9. Получить последний элемент\n";
        std::cout << "10. Добавить элемент в конец (Append)\n";
        std::cout << "11. Добавить элемент в начало (Prepend)\n";
        std::cout << "12. Вставить элемент по индексу (InsertAt)\n";
        std::cout << "13. Конкатенация с другой последовательностью\n";
        std::cout << "14. Получить подпоследовательность\n";
        std::cout << "15. Применить Map (умножение на 2)\n";
        std::cout << "16. Применить Where (только четные)\n";
        std::cout << "17. Применить Reduce (сумма)\n";
        std::cout << "18. Вывести последовательность\n";
        std::cout << "19. Показать информацию о последовательности\n";
        std::cout << "0. Назад\n";
    }

    inline void printLazySequence() {
        if (currentLazySequence == nullptr) {
            std::cout << "Последовательность не создана.\n";
            return;
        }

        std::cout << "[";
        
        size_t materialized = currentLazySequence->GetMaterializedCount();
        Cardinal length = currentLazySequence->GetLength();
        
        if (length.isInfinity()) {
            std::cout << "Бесконечная последовательность, материализовано: " << materialized << " элементов\n";
            std::cout << "Первые " << (materialized > 20 ? 20 : materialized) << " элементов: ";
        } else {
            std::cout << "Длина: " << length.getValue() << ", материализовано: " << materialized << "\n";
            std::cout << "Элементы: ";
        }

        int count = length.isInfinity() ? (materialized > 20 ? 20 : materialized) : length.getValue();
        for (int i = 0; i < count && i < materialized; ++i) {
            try {
                std::cout << currentLazySequence->Get(i);
                if (i < count - 1 && i < materialized - 1) {
                    std::cout << ", ";
                }
            } catch (const std::exception &e) {
                std::cout << "?";
            }
        }
        
        if (length.isInfinity() && materialized > 20) {
            std::cout << "...";
        }
        
        std::cout << "]\n";
    }

    inline void showSequenceInfo() {
        if (currentLazySequence == nullptr) {
            std::cout << "Последовательность не создана.\n";
            return;
        }

        Cardinal length = currentLazySequence->GetLength();
        size_t materialized = currentLazySequence->GetMaterializedCount();
        
        std::cout << "=== Информация о последовательности ===\n";
        if (length.isInfinity()) {
            std::cout << "Тип: Бесконечная последовательность\n";
        } else {
            std::cout << "Тип: Конечная последовательность\n";
            std::cout << "Длина: " << length.getValue() << "\n";
        }
        std::cout << "Материализовано элементов: " << materialized << "\n";
        
        if (materialized > 0) {
            try {
                std::cout << "Первый элемент: " << currentLazySequence->GetFirst() << "\n";
            } catch (...) {
                std::cout << "Первый элемент: недоступен\n";
            }
            
            if (!length.isInfinity()) {
                try {
                    std::cout << "Последний элемент: " << currentLazySequence->GetLast() << "\n";
                } catch (...) {
                    std::cout << "Последний элемент: недоступен\n";
                }
            }
        }
    }

    // Рекуррентные правила
    int fibonacciRule(Sequence<int> *history) {
        int size = history->getSize();
        if (size < 2) {
            return 1;
        }
        return history->get(size - 1) + history->get(size - 2);
    }

    int powerOfTwoRule(Sequence<int> *history) {
        int size = history->getSize();
        if (size == 0) {
            return 1;
        }
        return history->get(size - 1) * 2;
    }

    int factorialRule(Sequence<int> *history) {
        int size = history->getSize();
        if (size == 0) {
            return 1;
        }
        return history->get(size - 1) * (size + 1);
    }

    inline void run() {
        int choice;
        do {
            showMenu();
            choice = getIntInput("Ваш выбор: ");
            switch (choice) {
                case 1: {
                    if (currentLazySequence != nullptr) {
                        delete currentLazySequence;
                    }
                    currentLazySequence = new LazySequence<int>();
                    std::cout << "Пустая последовательность создана.\n";
                    break;
                }
                case 2: {
                    int size = getIntInput("Введите размер массива: ");
                    if (size < 0) {
                        std::cout << "Размер должен быть неотрицательным.\n";
                        break;
                    }
                    int *arr = new int[size];
                    std::cout << "Введите " << size << " элементов:\n";
                    for (int i = 0; i < size; i++) {
                        arr[i] = getIntInput(("Элемент " + std::to_string(i) + ": ").c_str());
                    }
                    if (currentLazySequence != nullptr) {
                        delete currentLazySequence;
                    }
                    currentLazySequence = new LazySequence<int>(arr, size);
                    delete[] arr;
                    std::cout << "Последовательность создана из массива.\n";
                    break;
                }
                case 3: {
                    int size = getIntInput("Введите размер последовательности: ");
                    if (size < 0) {
                        std::cout << "Размер должен быть неотрицательным.\n";
                        break;
                    }
                    Sequence<int> *seq = new MutableArraySequence<int>();
                    std::cout << "Введите " << size << " элементов:\n";
                    for (int i = 0; i < size; i++) {
                        int val = getIntInput(("Элемент " + std::to_string(i) + ": ").c_str());
                        seq->inAppend(val);
                    }
                    if (currentLazySequence != nullptr) {
                        delete currentLazySequence;
                    }
                    currentLazySequence = new LazySequence<int>(seq);
                    delete seq;
                    std::cout << "Последовательность создана из Sequence.\n";
                    break;
                }
                case 4: {
                    Sequence<int> *initial = new MutableArraySequence<int>();
                    initial->inAppend(1);
                    initial->inAppend(1);
                    if (currentLazySequence != nullptr) {
                        delete currentLazySequence;
                    }
                    currentLazySequence = new LazySequence<int>(fibonacciRule, initial);
                    delete initial;
                    std::cout << "Бесконечная последовательность Фибоначчи создана.\n";
                    break;
                }
                case 5: {
                    Sequence<int> *initial = new MutableArraySequence<int>();
                    initial->inAppend(1);
                    if (currentLazySequence != nullptr) {
                        delete currentLazySequence;
                    }
                    currentLazySequence = new LazySequence<int>(powerOfTwoRule, initial);
                    delete initial;
                    std::cout << "Бесконечная последовательность степеней двойки создана.\n";
                    break;
                }
                case 6: {
                    Sequence<int> *initial = new MutableArraySequence<int>();
                    initial->inAppend(1);
                    if (currentLazySequence != nullptr) {
                        delete currentLazySequence;
                    }
                    currentLazySequence = new LazySequence<int>(factorialRule, initial);
                    delete initial;
                    std::cout << "Бесконечная последовательность факториалов создана.\n";
                    break;
                }
                case 7: {
                    if (currentLazySequence == nullptr) {
                        std::cout << "Последовательность не создана.\n";
                        break;
                    }
                    int index = getIntInput("Введите индекс: ");
                    try {
                        int value = currentLazySequence->Get(index);
                        std::cout << "Элемент по индексу " << index << ": " << value << "\n";
                    } catch (const std::exception &e) {
                        std::cout << "Ошибка: " << e.what() << "\n";
                    }
                    break;
                }
                case 8: {
                    if (currentLazySequence == nullptr) {
                        std::cout << "Последовательность не создана.\n";
                        break;
                    }
                    try {
                        int value = currentLazySequence->GetFirst();
                        std::cout << "Первый элемент: " << value << "\n";
                    } catch (const std::exception &e) {
                        std::cout << "Ошибка: " << e.what() << "\n";
                    }
                    break;
                }
                case 9: {
                    if (currentLazySequence == nullptr) {
                        std::cout << "Последовательность не создана.\n";
                        break;
                    }
                    try {
                        int value = currentLazySequence->GetLast();
                        std::cout << "Последний элемент: " << value << "\n";
                    } catch (const std::exception &e) {
                        std::cout << "Ошибка: " << e.what() << "\n";
                    }
                    break;
                }
                case 10: {
                    if (currentLazySequence == nullptr) {
                        std::cout << "Последовательность не создана.\n";
                        break;
                    }
                    int value = getIntInput("Введите значение: ");
                    LazySequence<int> *newSeq = currentLazySequence->Append(value);
                    delete currentLazySequence;
                    currentLazySequence = newSeq;
                    std::cout << "Элемент добавлен в конец.\n";
                    break;
                }
                case 11: {
                    if (currentLazySequence == nullptr) {
                        std::cout << "Последовательность не создана.\n";
                        break;
                    }
                    int value = getIntInput("Введите значение: ");
                    LazySequence<int> *newSeq = currentLazySequence->Prepend(value);
                    delete currentLazySequence;
                    currentLazySequence = newSeq;
                    std::cout << "Элемент добавлен в начало.\n";
                    break;
                }
                case 12: {
                    if (currentLazySequence == nullptr) {
                        std::cout << "Последовательность не создана.\n";
                        break;
                    }
                    int index = getIntInput("Введите индекс: ");
                    int value = getIntInput("Введите значение: ");
                    try {
                        LazySequence<int> *newSeq = currentLazySequence->InsertAt(value, index);
                        delete currentLazySequence;
                        currentLazySequence = newSeq;
                        std::cout << "Элемент вставлен.\n";
                    } catch (const std::exception &e) {
                        std::cout << "Ошибка: " << e.what() << "\n";
                    }
                    break;
                }
                case 13: {
                    if (currentLazySequence == nullptr) {
                        std::cout << "Последовательность не создана.\n";
                        break;
                    }
                    int size = getIntInput("Введите размер второй последовательности: ");
                    Sequence<int> *seq = new MutableArraySequence<int>();
                    std::cout << "Введите " << size << " элементов:\n";
                    for (int i = 0; i < size; i++) {
                        int val = getIntInput(("Элемент " + std::to_string(i) + ": ").c_str());
                        seq->inAppend(val);
                    }
                    LazySequence<int> *other = new LazySequence<int>(seq);
                    LazySequence<int> *newSeq = currentLazySequence->Concat(other);
                    delete currentLazySequence;
                    delete other;
                    delete seq;
                    currentLazySequence = newSeq;
                    std::cout << "Последовательности сцеплены.\n";
                    break;
                }
                case 14: {
                    if (currentLazySequence == nullptr) {
                        std::cout << "Последовательность не создана.\n";
                        break;
                    }
                    int start = getIntInput("Введите начальный индекс: ");
                    int end = getIntInput("Введите конечный индекс: ");
                    try {
                        LazySequence<int> *subseq = currentLazySequence->GetSubsequence(start, end);
                        std::cout << "Подпоследовательность:\n";
                        printLazySequence();
                        delete subseq;
                    } catch (const std::exception &e) {
                        std::cout << "Ошибка: " << e.what() << "\n";
                    }
                    break;
                }
                case 15: {
                    if (currentLazySequence == nullptr) {
                        std::cout << "Последовательность не создана.\n";
                        break;
                    }
                    LazySequence<int> *newSeq = currentLazySequence->Map(multiplyByTwo);
                    delete currentLazySequence;
                    currentLazySequence = newSeq;
                    std::cout << "Map применен (умножение на 2).\n";
                    break;
                }
                case 16: {
                    if (currentLazySequence == nullptr) {
                        std::cout << "Последовательность не создана.\n";
                        break;
                    }
                    LazySequence<int> *newSeq = currentLazySequence->Where(isEven);
                    delete currentLazySequence;
                    currentLazySequence = newSeq;
                    std::cout << "Where применен (только четные).\n";
                    break;
                }
                case 17: {
                    if (currentLazySequence == nullptr) {
                        std::cout << "Последовательность не создана.\n";
                        break;
                    }
                    try {
                        int result = currentLazySequence->Reduce(sum, 0);
                        std::cout << "Reduce (сумма): " << result << "\n";
                    } catch (const std::exception &e) {
                        std::cout << "Ошибка: " << e.what() << "\n";
                    }
                    break;
                }
                case 18: {
                    printLazySequence();
                    break;
                }
                case 19: {
                    showSequenceInfo();
                    break;
                }
                case 0:
                    if (currentLazySequence != nullptr) {
                        delete currentLazySequence;
                        currentLazySequence = nullptr;
                    }
                    break;
                default:
                    std::cout << "Неверный ввод.\n";
            }
            if (choice != 0) {
                pause();
            }
        } while (choice != 0);
    }

}

