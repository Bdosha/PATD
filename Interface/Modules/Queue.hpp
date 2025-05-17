#pragma once

#include "../../Classes/Types/Deques/Deques.h"
#include "../utils.hpp"
#include <iostream>

namespace QueueModule {
    static Queue<int> *currentQueue = nullptr;

    inline void showMenu() {
        clearConsole();
        std::cout << "\n=== Очередь (Queue) ===\n";
        std::cout << "1. Создать очередь\n";
        std::cout << "2. Добавить элемент\n";
        std::cout << "3. Удалить элемент\n";
        std::cout << "4. Просмотр первого элемента\n";
        std::cout << "0. Назад\n";
    }

    inline void printQueue() {
        if (currentQueue) {
            for (int i = 0; i < currentQueue->size(); ++i) {
                std::cout << currentQueue->front() << " ";
                currentQueue->pop();
            }
        } else {
            std::cout << "Очередь пустая или не создана.\n";
        }
        std::cout << "\n";
    }

    inline void run() {
        int choice;
        currentQueue = new Queue<int>();
        std::cout << "Очередь создана.\n";
        do {
            showMenu();
            choice = getIntInput("Ваш выбор: ");
            switch (choice) {
                case 1: {
                    currentQueue = new Queue<int>();
                    std::cout << "Очередь создана.\n";
                    break;
                }
                case 2: {
                    int val = getIntInput("Введите значение: ");
                    currentQueue->push(val);
                    break;
                }
                case 3: {
                    currentQueue->pop();
                    std::cout << "Элемент удален.\n";
                    break;
                }
                case 4: {
                    std::cout << "Первый элемент: " << currentQueue->front() << "\n";
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
