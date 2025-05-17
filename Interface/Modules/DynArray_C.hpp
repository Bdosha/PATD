// Interface/Modules/DynArray_C.hpp
#pragma once

#include "../../Classes/CDynamicArray/CynamicArray.h"
#include "../utils.hpp"
#include <iostream>

namespace DynArray_C {
    static List *currentList = nullptr;

    inline void showMenu() {
        clearConsole();
        std::cout << "\n=== Динамический массив (C) ===\n";
        std::cout << "1. Создать список\n";
        std::cout << "2. Добавить элемент\n";
        std::cout << "3. Получить элемент по индексу\n";
        std::cout << "4. Изменить элемент по индексу\n";
        std::cout << "5. Отсортировать\n";
        std::cout << "6. Вывести содержимое\n";
        std::cout << "0. Назад\n";
    }

    inline void printList(const List *l) {
        if (!l || !l->type || !l->type->print) {
            std::cout << "Невозможно вывести элементы.\n";
            return;
        }
        std::cout << "[ ";
        for (unsigned int i = 0; i < l->len; ++i) {
            l->type->print((char *) l->element + i * l->type->size);
            if (i < l->len - 1) std::cout << ", ";
        }
        std::cout << " ]\n";
    }

    inline void run() {
        int choice;
        currentList = createList(0, &Int);
        std::cout << "Список создан.\n";
        do {
            showMenu();
            choice = getIntInput("Ваш выбор: ");
            switch (choice) {
                case 1: {
                    int cap = getIntInput("Введите размер: ");
                    currentList = createList(cap, &Int);
                    std::cout << "Список создан.\n";
                    break;
                }
                case 2: {
                    // if (!check_list(currentList)) break;
                    int val = getIntInput("Введите целое число: ");
                    append(currentList, &val);

                    break;
                }
                case 3: {
                    // if (!check_list(currentList)) break;
                    int index = getIntInput("Введите индекс: ");
                    if (currentList->len < index or index < 0) {
                        std::cout << "Индекс вне диапазона.\n";
                        break;
                    }

                    void *elem = getElement(currentList, index);
                    currentList->type->print(elem);
                    std::cout << "\n";
                    break;
                }
                case 4: {
                    // if (!check_list(currentList)) break;
                    int index = getIntInput("Введите индекс: ");
                    if (currentList->len < index or index < 0) {
                        std::cout << "Индекс вне диапазона.\n";
                        break;
                    }
                    int val = getIntInput("Новое значение: ");
                    setElement(currentList, index, &val);

                    break;
                }
                case 5: {
                    // if (!check_list(currentList)) break;
                    sort(currentList);
                    std::cout << "Список отсортирован.\n";
                    break;
                }
                case 6: {
                    // if (!check_list(currentList)) break;
                    // printList(currentList);
                    break;
                }
                case 0: {
                    break;
                }
                default:
                    std::cout << "Неверный ввод.\n";
            }
            std::cout << "Текущий список: ";;
            printList(currentList);
            pause();
        } while (choice != 0);
    }
} // namespace DynArray_C
