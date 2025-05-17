#pragma once

#include "../../Classes/Types/Deques/Deques.h"
#include "../utils.hpp"
#include <iostream>

namespace StackModule {
    static Stack<int> *currentStack = nullptr;

    inline void showMenu() {
        clearConsole();
        std::cout << "\n=== Стек (Stack) ===\n";
        std::cout << "1. Создать стек\n";
        std::cout << "2. Добавить элемент\n";
        std::cout << "3. Удалить элемент\n";
        std::cout << "4. Просмотр последнего элемента\n";
        std::cout << "0. Назад\n";
    }


    inline void run() {
        int choice;
        currentStack = new Stack<int>();
        std::cout << "Стек создан.\n";

        do {
            showMenu();
            choice = getIntInput("Ваш выбор: ");
            switch (choice) {
                case 1: {
                    currentStack = new Stack<int>();
                    std::cout << "Стек создан.\n";
                    break;
                }
                case 2: {
                    int val = getIntInput("Введите значение: ");
                    currentStack->push(val);
                    break;
                }
                case 3: {
                    currentStack->pop();
                    std::cout << "Элемент удален.\n";
                    break;
                }
                case 4: {
                    std::cout << "Последний элемент: " << currentStack->back() << "\n";
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
