#pragma once

#include "../../Classes/Sequences/LinkedList/capsule/LinkedList.h"
#include "../utils.hpp"
#include <iostream>

namespace LinkedListModule {
    static LinkedList<int> *intList = nullptr;

    inline void showMenu() {
        clearConsole();
        std::cout << "\n=== Связный список (C++) ===\n";
        std::cout << "1. Пересоздать список\n";
        std::cout << "2. Добавить элемент в конец\n";
        std::cout << "3. Добавить элемент в начало\n";
        std::cout << "4. Вставить по индексу\n";
        std::cout << "5. Получить элемент по индексу\n";
        std::cout << "6. Изменить элемент\n";
        std::cout << "7. Удалить элемент\n";
        std::cout << "8. Вывести список\n";
        std::cout << "0. Назад\n";
    }

    inline void printList() {
        std::cout << '[';
        for (int i = 0; i < intList->getSize(); ++i) {
            std::cout << (*intList)[i] << " ";
        }

        std::cout << "]\n";
    }

    inline void run() {
        int choice;
        intList = new LinkedList<int>();
        std::cout << "Список создан.\n";
        do {
            showMenu();
            choice = getIntInput("Ваш выбор: ");
            switch (choice) {
                case 1: {
                    delete intList;
                    intList = new LinkedList<int>();
                    std::cout << "Список создан.\n";
                    break;
                }
                case 2: {
                    if (intList) {
                        int val = getIntInput("Введите значение: ");
                        intList->append(val);
                    }
                    break;
                }
                case 3: {
                    if (intList) {
                        int val = getIntInput("Введите значение: ");
                        intList->prepend(val);
                    }
                    break;
                }
                case 4: {
                    int idx = getIntInput("Индекс: ");

                    try {
                        int val = getIntInput("Введите значение: ");
                        intList->insertAt(val, idx);
                    } catch (std::out_of_range &e) {
                        std::cout << e.what() << "\n";
                    }
                    break;
                }
                case 5: {
                    int idx = getIntInput("Индекс: ");
                    try {
                        std::cout << "Значение: " << intList->get(idx) << "\n";
                    } catch (std::out_of_range &e) {
                        std::cout << e.what() << "\n";
                    }
                    break;
                }

                case 6: {
                    int idx = getIntInput("Индекс: ");


                    try {
                        int val = getIntInput("Новое значение: ");
                        intList->set(idx, val);
                    } catch (std::out_of_range &e) {
                        std::cout << e.what() << "\n";
                    }
                    break;
                }
                case 7: {
                    int idx = getIntInput("Индекс: ");
                    try {
                        intList->remove(idx);
                    } catch (std::out_of_range &e) {
                        std::cout << e.what() << "\n";
                    }

                    break;
                }
                case 8: {
                    break;
                }
                case 0: break;
                default:
                    std::cout << "Неверный ввод.\n";
            }
            std::cout << "Текущий список: ";
            printList();
            pause();
        } while (choice != 0);
    }
}
