#pragma once
#include <iostream>
#include "Modules/DynArray_C.hpp"
#include "Modules/LinkedList.hpp"
#include "Modules/DynamicArray.hpp"
#include "Modules/Sequence.hpp"
#include "Modules/Deque.hpp"
#include "Modules/Queue.hpp"
#include "Modules/Stack.hpp"
#include "Modules/Set.hpp"
#include "Modules/BinTree.hpp"
#include "Modules/TreeSet.hpp"
#include "Modules/SmartPointers.hpp"
#include "utils.hpp"


inline void showMainMenu() {
    clearConsole();
    std::cout << "=== Главное меню ===\n";
    std::cout << "1. Динамический массив (C)\n";
    std::cout << "2. Связный список (C++)\n";
    std::cout << "3. Динамический массив (C++)\n";
    std::cout << "4. Последовательность (Sequence)\n";
    std::cout << "5. Стек (Stack)\n";
    std::cout << "6. Очередь (Queue)\n";
    std::cout << "7. Дек (Deque)\n";
    std::cout << "8. Множество (Set)\n";
    std::cout << "9. Бинарное дерево поиска (BinTree)\n";
    std::cout << "10. Множество на бинарном дереве (TreeSet)\n";
    std::cout << "11. Умные указатели (Smart Pointers)\n";

    std::cout << "0. Выход\n";
}


inline void handleMainMenuChoice(int choice) {
    switch (choice) {
        case 1:
            DynArray_C::run();

            break;
        case 2:
            LinkedListModule::run();
            break;
        case 3:
            DynamicArrayModule::run();
            break;

        case 4:
            SequenceModule::run();
            break;
        case 5:
            StackModule::run();
            break;
        case 6:
            QueueModule::run();
            break;
        case 7:
            DequeModule::run();
            break;
        case 8:
            SetModule::run();
            break;
        case 9:
            BinTreeModule::run();
            break;
        case 10:
            TreeSetModule::run();
            break;
        case 11:
            SmartPointersModule::run();
            break;
        case 0:
            std::cout << "Выход...\n";
            break;
        default:
            std::cout << "Неверный выбор!\n";
    }
}

namespace MainMenu {
    inline void main() {
        int choice;
        do {
            showMainMenu();
            choice = getIntInput("Ваш выбор: ");
            handleMainMenuChoice(choice);
        } while (choice != 0);
    }
}
