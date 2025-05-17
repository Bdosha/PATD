#include "../Classes/Sequences/Sequence.h"

auto test = new int[5]{1, 2, 3, 4, 5};
auto test2 = new int[5]{5, 4, 3, 2, 1};


void testLinkedList() {
    std::cout << "--- Проверка ListSequence ---\n";
    auto *list = new LinkedList<int>();
    list->append(5);
    list->append(6);
    // testing("LinkedList create, append", list->getFirst() == 5 && list->getLast() == 6);


    Sequence<double> *Lls1 = new ListSequence<double>;
    Lls1->append(1.8);
    Lls1->append(1);
    testing("Empty create, append", Lls1->getFirst() == 1.8 && Lls1->getLast() == 1);


    Sequence<int> *Lls2 = new ListSequence(test, 2);
    Lls2->prepend(9);
    testing("Pointer create, prepend", Lls2->getFirst() == 9 && Lls2->getLast() == 2 && Lls2->get(1) == 1);

    Sequence<int> *Lls3 = new ListSequence(*list);
    Lls3->insertAt(8, 1);
    testing("List create, insert",
            Lls3->getFirst() == list->getFirst() && Lls3->getLast() == list->getLast() && Lls3->get(1) == 8);


    Lls2->getSubsequence(1, 2);
    testing("SubSequence", Lls2->get(0) == 1 and Lls2->getSize() == 2);

    Lls2->remove(0);
    Lls2->remove(0);
    testing("Removing items", Lls2->getSize() == 0);
    std::cout << std::endl;
}

void testMutableArray() {
    std::cout << "--- Проверка MutableArraySequence ---\n";
    // Связанный список
    auto list = new DynamicArray(test, 5);
    list->resize(3);
    testing("DArray create, resize",
            list->get(0) == 1 && list->get(2) == 3 && list->get(1) == 2 && list->getSize() == 3);
    // print(list);

    Sequence<double> *Das1 = new MutableArraySequence<double>;
    Das1->append(1.8);
    auto temp = Das1->append(1);
    testing("Empty create, append", Das1->getFirst() == 1.8 && Das1->getLast() == 1 && Das1 == temp);

    Sequence<int> *Das2 = new MutableArraySequence(test, 5);
    Das2->prepend(9);

    testing("Pointer create, prepend",
            Das2->getSize() == 6 && Das2->getFirst() == 9 && Das2->getLast() == 5 && Das2->get(2) == 2);

    Sequence<int> *Das3 = new MutableArraySequence<int>(*list);
    // print(Das3);
    Das3->insertAt(8, 1);
    // print(Das3);
    testing("List create, insert", Das3->get(1) == 8 && Das3->getSize() == 4 && Das3->get(2) == 2);
    Das2->getSubsequence(2, 4);
    testing("SubSequence", Das2->get(0) == 2 and Das2->getSize() == 3 && Das2->get(1) == 3 && Das2->get(2) == 4);
    std::cout << std::endl;
}


void testImmutableArray() {
    std::cout << "--- Проверка ImmutableArraySequence ---\n";

    Sequence<int> *Das1 = new ImmutableArraySequence(test, 3);
    Das1->append(2);
    Das1->append(1);
    auto Das2 = Das1->append(2);
    Das2 = Das2->append(1);
    testing("Immutability", Das1->getSize() == 3 && Das2 != Das1 && Das2->getSize() == 5);

    std::cout << std::endl;
}

int plus2(int a) {
    return a + 2;
}

bool del2(int a) {
    return a % 2 == 0;
}

int perm(int a, int b) {
    return a * b;
}

void testMore() {
    std::cout << "--- Проверка Дополнительных функций ---\n";


    Sequence<int> *seq = new MutableArraySequence(test, 5);

    Sequence<int> *ans1 = seq->map(&plus2);
    bool flag = true;
    for (int i = 0; i < seq->getSize(); i++) {
        if (ans1->get(i) - seq->get(i) != 2) flag = false;
    }
    testing("Map", flag);


    ans1 = ans1->where(&del2);
    flag = true;
    for (int i = 0; i < ans1->getSize(); i++) {
        if (ans1->get(i) % 2 != 0) flag = false;
    }
    testing("Where", flag);
    int a = ans1->reduce(&perm, 1);
    int b = 1;
    for (int i = 0; i < ans1->getSize(); i++) {
        b *= ans1->get(i);
    }
    testing("Reduce", b == a);
    std::cout << std::endl;
}

