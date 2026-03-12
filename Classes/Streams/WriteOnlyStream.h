#ifndef WRITE_ONLY_STREAM_H
#define WRITE_ONLY_STREAM_H

#include "../Sequences/Sequence.h"
#include "Serializer.h"
#include <string>

// Forward declaration
template<class T>
class WriteOnlyStream;

enum class StreamDestinationType {
    SEQUENCE,   // Обычная последовательность Sequence<T>*
    FILE,       // Файл (FILE*)
    STREAM      // Другой поток WriteOnlyStream<T>*
};

template<class T>
class WriteOnlyStream {
public:
    explicit WriteOnlyStream(Sequence<T> *seq);

    WriteOnlyStream(FILE *file, Serializer<T> serializer);

    explicit WriteOnlyStream(WriteOnlyStream<T> *stream);

    WriteOnlyStream(const WriteOnlyStream<T> &other);

    ~WriteOnlyStream();

    [[nodiscard]] size_t GetPosition() const;

    size_t Write(const T &item);

    void Open();

    void Close();

private:
    StreamDestinationType destinationType;  // Тип назначения данных
    size_t position;                         // Текущая позиция (количество записанных элементов)
    bool isOpened;                           // Флаг открытия потока

    // Для Sequence
    Sequence<T> *sequence;

    // Для файла
    FILE *file;
    Serializer<T> serializer;
    bool ownsFile;  // Управляем ли мы файлом (для копирования)

    // Для другого потока
    WriteOnlyStream<T> *destinationStream;

    void writeToSequence(const T &item);
    void writeToFile(const T &item);
    void writeToStream(const T &item);

    void initToSequence(Sequence<T> *seq);

    void initToFile(FILE *f, Serializer<T> ser);

    void initToStream(WriteOnlyStream<T> *stream);

    void cleanup();
};

#include "WriteOnlyStream.tpp"

#endif //WRITE_ONLY_STREAM_H

