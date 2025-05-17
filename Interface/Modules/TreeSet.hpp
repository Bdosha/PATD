#pragma once

#include "../../Classes/Trees/TreeSet.h"
#include "../utils.hpp"
#include <iostream>

namespace TreeSetModule {
    static TreeSet<int> currentSet;

    inline void showMenu() {
        clearConsole();
        std::cout << "\n=== Множество (Set) ===\n";
        std::cout << "1. Создать множество\n";
        std::cout << "2. Добавить элемент\n";
        std::cout << "3. Удалить элемент\n";
        std::cout << "4. Проверить наличие элемента\n";
        std::cout << "5. Получить размер множества\n";
        std::cout << "6. Объединение с другим множеством (1, 2)\n";
        std::cout << "7. Пересечение с другим множеством (2, 3)\n";
        std::cout << "8. Разность с другим множеством (1)\n";
        std::cout << "9. Вывод множества\n";

        std::cout << "0. Назад\n";
    }

    inline void run() {
        int choice;
        currentSet = TreeSet<int>(); // Создаем пустое множество
        std::cout << "Множество создано.\n";

        do {
            showMenu();
            choice = getIntInput("Ваш выбор: "); // Получаем выбор пользователя

            switch (choice) {
                case 1: {
                    currentSet = TreeSet<int>(); // Создать новое множество
                    std::cout << "Множество создано.\n";
                    break;
                }
                case 2: {
                    int val = getIntInput("Введите значение для добавления: ");
                    currentSet.add(val); // Добавление элемента в множество
                    std::cout << "Элемент добавлен.\n";
                    break;
                }
                case 3: {
                    int val = getIntInput("Введите значение для удаления: ");
                    currentSet.remove(val); // Удаление элемента из множества
                    std::cout << "Элемент удален.\n";
                    break;
                }
                case 4: {
                    int val = getIntInput("Введите значение для проверки: ");
                    if (currentSet.contains(val)) {
                        std::cout << "Элемент найден в множестве.\n";
                    } else {
                        std::cout << "Элемент не найден в множестве.\n";
                    }
                    break;
                }
                case 5: {
                    std::cout << "Размер множества: " << currentSet.size() << "\n";
                    break;
                }
                case 6: {
                    // Пример объединения множества с другим множеством
                    auto *otherSet = new TreeSet<int>();
                    otherSet->add(1);
                    otherSet->add(2);
                    currentSet = currentSet.unite(*otherSet);
                    std::cout << "Множества объединены.\n";
                    break;
                }
                case 7: {
                    // Пример пересечения множеств
                    auto *otherSet = new TreeSet<int>();
                    otherSet->add(2);
                    otherSet->add(3);
                    currentSet = currentSet.intersect(*otherSet);
                    std::cout << "Множества пересечены.\n";
                    break;
                }
                case 8: {
                    // Пример разности множества
                    auto *otherSet = new TreeSet<int>();
                    otherSet->add(1);
                    currentSet = currentSet.subtract(*otherSet);
                    std::cout << "Разность множеств выполнена.\n";
                    break;
                }
                case 9: {
                    // Пример разности множества
                    std::cout << currentSet;
                    break;
                }
                case 0:
                    break;
                default:
                    std::cout << "Неверный ввод.\n";
            }
            pause();
        } while (choice != 0);
    }
}
