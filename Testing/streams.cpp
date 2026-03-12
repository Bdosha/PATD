#include "../Classes/Streams/Streams.h"
#include "../Classes/Sequences/Array/ArraySequences.h"
#include "../Classes/Sequences/Lazy/LazySequences.h"
#include "Testing.h"
#include <iostream>
#include <cstdio>
#include <fstream>
#include <sstream>

// Вспомогательные функции для сериализации/десериализации int
int deserializeInt(const std::string &str) {
    return std::stoi(str);
}

std::string serializeInt(const int &item) {
    return std::to_string(item);
}

// Вспомогательные функции для сериализации/десериализации string
std::string deserializeString(const std::string &str) {
    return str;
}

std::string serializeString(const std::string &item) {
    return item;
}

// Тесты для ReadOnlyStream - создание из Sequence
void testReadOnlyStreamFromSequence() {
    std::cout << "--- Проверка ReadOnlyStream из Sequence ---\n";

    int arr[] = {1, 2, 3, 4, 5};
    Sequence<int> *seq = new MutableArraySequence<int>(arr, 5);
    ReadOnlyStream<int> *stream = new ReadOnlyStream<int>(seq);

    testing("ReadOnlyStream from Sequence IsEndOfStream false", !stream->IsEndOfStream());
    testing("ReadOnlyStream from Sequence GetPosition 0", stream->GetPosition() == 0);
    testing("ReadOnlyStream from Sequence Read first", stream->Read() == 1);
    testing("ReadOnlyStream from Sequence GetPosition 1", stream->GetPosition() == 1);
    testing("ReadOnlyStream from Sequence Read second", stream->Read() == 2);
    testing("ReadOnlyStream from Sequence Read third", stream->Read() == 3);

    // Читаем оставшиеся элементы
    stream->Read();  // 4
    stream->Read();  // 5

    testing("ReadOnlyStream from Sequence IsEndOfStream true", stream->IsEndOfStream());

    // Проверка исключения при чтении после конца
    bool exceptionThrown = false;
    try {
        stream->Read();
    } catch (const EndOfStream &) {
        exceptionThrown = true;
    }
    testing("ReadOnlyStream from Sequence throws EndOfStream", exceptionThrown);

    delete stream;
    delete seq;

    std::cout << std::endl;
}

// Тесты для ReadOnlyStream - создание из LazySequence
void testReadOnlyStreamFromLazySequence() {
    std::cout << "--- Проверка ReadOnlyStream из LazySequence ---\n";

    int arr[] = {10, 20, 30};
    LazySequence<int> *lazySeq = new LazySequence<int>(arr, 3);
    ReadOnlyStream<int> *stream = new ReadOnlyStream<int>(lazySeq);

    testing("ReadOnlyStream from LazySequence Read first", stream->Read() == 10);
    testing("ReadOnlyStream from LazySequence Read second", stream->Read() == 20);
    testing("ReadOnlyStream from LazySequence Read third", stream->Read() == 30);
    // После чтения всех элементов проверяем конец потока
    bool isEnd = stream->IsEndOfStream();
    testing("ReadOnlyStream from LazySequence IsEndOfStream true", isEnd);

    delete stream;
    delete lazySeq;

    std::cout << std::endl;
}

