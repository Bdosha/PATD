#ifndef READ_ONLY_STREAM_H
#define READ_ONLY_STREAM_H

#include "../Sequences/Sequence.h"
#include "Deserializer.h"
#include <string>

// Forward declaration
template<class T>
class ReadOnlyStream;

/**
 * Enum для типа источника потока
 */
enum class StreamSourceType {
    SEQUENCE,        // Обычная последовательность Sequence<T>*
    LAZY_SEQUENCE,   // Ленивая последовательность LazySequence<T>*
    FILE,            // Файл (FILE*)
    STRING,          // Строка (std::string)
    STREAM           // Другой поток ReadOnlyStream<T>*
};

template<class T>
class ReadOnlyStream {
public:
    explicit ReadOnlyStream(Sequence<T> *seq);

    explicit ReadOnlyStream(LazySequence<T> *lazySeq);

    ReadOnlyStream(FILE *file, Deserializer<T> deserializer);

    ReadOnlyStream(const std::string &str, Deserializer<T> deserializer);

    explicit ReadOnlyStream(ReadOnlyStream<T> *stream);

    ReadOnlyStream(const ReadOnlyStream<T> &other);

    ~ReadOnlyStream();

    [[nodiscard]] bool IsEndOfStream() const;

    T Read();

    [[nodiscard]] size_t GetPosition() const;

    [[nodiscard]] bool IsCanSeek() const;

    size_t Seek(size_t index);

    [[nodiscard]] bool IsCanGoBack() const;

    void Open();

    void Close();

private:
    StreamSourceType sourceType;  // Тип источника данных
    size_t position;              // Текущая позиция (количество прочитанных элементов)
    bool isOpened;                 // Флаг открытия потока

    Sequence<T> *sequence;
    size_t sequenceSize;

    // Для LazySequence
    LazySequence<T> *lazySequence;
    size_t lazySequenceSize;  // Размер материализованной части

    // Для файла
    FILE *file;
    Deserializer<T> deserializer;
    bool ownsFile;  // Управляем ли мы файлом (для копирования)

    // Для строки
    std::string *stringData;
    size_t stringPosition;  // Позиция в строке

    // Для другого потока
    ReadOnlyStream<T> *sourceStream;

    T readFromSequence();
    T readFromLazySequence();
    T readFromFile();
    T readFromString();
    T readFromStream();

    void initFromSequence(Sequence<T> *seq);

    void initFromLazySequence(LazySequence<T> *lazySeq);

    void initFromFile(FILE *f, Deserializer<T> deser);

    void initFromString(const std::string &str, Deserializer<T> deser);

    void initFromStream(ReadOnlyStream<T> *stream);

    void cleanup();
};

#include "ReadOnlyStream.tpp"

#endif //READ_ONLY_STREAM_H

