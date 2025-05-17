#pragma once
#include <string>
#include <iostream>
#ifdef  PAUSE
#define PAUSE pause()
#endif

inline void clearConsole() {
    system("clear");
}

inline void pause() {
    std::cout << "\nНажмите Enter для продолжения...";
    std::cin.ignore();
    std::cin.get();
}

inline int getIntInput(const std::string &prompt) {
    int value;
    std::cout << prompt;
    while (!(std::cin >> value)) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        std::cout << "Ошибка ввода. Попробуйте снова: ";
    }
    return value;
}

inline double getDoubleInput(const std::string &prompt) {
    double value;
    std::cout << prompt;
    while (!(std::cin >> value)) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        std::cout << "Ошибка ввода. Попробуйте снова: ";
    }
    return value;
}

inline std::string getStringInput(const std::string &prompt) {
    std::cout << prompt;
    std::string s;
    std::cin >> s;
    return s;
}
