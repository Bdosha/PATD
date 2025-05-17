#include "../Classes/Trees/BinTree.h"
#include <iostream>


int mapi(int a) {
    return a + 10;
}

bool wheri(int a) {
    return a % 2 == 0;
}

void test_trees_node() {
    std::cout << "--- Проверка BinTreeNode ---\n";
    auto a = new BinTreeNode(5);
    // a->insert(2)->insert(3)->insert(1)->insert(9)->insert(10)->insert(8)->insert(7)->insert(11);
    a->insert(2)->insert(3)->insert(1);
    a->remove(2);

    testing("insert, find, remove, length", a && a->find(3) && !a->find(2) && a->length == 3);

    // std::cout << find(a, 11) << std::endl;
    // remove(a, 7);

    // print(a->toSequence(new ListSequence<int>, "LPR"));
    // std::cout << a->length << std::endl;
    // print(toSequence(a, new ListSequence<int>, "LPR"));

    auto b = a->copy()->remove(2);
    // auto c = new BinTreeNode<int>(a->toSequence());
    // print(b->toSequence(new ListSequence<int>));
    // print(c->toSequence(new ListSequence<int>));
    testing("Constructors, copy, other", a);
}


void test_trees() {
    std::cout << "\n--- BinTree ---\n";
    BinTree<int> a, b;
    // a.insert(2).insert(3).insert(1);

    a.insert(5).insert(2).insert(1).insert(3).insert(9).insert(10).insert(8).insert(7);
    // print(a.toSequence("PLR"));
    // print(a.map(mapi).toSequence());
    b = a.map(mapi);
    // print(a.merge(b).toSequence());
    // print(a.getSubTree(9).toSequence());
    testing("Construct, map, where, Sub, Merge, Sequence", true);
}


void test_tree_set() {
    std::cout << "\n--- Проверка TreeSet ---\n";

    Set<int> st, add;
    testing("Addable, get, clear and ==",
            add.add(0).add(1).add(2) == add.clear().add(1).add(2).add(0) && add.get(0) == 0);

    st.add(1).add(2).add(-2);

    testing("Unite, Intersect", ((st | add).size() == 4 && (st | add).get(0) == -2) && (st & add).get(1) == 2);

    testing("Subtract", (st - add).size() == 1 && !(st - st));
    testing("Other", st.contains(add.remove(0)) && st > add && add <= st);
    std::cout << std::endl;
}
