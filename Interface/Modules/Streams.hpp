#pragma once

#include "../../Classes/Streams/Streams.h"
#include "../../Classes/Sequences/Array/ArraySequences.h"
#include "../../Classes/Sequences/Lazy/LazySequences.h"
#include "../utils.hpp"
#include <iostream>
#include <cstdio>
#include <string>

namespace StreamsModule {

    // Вспомогательные функции для сериализации/десериализации int
    inline int deserializeInt(const std::string &str) {
        try {
            return std::stoi(str);
        } catch (...) {
            throw std::runtime_error("Invalid integer format");
        }
    }

    inline std::string serializeInt(const int &item) {
        return std::to_string(item);
    }

    // Вспомогательные функции для сериализации/десериализации string
    inline std::string deserializeString(const std::string &str) {
        return str;
    }

    inline std::string serializeString(const std::string &item) {
        return item;
    }

    static ReadOnlyStream<int> *currentReadStream = nullptr;
    static WriteOnlyStream<int> *currentWriteStream = nullptr;
    static Sequence<int> *currentSequence = nullptr;

    inline void showReadOnlyStreamMenu() {
        clearConsole();
        std::cout << "\n=== Поток только для чтения (ReadOnlyStream) ===\n";
        std::cout << "1. Создать из Sequence\n";
        std::cout << "2. Создать из LazySequence\n";
        std::cout << "3. Создать из файла\n";
        std::cout << "4. Создать из строки\n";
        std::cout << "5. Прочитать элемент (Read)\n";
        std::cout << "6. Проверить конец потока (IsEndOfStream)\n";
        std::cout << "7. Получить позицию (GetPosition)\n";
        std::cout << "8. Переместиться на позицию (Seek)\n";
        std::cout << "9. Проверить возможность Seek (IsCanSeek)\n";
        std::cout << "10. Проверить возможность возврата назад (IsCanGoBack)\n";
        std::cout << "11. Открыть поток (Open)\n";
        std::cout << "12. Закрыть поток (Close)\n";
        std::cout << "13. Прочитать все элементы\n";
        std::cout << "0. Назад\n";
    }

    inline void showWriteOnlyStreamMenu() {
        clearConsole();
        std::cout << "\n=== Поток только для записи (WriteOnlyStream) ===\n";
        std::cout << "1. Создать в Sequence\n";
        std::cout << "2. Создать в файл\n";
        std::cout << "3. Создать в другой поток\n";
        std::cout << "4. Записать элемент (Write)\n";
        std::cout << "5. Получить позицию (GetPosition)\n";
        std::cout << "6. Открыть поток (Open)\n";
        std::cout << "7. Закрыть поток (Close)\n";
        std::cout << "8. Показать записанные данные\n";
        std::cout << "0. Назад\n";
    }

    inline void showMainMenu() {
        clearConsole();
        std::cout << "\n=== Потоки (Streams) ===\n";
        std::cout << "1. Поток только для чтения (ReadOnlyStream)\n";
        std::cout << "2. Поток только для записи (WriteOnlyStream)\n";
        std::cout << "0. Назад\n";
    }

