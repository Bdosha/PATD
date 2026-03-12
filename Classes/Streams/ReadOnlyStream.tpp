#pragma once
#include "ReadOnlyStream.h"
#include "../Sequences/Array/ArraySequences.h"
#include <stdexcept>

template<class T>
ReadOnlyStream<T>::ReadOnlyStream(Sequence<T> *seq)
    : sourceType(StreamSourceType::SEQUENCE),
      position(0),
      isOpened(false),
      sequence(nullptr),
      sequenceSize(0),
      lazySequence(nullptr),
      lazySequenceSize(0),
      file(nullptr),
      deserializer(nullptr),
      ownsFile(false),
      stringData(nullptr),
      stringPosition(0),
      sourceStream(nullptr) {
    if (seq == nullptr) {
        throw std::invalid_argument("Sequence cannot be nullptr");
    }
    initFromSequence(seq);
}

template<class T>
ReadOnlyStream<T>::ReadOnlyStream(LazySequence<T> *lazySeq)
    : sourceType(StreamSourceType::LAZY_SEQUENCE),
      position(0),
      isOpened(false),
      sequence(nullptr),
      sequenceSize(0),
      lazySequence(nullptr),
      lazySequenceSize(0),
      file(nullptr),
      deserializer(nullptr),
      ownsFile(false),
      stringData(nullptr),
      stringPosition(0),
      sourceStream(nullptr) {
    if (lazySeq == nullptr) {
        throw std::invalid_argument("LazySequence cannot be nullptr");
    }
    initFromLazySequence(lazySeq);
}

template<class T>
ReadOnlyStream<T>::ReadOnlyStream(FILE *f, Deserializer<T> deser)
    : sourceType(StreamSourceType::FILE),
      position(0),
      isOpened(false),
      sequence(nullptr),
      sequenceSize(0),
      lazySequence(nullptr),
      lazySequenceSize(0),
      file(nullptr),
      deserializer(deser),
      ownsFile(false),
      stringData(nullptr),
      stringPosition(0),
      sourceStream(nullptr) {
    if (f == nullptr) {
        throw std::invalid_argument("File cannot be nullptr");
    }
    if (deser == nullptr) {
        throw std::invalid_argument("Deserializer cannot be nullptr");
    }
    initFromFile(f, deser);
}

template<class T>
ReadOnlyStream<T>::ReadOnlyStream(const std::string &str, Deserializer<T> deser)
    : sourceType(StreamSourceType::STRING),
      position(0),
      isOpened(false),
      sequence(nullptr),
      sequenceSize(0),
      lazySequence(nullptr),
      lazySequenceSize(0),
      file(nullptr),
      deserializer(deser),
      ownsFile(false),
      stringData(nullptr),
      stringPosition(0),
      sourceStream(nullptr) {
    if (deser == nullptr) {
        throw std::invalid_argument("Deserializer cannot be nullptr");
    }
    initFromString(str, deser);
}

template<class T>
ReadOnlyStream<T>::ReadOnlyStream(ReadOnlyStream<T> *stream)
    : sourceType(StreamSourceType::STREAM),
      position(0),
      isOpened(false),
      sequence(nullptr),
      sequenceSize(0),
      lazySequence(nullptr),
      lazySequenceSize(0),
      file(nullptr),
      deserializer(nullptr),
      ownsFile(false),
      stringData(nullptr),
      stringPosition(0),
      sourceStream(nullptr) {
    if (stream == nullptr) {
        throw std::invalid_argument("Stream cannot be nullptr");
    }
    initFromStream(stream);
}

