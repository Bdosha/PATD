#ifndef SMARTSEQUENCE_H
#define SMARTSEQUENCE_H

#include "../SmartPointers/SmartPointers.h"
#include "Sequence.h"
#include <stdexcept>

template<class T>
class SmartSequence : public Sequence<T> {
private:
    SmrtPtr<Sequence<T>> sequence;

    Sequence<T> *clone() override {
        if (sequence) {
            return sequence->construct();
        }
        return nullptr;
    }

    Sequence<T> *instance() override {
        if (sequence) {
            return sequence.get();
        }
        return nullptr;
    }

public:
    explicit SmartSequence(Sequence<T> *seq) : sequence(seq) {
        if (seq == nullptr) {
            throw std::invalid_argument("SmartSequence: sequence cannot be null");
        }
    }

    SmartSequence(const SmartSequence &other) : sequence(other.sequence) {}

    SmartSequence(SmartSequence &&other) noexcept : sequence(std::move(other.sequence)) {}

    ~SmartSequence() override = default;

    SmartSequence &operator=(const SmartSequence &other) {
        if (this != &other) {
            sequence = other.sequence;
        }
        return *this;
    }

    SmartSequence &operator=(SmartSequence &&other) noexcept {
        if (this != &other) {
            sequence = std::move(other.sequence);
        }
        return *this;
    }

    T &getFirst() override {
        if (!sequence) throw std::runtime_error("SmartSequence: sequence is null");
        return sequence->getFirst();
    }

    T &getLast() override {
        if (!sequence) throw std::runtime_error("SmartSequence: sequence is null");
        return sequence->getLast();
    }

    T &get(int index) const override {
        if (!sequence) throw std::runtime_error("SmartSequence: sequence is null");
        return sequence->get(index);
    }

    Sequence<T> *set(int index, T value) override {
        if (!sequence) throw std::runtime_error("SmartSequence: sequence is null");
        return sequence->set(index, value);
    }

    Sequence<T> *inSet(int index, T item) override {
        if (!sequence) throw std::runtime_error("SmartSequence: sequence is null");
        return sequence->inSet(index, item);
    }

    [[nodiscard]] int getSize() const override {
        if (!sequence) return 0;
        return sequence->getSize();
    }

    Sequence<T> *append(T item) override {
        if (!sequence) throw std::runtime_error("SmartSequence: sequence is null");
        return sequence->append(item);
    }

    Sequence<T> *prepend(T item) override {
        if (!sequence) throw std::runtime_error("SmartSequence: sequence is null");
        return sequence->prepend(item);
    }

    Sequence<T> *insertAt(T item, int index) override {
        if (!sequence) throw std::runtime_error("SmartSequence: sequence is null");
        return sequence->insertAt(item, index);
    }

    Sequence<T> *getSubsequence(int startIndex, int endIndex) override {
        if (!sequence) throw std::runtime_error("SmartSequence: sequence is null");
        return sequence->getSubsequence(startIndex, endIndex);
    }

    Sequence<T> *inGetSubsequence(int startIndex, int endIndex) override {
        if (!sequence) throw std::runtime_error("SmartSequence: sequence is null");
        return sequence->inGetSubsequence(startIndex, endIndex);
    }

    Sequence<T> *inAppend(T item) override {
        if (!sequence) throw std::runtime_error("SmartSequence: sequence is null");
        return sequence->inAppend(item);
    }

    Sequence<T> *inPrepend(T item) override {
        if (!sequence) throw std::runtime_error("SmartSequence: sequence is null");
        return sequence->inPrepend(item);
    }

    Sequence<T> *inInsertAt(T item, int index) override {
        if (!sequence) throw std::runtime_error("SmartSequence: sequence is null");
        return sequence->inInsertAt(item, index);
    }

    Sequence<T> *construct() override {
        if (!sequence) throw std::runtime_error("SmartSequence: sequence is null");
        return sequence->construct();
    }

    Sequence<T> *remove(int index) override {
        if (!sequence) throw std::runtime_error("SmartSequence: sequence is null");
        return sequence->remove(index);
    }

    Sequence<T> *inRemove(int index) override {
        if (!sequence) throw std::runtime_error("SmartSequence: sequence is null");
        return sequence->inRemove(index);
    }

    bool contains(T item) override {
        if (!sequence) return false;
        return sequence->contains(item);
    }

    int find(T item) override {
        if (!sequence) return -1;
        return sequence->find(item);
    }

    T &operator[](int index) override {
        if (!sequence) throw std::runtime_error("SmartSequence: sequence is null");
        return (*sequence)[index];
    }

    [[nodiscard]] SmrtPtr<Sequence<T>> getSequence() const {
        return sequence;
    }

    [[nodiscard]] size_t getSequenceUseCount() const {
        return sequence.useCount();
    }
};

#endif

