#pragma once

#include "../../Classes/Sequences/Lazy/TuringTape.h"
#include "../../Classes/Sequences/Array/ArraySequences.h"
#include "../utils.hpp"
#include <iostream>
#include <functional>

namespace TuringTapeModule {

    static TuringTape<int>* currentTape = nullptr;

    // Вспомогательные функции для функциональных операций
    inline int multiplyByTwo(int x) { return x * 2; }
    inline bool isEven(int x) { return x % 2 == 0; }

    // Рекуррентные правила
    inline int rightPowerOfTwoRule(Sequence<int> *history) {
        int size = history->getSize();
        if (size == 0) {
            return 1;
        }
        return history->get(size - 1) * 2;
    }

    inline int leftPowerOfTwoRule(Sequence<int> *history) {
        int size = history->getSize();
        if (size == 0) {
            return 1;
        }
        return history->get(size - 1) / 2;
    }

    inline void showMenu() {
        clearConsole();
        std::cout << "\n=== Лента Тьюринга (TuringTape) ===\n";
        std::cout << "1. Создать ленту с начальным значением\n";
        std::cout << "2. Создать из массива\n";
        std::cout << "3. Создать из последовательности\n";
        std::cout << "4. Создать бесконечную ленту (степени двойки)\n";
        std::cout << "5. Получить элемент по индексу\n";
        std::cout << "6. Установить значение элемента\n";
        std::cout << "7. Вставить элемент по индексу\n";
        std::cout << "8. Удалить элемент по индексу\n";
        std::cout << "9. Получить подленту\n";
        std::cout << "10. Применить Map (умножение на 2)\n";
        std::cout << "11. Применить Where (только четные)\n";
        std::cout << "12. Вывести ленту\n";
        std::cout << "13. Показать информацию о ленте\n";
        std::cout << "0. Назад\n";
    }

    inline void printTape() {
        if (currentTape == nullptr) {
            std::cout << "Лента не создана.\n";
            return;
        }

        std::cout << "Лента: [";
        
        size_t leftMaterialized = currentTape->GetLeftMaterializedCount();
        size_t rightMaterialized = currentTape->GetRightMaterializedCount();
        Cardinal leftLength = currentTape->GetLeftLength();
        Cardinal rightLength = currentTape->GetRightLength();
        
        // Выводим левую сторону (отрицательные индексы)
        if (leftMaterialized > 0) {
            std::cout << "... ";
            int startLeft = -static_cast<int>(leftMaterialized);
            int endLeft = -1;
            int displayCount = leftMaterialized > 10 ? 10 : leftMaterialized;
            int startDisplay = endLeft - displayCount + 1;
            
            for (int i = startDisplay; i <= endLeft; i++) {
                try {
                    std::cout << currentTape->Get(i);
                    if (i < endLeft) {
                        std::cout << ", ";
                    }
                } catch (const std::exception &) {
                    std::cout << "?";
                }
            }
            if (leftMaterialized > 10) {
                std::cout << " ...";
            }
            std::cout << " | ";
        }
        
        // Выводим правую сторону (неотрицательные индексы)
        int displayCount = rightMaterialized > 20 ? 20 : rightMaterialized;
        for (int i = 0; i < displayCount; i++) {
            try {
                std::cout << currentTape->Get(i);
                if (i < displayCount - 1) {
                    std::cout << ", ";
                }
            } catch (const std::exception &) {
                std::cout << "?";
            }
        }
        
        if (rightLength.isInfinity() && rightMaterialized > 20) {
            std::cout << " ...";
        }
        
        std::cout << "]\n";
        
        std::cout << "Левая сторона: ";
        if (leftLength.isInfinity()) {
            std::cout << "бесконечна";
        } else {
            std::cout << "длина " << leftLength.getValue();
        }
        std::cout << ", материализовано: " << leftMaterialized << "\n";
        
        std::cout << "Правая сторона: ";
        if (rightLength.isInfinity()) {
            std::cout << "бесконечна";
        } else {
            std::cout << "длина " << rightLength.getValue();
        }
        std::cout << ", материализовано: " << rightMaterialized << "\n";
    }