template<class T>
ReadOnlyStream<T>::ReadOnlyStream(const ReadOnlyStream<T> &other)
    : sourceType(other.sourceType),
      position(other.position),
      isOpened(false),
      sequence(nullptr),
      sequenceSize(other.sequenceSize),
      lazySequence(nullptr),
      lazySequenceSize(other.lazySequenceSize),
      file(nullptr),
      deserializer(other.deserializer),
      ownsFile(false),
      stringData(nullptr),
      stringPosition(other.stringPosition),
      sourceStream(nullptr) {
    // Копируем данные в зависимости от типа источника
    switch (sourceType) {
        case StreamSourceType::SEQUENCE:
            if (other.sequence != nullptr) {
                // Создаем копию последовательности
                sequence = new MutableArraySequence<T>(other.sequence);
            }
            break;
        case StreamSourceType::LAZY_SEQUENCE:
            if (other.lazySequence != nullptr) {
                lazySequence = new LazySequence<T>(*other.lazySequence);
            }
            break;
        case StreamSourceType::FILE:
            // Для файла просто копируем указатель (не владеем файлом)
            file = other.file;
            break;
        case StreamSourceType::STRING:
            if (other.stringData != nullptr) {
                stringData = new std::string(*other.stringData);
                // Копируем позицию в строке
                stringPosition = other.stringPosition;
            }
            break;
        case StreamSourceType::STREAM:
            if (other.sourceStream != nullptr) {
                sourceStream = new ReadOnlyStream<T>(*other.sourceStream);
            }
            break;
    }
}

template<class T>
ReadOnlyStream<T>::~ReadOnlyStream() {
    try {
        cleanup();
    } catch (...) {
        // Игнорируем исключения в деструкторе для безопасности
    }
}

template<class T>
void ReadOnlyStream<T>::initFromSequence(Sequence<T> *seq) {
    sequence = seq;
    sequenceSize = seq->getSize();
    isOpened = true;
}

template<class T>
void ReadOnlyStream<T>::initFromLazySequence(LazySequence<T> *lazySeq) {
    lazySequence = lazySeq;
    // Для бесконечных последовательностей размер может быть бесконечным
    Cardinal length = lazySeq->GetLength();
    if (length.isInfinity()) {
        lazySequenceSize = SIZE_MAX;  // Максимальный размер
    } else {
        lazySequenceSize = length.getValue();
    }
    isOpened = true;
}

template<class T>
void ReadOnlyStream<T>::initFromFile(FILE *f, Deserializer<T> deser) {
    file = f;
    deserializer = deser;
    isOpened = true;
}

template<class T>
void ReadOnlyStream<T>::initFromString(const std::string &str, Deserializer<T> deser) {
    stringData = new std::string(str);
    deserializer = deser;
    stringPosition = 0;
    isOpened = true;
}

template<class T>
void ReadOnlyStream<T>::initFromStream(ReadOnlyStream<T> *stream) {
    sourceStream = stream;
    isOpened = true;
}

template<class T>
void ReadOnlyStream<T>::cleanup() {
    if (sourceType == StreamSourceType::SEQUENCE) {
        // Не удаляем sequence, т.к. он может использоваться снаружи
        sequence = nullptr;
    } else if (sourceType == StreamSourceType::LAZY_SEQUENCE) {
        // Не удаляем lazySequence, т.к. он может использоваться снаружи
        lazySequence = nullptr;
    } else if (sourceType == StreamSourceType::FILE) {
        // Не закрываем файл, т.к. он может использоваться снаружи
        file = nullptr;
    } else if (sourceType == StreamSourceType::STRING) {
        if (stringData != nullptr) {
            delete stringData;
            stringData = nullptr;
        }
    } else if (sourceType == StreamSourceType::STREAM) {
        if (sourceStream != nullptr) {
            delete sourceStream;
            sourceStream = nullptr;
        }
    }
}

template<class T>
bool ReadOnlyStream<T>::IsEndOfStream() const {
    if (!isOpened) {
        return true;
    }

    switch (sourceType) {
        case StreamSourceType::SEQUENCE:
            return position >= sequenceSize;
        case StreamSourceType::LAZY_SEQUENCE: {
            // Для бесконечных последовательностей никогда не достигнем конца
            if (lazySequenceSize == SIZE_MAX) {
                return false;
            }
            // Проверяем, достигли ли мы конца конечной последовательности
            if (position >= lazySequenceSize) {
                return true;
            }
            // Материализуем элементы до текущей позиции для проверки
            try {
                lazySequence->Get(position);
                return false;
            } catch (const std::out_of_range &) {
                return true;
            }
        }
        case StreamSourceType::FILE: {
            // Проверяем feof и текущую позицию в файле
            if (file == nullptr) {
                return true;
            }
            // Проверяем, достигли ли мы конца файла
            // feof устанавливается только после попытки чтения за пределами файла
            if (feof(file)) {
                return true;
            }
            // Дополнительная проверка: смотрим текущую позицию
            long currentPos = ftell(file);
            if (currentPos < 0) {
                return true;  // Ошибка получения позиции
            }
            fseek(file, 0, SEEK_END);
            long endPos = ftell(file);
            fseek(file, currentPos, SEEK_SET);
            // Если текущая позиция >= конца файла, значит достигли конца
            if (currentPos >= endPos) {
                return true;
            }
            return false;
        }
        case StreamSourceType::STRING:
            return stringPosition >= stringData->length();
        case StreamSourceType::STREAM:
            return sourceStream->IsEndOfStream();
        default:
            return true;
    }
}

