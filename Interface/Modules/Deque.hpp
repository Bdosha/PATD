#pragma once

#include "../../Classes/Types/Deques/Deque.h"
#include "../utils.hpp"
#include <iostream>

namespace DequeModule {
    static Deque<int> *currentDeque = nullptr;

    inline void showMenu() {
        clearConsole();
        std::cout << "\n=== Дек (Deque) ===\n";
        std::cout << "1. Создать дек\n";
        std::cout << "2. Добавить элемент в начало\n";
        std::cout << "3. Добавить элемент в конец\n";
        std::cout << "4. Удалить элемент из начала\n";
        std::cout << "5. Удалить элемент из конца\n";
        std::cout << "6. Просмотр первого элемента\n";
        std::cout << "7. Просмотр последнего элемента\n";
        std::cout << "0. Назад\n";
    }

    inline void printDeque() {
        if (currentDeque) {
            for (int i = 0; i < currentDeque->size(); ++i) {
                std::cout << currentDeque->front() << " ";
                currentDeque->pop_front();
            }
        } else {
            std::cout << "Дек пуст или не создан.\n";
        }
        std::cout << "\n";
    }

    inline void run() {
        int choice;
        currentDeque = new Deque<int>();
        std::cout << "Дек создан.\n";
        do {
            showMenu();
            choice = getIntInput("Ваш выбор: ");
            switch (choice) {
                case 1: {
                    currentDeque = new Deque<int>();
                    std::cout << "Дек создан.\n";
                    break;
                }
                case 2: {
                    int val = getIntInput("Введите значение: ");
                    currentDeque->push_front(val);
                    break;
                }
                case 3: {
                    int val = getIntInput("Введите значение: ");
                    currentDeque->push_back(val);
                    break;
                }
                case 4: {
                    currentDeque->pop_front();
                    std::cout << "Элемент удален из начала.\n";
                    break;
                }
                case 5: {
                    currentDeque->pop_back();
                    std::cout << "Элемент удален из конца.\n";
                    break;
                }
                case 6: {
                    std::cout << "Первый элемент: " << currentDeque->front() << "\n";
                    break;
                }
                case 7: {
                    std::cout << "Последний элемент: " << currentDeque->back() << "\n";
                    break;
                }

                case 0: break;
                default:
                    std::cout << "Неверный ввод.\n";
            }
            pause();

        } while (choice != 0);
    }
}