    inline void showTapeInfo() {
        if (currentTape == nullptr) {
            std::cout << "Лента не создана.\n";
            return;
        }

        std::cout << "=== Информация о ленте ===\n";
        
        Cardinal leftLength = currentTape->GetLeftLength();
        Cardinal rightLength = currentTape->GetRightLength();
        Cardinal totalLength = currentTape->GetTotalLength();
        
        std::cout << "Левая сторона: ";
        if (leftLength.isInfinity()) {
            std::cout << "бесконечна";
        } else {
            std::cout << "длина " << leftLength.getValue();
        }
        std::cout << " (материализовано: " << currentTape->GetLeftMaterializedCount() << ")\n";
        
        std::cout << "Правая сторона: ";
        if (rightLength.isInfinity()) {
            std::cout << "бесконечна";
        } else {
            std::cout << "длина " << rightLength.getValue();
        }
        std::cout << " (материализовано: " << currentTape->GetRightMaterializedCount() << ")\n";
        
        std::cout << "Общая длина: ";
        if (totalLength.isInfinity()) {
            std::cout << "бесконечна";
        } else {
            std::cout << totalLength.getValue();
        }
        std::cout << " (материализовано: " << currentTape->GetTotalMaterializedCount() << ")\n";
        
        if (currentTape->GetTotalMaterializedCount() > 0) {
            try {
                std::cout << "Элемент в позиции 0: " << currentTape->Get(0) << "\n";
            } catch (...) {
                std::cout << "Элемент в позиции 0: недоступен\n";
            }
        }
    }

