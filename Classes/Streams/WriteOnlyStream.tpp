#pragma once
#include "WriteOnlyStream.h"
#include "../Sequences/Array/ArraySequences.h"
#include <stdexcept>
#include <cstring>

template<class T>
WriteOnlyStream<T>::WriteOnlyStream(Sequence<T> *seq)
    : destinationType(StreamDestinationType::SEQUENCE),
      position(0),
      isOpened(false),
      sequence(nullptr),
      file(nullptr),
      serializer(nullptr),
      ownsFile(false),
      destinationStream(nullptr) {
    if (seq == nullptr) {
        throw std::invalid_argument("Sequence cannot be nullptr");
    }
    initToSequence(seq);
}

template<class T>
WriteOnlyStream<T>::WriteOnlyStream(FILE *f, Serializer<T> ser)
    : destinationType(StreamDestinationType::FILE),
      position(0),
      isOpened(false),
      sequence(nullptr),
      file(nullptr),
      serializer(ser),
      ownsFile(false),
      destinationStream(nullptr) {
    if (f == nullptr) {
        throw std::invalid_argument("File cannot be nullptr");
    }
    if (ser == nullptr) {
        throw std::invalid_argument("Serializer cannot be nullptr");
    }
    initToFile(f, ser);
}

template<class T>
WriteOnlyStream<T>::WriteOnlyStream(WriteOnlyStream<T> *stream)
    : destinationType(StreamDestinationType::STREAM),
      position(0),
      isOpened(false),
      sequence(nullptr),
      file(nullptr),
      serializer(nullptr),
      ownsFile(false),
      destinationStream(nullptr) {
    if (stream == nullptr) {
        throw std::invalid_argument("Stream cannot be nullptr");
    }
    initToStream(stream);
}

template<class T>
WriteOnlyStream<T>::WriteOnlyStream(const WriteOnlyStream<T> &other)
    : destinationType(other.destinationType),
      position(other.position),
      isOpened(false),
      sequence(nullptr),
      file(nullptr),
      serializer(other.serializer),
      ownsFile(false),
      destinationStream(nullptr) {
    // Копируем данные в зависимости от типа назначения
    switch (destinationType) {
        case StreamDestinationType::SEQUENCE:
            if (other.sequence != nullptr) {
                // Создаем копию последовательности
                sequence = new MutableArraySequence<T>(other.sequence);
            }
            break;
        case StreamDestinationType::FILE:
            // Для файла просто копируем указатель (не владеем файлом)
            file = other.file;
            break;
        case StreamDestinationType::STREAM:
            if (other.destinationStream != nullptr) {
                destinationStream = new WriteOnlyStream<T>(*other.destinationStream);
            }
            break;
    }
}

template<class T>
WriteOnlyStream<T>::~WriteOnlyStream() {
    try {
        cleanup();
    } catch (...) {
        // Игнорируем исключения в деструкторе для безопасности
    }
}

template<class T>
void WriteOnlyStream<T>::initToSequence(Sequence<T> *seq) {
    sequence = seq;
    position = seq->getSize();  // Начинаем с текущего размера последовательности
    isOpened = true;
}

template<class T>
void WriteOnlyStream<T>::initToFile(FILE *f, Serializer<T> ser) {
    file = f;
    serializer = ser;
    // Позиция файла определяется текущей позицией в файле
    position = ftell(f);
    if (position == -1) {
        position = 0;  // Если не можем определить позицию, начинаем с 0
    }
    isOpened = true;
}

template<class T>
void WriteOnlyStream<T>::initToStream(WriteOnlyStream<T> *stream) {
    destinationStream = stream;
    position = stream->GetPosition();
    isOpened = true;
}

template<class T>
void WriteOnlyStream<T>::cleanup() {
    if (destinationType == StreamDestinationType::SEQUENCE) {
        // Не удаляем sequence, т.к. он может использоваться снаружи
        sequence = nullptr;
    } else if (destinationType == StreamDestinationType::FILE) {
        // Не закрываем файл, т.к. он может использоваться снаружи
        file = nullptr;
    } else if (destinationType == StreamDestinationType::STREAM) {
        if (destinationStream != nullptr) {
            delete destinationStream;
            destinationStream = nullptr;
        }
    }
}

template<class T>
size_t WriteOnlyStream<T>::GetPosition() const {
    return position;
}

template<class T>
size_t WriteOnlyStream<T>::Write(const T &item) {
    if (!isOpened) {
        throw std::runtime_error("Stream is not opened");
    }

    switch (destinationType) {
        case StreamDestinationType::SEQUENCE:
            writeToSequence(item);
            break;
        case StreamDestinationType::FILE:
            writeToFile(item);
            break;
        case StreamDestinationType::STREAM:
            writeToStream(item);
            break;
        default:
            throw std::runtime_error("Unknown destination type");
    }

    position++;
    return position;
}

template<class T>
void WriteOnlyStream<T>::writeToSequence(const T &item) {
    if (sequence == nullptr) {
        throw std::runtime_error("Sequence is null");
    }
    sequence->inAppend(item);
}

template<class T>
void WriteOnlyStream<T>::writeToFile(const T &item) {
    if (file == nullptr) {
        throw std::runtime_error("File is null");
    }
    if (serializer == nullptr) {
        throw std::runtime_error("Serializer is null");
    }

    std::string serialized = serializer(item);
    if (fputs(serialized.c_str(), file) == EOF) {
        throw std::runtime_error("Error writing to file");
    }
    // Добавляем символ новой строки для разделения элементов
    if (fputc('\n', file) == EOF) {
        throw std::runtime_error("Error writing newline to file");
    }
}

template<class T>
void WriteOnlyStream<T>::writeToStream(const T &item) {
    if (destinationStream == nullptr) {
        throw std::runtime_error("Destination stream is null");
    }
    destinationStream->Write(item);
}

template<class T>
void WriteOnlyStream<T>::Open() {
    if (isOpened) {
        return;  // Уже открыт
    }

    switch (destinationType) {
        case StreamDestinationType::SEQUENCE:
            if (sequence == nullptr) {
                throw std::runtime_error("Sequence is null");
            }
            break;
        case StreamDestinationType::FILE:
            if (file == nullptr) {
                throw std::runtime_error("File is null");
            }
            break;
        case StreamDestinationType::STREAM:
            if (destinationStream == nullptr) {
                throw std::runtime_error("Destination stream is null");
            }
            destinationStream->Open();
            break;
    }

    isOpened = true;
}

template<class T>
void WriteOnlyStream<T>::Close() {
    if (!isOpened) {
        return;  // Уже закрыт
    }

    switch (destinationType) {
        case StreamDestinationType::STREAM:
            if (destinationStream != nullptr) {
                destinationStream->Close();
            }
            break;
        default:
            // Для других типов просто сбрасываем флаг
            break;
    }

    isOpened = false;
}