    inline void handleReadOnlyStreamChoice(int choice) {
        switch (choice) {
            case 1: {
                int size = getIntInput("Введите размер Sequence: ");
                if (size < 0) {
                    std::cout << "Размер должен быть неотрицательным.\n";
                    break;
                }
                Sequence<int> *seq = new MutableArraySequence<int>();
                std::cout << "Введите " << size << " элементов:\n";
                for (int i = 0; i < size; i++) {
                    int val = getIntInput(("Элемент " + std::to_string(i) + ": ").c_str());
                    seq->inAppend(val);
                }
                if (currentReadStream != nullptr) {
                    delete currentReadStream;
                }
                currentReadStream = new ReadOnlyStream<int>(seq);
                std::cout << "Поток создан из Sequence.\n";
                break;
            }
            case 2: {
                int size = getIntInput("Введите размер LazySequence: ");
                if (size < 0) {
                    std::cout << "Размер должен быть неотрицательным.\n";
                    break;
                }
                Sequence<int> *seq = new MutableArraySequence<int>();
                std::cout << "Введите " << size << " элементов:\n";
                for (int i = 0; i < size; i++) {
                    int val = getIntInput(("Элемент " + std::to_string(i) + ": ").c_str());
                    seq->inAppend(val);
                }
                LazySequence<int> *lazySeq = new LazySequence<int>(seq);
                if (currentReadStream != nullptr) {
                    delete currentReadStream;
                }
                currentReadStream = new ReadOnlyStream<int>(lazySeq);
                delete seq;
                std::cout << "Поток создан из LazySequence.\n";
                break;
            }
            case 3: {
                std::string filename = getStringInput("Введите имя файла: ");
                FILE *file = fopen(filename.c_str(), "r");
                if (file == nullptr) {
                    std::cout << "Ошибка: не удалось открыть файл.\n";
                    break;
                }
                if (currentReadStream != nullptr) {
                    delete currentReadStream;
                }
                currentReadStream = new ReadOnlyStream<int>(file, deserializeInt);
                std::cout << "Поток создан из файла.\n";
                std::cout << "Внимание: файл будет закрыт при удалении потока.\n";
                break;
            }
            case 4: {
                std::string data = getStringInput("Введите строку с данными (разделитель - новая строка): ");
                if (currentReadStream != nullptr) {
                    delete currentReadStream;
                }
                currentReadStream = new ReadOnlyStream<int>(data, deserializeInt);
                std::cout << "Поток создан из строки.\n";
                break;
            }
            case 5: {
                if (currentReadStream == nullptr) {
                    std::cout << "Поток не создан.\n";
                    break;
                }
                try {
                    int value = currentReadStream->Read();
                    std::cout << "Прочитано: " << value << "\n";
                } catch (const EndOfStream &e) {
                    std::cout << "Ошибка: " << e.what() << "\n";
                } catch (const std::exception &e) {
                    std::cout << "Ошибка: " << e.what() << "\n";
                }
                break;
            }
            case 6: {
                if (currentReadStream == nullptr) {
                    std::cout << "Поток не создан.\n";
                    break;
                }
                bool isEnd = currentReadStream->IsEndOfStream();
                std::cout << "Конец потока: " << (isEnd ? "да" : "нет") << "\n";
                break;
            }
            case 7: {
                if (currentReadStream == nullptr) {
                    std::cout << "Поток не создан.\n";
                    break;
                }
                size_t pos = currentReadStream->GetPosition();
                std::cout << "Текущая позиция: " << pos << "\n";
                break;
            }
            case 8: {
                if (currentReadStream == nullptr) {
                    std::cout << "Поток не создан.\n";
                    break;
                }
                int index = getIntInput("Введите позицию: ");
                try {
                    size_t newPos = currentReadStream->Seek(index);
                    std::cout << "Перемещено на позицию: " << newPos << "\n";
                } catch (const std::exception &e) {
                    std::cout << "Ошибка: " << e.what() << "\n";
                }
                break;
            }
            case 9: {
                if (currentReadStream == nullptr) {
                    std::cout << "Поток не создан.\n";
                    break;
                }
                bool canSeek = currentReadStream->IsCanSeek();
                std::cout << "Seek возможен: " << (canSeek ? "да" : "нет") << "\n";
                break;
            }
            case 10: {
                if (currentReadStream == nullptr) {
                    std::cout << "Поток не создан.\n";
                    break;
                }
                bool canGoBack = currentReadStream->IsCanGoBack();
                std::cout << "Возврат назад возможен: " << (canGoBack ? "да" : "нет") << "\n";
                break;
            }
            case 11: {
                if (currentReadStream == nullptr) {
                    std::cout << "Поток не создан.\n";
                    break;
                }
                try {
                    currentReadStream->Open();
                    std::cout << "Поток открыт.\n";
                } catch (const std::exception &e) {
                    std::cout << "Ошибка: " << e.what() << "\n";
                }
                break;
            }
            case 12: {
                if (currentReadStream == nullptr) {
                    std::cout << "Поток не создан.\n";
                    break;
                }
                currentReadStream->Close();
                std::cout << "Поток закрыт.\n";
                break;
            }
            case 13: {
                if (currentReadStream == nullptr) {
                    std::cout << "Поток не создан.\n";
                    break;
                }
                std::cout << "Чтение всех элементов:\n";
                int count = 0;
                while (!currentReadStream->IsEndOfStream()) {
                    try {
                        int value = currentReadStream->Read();
                        std::cout << "[" << count << "] = " << value << "\n";
                        count++;
                        if (count > 1000) {
                            std::cout << "Прервано: прочитано более 1000 элементов.\n";
                            break;
                        }
                    } catch (const EndOfStream &) {
                        break;
                    } catch (const std::exception &e) {
                        std::cout << "Ошибка: " << e.what() << "\n";
                        break;
                    }
                }
                std::cout << "Всего прочитано: " << count << " элементов.\n";
                break;
            }
            case 0:
                if (currentReadStream != nullptr) {
                    delete currentReadStream;
                    currentReadStream = nullptr;
                }
                break;
            default:
                std::cout << "Неверный ввод.\n";
        }
    }