    inline void run() {
        int choice;
        do {
            showMenu();
            choice = getIntInput("Ваш выбор: ");
            switch (choice) {
                case 1: {
                    if (currentTape != nullptr) {
                        delete currentTape;
                    }
                    int value = getIntInput("Введите начальное значение: ");
                    currentTape = new TuringTape<int>(value);
                    std::cout << "Лента создана с начальным значением " << value << " в позиции 0.\n";
                    break;
                }
                case 2: {
                    int size = getIntInput("Введите размер массива: ");
                    if (size < 0) {
                        std::cout << "Размер должен быть неотрицательным.\n";
                        break;
                    }
                    int startIndex = getIntInput("Введите начальный индекс (может быть отрицательным): ");
                    int *arr = new int[size];
                    std::cout << "Введите " << size << " элементов:\n";
                    for (int i = 0; i < size; i++) {
                        arr[i] = getIntInput(("Элемент " + std::to_string(i) + ": ").c_str());
                    }
                    if (currentTape != nullptr) {
                        delete currentTape;
                    }
                    currentTape = new TuringTape<int>(arr, size, startIndex);
                    delete[] arr;
                    std::cout << "Лента создана из массива.\n";
                    break;
                }
                case 3: {
                    int size = getIntInput("Введите размер последовательности: ");
                    if (size < 0) {
                        std::cout << "Размер должен быть неотрицательным.\n";
                        break;
                    }
                    int startIndex = getIntInput("Введите начальный индекс (может быть отрицательным): ");
                    Sequence<int> *seq = new MutableArraySequence<int>();
                    std::cout << "Введите " << size << " элементов:\n";
                    for (int i = 0; i < size; i++) {
                        int val = getIntInput(("Элемент " + std::to_string(i) + ": ").c_str());
                        seq->inAppend(val);
                    }
                    if (currentTape != nullptr) {
                        delete currentTape;
                    }
                    currentTape = new TuringTape<int>(seq, startIndex);
                    delete seq;
                    std::cout << "Лента создана из последовательности.\n";
                    break;
                }
                case 4: {
                    Sequence<int> *leftInit = new MutableArraySequence<int>();
                    leftInit->inAppend(1);
                    Sequence<int> *rightInit = new MutableArraySequence<int>();
                    rightInit->inAppend(1);
                    if (currentTape != nullptr) {
                        delete currentTape;
                    }
                    currentTape = new TuringTape<int>(leftPowerOfTwoRule, rightPowerOfTwoRule, leftInit, rightInit);
                    delete leftInit;
                    delete rightInit;
                    std::cout << "Бесконечная лента создана (степени двойки).\n";
                    break;
                }
                case 5: {
                    if (currentTape == nullptr) {
                        std::cout << "Лента не создана.\n";
                        break;
                    }
                    int index = getIntInput("Введите индекс (может быть отрицательным): ");
                    try {
                        int value = currentTape->Get(index);
                        std::cout << "Элемент по индексу " << index << ": " << value << "\n";
                    } catch (const std::exception &e) {
                        std::cout << "Ошибка: " << e.what() << "\n";
                    }
                    break;
                }
                case 6: {
                    if (currentTape == nullptr) {
                        std::cout << "Лента не создана.\n";
                        break;
                    }
                    int index = getIntInput("Введите индекс (может быть отрицательным): ");
                    int value = getIntInput("Введите значение: ");
                    try {
                        currentTape->Set(index, value);
                        std::cout << "Значение установлено.\n";
                    } catch (const std::exception &e) {
                        std::cout << "Ошибка: " << e.what() << "\n";
                    }
                    break;
                }
                case 7: {
                    if (currentTape == nullptr) {
                        std::cout << "Лента не создана.\n";
                        break;
                    }
                    int index = getIntInput("Введите индекс (может быть отрицательным): ");
                    int value = getIntInput("Введите значение: ");
                    try {
                        TuringTape<int> *newTape = currentTape->InsertAt(index, value);
                        delete currentTape;
                        currentTape = newTape;
                        std::cout << "Элемент вставлен.\n";
                    } catch (const std::exception &e) {
                        std::cout << "Ошибка: " << e.what() << "\n";
                    }
                    break;
                }
                case 8: {
                    if (currentTape == nullptr) {
                        std::cout << "Лента не создана.\n";
                        break;
                    }
                    int index = getIntInput("Введите индекс (может быть отрицательным): ");
                    try {
                        TuringTape<int> *newTape = currentTape->Remove(index);
                        delete currentTape;
                        currentTape = newTape;
                        std::cout << "Элемент удален.\n";
                    } catch (const std::exception &e) {
                        std::cout << "Ошибка: " << e.what() << "\n";
                    }
                    break;
                }
                case 9: {
                    if (currentTape == nullptr) {
                        std::cout << "Лента не создана.\n";
                        break;
                    }
                    int start = getIntInput("Введите начальный индекс (может быть отрицательным): ");
                    int end = getIntInput("Введите конечный индекс (может быть отрицательным): ");
                    try {
                        TuringTape<int> *subtape = currentTape->GetSubsequence(start, end);
                        std::cout << "Подлента создана.\n";
                        // Можно было бы вывести подленту, но для простоты просто удаляем
                        delete subtape;
                    } catch (const std::exception &e) {
                        std::cout << "Ошибка: " << e.what() << "\n";
                    }
                    break;
                }
                case 10: {
                    if (currentTape == nullptr) {
                        std::cout << "Лента не создана.\n";
                        break;
                    }
                    TuringTape<int> *newTape = currentTape->Map(multiplyByTwo);
                    delete currentTape;
                    currentTape = newTape;
                    std::cout << "Map применен (умножение на 2).\n";
                    break;
                }
                case 11: {
                    if (currentTape == nullptr) {
                        std::cout << "Лента не создана.\n";
                        break;
                    }
                    TuringTape<int> *newTape = currentTape->Where(isEven);
                    delete currentTape;
                    currentTape = newTape;
                    std::cout << "Where применен (только четные).\n";
                    break;
                }
                case 12: {
                    printTape();
                    break;
                }
                case 13: {
                    showTapeInfo();
                    break;
                }
                case 0:
                    if (currentTape != nullptr) {
                        delete currentTape;
                        currentTape = nullptr;
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
