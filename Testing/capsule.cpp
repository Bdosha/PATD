#include "../Classes/Sequences/Array/capsule/DynamicArray.h"
#include "../Classes/Sequences/LinkedList/capsule/LinkedList.h"

void test_capsules() {
    std::cout << "--- Проверка Dynamic Array ---\n";

    DynamicArray<int> a;
    DynamicArray<int> a2;
    a2.prepend(2);
    a2.prepend(10);
    a2.prepend(10);

    a.resize(3);

    bool flag = true;
    for (int i = 0; i < 3; i++) {
        a[i] = i;
        if (a[i] != i) flag = false;
    }
    testing("Resize, set, get", flag);

    // a.prepend(1);
    testing("Append, prepend, insert",
            a.append(10)[3] == 10 && a.prepend(-1)[1] == 0 && a.insert(3, -999)[3] == -999 && a.getSize() == 6);

    std::cout << "\n--- Проверка LinkedList ---\n";
    auto *list = new LinkedList<int>();
    list->append(5);
    list->append(6);
    testing("LinkedList create, append", list->getFirst() == 5 && list->getLast() == 6);
    std::cout << std::endl;



    // a.insert(3, -999);
    // print(&a);

    // print(&a);
    // a.insert(0, -9);
    // a.insert(2, 0);

    // a.remove(2);
    // print(&a);
    // print(&a);
}
