#pragma once

#include "../../Classes/Sequences/Array/capsule/DynamicArray.h"
#include "../utils.hpp"
#include <iostream>

namespace DynamicArrayModule {
    static DynamicArray<int> *intArray = nullptr;

    inline void showMenu() {
        clearConsole();
        std::cout << "\n=== Динамический массив (C++) ===\n";
        std::cout << "1. Пересоздать массив\n";
        std::cout << "2. Добавить элемент в конец\n";
        std::cout << "3. Добавить элемент в начало\n";
        std::cout << "4. Вставить по индексу\n";
        std::cout << "5. Получить элемент по индексу\n";
        std::cout << "6. Изменить элемент\n";
        std::cout << "7. Удалить элемент\n";
        std::cout << "8. Вывести массив\n";
        std::cout << "0. Назад\n";
    }

    inline void printArray() {
        std::cout << "[";
        for (int i = 0; i < intArray->getSize(); ++i) {
            std::cout << (*intArray)[i] << " ";
        }

        std::cout << "]\n";
    }

    inline void run() {
        int choice;
        intArray = new DynamicArray<int>;
        std::cout << "Массив создан.\n";
        do {
            showMenu();
            choice = getIntInput("Ваш выбор: ");
            switch (choice) {
                case 1: {
                    delete intArray;
                    int size = getIntInput("Введите размер массива: ");
                    intArray = new DynamicArray<int>(size);
                    std::cout << "Массив создан.\n";
                    break;
                }
                case 2: {
                    intArray->append(getIntInput("Введите значение: "));
                    break;
                }
                case 3: {
                    intArray->prepend(getIntInput("Введите значение: "));
                    break;
                }
                case 4: {
                    int idx = getIntInput("Индекс: ");
                    if (idx > intArray->getSize() or idx < 0) {
                        std::cout << "Индекс вне диапазона.\n";
                        break;
                    }
                    intArray->insert(idx, getIntInput("Введите значение: "));

                    break;
                }
                case 5: {
                    int idx = getIntInput("Индекс: ");
                    if (idx > intArray->getSize() or idx < 0) {
                        std::cout << "Индекс вне диапазона.\n";
                        break;
                    }
                    std::cout << "Значение: " << intArray->get(idx) << "\n";

                    break;
                }
                case 6: {
                    int idx = getIntInput("Индекс: ");
                    if (idx > intArray->getSize() or idx < 0) {
                        std::cout << "Индекс вне диапазона.\n";
                        break;
                    }
                    int val = getIntInput("Новое значение: ");
                    intArray->set(idx, val);

                    break;
                }
                case 7: {
                    int idx = getIntInput("Индекс: ");
                    if (idx > intArray->getSize() or idx < 0) {
                        std::cout << "Индекс вне диапазона.\n";
                        break;
                    }
                    intArray->remove(idx);

                    break;
                }
                case 8: {
                    break;
                }
                case 0: break;
                default:
                    std::cout << "Неверный ввод.\n";
            }
            std::cout << "Текущий массив: ";
            printArray();
            pause();
        } while (choice != 0);
    }
}
