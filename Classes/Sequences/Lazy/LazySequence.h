#ifndef LAZYSEQUENCE_H
#define LAZYSEQUENCE_H

#include "../Sequence.h"
#include "Generator.h"
#include "Cardinal.h"
#include <functional>
#include <stdexcept>

// Forward declaration
template<class T>
class Generator;

/**
 * LazySequence<T> - ленивая последовательность с поддержкой бесконечных последовательностей
 * Элементы вычисляются по требованию и кэшируются (мемоизация)
 */
template<class T>
class LazySequence {
    friend class Generator<T>;

public:
    /**
     * Создает пустую ленивую последовательность
     */
    LazySequence();

    /**
     * Создает ленивую последовательность из массива
     * @param items - массив элементов
     * @param count - количество элементов
     */
    LazySequence(T *items, int count);

    /**
     * Создает ленивую последовательность из обычной последовательности
     * @param seq - исходная последовательность
     */
    explicit LazySequence(Sequence<T> *seq);

    /**
     * Создает ленивую последовательность на основе рекуррентного правила
     * @param rule - функция порождения следующего элемента: T(Sequence<T>*)
     * @param initialElements - начальные элементы (первые k элементов)
     */
    LazySequence(T (*rule)(Sequence<T>*), Sequence<T> *initialElements);

    /**
     * Создает ленивую последовательность на основе рекуррентного правила (std::function)
     * @param rule - функция порождения следующего элемента
     * @param initialElements - начальные элементы
     */
    LazySequence(std::function<T(Sequence<T>*)> rule, Sequence<T> *initialElements);

    /**
     * Копирующий конструктор
     */
    LazySequence(const LazySequence<T> &other);

    /**
     * Деструктор
     */
    ~LazySequence();

    /**
     * Получить первый элемент
     * @throws std::out_of_range если список пуст
     */
    T GetFirst();

    /**
     * Получить последний элемент
     * @throws std::out_of_range если список пуст
     * @note Может не завершиться для бесконечных последовательностей
     */
    T GetLast();

    /**
     * Получить элемент по индексу
     * @param index - индекс элемента
     * @throws std::out_of_range если индекс некорректен
     * @note Может не завершиться для бесконечных последовательностей
     */
    T Get(int index);

    /**
     * Получить подпоследовательность
     * @param startIndex - начальный индекс
     * @param endIndex - конечный индекс
     * @throws std::out_of_range если индексы некорректны
     */
    LazySequence<T> *GetSubsequence(int startIndex, int endIndex);

    /**
     * Получить длину последовательности (Cardinal)
     */
    [[nodiscard]] Cardinal GetLength() const;

    /**
     * Получить количество материализованных элементов
     */
    [[nodiscard]] size_t GetMaterializedCount() const;

    /**
     * Добавить элемент в конец
     * @note Для бесконечных последовательностей операция запоминается, но не выполняется сразу
     */
    LazySequence<T> *Append(T item);

    /**
     * Добавить элемент в начало
     */
    LazySequence<T> *Prepend(T item);

    /**
     * Вставить элемент в заданную позицию
     * @param item - элемент для вставки
     * @param index - позиция вставки
     * @throws std::out_of_range если индекс некорректен
     */
    LazySequence<T> *InsertAt(T item, int index);

    /**
     * Сцепить две ленивые последовательности
     * @param list - последовательность для сцепления
     * @note Если первый список бесконечный, второй недостижим
     */
    LazySequence<T> *Concat(LazySequence<T> *list);

    /**
     * Функциональные операции
     */
    template<class T2>
    LazySequence<T2> *Map(T2 (*func)(T));

    T Reduce(T (*func)(T, T), T start);

    LazySequence<T> *Where(bool (*predicate)(T));

    LazySequence<T> *Zip(Sequence<T> *seq);

    /**
     * Оператор доступа по индексу
     */
    T operator[](int index);

private:
    Generator<T> *generator;  // генератор элементов
    Sequence<T> *materialized;  // материализованные (кэшированные) элементы
    bool isInfinite;  // является ли последовательность бесконечной
    Cardinal length;  // длина последовательности

    /**
     * Материализует элементы до указанного индекса включительно
     */
    void materializeUpTo(int index);

    /**
     * Создает генератор для простой последовательности (из массива/Sequence)
     */
    void createSimpleGenerator(Sequence<T> *seq);

    /**
     * Создает генератор для рекуррентного правила
     */
    void createRecurrentGenerator(std::function<T(Sequence<T>*)> rule, Sequence<T> *initialElements);
};

#include "LazySequence.tpp"

#endif //LAZYSEQUENCE_H

