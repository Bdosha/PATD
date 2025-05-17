#pragma once

#include "../../Classes/Sequences/Sequence.h"
#include "../utils.hpp"
#include <iostream>

namespace SequenceModule {

    static Sequence<int>* currentSequence = nullptr;

    inline void showMenu() {
        clearConsole();
        std::cout << "\n=== Последовательность (Sequence) ===\n";
        std::cout << "1. Пересоздать последовательность\n";
        std::cout << "2. Добавить элемент в конец\n";
        std::cout << "3. Добавить элемент в начало\n";
        std::cout << "4. Вставить по индексу\n";
        std::cout << "5. Получить элемент по индексу\n";
        std::cout << "6. Изменить элемент\n";
        std::cout << "7. Удалить элемент\n";
        std::cout << "8. Вывести последовательность\n";
        std::cout << "0. Назад\n";
    }

    inline void printSequence() {
        std::cout << "[";

        if (currentSequence) {
            for (int i = 0; i < currentSequence->getSize(); ++i) {
                std::cout << (*currentSequence)[i] << " ";
            }
        } else {
            std::cout << "Последовательность пустая или не создана.";
        }
        std::cout << "]\n";
    }

    inline void run() {
        int choice;
        currentSequence = new MutableArraySequence<int>;
        std::cout << "Последовательность создана.\n";
        do {
            showMenu();
            choice = getIntInput("Ваш выбор: ");
            switch (choice) {
                case 1: {
                    int size = getIntInput("Введите размер последовательности: ");
                    currentSequence = new MutableArraySequence<int>(size);
                    std::cout << "Последовательность создана.\n";
                    break;
                }
                case 2: {
                    if (currentSequence) {
                        int val = getIntInput("Введите значение: ");
                        currentSequence->append(val);
                    }
                    break;
                }
                case 3: {
                    if (currentSequence) {
                        int val = getIntInput("Введите значение: ");
                        currentSequence->prepend(val);
                    }
                    break;
                }
                case 4: {
                    int idx = getIntInput("Индекс: ");
                    if (currentSequence) {
                        int val = getIntInput("Введите значение: ");
                        currentSequence->insertAt(val, idx);
                    }
                    break;
                }
                case 5: {
                    int idx = getIntInput("Индекс: ");
                    if (currentSequence) {
                        std::cout << "Значение: " << currentSequence->get(idx) << "\n";
                    }
                    break;
                }
                case 6: {
                    int idx = getIntInput("Индекс: ");
                    if (currentSequence) {
                        int val = getIntInput("Новое значение: ");
                        currentSequence->set(idx, val);
                    }
                    break;
                }
                case 7: {
                    int idx = getIntInput("Индекс: ");
                    if (currentSequence) {
                        currentSequence->remove(idx);
                    }
                    break;
                }
                case 8: {
                    printSequence();
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
