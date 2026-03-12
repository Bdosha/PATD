#ifndef TESTING_H
#define TESTING_H
#include <iostream>

inline void testing(const char *string, bool check) {
    if (check)std::cout << string << " -> " << "OK" << std::endl;
    else std::cerr << string << " -> " << "FAILED" << std::endl;
}

template<class T>
void print(T *seq) {
    for (int i = 0; i < seq->getSize(); i++) {
        std::cout << seq->get(i) << ' ';
    }
    std::cout << std::endl;
}

int multiplyByTwo(int x) {
    return x * 2;
}

// Предикат для where (четные числа)
bool isEven(int x) {
    return x % 2 == 0;
}
#include "DA.cpp"
#include "capsule.cpp"
#include "sequences.cpp"
#include "types.cpp"
#include "trees.cpp"
#include "smart_pointers.cpp"
#include "lazy.cpp"
#include "streams.cpp"
#include "turing_tape.cpp"

inline void mainTest() {
    // test_C();
    // test_capsules();
    // mainSequencesTest();
    // test_deque();
    // test_set();
    // test_trees_node();
    // test_trees();
    // test_tree_set();
    mainSmartPointersTest();
    mainLazyTest();
    mainStreamsTest();
    mainTuringTapeTest();
}


#endif