    inline void handleWriteOnlyStreamChoice(int choice) {
        switch (choice) {
            case 1: {
                if (currentSequence != nullptr) {
                    delete currentSequence;
                }
                currentSequence = new MutableArraySequence<int>();
                if (currentWriteStream != nullptr) {
                    delete currentWriteStream;
                }
                currentWriteStream = new WriteOnlyStream<int>(currentSequence);
                std::cout << "Поток создан для записи в Sequence.\n";
                break;
            }
            case 2: {
                std::string filename = getStringInput("Введите имя файла: ");
                FILE *file = fopen(filename.c_str(), "w");
                if (file == nullptr) {
                    std::cout << "Ошибка: не удалось создать файл.\n";
                    break;
                }
                if (currentWriteStream != nullptr) {
                    delete currentWriteStream;
                }
                currentWriteStream = new WriteOnlyStream<int>(file, serializeInt);
                std::cout << "Поток создан для записи в файл.\n";
                std::cout << "Внимание: файл будет закрыт при удалении потока.\n";
                break;
            }
            case 3: {
                if (currentSequence == nullptr) {
                    currentSequence = new MutableArraySequence<int>();
                }
                WriteOnlyStream<int> *destStream = new WriteOnlyStream<int>(currentSequence);
                if (currentWriteStream != nullptr) {
                    delete currentWriteStream;
                }
                currentWriteStream = new WriteOnlyStream<int>(destStream);
                std::cout << "Поток создан для записи в другой поток.\n";
                break;
            }
            case 4: {
                if (currentWriteStream == nullptr) {
                    std::cout << "Поток не создан.\n";
                    break;
                }
                int value = getIntInput("Введите значение для записи: ");
                try {
                    size_t pos = currentWriteStream->Write(value);
                    std::cout << "Записано на позицию: " << pos << "\n";
                } catch (const std::exception &e) {
                    std::cout << "Ошибка: " << e.what() << "\n";
                }
                break;
            }
            case 5: {
                if (currentWriteStream == nullptr) {
                    std::cout << "Поток не создан.\n";
                    break;
                }
                size_t pos = currentWriteStream->GetPosition();
                std::cout << "Текущая позиция: " << pos << "\n";
                break;
            }
            case 6: {
                if (currentWriteStream == nullptr) {
                    std::cout << "Поток не создан.\n";
                    break;
                }
                try {
                    currentWriteStream->Open();
                    std::cout << "Поток открыт.\n";
                } catch (const std::exception &e) {
                    std::cout << "Ошибка: " << e.what() << "\n";
                }
                break;
            }
            case 7: {
                if (currentWriteStream == nullptr) {
                    std::cout << "Поток не создан.\n";
                    break;
                }
                currentWriteStream->Close();
                std::cout << "Поток закрыт.\n";
                break;
            }
            case 8: {
                if (currentSequence == nullptr) {
                    std::cout << "Нет данных для отображения.\n";
                    break;
                }
                std::cout << "Записанные данные:\n";
                std::cout << "[";
                for (int i = 0; i < currentSequence->getSize(); i++) {
                    std::cout << currentSequence->get(i);
                    if (i < currentSequence->getSize() - 1) {
                        std::cout << ", ";
                    }
                }
                std::cout << "]\n";
                break;
            }
            case 0:
                if (currentWriteStream != nullptr) {
                    delete currentWriteStream;
                    currentWriteStream = nullptr;
                }
                if (currentSequence != nullptr) {
                    delete currentSequence;
                    currentSequence = nullptr;
                }
                break;
            default:
                std::cout << "Неверный ввод.\n";
        }
    }

    inline void run() {
        int choice;
        do {
            showMainMenu();
            choice = getIntInput("Ваш выбор: ");
            switch (choice) {
                case 1: {
                    int subChoice;
                    do {
                        showReadOnlyStreamMenu();
                        subChoice = getIntInput("Ваш выбор: ");
                        handleReadOnlyStreamChoice(subChoice);
                        if (subChoice != 0) {
                            pause();
                        }
                    } while (subChoice != 0);
                    break;
                }
                case 2: {
                    int subChoice;
                    do {
                        showWriteOnlyStreamMenu();
                        subChoice = getIntInput("Ваш выбор: ");
                        handleWriteOnlyStreamChoice(subChoice);
                        if (subChoice != 0) {
                            pause();
                        }
                    } while (subChoice != 0);
                    break;
                }
                case 0:
                    if (currentReadStream != nullptr) {
                        delete currentReadStream;
                        currentReadStream = nullptr;
                    }
                    if (currentWriteStream != nullptr) {
                        delete currentWriteStream;
                        currentWriteStream = nullptr;
                    }
                    if (currentSequence != nullptr) {
                        delete currentSequence;
                        currentSequence = nullptr;
                    }
                    break;
                default:
                    std::cout << "Неверный ввод.\n";
            }
            if (choice != 0) {
                pause();
            }
        } while (choice != 0);
    }

}