template<class T>
T ReadOnlyStream<T>::Read() {
    if (!isOpened) {
        throw std::runtime_error("Stream is not opened");
    }

    if (IsEndOfStream()) {
        throw EndOfStream("End of stream reached");
    }

    T result;
    switch (sourceType) {
        case StreamSourceType::SEQUENCE:
            result = readFromSequence();
            break;
        case StreamSourceType::LAZY_SEQUENCE:
            result = readFromLazySequence();
            break;
        case StreamSourceType::FILE:
            result = readFromFile();
            break;
        case StreamSourceType::STRING:
            result = readFromString();
            break;
        case StreamSourceType::STREAM:
            result = readFromStream();
            break;
        default:
            throw std::runtime_error("Unknown source type");
    }

    position++;
    return result;
}

template<class T>
T ReadOnlyStream<T>::readFromSequence() {
    if (position >= sequenceSize) {
        throw EndOfStream("End of sequence reached");
    }
    return sequence->get(position);
}

template<class T>
T ReadOnlyStream<T>::readFromLazySequence() {
    try {
        return lazySequence->Get(position);
    } catch (const std::out_of_range &e) {
        throw EndOfStream("End of lazy sequence reached");
    }
}

template<class T>
T ReadOnlyStream<T>::readFromFile() {
    if (feof(file)) {
        throw EndOfStream("End of file reached");
    }

    // Читаем строку из файла (до символа новой строки или EOF)
    char buffer[4096];
    if (fgets(buffer, sizeof(buffer), file) == nullptr) {
        if (feof(file)) {
            throw EndOfStream("End of file reached");
        }
        throw std::runtime_error("Error reading from file");
    }

    // Удаляем символ новой строки если есть
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }

    std::string line(buffer);
    if (deserializer == nullptr) {
        throw std::runtime_error("Deserializer is null");
    }
    return deserializer(line);
}

template<class T>
T ReadOnlyStream<T>::readFromString() {
    if (stringPosition >= stringData->length()) {
        throw EndOfStream("End of string reached");
    }

    // Ищем следующую строку (до символа новой строки или конца строки)
    size_t startPos = stringPosition;
    size_t endPos = stringData->find('\n', startPos);
    if (endPos == std::string::npos) {
        endPos = stringData->length();
    }

    std::string line = stringData->substr(startPos, endPos - startPos);
    stringPosition = endPos;
    if (stringPosition < stringData->length() && (*stringData)[stringPosition] == '\n') {
        stringPosition++;  // Пропускаем символ новой строки
    }

    if (deserializer == nullptr) {
        throw std::runtime_error("Deserializer is null");
    }
    return deserializer(line);
}

template<class T>
T ReadOnlyStream<T>::readFromStream() {
    return sourceStream->Read();
}

template<class T>
size_t ReadOnlyStream<T>::GetPosition() const {
    return position;
}

template<class T>
bool ReadOnlyStream<T>::IsCanSeek() const {
    switch (sourceType) {
        case StreamSourceType::SEQUENCE:
        case StreamSourceType::LAZY_SEQUENCE:
            return true;  // Можно перемещаться по индексам
        case StreamSourceType::FILE:
            return true;  // Можно использовать fseek
        case StreamSourceType::STRING:
            return true;  // Можно перемещаться по позициям в строке
        case StreamSourceType::STREAM:
            return sourceStream->IsCanSeek();
        default:
            return false;
    }
}

