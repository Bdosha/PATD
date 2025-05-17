#include "../Classes/Types/Types.h"

template<class T>
void print(T &d) {
    print(d.toSequence());
}

int a(int b) {
    return b - 5;
}

bool b(int a) {
    return a <= 0;
}

int red(int a, int b) {
    return a + b;
}

void test_deque() {
    std::cout << "--- Проверка Deques ---\n";

    Deque<int> deque;
    deque.push_front(-1).push_back(1);
    testing("Pushes, getters", deque.size() == 2 && deque.front() == -1 && deque.back() == 1 && deque);
    auto temp = deque;
    temp = temp.concat(temp).map(a);
    testing("Concat, map, pops",
            temp.size() == 4 && temp.front() == -6 && temp.back() == -4 && temp.pop_back().pop_front().size() == 2 &&
            temp.front() == -4 && temp.back() == -6);


    temp.push_back(1).push_back(2).push_back(3);
    auto t = deque;
    testing("Sub, where, reduce, ==", temp.subDeque(1, 4).where(b).reduce(red, 0) == -6 && temp && (t == deque)  == 1);

    Queue<int> q;

    testing("Queue", q.push(1).push(2).size() == 2 && q.front() == 1 && q.pop().front() == 2 && q);

    Stack<int> s;
    testing("Stack", s.push(1).push(2).size() == 2 && s.back() == 2 && s.pop().back() == 1 && s);
    std::cout << std::endl;
}


void test_set() {
    std::cout << "--- Проверка Set ---\n";

    Set<int> st, add;
    testing("Addable, get, clear and ==",
            add.add(0).add(1).add(2) == add.clear().add(1).add(2).add(0) && add.get(0) == 0);

    st.add(1).add(2).add(-2);

    testing("Unite, Intersect", ((st | add).size() == 4 && (st | add).get(0) == -2) && (st & add).get(1) == 2);

    testing("Subtract", (st - add).size() == 1 && !(st - st));
    testing("Other", st.contains(add.remove(0)) && st > add && add <= st);
    std::cout << std::endl;
}