// Тесты для ReadOnlyStream - Seek
void testReadOnlyStreamSeek() {
    std::cout << "--- Проверка Seek для ReadOnlyStream ---\n";

    int arr[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    Sequence<int> *seq = new MutableArraySequence<int>(arr, 10);
    auto *stream = new ReadOnlyStream<int>(seq);

    testing("ReadOnlyStream IsCanSeek true", stream->IsCanSeek());
    testing("ReadOnlyStream Seek to position 5", stream->Seek(5) == 5);
    testing("ReadOnlyStream Read after Seek", stream->Read() == 5);
    testing("ReadOnlyStream Seek back to 0", stream->Seek(0) == 0);
    testing("ReadOnlyStream Read after Seek to 0", stream->Read() == 0);

    delete stream;
    delete seq;

    std::cout << std::endl;
}

// Тесты для ReadOnlyStream - из файла
void testReadOnlyStreamFromFile() {
    std::cout << "--- Проверка ReadOnlyStream из файла ---\n";

    // Создаем временный файл
    const char *filename = "test_stream_file.txt";
    FILE *file = fopen(filename, "w");
    if (file == nullptr) {
        std::cerr << "Cannot create test file\n";
        return;
    }

    fprintf(file, "10\n");
    fprintf(file, "20\n");
    fprintf(file, "30\n");
    fclose(file);

    // Открываем файл для чтения
    file = fopen(filename, "r");
    if (file == nullptr) {
        std::cerr << "Cannot open test file for reading\n";
        return;
    }

    ReadOnlyStream<int> *stream = new ReadOnlyStream<int>(file, deserializeInt);

    testing("ReadOnlyStream from File Read first", stream->Read() == 10);
    testing("ReadOnlyStream from File Read second", stream->Read() == 20);
    testing("ReadOnlyStream from File Read third", stream->Read() == 30);
    // После чтения всех элементов проверяем конец потока
    bool isEnd = stream->IsEndOfStream();
    testing("ReadOnlyStream from File IsEndOfStream true", isEnd);

    delete stream;
    fclose(file);

    // Удаляем временный файл
    remove(filename);

    std::cout << std::endl;
}

// Тесты для ReadOnlyStream - из строки
void testReadOnlyStreamFromString() {
    std::cout << "--- Проверка ReadOnlyStream из строки ---\n";

    std::string data = "100\n200\n300\n";
    ReadOnlyStream<int> *stream = new ReadOnlyStream<int>(data, deserializeInt);

    testing("ReadOnlyStream from String Read first", stream->Read() == 100);
    testing("ReadOnlyStream from String Read second", stream->Read() == 200);
    testing("ReadOnlyStream from String Read third", stream->Read() == 300);
    testing("ReadOnlyStream from String IsEndOfStream true", stream->IsEndOfStream());

    delete stream;

    std::cout << std::endl;
}

// Тесты для WriteOnlyStream - в Sequence
void testWriteOnlyStreamToSequence() {
    std::cout << "--- Проверка WriteOnlyStream в Sequence ---\n";

    Sequence<int> *seq = new MutableArraySequence<int>();
    WriteOnlyStream<int> *stream = new WriteOnlyStream<int>(seq);

    testing("WriteOnlyStream to Sequence GetPosition 0", stream->GetPosition() == 0);
    testing("WriteOnlyStream to Sequence Write first", stream->Write(1) == 1);
    testing("WriteOnlyStream to Sequence Write second", stream->Write(2) == 2);
    testing("WriteOnlyStream to Sequence Write third", stream->Write(3) == 3);
    testing("WriteOnlyStream to Sequence GetPosition 3", stream->GetPosition() == 3);

    testing("WriteOnlyStream to Sequence sequence size", seq->getSize() == 3);
    testing("WriteOnlyStream to Sequence sequence[0]", seq->get(0) == 1);
    testing("WriteOnlyStream to Sequence sequence[1]", seq->get(1) == 2);
    testing("WriteOnlyStream to Sequence sequence[2]", seq->get(2) == 3);

    delete stream;
    delete seq;

    std::cout << std::endl;
}

// Тесты для WriteOnlyStream - в файл
void testWriteOnlyStreamToFile() {
    std::cout << "--- Проверка WriteOnlyStream в файл ---\n";

    const char *filename = "test_write_stream_file.txt";
    FILE *file = fopen(filename, "w");
    if (file == nullptr) {
        std::cerr << "Cannot create test file\n";
        return;
    }

    WriteOnlyStream<int> *stream = new WriteOnlyStream<int>(file, serializeInt);

    stream->Write(100);
    stream->Write(200);
    stream->Write(300);

    fclose(file);
    delete stream;

    // Проверяем содержимое файла
    file = fopen(filename, "r");
    if (file == nullptr) {
        std::cerr << "Cannot open test file for reading\n";
        return;
    }

    char buffer[256];
    fgets(buffer, sizeof(buffer), file);
    testing("WriteOnlyStream to File first line", std::stoi(buffer) == 100);
    fgets(buffer, sizeof(buffer), file);
    testing("WriteOnlyStream to File second line", std::stoi(buffer) == 200);
    fgets(buffer, sizeof(buffer), file);
    testing("WriteOnlyStream to File third line", std::stoi(buffer) == 300);

    fclose(file);
    remove(filename);

    std::cout << std::endl;
}

// Тесты для WriteOnlyStream - в другой поток
void testWriteOnlyStreamToStream() {
    std::cout << "--- Проверка WriteOnlyStream в другой поток ---\n";

    Sequence<int> *seq = new MutableArraySequence<int>();
    WriteOnlyStream<int> *destStream = new WriteOnlyStream<int>(seq);
    WriteOnlyStream<int> *stream = new WriteOnlyStream<int>(destStream);

    stream->Write(10);
    stream->Write(20);
    stream->Write(30);

    testing("WriteOnlyStream to Stream sequence size", seq->getSize() == 3);
    testing("WriteOnlyStream to Stream sequence[0]", seq->get(0) == 10);
    testing("WriteOnlyStream to Stream sequence[1]", seq->get(1) == 20);
    testing("WriteOnlyStream to Stream sequence[2]", seq->get(2) == 30);

    // Важно: удаляем в правильном порядке
    // stream владеет destStream, поэтому удаляем только stream
    delete stream;  // Это также удалит destStream
    delete seq;

    std::cout << std::endl;
}

// Тесты для Open/Close
void testStreamOpenClose() {
    std::cout << "--- Проверка Open/Close для потоков ---\n";

    int arr[] = {1, 2, 3};
    Sequence<int> *seq = new MutableArraySequence<int>(arr, 3);
    ReadOnlyStream<int> *readStream = new ReadOnlyStream<int>(seq);

    // Поток должен быть открыт автоматически при создании
    testing("ReadOnlyStream is opened after creation", !readStream->IsEndOfStream());

    readStream->Close();
    // После закрытия поток должен быть закрыт, но это не влияет на IsEndOfStream для Sequence
    readStream->Open();
    testing("ReadOnlyStream can be reopened", !readStream->IsEndOfStream());

    delete readStream;
    delete seq;

    Sequence<int> *writeSeq = new MutableArraySequence<int>();
    WriteOnlyStream<int> *writeStream = new WriteOnlyStream<int>(writeSeq);

    writeStream->Close();
    writeStream->Open();
    writeStream->Write(42);
    testing("WriteOnlyStream can write after reopen", writeSeq->getSize() == 1);

    delete writeStream;
    delete writeSeq;

    std::cout << std::endl;
}

// Тесты для граничных случаев
void testStreamEdgeCases() {
    std::cout << "--- Проверка граничных случаев ---\n";

    // Пустая последовательность
    Sequence<int> *emptySeq = new MutableArraySequence<int>();
    auto *emptyStream = new ReadOnlyStream<int>(emptySeq);

    testing("ReadOnlyStream from empty sequence IsEndOfStream", emptyStream->IsEndOfStream());

    bool exceptionThrown = false;
    try {
        emptyStream->Read();
    } catch (const EndOfStream &) {
        exceptionThrown = true;
    }
    testing("ReadOnlyStream from empty sequence throws EndOfStream", exceptionThrown);

    delete emptyStream;
    delete emptySeq;

    // Один элемент
    int arr[] = {42};
    Sequence<int> *singleSeq = new MutableArraySequence<int>(arr, 1);
    auto *singleStream = new ReadOnlyStream<int>(singleSeq);

    testing("ReadOnlyStream single element Read", singleStream->Read() == 42);
    testing("ReadOnlyStream single element IsEndOfStream", singleStream->IsEndOfStream());

    delete singleStream;
    delete singleSeq;

    // Seek за границы
    int arr2[] = {1, 2, 3};
    Sequence<int> *seq = new MutableArraySequence<int>(arr2, 3);
    auto *stream = new ReadOnlyStream<int>(seq);

    exceptionThrown = false;
    try {
        stream->Seek(10);
    } catch (const std::out_of_range &) {
        exceptionThrown = true;
    }
    testing("ReadOnlyStream Seek out of range throws", exceptionThrown);

    delete stream;
    delete seq;

    std::cout << std::endl;
}

// Тесты для IsCanGoBack
void testStreamCanGoBack() {
    std::cout << "--- Проверка IsCanGoBack для потоков ---\n";

    int arr[] = {1, 2, 3, 4, 5};
    Sequence<int> *seq = new MutableArraySequence<int>(arr, 5);
    auto *stream = new ReadOnlyStream<int>(seq);

    testing("ReadOnlyStream IsCanGoBack true", stream->IsCanGoBack());

    // Читаем несколько элементов
    stream->Read();
    stream->Read();
    stream->Read();

    // Возвращаемся назад через Seek
    stream->Seek(0);
    testing("ReadOnlyStream Seek back to 0", stream->GetPosition() == 0);
    testing("ReadOnlyStream Read after Seek back", stream->Read() == 1);

    delete stream;
    delete seq;

    std::cout << std::endl;
}

// Рекуррентное правило для Фибоначчи (для тестов)
int fibonacciRuleForStream(Sequence<int> *history) {
    int size = history->getSize();
    if (size < 2) {
        return 1;
    }
    return history->get(size - 1) + history->get(size - 2);
}

// Тесты для бесконечной LazySequence
void testReadOnlyStreamInfiniteLazySequence() {
    std::cout << "--- Проверка ReadOnlyStream с бесконечной LazySequence ---\n";

    // Создаем бесконечную последовательность Фибоначчи
    Sequence<int> *initial = new MutableArraySequence<int>();
    initial->inAppend(1);
    initial->inAppend(1);

    auto *fib = new LazySequence<int>(fibonacciRuleForStream, initial);
    auto *stream = new ReadOnlyStream<int>(fib);

    testing("ReadOnlyStream infinite LazySequence IsEndOfStream false", !stream->IsEndOfStream());
    testing("ReadOnlyStream infinite LazySequence Read(0)", stream->Read() == 1);
    testing("ReadOnlyStream infinite LazySequence Read(1)", stream->Read() == 1);
    testing("ReadOnlyStream infinite LazySequence Read(2)", stream->Read() == 2);
    testing("ReadOnlyStream infinite LazySequence Read(3)", stream->Read() == 3);
    testing("ReadOnlyStream infinite LazySequence Read(4)", stream->Read() == 5);
    testing("ReadOnlyStream infinite LazySequence IsEndOfStream still false", !stream->IsEndOfStream());

    delete stream;
    delete fib;
    delete initial;

    std::cout << std::endl;
}

// Главная функция тестирования
inline void mainStreamsTest() {
    testReadOnlyStreamFromSequence();
    testReadOnlyStreamFromLazySequence();
    testReadOnlyStreamSeek();
    testReadOnlyStreamFromFile();
    testReadOnlyStreamFromString();
    testWriteOnlyStreamToSequence();
    testWriteOnlyStreamToFile();
    testWriteOnlyStreamToStream();
    testStreamOpenClose();
    testStreamEdgeCases();
    testStreamCanGoBack();
    testReadOnlyStreamInfiniteLazySequence();
}