template<class T>
size_t ReadOnlyStream<T>::Seek(size_t index) {
    if (!IsCanSeek()) {
        throw std::runtime_error("Seek is not supported for this stream type");
    }

    switch (sourceType) {
        case StreamSourceType::SEQUENCE:
            if (index > sequenceSize) {
                throw std::out_of_range("Seek index out of range");
            }
            position = index;
            return position;
        case StreamSourceType::LAZY_SEQUENCE: {
            // Для бесконечных последовательностей можно искать в любую позицию
            // Материализуем элементы до нужной позиции
            try {
                lazySequence->Get(index);
                position = index;
                return position;
            } catch (const std::out_of_range &) {
                throw std::out_of_range("Seek index out of range");
            }
        }
        case StreamSourceType::FILE: {
            // Для файла Seek сложнее - нужно перечитать все элементы до нужной позиции
            // Упрощенная реализация: возвращаемся в начало и читаем до нужной позиции
            rewind(file);
            position = 0;
            for (size_t i = 0; i < index; i++) {
                if (feof(file)) {
                    throw std::out_of_range("Seek index out of range");
                }
                char buffer[4096];
                if (fgets(buffer, sizeof(buffer), file) == nullptr) {
                    if (feof(file)) {
                        throw std::out_of_range("Seek index out of range");
                    }
                    throw std::runtime_error("Error seeking in file");
                }
                position++;
            }
            return position;
        }
        case StreamSourceType::STRING:
            if (index > stringData->length()) {
                throw std::out_of_range("Seek index out of range");
            }
            // Для строки Seek означает перемещение позиции чтения
            // Нужно пересчитать позицию в строке
            stringPosition = 0;
            for (size_t i = 0; i < index; i++) {
                size_t nextPos = stringData->find('\n', stringPosition);
                if (nextPos == std::string::npos) {
                    if (i < index - 1) {
                        throw std::out_of_range("Seek index out of range");
                    }
                    stringPosition = stringData->length();
                    break;
                }
                stringPosition = nextPos + 1;
            }
            position = index;
            return position;
        case StreamSourceType::STREAM:
            return sourceStream->Seek(index);
        default:
            throw std::runtime_error("Unknown source type");
    }
}

template<class T>
bool ReadOnlyStream<T>::IsCanGoBack() const {
    switch (sourceType) {
        case StreamSourceType::SEQUENCE:
        case StreamSourceType::LAZY_SEQUENCE:
            return true;  // Можно вернуться назад по индексам
        case StreamSourceType::FILE:
            return true;  // Можно использовать fseek с отрицательным смещением
        case StreamSourceType::STRING:
            return true;  // Можно вернуться назад по позициям
        case StreamSourceType::STREAM:
            return sourceStream->IsCanGoBack();
        default:
            return false;
    }
}

template<class T>
void ReadOnlyStream<T>::Open() {
    if (isOpened) {
        return;  // Уже открыт
    }

    switch (sourceType) {
        case StreamSourceType::SEQUENCE:
            if (sequence == nullptr) {
                throw std::runtime_error("Sequence is null");
            }
            break;
        case StreamSourceType::LAZY_SEQUENCE:
            if (lazySequence == nullptr) {
                throw std::runtime_error("LazySequence is null");
            }
            break;
        case StreamSourceType::FILE:
            if (file == nullptr) {
                throw std::runtime_error("File is null");
            }
            break;
        case StreamSourceType::STRING:
            if (stringData == nullptr) {
                throw std::runtime_error("String data is null");
            }
            break;
        case StreamSourceType::STREAM:
            if (sourceStream == nullptr) {
                throw std::runtime_error("Source stream is null");
            }
            sourceStream->Open();
            break;
    }

    isOpened = true;
}

template<class T>
void ReadOnlyStream<T>::Close() {
    if (!isOpened) {
        return;  // Уже закрыт
    }

    switch (sourceType) {
        case StreamSourceType::STREAM:
            if (sourceStream != nullptr) {
                sourceStream->Close();
            }
            break;
        default:
            // Для других типов просто сбрасываем флаг
            break;
    }

    isOpened = false;
}