void testPolymorph() {
    std::cout << "--- Проверка перехода от типа к типу последовательности ---\n";

    Sequence<int> *ds = new MutableArraySequence(test, 5);
    Sequence<int> *ls = new ListSequence(test, 5);
    Sequence<int> *ls_from_ds = new ListSequence<int>(ds);
    Sequence<int> *ds_from_ls = new MutableArraySequence<int>(ls);

    bool flag = true;
    for (int i = 0; i < ds->getSize(); i++) {
        if (ds->get(i) != i + 1 or ls->get(i) != i + 1 or ds_from_ls->get(i) != i + 1 or ls_from_ds->get(i) != i + 1)
            flag = false;
    }
    testing("Polymorph", flag);

    std::cout << std::endl;
}

int del10(int a) {
    return a % 10;
}

void hardConcat() {
    std::cout << "--- Сложные операции с последовательностью последовательностей ---\n";

    Sequence<int> *s1 = new MutableArraySequence(new int[5]{1, 2, 3, 4, 5}, 4);
    Sequence<int> *s2 = new ListSequence(new int[5]{9, 88, 2}, 3);
    Sequence<int> *s3 = new MutableArraySequence(new int[8]{13, 12, 55, 35, 86, 12, 4, 4}, 5);
    Sequence<int> *s4 = new ListSequence<int>();

    auto all_seq = MutableArraySequence<Sequence<int> *>(new Sequence<int> *[3]{s3, s2, s1}, 3);
    // std::cout << "--- Создадим 3 последовательности разных видов ---\n";

    // for (int i = 0; i < len(all_seq); i++) print(all_seq[i]);

    // std::cout <<
    // "Теперь добавим их всех в идиную последовательность последовательностей.\nА теперь пройдемся по ним for'ом и добавим в конец 0 и применим mod 10 через map\n";


    for (int i = 0; i < len(all_seq); i++) {
        all_seq[i]->append(0);
        all_seq[i] = all_seq[i]->map(del10);
        // std::cout << all_seq[i] << '\n';
    }
    // std::cout << &all_seq << std::endl;
    // std::cout << "А теперь конкатенируем все подпоследовательности через reduce \n";

    const auto concat_seq = all_seq.reduce(concat, s4);
    // std::cout << concat_seq << '\n';
    int *check = new int[15]{1, 2, 3, 4, 0, 9, 8, 2, 0, 3, 2, 5, 5, 6, 0};
    bool flag = true;
    for (int i = 0; i < concat_seq->getSize(); i++) {
        if (concat_seq->get(i) != check[i]) flag = false;
    }
    testing("Sequence of sequences", flag);
    // print(concat_seq);

    // std::cout << std::endl;
}


void testOperators() {
    std::cout << "--- Проверка работы операторов ---\n";

    auto array = MutableArraySequence(new int[5]{1, 2, 3, 4, 5}, 5);
    Sequence<int> *s2 = new ListSequence(new int[5]{1, 2, 3, 4, 5}, 5);
    auto s3 = ListSequence(new int[5]{1, 2, 9, 4, 5}, 5);


    testing("[index]", array[3] == 4);
    testing("[-index]", array[-4] == 2);
    testing("Operator==", array == s3 == false and array == *s2 == true);
    std::cout << std::endl;
}

void testNew() {
    std::cout << "--- Проверка поиска и получения индекса ---\n";

    auto array = new ListSequence(new int[5]{1, 2, 3, 4, 5}, 5);
    testing("Contains", array->contains(3) == true and array->contains(9) == false);
    testing("Find", array->find(3) == 2 and array->find(9) == 5);
    std::cout << std::endl;
}

void testOut() {
    Sequence<int> *s = new ListSequence(new int[5]{1, 2, 3, 4, 5}, 5);
    std::cout << s;
}

void mainSequencesTest() {
    testLinkedList();
    testMutableArray();
    testImmutableArray();
    testMore();
    testPolymorph();
    testOperators();
    testNew();
    // testOut();
    hardConcat();
}

// дисперсия 1 2 5
