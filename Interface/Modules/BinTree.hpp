#pragma once
#include "../../Classes/Trees/BinTree.h"
#include "../utils.hpp"
#include <iostream>

namespace BinTreeModule {
    static BinTree<int> *currentTree = nullptr;


    inline void showMenu() {
        clearConsole();
        std::cout << "\n=== Двоичное дерево (BinTree) ===\n";
        std::cout << "1. Создать дерево\n";
        std::cout << "2. Вставить элемент\n";
        std::cout << "3. Удалить элемент\n";
        std::cout << "4. Найти элемент\n";
        std::cout << "5. Вывести дерево (по обходу)\n";
        std::cout << "6. Вывести дерево (ASCII)\n";
        std::cout << "0. Назад\n";
    }

    inline void run() {
        int choice;
        currentTree = new BinTree<int>();

        do {
            showMenu();
            choice = getIntInput("Ваш выбор: ");
            switch (choice) {
                case 1: {
                    delete currentTree;
                    currentTree = new BinTree<int>();
                    std::cout << "Дерево создано.\n";
                    break;
                }
                case 2: {
                    int val = getIntInput("Введите значение для вставки: ");
                    currentTree->insert(val);
                    std::cout << "Элемент вставлен.\n";
                    break;
                }
                case 3: {
                    int val = getIntInput("Введите значение для удаления: ");
                    currentTree->remove(val);
                    std::cout << "Элемент удален.\n";
                    break;
                }
                case 4: {
                    int val = getIntInput("Введите значение для поиска: ");
                    if (currentTree->find(val)) {
                        std::cout << "Элемент найден.\n";
                    } else {
                        std::cout << "Элемент не найден.\n";
                    }
                    break;
                }
                case 5: {
                    std::cout << "Введите перестановку LPR\n";

                    // std::cin >> r1 >> r2 >> r3;
                    char rule[4];
                    std::cin >> rule;
                    std::cout << "Дерево по обходу " << rule << " ";

                    try {
                        auto seq = currentTree->toSequence(rule);
                        std::cout << "Дерево по обходу " << rule << " " << *seq;
                    } catch (std::invalid_argument &e) {
                        std::cout << "Неверный обход " << rule << " " << e.what();
                    }
                    std::cout << "\n";
                    break;
                }

                case 0: break;
                default:
                    std::cout << "Неверный выбор. Попробуйте снова.\n";
            }
            pause();
        } while (choice != 0);
    }
}
